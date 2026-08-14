#include "graphics_system.h"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <mutex>
#include <set>
#include <tuple>
#include <unordered_set>
#include <utility>

#include <fmt/format.h>
#include <zstd.h>
#include <smolv.h>
#include <spirv/unified1/spirv.hpp11>
#include <xxhash.h>

#include <rex/logging.h>
#include <rex/cvar.h>
#include <rex/chrono/clock.h>
#include <rex/dbg.h>
#include <rex/diagnostics/gta4_transition.h>
#include <rex/filesystem.h>
#include <rex/graphics/primitive_restart.h>
#include <rex/graphics/gta4_native/pipeline_cache_file.h>
#include <rex/graphics/gta4_native/surface_view.h>
#include <rex/graphics/util/draw.h>
#include <rex/graphics/pipeline/texture/conversion.h>
#include <rex/graphics/pipeline/texture/util.h>
#include <rex/math.h>
#include <rex/memory.h>
#include <rex/system/function_dispatcher.h>
#include <rex/ui/vulkan/presenter.h>
#include <rex/ui/image_decode.h>
#include <rex/ui/vulkan/provider.h>
#include <rex/ui/vulkan/submission_tracker.h>
#include <rex/ui/vulkan/util.h>
#include <rex/ui/windowed_app_context.h>

#include <shader/shader_cache.h>
#include <shader_overrides/shader_override_cache.h>

#include "../shaders/vulkan_spirv/fullscreen_cw_vs.h"
#include "hdr_present_ps.h"
#include "reflection_mip_filter_ps.h"
#include "resolve_convert_msaa_ps.h"
#include "resolve_convert_ps.h"
#include "resolve_depth_msaa_ps.h"

#ifndef GTA4_NATIVE_FONT_ASSET_ROOT
#define GTA4_NATIVE_FONT_ASSET_ROOT ""
#endif

REXCVAR_DEFINE_BOOL(gta4_native_vector_fonts, true, "GTA IV/Graphics/Text",
                    "Replace stock compressed font atlases with licensed high-resolution atlases")
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
REXCVAR_DEFINE_BOOL(gta4_trace_vector_fonts, true, "GTA IV/Diagnostics",
                    "Log the guest-to-Vulkan vector-font data path with bounded draw details");
REXCVAR_DEFINE_BOOL(gta4_native_spatial_aa, true, "GTA IV/Graphics/Anti-Aliasing",
                    "Legacy compatibility toggle for the native spatial edge resolve");
REXCVAR_DEFINE_BOOL(gta4_native_output_dither, true, "GTA IV/Graphics/Post-Processing",
                    "Apply stable display-space dithering to reduce output banding");
REXCVAR_DEFINE_STRING(gta4_texture_filtering, "original",
                      "GTA IV/Graphics/Texture Filtering",
                      "Material texture filtering: original, bilinear, or trilinear")
    .allowed({"original", "bilinear", "trilinear"});
REXCVAR_DEFINE_STRING(gta4_anisotropic_filtering, "original",
                      "GTA IV/Graphics/Texture Filtering",
                      "Material anisotropic filtering: original, off, 2x, 4x, 8x, or 16x")
    .allowed({"original", "off", "2x", "4x", "8x", "16x"});
REXCVAR_DEFINE_BOOL(gta4_trace_startup_content, false, "GTA IV/Diagnostics",
                    "Probe every rendered startup frame through the legal and loading screens");
REXCVAR_DEFINE_BOOL(
    gta4_trace_native_renderer, true, "GTA IV/Diagnostics",
    "Emit deterministic frame-correlated native renderer state, resource, draw, resolve, and "
    "presentation diagnostics");
REXCVAR_DEFINE_UINT32(
    gta4_trace_native_interval, 120, "GTA IV/Diagnostics",
    "Trace every Nth submitted native-renderer frame (startup frames are always traced)")
    .range(1, 3600);
REXCVAR_DEFINE_UINT32(
    gta4_trace_native_command_limit, 4096, "GTA IV/Diagnostics",
    "Maximum command index included in a periodically sampled native-renderer frame trace")
    .range(1, 65536);
REXCVAR_DEFINE_BOOL(
    gta4_profile_native_renderer, false, "GTA IV/Diagnostics",
    "Capture asynchronous CPU and Vulkan GPU timings for the native renderer");
REXCVAR_DEFINE_UINT32(
    gta4_profile_native_interval, 120, "GTA IV/Diagnostics",
    "Capture one native-renderer profile after this many submitted frames")
    .range(1, 3600);
REXCVAR_DEFINE_STRING(
    gta4_profile_native_detail, "phase", "GTA IV/Diagnostics",
    "Native-renderer GPU profile detail: phase or command")
    .allowed({"phase", "command"});
REXCVAR_DEFINE_UINT32(
    gta4_profile_native_top, 20, "GTA IV/Diagnostics",
    "Number of ranked GPU, CPU, and shader hotspots emitted per captured frame")
    .range(1, 100);

namespace rex::graphics::gta4_native {

namespace {

namespace transition = rex::diagnostics::gta4_transition;

template <typename Callable>
class ScopeExit {
 public:
  explicit ScopeExit(Callable&& callable) : callable_(std::move(callable)) {}
  ScopeExit(const ScopeExit&) = delete;
  ScopeExit& operator=(const ScopeExit&) = delete;
  ~ScopeExit() { callable_(); }

 private:
  Callable callable_;
};

template <typename Callable>
ScopeExit<Callable> MakeScopeExit(Callable&& callable) {
  return ScopeExit<Callable>(std::forward<Callable>(callable));
}

uint32_t GetNativePresentationAABits() {
  const std::string mode = rex::cvar::Query<std::string>("gta4_native_anti_aliasing");
  if (mode == "fxaa") {
    return 16u;
  }
  if (mode == "spatial") {
    return 2u;
  }
  if (mode == "off") {
    return 0u;
  }
  if (mode == "smaa") {
    return 0u;
  }
  return REXCVAR_GET(gta4_native_spatial_aa) ? 2u : 0u;
}

bool IsNativeSmaaEnabled() {
  return rex::cvar::Query<std::string>("gta4_native_anti_aliasing") == "smaa";
}

constexpr uint32_t kMaximumUpPayloadSize = 0x400000;
constexpr uint32_t kMaximumResourcePayloadSize = 0x4000000;
constexpr uint32_t kDefaultOutputWidth = 1280;
constexpr uint32_t kDefaultOutputHeight = 720;
constexpr uint32_t kStockFontAtlasExtent = 512;
// The host BC3 upload contains one logical 512x512 top level before any
// backend padding. This prefix is stable across address changes and identifies
// the episode-specific stock atlas that the game actually loaded.
constexpr size_t kStockFontIdentityPayloadSize = 262144;
constexpr uint32_t kVectorFontAtlasExtent = 2048;
constexpr uint32_t kVectorFontAtlasExtentMinusOne = 2047;
// The replacement atlas is four times the stock extent. Keep enough
// prefiltered coverage levels to reach the stock resolution and to handle the
// smaller HUD/subtitle draws without undersampling the full-resolution alpha.
constexpr uint32_t kVectorFontMipLevelCount = 5;
constexpr uint32_t kCompletedFrameOffset = 16552;
constexpr size_t kMaximumQueuedCommands = 65536;
constexpr size_t kIndexedFrameDetailedCommandLimit = 256;
constexpr size_t kIndexedFrameProgressInterval = 128;
constexpr uint32_t kSpirvMagic = 0x07230203;
constexpr VkDeviceSize kNativeUploadBufferSize = 0x4000000;
constexpr VkDeviceSize kNativeUploadGrowthGranularity = 0x1000000;
constexpr uint32_t kContentProbeAxis = 16;
constexpr uint32_t kContentProbeSampleCount = 256;
constexpr VkDeviceSize kContentProbeSampleStride = 16;
constexpr VkDeviceSize kContentProbeStageStride = 4096;
constexpr uint32_t kContentProbeMaximumStages = 128;
constexpr uint32_t kSceneWriteCheckpointMaximumStages = 48;
constexpr VkDeviceSize kContentProbeBufferSize =
    VkDeviceSize(kContentProbeMaximumStages) * kContentProbeStageStride;
// Linear-light value that rounds to half of the first nonzero 8-bit sRGB code.
// Values below this are presentation dither/noise, not a visible output pixel.
constexpr float kContentProbeVisibleLinearThreshold = 0.000151763492f;
constexpr float kContentProbeOneCodeValue = 0.003921568627f;
constexpr float kContentProbeDarkThreshold = 0.015625000000f;
constexpr float kContentProbeDimThreshold = 0.062500000000f;
constexpr float kContentProbeBrightThreshold = 0.250000000000f;
constexpr size_t kContentProbeP10Index = 25;
constexpr size_t kContentProbeP50Index = 127;
constexpr size_t kContentProbeP90Index = 229;
constexpr size_t kContentProbeP99Index = 252;
constexpr uint32_t kStartupContentProbeFrameLimit = 1200;
constexpr uint32_t kVertexConstantsOffset = 0x780;
constexpr uint32_t kVertexConstantsSize = 0x1000;
constexpr uint32_t kPixelConstantsOffset = 0x1780;
constexpr size_t kDofProjectionSnapshotOffset = 0x2490;
constexpr size_t kDofDistanceSnapshotOffset = 0x24A0;
constexpr size_t kDofBlurSnapshotOffset = 0x24B0;
constexpr uint32_t kSplitPostFxCombinedDescriptorCount = 16;
constexpr uint32_t kSplitPostFxDescriptorSetCount = 4;
constexpr uint32_t kSunShaftCombinedDescriptorCount = 12;
constexpr uint32_t kSunShaftDescriptorSetCount = 4;
constexpr uint32_t kPixelConstantsSize = 0xE00;
constexpr uint32_t kDescriptorSetCount = 6;
constexpr uint32_t kShaderTextureCount = kTextureStageCount;
constexpr uint32_t kDrawDescriptorSetCount = 5;
constexpr uint32_t kTextureFetchBase = 0x480;
constexpr uint32_t kTextureFetchSize = 0x18;
constexpr uint32_t kTextureHandleBase = 0x30F8;
constexpr uint32_t kStreamBufferBase = 12452;
constexpr uint32_t kIndexBufferOffset = 12428;
constexpr uint32_t kRenderTargetOffset = 12432;
constexpr uint32_t kDepthStencilOffset = 12448;
constexpr uint32_t kResourceDataOffset = 24;
constexpr uint32_t kResourceSizeOffset = 28;
constexpr uint32_t kResourceAddressMask = 0xFFFFFFFC;
constexpr uint32_t kResourceSizeMask = 0x0FFFFFFF;
constexpr uint32_t kIndex32Flag = 0x80000000;
constexpr uint32_t kD3dFmtA8R8G8B8 = 0x18280186;
constexpr uint32_t kD3dFmtA16B16G16R16F2 = 0x1A2201BF;
constexpr uint32_t kD3dFmtD24FS8 = 0x1A220197;
constexpr uint32_t kD3dFmtR32F = 0x2DA2ABA4;
constexpr uint32_t kD3dFmtG16R16F = 0x2D22AB9F;
constexpr uint32_t kD3dFmtG16R16F2 = 0x2D20AB8D;
constexpr uint32_t kAlphaReferenceOffset = 10500;
constexpr uint32_t kBlendConstantsOffset = 10464;
constexpr uint32_t kHardwareBlendPackedOffset = 10552;
constexpr uint32_t kDepthStencilPackedOffset = 10548;
constexpr uint32_t kAlphaTestFunctionPackedOffset = 10556;
constexpr uint32_t kCullModePackedOffset = 10568;
constexpr uint32_t kDepthBiasOffset = 10832;
constexpr uint32_t kSlopeScaledDepthBiasOffset = 10836;
constexpr uint32_t kBackDepthBiasOffset = 10840;
constexpr uint32_t kBackSlopeScaledDepthBiasOffset = 10844;
constexpr uint32_t kBlendControlOffset = 11844;
constexpr uint32_t kScissorEnableOffset = 11848;
constexpr uint32_t kColorWriteMaskOffset = 11852;
constexpr uint32_t kDepthEnableOffset = 11868;
constexpr uint32_t kStencilEnableOffset = 11872;
constexpr uint32_t kStencilReferenceOffset = 10499;
constexpr uint32_t kStencilMaskOffset = 10498;
constexpr uint32_t kStencilWriteMaskOffset = 10497;
constexpr uint32_t kBackStencilReferenceOffset = 10495;
constexpr uint32_t kBackStencilMaskOffset = 10494;
constexpr uint32_t kBackStencilWriteMaskOffset = 10493;
constexpr uint32_t kViewportXOffset = 12640;
constexpr uint32_t kViewportYOffset = 12644;
constexpr uint32_t kViewportWidthOffset = 12648;
constexpr uint32_t kViewportHeightOffset = 12652;
constexpr uint32_t kViewportMinDepthOffset = 12656;
constexpr uint32_t kViewportMaxDepthOffset = 12660;
constexpr uint32_t kScissorLeftOffset = 12668;
constexpr uint32_t kScissorTopOffset = 12672;
constexpr uint32_t kScissorRightOffset = 12676;
constexpr uint32_t kScissorBottomOffset = 12680;

constexpr uint32_t kSpecConstantAlphaTestEnable = 0x00000002;
constexpr uint32_t kSpecConstantAlphaTestFunctionMask = 0x00000700;
constexpr uint32_t kSpecConstantAlphaTestCapabilityMask =
    kSpecConstantAlphaTestEnable | kSpecConstantAlphaTestFunctionMask;
constexpr uint32_t kSpecConstantAlphaTestFunctionValueMask = 0x7;
constexpr uint32_t kSpecConstantAlphaTestFunctionShift = 8;
static_assert(kSpecConstantAlphaTestCapabilityMask == 0x00000702);
constexpr uint32_t kDefaultVertexBinding = 15;
constexpr VkDeviceSize kDefaultVertexDataSize = 16;
// Driver pipeline caches are normally small. Bound untrusted on-disk input so
// a corrupt cache cannot force an arbitrarily large allocation at startup.
constexpr uint64_t kMaximumNativePipelineCacheFileSize = 256ull * 1024ull * 1024ull;

bool IsAlphaCoverageRequested(uint32_t alpha_test_enable, uint32_t alpha_function) {
  return alpha_test_enable != 0 &&
         alpha_function != uint32_t(xenos::CompareFunction::kAlways);
}

bool HasAlphaTestCapability(uint32_t specialization_constants_mask) {
  return (specialization_constants_mask & kSpecConstantAlphaTestCapabilityMask) ==
         kSpecConstantAlphaTestCapabilityMask;
}

uint32_t PackAlphaTestSpecialization(uint32_t alpha_function) {
  return kSpecConstantAlphaTestEnable |
         ((alpha_function & kSpecConstantAlphaTestFunctionValueMask)
          << kSpecConstantAlphaTestFunctionShift);
}

enum class EarlyFragmentTestsStatus : uint8_t {
  kInvalid,
  kAbsent,
  kPresent,
};

EarlyFragmentTestsStatus InspectEarlyFragmentTests(const std::vector<uint32_t>& spirv) {
  constexpr size_t kSpirvHeaderWordCount = 5;
  if (spirv.size() < kSpirvHeaderWordCount || spirv[0] != kSpirvMagic) {
    return EarlyFragmentTestsStatus::kInvalid;
  }

  size_t cursor = kSpirvHeaderWordCount;
  while (cursor < spirv.size()) {
    const uint32_t instruction = spirv[cursor];
    const uint32_t word_count = instruction >> 16;
    const spv::Op opcode = spv::Op(instruction & 0xFFFFu);
    if (!word_count || word_count > spirv.size() - cursor) {
      return EarlyFragmentTestsStatus::kInvalid;
    }
    const uint32_t* words = spirv.data() + cursor;
    if (opcode == spv::Op::OpExecutionMode && word_count >= 3 &&
        words[2] == uint32_t(spv::ExecutionMode::EarlyFragmentTests)) {
      return EarlyFragmentTestsStatus::kPresent;
    }
    cursor += word_count;
  }
  return EarlyFragmentTestsStatus::kAbsent;
}

struct VectorFontAtlas {
  const char* filename = nullptr;
  std::vector<uint8_t> alpha;
};

enum class VectorFontSet : size_t {
  kGta4 = 0,
  kTlad = 1,
  kTbogt = 2,
};

constexpr size_t kVectorFontSetCount = 3;
constexpr size_t kVectorFontAtlasCount = 3;

const char* VectorFontSetName(VectorFontSet set) {
  switch (set) {
    case VectorFontSet::kGta4:
      return "gta4";
    case VectorFontSet::kTlad:
      return "tlad";
    case VectorFontSet::kTbogt:
      return "tbogt";
  }
  return "unknown";
}

std::optional<std::vector<uint8_t>> ReadBinaryFile(const std::filesystem::path& path) {
  std::ifstream stream(path, std::ios::binary | std::ios::ate);
  if (!stream) {
    return std::nullopt;
  }
  const auto end = stream.tellg();
  if (end <= 0 || static_cast<uint64_t>(end) > std::numeric_limits<size_t>::max() ||
      static_cast<uint64_t>(end) > kMaximumNativePipelineCacheFileSize) {
    return std::nullopt;
  }
  std::vector<uint8_t> result(static_cast<size_t>(end));
  stream.seekg(0, std::ios::beg);
  if (!stream.read(reinterpret_cast<char*>(result.data()), result.size())) {
    return std::nullopt;
  }
  return result;
}

std::filesystem::path FindVectorFontAsset(const char* filename) {
  std::error_code error;
  const auto bundled = rex::filesystem::GetExecutableFolder().parent_path() / "Resources" /
                       "font_atlases" / filename;
  if (std::filesystem::is_regular_file(bundled, error)) {
    return bundled;
  }
  const auto source = std::filesystem::path(GTA4_NATIVE_FONT_ASSET_ROOT) / filename;
  error.clear();
  return std::filesystem::is_regular_file(source, error) ? source : std::filesystem::path{};
}

const VectorFontAtlas* FindVectorFontAtlas(VectorFontSet set, size_t atlas_index) {
  static std::array<std::array<VectorFontAtlas, kVectorFontAtlasCount>,
                    kVectorFontSetCount>
      atlases = {{{{{"font1.png", {}}, {"font2.png", {}}, {"font3.png", {}}}},
                  {{{"tlad/font1.png", {}},
                    {"tlad/font2.png", {}},
                    {"tlad/font3.png", {}}}},
                  {{{"tbogt/font1.png", {}},
                    {"tbogt/font2.png", {}},
                    {"tbogt/font3.png", {}}}}}};
  static std::once_flag load_once;
  std::call_once(load_once, []() {
    for (auto& set_atlases : atlases) {
      for (auto& atlas : set_atlases) {
        const auto path = FindVectorFontAsset(atlas.filename);
        const auto encoded = path.empty() ? std::nullopt : ReadBinaryFile(path);
        int width = 0;
        int height = 0;
        const auto rgba =
            encoded ? rex::ui::DecodeImageRGBA(encoded->data(), encoded->size(), width, height)
                    : std::vector<uint8_t>{};
        if (rgba.empty() || width != int(kVectorFontAtlasExtent) ||
            height != int(kVectorFontAtlasExtent)) {
          REXLOG_ERROR("gta4-native-fonts: failed to load {} as a {}x{} RGBA image",
                       path.string(), kVectorFontAtlasExtent, kVectorFontAtlasExtent);
          continue;
        }
        atlas.alpha.resize(size_t(width) * size_t(height));
        for (size_t pixel = 0; pixel < atlas.alpha.size(); ++pixel) {
          atlas.alpha[pixel] = rgba[pixel * 4 + 3];
        }
        REXLOG_INFO(
            "gta4-native-font-debug: atlas-load file={} path={} encoded-bytes={} "
            "encoded-hash={:016X} rgba-hash={:016X} alpha-bytes={} alpha-hash={:016X} "
            "alpha-corners={:02X},{:02X},{:02X},{:02X}",
            atlas.filename, path.string(), encoded->size(),
            XXH3_64bits(encoded->data(), encoded->size()), XXH3_64bits(rgba.data(), rgba.size()),
            atlas.alpha.size(), XXH3_64bits(atlas.alpha.data(), atlas.alpha.size()),
            atlas.alpha.front(), atlas.alpha[kVectorFontAtlasExtentMinusOne],
            atlas.alpha[atlas.alpha.size() - kVectorFontAtlasExtent], atlas.alpha.back());
      }
    }
  });

  const size_t set_index = static_cast<size_t>(set);
  return set_index < atlases.size() && atlas_index < atlases[set_index].size() &&
                 !atlases[set_index][atlas_index].alpha.empty()
             ? &atlases[set_index][atlas_index]
             : nullptr;
}

VectorFontSet SelectVectorFontSet(size_t atlas_index, uint64_t stock_identity_hash) {
  // font1's alpha is byte-identical in GTA IV, TLAD, and TBoGT. TLAD also
  // reuses GTA IV's streamed font2. Only select an episode atlas where the
  // installed stock texture itself proves the distinction.
  // These are the XXH3 hashes of the renderer's stock-identity payload. The
  // values are generated from the installed XTDs with the same leading
  // alignment block and linear BC3 layout produced by CaptureTextureResource.
  constexpr uint64_t kTbogtFont2Hash = 0xD319ABCEFD23508Dull;
  constexpr uint64_t kTladFont3Hash = 0x57551F2730323DF3ull;
  constexpr uint64_t kTbogtFont3Hash = 0x6C569517F27F53D2ull;
  if (atlas_index == 1 && stock_identity_hash == kTbogtFont2Hash) {
    return VectorFontSet::kTbogt;
  }
  if (atlas_index == 2) {
    if (stock_identity_hash == kTladFont3Hash) {
      return VectorFontSet::kTlad;
    }
    if (stock_identity_hash == kTbogtFont3Hash) {
      return VectorFontSet::kTbogt;
    }
  }
  return VectorFontSet::kGta4;
}

const char* CommandTypeName(CommandType type) {
  switch (type) {
    case CommandType::kDeviceCreated:
      return "device-created";
    case CommandType::kDeviceDestroyed:
      return "device-destroyed";
    case CommandType::kRegisterShader:
      return "register-shader";
    case CommandType::kRegisterVertexDeclaration:
      return "register-vdecl";
    case CommandType::kSetRenderState:
      return "set-render-state";
    case CommandType::kSetPixelShader:
      return "set-ps";
    case CommandType::kSetVertexShader:
      return "set-vs";
    case CommandType::kSetVertexDeclaration:
      return "set-vdecl";
    case CommandType::kSetTexture:
      return "set-texture";
    case CommandType::kInvalidateTexture:
      return "invalidate-texture";
    case CommandType::kSetDepthStencil:
      return "set-depth";
    case CommandType::kSetRenderTarget:
      return "set-rt";
    case CommandType::kSetVertexStream:
      return "set-stream";
    case CommandType::kSetIndexBuffer:
      return "set-index";
    case CommandType::kDrawPrimitive:
      return "draw";
    case CommandType::kDrawPrimitiveUp:
      return "draw-up";
    case CommandType::kDrawIndexedPrimitive:
      return "draw-indexed";
    case CommandType::kResolve:
      return "resolve";
    case CommandType::kTextureLock:
      return "texture-lock";
    case CommandType::kClear:
      return "clear";
    case CommandType::kRenderPhaseMarker:
      return "render-phase-marker";
    case CommandType::kPresent:
      return "present";
    case CommandType::kQueryDeviceCapabilities:
      return "query-device-capabilities";
    case CommandType::kRegisterReflectionTarget:
      return "register-reflection-target";
    case CommandType::kReleaseResource:
      return "release-resource";
    case CommandType::kUpdateEnvironmentalData:
      return "update-environmental-data";
    case CommandType::kDepthSurfaceHandoff:
      return "depth-surface-handoff";
  }
  return "unknown";
}

bool IsProfiledNativeGpuCommand(CommandType type) {
  return type == CommandType::kDrawPrimitive ||
         type == CommandType::kDrawPrimitiveUp ||
         type == CommandType::kDrawIndexedPrimitive ||
         type == CommandType::kResolve || type == CommandType::kClear ||
         type == CommandType::kDepthSurfaceHandoff;
}

constexpr uint64_t NativeTimestampMask(uint32_t valid_bits) {
  return valid_bits >= 64 ? UINT64_MAX
                          : (uint64_t(1) << valid_bits) - 1;
}

constexpr uint64_t NativeTimestampDelta(uint64_t begin, uint64_t end,
                                        uint32_t valid_bits) {
  const uint64_t mask = NativeTimestampMask(valid_bits);
  return ((end & mask) - (begin & mask)) & mask;
}

static_assert(NativeTimestampDelta(0xFFFFFFF8u, 0x5u, 32) == 13u);
static_assert(
    NativeTimestampDelta(0xFFFFFFFFFFFFFFF8ull, 0x5u, 64) == 13u);

const char* RenderPhaseName(RenderPhase phase) {
  switch (phase) {
    case RenderPhase::kUnknown:
      return "unknown";
    case RenderPhase::kSceneToGBuffer:
      return "scene-to-gbuffer";
    case RenderPhase::kLightsToScreen:
      return "lights-to-screen";
    case RenderPhase::kLightSetup:
      return "light-setup";
    case RenderPhase::kLightDraw:
      return "light-draw";
    case RenderPhase::kRadarMap:
      return "radar-map";
    case RenderPhase::kCompositePostFx:
      return "composite-postfx";
  }
  return "invalid";
}

int32_t ScaleCoordinateFloor(int32_t value, uint32_t logical_extent, uint32_t physical_extent) {
  if (value <= 0 || !logical_extent || logical_extent == physical_extent) {
    return value;
  }
  return int32_t((uint64_t(uint32_t(value)) * physical_extent) / logical_extent);
}

int32_t ScaleCoordinateCeil(int32_t value, uint32_t logical_extent,
                            uint32_t physical_extent) {
  if (value <= 0 || !logical_extent || logical_extent == physical_extent) {
    return value;
  }
  const uint64_t numerator = uint64_t(uint32_t(value)) * physical_extent + logical_extent - 1;
  return int32_t(numerator / logical_extent);
}

bool ShouldLogDiagnosticFrame(uint32_t submitted_frame) {
  if (!submitted_frame) {
    return false;
  }
  if (REXCVAR_GET(gta4_trace_startup_content)) {
    return submitted_frame <= 16 || !(submitted_frame % 120);
  }
  if (!REXCVAR_GET(gta4_trace_native_renderer)) {
    return false;
  }
  const uint32_t interval = std::max(1u, REXCVAR_GET(gta4_trace_native_interval));
  return submitted_frame <= 4 || !(submitted_frame % interval);
}

bool ShouldProbeContentFrame(uint32_t submitted_frame) {
  if (!submitted_frame) {
    return false;
  }
  if (REXCVAR_GET(gta4_trace_startup_content) &&
      (submitted_frame <= 16 || !(submitted_frame % 120) ||
       submitted_frame <= kStartupContentProbeFrameLimit)) {
    return true;
  }
  if (!REXCVAR_GET(gta4_trace_native_renderer)) {
    return false;
  }
  const uint32_t interval = std::max(1u, REXCVAR_GET(gta4_trace_native_interval));
  return submitted_frame <= 4 || !(submitted_frame % interval);
}

std::string_view ClassifyTranslucentDiagnosticShader(std::string_view filename) {
  if (filename.find("/watertex/") != std::string_view::npos) {
    return "water-texture";
  }
  if (filename.find("/water/") != std::string_view::npos ||
      filename.find("water_e") != std::string_view::npos) {
    return "water-surface";
  }
  if (filename.find("/gta_vehicle_vehglass/") != std::string_view::npos) {
    return "vehicle-glass";
  }
  if (filename.find("/gta_glass") != std::string_view::npos) {
    return "glass";
  }
  if (filename.find("/gta_normal_reflect_alpha/") != std::string_view::npos) {
    return "alpha-reflect";
  }
  if (filename.find("/gta_rmptfx_litsprite/") != std::string_view::npos) {
    return "light-sprite";
  }
  if (filename.find("/gta_vehicle_lightsemissive/") != std::string_view::npos) {
    return "vehicle-light";
  }
  if (filename.find("/gta_emissive") != std::string_view::npos) {
    return "emissive";
  }
  if (filename.find("/deferred_lighting/") != std::string_view::npos) {
    return "deferred-light";
  }
  return {};
}

uint32_t TranslucentDeepCaptureLimit(std::string_view category) {
  if (category == "water-surface") {
    return 8;
  }
  if (category == "water-texture" || category == "deferred-light") {
    return 2;
  }
  return category.empty() ? 0 : 4;
}

struct NativeSharedConstants {
  uint32_t texture_2d_indices[kShaderTextureCount]{};
  uint32_t texture_2d_array_indices[kShaderTextureCount]{};
  uint32_t texture_3d_indices[kShaderTextureCount]{};
  uint32_t texture_cube_indices[kShaderTextureCount]{};
  uint32_t sampler_indices[kShaderTextureCount]{};
  // Five 26-word descriptor arrays occupy 130 words. Pad the descriptor block to the next
  // 16-byte constant-buffer register so the raw SPIR-V/AIR loads and HLSL packoffset layout are
  // identical.
  uint32_t descriptor_padding[2]{};
  uint32_t booleans = 0;
  uint32_t swapped_texcoords = 0;
  uint32_t swapped_normals = 0;
  uint32_t swapped_binormals = 0;
  uint32_t swapped_tangents = 0;
  uint32_t swapped_blend_weights = 0;
  float half_pixel_offset_x = 0.0f;
  float half_pixel_offset_y = 0.0f;
  float clip_plane[4]{};
  uint8_t clip_plane_enabled = 0;
  uint8_t clip_plane_padding[3]{};
  float alpha_threshold = 0.0f;
  uint32_t conditional_survey_index = 0;
  uint32_t conditional_rendering_index = 0;
  // GTA IV authored its directional motion blur around a 30 Hz frame. Exact
  // composite shader overrides consume this correction from the next complete
  // constant-buffer register; stock shaders never address it.
  float motion_blur_time_scale = 1.0f;
  float motion_blur_padding[3]{};
  // Fusion-compatible exponential-height fog parameters followed by the
  // camera, projection scale, and guest row-major inverse-view matrix. Shader overrides use
  // the validity mask before consuming any captured guest state.
  float fog_parameters[4]{};
  float camera_position[4]{};
  float view_inverse_matrix[16]{};
  float projection_scale[4]{};
  uint64_t environmental_valid_fields = 0;
  uint64_t environmental_padding = 0;
};

static_assert(kShaderTextureCount == kTextureStageCount);
static_assert(offsetof(NativeSharedConstants, texture_2d_indices) == 0x000);
static_assert(offsetof(NativeSharedConstants, texture_2d_array_indices) == 0x068);
static_assert(offsetof(NativeSharedConstants, texture_3d_indices) == 0x0D0);
static_assert(offsetof(NativeSharedConstants, texture_cube_indices) == 0x138);
static_assert(offsetof(NativeSharedConstants, sampler_indices) == 0x1A0);
static_assert(offsetof(NativeSharedConstants, booleans) == 0x210);
static_assert(offsetof(NativeSharedConstants, clip_plane) == 0x230);
static_assert(offsetof(NativeSharedConstants, clip_plane_enabled) == 0x240);
static_assert(offsetof(NativeSharedConstants, alpha_threshold) == 0x244);
static_assert(offsetof(NativeSharedConstants, conditional_survey_index) == 0x248);
static_assert(offsetof(NativeSharedConstants, conditional_rendering_index) == 0x24C);
static_assert(offsetof(NativeSharedConstants, motion_blur_time_scale) == 0x250);
static_assert(offsetof(NativeSharedConstants, fog_parameters) == 0x260);
static_assert(offsetof(NativeSharedConstants, camera_position) == 0x270);
static_assert(offsetof(NativeSharedConstants, view_inverse_matrix) == 0x280);
static_assert(offsetof(NativeSharedConstants, projection_scale) == 0x2C0);
static_assert(offsetof(NativeSharedConstants, environmental_valid_fields) == 0x2D0);
static_assert(sizeof(NativeSharedConstants) == 0x2E0);

float MotionBlurTimeScale(const EnvironmentalDataV1* environmental_data) {
  constexpr float kReferenceFramesPerSecond = 30.0f;
  constexpr uint64_t kRequiredFields =
      EnvironmentalFieldBit(EnvironmentalField::kTimeStepSeconds) |
      EnvironmentalFieldBit(EnvironmentalField::kDirectionalMotionBlurLength);
  if (!environmental_data ||
      (environmental_data->valid_fields & kRequiredFields) != kRequiredFields ||
      !std::isfinite(environmental_data->time_step_seconds) ||
      !std::isfinite(environmental_data->directional_motion_blur_length) ||
      environmental_data->time_step_seconds <= 0.0f) {
    return 1.0f;
  }

  const float reference_frame_seconds = 1.0f / kReferenceFramesPerSecond;
  // This feature corrects only the high-frame-rate shrinkage. At 30 Hz and
  // below, preserve the Xbox-authored motion vector and its existing clamps.
  const float clamped_frame_seconds =
      std::min(environmental_data->time_step_seconds, reference_frame_seconds);
  const float scale = reference_frame_seconds / clamped_frame_seconds;
  return std::isfinite(scale) && scale >= 1.0f ? scale : 1.0f;
}
VkPrimitiveTopology ConvertPrimitiveTopology(uint32_t primitive_type) {
  switch (primitive_type) {
    case 1:
      return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case 2:
      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case 3:
      return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case 4:
    case 13:
      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case 5:
      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    case 6:
    case 8:
      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    default:
      return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
  }
}

constexpr uint32_t GetQuadListTriangleListIndexCount(uint32_t quad_list_index_count) {
  return (quad_list_index_count / 4) * 6;
}

uint32_t ConvertVertexUsageToLocation(uint8_t usage, uint8_t usage_index) {
  struct UsageLocation {
    uint8_t usage;
    uint8_t usage_index;
    uint32_t location;
  };
  static constexpr UsageLocation kUsageLocations[] = {
      {0, 0, 0},   {9, 0, 0},   {0, 1, 1},   {0, 2, 2},   {0, 3, 3},
      {3, 0, 4},   {3, 1, 5},   {3, 2, 6},   {3, 3, 7},   {6, 0, 8},
      {6, 1, 9},   {6, 2, 10},  {6, 3, 11},  {7, 0, 12},  {5, 0, 13},
      {5, 1, 14},  {5, 2, 15},  {5, 3, 16},  {10, 0, 17}, {2, 0, 18},
      {1, 0, 19},  {5, 4, 20},  {5, 5, 21},  {5, 6, 22},  {5, 7, 23},
      {5, 8, 24},  {5, 9, 25},  {5, 10, 26}, {5, 11, 27}, {5, 12, 28},
      {5, 13, 29}, {5, 14, 30}, {5, 15, 31}, {10, 1, 32}, {3, 4, 33},
      {3, 5, 34},  {6, 4, 35},  {6, 5, 36},  {7, 1, 37},  {7, 2, 38},
      {7, 3, 39},
  };
  for (const UsageLocation& candidate : kUsageLocations) {
    if (candidate.usage == usage && candidate.usage_index == usage_index) {
      return candidate.location;
    }
  }
  return UINT32_MAX;
}

VkFormat ConvertVertexElementFormat(uint32_t type) {
  switch (type) {
    case 0x2C83A4:
      return VK_FORMAT_R32_SFLOAT;
    case 0x2C23A5:
      return VK_FORMAT_R32G32_SFLOAT;
    case 0x2A23B9:
      return VK_FORMAT_R32G32B32_SFLOAT;
    case 0x1A23A6:
      return VK_FORMAT_R32G32B32A32_SFLOAT;
    case 0x182886:
      return VK_FORMAT_B8G8R8A8_UNORM;
    case 0x1A2286:
    case 0x1A2386:
      return VK_FORMAT_R8G8B8A8_UINT;
    case 0x2C2359:
      return VK_FORMAT_R16G16_SINT;
    case 0x1A235A:
      return VK_FORMAT_R16G16B16A16_SINT;
    case 0x1A2086:
    case 0x1A2186:
      return VK_FORMAT_R8G8B8A8_UNORM;
    case 0x2C2159:
      return VK_FORMAT_R16G16_SNORM;
    case 0x1A215A:
      return VK_FORMAT_R16G16B16A16_SNORM;
    case 0x2C2059:
      return VK_FORMAT_R16G16_UNORM;
    case 0x1A205A:
      return VK_FORMAT_R16G16B16A16_UNORM;
    case 0x2C82A1:
    case 0x2A2287:
    case 0x2A2187:
    case 0x2A2190:
    case 0x2A2390:
      return VK_FORMAT_R32_UINT;
    // D3DDECLTYPE_DEC3N. The upload path supplies its implicit w=1 in the two
    // high bits before Vulkan consumes the packed 10:10:10:2 value.
    case 0x1A2187:
      return VK_FORMAT_A2B10G10R10_SNORM_PACK32;
    case 0x2C235F:
      return VK_FORMAT_R16G16_SFLOAT;
    case 0x1A2360:
      return VK_FORMAT_R16G16B16A16_SFLOAT;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

void CopyGuestWordsToHost(uint8_t* destination, const uint8_t* source, size_t size) {
  size_t offset = 0;
  while (size - offset >= sizeof(uint32_t)) {
    uint32_t word;
    std::memcpy(&word, source + offset, sizeof(word));
    word = __builtin_bswap32(word);
    std::memcpy(destination + offset, &word, sizeof(word));
    offset += sizeof(word);
  }
  if (offset != size) {
    std::memcpy(destination + offset, source + offset, size - offset);
  }
}

uint32_t GetVertexElement16BitComponentCount(uint32_t type) {
  switch (type) {
    case 0x2C2359:
    case 0x2C2159:
    case 0x2C2059:
    case 0x2C235F:
      return 2;
    case 0x1A235A:
    case 0x1A215A:
    case 0x1A205A:
    case 0x1A2360:
      return 4;
    default:
      return 0;
  }
}

uint32_t GetFloat32VertexElementComponentCount(uint32_t type) {
  switch (type) {
    case 0x2C83A4:
      return 1;
    case 0x2C23A5:
      return 2;
    case 0x2A23B9:
      return 3;
    case 0x1A23A6:
      return 4;
    default:
      return 0;
  }
}

struct VertexPayloadConversionCounts {
  uint64_t components_16 = 0;
  uint64_t dec3n = 0;
  uint64_t color_uint = 0;
};

template <typename Declaration, typename Shader>
VertexPayloadConversionCounts ConvertGuestVertexPayload(
    uint8_t* destination, const uint8_t* source, size_t size,
    const Declaration& declaration, const Shader& shader, uint32_t vertex_stream,
    uint32_t stream_offset, uint32_t stride) {
  VertexPayloadConversionCounts counts;
  CopyGuestWordsToHost(destination, source, size);
  if (!stride || stream_offset >= size) {
    return counts;
  }

  for (const VertexElement& element : declaration.elements) {
    if (element.stream != vertex_stream || element.offset >= stride) {
      continue;
    }
    const auto shader_input = std::find_if(
        shader.vertex_inputs.begin(), shader.vertex_inputs.end(),
        [&element](const auto& input) {
          return input.location ==
                 ConvertVertexUsageToLocation(element.usage, element.usage_index);
        });
    if (shader_input == shader.vertex_inputs.end()) {
      continue;
    }

    const uint32_t component_16_count = GetVertexElement16BitComponentCount(element.type);
    const size_t element_size = component_16_count ? size_t(component_16_count) * sizeof(uint16_t)
                                                   : sizeof(uint32_t);
    for (size_t vertex_offset = size_t(stream_offset) + element.offset;
         vertex_offset <= size - std::min(size, element_size); vertex_offset += stride) {
      if (element_size > size - vertex_offset) {
        break;
      }
      if (component_16_count) {
        for (uint32_t component = 0; component < component_16_count; ++component) {
          uint16_t value;
          const size_t component_offset = vertex_offset + size_t(component) * sizeof(uint16_t);
          std::memcpy(&value, source + component_offset, sizeof(value));
          value = __builtin_bswap16(value);
          std::memcpy(destination + component_offset, &value, sizeof(value));
        }
        ++counts.components_16;
      } else if (element.type == 0x1A2187) {
        uint32_t value;
        std::memcpy(&value, destination + vertex_offset, sizeof(value));
        value = (value & 0x3FFFFFFF) | 0x40000000;
        std::memcpy(destination + vertex_offset, &value, sizeof(value));
        ++counts.dec3n;
      } else if (element.type == 0x182886) {
        using NumericType = std::remove_cvref_t<decltype(shader_input->numeric_type)>;
        if (shader_input->numeric_type != NumericType::kUnsignedInteger) {
          continue;
        }
        std::swap(destination[vertex_offset], destination[vertex_offset + 2]);
        ++counts.color_uint;
      }
    }
  }
  return counts;
}

void CopyGuestIndicesToHost(uint8_t* destination, const uint8_t* source, size_t size,
                            bool index32) {
  const size_t element_size = index32 ? sizeof(uint32_t) : sizeof(uint16_t);
  size_t offset = 0;
  while (size - offset >= element_size) {
    if (index32) {
      uint32_t value;
      std::memcpy(&value, source + offset, sizeof(value));
      value = __builtin_bswap32(value);
      const uint32_t masked_value = value & xenos::kVertexIndexMask;
      if (value != masked_value) {
        static std::atomic<uint64_t> masked_index_count{0};
        const uint64_t count = ++masked_index_count;
        if (count <= 32) {
          REXLOG_WARN(
              "gta4-native-index: masked 32-bit index #{} offset={} raw={:08X} "
              "xenos={:08X}",
              count, offset, value, masked_value);
        }
      }
      value = masked_value;
      std::memcpy(destination + offset, &value, sizeof(value));
    } else {
      uint16_t value;
      std::memcpy(&value, source + offset, sizeof(value));
      value = __builtin_bswap16(value);
      std::memcpy(destination + offset, &value, sizeof(value));
    }
    offset += element_size;
  }
  if (offset != size) {
    std::memcpy(destination + offset, source + offset, size - offset);
  }
}

uint32_t LoadGuestWord(const std::vector<uint8_t>& bytes, size_t offset) {
  if (offset > bytes.size() || sizeof(uint32_t) > bytes.size() - offset) {
    return 0;
  }
  uint32_t value;
  std::memcpy(&value, bytes.data() + offset, sizeof(value));
  return __builtin_bswap32(value);
}

VkCompareOp ConvertCompareFunction(uint32_t function) {
  switch (function) {
    case 0:
      return VK_COMPARE_OP_NEVER;
    case 1:
      return VK_COMPARE_OP_LESS;
    case 2:
      return VK_COMPARE_OP_EQUAL;
    case 3:
      return VK_COMPARE_OP_LESS_OR_EQUAL;
    case 4:
      return VK_COMPARE_OP_GREATER;
    case 5:
      return VK_COMPARE_OP_NOT_EQUAL;
    case 6:
      return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case 7:
      return VK_COMPARE_OP_ALWAYS;
    default:
      return VK_COMPARE_OP_ALWAYS;
  }
}

VkStencilOp ConvertStencilOperation(uint32_t operation) {
  switch (operation) {
    case 0:
      return VK_STENCIL_OP_KEEP;
    case 1:
      return VK_STENCIL_OP_ZERO;
    case 2:
      return VK_STENCIL_OP_REPLACE;
    case 3:
      return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
    case 4:
      return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
    case 5:
      return VK_STENCIL_OP_INVERT;
    case 6:
      return VK_STENCIL_OP_INCREMENT_AND_WRAP;
    case 7:
      return VK_STENCIL_OP_DECREMENT_AND_WRAP;
    default:
      return VK_STENCIL_OP_KEEP;
  }
}

VkBlendFactor ConvertBlendFactor(uint32_t factor) {
  switch (factor) {
    case 0:
      return VK_BLEND_FACTOR_ZERO;
    case 1:
      return VK_BLEND_FACTOR_ONE;
    case 4:
      return VK_BLEND_FACTOR_SRC_COLOR;
    case 5:
      return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case 6:
      return VK_BLEND_FACTOR_SRC_ALPHA;
    case 7:
      return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case 8:
      return VK_BLEND_FACTOR_DST_COLOR;
    case 9:
      return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case 10:
      return VK_BLEND_FACTOR_DST_ALPHA;
    case 11:
      return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case 12:
      return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case 13:
      return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case 14:
      return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case 15:
      return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case 16:
      return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    default:
      return VK_BLEND_FACTOR_ONE;
  }
}

VkBlendOp ConvertBlendOperation(uint32_t operation) {
  switch (operation) {
    case 0:
      return VK_BLEND_OP_ADD;
    case 1:
      return VK_BLEND_OP_SUBTRACT;
    case 2:
      return VK_BLEND_OP_MIN;
    case 3:
      return VK_BLEND_OP_MAX;
    case 4:
      return VK_BLEND_OP_REVERSE_SUBTRACT;
    default:
      return VK_BLEND_OP_ADD;
  }
}

VkColorComponentFlags ConvertColorWriteMask(uint32_t mask) {
  VkColorComponentFlags result = 0;
  if (mask & 0x1) {
    result |= VK_COLOR_COMPONENT_R_BIT;
  }
  if (mask & 0x2) {
    result |= VK_COLOR_COMPONENT_G_BIT;
  }
  if (mask & 0x4) {
    result |= VK_COLOR_COMPONENT_B_BIT;
  }
  if (mask & 0x8) {
    result |= VK_COLOR_COMPONENT_A_BIT;
  }
  return result;
}

VkFormat ConvertTextureFormat(xenos::TextureFormat format) {
  switch (GetBaseFormat(format)) {
    case xenos::TextureFormat::k_8:
      return VK_FORMAT_R8_UNORM;
    case xenos::TextureFormat::k_8_8_8_8:
      return VK_FORMAT_R8G8B8A8_UNORM;
    case xenos::TextureFormat::k_DXT1:
      return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case xenos::TextureFormat::k_DXT2_3:
      return VK_FORMAT_BC2_UNORM_BLOCK;
    case xenos::TextureFormat::k_DXT4_5:
      return VK_FORMAT_BC3_UNORM_BLOCK;
    case xenos::TextureFormat::k_DXN:
      return VK_FORMAT_R8G8_UNORM;
    case xenos::TextureFormat::k_DXT5A:
      return VK_FORMAT_R8_UNORM;
    case xenos::TextureFormat::k_DXT3A:
      return VK_FORMAT_BC2_UNORM_BLOCK;
    case xenos::TextureFormat::k_CTX1:
      return VK_FORMAT_R8G8_UNORM;
    case xenos::TextureFormat::k_16_16_16_16_FLOAT:
      return VK_FORMAT_R16G16B16A16_SFLOAT;
    case xenos::TextureFormat::k_16_16_FLOAT:
      return VK_FORMAT_R16G16_SFLOAT;
    case xenos::TextureFormat::k_32_FLOAT:
      return VK_FORMAT_R32_SFLOAT;
    case xenos::TextureFormat::k_24_8:
      return VK_FORMAT_D24_UNORM_S8_UINT;
    case xenos::TextureFormat::k_24_8_FLOAT:
      return VK_FORMAT_D32_SFLOAT_S8_UINT;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

uint32_t GetNativeHostFormatSwizzle(xenos::TextureFormat format) {
  switch (GetBaseFormat(format)) {
    case xenos::TextureFormat::k_8:
    case xenos::TextureFormat::k_32_FLOAT:
    case xenos::TextureFormat::k_24_8:
    case xenos::TextureFormat::k_24_8_FLOAT:
      return xenos::XE_GPU_TEXTURE_SWIZZLE_RRRR;
    case xenos::TextureFormat::k_DXN:
      return xenos::XE_GPU_TEXTURE_SWIZZLE_RGGG;
    case xenos::TextureFormat::k_DXT5A:
      return xenos::XE_GPU_TEXTURE_SWIZZLE_RRRR;
    case xenos::TextureFormat::k_DXT3A:
      return XE_GPU_MAKE_TEXTURE_SWIZZLE(A, A, A, A);
    case xenos::TextureFormat::k_CTX1:
      return xenos::XE_GPU_TEXTURE_SWIZZLE_RGGG;
    default:
      return xenos::XE_GPU_TEXTURE_SWIZZLE_RGBA;
  }
}

uint32_t GuestToNativeHostSwizzle(uint32_t guest_swizzle, uint32_t host_format_swizzle) {
  uint32_t host_swizzle = 0;
  for (uint32_t i = 0; i < 4; ++i) {
    uint32_t guest_component = (guest_swizzle >> (3 * i)) & 0b111;
    uint32_t host_component;
    if (guest_component >= xenos::XE_GPU_TEXTURE_SWIZZLE_0) {
      host_component = guest_component & 0b101;
    } else {
      host_component = (host_format_swizzle >> (3 * guest_component)) & 0b111;
    }
    host_swizzle |= host_component << (3 * i);
  }
  return host_swizzle;
}

VkComponentSwizzle GetNativeComponentSwizzle(uint32_t texture_swizzle, uint32_t component_index) {
  const auto component =
      xenos::XE_GPU_TEXTURE_SWIZZLE((texture_swizzle >> (3 * component_index)) & 0b111);
  if (component == xenos::XE_GPU_TEXTURE_SWIZZLE(component_index)) {
    return VK_COMPONENT_SWIZZLE_IDENTITY;
  }
  switch (component) {
    case xenos::XE_GPU_TEXTURE_SWIZZLE_R:
      return VK_COMPONENT_SWIZZLE_R;
    case xenos::XE_GPU_TEXTURE_SWIZZLE_G:
      return VK_COMPONENT_SWIZZLE_G;
    case xenos::XE_GPU_TEXTURE_SWIZZLE_B:
      return VK_COMPONENT_SWIZZLE_B;
    case xenos::XE_GPU_TEXTURE_SWIZZLE_A:
      return VK_COMPONENT_SWIZZLE_A;
    case xenos::XE_GPU_TEXTURE_SWIZZLE_0:
      return VK_COMPONENT_SWIZZLE_ZERO;
    case xenos::XE_GPU_TEXTURE_SWIZZLE_1:
      return VK_COMPONENT_SWIZZLE_ONE;
    default:
      return VK_COMPONENT_SWIZZLE_IDENTITY;
  }
}

bool SurfaceDescriptorsEqual(const SurfaceDescriptor& left, const SurfaceDescriptor& right) {
  // Host images are cached surface views. Guest placement content is shared separately through
  // the placement-owner map, but one cached image must not silently change the placement whose
  // bytes it currently represents.
  return left.handle == right.handle && left.format == right.format && left.width == right.width &&
         left.height == right.height && left.sample_type == right.sample_type &&
         left.base == right.base &&
         (left.address & 0x7FFu) == (right.address & 0x7FFu);
}

VkSampleCountFlagBits ConvertSurfaceSamples(uint32_t sample_type) {
  switch (sample_type) {
    case uint32_t(xenos::MsaaSamples::k1X):
      return VK_SAMPLE_COUNT_1_BIT;
    case uint32_t(xenos::MsaaSamples::k2X):
      return VK_SAMPLE_COUNT_2_BIT;
    case uint32_t(xenos::MsaaSamples::k4X):
      return VK_SAMPLE_COUNT_4_BIT;
    default:
      return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
  }
}

VkSampleCountFlagBits ConvertNativeSampleCount(uint32_t sample_count) {
  switch (sample_count) {
    case 1:
      return VK_SAMPLE_COUNT_1_BIT;
    case 2:
      return VK_SAMPLE_COUNT_2_BIT;
    case 4:
      return VK_SAMPLE_COUNT_4_BIT;
    default:
      return VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM;
  }
}

xenos::MsaaSamples ConvertHostSamplesToGuestSamples(VkSampleCountFlagBits samples) {
  switch (samples) {
    case VK_SAMPLE_COUNT_1_BIT:
      return xenos::MsaaSamples::k1X;
    case VK_SAMPLE_COUNT_2_BIT:
      return xenos::MsaaSamples::k2X;
    case VK_SAMPLE_COUNT_4_BIT:
      return xenos::MsaaSamples::k4X;
    default:
      return xenos::MsaaSamples::k1X;
  }
}

xenos::CopySampleSelect ResolveAllHostSamples(xenos::MsaaSamples samples) {
  switch (samples) {
    case xenos::MsaaSamples::k4X:
      return xenos::CopySampleSelect::k0123;
    case xenos::MsaaSamples::k2X:
      return xenos::CopySampleSelect::k01;
    default:
      return xenos::CopySampleSelect::k0;
  }
}

VkFormat ConvertSurfaceFormat(uint32_t raw_format, bool depth) {
  if (depth) {
    switch (raw_format) {
      case kD3dFmtD24FS8:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
      default:
        return VK_FORMAT_UNDEFINED;
    }
  }
  switch (raw_format) {
    case kD3dFmtA8R8G8B8:
      return VK_FORMAT_R8G8B8A8_UNORM;
    case kD3dFmtA16B16G16R16F2:
      return VK_FORMAT_R16G16B16A16_SFLOAT;
    case kD3dFmtR32F:
      return VK_FORMAT_R32_SFLOAT;
    case kD3dFmtG16R16F:
    case kD3dFmtG16R16F2:
      return VK_FORMAT_R16G16_SFLOAT;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

size_t CommandSize(CommandType type) {
  switch (type) {
    case CommandType::kDeviceCreated:
    case CommandType::kDeviceDestroyed:
      return sizeof(DeviceCommand);
    case CommandType::kRegisterShader:
      return sizeof(RegisterShaderCommand);
    case CommandType::kRegisterVertexDeclaration:
      return sizeof(RegisterVertexDeclarationCommand);
    case CommandType::kSetRenderState:
      return sizeof(SetRenderStateCommand);
    case CommandType::kSetPixelShader:
    case CommandType::kSetVertexShader:
      return sizeof(SetShaderCommand);
    case CommandType::kSetVertexDeclaration:
      return sizeof(SetVertexDeclarationCommand);
    case CommandType::kSetTexture:
      return sizeof(SetTextureCommand);
    case CommandType::kInvalidateTexture:
      return sizeof(InvalidateTextureCommand);
    case CommandType::kSetDepthStencil:
      return sizeof(SetDepthStencilCommand);
    case CommandType::kSetRenderTarget:
      return sizeof(SetRenderTargetCommand);
    case CommandType::kSetVertexStream:
      return sizeof(SetVertexStreamCommand);
    case CommandType::kSetIndexBuffer:
      return sizeof(SetIndexBufferCommand);
    case CommandType::kDrawPrimitive:
      return sizeof(DrawPrimitiveCommand);
    case CommandType::kDrawPrimitiveUp:
      return sizeof(DrawPrimitiveUpCommand);
    case CommandType::kDrawIndexedPrimitive:
      return sizeof(DrawIndexedPrimitiveCommand);
    case CommandType::kResolve:
      return sizeof(ResolveCommand);
    case CommandType::kTextureLock:
      return sizeof(TextureLockCommand);
    case CommandType::kClear:
      return sizeof(ClearCommand);
    case CommandType::kRenderPhaseMarker:
      return sizeof(RenderPhaseMarkerCommand);
    case CommandType::kPresent:
      return sizeof(PresentCommand);
    case CommandType::kQueryDeviceCapabilities:
      return sizeof(QueryDeviceCapabilitiesCommand);
    case CommandType::kRegisterReflectionTarget:
      return sizeof(RegisterReflectionTargetCommand);
    case CommandType::kReleaseResource:
      return sizeof(ReleaseResourceCommand);
    case CommandType::kUpdateEnvironmentalData:
      return sizeof(UpdateEnvironmentalDataCommand);
    case CommandType::kDepthSurfaceHandoff:
      return sizeof(DepthSurfaceHandoffCommand);
  }
  return 0;
}

uint32_t CommandDevice(CommandType type, const void* command) {
  switch (type) {
    case CommandType::kDeviceCreated:
    case CommandType::kDeviceDestroyed:
      return static_cast<const DeviceCommand*>(command)->device;
    case CommandType::kRegisterShader:
      return 0;
    case CommandType::kRegisterVertexDeclaration:
      return static_cast<const RegisterVertexDeclarationCommand*>(command)->device;
    case CommandType::kSetRenderState:
      return static_cast<const SetRenderStateCommand*>(command)->device;
    case CommandType::kSetPixelShader:
    case CommandType::kSetVertexShader:
      return static_cast<const SetShaderCommand*>(command)->device;
    case CommandType::kSetVertexDeclaration:
      return static_cast<const SetVertexDeclarationCommand*>(command)->device;
    case CommandType::kSetTexture:
      return static_cast<const SetTextureCommand*>(command)->device;
    case CommandType::kInvalidateTexture:
      return 0;
    case CommandType::kSetDepthStencil:
      return static_cast<const SetDepthStencilCommand*>(command)->device;
    case CommandType::kSetRenderTarget:
      return static_cast<const SetRenderTargetCommand*>(command)->device;
    case CommandType::kSetVertexStream:
      return static_cast<const SetVertexStreamCommand*>(command)->device;
    case CommandType::kSetIndexBuffer:
      return static_cast<const SetIndexBufferCommand*>(command)->device;
    case CommandType::kDrawPrimitive:
      return static_cast<const DrawPrimitiveCommand*>(command)->device;
    case CommandType::kDrawPrimitiveUp:
      return static_cast<const DrawPrimitiveUpCommand*>(command)->device;
    case CommandType::kDrawIndexedPrimitive:
      return static_cast<const DrawIndexedPrimitiveCommand*>(command)->device;
    case CommandType::kResolve:
      return static_cast<const ResolveCommand*>(command)->device;
    case CommandType::kTextureLock:
      return 0;
    case CommandType::kClear:
      return static_cast<const ClearCommand*>(command)->device;
    case CommandType::kRenderPhaseMarker:
      return static_cast<const RenderPhaseMarkerCommand*>(command)->device;
    case CommandType::kPresent:
      return static_cast<const PresentCommand*>(command)->device;
    case CommandType::kQueryDeviceCapabilities:
    case CommandType::kRegisterReflectionTarget:
    case CommandType::kReleaseResource:
      return 0;
    case CommandType::kUpdateEnvironmentalData:
      return static_cast<const UpdateEnvironmentalDataCommand*>(command)->device;
    case CommandType::kDepthSurfaceHandoff:
      return static_cast<const DepthSurfaceHandoffCommand*>(command)->device;
  }
  return 0;
}

}  // namespace

Gta4NativeGraphicsSystem::Gta4NativeGraphicsSystem() = default;

Gta4NativeGraphicsSystem::~Gta4NativeGraphicsSystem() {
  Shutdown();
}

void Gta4NativeGraphicsSystem::TraceNativeRendererEvent(std::string_view point,
                                                         std::string_view details) {
  if (!deterministic_trace_active_) {
    return;
  }
  const uint64_t event = deterministic_trace_event_++;
  std::string trace_line;
  if (diagnostic_command_index_ == SIZE_MAX) {
    trace_line = fmt::format("[NativeTrace] frame={} event={} cmd=- point={}{}{}",
                             diagnostic_submitted_frame_, event, point,
                             details.empty() ? "" : " ", details);
  } else {
    trace_line = fmt::format("[NativeTrace] frame={} event={} cmd={} point={}{}{}",
                             diagnostic_submitted_frame_, event, diagnostic_command_index_, point,
                             details.empty() ? "" : " ", details);
  }
  std::fprintf(stderr, "%s\n", trace_line.c_str());
  std::fflush(stderr);
  // stderr is /dev/null when the app is launched normally on macOS. Mirror the
  // deterministic trace into the persistent Rex log so the same causal record
  // is visible for both terminal and app-bundle launches.
  REXLOG_INFO("{}", trace_line);
}

X_STATUS Gta4NativeGraphicsSystem::SetupPresentation(ui::WindowedAppContext* app_context) {
  if (presenter_) {
    return X_STATUS_SUCCESS;
  }

  if (!provider_) {
    provider_ = ui::vulkan::VulkanProvider::Create(false, true, true, true);
  }
  if (!provider_) {
    return X_STATUS_UNSUCCESSFUL;
  }

  app_context_ = app_context;
  auto create_presenter = [this]() { presenter_ = provider_->CreatePresenter(); };
  if (app_context_) {
    app_context_->CallInUIThreadSynchronous(create_presenter);
  } else {
    create_presenter();
  }
  return presenter_ ? X_STATUS_SUCCESS : X_STATUS_UNSUCCESSFUL;
}

X_STATUS Gta4NativeGraphicsSystem::SetupGuestGpu(runtime::FunctionDispatcher* function_dispatcher,
                                                 system::KernelState* kernel_state) {
  if (memory_) {
    return X_STATUS_SUCCESS;
  }
  if (!function_dispatcher || !kernel_state) {
    return X_STATUS_INVALID_PARAMETER;
  }

  memory_ = function_dispatcher->memory();
  if (!memory_) {
    return X_STATUS_UNSUCCESSFUL;
  }

  if (!provider_) {
    provider_ = ui::vulkan::VulkanProvider::Create(false, false, true, true);
    if (!provider_) {
      memory_ = nullptr;
      return X_STATUS_UNSUCCESSFUL;
    }
  }

  StartRenderWorker();
  return X_STATUS_SUCCESS;
}

uint32_t Gta4NativeGraphicsSystem::GetTitleCommandAbi(uint32_t title_id) const {
  return title_id == kTitleId ? kTitleCommandAbi : 0;
}

bool Gta4NativeGraphicsSystem::SubmitTitleCommand(uint32_t title_id, uint32_t abi_version,
                                                  const void* command, size_t command_size) {
  if (title_id != kTitleId || abi_version != kTitleCommandAbi || !command || !memory_) {
    return false;
  }

  NativeCommand native_command;
  if (!ValidateAndCopyCommand(command, command_size, native_command)) {
    return false;
  }

  {
    std::unique_lock lock(render_mutex_);
    render_condition_.wait(lock, [this]() {
      return !render_worker_running_ || render_queue_.size() < kMaximumQueuedCommands;
    });
    if (!render_worker_running_) {
      return false;
    }
    render_queue_.push_back(std::move(native_command));
  }
  render_condition_.notify_one();
  return true;
}

bool Gta4NativeGraphicsSystem::ExecuteTitleCommand(uint32_t title_id, uint32_t abi_version,
                                                   const void* command, size_t command_size,
                                                   void* result, size_t result_size) {
  if (title_id != kTitleId || abi_version != kTitleCommandAbi || !command || !result ||
      command_size < sizeof(CommandHeader) || !memory_) {
    return false;
  }

  CommandHeader header;
  std::memcpy(&header, command, sizeof(header));
  if (header.size != command_size) {
    return false;
  }

  if (header.type == CommandType::kQueryDeviceCapabilities) {
    if (command_size != sizeof(QueryDeviceCapabilitiesCommand) ||
        result_size != sizeof(DeviceCapabilitiesResult) || !provider_) {
      return false;
    }
    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const auto* vulkan_device = vulkan_provider->vulkan_device();
    if (!vulkan_device) {
      return false;
    }
    DeviceCapabilitiesResult capabilities{};
    capabilities.max_image_dimension_2d = vulkan_device->properties().maxImageDimension2D;
    std::memcpy(result, &capabilities, sizeof(capabilities));
    return true;
  }

  if (command_size != sizeof(TextureLockCommand) ||
      result_size != sizeof(TextureLockResult)) {
    return false;
  }
  TextureLockCommand lock_command;
  std::memcpy(&lock_command, command, sizeof(lock_command));
  if (lock_command.header.size != sizeof(lock_command) ||
      lock_command.header.type != CommandType::kTextureLock || !lock_command.texture ||
      lock_command.flags != 0) {
    return false;
  }

  NativeCommand native_command;
  native_command.type = CommandType::kTextureLock;
  native_command.bytes.resize(sizeof(lock_command));
  std::memcpy(native_command.bytes.data(), &lock_command, sizeof(lock_command));
  native_command.synchronous = std::make_shared<SynchronousCommand>();
  const auto synchronous = native_command.synchronous;
  {
    std::unique_lock lock(render_mutex_);
    render_condition_.wait(lock, [this]() {
      return !render_worker_running_ || render_queue_.size() < kMaximumQueuedCommands;
    });
    if (!render_worker_running_) {
      return false;
    }
    render_queue_.push_back(std::move(native_command));
  }
  render_condition_.notify_one();

  std::unique_lock completion_lock(synchronous->mutex);
  synchronous->condition.wait(completion_lock, [&synchronous]() { return synchronous->complete; });
  std::memcpy(result, &synchronous->result, sizeof(synchronous->result));
  return synchronous->succeeded;
}

void Gta4NativeGraphicsSystem::InitializeShaderStorage(
    const std::filesystem::path& cache_root, uint32_t title_id, bool blocking) {
  (void)blocking;
  if (title_id != kTitleId || cache_root.empty()) {
    return;
  }
  native_pipeline_cache_root_ = cache_root;
  native_pipeline_cache_title_id_ = title_id;
}

Gta4NativeGraphicsSystem::NativeFixedFunctionState
Gta4NativeGraphicsSystem::DecodeFixedFunctionState(
    const std::vector<uint8_t>& device_snapshot) const {
  NativeFixedFunctionState state{};
  // sub_82A50160 installs getter function pointers at device + 548 + state.
  // The getters themselves read these packed, authoritative device fields.
  const uint32_t depth_stencil = LoadGuestWord(device_snapshot, kDepthStencilPackedOffset);
  const uint32_t alpha_test_function =
      LoadGuestWord(device_snapshot, kAlphaTestFunctionPackedOffset);
  // D3DDevice_SetRenderState_SeparateAlphaBlendEnable expands the D3D-facing
  // blend word into the hardware blend-control words, mirroring the color
  // factors into alpha while separate alpha blending is disabled. Consume the
  // expanded primary target word so stale alpha factors can't affect draws.
  const uint32_t blend = LoadGuestWord(device_snapshot, kHardwareBlendPackedOffset);
  const uint32_t blend_control = LoadGuestWord(device_snapshot, kBlendControlOffset);

  state.depth_enable = LoadGuestWord(device_snapshot, kDepthEnableOffset);
  state.depth_function = (depth_stencil & 0x00000070) >> 4;
  state.depth_write_enable = (depth_stencil & 0x00000004) >> 2;
  const uint32_t cull_mode_packed = LoadGuestWord(device_snapshot, kCullModePackedOffset);
  state.cull_mode = cull_mode_packed & 0x7;
  state.blend_enable = (blend_control & 0x80000000) >> 31;
  state.source_blend = blend & 0x0000001F;
  state.destination_blend = (blend & 0x00001F00) >> 8;
  state.blend_operation = (blend & 0x000000E0) >> 5;
  state.source_blend_alpha = (blend & 0x001F0000) >> 16;
  state.destination_blend_alpha = (blend & 0x1F000000) >> 24;
  state.blend_operation_alpha = (blend & 0x00E00000) >> 21;
  for (uint32_t component = 0; component < state.blend_constants.size(); ++component) {
    state.blend_constants[component] = std::bit_cast<float>(
        LoadGuestWord(device_snapshot, kBlendConstantsOffset + component * sizeof(uint32_t)));
  }
  state.alpha_test_enable = (alpha_test_function & 0x00000008) >> 3;
  state.alpha_function = alpha_test_function & 0x7;
  state.alpha_reference =
      std::bit_cast<float>(LoadGuestWord(device_snapshot, kAlphaReferenceOffset));
  state.stencil_enable = LoadGuestWord(device_snapshot, kStencilEnableOffset);
  state.two_sided_stencil = (depth_stencil & 0x00000080) >> 7;
  state.stencil_function = (depth_stencil & 0x00000700) >> 8;
  state.stencil_fail = (depth_stencil & 0x00003800) >> 11;
  state.stencil_pass = (depth_stencil & 0x0001C000) >> 14;
  state.stencil_depth_fail = (depth_stencil & 0x000E0000) >> 17;
  state.ccw_stencil_function = (depth_stencil & 0x00700000) >> 20;
  state.ccw_stencil_fail = (depth_stencil & 0x03800000) >> 23;
  state.ccw_stencil_pass = (depth_stencil & 0x1C000000) >> 26;
  state.ccw_stencil_depth_fail = (depth_stencil & 0xE0000000) >> 29;
  state.stencil_reference = device_snapshot[kStencilReferenceOffset];
  state.stencil_mask = device_snapshot[kStencilMaskOffset];
  state.stencil_write_mask = device_snapshot[kStencilWriteMaskOffset];
  state.back_stencil_reference = device_snapshot[kBackStencilReferenceOffset];
  state.back_stencil_mask = device_snapshot[kBackStencilMaskOffset];
  state.back_stencil_write_mask = device_snapshot[kBackStencilWriteMaskOffset];
  state.scissor_enable = LoadGuestWord(device_snapshot, kScissorEnableOffset);
  const bool front_culled = state.cull_mode == 1 || state.cull_mode == 5;
  const uint32_t depth_bias_offset = front_culled ? kBackDepthBiasOffset : kDepthBiasOffset;
  const uint32_t slope_depth_bias_offset =
      front_culled ? kBackSlopeScaledDepthBiasOffset : kSlopeScaledDepthBiasOffset;
  state.slope_scaled_depth_bias_bits = LoadGuestWord(device_snapshot, slope_depth_bias_offset);
  state.depth_bias_bits = LoadGuestWord(device_snapshot, depth_bias_offset);
  state.depth_bias_enable =
      (cull_mode_packed & (front_culled ? uint32_t(0x1000) : uint32_t(0x800))) != 0;
  state.color_write_mask = LoadGuestWord(device_snapshot, kColorWriteMaskOffset);
  state.viewport_bits = {
      LoadGuestWord(device_snapshot, kViewportXOffset),
      LoadGuestWord(device_snapshot, kViewportYOffset),
      LoadGuestWord(device_snapshot, kViewportWidthOffset),
      LoadGuestWord(device_snapshot, kViewportHeightOffset),
      LoadGuestWord(device_snapshot, kViewportMinDepthOffset),
      LoadGuestWord(device_snapshot, kViewportMaxDepthOffset),
  };
  state.scissor = {
      int32_t(LoadGuestWord(device_snapshot, kScissorLeftOffset)),
      int32_t(LoadGuestWord(device_snapshot, kScissorTopOffset)),
      int32_t(LoadGuestWord(device_snapshot, kScissorRightOffset)),
      int32_t(LoadGuestWord(device_snapshot, kScissorBottomOffset)),
  };
  return state;
}

bool Gta4NativeGraphicsSystem::ValidateAndCopyCommand(const void* command, size_t command_size,
                                                      NativeCommand& native_command) {
  auto reject = [command_size](CommandType type, const char* reason) {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    if (count <= 32 || !(count % 1024)) {
      REXLOG_WARN("gta4-native-diag: rejected title command #{} type={}({}) size={} reason={}",
                  count, uint32_t(type), CommandTypeName(type), command_size, reason);
    }
    return false;
  };

  if (command_size < sizeof(CommandHeader)) {
    return reject(CommandType::kPresent, "short-header");
  }

  CommandHeader header;
  std::memcpy(&header, command, sizeof(header));
  const size_t expected_size = CommandSize(header.type);
  if (!expected_size || command_size != expected_size || header.size != expected_size ||
      (header.type != CommandType::kRegisterShader &&
       header.type != CommandType::kInvalidateTexture && header.type != CommandType::kTextureLock &&
       header.type != CommandType::kRegisterReflectionTarget &&
       header.type != CommandType::kReleaseResource &&
       !CommandDevice(header.type, command))) {
    return reject(header.type, "header-size-or-device");
  }

  if (header.type == CommandType::kRegisterShader) {
    const auto& register_shader = *static_cast<const RegisterShaderCommand*>(command);
    if (!register_shader.shader || !register_shader.hash ||
        (register_shader.stage != ShaderStage::kPixel &&
         register_shader.stage != ShaderStage::kVertex)) {
      return reject(header.type, "invalid-shader");
    }
  } else if (header.type == CommandType::kRegisterVertexDeclaration) {
    const auto& declaration = *static_cast<const RegisterVertexDeclarationCommand*>(command);
    if (!declaration.declaration || !declaration.element_count ||
        declaration.element_count > kMaximumVertexElementCount ||
        declaration.maximum_stream >= kVertexStreamCount) {
      return reject(header.type, "invalid-vdecl-header");
    }
    for (uint32_t index = 0; index < declaration.element_count; ++index) {
      const VertexElement& element = declaration.elements[index];
      if (element.stream >= kVertexStreamCount || element.stream > declaration.maximum_stream ||
          element.usage > 13) {
        return reject(header.type, "invalid-vdecl-element");
      }
    }
  }

  if (header.type == CommandType::kSetTexture) {
    const auto& set_texture = *static_cast<const SetTextureCommand*>(command);
    if (set_texture.stage >= kTextureStageCount || set_texture.vector_font_id > 3) {
      return reject(header.type, "texture-stage-range");
    }
    if (set_texture.texture && set_texture.vector_font_id) {
      std::lock_guard lock(texture_resource_mutex_);
      const auto previous = vector_font_ids_.find(set_texture.texture);
      if (previous == vector_font_ids_.end() || previous->second != set_texture.vector_font_id) {
        vector_font_ids_[set_texture.texture] = set_texture.vector_font_id;
        dirty_texture_handles_.insert(set_texture.texture);
        REXLOG_INFO("gta4-native-fonts: registered font{} owner texture {:08X}",
                    set_texture.vector_font_id, set_texture.texture);
      }
    }
  } else if (header.type == CommandType::kSetRenderTarget) {
    const auto& set_target = *static_cast<const SetRenderTargetCommand*>(command);
    if (set_target.index >= kRenderTargetCount) {
      return reject(header.type, "render-target-range");
    }
  } else if (header.type == CommandType::kSetVertexStream) {
    const auto& set_stream = *static_cast<const SetVertexStreamCommand*>(command);
    if (set_stream.stream >= kVertexStreamCount) {
      return reject(header.type, "vertex-stream-range");
    }
  } else if (header.type == CommandType::kResolve) {
    const auto& resolve = *static_cast<const ResolveCommand*>(command);
    if (!resolve.source.handle || !resolve.destination_texture) {
      return reject(header.type, "resolve-source-or-destination");
    }
  } else if (header.type == CommandType::kDepthSurfaceHandoff) {
    const auto& handoff = *static_cast<const DepthSurfaceHandoffCommand*>(command);
    if (!handoff.source.handle || !handoff.destination.handle ||
        handoff.source.handle == handoff.destination.handle ||
        !handoff.source_wrapper || !handoff.destination_wrapper ||
        !handoff.source_texture) {
      return reject(header.type, "depth-handoff-resource");
    }
  } else if (header.type == CommandType::kRegisterReflectionTarget) {
    const auto& reflection =
        *static_cast<const RegisterReflectionTargetCommand*>(command);
    if (reflection.family > ReflectionFamily::kEnvironment ||
        reflection.role > ReflectionRole::kDepth || !reflection.wrapper ||
        !reflection.surface ||
        (reflection.role == ReflectionRole::kColor && !reflection.texture) ||
        !reflection.logical_width ||
        !reflection.logical_height || !reflection.physical_width ||
        !reflection.physical_height ||
        (reflection.sample_count_override != 0 &&
         reflection.sample_count_override != 1 &&
         reflection.sample_count_override != 2 &&
         reflection.sample_count_override != 4)) {
      return reject(header.type, "invalid-reflection-target");
    }
  } else if (header.type == CommandType::kUpdateEnvironmentalData) {
    const auto& update = *static_cast<const UpdateEnvironmentalDataCommand*>(command);
    const EnvironmentalDataV1& data = update.data;
    if (update.reserved || data.version != kEnvironmentalDataVersion ||
        data.byte_size != sizeof(data) || !data.source_sequence ||
        (data.valid_fields & ~kEnvironmentalFieldMask)) {
      return reject(header.type, "invalid-environmental-header");
    }
    auto field_is_valid = [&data](EnvironmentalField field) {
      return (data.valid_fields & EnvironmentalFieldBit(field)) != 0;
    };
    auto finite_scalar = [&field_is_valid](EnvironmentalField field, float value) {
      return !field_is_valid(field) || std::isfinite(value);
    };
    auto finite_array = [&field_is_valid](EnvironmentalField field, const auto& values) {
      return !field_is_valid(field) ||
             std::all_of(values.begin(), values.end(),
                         [](float value) { return std::isfinite(value); });
    };
    if (!finite_scalar(EnvironmentalField::kTimeStepSeconds, data.time_step_seconds) ||
        (field_is_valid(EnvironmentalField::kTimeStepSeconds) &&
         data.time_step_seconds < 0.0f) ||
        !finite_scalar(EnvironmentalField::kMotionBlurScale, data.motion_blur_scale) ||
        !finite_scalar(EnvironmentalField::kDirectionalMotionBlurLength,
                       data.directional_motion_blur_length) ||
        !finite_scalar(EnvironmentalField::kFogStart, data.fog_start) ||
        !finite_scalar(EnvironmentalField::kFogDensity, data.fog_density) ||
        !finite_scalar(EnvironmentalField::kFogHeightFalloff, data.fog_height_falloff) ||
        !finite_scalar(EnvironmentalField::kFogAltitudeTweak, data.fog_altitude_tweak) ||
        !finite_scalar(EnvironmentalField::kFogPower, data.fog_power) ||
        !finite_scalar(EnvironmentalField::kCameraAltitude, data.camera_altitude) ||
        !finite_array(EnvironmentalField::kFogColor, data.fog_color) ||
        !finite_array(EnvironmentalField::kSunDirection, data.sun_direction) ||
        !finite_array(EnvironmentalField::kSunColor, data.sun_color) ||
        !finite_array(EnvironmentalField::kCameraPosition, data.camera_position) ||
        !finite_array(EnvironmentalField::kViewMatrix, data.view_matrix) ||
        !finite_array(EnvironmentalField::kViewInverseMatrix, data.view_inverse_matrix) ||
        !finite_array(EnvironmentalField::kProjectionMatrix, data.projection_matrix) ||
        !finite_array(EnvironmentalField::kViewProjectionMatrix,
                      data.view_projection_matrix)) {
      return reject(header.type, "invalid-environmental-value");
    }
  }

  if (header.type == CommandType::kInvalidateTexture) {
    const auto& invalidate = *static_cast<const InvalidateTextureCommand*>(command);
    if (!invalidate.texture) {
      return reject(header.type, "invalidate-null-texture");
    }
    std::lock_guard lock(texture_resource_mutex_);
    dirty_texture_handles_.insert(invalidate.texture);
  }

  if (header.type == CommandType::kReleaseResource) {
    const auto& release = *static_cast<const ReleaseResourceCommand*>(command);
    if (!release.resource) {
      return reject(header.type, "release-null-resource");
    }

    size_t remaining_textures = 0;
    size_t remaining_buffers = 0;
    bool erased_texture = false;
    bool erased_buffer = false;
    {
      std::lock_guard lock(texture_resource_mutex_);
      erased_texture = texture_resources_.erase(release.resource) != 0;
      dirty_texture_handles_.erase(release.resource);
      vector_font_ids_.erase(release.resource);
      remaining_textures = texture_resources_.size();
    }
    {
      std::lock_guard lock(buffer_resource_mutex_);
      erased_buffer = buffer_resources_.erase(release.resource) != 0;
      remaining_buffers = buffer_resources_.size();
    }

    if (erased_texture || erased_buffer) {
      static std::atomic<uint64_t> release_count{0};
      const uint64_t count = ++release_count;
      if (count <= 64 || !(count % 4096)) {
        REXLOG_INFO(
            "gta4-native-resource-release: release={} handle={:08X} texture={} buffer={} "
            "remaining-textures={} remaining-buffers={}",
            count, release.resource, erased_texture, erased_buffer, remaining_textures,
            remaining_buffers);
      }
    }
  }

  if (header.type == CommandType::kRenderPhaseMarker) {
    const auto& marker = *static_cast<const RenderPhaseMarkerCommand*>(command);
    if (marker.phase <= RenderPhase::kUnknown || marker.phase > RenderPhase::kCompositePostFx ||
        (marker.event != RenderPhaseEvent::kBegin && marker.event != RenderPhaseEvent::kEnd)) {
      return reject(header.type, "render-phase-range");
    }
  }

  native_command.type = header.type;
  native_command.bytes.resize(command_size);
  std::memcpy(native_command.bytes.data(), command, command_size);
  if (header.type == CommandType::kPresent) {
    const auto& present = *static_cast<const PresentCommand*>(command);
    auto fetch_matches = [&present](const NativeTextureResource& resource) {
      return resource.fetch.dword_0 == present.frontbuffer_fetch[0] &&
             resource.fetch.dword_1 == present.frontbuffer_fetch[1] &&
             resource.fetch.dword_2 == present.frontbuffer_fetch[2] &&
             resource.fetch.dword_3 == present.frontbuffer_fetch[3] &&
             resource.fetch.dword_4 == present.frontbuffer_fetch[4] &&
             resource.fetch.dword_5 == present.frontbuffer_fetch[5];
    };
    std::lock_guard lock(texture_resource_mutex_);
    auto direct = texture_resources_.find(present.frontbuffer_texture);
    auto direct_resource =
        direct != texture_resources_.end() ? direct->second : nullptr;
    const bool direct_dirty = dirty_texture_handles_.contains(present.frontbuffer_texture);
    const bool direct_fetch_match = direct_resource && fetch_matches(*direct_resource);
    const char* selection = "none";
    if (direct_resource &&
        !direct_dirty && (direct_resource->gpu_produced || direct_fetch_match)) {
      native_command.present_source = std::move(direct_resource);
      selection = "direct";
    } else {
      for (const auto& [handle, resource] : texture_resources_) {
        (void)handle;
        if (resource && fetch_matches(*resource) &&
            (!native_command.present_source ||
             resource->generation > native_command.present_source->generation)) {
          native_command.present_source = resource;
        }
      }
      if (native_command.present_source) {
        selection = "fetch-fallback";
      }
    }
    if (ShouldLogDiagnosticFrame(present.submitted_frame)) {
      std::fprintf(
          stderr,
          "[PresentSource] frame=%u request=%08X selection=%s direct-dirty=%u "
          "direct-fetch-match=%u selected=%08X@%llu gpu-produced=%u size=%ux%u\n",
          present.submitted_frame, present.frontbuffer_texture, selection,
          unsigned(direct_dirty), unsigned(direct_fetch_match),
          native_command.present_source ? native_command.present_source->handle : 0,
          static_cast<unsigned long long>(
              native_command.present_source ? native_command.present_source->generation : 0),
          unsigned(native_command.present_source
                       ? native_command.present_source->gpu_produced
                       : false),
          native_command.present_source ? native_command.present_source->info.width + 1 : 0,
          native_command.present_source ? native_command.present_source->info.height + 1 : 0);
      std::fflush(stderr);
    }
  }
  if (header.type == CommandType::kResolve) {
    native_command.resolve_destination =
        CreateResolvedTextureResource(*static_cast<const ResolveCommand*>(command));
    if (!native_command.resolve_destination) {
      return reject(header.type, "resolve-destination-resource");
    }
  }
  if (header.type == CommandType::kDepthSurfaceHandoff) {
    const auto& handoff = *static_cast<const DepthSurfaceHandoffCommand*>(command);
    std::lock_guard lock(texture_resource_mutex_);
    const auto source = texture_resources_.find(handoff.source_texture);
    if (source == texture_resources_.end() || !source->second ||
        !source->second->gpu_produced) {
      return reject(header.type, "depth-handoff-source-texture");
    }
    native_command.depth_handoff_source = source->second;
  }

  const bool draw_command = header.type == CommandType::kDrawPrimitive ||
                            header.type == CommandType::kDrawPrimitiveUp ||
                            header.type == CommandType::kDrawIndexedPrimitive;
  if (draw_command || header.type == CommandType::kClear) {
    const uint32_t device = CommandDevice(header.type, command);
    const uint8_t* device_memory = memory_->TranslateVirtual<const uint8_t*>(device);
    if (!device_memory) {
      return reject(header.type, "device-snapshot-translation");
    }
    native_command.device_snapshot.resize(kGuestDeviceSize);
    std::memcpy(native_command.device_snapshot.data(), device_memory, kGuestDeviceSize);
    if (draw_command) {
      native_command.fixed_function_state =
          DecodeFixedFunctionState(native_command.device_snapshot);
      native_command.vertex_constants_hash =
          XXH3_64bits(native_command.device_snapshot.data() + kVertexConstantsOffset,
                      kVertexConstantsSize);
      native_command.pixel_constants_hash =
          XXH3_64bits(native_command.device_snapshot.data() + kPixelConstantsOffset,
                      kPixelConstantsSize);
    }
    for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
      native_command.snapshot_render_targets[index] = CaptureSurfaceDescriptor(
          LoadGuestWord(native_command.device_snapshot,
                        kRenderTargetOffset + index * sizeof(uint32_t)));
    }
    native_command.snapshot_depth_stencil = CaptureSurfaceDescriptor(
        LoadGuestWord(native_command.device_snapshot, kDepthStencilOffset));
  }

  if (header.type == CommandType::kDrawPrimitiveUp) {
    const auto& draw = *static_cast<const DrawPrimitiveUpCommand*>(command);
    if (draw.vertex_data_size > kMaximumUpPayloadSize ||
        (draw.vertex_data_size && !draw.vertex_data)) {
      return reject(header.type, "up-payload-range");
    }
    if (draw.vertex_data_size) {
      const uint8_t* vertex_data = memory_->TranslateVirtual<const uint8_t*>(draw.vertex_data);
      if (!vertex_data) {
        return reject(header.type, "up-payload-translation");
      }
      native_command.payload.resize(draw.vertex_data_size);
      std::memcpy(native_command.payload.data(), vertex_data, draw.vertex_data_size);
    }
  } else if (header.type == CommandType::kDrawPrimitive ||
             header.type == CommandType::kDrawIndexedPrimitive) {
    for (uint32_t stream = 0; stream < kVertexStreamCount; ++stream) {
      const uint32_t handle = LoadGuestWord(native_command.device_snapshot,
                                            kStreamBufferBase + stream * sizeof(uint32_t));
      if (handle) {
        native_command.vertex_buffers[stream] = CaptureBufferResource(handle);
      }
    }
    if (header.type == CommandType::kDrawIndexedPrimitive) {
      const uint32_t handle = LoadGuestWord(native_command.device_snapshot, kIndexBufferOffset);
      if (handle) {
        native_command.index_buffer = CaptureBufferResource(handle);
      }
    }
  }

  if (header.type == CommandType::kDrawPrimitive || header.type == CommandType::kDrawPrimitiveUp ||
      header.type == CommandType::kDrawIndexedPrimitive) {
    for (uint32_t stage = 0; stage < kShaderTextureCount; ++stage) {
      xenos::xe_gpu_texture_fetch_t& fetch = native_command.texture_fetches[stage];
      const size_t fetch_offset = kTextureFetchBase + stage * kTextureFetchSize;
      fetch.dword_0 = LoadGuestWord(native_command.device_snapshot, fetch_offset);
      fetch.dword_1 =
          LoadGuestWord(native_command.device_snapshot, fetch_offset + sizeof(uint32_t));
      fetch.dword_2 =
          LoadGuestWord(native_command.device_snapshot, fetch_offset + sizeof(uint32_t) * 2);
      fetch.dword_3 =
          LoadGuestWord(native_command.device_snapshot, fetch_offset + sizeof(uint32_t) * 3);
      fetch.dword_4 =
          LoadGuestWord(native_command.device_snapshot, fetch_offset + sizeof(uint32_t) * 4);
      fetch.dword_5 =
          LoadGuestWord(native_command.device_snapshot, fetch_offset + sizeof(uint32_t) * 5);
      const uint32_t handle = LoadGuestWord(native_command.device_snapshot,
                                            kTextureHandleBase + stage * sizeof(uint32_t));
      if (handle) {
        native_command.textures[stage] =
            CaptureTextureResource(handle, native_command.device_snapshot, stage);
      }
    }
  }

  return true;
}

std::shared_ptr<const Gta4NativeGraphicsSystem::NativeTextureResource>
Gta4NativeGraphicsSystem::CreateResolvedTextureResource(const ResolveCommand& command) {
  xenos::xe_gpu_texture_fetch_t fetch{};
  fetch.dword_0 = command.destination_fetch[0];
  fetch.dword_1 = command.destination_fetch[1];
  fetch.dword_2 = command.destination_fetch[2];
  fetch.dword_3 = command.destination_fetch[3];
  fetch.dword_4 = command.destination_fetch[4];
  fetch.dword_5 = command.destination_fetch[5];

  TextureInfo info{};
  auto reject = [&command, &fetch,
                 &info](const char* reason) -> std::shared_ptr<const NativeTextureResource> {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    if (count <= 64 || !(count % 1024)) {
      REXLOG_WARN(
          "gta4-native-diag: resolve texture reject #{} reason={} destination={:08X} "
          "flags={:08X} source={:08X} fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
          "dimension={} format={} base={:08X} tiled={} stacked={} size={}x{}",
          count, reason, command.destination_texture, command.flags, command.source.handle,
          fetch.dword_0, fetch.dword_1, fetch.dword_2, fetch.dword_3, fetch.dword_4, fetch.dword_5,
          uint32_t(info.dimension), uint32_t(info.format), info.memory.base_address, info.is_tiled,
          info.is_stacked, info.width + 1, info.height + 1);
    }
    return nullptr;
  };
  if (!TextureInfo::Prepare(fetch, &info)) {
    return reject("prepare");
  }
  if (info.dimension != xenos::DataDimension::k2DOrStacked &&
      info.dimension != xenos::DataDimension::kCube) {
    return reject("dimension");
  }
  if (info.is_stacked) {
    return reject("stacked");
  }
  if (ConvertTextureFormat(info.format) == VK_FORMAT_UNDEFINED) {
    return reject("format");
  }
  if (!info.memory.base_address) {
    return reject("base-address");
  }

  {
    std::lock_guard lock(texture_resource_mutex_);
    auto existing = texture_resources_.find(command.destination_texture);
    auto existing_resource =
        existing != texture_resources_.end() ? existing->second : nullptr;
    if (existing_resource && existing_resource->gpu_produced &&
        !dirty_texture_handles_.contains(command.destination_texture)) {
      const NativeTextureResource& resource = *existing_resource;
      if (resource.fetch.dword_0 == fetch.dword_0 && resource.fetch.dword_1 == fetch.dword_1 &&
          resource.fetch.dword_2 == fetch.dword_2 && resource.fetch.dword_3 == fetch.dword_3 &&
          resource.fetch.dword_4 == fetch.dword_4 && resource.fetch.dword_5 == fetch.dword_5) {
        return existing_resource;
      }
    }
  }

  auto resource = std::make_shared<NativeTextureResource>();
  resource->handle = command.destination_texture;
  resource->fetch = fetch;
  resource->info = info;
  resource->gpu_produced = true;
  std::lock_guard lock(texture_resource_mutex_);
  resource->generation = next_texture_generation_++;
  texture_resources_[command.destination_texture] = resource;
  dirty_texture_handles_.erase(command.destination_texture);
  return resource;
}

std::shared_ptr<const Gta4NativeGraphicsSystem::NativeBufferResource>
Gta4NativeGraphicsSystem::CaptureBufferResource(uint32_t handle) {
  auto reject = [handle](const char* reason, uint32_t flags, uint32_t data_address,
                         uint32_t data_size) {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    if (count <= 64 || !(count % 1024)) {
      REXLOG_WARN(
          "gta4-native-diag: buffer capture reject #{} reason={} handle={:08X} "
          "flags={:08X} address={:08X} size={}",
          count, reason, handle, flags, data_address, data_size);
    }
    return std::shared_ptr<const NativeBufferResource>{};
  };

  const uint8_t* object = memory_->TranslateVirtual<const uint8_t*>(handle);
  if (!object) {
    return reject("object-translation", 0, 0, 0);
  }

  uint32_t flags;
  uint32_t data_address;
  uint32_t data_size;
  std::memcpy(&flags, object, sizeof(flags));
  std::memcpy(&data_address, object + kResourceDataOffset, sizeof(data_address));
  std::memcpy(&data_size, object + kResourceSizeOffset, sizeof(data_size));
  flags = __builtin_bswap32(flags);
  data_address = __builtin_bswap32(data_address) & kResourceAddressMask;
  data_size = __builtin_bswap32(data_size) & kResourceSizeMask;
  if (!data_address || !data_size || data_size > kMaximumResourcePayloadSize) {
    return reject("metadata", flags, data_address, data_size);
  }
  const uint8_t* data = memory_->TranslateVirtual<const uint8_t*>(data_address);
  if (!data) {
    return reject("payload-translation", flags, data_address, data_size);
  }

  std::vector<uint8_t> payload(data, data + data_size);
  const uint64_t content_hash = XXH3_64bits(payload.data(), payload.size());
  std::lock_guard lock(buffer_resource_mutex_);
  auto existing = buffer_resources_.find(handle);
  if (existing != buffer_resources_.end()) {
    const auto& resource = existing->second;
    if (resource && resource->flags == flags && resource->guest_address == data_address &&
        resource->guest_size == data_size && resource->content_hash == content_hash &&
        resource->payload == payload) {
      return resource;
    }
  }

  auto resource = std::make_shared<NativeBufferResource>();
  resource->handle = handle;
  resource->flags = flags;
  resource->guest_address = data_address;
  resource->guest_size = data_size;
  resource->content_hash = content_hash;
  resource->generation = next_buffer_generation_++;
  resource->payload = std::move(payload);
  buffer_resources_[handle] = resource;
  return resource;
}

SurfaceDescriptor Gta4NativeGraphicsSystem::CaptureSurfaceDescriptor(uint32_t handle) const {
  SurfaceDescriptor descriptor;
  descriptor.handle = handle;
  if (!handle) {
    return descriptor;
  }
  const uint8_t* object = memory_->TranslateVirtual<const uint8_t*>(handle);
  if (!object) {
    REXLOG_WARN("gta4-native-surface: unable to translate snapshot surface {:08X}", handle);
    return descriptor;
  }
  auto load_word = [object](size_t offset) {
    uint32_t value;
    std::memcpy(&value, object + offset, sizeof(value));
    return __builtin_bswap32(value);
  };
  descriptor.flags = load_word(0);
  descriptor.base = load_word(24);
  descriptor.address = load_word(28);
  descriptor.packed_dimensions = load_word(36);
  descriptor.format = load_word(40);
  descriptor.width = (std::rotl(descriptor.packed_dimensions, 14) & 0x3FFF) + 1;
  descriptor.height = (std::rotl(descriptor.packed_dimensions, 29) & 0x7FFF) + 1;
  descriptor.sample_type = DecodeSurfaceSampleType(descriptor.base);
  return descriptor;
}

std::shared_ptr<const Gta4NativeGraphicsSystem::NativeTextureResource>
Gta4NativeGraphicsSystem::CaptureTextureResource(uint32_t handle,
                                                 const std::vector<uint8_t>& device_snapshot,
                                                 uint32_t stage) {
  if (stage >= kShaderTextureCount) {
    return nullptr;
  }

  xenos::xe_gpu_texture_fetch_t fetch{};
  const size_t fetch_offset = kTextureFetchBase + stage * kTextureFetchSize;
  fetch.dword_0 = LoadGuestWord(device_snapshot, fetch_offset);
  fetch.dword_1 = LoadGuestWord(device_snapshot, fetch_offset + sizeof(uint32_t));
  fetch.dword_2 = LoadGuestWord(device_snapshot, fetch_offset + sizeof(uint32_t) * 2);
  fetch.dword_3 = LoadGuestWord(device_snapshot, fetch_offset + sizeof(uint32_t) * 3);
  fetch.dword_4 = LoadGuestWord(device_snapshot, fetch_offset + sizeof(uint32_t) * 4);
  fetch.dword_5 = LoadGuestWord(device_snapshot, fetch_offset + sizeof(uint32_t) * 5);
  TextureInfo info{};
  bool cache_entry_present = false;
  bool cache_entry_dirty = false;
  bool cache_fetch_matches = false;
  bool cache_gpu_image_matches = false;
  bool cache_font_identity_matches = false;
  bool cache_gpu_produced = false;
  uint64_t cache_generation = 0;
  uint32_t cache_width = 0;
  uint32_t cache_height = 0;
  uint32_t cache_format = 0;
  uint32_t rejected_mip = UINT32_MAX;
  uint32_t rejected_layer = UINT32_MAX;
  uint64_t rejected_subresource_payload_size = 0;
  uint64_t rejected_payload_offset = 0;
  auto reject = [handle, stage, &fetch, &info, &cache_entry_present, &cache_entry_dirty,
                 &cache_fetch_matches, &cache_gpu_image_matches, &cache_font_identity_matches,
                 &cache_gpu_produced, &cache_generation, &cache_width, &cache_height,
                 &cache_format, &rejected_mip, &rejected_layer,
                 &rejected_subresource_payload_size,
                 &rejected_payload_offset](const char* reason)
      -> std::shared_ptr<const NativeTextureResource> {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    if (count <= 64 || !(count % 4096)) {
      REXLOG_WARN(
          "gta4-native-diag: texture capture reject #{} reason={} stage={} handle={:08X} "
          "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} type={} dimension={} "
          "format={} base={:08X} tiled={} stacked={} pitch={} size={}x{} mips={}-{} "
          "reject-mip={} layer={} subresource-bytes={} payload-offset={} limit={} "
          "cache[present={},dirty={},generation={},gpu={},fetch-match={},image-match={},"
          "font-match={},size={}x{},format={}]",
          count, reason, stage, handle, fetch.dword_0, fetch.dword_1, fetch.dword_2, fetch.dword_3,
          fetch.dword_4, fetch.dword_5, uint32_t(fetch.type), uint32_t(info.dimension),
          uint32_t(info.format), info.memory.base_address, info.is_tiled, info.is_stacked,
          info.pitch, info.width + 1, info.height + 1, info.mip_min_level, info.mip_max_level,
          rejected_mip, rejected_layer, rejected_subresource_payload_size,
          rejected_payload_offset, uint64_t(kMaximumResourcePayloadSize), cache_entry_present,
          cache_entry_dirty, cache_generation, cache_gpu_produced, cache_fetch_matches,
          cache_gpu_image_matches, cache_font_identity_matches, cache_width, cache_height,
          cache_format);
    }
    if (std::string_view(reason) == "payload-size") {
      static std::mutex cause_mutex;
      static std::set<std::pair<uint32_t, uint32_t>> logged_causes;
      std::lock_guard lock(cause_mutex);
      if (logged_causes.emplace(handle, stage).second) {
        REXLOG_ERROR(
            "gta4-native-cause: point=texture-payload-rejected stage={} handle={:08X} "
            "format={} size={}x{} mips={}-{} reject-mip={} layer={} "
            "subresource-bytes={} payload-offset={} limit={} "
            "cache[present={},dirty={},generation={},gpu={},fetch-match={},image-match={},"
            "font-match={},size={}x{},format={}]",
            stage, handle, uint32_t(info.format), info.width + 1, info.height + 1,
            info.mip_min_level, info.mip_max_level, rejected_mip, rejected_layer,
            rejected_subresource_payload_size, rejected_payload_offset,
            uint64_t(kMaximumResourcePayloadSize), cache_entry_present, cache_entry_dirty,
            cache_generation, cache_gpu_produced, cache_fetch_matches, cache_gpu_image_matches,
            cache_font_identity_matches, cache_width, cache_height, cache_format);
      }
    }
    return nullptr;
  };
  if (fetch.type != xenos::FetchConstantType::kTexture) {
    return reject("fetch-type");
  }
  if (!TextureInfo::Prepare(fetch, &info)) {
    return reject("prepare");
  }

  uint32_t vector_font_id = 0;
  {
    std::lock_guard lock(texture_resource_mutex_);
    const auto font = vector_font_ids_.find(handle);
    if (font != vector_font_ids_.end()) {
      vector_font_id = font->second;
    }
  }
  const bool stock_font_shape = info.format == xenos::TextureFormat::k_DXT4_5 &&
                                info.width + 1 == kStockFontAtlasExtent &&
                                info.height + 1 == kStockFontAtlasExtent &&
                                info.mip_min_level == 0 && info.mip_max_level == 0;
  std::optional<size_t> vector_font_index;
  if (REXCVAR_GET(gta4_native_vector_fonts) && stock_font_shape) {
    switch (vector_font_id) {
      case 1:
        vector_font_index = 0;
        break;
      case 2:
        vector_font_index = 1;
        break;
      case 3:
        vector_font_index = 2;
        break;
      default:
        break;
    }
  }
  const bool vector_font_candidate = vector_font_index.has_value();
  if (vector_font_id && REXCVAR_GET(gta4_trace_vector_fonts)) {
    static std::atomic<uint64_t> font_capture_trace_count{0};
    const uint64_t trace = ++font_capture_trace_count;
    if (trace <= 96) {
      REXLOG_INFO(
          "gta4-native-font-debug: capture #{} stage={} font={} handle={:08X} "
          "enabled={} stock-shape={} atlas-index={} replacement-candidate={} "
          "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
          "decoded-format={} size={}x{} pitch={} tiled={} stacked={} endian={} base={:08X} "
          "mips={}-{}",
          trace, stage, vector_font_id, handle, REXCVAR_GET(gta4_native_vector_fonts),
          stock_font_shape, vector_font_index ? int64_t(*vector_font_index) : int64_t(-1),
          vector_font_candidate, fetch.dword_0, fetch.dword_1, fetch.dword_2, fetch.dword_3,
          fetch.dword_4, fetch.dword_5, uint32_t(info.format), info.width + 1, info.height + 1,
          info.pitch, info.is_tiled, info.is_stacked, uint32_t(info.endianness),
          info.memory.base_address, info.mip_min_level, info.mip_max_level);
    }
  }

  {
    std::lock_guard lock(texture_resource_mutex_);
    auto existing = texture_resources_.find(handle);
    cache_entry_dirty = dirty_texture_handles_.contains(handle);
    if (existing != texture_resources_.end()) {
      const auto& resource = existing->second;
      if (resource) {
        cache_entry_present = true;
        cache_generation = resource->generation;
        cache_gpu_produced = resource->gpu_produced;
        cache_width = resource->info.width + 1;
        cache_height = resource->info.height + 1;
        cache_format = uint32_t(resource->info.format);
        cache_fetch_matches =
            resource->fetch.dword_0 == fetch.dword_0 &&
            resource->fetch.dword_1 == fetch.dword_1 &&
            resource->fetch.dword_2 == fetch.dword_2 &&
            resource->fetch.dword_3 == fetch.dword_3 &&
            resource->fetch.dword_4 == fetch.dword_4 &&
            resource->fetch.dword_5 == fetch.dword_5;
        cache_gpu_image_matches =
            resource->gpu_produced && resource->info.dimension == info.dimension &&
            resource->info.format == info.format && resource->info.width == info.width &&
            resource->info.height == info.height && resource->info.depth == info.depth;
        cache_font_identity_matches = resource->vector_font_id == vector_font_id &&
                                      resource->vector_font_replacement ==
                                          vector_font_candidate;
        const bool cache_reusable_vector_font =
            vector_font_candidate && resource->vector_font_replacement &&
            !cache_entry_dirty;
        // An Xbox texture-cache invalidation does not turn a render-to-texture
        // resource back into CPU-owned guest memory. GTA IV also mutates the
        // sampler and mip-range words of a texture fetch while retaining the
        // same resolved image object. Descriptor identity must follow the GPU
        // image's shape; the draw's current fetch still supplies sampler state.
        if (cache_font_identity_matches &&
            (cache_reusable_vector_font || cache_gpu_image_matches ||
             (cache_fetch_matches && !cache_entry_dirty))) {
          return resource;
        }
      }
    }
  }

  if (info.dimension != xenos::DataDimension::k2DOrStacked &&
      info.dimension != xenos::DataDimension::k3D &&
      info.dimension != xenos::DataDimension::kCube) {
    return reject("dimension");
  }
  if (ConvertTextureFormat(info.format) == VK_FORMAT_UNDEFINED) {
    return reject("format");
  }
  if (!info.memory.base_address) {
    return reject("base-address");
  }

  const FormatInfo* guest_format_info = info.format_info();
  const xenos::TextureFormat base_format = GetBaseFormat(info.format);
  const FormatInfo* host_format_info =
      base_format == xenos::TextureFormat::k_DXT3A
          ? FormatInfo::Get(uint32_t(xenos::TextureFormat::k_DXT2_3))
      : base_format == xenos::TextureFormat::k_CTX1 ||
              base_format == xenos::TextureFormat::k_DXN
          ? FormatInfo::Get(uint32_t(xenos::TextureFormat::k_8_8))
      : base_format == xenos::TextureFormat::k_DXT5A
          ? FormatInfo::Get(uint32_t(xenos::TextureFormat::k_8))
          : guest_format_info;
  if (!guest_format_info || !host_format_info) {
    return reject("format-info");
  }
  const uint32_t guest_bytes_per_block = guest_format_info->bytes_per_block();
  const uint32_t host_bytes_per_block = host_format_info->bytes_per_block();
  if (!guest_bytes_per_block || !host_bytes_per_block ||
      !rex::is_pow2(guest_bytes_per_block) || !rex::is_pow2(host_bytes_per_block)) {
    return reject("block-size");
  }

  std::vector<uint8_t> payload;
  std::vector<NativeTextureResource::MipLevel> mip_levels;
  const uint32_t guest_bytes_per_block_log2 = rex::log2_floor(guest_bytes_per_block);
  const bool is_3d = info.dimension == xenos::DataDimension::k3D;
  const uint32_t array_layer_count =
      info.dimension == xenos::DataDimension::kCube ? 6
      : info.is_stacked                          ? info.depth + 1
                                                 : 1;
  const texture_util::TextureGuestLayout guest_layout = texture_util::GetGuestTextureLayout(
      info.dimension, info.pitch >> 5, info.width + 1, info.height + 1, info.depth + 1,
      info.is_tiled, info.format, info.has_packed_mips, info.memory.base_address != 0,
      info.mip_max_level);
  for (uint32_t mip = info.mip_min_level; mip <= info.mip_max_level; ++mip) {
    uint32_t mip_width = 0;
    uint32_t mip_height = 0;
    info.GetMipSize(mip, &mip_width, &mip_height);
    const uint32_t mip_depth =
        is_3d ? std::max((info.depth + 1) >> mip, uint32_t(1)) : uint32_t(1);
    const TextureExtent guest_extent = info.GetMipExtent(mip, true);
    const bool convert_block_to_pixels = base_format == xenos::TextureFormat::k_CTX1 ||
                                         base_format == xenos::TextureFormat::k_DXN ||
                                         base_format == xenos::TextureFormat::k_DXT5A;
    const uint32_t host_storage_width =
        convert_block_to_pixels
            ? guest_extent.block_width * guest_format_info->block_width
            : mip_width;
    const uint32_t host_storage_height =
        convert_block_to_pixels
            ? guest_extent.block_height * guest_format_info->block_height
            : mip_height;
    const TextureExtent host_extent = TextureExtent::Calculate(
        host_format_info, host_storage_width, host_storage_height, mip_depth, false, false);
    // guest_extent includes Xbox pitch/tile padding for source addressing.
    // Ordinary host payloads contain only visible blocks. Block-to-pixel
    // conversions intentionally expand every guest block into host pixels.
    const uint32_t copy_block_width =
        convert_block_to_pixels ? guest_extent.block_width : host_extent.block_width;
    const uint32_t copy_block_height =
        convert_block_to_pixels ? guest_extent.block_height : host_extent.block_height;
    const uint64_t subresource_payload_size =
        uint64_t(host_extent.visible_blocks()) * uint64_t(host_bytes_per_block);
    if (!subresource_payload_size ||
        subresource_payload_size > kMaximumResourcePayloadSize) {
      rejected_mip = mip;
      rejected_subresource_payload_size = subresource_payload_size;
      rejected_payload_offset = payload.size();
      return reject("payload-size");
    }

    uint32_t packed_offset_x = 0;
    uint32_t packed_offset_y = 0;
    const uint32_t mip_address = info.GetMipLocation(mip, &packed_offset_x, &packed_offset_y, true);
    const uint8_t* source = memory_->TranslatePhysical<const uint8_t*>(mip_address);
    if (!source) {
      return reject("source-translation");
    }

    const texture_util::TextureGuestLayout::Level& level_layout =
        mip == 0 ? guest_layout.base : guest_layout.mips[mip];
    const uint32_t subresource_count = is_3d ? 1 : array_layer_count;
    for (uint32_t layer = 0; layer < subresource_count; ++layer) {
      const size_t payload_alignment = std::max<size_t>(4, host_bytes_per_block);
      // The first subresource starts at byte zero. rex::round_up defaults to
      // forcing zero to one alignment unit, which makes an otherwise valid
      // resource whose first mip exactly reaches the payload limit exceed it
      // by payload_alignment bytes.
      const size_t payload_offset =
          rex::round_up(payload.size(), payload_alignment, false);
      if (uint64_t(payload_offset) + subresource_payload_size >
          kMaximumResourcePayloadSize) {
        rejected_mip = mip;
        rejected_layer = layer;
        rejected_subresource_payload_size = subresource_payload_size;
        rejected_payload_offset = payload_offset;
        return reject("payload-size");
      }
      if (!payload_offset &&
          subresource_payload_size == kMaximumResourcePayloadSize) {
        static std::atomic<uint64_t> exact_limit_accept_count{0};
        const uint64_t accept_index = ++exact_limit_accept_count;
        if (accept_index <= 8) {
          REXLOG_INFO(
              "gta4-native-cause: point=texture-exact-limit-accepted index={} "
              "stage={} handle={:08X} format={} size={}x{} mip={} layer={} "
              "subresource-bytes={} payload-offset={} limit={}",
              accept_index, stage, handle, uint32_t(info.format), info.width + 1,
              info.height + 1, mip, layer, subresource_payload_size,
              payload_offset, kMaximumResourcePayloadSize);
        }
      }
      payload.resize(payload_offset + static_cast<size_t>(subresource_payload_size), uint8_t{});
      uint8_t* destination = payload.data() + payload_offset;
      const uint8_t* layer_source = source + size_t(layer) * level_layout.array_slice_stride_bytes;
      for (uint32_t block_z = 0; block_z < mip_depth; ++block_z) {
        for (uint32_t block_y = 0; block_y < copy_block_height; ++block_y) {
          for (uint32_t block_x = 0; block_x < copy_block_width; ++block_x) {
            const uint32_t source_x = packed_offset_x + block_x;
            const uint32_t source_y = packed_offset_y + block_y;
            int32_t source_offset;
            if (info.is_tiled) {
              source_offset = is_3d
                                  ? texture_util::GetTiledOffset3D(
                                        int32_t(source_x), int32_t(source_y), int32_t(block_z),
                                        guest_extent.block_pitch_h, guest_extent.block_pitch_v,
                                        guest_bytes_per_block_log2)
                                  : texture_util::GetTiledOffset2D(
                                        int32_t(source_x), int32_t(source_y),
                                        guest_extent.block_pitch_h,
                                        guest_bytes_per_block_log2);
            } else {
              source_offset = int32_t(
                  ((block_z * guest_extent.block_pitch_v + source_y) *
                       guest_extent.block_pitch_h +
                   source_x) *
                  guest_bytes_per_block);
            }
            if (source_offset < 0) {
              return reject("source-offset");
            }
            const uint8_t* source_block = layer_source + uint32_t(source_offset);
            const size_t host_slice_offset =
                size_t(block_z) * host_extent.block_height * host_extent.block_pitch_h *
                host_bytes_per_block;
            if (convert_block_to_pixels) {
              const size_t destination_offset =
                  host_slice_offset +
                  size_t(block_y * guest_format_info->block_height) *
                      host_extent.block_pitch_h * host_bytes_per_block +
                  size_t(block_x * guest_format_info->block_width) * host_bytes_per_block;
              const size_t destination_pitch =
                  size_t(host_extent.block_pitch_h) * host_bytes_per_block;
              if (base_format == xenos::TextureFormat::k_CTX1) {
                texture_conversion::ConvertTexelCTX1ToR8G8(
                    info.endianness, destination + destination_offset, source_block,
                    destination_pitch);
              } else if (base_format == xenos::TextureFormat::k_DXN) {
                texture_conversion::ConvertTexelDXNToR8G8(
                    info.endianness, destination + destination_offset, source_block,
                    destination_pitch);
              } else {
                texture_conversion::ConvertTexelDXT5AToR8(
                    info.endianness, destination + destination_offset, source_block,
                    destination_pitch);
              }
            } else if (base_format == xenos::TextureFormat::k_DXT3A) {
              const size_t destination_offset =
                  host_slice_offset +
                  size_t(block_y * host_extent.block_pitch_h + block_x) *
                      host_bytes_per_block;
              texture_conversion::ConvertTexelDXT3AToDXT3(
                  info.endianness, destination + destination_offset, source_block,
                  host_bytes_per_block);
            } else {
              const size_t destination_offset =
                  host_slice_offset +
                  size_t(block_y * host_extent.block_pitch_h + block_x) *
                      host_bytes_per_block;
              texture_conversion::CopySwapBlock(info.endianness,
                                                destination + destination_offset,
                                                source_block, host_bytes_per_block);
            }
          }
        }
      }

      NativeTextureResource::MipLevel mip_level{};
      mip_level.level = mip;
      mip_level.width = mip_width;
      mip_level.height = mip_height;
      mip_level.depth = mip_depth;
      mip_level.base_array_layer = layer;
      mip_level.buffer_row_length = host_extent.block_pitch_h * host_format_info->block_width;
      mip_level.buffer_image_height = host_extent.block_height * host_format_info->block_height;
      mip_level.payload_offset = payload_offset;
      mip_level.payload_size = static_cast<size_t>(subresource_payload_size);
      mip_levels.push_back(mip_level);
    }
  }

  const uint64_t content_hash = XXH3_64bits(payload.data(), payload.size());
  const size_t stock_identity_size =
      std::min(payload.size(), kStockFontIdentityPayloadSize);
  const uint64_t stock_identity_hash =
      stock_identity_size ? XXH3_64bits(payload.data(), stock_identity_size) : 0;
  VectorFontSet vector_font_set = VectorFontSet::kGta4;
  const VectorFontAtlas* vector_font = nullptr;
  if (vector_font_index) {
    vector_font_set = SelectVectorFontSet(*vector_font_index, stock_identity_hash);
    vector_font = FindVectorFontAtlas(vector_font_set, *vector_font_index);
    if (REXCVAR_GET(gta4_trace_vector_fonts)) {
      REXLOG_INFO(
          "gta4-native-font-debug: identify font={} atlas-index={} set={} "
          "identity-bytes={} identity-hash={:016X} full-bytes={} full-hash={:016X} "
          "atlas-loaded={}",
          vector_font_id, *vector_font_index, VectorFontSetName(vector_font_set),
          stock_identity_size, stock_identity_hash, payload.size(), content_hash,
          vector_font != nullptr);
    }
  }
  const bool wants_vector_font = vector_font != nullptr;
  std::lock_guard lock(texture_resource_mutex_);
  auto existing = texture_resources_.find(handle);
  if (existing != texture_resources_.end()) {
    const auto& resource = existing->second;
    if (resource && resource->fetch.dword_0 == fetch.dword_0 &&
        resource->fetch.dword_1 == fetch.dword_1 &&
        resource->fetch.dword_2 == fetch.dword_2 && resource->fetch.dword_3 == fetch.dword_3 &&
        resource->fetch.dword_4 == fetch.dword_4 && resource->fetch.dword_5 == fetch.dword_5 &&
        resource->content_hash == content_hash &&
        resource->vector_font_id == vector_font_id &&
        resource->vector_font_replacement == wants_vector_font &&
        (resource->vector_font_replacement || resource->payload == payload)) {
      return resource;
    }
  }

  auto resource = std::make_shared<NativeTextureResource>();
  resource->handle = handle;
  resource->content_hash = content_hash;
  resource->generation = next_texture_generation_++;
  resource->fetch = fetch;
  resource->info = info;
  resource->vector_font_id = vector_font_id;
  if (vector_font) {
    const TextureInfo stock_info = resource->info;
    resource->info.format = xenos::TextureFormat::k_8;
    resource->info.width = kVectorFontAtlasExtentMinusOne;
    resource->info.height = kVectorFontAtlasExtentMinusOne;
    resource->info.depth = 1;
    resource->info.pitch = kVectorFontAtlasExtent;
    resource->info.mip_min_level = 0;
    resource->info.mip_max_level = kVectorFontMipLevelCount - 1;
    resource->info.is_stacked = false;
    resource->info.is_tiled = false;
    resource->info.has_packed_mips = false;

    // Alpha from FreeType is a coverage map, so mip texels must contain the
    // arithmetic mean coverage of their four source texels. This is the
    // prefilter required for stable minification; bilinear sampling of only the
    // 2048x2048 base level aliases when a glyph is drawn near its stock size.
    std::vector<uint8_t> mip_alpha = vector_font->alpha;
    uint32_t mip_width = kVectorFontAtlasExtent;
    uint32_t mip_height = kVectorFontAtlasExtent;
    resource->payload.clear();
    resource->mip_levels.clear();
    for (uint32_t level = 0; level < kVectorFontMipLevelCount; ++level) {
      NativeTextureResource::MipLevel replacement_mip{};
      replacement_mip.level = level;
      replacement_mip.width = mip_width;
      replacement_mip.height = mip_height;
      replacement_mip.buffer_row_length = mip_width;
      replacement_mip.buffer_image_height = mip_height;
      replacement_mip.payload_offset = resource->payload.size();
      replacement_mip.payload_size = mip_alpha.size();
      resource->payload.insert(resource->payload.end(), mip_alpha.begin(), mip_alpha.end());
      resource->mip_levels.push_back(replacement_mip);

      if (level + 1 == kVectorFontMipLevelCount) {
        break;
      }
      const uint32_t next_width = std::max(mip_width >> 1, uint32_t(1));
      const uint32_t next_height = std::max(mip_height >> 1, uint32_t(1));
      std::vector<uint8_t> next_alpha(size_t(next_width) * size_t(next_height));
      for (uint32_t y = 0; y < next_height; ++y) {
        const size_t source_row_0 = size_t(y * 2) * mip_width;
        const size_t source_row_1 = source_row_0 + mip_width;
        const size_t destination_row = size_t(y) * next_width;
        for (uint32_t x = 0; x < next_width; ++x) {
          const size_t source_x = size_t(x) * 2;
          const uint32_t coverage =
              uint32_t(mip_alpha[source_row_0 + source_x]) +
              uint32_t(mip_alpha[source_row_0 + source_x + 1]) +
              uint32_t(mip_alpha[source_row_1 + source_x]) +
              uint32_t(mip_alpha[source_row_1 + source_x + 1]);
          next_alpha[destination_row + x] = uint8_t((coverage + 2) / 4);
        }
      }
      mip_alpha = std::move(next_alpha);
      mip_width = next_width;
      mip_height = next_height;
    }
    const uint64_t replacement_hash =
        XXH3_64bits(resource->payload.data(), resource->payload.size());
    resource->vector_font_replacement = true;
    REXLOG_INFO(
        "gta4-native-font-debug: substitute font={} handle={:08X} generation={} "
        "set={} identity-bytes={} identity-hash={:016X} "
        "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
        "stock-format={} stock-size={}x{} stock-pitch={} stock-tiled={} stock-endian={} "
        "stock-base={:08X} stock-payload={} stock-hash={:016X} "
        "replacement-format={} replacement-size={}x{} replacement-pitch={} "
        "replacement-mips={} replacement-payload={} replacement-hash={:016X}",
        vector_font_id, handle, resource->generation, VectorFontSetName(vector_font_set),
        stock_identity_size, stock_identity_hash, fetch.dword_0, fetch.dword_1,
        fetch.dword_2, fetch.dword_3, fetch.dword_4, fetch.dword_5,
        uint32_t(stock_info.format), stock_info.width + 1, stock_info.height + 1,
        stock_info.pitch, stock_info.is_tiled, uint32_t(stock_info.endianness),
        stock_info.memory.base_address, payload.size(), content_hash,
        uint32_t(resource->info.format), resource->info.width + 1, resource->info.height + 1,
        resource->info.pitch, resource->mip_levels.size(), resource->payload.size(),
        replacement_hash);
  } else {
    resource->payload = std::move(payload);
    resource->mip_levels = std::move(mip_levels);
  }
  texture_resources_[handle] = resource;
  dirty_texture_handles_.erase(handle);
  return resource;
}

void Gta4NativeGraphicsSystem::StartRenderWorker() {
  if (render_worker_running_.exchange(true)) {
    return;
  }
  render_worker_ = std::thread([this]() { RenderWorkerMain(); });
}

void Gta4NativeGraphicsSystem::RenderWorkerMain() {
  std::vector<std::pair<RenderPhase, uint32_t>> render_phase_stack;
  uint64_t startup_texture_lock_count = 0;
  bool startup_present_follows_texture_lock_flush = false;
  while (true) {
    NativeCommand command;
    {
      std::unique_lock lock(render_mutex_);
      render_condition_.wait(
          lock, [this]() { return !render_worker_running_ || !render_queue_.empty(); });
      if (render_queue_.empty()) {
        if (!render_worker_running_) {
          break;
        }
        continue;
      }
      command = std::move(render_queue_.front());
      render_queue_.pop_front();
    }
    render_condition_.notify_all();

    switch (command.type) {
      case CommandType::kDeviceCreated: {
        DeviceCommand device;
        std::memcpy(&device, command.bytes.data(), sizeof(device));
        if (device.mode != 2) {
          pipeline_state_ = std::make_shared<NativePipelineState>();
          current_frame_.clear();
        }
        break;
      }
      case CommandType::kDeviceDestroyed:
        {
          DeviceCommand device{};
          std::memcpy(&device, command.bytes.data(), sizeof(device));
          environmental_data_by_device_.erase(device.device);
        }
        current_frame_.clear();
        render_phase_stack.clear();
        pipeline_state_ = std::make_shared<NativePipelineState>();
        vertex_declarations_.clear();
        {
          std::lock_guard lock(buffer_resource_mutex_);
          buffer_resources_.clear();
        }
        {
          std::lock_guard lock(texture_resource_mutex_);
          texture_resources_.clear();
          dirty_texture_handles_.clear();
        }
        reflection_resources_.clear();
        break;
      case CommandType::kRegisterShader: {
        RegisterShaderCommand register_shader;
        std::memcpy(&register_shader, command.bytes.data(), sizeof(register_shader));
        RegisterShader(register_shader);
        break;
      }
      case CommandType::kRegisterVertexDeclaration: {
        RegisterVertexDeclarationCommand declaration;
        std::memcpy(&declaration, command.bytes.data(), sizeof(declaration));
        RegisterVertexDeclaration(declaration);
        break;
      }
      case CommandType::kSetRenderState:
      case CommandType::kSetPixelShader:
      case CommandType::kSetVertexShader:
      case CommandType::kSetVertexDeclaration:
      case CommandType::kSetTexture:
      case CommandType::kSetDepthStencil:
      case CommandType::kSetRenderTarget:
      case CommandType::kSetVertexStream:
      case CommandType::kSetIndexBuffer:
        ApplyStateCommand(command);
        break;
      case CommandType::kInvalidateTexture:
        break;
      case CommandType::kRegisterReflectionTarget: {
        RegisterReflectionTargetCommand registration{};
        std::memcpy(&registration, command.bytes.data(), sizeof(registration));
        NativeReflectionTarget target{};
        target.family = registration.family;
        target.role = registration.role;
        target.wrapper = registration.wrapper;
        target.logical_width = registration.logical_width;
        target.logical_height = registration.logical_height;
        target.physical_width = registration.physical_width;
        target.physical_height = registration.physical_height;
        target.sample_count_override = registration.sample_count_override;
        reflection_resources_[registration.surface] = target;
        if (registration.texture) {
          reflection_resources_[registration.texture] = target;
        }
        const NativeSurfaceImage* cached_surface = nullptr;
        for (const auto& image : native_surface_images_) {
          if (image->descriptor.handle == registration.surface) {
            cached_surface = image.get();
            break;
          }
        }
        if (cached_surface) {
          const bool compatible =
              cached_surface->is_reflection &&
              cached_surface->reflection.family == registration.family &&
              cached_surface->reflection.role == registration.role &&
              cached_surface->width == registration.physical_width &&
              cached_surface->height == registration.physical_height;
          REXLOG_WARN(
              "gta4-native-cause: point=reflection-registration-after-surface-create "
              "compatible={} family={} role={} wrapper={:08X} surface={:08X} texture={:08X} "
              "registered-logical={}x{} registered-host={}x{} "
              "cached-reflection={} cached-family={} cached-role={} cached-logical={}x{} "
              "cached-host={}x{} cached-samples={} cached-written={}",
              compatible, uint32_t(registration.family), uint32_t(registration.role),
              registration.wrapper, registration.surface, registration.texture,
              registration.logical_width, registration.logical_height,
              registration.physical_width, registration.physical_height,
              cached_surface->is_reflection, uint32_t(cached_surface->reflection.family),
              uint32_t(cached_surface->reflection.role), cached_surface->logical_width,
              cached_surface->logical_height, cached_surface->width, cached_surface->height,
              uint32_t(cached_surface->samples), cached_surface->ever_written);
        }
        REXLOG_INFO(
            "gta4-native-reflection: host registration family={} role={} logical={}x{} "
            "physical={}x{} sample-override={} wrapper={:08X} surface={:08X} texture={:08X}",
            uint32_t(registration.family), uint32_t(registration.role),
            registration.logical_width, registration.logical_height,
            registration.physical_width, registration.physical_height,
            registration.sample_count_override, registration.wrapper, registration.surface,
            registration.texture);
        break;
      }
      case CommandType::kUpdateEnvironmentalData: {
        UpdateEnvironmentalDataCommand update{};
        std::memcpy(&update, command.bytes.data(), sizeof(update));
        environmental_data_by_device_[update.device] = update.data;
        break;
      }
      case CommandType::kReleaseResource: {
        ReleaseResourceCommand release{};
        std::memcpy(&release, command.bytes.data(), sizeof(release));
        reflection_resources_.erase(release.resource);
        break;
      }
      case CommandType::kTextureLock: {
        TextureLockCommand lock_command;
        std::memcpy(&lock_command, command.bytes.data(), sizeof(lock_command));
        bool succeeded = true;
        const bool flushed_pending_frame = !current_frame_.empty();
        if (REXCVAR_GET(gta4_trace_startup_content) &&
            startup_texture_lock_count < kStartupContentProbeFrameLimit) {
          ++startup_texture_lock_count;
          const NativeTextureResource* first_texture = nullptr;
          const NativeTextureResource* resolve_destination = nullptr;
          for (const NativeCommand& pending : current_frame_) {
            if (!resolve_destination && pending.resolve_destination) {
              resolve_destination = pending.resolve_destination.get();
            }
            if (!first_texture) {
              for (const auto& texture : pending.textures) {
                if (texture) {
                  first_texture = texture.get();
                  break;
                }
              }
            }
          }
          REXLOG_WARN(
              "gta4-startup-texture-lock: index={} texture={:08X} pending={} flush={} "
              "first-texture={:08X}@{} hash={:016X} bytes={} gpu={} "
              "resolve-destination={:08X}@{}",
              startup_texture_lock_count, lock_command.texture, current_frame_.size(),
              flushed_pending_frame, first_texture ? first_texture->handle : 0,
              first_texture ? first_texture->generation : 0,
              first_texture ? first_texture->content_hash : 0,
              first_texture ? first_texture->payload.size() : 0,
              first_texture ? first_texture->gpu_produced : false,
              resolve_destination ? resolve_destination->handle : 0,
              resolve_destination ? resolve_destination->generation : 0);
        }
        if (!current_frame_.empty()) {
          PresentCommand flush_present{};
          succeeded = PublishFrame(flush_present);
        }
        TextureLockResult result{};
        if (succeeded) {
          succeeded = ReadbackTextureToGuest(lock_command, result);
        }
        current_frame_.clear();
        startup_present_follows_texture_lock_flush = flushed_pending_frame;
        if (command.synchronous) {
          std::lock_guard completion_lock(command.synchronous->mutex);
          command.synchronous->result = result;
          command.synchronous->succeeded = succeeded;
          command.synchronous->complete = true;
          command.synchronous->condition.notify_one();
        }
        break;
      }
      case CommandType::kRenderPhaseMarker: {
        RenderPhaseMarkerCommand marker;
        std::memcpy(&marker, command.bytes.data(), sizeof(marker));
        const bool begin = marker.event == RenderPhaseEvent::kBegin;
        bool matched = true;
        if (begin) {
          render_phase_stack.emplace_back(marker.phase, marker.object);
        } else if (!render_phase_stack.empty() &&
                   render_phase_stack.back() == std::pair{marker.phase, marker.object}) {
          render_phase_stack.pop_back();
        } else {
          matched = false;
          render_phase_stack.clear();
        }
        if (REXCVAR_GET(gta4_trace_startup_content)) {
          static std::atomic<uint64_t> phase_marker_count{0};
          const uint64_t marker_index = ++phase_marker_count;
          if (marker_index <= 64 || !(marker_index % 4096) || !matched) {
            REXLOG_INFO(
                "gta4-native-light-phase: marker={} phase={} event={} object={:08X} "
                "caller={:08X} depth={} matched={}",
                marker_index, RenderPhaseName(marker.phase), begin ? "begin" : "end",
                marker.object, marker.caller, render_phase_stack.size(), matched);
          }
        }
        current_frame_.push_back(std::move(command));
        break;
      }
      case CommandType::kPresent: {
        PresentCommand present;
        std::memcpy(&present, command.bytes.data(), sizeof(present));
        const auto environmental = environmental_data_by_device_.find(present.device);
        if (environmental != environmental_data_by_device_.end()) {
          command.environmental_data =
              std::make_shared<const EnvironmentalDataV1>(environmental->second);
        }
        if (REXCVAR_GET(gta4_trace_startup_content) && present.submitted_frame &&
            present.submitted_frame <= kStartupContentProbeFrameLimit &&
            startup_present_follows_texture_lock_flush) {
          REXLOG_WARN(
              "gta4-startup-present-after-texture-lock: frame={} commands={} "
              "frontbuffer={:08X} generation={}",
              present.submitted_frame, current_frame_.size(), present.frontbuffer_texture,
              command.present_source ? command.present_source->generation : 0);
        }
        if (!PublishFrame(present, command.present_source, command.environmental_data)) {
          REXLOG_ERROR("gta4-native: failed to publish frame {}", present.submitted_frame);
        }
        uint32_t* completed_frame =
            memory_->TranslateVirtual<uint32_t*>(present.device + kCompletedFrameOffset);
        if (completed_frame) {
          *completed_frame = __builtin_bswap32(present.submitted_frame);
        }
        current_frame_.clear();
        startup_present_follows_texture_lock_flush = false;
        break;
      }
      case CommandType::kDrawPrimitive:
      case CommandType::kDrawPrimitiveUp:
      case CommandType::kDrawIndexedPrimitive:
      case CommandType::kResolve:
      case CommandType::kClear:
      case CommandType::kDepthSurfaceHandoff:
        command.render_phase = render_phase_stack.empty() ? RenderPhase::kUnknown
                                                          : render_phase_stack.back().first;
        command.render_phase_object =
            render_phase_stack.empty() ? 0 : render_phase_stack.back().second;
        if (command.type == CommandType::kDrawPrimitive ||
            command.type == CommandType::kDrawPrimitiveUp ||
            command.type == CommandType::kDrawIndexedPrimitive ||
            command.type == CommandType::kClear) {
          auto snapshot_state = std::make_shared<NativePipelineState>(*pipeline_state_);
          snapshot_state->render_targets = command.snapshot_render_targets;
          snapshot_state->depth_stencil = command.snapshot_depth_stencil;
          command.pipeline_state = std::move(snapshot_state);
        } else {
          command.pipeline_state = pipeline_state_;
        }
        if (command.render_phase == RenderPhase::kCompositePostFx &&
            (command.type == CommandType::kDrawPrimitive ||
             command.type == CommandType::kDrawPrimitiveUp ||
             command.type == CommandType::kDrawIndexedPrimitive)) {
          const uint32_t device = CommandDevice(command.type, command.bytes.data());
          const auto environmental = environmental_data_by_device_.find(device);
          if (environmental != environmental_data_by_device_.end()) {
            command.environmental_data =
                std::make_shared<const EnvironmentalDataV1>(environmental->second);
          }
        }
        current_frame_.push_back(std::move(command));
        startup_present_follows_texture_lock_flush = false;
        break;
      case CommandType::kQueryDeviceCapabilities:
        REXLOG_ERROR("gta4-native: asynchronous device-capability query rejected");
        break;
    }
  }

  current_frame_.clear();
  {
    std::lock_guard lock(render_mutex_);
    render_queue_.clear();
  }
  DestroyVulkanWorkerObjects();
}

void Gta4NativeGraphicsSystem::ApplyStateCommand(const NativeCommand& command) {
  auto next_state = std::make_shared<NativePipelineState>(*pipeline_state_);
  switch (command.type) {
    case CommandType::kSetRenderState: {
      SetRenderStateCommand state;
      std::memcpy(&state, command.bytes.data(), sizeof(state));
      next_state->render_states[state.state] = state.value;
      break;
    }
    case CommandType::kSetPixelShader:
    case CommandType::kSetVertexShader: {
      SetShaderCommand shader;
      std::memcpy(&shader, command.bytes.data(), sizeof(shader));
      if (command.type == CommandType::kSetPixelShader) {
        next_state->pixel_shader = shader.shader;
        next_state->pixel_shader_resource =
            FindRegisteredShader(shader.shader, ShaderStage::kPixel);
      } else {
        next_state->vertex_shader = shader.shader;
        next_state->vertex_shader_resource =
            FindRegisteredShader(shader.shader, ShaderStage::kVertex);
      }
      break;
    }
    case CommandType::kSetVertexDeclaration: {
      SetVertexDeclarationCommand declaration;
      std::memcpy(&declaration, command.bytes.data(), sizeof(declaration));
      next_state->vertex_declaration = declaration.declaration;
      auto declaration_resource = vertex_declarations_.find(declaration.declaration);
      next_state->vertex_declaration_resource = declaration_resource != vertex_declarations_.end()
                                                    ? declaration_resource->second
                                                    : nullptr;
      break;
    }
    case CommandType::kSetTexture: {
      SetTextureCommand texture;
      std::memcpy(&texture, command.bytes.data(), sizeof(texture));
      next_state->textures[texture.stage] = texture.texture;
      break;
    }
    case CommandType::kSetDepthStencil: {
      SetDepthStencilCommand depth;
      std::memcpy(&depth, command.bytes.data(), sizeof(depth));
      next_state->depth_stencil = depth.surface;
      next_state->depth_stencil_trace_wrapper = depth.trace_wrapper;
      next_state->depth_stencil_trace_caller = depth.trace_caller;
      break;
    }
    case CommandType::kSetRenderTarget: {
      SetRenderTargetCommand target;
      std::memcpy(&target, command.bytes.data(), sizeof(target));
      next_state->render_targets[target.index] = target.surface;
      break;
    }
    case CommandType::kSetVertexStream: {
      SetVertexStreamCommand stream;
      std::memcpy(&stream, command.bytes.data(), sizeof(stream));
      auto& next_stream = next_state->vertex_streams[stream.stream];
      next_stream.buffer = stream.buffer;
      next_stream.offset = stream.offset;
      next_stream.stride = stream.stride;
      next_stream.stride_words = stream.stride_words;
      break;
    }
    case CommandType::kSetIndexBuffer: {
      SetIndexBufferCommand index;
      std::memcpy(&index, command.bytes.data(), sizeof(index));
      next_state->index_buffer = index.buffer;
      break;
    }
    default:
      return;
  }
  ++next_state->version;
  pipeline_state_ = std::move(next_state);
}

void Gta4NativeGraphicsSystem::RegisterVertexDeclaration(
    const RegisterVertexDeclarationCommand& command) {
  auto declaration = std::make_shared<NativeVertexDeclaration>();
  declaration->handle = command.declaration;
  declaration->generation = next_vertex_declaration_generation_++;
  declaration->maximum_stream = command.maximum_stream;
  declaration->elements.assign(command.elements, command.elements + command.element_count);
  declaration->content_hash = XXH3_64bits_withSeed(
      declaration->elements.data(), declaration->elements.size() * sizeof(VertexElement),
      declaration->maximum_stream);
  vertex_declarations_[command.declaration] = std::move(declaration);
}

bool Gta4NativeGraphicsSystem::InitializeShaderCache() {
  if (shader_cache_initialized_) {
    return true;
  }
  if (shader_cache_load_attempted_) {
    return false;
  }
  shader_cache_load_attempted_ = true;

  if (!g_shaderCacheEntryCount || !g_spirvCacheCompressedSize || !g_spirvCacheDecompressedSize) {
    REXLOG_ERROR("gta4-native: Liberty SPIR-V shader cache is empty");
    return false;
  }

  shader_cache_data_.resize(g_spirvCacheDecompressedSize);
  const size_t decompressed_size =
      ZSTD_decompress(shader_cache_data_.data(), shader_cache_data_.size(), g_compressedSpirvCache,
                      g_spirvCacheCompressedSize);
  if (ZSTD_isError(decompressed_size) || decompressed_size != g_spirvCacheDecompressedSize) {
    REXLOG_ERROR(
        "gta4-native: failed to decompress Liberty SPIR-V shader cache: {}",
        ZSTD_isError(decompressed_size) ? ZSTD_getErrorName(decompressed_size) : "size mismatch");
    shader_cache_data_.clear();
    return false;
  }

  shader_cache_initialized_ = true;
  REXLOG_INFO("gta4-native: loaded {} cached shaders ({} SPIR-V bytes)", g_shaderCacheEntryCount,
              shader_cache_data_.size());
  return true;
}

bool Gta4NativeGraphicsSystem::ReflectVertexInputs(
    const std::vector<uint32_t>& spirv, std::vector<NativeVertexInput>& inputs) {
  inputs.clear();
  if (spirv.size() < 5 || spirv[0] != kSpirvMagic || !spirv[3]) {
    return false;
  }

  struct TypeInfo {
    enum class Kind : uint8_t { kUnknown, kFloat, kSignedInteger, kUnsignedInteger, kVector, kPointer };
    Kind kind = Kind::kUnknown;
    uint32_t element_type = 0;
    uint32_t component_count = 1;
  };
  struct VariableInfo {
    uint32_t type = 0;
    uint32_t id = 0;
  };

  const uint32_t id_bound = spirv[3];
  std::vector<TypeInfo> types(id_bound);
  std::vector<uint32_t> locations(id_bound, UINT32_MAX);
  std::vector<bool> built_ins(id_bound, false);
  std::vector<VariableInfo> variables;

  size_t cursor = 5;
  while (cursor < spirv.size()) {
    const uint32_t instruction = spirv[cursor];
    const uint32_t word_count = instruction >> 16;
    const spv::Op opcode = spv::Op(instruction & 0xFFFFu);
    if (!word_count || word_count > spirv.size() - cursor) {
      return false;
    }
    const uint32_t* words = spirv.data() + cursor;
    switch (opcode) {
      case spv::Op::OpTypeFloat:
        if (word_count >= 3 && words[1] < id_bound && words[2] == 32) {
          types[words[1]].kind = TypeInfo::Kind::kFloat;
        }
        break;
      case spv::Op::OpTypeInt:
        if (word_count >= 4 && words[1] < id_bound && words[2] == 32) {
          types[words[1]].kind =
              words[3] ? TypeInfo::Kind::kSignedInteger : TypeInfo::Kind::kUnsignedInteger;
        }
        break;
      case spv::Op::OpTypeVector:
        if (word_count >= 4 && words[1] < id_bound && words[2] < id_bound) {
          TypeInfo& type = types[words[1]];
          type.kind = TypeInfo::Kind::kVector;
          type.element_type = words[2];
          type.component_count = words[3];
        }
        break;
      case spv::Op::OpTypePointer:
        if (word_count >= 4 && words[1] < id_bound && words[3] < id_bound) {
          TypeInfo& type = types[words[1]];
          type.kind = TypeInfo::Kind::kPointer;
          type.element_type = words[3];
        }
        break;
      case spv::Op::OpVariable:
        if (word_count >= 4 && words[1] < id_bound && words[2] < id_bound &&
            words[3] == uint32_t(spv::StorageClass::Input)) {
          variables.push_back({words[1], words[2]});
        }
        break;
      case spv::Op::OpDecorate:
        if (word_count >= 3 && words[1] < id_bound) {
          const spv::Decoration decoration = spv::Decoration(words[2]);
          if (decoration == spv::Decoration::Location && word_count >= 4) {
            locations[words[1]] = words[3];
          } else if (decoration == spv::Decoration::BuiltIn) {
            built_ins[words[1]] = true;
          }
        }
        break;
      default:
        break;
    }
    cursor += word_count;
  }

  for (const VariableInfo& variable : variables) {
    if (built_ins[variable.id] || locations[variable.id] == UINT32_MAX) {
      continue;
    }
    const TypeInfo& pointer = types[variable.type];
    if (pointer.kind != TypeInfo::Kind::kPointer || pointer.element_type >= id_bound) {
      return false;
    }
    const TypeInfo* value_type = &types[pointer.element_type];
    uint32_t component_count = 1;
    if (value_type->kind == TypeInfo::Kind::kVector) {
      component_count = value_type->component_count;
      if (value_type->element_type >= id_bound) {
        return false;
      }
      value_type = &types[value_type->element_type];
    }

    NativeVertexNumericType numeric_type;
    switch (value_type->kind) {
      case TypeInfo::Kind::kFloat:
        numeric_type = NativeVertexNumericType::kFloat;
        break;
      case TypeInfo::Kind::kSignedInteger:
        numeric_type = NativeVertexNumericType::kSignedInteger;
        break;
      case TypeInfo::Kind::kUnsignedInteger:
        numeric_type = NativeVertexNumericType::kUnsignedInteger;
        break;
      default:
        return false;
    }
    if (!component_count || component_count > 4) {
      return false;
    }
    inputs.push_back({locations[variable.id], numeric_type, component_count});
  }

  std::sort(inputs.begin(), inputs.end(),
            [](const NativeVertexInput& left, const NativeVertexInput& right) {
              return left.location < right.location;
            });
  return std::adjacent_find(inputs.begin(), inputs.end(),
                            [](const NativeVertexInput& left, const NativeVertexInput& right) {
                              return left.location == right.location;
                            }) == inputs.end();
}

VkFormat Gta4NativeGraphicsSystem::GetCompatibleVertexFormat(
    uint32_t element_type, NativeVertexNumericType numeric_type) {
  const VkFormat format = ConvertVertexElementFormat(element_type);
  if (numeric_type == NativeVertexNumericType::kFloat) {
    return format;
  }
  if (numeric_type == NativeVertexNumericType::kUnsignedInteger) {
    switch (format) {
      case VK_FORMAT_R8G8B8A8_UINT:
      case VK_FORMAT_R32_UINT:
        return format;
      case VK_FORMAT_B8G8R8A8_UNORM:
      case VK_FORMAT_R8G8B8A8_UNORM:
        return VK_FORMAT_R8G8B8A8_UINT;
      case VK_FORMAT_R16G16_UNORM:
        return VK_FORMAT_R16G16_UINT;
      case VK_FORMAT_R16G16B16A16_UNORM:
        return VK_FORMAT_R16G16B16A16_UINT;
      default:
        return VK_FORMAT_UNDEFINED;
    }
  }
  switch (format) {
    case VK_FORMAT_R16G16_SINT:
    case VK_FORMAT_R16G16B16A16_SINT:
      return format;
    case VK_FORMAT_R16G16_SNORM:
      return VK_FORMAT_R16G16_SINT;
    case VK_FORMAT_R16G16B16A16_SNORM:
      return VK_FORMAT_R16G16B16A16_SINT;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

VkFormat Gta4NativeGraphicsSystem::GetDefaultVertexFormat(
    NativeVertexNumericType numeric_type, uint32_t component_count) {
  static constexpr VkFormat kFloatFormats[] = {
      VK_FORMAT_UNDEFINED, VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT,
      VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT};
  static constexpr VkFormat kSignedFormats[] = {
      VK_FORMAT_UNDEFINED, VK_FORMAT_R32_SINT, VK_FORMAT_R32G32_SINT,
      VK_FORMAT_R32G32B32_SINT, VK_FORMAT_R32G32B32A32_SINT};
  static constexpr VkFormat kUnsignedFormats[] = {
      VK_FORMAT_UNDEFINED, VK_FORMAT_R32_UINT, VK_FORMAT_R32G32_UINT,
      VK_FORMAT_R32G32B32_UINT, VK_FORMAT_R32G32B32A32_UINT};
  if (!component_count || component_count >= std::size(kFloatFormats)) {
    return VK_FORMAT_UNDEFINED;
  }
  switch (numeric_type) {
    case NativeVertexNumericType::kFloat:
      return kFloatFormats[component_count];
    case NativeVertexNumericType::kSignedInteger:
      return kSignedFormats[component_count];
    case NativeVertexNumericType::kUnsignedInteger:
      return kUnsignedFormats[component_count];
  }
  return VK_FORMAT_UNDEFINED;
}

void Gta4NativeGraphicsSystem::RegisterShader(const RegisterShaderCommand& command) {
  static_assert(uint32_t(ShaderStage::kPixel) == kShaderOverrideStagePixel);
  static_assert(uint32_t(ShaderStage::kVertex) == kShaderOverrideStageVertex);

  const uint32_t override_stage = uint32_t(command.stage);
  const auto override_key = std::pair{override_stage, command.hash};
  const ShaderOverrideCacheEntry* override_end =
      g_shaderOverrideEntries + g_shaderOverrideEntryCount;
  const ShaderOverrideCacheEntry* override_entry = std::lower_bound(
      g_shaderOverrideEntries, override_end, override_key,
      [](const ShaderOverrideCacheEntry& candidate, const std::pair<uint32_t, uint64_t>& key) {
        return candidate.stage < key.first ||
               (candidate.stage == key.first && candidate.hash < key.second);
      });
  if (override_entry == override_end || override_entry->stage != override_stage ||
      override_entry->hash != command.hash) {
    override_entry = nullptr;
  }

  const ShaderCacheEntry* cache_entry = nullptr;
  if (!override_entry) {
    if (!InitializeShaderCache()) {
      return;
    }

    const ShaderCacheEntry* entry_begin = g_shaderCacheEntries;
    const ShaderCacheEntry* entry_end = entry_begin + g_shaderCacheEntryCount;
    cache_entry = std::lower_bound(
        entry_begin, entry_end, command.hash,
        [](const ShaderCacheEntry& candidate, uint64_t hash) { return candidate.hash < hash; });
    if (cache_entry == entry_end || cache_entry->hash != command.hash) {
      REXLOG_ERROR("gta4-native: shader cache miss for {} shader {:016X}",
                   command.stage == ShaderStage::kVertex ? "vertex" : "pixel", command.hash);
      return;
    }

    const bool filename_is_vertex = std::strstr(cache_entry->filename, "_vs") != nullptr;
    const bool filename_is_pixel = std::strstr(cache_entry->filename, "_ps") != nullptr;
    if ((command.stage == ShaderStage::kVertex && filename_is_pixel && !filename_is_vertex) ||
        (command.stage == ShaderStage::kPixel && filename_is_vertex && !filename_is_pixel)) {
      REXLOG_ERROR("gta4-native: shader stage mismatch for cache entry {}",
                   cache_entry->filename);
      return;
    }
  }

  auto& resources_by_hash =
      command.stage == ShaderStage::kVertex ? vertex_shaders_by_hash_ : pixel_shaders_by_hash_;
  NativeShader* shader = nullptr;
  auto existing = resources_by_hash.find(command.hash);
  if (existing != resources_by_hash.end()) {
    shader = existing->second;
  } else {
    std::vector<uint32_t> early_spirv;
    std::vector<uint32_t> late_spirv;
    size_t early_spirv_size = 0;
    size_t late_spirv_size = 0;
    const char* shader_filename = nullptr;
    uint32_t specialization_constants_mask = 0;
    if (override_entry) {
      early_spirv_size = override_entry->spirv_size;
      late_spirv_size = override_entry->late_spirv_size;
      shader_filename = override_entry->filename;
      specialization_constants_mask = override_entry->specialization_constants_mask;
      if (!override_entry->spirv || !early_spirv_size ||
          early_spirv_size % sizeof(uint32_t)) {
        REXLOG_ERROR("gta4-native: invalid SPIR-V override for {} shader {:016X}",
                     command.stage == ShaderStage::kVertex ? "vertex" : "pixel", command.hash);
        return;
      }
      if ((override_entry->late_spirv == nullptr) != (late_spirv_size == 0) ||
          late_spirv_size % sizeof(uint32_t)) {
        REXLOG_ERROR("gta4-native: invalid late SPIR-V override for {} shader {:016X}",
                     command.stage == ShaderStage::kVertex ? "vertex" : "pixel", command.hash);
        return;
      }
      early_spirv.resize(early_spirv_size / sizeof(uint32_t));
      std::memcpy(early_spirv.data(), override_entry->spirv, early_spirv_size);
      if (late_spirv_size) {
        late_spirv.resize(late_spirv_size / sizeof(uint32_t));
        std::memcpy(late_spirv.data(), override_entry->late_spirv, late_spirv_size);
      }
    } else {
      const size_t cache_size = shader_cache_data_.size();
      auto decode_cached_spirv = [&](uint32_t offset, uint32_t size,
                                     std::string_view variant,
                                     std::vector<uint32_t>& decoded,
                                     size_t& decoded_size) {
        if (!size || offset > cache_size || size > cache_size - offset) {
          REXLOG_ERROR("gta4-native: invalid {} SMOL-V range for cache entry {}",
                       variant, cache_entry->filename);
          return false;
        }
        const uint8_t* smolv_data = shader_cache_data_.data() + offset;
        decoded_size = smolv::GetDecodedBufferSize(smolv_data, size);
        if (!decoded_size || decoded_size % sizeof(uint32_t)) {
          REXLOG_ERROR("gta4-native: invalid decoded {} SPIR-V size for cache entry {}",
                       variant, cache_entry->filename);
          return false;
        }
        decoded.resize(decoded_size / sizeof(uint32_t));
        if (!smolv::Decode(smolv_data, size, decoded.data(), decoded_size)) {
          REXLOG_ERROR("gta4-native: failed to decode {} SMOL-V cache entry {}",
                       variant, cache_entry->filename);
          return false;
        }
        return true;
      };
      if (!decode_cached_spirv(cache_entry->spirvOffset, cache_entry->spirvSize,
                               "early", early_spirv, early_spirv_size)) {
        return;
      }
      if (cache_entry->lateSpirvSize &&
          !decode_cached_spirv(cache_entry->lateSpirvOffset, cache_entry->lateSpirvSize,
                               "late", late_spirv, late_spirv_size)) {
        return;
      }
      shader_filename = cache_entry->filename;
      specialization_constants_mask = cache_entry->specConstantsMask;
    }

    if (early_spirv.empty() || early_spirv.front() != kSpirvMagic) {
      REXLOG_ERROR("gta4-native: invalid early SPIR-V module for shader {}", shader_filename);
      return;
    }
    if (!late_spirv.empty()) {
      const EarlyFragmentTestsStatus late_early_tests = InspectEarlyFragmentTests(late_spirv);
      if (late_early_tests == EarlyFragmentTestsStatus::kInvalid) {
        REXLOG_ERROR("gta4-native: malformed late SPIR-V module for shader {}",
                     shader_filename);
        return;
      }
      if (late_early_tests == EarlyFragmentTestsStatus::kPresent) {
        REXLOG_ERROR(
            "gta4-native: rejecting late SPIR-V module with EarlyFragmentTests for shader {}",
            shader_filename);
        return;
      }
    }
    const bool alpha_capable = HasAlphaTestCapability(specialization_constants_mask);
    if (command.stage == ShaderStage::kPixel && alpha_capable && late_spirv.empty()) {
      REXLOG_ERROR(
          "gta4-native: alpha-capable pixel shader {} has no late fragment-test module",
          shader_filename);
    }

    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const ui::vulkan::VulkanDevice* vulkan_device =
        vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
    if (!vulkan_device) {
      REXLOG_ERROR("gta4-native: Vulkan device unavailable while registering shader");
      return;
    }

    VkShaderModule early_module = ui::vulkan::util::CreateShaderModule(
        vulkan_device, early_spirv.data(), early_spirv_size);
    if (!early_module) {
      REXLOG_ERROR("gta4-native: Vulkan rejected early shader {}", shader_filename);
      return;
    }
    VkShaderModule late_module = VK_NULL_HANDLE;
    if (!late_spirv.empty()) {
      late_module = ui::vulkan::util::CreateShaderModule(
          vulkan_device, late_spirv.data(), late_spirv_size);
      if (!late_module) {
        vulkan_device->functions().vkDestroyShaderModule(
            vulkan_device->device(), early_module, nullptr);
        REXLOG_ERROR("gta4-native: Vulkan rejected late shader {}", shader_filename);
        return;
      }
    }

    auto resource = std::make_unique<NativeShader>();
    resource->stage = command.stage;
    resource->hash = command.hash;
    resource->specialization_constants_mask = specialization_constants_mask;
    resource->early_module = early_module;
    resource->late_module = late_module;
    if (override_entry) {
      resource->filename = shader_filename;
    } else {
      resource->filename.assign(cache_entry->filename,
                                ::strnlen(cache_entry->filename, sizeof(cache_entry->filename)));
    }
    if (command.stage == ShaderStage::kVertex &&
        !ReflectVertexInputs(early_spirv, resource->vertex_inputs)) {
      if (late_module) {
        vulkan_device->functions().vkDestroyShaderModule(
            vulkan_device->device(), late_module, nullptr);
      }
      vulkan_device->functions().vkDestroyShaderModule(
          vulkan_device->device(), early_module, nullptr);
      REXLOG_ERROR("gta4-native: failed to reflect vertex inputs for shader {}", shader_filename);
      return;
    }
    if (command.stage == ShaderStage::kVertex) {
      const auto blend_indices = std::find_if(
          resource->vertex_inputs.begin(), resource->vertex_inputs.end(),
          [](const NativeVertexInput& input) { return input.location == 18; });
      if (blend_indices != resource->vertex_inputs.end()) {
        static std::atomic<uint64_t> blend_indices_shader_count{0};
        const uint64_t index = ++blend_indices_shader_count;
        if (index <= 128) {
          REXLOG_INFO(
              "gta4-native-shader-interface: index={} hash={:016X} file={} "
              "location=18 numeric-type={} components={}",
              index, command.hash, resource->filename,
              uint32_t(blend_indices->numeric_type), blend_indices->component_count);
        }
      }
    }
    shader = resource.get();
    shader_resources_.push_back(std::move(resource));
    resources_by_hash.emplace(command.hash, shader);
    if (override_entry) {
      REXLOG_INFO("gta4-native: using {} shader override {:016X} ({})",
                  command.stage == ShaderStage::kVertex ? "vertex" : "pixel", command.hash,
                  shader_filename);
    }
  }

  shader_handles_[command.shader] = shader;
  ++shader_registration_count_;
  if (shader_registration_count_ <= 16 || !(shader_registration_count_ % 256)) {
    REXLOG_INFO("gta4-native: registered {} shader handle {:08X} hash {:016X} ({})",
                command.stage == ShaderStage::kVertex ? "vertex" : "pixel", command.shader,
                command.hash, shader->filename);
  }
}

const Gta4NativeGraphicsSystem::NativeShader* Gta4NativeGraphicsSystem::FindRegisteredShader(
    uint32_t handle, ShaderStage stage) const {
  if (!handle) {
    return nullptr;
  }
  auto shader = shader_handles_.find(handle);
  return shader != shader_handles_.end() && shader->second->stage == stage ? shader->second
                                                                           : nullptr;
}

void Gta4NativeGraphicsSystem::DestroyShaderResources() {
  if (provider_) {
    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
    if (vulkan_device) {
      const auto& dfn = vulkan_device->functions();
      const VkDevice device = vulkan_device->device();
      for (const auto& shader : shader_resources_) {
        if (shader->late_module) {
          dfn.vkDestroyShaderModule(device, shader->late_module, nullptr);
        }
        if (shader->early_module) {
          dfn.vkDestroyShaderModule(device, shader->early_module, nullptr);
        }
      }
    }
  }
  shader_handles_.clear();
  pixel_shaders_by_hash_.clear();
  vertex_shaders_by_hash_.clear();
  shader_resources_.clear();
  shader_cache_data_.clear();
  shader_cache_initialized_ = false;
  shader_cache_load_attempted_ = false;
}

bool Gta4NativeGraphicsSystem::CreateNativeUploadBuffer(VkDeviceSize capacity,
                                                         NativeUploadBuffer& upload_buffer) {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device || !capacity ||
      capacity > vulkan_device->properties().maxStorageBufferRange) {
    return false;
  }

  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  VkBufferCreateInfo buffer_info{};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = capacity;
  buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                      VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (dfn.vkCreateBuffer(device, &buffer_info, nullptr, &upload_buffer.buffer) != VK_SUCCESS) {
    return false;
  }

  VkMemoryRequirements requirements{};
  dfn.vkGetBufferMemoryRequirements(device, upload_buffer.buffer, &requirements);
  upload_buffer.memory_type = ui::vulkan::util::ChooseHostMemoryType(
      vulkan_device->memory_types(), requirements.memoryTypeBits, false);
  if (upload_buffer.memory_type == UINT32_MAX) {
    DestroyNativeUploadBuffer(upload_buffer);
    return false;
  }

  VkMemoryAllocateFlagsInfo flags_info{};
  flags_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
  flags_info.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
  VkMemoryAllocateInfo allocate_info{};
  allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocate_info.pNext = &flags_info;
  allocate_info.allocationSize = requirements.size;
  allocate_info.memoryTypeIndex = upload_buffer.memory_type;
  if (dfn.vkAllocateMemory(device, &allocate_info, nullptr, &upload_buffer.memory) != VK_SUCCESS ||
      dfn.vkBindBufferMemory(device, upload_buffer.buffer, upload_buffer.memory, 0) !=
          VK_SUCCESS ||
      dfn.vkMapMemory(device, upload_buffer.memory, 0, VK_WHOLE_SIZE, 0,
                      reinterpret_cast<void**>(&upload_buffer.mapping)) != VK_SUCCESS) {
    DestroyNativeUploadBuffer(upload_buffer);
    return false;
  }

  VkBufferDeviceAddressInfo address_info{};
  address_info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
  address_info.buffer = upload_buffer.buffer;
  upload_buffer.device_address = dfn.vkGetBufferDeviceAddress(device, &address_info);
  upload_buffer.capacity = capacity;
  upload_buffer.allocation_size = requirements.size;
  std::memset(upload_buffer.mapping, 0, size_t(kDefaultVertexDataSize));
  upload_buffer.write_offset = kDefaultVertexDataSize;
  if (!upload_buffer.device_address) {
    DestroyNativeUploadBuffer(upload_buffer);
    return false;
  }
  return true;
}

void Gta4NativeGraphicsSystem::DestroyNativeUploadBuffer(NativeUploadBuffer& upload_buffer) {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    upload_buffer = {};
    return;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  if (upload_buffer.mapping && upload_buffer.memory) {
    dfn.vkUnmapMemory(device, upload_buffer.memory);
  }
  if (upload_buffer.buffer) {
    dfn.vkDestroyBuffer(device, upload_buffer.buffer, nullptr);
  }
  if (upload_buffer.memory) {
    dfn.vkFreeMemory(device, upload_buffer.memory, nullptr);
  }
  upload_buffer = {};
}

bool Gta4NativeGraphicsSystem::InitializeContentProbeBuffer() {
  if (content_probe_buffer_.buffer) {
    return true;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }
  if (!ui::vulkan::util::CreateDedicatedAllocationBuffer(
          vulkan_device, kContentProbeBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
          ui::vulkan::util::MemoryPurpose::kReadback, content_probe_buffer_.buffer,
          content_probe_buffer_.memory, &content_probe_buffer_.memory_type,
          &content_probe_buffer_.allocation_size)) {
    return false;
  }
  const VkResult map_result = vulkan_device->functions().vkMapMemory(
      vulkan_device->device(), content_probe_buffer_.memory, 0, VK_WHOLE_SIZE, 0,
      reinterpret_cast<void**>(&content_probe_buffer_.mapping));
  if (map_result != VK_SUCCESS) {
    DestroyContentProbeBuffer();
    return false;
  }
  std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
  return true;
}

void Gta4NativeGraphicsSystem::DestroyContentProbeBuffer() {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (vulkan_device) {
    const auto& dfn = vulkan_device->functions();
    const VkDevice device = vulkan_device->device();
    if (content_probe_buffer_.mapping && content_probe_buffer_.memory) {
      dfn.vkUnmapMemory(device, content_probe_buffer_.memory);
    }
    if (content_probe_buffer_.buffer) {
      dfn.vkDestroyBuffer(device, content_probe_buffer_.buffer, nullptr);
    }
    if (content_probe_buffer_.memory) {
      dfn.vkFreeMemory(device, content_probe_buffer_.memory, nullptr);
    }
  }
  content_probe_buffer_ = {};
}

bool Gta4NativeGraphicsSystem::InitializeTranslucentQueryPool() {
  if (translucent_query_state_.pool) {
    return true;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }
  VkQueryPoolCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
  create_info.queryType = VK_QUERY_TYPE_OCCLUSION;
  create_info.queryCount = kTranslucentQueryCapacity;
  if (vulkan_device->functions().vkCreateQueryPool(
          vulkan_device->device(), &create_info, nullptr,
          &translucent_query_state_.pool) != VK_SUCCESS) {
    translucent_query_state_ = {};
    REXLOG_ERROR(
        "gta4-native-cause: point=translucent-query-pool-failed capacity={}",
        kTranslucentQueryCapacity);
    return false;
  }
  REXLOG_INFO("gta4-native-cause: point=translucent-query-pool-created capacity={}",
              kTranslucentQueryCapacity);
  return true;
}

void Gta4NativeGraphicsSystem::DestroyTranslucentQueryPool() {
  if (translucent_query_state_.pool && provider_) {
    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
    if (vulkan_device) {
      vulkan_device->functions().vkDestroyQueryPool(
          vulkan_device->device(), translucent_query_state_.pool, nullptr);
    }
  }
  translucent_query_state_ = {};
}

void Gta4NativeGraphicsSystem::AnalyzePendingTranslucentQueries() {
  if (!translucent_query_state_.pool || !translucent_query_state_.pending_count) {
    return;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return;
  }
  struct QueryResult {
    uint64_t samples = 0;
    uint64_t available = 0;
  };
  std::array<QueryResult, kTranslucentQueryCapacity> results{};
  const VkResult result = vulkan_device->functions().vkGetQueryPoolResults(
      vulkan_device->device(), translucent_query_state_.pool, 0,
      translucent_query_state_.pending_count, sizeof(results),
      results.data(), sizeof(QueryResult),
      VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
  if (result != VK_SUCCESS && result != VK_NOT_READY) {
    REXLOG_ERROR(
        "gta4-native-cause: point=translucent-query-read-failed frame={} count={} result={}",
        translucent_query_state_.pending_frame,
        translucent_query_state_.pending_count, uint32_t(result));
  }
  for (uint32_t index = 0; index < translucent_query_state_.pending_count; ++index) {
    const NativeTranslucentQuery& query = translucent_query_state_.queries[index];
    if (!query.valid) {
      continue;
    }
    const QueryResult& query_result = results[index];
    const NativeFixedFunctionState& fixed = query.fixed_function;
    const bool alpha_specialized = query.pixel_shader_uses_late_module;
    REXLOG_INFO(
        "gta4-native-cause: point=translucent-query-result frame={} cmd={} draw={:016X} "
        "category={} variant={} shader={} vs={:016X} ps={:016X} target={:08X}/{:08X} "
        "depth={:08X}/{:08X} samples-passed={} available={} recorded={} phase={} "
        "blend={}:{}:{}:{}:{}:{}:{} alpha={}:{}:{}:specialized={}:packed={:08X}:module={} "
        "z={}:{}:{}:vk{} cull={} color-mask={:08X} "
        "stencil={}:two={} front={}:{}:{}:{}:{:02X}:{:02X}:{:02X} "
        "back={}:{}:{}:{}:{:02X}:{:02X}:{:02X} "
        "effective-front=vk{}/vk{}/vk{}/vk{}:{:02X}:{:02X}:{:02X} "
        "effective-back=vk{}/vk{}/vk{}/vk{}:{:02X}:{:02X}:{:02X}",
        query.frame, query.command_index, query.draw_id, query.category,
        query.variant, query.shader_filename, query.vertex_shader_hash, query.pixel_shader_hash,
        query.target_handle, query.target_address, query.depth_handle,
        query.depth_address, query_result.samples, query_result.available,
        query.draw_recorded, query.render_phase,
        fixed.blend_enable, fixed.source_blend, fixed.destination_blend,
        fixed.blend_operation, fixed.source_blend_alpha,
        fixed.destination_blend_alpha, fixed.blend_operation_alpha,
        fixed.alpha_test_enable, fixed.alpha_function, fixed.alpha_reference,
        alpha_specialized, query.pixel_shader_specialization_value,
        query.pixel_shader_uses_late_module ? "late" : "early",
        fixed.depth_enable, fixed.depth_function,
        fixed.depth_write_enable, uint32_t(ConvertCompareFunction(fixed.depth_function)),
        fixed.cull_mode, fixed.color_write_mask, fixed.stencil_enable,
        fixed.two_sided_stencil, fixed.stencil_function, fixed.stencil_fail,
        fixed.stencil_depth_fail, fixed.stencil_pass, fixed.stencil_reference,
        fixed.stencil_mask, fixed.stencil_write_mask, fixed.ccw_stencil_function,
        fixed.ccw_stencil_fail, fixed.ccw_stencil_depth_fail,
        fixed.ccw_stencil_pass, fixed.back_stencil_reference,
        fixed.back_stencil_mask, fixed.back_stencil_write_mask,
        uint32_t(ConvertCompareFunction(fixed.stencil_function)),
        uint32_t(ConvertStencilOperation(fixed.stencil_fail)),
        uint32_t(ConvertStencilOperation(fixed.stencil_depth_fail)),
        uint32_t(ConvertStencilOperation(fixed.stencil_pass)),
        fixed.stencil_reference, fixed.stencil_mask, fixed.stencil_write_mask,
        uint32_t(ConvertCompareFunction(fixed.two_sided_stencil
                                            ? fixed.ccw_stencil_function
                                            : fixed.stencil_function)),
        uint32_t(ConvertStencilOperation(fixed.two_sided_stencil
                                             ? fixed.ccw_stencil_fail
                                             : fixed.stencil_fail)),
        uint32_t(ConvertStencilOperation(fixed.two_sided_stencil
                                             ? fixed.ccw_stencil_depth_fail
                                             : fixed.stencil_depth_fail)),
        uint32_t(ConvertStencilOperation(fixed.two_sided_stencil
                                             ? fixed.ccw_stencil_pass
                                             : fixed.stencil_pass)),
        fixed.stencil_reference, fixed.stencil_mask, fixed.stencil_write_mask);
  }
  translucent_query_state_.pending_frame = 0;
  translucent_query_state_.pending_count = 0;
  translucent_query_state_.queries = {};
}

bool Gta4NativeGraphicsSystem::InitializeNativeGpuProfiler() {
  if (native_gpu_profile_state_.pool) {
    return true;
  }
  if (native_gpu_profile_state_.support_checked) {
    return native_gpu_profile_state_.supported;
  }
  native_gpu_profile_state_.support_checked = true;
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }

  const auto& ifn = vulkan_device->vulkan_instance()->functions();
  VkPhysicalDeviceProperties physical_properties{};
  ifn.vkGetPhysicalDeviceProperties(vulkan_device->physical_device(),
                                    &physical_properties);
  if (physical_properties.limits.timestampPeriod <= 0.0f) {
    REXLOG_WARN(
        "gta4-native-perf: point=unsupported reason=device-timestamps "
        "timestamp-compute-graphics={} timestamp-period-ns={}",
        physical_properties.limits.timestampComputeAndGraphics,
        physical_properties.limits.timestampPeriod);
    return false;
  }

  uint32_t queue_family_count = 0;
  ifn.vkGetPhysicalDeviceQueueFamilyProperties(
      vulkan_device->physical_device(), &queue_family_count, nullptr);
  std::vector<VkQueueFamilyProperties> queue_properties(queue_family_count);
  ifn.vkGetPhysicalDeviceQueueFamilyProperties(
      vulkan_device->physical_device(), &queue_family_count,
      queue_properties.data());
  const uint32_t queue_family = vulkan_device->queue_family_graphics_compute();
  if (queue_family >= queue_family_count ||
      !queue_properties[queue_family].timestampValidBits) {
    REXLOG_WARN(
        "gta4-native-perf: point=unsupported reason=queue-timestamps "
        "queue-family={} family-count={} valid-bits={}",
        queue_family, queue_family_count,
        queue_family < queue_family_count
            ? queue_properties[queue_family].timestampValidBits
            : 0);
    return false;
  }

  VkQueryPoolCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
  create_info.queryType = VK_QUERY_TYPE_TIMESTAMP;
  create_info.queryCount = kNativeGpuProfileQueryCapacity;
  if (vulkan_device->functions().vkCreateQueryPool(
          vulkan_device->device(), &create_info, nullptr,
          &native_gpu_profile_state_.pool) != VK_SUCCESS) {
    REXLOG_ERROR(
        "gta4-native-perf: point=query-pool-failed capacity={}",
        kNativeGpuProfileQueryCapacity);
    return false;
  }

  vulkan_device->SetObjectName(VK_OBJECT_TYPE_QUERY_POOL,
                               native_gpu_profile_state_.pool,
                               "GTA IV native renderer GPU profiler");
  native_gpu_profile_state_.timestamp_period_ns =
      physical_properties.limits.timestampPeriod;
  native_gpu_profile_state_.timestamp_valid_bits =
      queue_properties[queue_family].timestampValidBits;
  native_gpu_profile_state_.supported = true;
  native_gpu_profile_state_.spans.reserve(2048);
  REXLOG_INFO(
      "gta4-native-perf: point=initialized capacity={} timestamp-period-ns={} "
      "valid-bits={} queue-family={} timestamp-compute-graphics={}",
      kNativeGpuProfileQueryCapacity,
      native_gpu_profile_state_.timestamp_period_ns,
      native_gpu_profile_state_.timestamp_valid_bits, queue_family,
      physical_properties.limits.timestampComputeAndGraphics);
  return true;
}

void Gta4NativeGraphicsSystem::DestroyNativeGpuProfiler() {
  if (native_gpu_profile_state_.pool && provider_) {
    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const ui::vulkan::VulkanDevice* vulkan_device =
        vulkan_provider->vulkan_device();
    if (vulkan_device) {
      vulkan_device->functions().vkDestroyQueryPool(
          vulkan_device->device(), native_gpu_profile_state_.pool, nullptr);
    }
  }
  native_gpu_profile_state_ = {};
}

bool Gta4NativeGraphicsSystem::BeginNativeGpuProfileFrame(
    VkCommandBuffer command_buffer, uint32_t submitted_frame,
    uint64_t cpu_wait_ticks) {
  if (!REXCVAR_GET(gta4_profile_native_renderer) || !submitted_frame) {
    return false;
  }
  const uint32_t interval =
      std::max(1u, REXCVAR_GET(gta4_profile_native_interval));
  if (native_gpu_profile_state_.last_sampled_frame &&
      submitted_frame - native_gpu_profile_state_.last_sampled_frame < interval) {
    return false;
  }
  if (!InitializeNativeGpuProfiler() || native_gpu_profile_state_.pending) {
    return false;
  }

  native_gpu_profile_state_.active = true;
  native_gpu_profile_state_.command_detail =
      REXCVAR_GET(gta4_profile_native_detail) == "command";
  native_gpu_profile_state_.frame = submitted_frame;
  native_gpu_profile_state_.query_count = 0;
  native_gpu_profile_state_.dropped_spans = 0;
  native_gpu_profile_state_.last_sampled_frame = submitted_frame;
  native_gpu_profile_state_.frame_span = SIZE_MAX;
  native_gpu_profile_state_.spans.clear();
  native_gpu_profile_state_.cpu_wait_ticks = cpu_wait_ticks;
  native_gpu_profile_state_.cpu_texture_prepare_ticks = 0;
  native_gpu_profile_state_.cpu_record_ticks = 0;
  native_gpu_profile_state_.cpu_submit_ticks = 0;
  native_gpu_profile_state_.cpu_callback_ticks = 0;
  native_gpu_profile_state_.upload_bytes = 0;
  native_gpu_profile_state_.pipelines_before = native_pipelines_.size();
  native_gpu_profile_state_.pipelines_after = native_pipelines_.size();
  native_gpu_profile_state_.texture_images_before = native_texture_images_.size();
  native_gpu_profile_state_.texture_images_after = native_texture_images_.size();
  native_gpu_profile_state_.surface_images_before = native_surface_images_.size();
  native_gpu_profile_state_.surface_images_after = native_surface_images_.size();
  native_gpu_profile_state_.samplers_before = native_samplers_.size();
  native_gpu_profile_state_.samplers_after = native_samplers_.size();

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider->vulkan_device();
  vulkan_device->functions().vkCmdResetQueryPool(
      command_buffer, native_gpu_profile_state_.pool, 0,
      kNativeGpuProfileQueryCapacity);
  native_gpu_profile_state_.frame_span = BeginNativeGpuProfileSpan(
      command_buffer, NativeGpuProfileScopeKind::kFrame);
  REXLOG_INFO(
      "gta4-native-perf: point=capture-begin frame={} detail={} interval={} "
      "trace-native={} startup-probes={}",
      submitted_frame,
      native_gpu_profile_state_.command_detail ? "command" : "phase",
      interval, REXCVAR_GET(gta4_trace_native_renderer),
      REXCVAR_GET(gta4_trace_startup_content));
  return true;
}

size_t Gta4NativeGraphicsSystem::BeginNativeGpuProfileSpan(
    VkCommandBuffer command_buffer, NativeGpuProfileScopeKind kind,
    RenderPhase render_phase, CommandType command_type,
    uint32_t command_index, const NativeCommand* command) {
  if (!native_gpu_profile_state_.active ||
      native_gpu_profile_state_.query_count + 2 >
          kNativeGpuProfileQueryCapacity) {
    if (native_gpu_profile_state_.active) {
      ++native_gpu_profile_state_.dropped_spans;
    }
    return SIZE_MAX;
  }
  NativeGpuProfileSpan span{};
  span.kind = kind;
  span.render_phase = render_phase;
  span.command_type = command_type;
  span.command_index = command_index;
  span.begin_query = native_gpu_profile_state_.query_count++;
  span.end_query = native_gpu_profile_state_.query_count++;
  if (command && command->pipeline_state) {
    const NativePipelineState& pipeline = *command->pipeline_state;
    span.target_handle = pipeline.render_targets[0].handle;
    span.vertex_shader_hash = pipeline.vertex_shader_resource
                                  ? pipeline.vertex_shader_resource->hash
                                  : 0;
    span.pixel_shader_hash = pipeline.pixel_shader_resource
                                 ? pipeline.pixel_shader_resource->hash
                                 : 0;
  }
  const size_t span_index = native_gpu_profile_state_.spans.size();
  native_gpu_profile_state_.spans.push_back(span);
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  vulkan_provider->vulkan_device()->functions().vkCmdWriteTimestamp(
      command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
      native_gpu_profile_state_.pool, span.begin_query);
  return span_index;
}

void Gta4NativeGraphicsSystem::EndNativeGpuProfileSpan(
    VkCommandBuffer command_buffer, size_t span_index,
    uint64_t cpu_begin_ticks) {
  if (!native_gpu_profile_state_.active || span_index == SIZE_MAX ||
      span_index >= native_gpu_profile_state_.spans.size()) {
    return;
  }
  NativeGpuProfileSpan& span = native_gpu_profile_state_.spans[span_index];
  if (span.ended) {
    return;
  }
  span.ended = true;
  if (cpu_begin_ticks) {
    span.cpu_ticks =
        rex::chrono::Clock::QueryHostTickCount() - cpu_begin_ticks;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  vulkan_provider->vulkan_device()->functions().vkCmdWriteTimestamp(
      command_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      native_gpu_profile_state_.pool, span.end_query);
}

void Gta4NativeGraphicsSystem::EndNativeGpuProfileFrame(
    VkCommandBuffer command_buffer) {
  if (!native_gpu_profile_state_.active) {
    return;
  }
  EndNativeGpuProfileSpan(command_buffer,
                          native_gpu_profile_state_.frame_span);
  native_gpu_profile_state_.active = false;
  native_gpu_profile_state_.pending = true;
  native_gpu_profile_state_.upload_bytes = upload_buffer_.write_offset;
  native_gpu_profile_state_.pipelines_after = native_pipelines_.size();
  native_gpu_profile_state_.texture_images_after = native_texture_images_.size();
  native_gpu_profile_state_.surface_images_after = native_surface_images_.size();
  native_gpu_profile_state_.samplers_after = native_samplers_.size();
}

void Gta4NativeGraphicsSystem::CancelNativeGpuProfileFrame() {
  native_gpu_profile_state_.active = false;
  native_gpu_profile_state_.pending = false;
  native_gpu_profile_state_.frame = 0;
  native_gpu_profile_state_.query_count = 0;
  native_gpu_profile_state_.frame_span = SIZE_MAX;
  native_gpu_profile_state_.spans.clear();
}

void Gta4NativeGraphicsSystem::AnalyzePendingNativeGpuProfile() {
  if (!native_gpu_profile_state_.pending ||
      !native_gpu_profile_state_.pool ||
      !native_gpu_profile_state_.query_count) {
    return;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return;
  }
  struct QueryResult {
    uint64_t timestamp = 0;
    uint64_t available = 0;
  };
  std::vector<QueryResult> query_results(native_gpu_profile_state_.query_count);
  const VkResult result = vulkan_device->functions().vkGetQueryPoolResults(
      vulkan_device->device(), native_gpu_profile_state_.pool, 0,
      native_gpu_profile_state_.query_count,
      query_results.size() * sizeof(QueryResult), query_results.data(),
      sizeof(QueryResult),
      VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
  if (result != VK_SUCCESS) {
    REXLOG_WARN(
        "gta4-native-perf: point=query-read-failed frame={} queries={} result={}",
        native_gpu_profile_state_.frame,
        native_gpu_profile_state_.query_count, uint32_t(result));
    CancelNativeGpuProfileFrame();
    return;
  }

  const uint64_t host_frequency =
      rex::chrono::Clock::QueryHostTickFrequency();
  const auto host_ticks_to_ms = [host_frequency](uint64_t ticks) {
    return host_frequency
               ? (double(ticks) * 1000.0) / double(host_frequency)
               : 0.0;
  };

  struct TimedSpan {
    const NativeGpuProfileSpan* span = nullptr;
    double gpu_ms = 0.0;
    double cpu_ms = 0.0;
  };
  std::vector<TimedSpan> timed_spans;
  timed_spans.reserve(native_gpu_profile_state_.spans.size());
  uint32_t unavailable_spans = 0;
  for (const NativeGpuProfileSpan& span : native_gpu_profile_state_.spans) {
    if (span.begin_query >= query_results.size() ||
        span.end_query >= query_results.size() ||
        !query_results[span.begin_query].available ||
        !query_results[span.end_query].available) {
      ++unavailable_spans;
      continue;
    }
    const uint64_t elapsed_ticks = NativeTimestampDelta(
        query_results[span.begin_query].timestamp,
        query_results[span.end_query].timestamp,
        native_gpu_profile_state_.timestamp_valid_bits);
    const double gpu_ms =
        (double(elapsed_ticks) *
         double(native_gpu_profile_state_.timestamp_period_ns)) /
        1000000.0;
    timed_spans.push_back(
        TimedSpan{&span, gpu_ms, host_ticks_to_ms(span.cpu_ticks)});
  }

  struct Aggregate {
    uint32_t count = 0;
    double total_ms = 0.0;
    double maximum_ms = 0.0;
  };
  std::array<Aggregate, uint32_t(RenderPhase::kCompositePostFx) + 1>
      phase_aggregates{};
  std::unordered_map<uint32_t, Aggregate> command_aggregates;
  std::unordered_map<uint64_t, Aggregate> shader_aggregates;
  std::vector<TimedSpan> command_spans;
  double frame_gpu_ms = 0.0;
  for (const TimedSpan& timed : timed_spans) {
    const NativeGpuProfileSpan& span = *timed.span;
    if (span.kind == NativeGpuProfileScopeKind::kFrame) {
      frame_gpu_ms = timed.gpu_ms;
    }
    if (span.kind == NativeGpuProfileScopeKind::kRenderPhase ||
        span.kind == NativeGpuProfileScopeKind::kCommand) {
      const uint32_t phase_index = uint32_t(span.render_phase);
      if (phase_index < phase_aggregates.size()) {
        Aggregate& aggregate = phase_aggregates[phase_index];
        ++aggregate.count;
        aggregate.total_ms += timed.gpu_ms;
        aggregate.maximum_ms =
            std::max(aggregate.maximum_ms, timed.gpu_ms);
      }
    }
    if (span.kind != NativeGpuProfileScopeKind::kCommand) {
      continue;
    }
    command_spans.push_back(timed);
    Aggregate& command_aggregate =
        command_aggregates[uint32_t(span.command_type)];
    ++command_aggregate.count;
    command_aggregate.total_ms += timed.gpu_ms;
    command_aggregate.maximum_ms =
        std::max(command_aggregate.maximum_ms, timed.gpu_ms);
    if (span.pixel_shader_hash) {
      Aggregate& shader_aggregate =
          shader_aggregates[span.pixel_shader_hash];
      ++shader_aggregate.count;
      shader_aggregate.total_ms += timed.gpu_ms;
      shader_aggregate.maximum_ms =
          std::max(shader_aggregate.maximum_ms, timed.gpu_ms);
    }
  }

  const auto resource_delta = [](size_t before, size_t after) {
    return after >= before ? after - before : size_t(0);
  };
  REXLOG_INFO(
      "gta4-native-perf: point=frame frame={} detail={} gpu-ms={:.6f} "
      "cpu-callback-ms={:.6f} cpu-wait-ms={:.6f} cpu-texture-ms={:.6f} "
      "cpu-record-ms={:.6f} cpu-submit-ms={:.6f} queries={} spans={} "
      "unavailable={} dropped={} upload-bytes={} "
      "created[pipelines={},textures={},surfaces={},samplers={}] "
      "resident[pipelines={},textures={},surfaces={},samplers={}]",
      native_gpu_profile_state_.frame,
      native_gpu_profile_state_.command_detail ? "command" : "phase",
      frame_gpu_ms,
      host_ticks_to_ms(native_gpu_profile_state_.cpu_callback_ticks),
      host_ticks_to_ms(native_gpu_profile_state_.cpu_wait_ticks),
      host_ticks_to_ms(native_gpu_profile_state_.cpu_texture_prepare_ticks),
      host_ticks_to_ms(native_gpu_profile_state_.cpu_record_ticks),
      host_ticks_to_ms(native_gpu_profile_state_.cpu_submit_ticks),
      native_gpu_profile_state_.query_count, timed_spans.size(),
      unavailable_spans, native_gpu_profile_state_.dropped_spans,
      native_gpu_profile_state_.upload_bytes,
      resource_delta(native_gpu_profile_state_.pipelines_before,
                     native_gpu_profile_state_.pipelines_after),
      resource_delta(native_gpu_profile_state_.texture_images_before,
                     native_gpu_profile_state_.texture_images_after),
      resource_delta(native_gpu_profile_state_.surface_images_before,
                     native_gpu_profile_state_.surface_images_after),
      resource_delta(native_gpu_profile_state_.samplers_before,
                     native_gpu_profile_state_.samplers_after),
      native_gpu_profile_state_.pipelines_after,
      native_gpu_profile_state_.texture_images_after,
      native_gpu_profile_state_.surface_images_after,
      native_gpu_profile_state_.samplers_after);

  const auto scope_kind_name = [](NativeGpuProfileScopeKind kind) {
    switch (kind) {
      case NativeGpuProfileScopeKind::kFrame:
        return "frame";
      case NativeGpuProfileScopeKind::kTexturePreparation:
        return "texture-preparation";
      case NativeGpuProfileScopeKind::kNativeFrame:
        return "native-frame";
      case NativeGpuProfileScopeKind::kRenderPhase:
        return "render-phase";
      case NativeGpuProfileScopeKind::kCommand:
        return "command";
      case NativeGpuProfileScopeKind::kPresent:
        return "present";
    }
    return "unknown";
  };
  for (const TimedSpan& timed : timed_spans) {
    const NativeGpuProfileSpan& span = *timed.span;
    if (span.kind == NativeGpuProfileScopeKind::kFrame ||
        span.kind == NativeGpuProfileScopeKind::kCommand) {
      continue;
    }
    REXLOG_INFO(
        "gta4-native-perf: point=scope frame={} kind={} phase={} "
        "gpu-ms={:.6f} cpu-ms={:.6f}",
        native_gpu_profile_state_.frame, scope_kind_name(span.kind),
        RenderPhaseName(span.render_phase), timed.gpu_ms, timed.cpu_ms);
  }

  for (uint32_t phase_index = 0; phase_index < phase_aggregates.size();
       ++phase_index) {
    const Aggregate& aggregate = phase_aggregates[phase_index];
    if (!aggregate.count) {
      continue;
    }
    REXLOG_INFO(
        "gta4-native-perf: point=phase frame={} phase={} count={} "
        "gpu-total-ms={:.6f} gpu-average-ms={:.6f} gpu-maximum-ms={:.6f}",
        native_gpu_profile_state_.frame,
        RenderPhaseName(RenderPhase(phase_index)), aggregate.count,
        aggregate.total_ms, aggregate.total_ms / double(aggregate.count),
        aggregate.maximum_ms);
  }

  for (const auto& [command_value, aggregate] : command_aggregates) {
    REXLOG_INFO(
        "gta4-native-perf: point=command-type frame={} type={} count={} "
        "gpu-total-ms={:.6f} gpu-average-ms={:.6f} gpu-maximum-ms={:.6f}",
        native_gpu_profile_state_.frame,
        CommandTypeName(CommandType(command_value)), aggregate.count,
        aggregate.total_ms, aggregate.total_ms / double(aggregate.count),
        aggregate.maximum_ms);
  }

  std::vector<std::pair<uint64_t, Aggregate>> ranked_shaders(
      shader_aggregates.begin(), shader_aggregates.end());
  std::sort(ranked_shaders.begin(), ranked_shaders.end(),
            [](const auto& left, const auto& right) {
              return left.second.total_ms > right.second.total_ms;
            });
  const size_t top_count =
      std::max<size_t>(1, REXCVAR_GET(gta4_profile_native_top));
  const size_t shader_count = std::min(top_count, ranked_shaders.size());
  for (size_t rank = 0; rank < shader_count; ++rank) {
    const auto& [shader_hash, aggregate] = ranked_shaders[rank];
    const auto shader = pixel_shaders_by_hash_.find(shader_hash);
    REXLOG_INFO(
        "gta4-native-perf: point=shader-hotspot frame={} rank={} ps={:016X} "
        "shader={} count={} gpu-total-ms={:.6f} gpu-average-ms={:.6f} "
        "gpu-maximum-ms={:.6f}",
        native_gpu_profile_state_.frame, rank + 1, shader_hash,
        shader != pixel_shaders_by_hash_.end() ? shader->second->filename : "unknown",
        aggregate.count, aggregate.total_ms,
        aggregate.total_ms / double(aggregate.count), aggregate.maximum_ms);
  }

  std::sort(command_spans.begin(), command_spans.end(),
            [](const TimedSpan& left, const TimedSpan& right) {
              return left.gpu_ms > right.gpu_ms;
            });
  const size_t gpu_hotspot_count = std::min(top_count, command_spans.size());
  for (size_t rank = 0; rank < gpu_hotspot_count; ++rank) {
    const TimedSpan& timed = command_spans[rank];
    const NativeGpuProfileSpan& span = *timed.span;
    REXLOG_INFO(
        "gta4-native-perf: point=gpu-hotspot frame={} rank={} cmd={} type={} "
        "phase={} gpu-ms={:.6f} cpu-ms={:.6f} vs={:016X} ps={:016X} "
        "target={:08X}",
        native_gpu_profile_state_.frame, rank + 1, span.command_index,
        CommandTypeName(span.command_type), RenderPhaseName(span.render_phase),
        timed.gpu_ms, timed.cpu_ms, span.vertex_shader_hash,
        span.pixel_shader_hash, span.target_handle);
  }

  std::sort(command_spans.begin(), command_spans.end(),
            [](const TimedSpan& left, const TimedSpan& right) {
              return left.cpu_ms > right.cpu_ms;
            });
  const size_t cpu_hotspot_count = std::min(top_count, command_spans.size());
  for (size_t rank = 0; rank < cpu_hotspot_count; ++rank) {
    const TimedSpan& timed = command_spans[rank];
    const NativeGpuProfileSpan& span = *timed.span;
    REXLOG_INFO(
        "gta4-native-perf: point=cpu-hotspot frame={} rank={} cmd={} type={} "
        "phase={} cpu-ms={:.6f} gpu-ms={:.6f} vs={:016X} ps={:016X} "
        "target={:08X}",
        native_gpu_profile_state_.frame, rank + 1, span.command_index,
        CommandTypeName(span.command_type), RenderPhaseName(span.render_phase),
        timed.cpu_ms, timed.gpu_ms, span.vertex_shader_hash,
        span.pixel_shader_hash, span.target_handle);
  }

  native_gpu_profile_state_.pending = false;
  native_gpu_profile_state_.frame = 0;
  native_gpu_profile_state_.query_count = 0;
  native_gpu_profile_state_.frame_span = SIZE_MAX;
  native_gpu_profile_state_.spans.clear();
}

void Gta4NativeGraphicsSystem::AnalyzePendingContentProbe() {
  if (!content_probe_buffer_.pending_frame || !content_probe_buffer_.mapping) {
    return;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return;
  }
  if (!(vulkan_device->memory_types().host_coherent &
        (uint32_t(1) << content_probe_buffer_.memory_type))) {
    VkMappedMemoryRange range{};
    range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    range.memory = content_probe_buffer_.memory;
    range.size = std::min(
        rex::round_up(kContentProbeBufferSize, vulkan_device->properties().nonCoherentAtomSize),
        content_probe_buffer_.allocation_size);
    vulkan_device->functions().vkInvalidateMappedMemoryRanges(vulkan_device->device(), 1, &range);
  }

  std::array<bool, 8> e2e_stage_seen{};
  std::array<uint32_t, 8> e2e_image_count{};
  std::array<uint32_t, 8> e2e_nonzero_samples{};
  std::array<uint32_t, 8> e2e_visible_samples{};
  std::array<float, 8> e2e_maximum_luminance{};
  std::array<float, 8> e2e_p90_luminance{};
  bool scene_checkpoint_seen = false;
  bool scene_checkpoint_nonfinite_seen = false;
  uint32_t previous_scene_checkpoint_command = UINT32_MAX;
  uint32_t previous_scene_checkpoint_nonfinite = 0;
  for (uint32_t stage_index = 0; stage_index < content_probe_buffer_.stages.size(); ++stage_index) {
    const NativeContentProbeStage& stage = content_probe_buffer_.stages[stage_index];
    if (!stage.valid) {
      continue;
    }
    const char* stage_name = "unknown";
    switch (stage.kind) {
      case 0:
        stage_name = "final-surface";
        break;
      case 1:
        stage_name = "retained-surface";
        break;
      case 2:
        stage_name = "frontbuffer";
        break;
      case 3:
        stage_name = "presenter";
        break;
      case 4:
        stage_name = "final-composite-input";
        break;
      case 5:
        stage_name = "resolve-source-before";
        break;
      case 6:
        stage_name = "resolve-predecessor-draw";
        break;
      case 7:
        stage_name = "deferred-lighting-input";
        break;
      case 8:
        stage_name = "scene-write-checkpoint";
        break;
      case 9:
        stage_name = "scene-before-first-write";
        break;
      case 10:
        stage_name = "scene-first-write-input";
        break;
      case 11:
        stage_name = "scene-after-materialization";
        break;
      case 12:
        stage_name = "reflection-resolve-source";
        break;
      case 13:
        stage_name = "reflection-resolve-destination";
        break;
      case 14:
        stage_name = "reflection-tail-mip";
        break;
      case 15:
        stage_name = "translucent-target-before";
        break;
      case 16:
        stage_name = "translucent-target-after";
        break;
      case 17:
        stage_name = "translucent-depth-before";
        break;
      case 18:
        stage_name = "translucent-input";
        break;
      case 19:
        stage_name = "depth-explicit-handoff-source";
        break;
      case 20:
        stage_name = "stencil-explicit-handoff-source";
        break;
      case 21:
        stage_name = "depth-materialize-destination-before";
        break;
      case 22:
        stage_name = "stencil-materialize-destination-before";
        break;
      case 23:
        stage_name = "depth-materialize-destination-after";
        break;
      case 24:
        stage_name = "stencil-materialize-destination-after";
        break;
      case 25:
        stage_name = "depth-resolve-clear-source-before";
        break;
      case 26:
        stage_name = "stencil-resolve-clear-source-before";
        break;
      case 27:
        stage_name = "depth-resolve-clear-source-after";
        break;
      case 28:
        stage_name = "stencil-resolve-clear-source-after";
        break;
      case 29:
        stage_name = "depth-explicit-resolve-destination";
        break;
      case 30:
        stage_name = "stencil-explicit-resolve-destination";
        break;
      case 31:
        stage_name = "stencil-deferred-clear-output";
        break;
      case 32:
        stage_name = "stencil-deferred-writer-output";
        break;
      case 33:
        stage_name = "stencil-explicit-resolve-source";
        break;
      case 34:
        stage_name = "depth-explicit-handoff-destination-before";
        break;
      case 35:
        stage_name = "stencil-explicit-handoff-destination-before";
        break;
      case 36:
        stage_name = "depth-explicit-handoff-destination-after";
        break;
      case 37:
        stage_name = "stencil-explicit-handoff-destination-after";
        break;
      case 38:
        stage_name = "depth-explicit-resolve-source-before";
        break;
      case 39:
        stage_name = "stencil-explicit-resolve-source-before";
        break;
      case 40:
        stage_name = "depth-forward-phase-entry";
        break;
      case 41:
        stage_name = "stencil-forward-phase-entry";
        break;
      case 42:
        stage_name = "depth-first-opaque-consumer";
        break;
      case 43:
        stage_name = "stencil-first-opaque-consumer";
        break;
      case 44:
        stage_name = "opaque-target-before";
        break;
      case 45:
        stage_name = "opaque-target-after";
        break;
      case 46:
        stage_name = "depth-first-translucent-consumer";
        break;
      case 47:
        stage_name = "stencil-first-translucent-consumer";
        break;
      case 48:
        stage_name = "translucent-family-target-before";
        break;
      case 49:
        stage_name = "translucent-family-target-after";
        break;
    }
    const uint8_t* stage_data = content_probe_buffer_.mapping +
                                VkDeviceSize(stage_index) * kContentProbeStageStride;
    if (stage.aspect == VK_IMAGE_ASPECT_STENCIL_BIT) {
      uint32_t zero_samples = 0;
      uint32_t one_samples = 0;
      uint32_t nonzero_samples = 0;
      uint8_t minimum = UINT8_MAX;
      uint8_t maximum = 0;
      for (uint32_t sample = 0; sample < kContentProbeSampleCount; ++sample) {
        const uint8_t value =
            stage_data[VkDeviceSize(sample) * kContentProbeSampleStride];
        minimum = std::min(minimum, value);
        maximum = std::max(maximum, value);
        zero_samples += value == 0;
        one_samples += value == 1;
        nonzero_samples += value != 0;
      }
      const uint64_t checksum =
          XXH3_64bits(stage_data, size_t(kContentProbeStageStride));
      REXLOG_WARN(
          "gta4-native-cause: point=depth-stencil-probe frame={} cmd={} stage={} "
          "handle={:08X} address={:08X} aspect=stencil format={} size={}x{} "
          "zero={}/{} one={}/{} nonzero={}/{} min={} max={} checksum={:016X}",
          content_probe_buffer_.pending_frame, stage.command_index, stage_name,
          stage.handle, stage.address, uint32_t(stage.format), stage.width,
          stage.height, zero_samples, kContentProbeSampleCount, one_samples,
          kContentProbeSampleCount, nonzero_samples, kContentProbeSampleCount,
          uint32_t(minimum), uint32_t(maximum), checksum);
      if (!stage.diagnostic_role.empty()) {
        REXLOG_WARN(
            "gta4-native-cause: point=depth-lifecycle-value frame={} cmd={} role={} "
            "stage={} wrapper={:08X} resource={:08X}/{:08X}@{} samples={} "
            "aspect=stencil format={} size={}x{} min={} max={} checksum={:016X} "
            "provenance={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
            content_probe_buffer_.pending_frame, stage.command_index,
            stage.diagnostic_role, stage_name, stage.trace_wrapper, stage.handle,
            stage.address, stage.resource_generation, stage.sample_count,
            uint32_t(stage.format), stage.width, stage.height, uint32_t(minimum),
            uint32_t(maximum), checksum, stage.provenance_handle,
            stage.provenance_frame, stage.provenance_command,
            stage.provenance_serial, stage.provenance_phase,
            stage.provenance_kind);
      }
      continue;
    }
    uint32_t nonzero_samples = 0;
    uint32_t visible_samples = 0;
    uint32_t nonfinite_samples = 0;
    uint32_t negative_samples = 0;
    uint32_t black_samples = 0;
    uint32_t sub_code_samples = 0;
    uint32_t dark_samples = 0;
    uint32_t dim_samples = 0;
    uint32_t bright_samples = 0;
    float minimum_luminance = std::numeric_limits<float>::infinity();
    float maximum_luminance = 0.0f;
    double luminance_sum = 0.0;
    std::array<float, kContentProbeSampleCount> luminances{};
    for (uint32_t sample = 0; sample < kContentProbeSampleCount; ++sample) {
      const uint8_t* pixel = stage_data + VkDeviceSize(sample) * kContentProbeSampleStride;
      float red = 0.0f;
      float green = 0.0f;
      float blue = 0.0f;
      if (stage.format == VK_FORMAT_R16G16B16A16_SFLOAT) {
        std::array<uint16_t, 4> components{};
        std::memcpy(components.data(), pixel, sizeof(components));
        // This is a readback from a Vulkan R16G16B16A16_SFLOAT image, whose
        // components use IEEE binary16. The Xenos extended-range half decoder
        // is only correct for guest memory, not for this native image.
        static_assert(sizeof(_Float16) == sizeof(uint16_t));
        red = float(std::bit_cast<_Float16>(components[0]));
        green = float(std::bit_cast<_Float16>(components[1]));
        blue = float(std::bit_cast<_Float16>(components[2]));
      } else if (stage.format == VK_FORMAT_R16G16_SFLOAT) {
        std::array<uint16_t, 2> components{};
        std::memcpy(components.data(), pixel, sizeof(components));
        static_assert(sizeof(_Float16) == sizeof(uint16_t));
        red = float(std::bit_cast<_Float16>(components[0]));
        green = float(std::bit_cast<_Float16>(components[1]));
      } else if (stage.format == VK_FORMAT_R32_SFLOAT ||
                 stage.format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
        std::memcpy(&red, pixel, sizeof(red));
        green = red;
        blue = red;
      } else if (stage.format == VK_FORMAT_B8G8R8A8_UNORM) {
        red = float(pixel[2]) / 255.0f;
        green = float(pixel[1]) / 255.0f;
        blue = float(pixel[0]) / 255.0f;
      } else if (stage.format == VK_FORMAT_R8G8B8A8_UNORM) {
        red = float(pixel[0]) / 255.0f;
        green = float(pixel[1]) / 255.0f;
        blue = float(pixel[2]) / 255.0f;
      } else {
        std::array<uint32_t, 4> components{};
        std::memcpy(components.data(), pixel, sizeof(components));
        red = std::bit_cast<float>(components[0]);
        green = std::bit_cast<float>(components[1]);
        blue = std::bit_cast<float>(components[2]);
      }
      if (!std::isfinite(red) || !std::isfinite(green) || !std::isfinite(blue)) {
        ++nonfinite_samples;
      }
      if (!std::isfinite(red)) red = 0.0f;
      if (!std::isfinite(green)) green = 0.0f;
      if (!std::isfinite(blue)) blue = 0.0f;
      const float luminance = red * 0.2126f + green * 0.7152f + blue * 0.0722f;
      luminances[sample] = luminance;
      luminance_sum += double(luminance);
      minimum_luminance = std::min(minimum_luminance, luminance);
      maximum_luminance = std::max(maximum_luminance, luminance);
      if (luminance < 0.0f) {
        ++negative_samples;
      } else if (luminance == 0.0f) {
        ++black_samples;
      } else if (luminance < kContentProbeOneCodeValue) {
        ++sub_code_samples;
      } else if (luminance < kContentProbeDarkThreshold) {
        ++dark_samples;
      } else if (luminance < kContentProbeDimThreshold) {
        ++dim_samples;
      } else if (luminance >= kContentProbeBrightThreshold) {
        ++bright_samples;
      }
      if (red != 0.0f || green != 0.0f || blue != 0.0f) {
        ++nonzero_samples;
      }
      if (red >= kContentProbeVisibleLinearThreshold ||
          green >= kContentProbeVisibleLinearThreshold ||
          blue >= kContentProbeVisibleLinearThreshold) {
        ++visible_samples;
      }
    }
    std::sort(luminances.begin(), luminances.end());
    const float mean_luminance =
        float(luminance_sum / double(kContentProbeSampleCount));
    const float p10_luminance = luminances[kContentProbeP10Index];
    const float p50_luminance = luminances[kContentProbeP50Index];
    const float p90_luminance = luminances[kContentProbeP90Index];
    const float p99_luminance = luminances[kContentProbeP99Index];
    const uint64_t checksum = XXH3_64bits(stage_data, size_t(kContentProbeStageStride));
    if (stage.kind < e2e_stage_seen.size()) {
      e2e_stage_seen[stage.kind] = true;
      ++e2e_image_count[stage.kind];
      e2e_nonzero_samples[stage.kind] += nonzero_samples;
      e2e_visible_samples[stage.kind] += visible_samples;
      e2e_maximum_luminance[stage.kind] =
          std::max(e2e_maximum_luminance[stage.kind], maximum_luminance);
      e2e_p90_luminance[stage.kind] =
          std::max(e2e_p90_luminance[stage.kind], p90_luminance);
    }
    REXLOG_WARN(
        "gta4-native-content: frame={} stage={} handle={:08X} address={:08X} format={} "
        "mip={} size={}x{} "
        "nonzero={}/{} visible={}/{} nonfinite={} negative={} "
        "bins=black:{},sub-code:{},dark:{},dim:{},bright:{} "
        "luma=min:{:.6f},mean:{:.6f},p90:{:.6f},p99:{:.6f},max:{:.6f} checksum={:016X}",
        content_probe_buffer_.pending_frame, stage_name, stage.handle, stage.address,
        uint32_t(stage.format), stage.mip_level, stage.width, stage.height, nonzero_samples,
        kContentProbeSampleCount, visible_samples, kContentProbeSampleCount, nonfinite_samples,
        negative_samples, black_samples, sub_code_samples, dark_samples, dim_samples,
        bright_samples,
        std::isfinite(minimum_luminance) ? minimum_luminance : 0.0f, mean_luminance,
        p90_luminance, p99_luminance, maximum_luminance, checksum);
    if (!stage.diagnostic_role.empty()) {
      const bool target_checksum =
          stage.kind == 44 || stage.kind == 45 || stage.kind == 48 ||
          stage.kind == 49;
      REXLOG_WARN(
          "gta4-native-cause: point={} frame={} cmd={} draw={:016X} category={} "
          "role={} stage={} wrapper={:08X} resource={:08X}/{:08X}@{} samples={} "
          "aspect={} format={} size={}x{} min={:.9g} max={:.9g} "
          "nonzero={}/{} checksum={:016X} vs={:016X} ps={:016X} "
          "provenance={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
          target_checksum ? "depth-lifecycle-target-checksum"
                          : "depth-lifecycle-value",
          content_probe_buffer_.pending_frame, stage.command_index, stage.draw_id,
          stage.diagnostic_category, stage.diagnostic_role, stage_name,
          stage.trace_wrapper, stage.handle, stage.address,
          stage.resource_generation, stage.sample_count, uint32_t(stage.aspect),
          uint32_t(stage.format), stage.width, stage.height,
          std::isfinite(minimum_luminance) ? minimum_luminance : 0.0f,
          maximum_luminance, nonzero_samples, kContentProbeSampleCount, checksum,
          stage.vertex_shader_hash, stage.pixel_shader_hash,
          stage.provenance_handle, stage.provenance_frame,
          stage.provenance_command, stage.provenance_serial,
          stage.provenance_phase, stage.provenance_kind);
    }
    if (stage.kind == 8 || stage.kind == 9) {
      REXLOG_WARN(
          "gta4-native-scene-checkpoint: frame={} stage={} checkpoint={}/{} cmd={} command-type={} "
          "draw={:016X} phase={} target={:08X}/{:08X} vs={:016X} ps={:016X} "
          "textures={:08X},{:08X},{:08X},{:08X} nonfinite={} checksum={:016X}",
          content_probe_buffer_.pending_frame, stage_name, stage.checkpoint_ordinal,
          stage.checkpoint_count, stage.command_index, uint32_t(stage.command_type),
          stage.draw_id, stage.render_phase, stage.handle, stage.address,
          stage.vertex_shader_hash, stage.pixel_shader_hash, stage.texture_handles[0],
          stage.texture_handles[1], stage.texture_handles[2], stage.texture_handles[3],
          nonfinite_samples, checksum);
      if (!scene_checkpoint_nonfinite_seen && nonfinite_samples) {
        REXLOG_ERROR(
            "gta4-native-cause: point=scene-first-nonfinite-checkpoint frame={} "
            "stage={} checkpoint={}/{} cmd={} previous-cmd={} previous-nonfinite={} "
            "command-type={} draw={:016X} phase={} target={:08X}/{:08X} "
            "vs={:016X} ps={:016X} textures={:08X},{:08X},{:08X},{:08X} samples={}",
            content_probe_buffer_.pending_frame, stage_name, stage.checkpoint_ordinal,
            stage.checkpoint_count, stage.command_index,
            scene_checkpoint_seen ? previous_scene_checkpoint_command : UINT32_MAX,
            scene_checkpoint_seen ? previous_scene_checkpoint_nonfinite : 0,
            uint32_t(stage.command_type), stage.draw_id, stage.render_phase, stage.handle,
            stage.address, stage.vertex_shader_hash, stage.pixel_shader_hash,
            stage.texture_handles[0], stage.texture_handles[1], stage.texture_handles[2],
            stage.texture_handles[3], nonfinite_samples);
        scene_checkpoint_nonfinite_seen = true;
      }
      scene_checkpoint_seen = true;
      previous_scene_checkpoint_command = stage.command_index;
      previous_scene_checkpoint_nonfinite = nonfinite_samples;
    }
    if (stage.kind == 10) {
      REXLOG_WARN(
          "gta4-native-scene-input: frame={} cmd={} texture-stage={} "
          "handle={:08X} format={} size={}x{} vs={:016X} ps={:016X} "
          "nonfinite={} checksum={:016X}",
          content_probe_buffer_.pending_frame, stage.command_index, stage.texture_stage,
          stage.handle, uint32_t(stage.format), stage.width, stage.height,
          stage.vertex_shader_hash, stage.pixel_shader_hash, nonfinite_samples, checksum);
    }
    if (stage.kind >= 12 && stage.kind <= 14) {
      REXLOG_WARN(
          "gta4-native-reflection-trace: point=mip-probe frame={} cmd={} stage={} "
          "handle={:08X} address={:08X} mip={} format={} size={}x{} nonzero={}/{} "
          "visible={}/{} nonfinite={} negative={} "
          "luma=min:{:.9g},mean:{:.9g},p90:{:.9g},p99:{:.9g},max:{:.9g} checksum={:016X}",
          content_probe_buffer_.pending_frame, stage.command_index, stage_name, stage.handle,
          stage.address, stage.mip_level, uint32_t(stage.format), stage.width, stage.height,
          nonzero_samples, kContentProbeSampleCount, visible_samples, kContentProbeSampleCount,
          nonfinite_samples, negative_samples,
          std::isfinite(minimum_luminance) ? minimum_luminance : 0.0f, mean_luminance,
          p90_luminance, p99_luminance, maximum_luminance, checksum);
    }
    if (stage.kind >= 15 && stage.kind <= 18) {
      REXLOG_WARN(
          "gta4-native-cause: point=translucent-content-probe frame={} cmd={} draw={:016X} "
          "category={} role={} handle={:08X} address={:08X} texture-stage={} "
          "format={} size={}x{} nonzero={}/{} visible={}/{} nonfinite={} negative={} "
          "luma=min:{:.9g},mean:{:.9g},p90:{:.9g},p99:{:.9g},max:{:.9g} "
          "checksum={:016X} vs={:016X} ps={:016X}",
          content_probe_buffer_.pending_frame, stage.command_index, stage.draw_id,
          stage.diagnostic_category, stage_name, stage.handle, stage.address,
          stage.texture_stage, uint32_t(stage.format), stage.width, stage.height,
          nonzero_samples, kContentProbeSampleCount, visible_samples,
          kContentProbeSampleCount, nonfinite_samples, negative_samples,
          std::isfinite(minimum_luminance) ? minimum_luminance : 0.0f,
          mean_luminance, p90_luminance, p99_luminance, maximum_luminance,
          checksum, stage.vertex_shader_hash, stage.pixel_shader_hash);
    }
    if (nonfinite_samples) {
      REXLOG_ERROR(
          "gta4-native-cause: point=nonfinite-content frame={} stage={} handle={:08X} "
          "address={:08X} format={} size={}x{} samples={} "
          "raw0={:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
          content_probe_buffer_.pending_frame, stage_name, stage.handle, stage.address,
          uint32_t(stage.format), stage.width, stage.height, nonfinite_samples, stage_data[0],
          stage_data[1], stage_data[2], stage_data[3], stage_data[4], stage_data[5],
          stage_data[6], stage_data[7]);
    }
    std::fprintf(
        stderr,
        "[PixelStage] frame=%u stage=%s handle=%08X address=%08X format=%u mip=%u size=%ux%u "
        "nonzero=%u visible=%u nonfinite=%u luma=%.6f/%.6f checksum=%016llX "
        "raw0=%02X%02X%02X%02X%02X%02X%02X%02X\n",
        content_probe_buffer_.pending_frame, stage_name, stage.handle, stage.address,
        uint32_t(stage.format), stage.mip_level, stage.width, stage.height, nonzero_samples,
        visible_samples, nonfinite_samples,
        double(std::isfinite(minimum_luminance) ? minimum_luminance : 0.0f),
        double(maximum_luminance), static_cast<unsigned long long>(checksum),
        stage_data[0], stage_data[1], stage_data[2], stage_data[3],
        stage_data[4], stage_data[5], stage_data[6], stage_data[7]);
    std::fprintf(
        stderr,
        "[NativePixels] frame=%u stage=%s handle=%08X address=%08X format=%u size=%ux%u "
        "samples=%u nonfinite=%u negative=%u bins=black:%u,sub-code:%u,dark:%u,dim:%u,bright:%u "
        "luma=min:%.9g,mean:%.9g,p10:%.9g,p50:%.9g,p90:%.9g,p99:%.9g,max:%.9g "
        "checksum=%016llX\n",
        content_probe_buffer_.pending_frame, stage_name, stage.handle, stage.address,
        uint32_t(stage.format), stage.width, stage.height, kContentProbeSampleCount,
        nonfinite_samples, negative_samples, black_samples, sub_code_samples, dark_samples,
        dim_samples, bright_samples,
        double(std::isfinite(minimum_luminance) ? minimum_luminance : 0.0f),
        double(mean_luminance), double(p10_luminance), double(p50_luminance),
        double(p90_luminance), double(p99_luminance), double(maximum_luminance),
        static_cast<unsigned long long>(checksum));
  }
  const bool upstream_rgb =
      (e2e_stage_seen[0] && e2e_visible_samples[0]) ||
      (e2e_stage_seen[1] && e2e_visible_samples[1]) ||
      (e2e_stage_seen[4] && e2e_visible_samples[4]);
  const bool frontbuffer_rgb = e2e_stage_seen[2] && e2e_visible_samples[2];
  const bool presenter_rgb = e2e_stage_seen[3] && e2e_visible_samples[3];
  const char* verdict =
      !e2e_stage_seen[3] ? "presenter-missing"
      : presenter_rgb    ? "rgb-reached-presenter"
      : frontbuffer_rgb  ? "lost-frontbuffer-to-presenter"
      : upstream_rgb     ? "lost-before-frontbuffer"
                         : "black-entered-native-pipeline";
  REXLOG_WARN(
      "[PixelE2E] frame={} verdict={} "
      "final={}:{}:{} input={}:{}:{} retained={}:{}:{}:{} frontbuffer={}:{}:{} "
      "presenter={}:{}:{} max-luma={:.6f}/{:.6f}/{:.6f}/{:.6f}/{:.6f}",
      content_probe_buffer_.pending_frame, verdict,
      e2e_stage_seen[0], e2e_nonzero_samples[0], e2e_visible_samples[0],
      e2e_stage_seen[4], e2e_nonzero_samples[4], e2e_visible_samples[4],
      e2e_stage_seen[1], e2e_image_count[1], e2e_nonzero_samples[1], e2e_visible_samples[1],
      e2e_stage_seen[2], e2e_nonzero_samples[2], e2e_visible_samples[2],
      e2e_stage_seen[3], e2e_nonzero_samples[3], e2e_visible_samples[3],
      e2e_maximum_luminance[0], e2e_maximum_luminance[4],
      e2e_maximum_luminance[1], e2e_maximum_luminance[2], e2e_maximum_luminance[3]);
  std::fprintf(
      stderr,
      "[PixelE2E] frame=%u verdict=%s final=%u:%u:%u input=%u:%u:%u "
      "retained=%u:%u:%u:%u frontbuffer=%u:%u:%u presenter=%u:%u:%u "
      "max-luma=%.6f/%.6f/%.6f/%.6f/%.6f\n",
      content_probe_buffer_.pending_frame, verdict,
      unsigned(e2e_stage_seen[0]), e2e_nonzero_samples[0], e2e_visible_samples[0],
      unsigned(e2e_stage_seen[4]), e2e_nonzero_samples[4], e2e_visible_samples[4],
      unsigned(e2e_stage_seen[1]), e2e_image_count[1], e2e_nonzero_samples[1],
      e2e_visible_samples[1],
      unsigned(e2e_stage_seen[2]), e2e_nonzero_samples[2], e2e_visible_samples[2],
      unsigned(e2e_stage_seen[3]), e2e_nonzero_samples[3], e2e_visible_samples[3],
      double(e2e_maximum_luminance[0]), double(e2e_maximum_luminance[4]),
      double(e2e_maximum_luminance[1]), double(e2e_maximum_luminance[2]),
      double(e2e_maximum_luminance[3]));
  float strongest_upstream_p90 = 0.0f;
  for (const uint32_t kind : {0u, 1u, 4u, 5u, 6u, 7u}) {
    strongest_upstream_p90 = std::max(strongest_upstream_p90, e2e_p90_luminance[kind]);
  }
  const float final_p90 = e2e_p90_luminance[0];
  const float input_p90 = e2e_p90_luminance[4];
  const float frontbuffer_p90 = e2e_p90_luminance[2];
  const float presenter_p90 = e2e_p90_luminance[3];
  const char* luminance_cause =
      e2e_stage_seen[4] && e2e_stage_seen[0] && input_p90 >= kContentProbeDimThreshold &&
              final_p90 < kContentProbeDarkThreshold
          ? "crushed-in-final-composite"
      : strongest_upstream_p90 >= kContentProbeDimThreshold && e2e_stage_seen[2] &&
              frontbuffer_p90 < kContentProbeDarkThreshold
          ? "crushed-before-frontbuffer"
      : e2e_stage_seen[2] && frontbuffer_p90 >= kContentProbeDimThreshold &&
              e2e_stage_seen[3] && presenter_p90 < kContentProbeDarkThreshold
          ? "crushed-during-presentation"
      : e2e_stage_seen[3] && presenter_p90 < kContentProbeDarkThreshold
          ? "dark-through-entire-pipeline"
      : e2e_stage_seen[3] && presenter_p90 < kContentProbeDimThreshold
          ? "dim-through-entire-pipeline"
          : "luminance-reaches-presenter";
  REXLOG_WARN(
      "[NativeCause] frame={} cause={} "
      "p90=input:{:.9g},final:{:.9g},upstream:{:.9g},frontbuffer:{:.9g},presenter:{:.9g} "
      "thresholds=dark:{:.9g},dim:{:.9g}",
      content_probe_buffer_.pending_frame, luminance_cause, input_p90, final_p90,
      strongest_upstream_p90, frontbuffer_p90, presenter_p90,
      kContentProbeDarkThreshold, kContentProbeDimThreshold);
  std::fprintf(
      stderr,
      "[NativeCause] frame=%u cause=%s p90=input:%.9g,final:%.9g,upstream:%.9g,frontbuffer:%.9g,presenter:%.9g "
      "thresholds=dark:%.9g,dim:%.9g\n",
      content_probe_buffer_.pending_frame, luminance_cause, double(input_p90),
      double(final_p90), double(strongest_upstream_p90), double(frontbuffer_p90),
      double(presenter_p90), double(kContentProbeDarkThreshold),
      double(kContentProbeDimThreshold));
  std::fflush(stderr);
  content_probe_buffer_.pending_frame = 0;
  content_probe_buffer_.stages = {};
}

bool Gta4NativeGraphicsSystem::RecordContentProbeImage(
    VkCommandBuffer command_buffer, uint32_t stage_index, VkImage image, VkFormat format,
    VkImageLayout layout, uint32_t width, uint32_t height, uint32_t handle,
    uint32_t address, uint8_t kind, uint32_t mip_level,
    VkImageAspectFlags aspect_override) {
  if (!image || layout == VK_IMAGE_LAYOUT_UNDEFINED || !width || !height ||
      stage_index >= content_probe_buffer_.stages.size()) {
    return false;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto& dfn = vulkan_provider->vulkan_device()->functions();
  NativeContentProbeStage& stage = content_probe_buffer_.stages[stage_index];
  stage.valid = true;
  stage.kind = kind;
  stage.format = format;
  stage.width = width;
  stage.height = height;
  stage.mip_level = mip_level;
  stage.handle = handle;
  stage.address = address;

  const VkImageAspectFlags probe_aspect =
      aspect_override
          ? aspect_override
          : format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
                    format == VK_FORMAT_D24_UNORM_S8_UINT
                ? VK_IMAGE_ASPECT_DEPTH_BIT
                : VK_IMAGE_ASPECT_COLOR_BIT;
  stage.aspect = probe_aspect;

  VkImageMemoryBarrier image_barrier{};
  image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  image_barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  image_barrier.oldLayout = layout;
  image_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  image_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  image_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  image_barrier.image = image;
  image_barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(probe_aspect, mip_level, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &image_barrier);

  std::array<VkBufferImageCopy, kContentProbeSampleCount> copies{};
  for (uint32_t sample_y = 0; sample_y < kContentProbeAxis; ++sample_y) {
    for (uint32_t sample_x = 0; sample_x < kContentProbeAxis; ++sample_x) {
      const uint32_t sample = sample_y * kContentProbeAxis + sample_x;
      VkBufferImageCopy& copy = copies[sample];
      copy.bufferOffset = VkDeviceSize(stage_index) * kContentProbeStageStride +
                          VkDeviceSize(sample) * kContentProbeSampleStride;
      copy.imageSubresource.aspectMask = probe_aspect;
      copy.imageSubresource.mipLevel = mip_level;
      copy.imageSubresource.layerCount = 1;
      copy.imageOffset.x = int32_t((uint64_t(sample_x) * width) / kContentProbeAxis +
                                   width / (kContentProbeAxis * 2));
      copy.imageOffset.y = int32_t((uint64_t(sample_y) * height) / kContentProbeAxis +
                                   height / (kContentProbeAxis * 2));
      copy.imageExtent = {1, 1, 1};
    }
  }
  dfn.vkCmdCopyImageToBuffer(command_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                             content_probe_buffer_.buffer, uint32_t(copies.size()),
                             copies.data());
  image_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  image_barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  image_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  image_barrier.newLayout = layout;
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &image_barrier);
  return true;
}

bool Gta4NativeGraphicsSystem::RecordDepthStencilDiagnosticProbe(
    VkCommandBuffer command_buffer, NativeSurfaceImage& image,
    uint8_t depth_kind, uint8_t stencil_kind, std::string_view role,
    uint32_t trace_wrapper, uint64_t resource_generation,
    const NativePlacementOwner* owner) {
  if (!ShouldLogDiagnosticFrame(diagnostic_submitted_frame_) ||
      !(image.aspect & VK_IMAGE_ASPECT_DEPTH_BIT) ||
      !(image.aspect & VK_IMAGE_ASPECT_STENCIL_BIT) ||
      !InitializeContentProbeBuffer()) {
    return false;
  }
  if (content_probe_buffer_.pending_frame != diagnostic_submitted_frame_) {
    std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
    content_probe_buffer_.pending_frame = diagnostic_submitted_frame_;
    content_probe_buffer_.stages = {};
  }
  uint32_t depth_stage = 0;
  while (depth_stage < content_probe_buffer_.stages.size() &&
         content_probe_buffer_.stages[depth_stage].valid) {
    ++depth_stage;
  }
  const uint32_t stencil_stage = depth_stage + 1;
  if (stencil_stage >= content_probe_buffer_.stages.size()) {
    return false;
  }
  const bool depth_recorded = RecordContentProbeImage(
      command_buffer, depth_stage, image.resource.image, image.format, image.layout,
      image.width, image.height, image.descriptor.handle, image.descriptor.address,
      depth_kind, 0, VK_IMAGE_ASPECT_DEPTH_BIT);
  const bool stencil_recorded = RecordContentProbeImage(
      command_buffer, stencil_stage, image.resource.image, image.format, image.layout,
      image.width, image.height, image.descriptor.handle, image.descriptor.address,
      stencil_kind, 0, VK_IMAGE_ASPECT_STENCIL_BIT);
  if (depth_recorded) {
    NativeContentProbeStage& stage = content_probe_buffer_.stages[depth_stage];
    stage.command_index = uint32_t(diagnostic_command_index_);
    stage.trace_wrapper = trace_wrapper;
    stage.sample_count = uint32_t(VK_SAMPLE_COUNT_1_BIT);
    stage.resource_generation = resource_generation;
    stage.provenance_handle = owner && owner->image
                                  ? owner->image->descriptor.handle
                                  : 0;
    stage.provenance_frame = owner ? owner->frame : 0;
    stage.provenance_command = owner ? uint32_t(owner->command_index) : UINT32_MAX;
    stage.provenance_serial = owner ? owner->serial : 0;
    stage.provenance_phase = owner ? uint32_t(owner->render_phase) : 0;
    stage.provenance_kind = owner ? uint32_t(owner->write_kind) : 0;
    stage.diagnostic_role = role;
  }
  if (stencil_recorded) {
    NativeContentProbeStage& stage = content_probe_buffer_.stages[stencil_stage];
    stage.command_index = uint32_t(diagnostic_command_index_);
    stage.trace_wrapper = trace_wrapper;
    stage.sample_count = uint32_t(VK_SAMPLE_COUNT_1_BIT);
    stage.resource_generation = resource_generation;
    stage.provenance_handle = owner && owner->image
                                  ? owner->image->descriptor.handle
                                  : 0;
    stage.provenance_frame = owner ? owner->frame : 0;
    stage.provenance_command = owner ? uint32_t(owner->command_index) : UINT32_MAX;
    stage.provenance_serial = owner ? owner->serial : 0;
    stage.provenance_phase = owner ? uint32_t(owner->render_phase) : 0;
    stage.provenance_kind = owner ? uint32_t(owner->write_kind) : 0;
    stage.diagnostic_role = role;
  }
  return depth_recorded && stencil_recorded;
}

bool Gta4NativeGraphicsSystem::RecordDepthStencilDiagnosticProbe(
    VkCommandBuffer command_buffer, NativeTextureImage& image,
    uint8_t depth_kind, uint8_t stencil_kind, std::string_view role,
    uint32_t trace_wrapper, uint64_t resource_generation,
    const NativePlacementOwner* owner) {
  if (!ShouldLogDiagnosticFrame(diagnostic_submitted_frame_) ||
      !(image.aspect & VK_IMAGE_ASPECT_DEPTH_BIT) ||
      !(image.aspect & VK_IMAGE_ASPECT_STENCIL_BIT) || !image.source ||
      !InitializeContentProbeBuffer()) {
    return false;
  }
  if (content_probe_buffer_.pending_frame != diagnostic_submitted_frame_) {
    std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
    content_probe_buffer_.pending_frame = diagnostic_submitted_frame_;
    content_probe_buffer_.stages = {};
  }
  uint32_t depth_stage = 0;
  while (depth_stage < content_probe_buffer_.stages.size() &&
         content_probe_buffer_.stages[depth_stage].valid) {
    ++depth_stage;
  }
  const uint32_t stencil_stage = depth_stage + 1;
  if (stencil_stage >= content_probe_buffer_.stages.size()) {
    return false;
  }
  const uint32_t address = image.source->info.memory.base_address;
  const bool depth_recorded = RecordContentProbeImage(
      command_buffer, depth_stage, image.resource.image, image.format, image.layout,
      image.width, image.height, image.source->handle, address, depth_kind, 0,
      VK_IMAGE_ASPECT_DEPTH_BIT);
  const bool stencil_recorded = RecordContentProbeImage(
      command_buffer, stencil_stage, image.resource.image, image.format, image.layout,
      image.width, image.height, image.source->handle, address, stencil_kind, 0,
      VK_IMAGE_ASPECT_STENCIL_BIT);
  if (depth_recorded) {
    NativeContentProbeStage& stage = content_probe_buffer_.stages[depth_stage];
    stage.command_index = uint32_t(diagnostic_command_index_);
    stage.trace_wrapper = trace_wrapper;
    stage.sample_count = uint32_t(VK_SAMPLE_COUNT_1_BIT);
    stage.resource_generation = resource_generation;
    stage.provenance_handle = owner && owner->image
                                  ? owner->image->descriptor.handle
                                  : 0;
    stage.provenance_frame = owner ? owner->frame : 0;
    stage.provenance_command = owner ? uint32_t(owner->command_index) : UINT32_MAX;
    stage.provenance_serial = owner ? owner->serial : 0;
    stage.provenance_phase = owner ? uint32_t(owner->render_phase) : 0;
    stage.provenance_kind = owner ? uint32_t(owner->write_kind) : 0;
    stage.diagnostic_role = role;
  }
  if (stencil_recorded) {
    NativeContentProbeStage& stage = content_probe_buffer_.stages[stencil_stage];
    stage.command_index = uint32_t(diagnostic_command_index_);
    stage.trace_wrapper = trace_wrapper;
    stage.sample_count = uint32_t(VK_SAMPLE_COUNT_1_BIT);
    stage.resource_generation = resource_generation;
    stage.provenance_handle = owner && owner->image
                                  ? owner->image->descriptor.handle
                                  : 0;
    stage.provenance_frame = owner ? owner->frame : 0;
    stage.provenance_command = owner ? uint32_t(owner->command_index) : UINT32_MAX;
    stage.provenance_serial = owner ? owner->serial : 0;
    stage.provenance_phase = owner ? uint32_t(owner->render_phase) : 0;
    stage.provenance_kind = owner ? uint32_t(owner->write_kind) : 0;
    stage.diagnostic_role = role;
  }
  return depth_recorded && stencil_recorded;
}

bool Gta4NativeGraphicsSystem::RecordStencilDiagnosticProbe(
    VkCommandBuffer command_buffer, NativeSurfaceImage& image, uint8_t kind) {
  if (!ShouldLogDiagnosticFrame(diagnostic_submitted_frame_) ||
      !(image.aspect & VK_IMAGE_ASPECT_STENCIL_BIT) ||
      !InitializeContentProbeBuffer()) {
    return false;
  }
  if (content_probe_buffer_.pending_frame != diagnostic_submitted_frame_) {
    std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
    content_probe_buffer_.pending_frame = diagnostic_submitted_frame_;
    content_probe_buffer_.stages = {};
  }
  uint32_t stage_index = 0;
  while (stage_index < content_probe_buffer_.stages.size() &&
         content_probe_buffer_.stages[stage_index].valid) {
    ++stage_index;
  }
  if (stage_index >= content_probe_buffer_.stages.size()) {
    return false;
  }
  const bool recorded = RecordContentProbeImage(
      command_buffer, stage_index, image.resource.image, image.format, image.layout,
      image.width, image.height, image.descriptor.handle, image.descriptor.address,
      kind, 0, VK_IMAGE_ASPECT_STENCIL_BIT);
  if (recorded) {
    content_probe_buffer_.stages[stage_index].command_index =
        uint32_t(diagnostic_command_index_);
  }
  return recorded;
}

bool Gta4NativeGraphicsSystem::RecordContentProbe(
    VkCommandBuffer command_buffer, uint32_t submitted_frame, NativeSurfaceImage* final_surface,
    NativeTextureImage* final_composite_input,
    const std::shared_ptr<const NativeTextureResource>& present_source, VkImage presenter_image,
    VkImageLayout presenter_layout, uint32_t presenter_width, uint32_t presenter_height) {
  if (!ShouldProbeContentFrame(submitted_frame) || !InitializeContentProbeBuffer()) {
    return false;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  if (content_probe_buffer_.pending_frame != submitted_frame) {
    std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
    content_probe_buffer_.pending_frame = submitted_frame;
    content_probe_buffer_.stages = {};
  }

  NativeTextureImage* frontbuffer = nullptr;
  if (present_source) {
    auto entry = native_texture_images_.find(present_source->generation);
    if (entry != native_texture_images_.end()) {
      frontbuffer = entry->second.get();
    }
  }

  struct ProbeSource {
    VkImage image;
    VkFormat format;
    VkImageLayout layout;
    uint32_t width;
    uint32_t height;
    uint32_t handle;
    uint8_t kind;
  };
  std::array<ProbeSource, kContentProbeMaximumStages> sources{};
  uint32_t source_count = 0;
  while (source_count < content_probe_buffer_.stages.size() &&
         content_probe_buffer_.stages[source_count].valid) {
    ++source_count;
  }
  auto append_source = [&](VkImage image, VkFormat format, VkImageLayout layout, uint32_t width,
                           uint32_t height, uint32_t handle, uint8_t kind) {
    if (!image || layout == VK_IMAGE_LAYOUT_UNDEFINED || !width || !height ||
        source_count >= sources.size()) {
      return;
    }
    sources[source_count++] = {image, format, layout, width, height, handle, kind};
  };

  append_source(final_surface ? final_surface->resource.image : VK_NULL_HANDLE,
                final_surface ? final_surface->format : VK_FORMAT_UNDEFINED,
                final_surface ? final_surface->layout : VK_IMAGE_LAYOUT_UNDEFINED,
                final_surface ? final_surface->width : 0, final_surface ? final_surface->height : 0,
                final_surface ? final_surface->descriptor.handle : 0, 0);
  append_source(final_composite_input ? final_composite_input->resource.image : VK_NULL_HANDLE,
                final_composite_input ? final_composite_input->format : VK_FORMAT_UNDEFINED,
                final_composite_input ? final_composite_input->layout : VK_IMAGE_LAYOUT_UNDEFINED,
                final_composite_input ? final_composite_input->width : 0,
                final_composite_input ? final_composite_input->height : 0,
                final_composite_input && final_composite_input->source
                    ? final_composite_input->source->handle
                    : 0,
                4);
  const uint32_t target_width = final_surface ? final_surface->width : presenter_width;
  const uint32_t target_height = final_surface ? final_surface->height : presenter_height;
  for (const auto& surface : native_surface_images_) {
    if (!surface->ever_written || surface->resource.image == (final_surface ? final_surface->resource.image
                                                                           : VK_NULL_HANDLE) ||
        surface->aspect != VK_IMAGE_ASPECT_COLOR_BIT || surface->samples != VK_SAMPLE_COUNT_1_BIT ||
        surface->width != target_width || surface->height != target_height) {
      continue;
    }
    append_source(surface->resource.image, surface->format, surface->layout, surface->width,
                  surface->height, surface->descriptor.handle, 1);
  }
  append_source(frontbuffer ? frontbuffer->resource.image : VK_NULL_HANDLE,
                frontbuffer ? frontbuffer->format : VK_FORMAT_UNDEFINED,
                frontbuffer ? frontbuffer->layout : VK_IMAGE_LAYOUT_UNDEFINED,
                present_source ? present_source->info.width + 1 : 0,
                present_source ? present_source->info.height + 1 : 0,
                present_source ? present_source->handle : 0, 2);
  append_source(presenter_image, ui::vulkan::VulkanPresenter::kGuestOutputFormat, presenter_layout,
                presenter_width, presenter_height, 0, 3);

  if (REXCVAR_GET(gta4_trace_startup_content) &&
      submitted_frame <= kStartupContentProbeFrameLimit) {
    REXLOG_WARN(
        "gta4-startup-content-map: frame={} commands={} final={:08X} present={:08X}@{} "
        "sources={} presenter={}x{}",
        submitted_frame, current_frame_.size(),
        final_surface ? final_surface->descriptor.handle : 0,
        present_source ? present_source->handle : 0,
        present_source ? present_source->generation : 0, source_count, presenter_width,
        presenter_height);
  }

  bool recorded = false;
  for (uint32_t stage_index = 0; stage_index < source_count; ++stage_index) {
    const ProbeSource& source = sources[stage_index];
    if (!source.image || source.layout == VK_IMAGE_LAYOUT_UNDEFINED || !source.width ||
        !source.height) {
      continue;
    }
    NativeContentProbeStage& stage = content_probe_buffer_.stages[stage_index];
    stage.valid = true;
    stage.format = source.format;
    stage.width = source.width;
    stage.height = source.height;
    recorded |= RecordContentProbeImage(command_buffer, stage_index, source.image, source.format,
                                        source.layout, source.width, source.height, source.handle,
                                        0, source.kind);
  }

  if (recorded) {
    VkBufferMemoryBarrier buffer_barrier{};
    buffer_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    buffer_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    buffer_barrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
    buffer_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    buffer_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    buffer_barrier.buffer = content_probe_buffer_.buffer;
    buffer_barrier.size = VK_WHOLE_SIZE;
    dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_HOST_BIT, 0, 0, nullptr, 1, &buffer_barrier, 0,
                             nullptr);
  } else {
    content_probe_buffer_.pending_frame = 0;
  }
  return recorded;
}

bool Gta4NativeGraphicsSystem::CreateNullImage(VkImageType image_type, VkImageViewType view_type,
                                               uint32_t array_layers, VkImageCreateFlags flags,
                                               NativeImageResource& resource) {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }

  VkImageCreateInfo image_info{};
  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.flags = flags;
  image_info.imageType = image_type;
  image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
  image_info.extent = {1, 1, 1};
  image_info.mipLevels = 1;
  image_info.arrayLayers = array_layers;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (!ui::vulkan::util::CreateDedicatedAllocationImage(
          vulkan_device, image_info, ui::vulkan::util::MemoryPurpose::kDeviceLocal, resource.image,
          resource.memory)) {
    return false;
  }

  VkImageViewCreateInfo view_info{};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.image = resource.image;
  view_info.viewType = view_type;
  view_info.format = image_info.format;
  view_info.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, array_layers);
  return vulkan_device->functions().vkCreateImageView(vulkan_device->device(), &view_info, nullptr,
                                                      &resource.view) == VK_SUCCESS;
}

bool Gta4NativeGraphicsSystem::CreateNativeDescriptors() {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  const auto& limits = vulkan_device->properties();
  const uint32_t required_stage_resources = kShaderTextureCount * kDrawDescriptorSetCount + 1;
  if (limits.maxPerStageDescriptorSampledImages < kShaderTextureCount * 4 ||
      limits.maxPerStageResources < required_stage_resources) {
    REXLOG_ERROR("gta4-native: Vulkan sampled-image descriptor capacity {} is below {}",
                 limits.maxPerStageDescriptorSampledImages, kShaderTextureCount * 4);
    return false;
  }
  native_descriptor_capacity_ = kShaderTextureCount;

  for (uint32_t set = 0; set < kDescriptorSetCount; ++set) {
    VkDescriptorSetLayoutBinding binding{};
    binding.binding = 0;
    binding.descriptorCount = set < 5 ? native_descriptor_capacity_ : 1;
    binding.descriptorType = set < 4    ? VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
                             : set == 4 ? VK_DESCRIPTOR_TYPE_SAMPLER
                                        : VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    VkDescriptorBindingFlags binding_flags =
        set < 5 ? VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT : 0;
    VkDescriptorSetLayoutBindingFlagsCreateInfo binding_flags_info{};
    binding_flags_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    binding_flags_info.bindingCount = 1;
    binding_flags_info.pBindingFlags = &binding_flags;
    layout_info.pNext = set < 5 ? &binding_flags_info : nullptr;
    layout_info.bindingCount = 1;
    layout_info.pBindings = &binding;
    if (dfn.vkCreateDescriptorSetLayout(device, &layout_info, nullptr,
                                        &descriptor_set_layouts_[set]) != VK_SUCCESS) {
      return false;
    }
  }

  VkDescriptorPoolSize pool_size{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1};
  VkDescriptorPoolCreateInfo pool_info{};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &pool_size;
  if (dfn.vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptor_pool_) != VK_SUCCESS) {
    return false;
  }

  VkDescriptorSetAllocateInfo set_allocate_info{};
  set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  set_allocate_info.descriptorPool = descriptor_pool_;
  set_allocate_info.descriptorSetCount = 1;
  set_allocate_info.pSetLayouts = &descriptor_set_layouts_[5];
  if (dfn.vkAllocateDescriptorSets(device, &set_allocate_info, &descriptor_sets_[5]) !=
      VK_SUCCESS) {
    return false;
  }

  VkDescriptorBufferInfo buffer_info{};
  buffer_info.buffer = upload_buffer_.buffer;
  buffer_info.offset = 0;
  buffer_info.range = upload_buffer_.capacity;

  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.dstSet = descriptor_sets_[5];
  write.dstBinding = 0;
  write.descriptorCount = 1;
  write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  write.pBufferInfo = &buffer_info;
  dfn.vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
  REXLOG_INFO("gta4-native: configured {} draw-local texture descriptor slots",
              native_descriptor_capacity_);

  VkPushConstantRange push_range{};
  push_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  push_range.size = sizeof(NativePushConstants);
  VkPipelineLayoutCreateInfo pipeline_layout_info{};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.setLayoutCount = kDescriptorSetCount;
  pipeline_layout_info.pSetLayouts = descriptor_set_layouts_.data();
  pipeline_layout_info.pushConstantRangeCount = 1;
  pipeline_layout_info.pPushConstantRanges = &push_range;
  return dfn.vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &pipeline_layout_) ==
         VK_SUCCESS;
}

bool Gta4NativeGraphicsSystem::CreateResolveConversionObjects() {
  static_assert(sizeof(NativeResolveConversionConstants) == 48);
  static_assert(sizeof(NativeReflectionMipConstants) == 16);
  static_assert(sizeof(NativeHDRPresentConstants) == 24);
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device || !null_sampler_) {
    return false;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  VkDescriptorSetLayoutBinding binding{};
  binding.binding = 0;
  binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  binding.descriptorCount = 1;
  binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  binding.pImmutableSamplers = &null_sampler_;
  VkDescriptorSetLayoutCreateInfo layout_info{};
  layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layout_info.bindingCount = 1;
  layout_info.pBindings = &binding;
  if (dfn.vkCreateDescriptorSetLayout(device, &layout_info, nullptr,
                                      &resolve_conversion_descriptor_set_layout_) != VK_SUCCESS) {
    return false;
  }

  VkPushConstantRange push_range{};
  push_range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  push_range.size = sizeof(NativeResolveConversionConstants);
  VkPipelineLayoutCreateInfo pipeline_layout_info{};
  pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_info.setLayoutCount = 1;
  pipeline_layout_info.pSetLayouts = &resolve_conversion_descriptor_set_layout_;
  pipeline_layout_info.pushConstantRangeCount = 1;
  pipeline_layout_info.pPushConstantRanges = &push_range;
  return dfn.vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr,
                                    &resolve_conversion_pipeline_layout_) == VK_SUCCESS;
}

bool Gta4NativeGraphicsSystem::InitializeNativeRendererObjects() {
  static_assert(sizeof(NativePushConstants) == 24);
  if (pipeline_layout_) {
    return true;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device || !vulkan_device->properties().dynamicRendering) {
    REXLOG_ERROR("gta4-native: Vulkan dynamic rendering is required by the native backend");
    return false;
  }
  if (!InitializeNativePipelineCache()) {
    REXLOG_WARN("gta4-native: persistent Vulkan pipeline cache is unavailable; continuing empty");
  }
  REXLOG_INFO(
      "gta4-native-filtering: texture={} anisotropy={} supported={} device-max={:.1f}x",
      REXCVAR_GET(gta4_texture_filtering), REXCVAR_GET(gta4_anisotropic_filtering),
      vulkan_device->properties().samplerAnisotropy,
      vulkan_device->properties().maxSamplerAnisotropy);
  if (!CreateNativeUploadBuffer(kNativeUploadBufferSize, upload_buffer_) ||
      !CreateNullImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, 1, 0, null_texture_2d_) ||
      !CreateNullImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D_ARRAY, 1, 0,
                       null_texture_2d_array_) ||
      !CreateNullImage(VK_IMAGE_TYPE_3D, VK_IMAGE_VIEW_TYPE_3D, 1, 0, null_texture_3d_) ||
      !CreateNullImage(VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_CUBE, 6,
                       VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, null_texture_cube_)) {
    DestroyNativeRendererObjects();
    return false;
  }

  VkSamplerCreateInfo sampler_info{};
  sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  sampler_info.magFilter = VK_FILTER_LINEAR;
  sampler_info.minFilter = VK_FILTER_LINEAR;
  sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sampler_info.maxLod = VK_LOD_CLAMP_NONE;
  if (vulkan_device->functions().vkCreateSampler(vulkan_device->device(), &sampler_info, nullptr,
                                                 &null_sampler_) != VK_SUCCESS) {
    DestroyNativeRendererObjects();
    return false;
  }
  if (!CreateNativeDescriptors() || !CreateResolveConversionObjects()) {
    DestroyNativeRendererObjects();
    return false;
  }
  return true;
}

bool Gta4NativeGraphicsSystem::InitializeNativePipelineCache() {
  if (native_pipeline_cache_) {
    return true;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }
  VkPhysicalDeviceProperties properties{};
  vulkan_device->vulkan_instance()->functions().vkGetPhysicalDeviceProperties(
      vulkan_device->physical_device(), &properties);
  NativePipelineCacheIdentity identity{};
  identity.title_id = native_pipeline_cache_title_id_ ? native_pipeline_cache_title_id_ : kTitleId;
  identity.vendor_id = properties.vendorID;
  identity.device_id = properties.deviceID;
  identity.driver_version = properties.driverVersion;
  identity.api_version = properties.apiVersion;
  std::copy_n(properties.pipelineCacheUUID, identity.uuid.size(), identity.uuid.begin());

  std::vector<uint8_t> initial_data;
  if (!native_pipeline_cache_root_.empty()) {
    native_pipeline_cache_path_ =
        GetNativePipelineCachePath(native_pipeline_cache_root_, identity);
    if (auto data = ReadBinaryFile(native_pipeline_cache_path_);
        data && ValidateNativePipelineCacheData(*data, identity)) {
      initial_data = std::move(*data);
    } else if (data) {
      REXLOG_WARN("gta4-native: ignoring incompatible pipeline cache {}",
                  native_pipeline_cache_path_.string());
    }
  }

  VkPipelineCacheCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  create_info.initialDataSize = initial_data.size();
  create_info.pInitialData = initial_data.empty() ? nullptr : initial_data.data();
  const VkResult result = vulkan_device->functions().vkCreatePipelineCache(
      vulkan_device->device(), &create_info, nullptr, &native_pipeline_cache_);
  if (result != VK_SUCCESS && !initial_data.empty()) {
    create_info.initialDataSize = 0;
    create_info.pInitialData = nullptr;
    const VkResult retry_result = vulkan_device->functions().vkCreatePipelineCache(
        vulkan_device->device(), &create_info, nullptr, &native_pipeline_cache_);
    if (retry_result != VK_SUCCESS) {
      native_pipeline_cache_ = VK_NULL_HANDLE;
      return false;
    }
    REXLOG_WARN("gta4-native: driver rejected stored pipeline cache; started empty");
  } else if (result != VK_SUCCESS) {
    native_pipeline_cache_ = VK_NULL_HANDLE;
    return false;
  }
  REXLOG_INFO("gta4-native: Vulkan pipeline cache {} ({})",
              initial_data.empty() ? "started empty" : "loaded",
              native_pipeline_cache_path_.empty() ? "memory only"
                                                   : native_pipeline_cache_path_.string());
  return true;
}

void Gta4NativeGraphicsSystem::SaveNativePipelineCache() {
  if (!native_pipeline_cache_ || !native_pipeline_cache_dirty_ ||
      native_pipeline_cache_path_.empty() || !provider_) {
    return;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  if (!vulkan_device) {
    return;
  }
  const auto& dfn = vulkan_device->functions();
  size_t size = 0;
  if (dfn.vkGetPipelineCacheData(vulkan_device->device(), native_pipeline_cache_, &size,
                                 nullptr) != VK_SUCCESS || !size) {
    return;
  }
  std::vector<uint8_t> data(size);
  if (dfn.vkGetPipelineCacheData(vulkan_device->device(), native_pipeline_cache_, &size,
                                 data.data()) != VK_SUCCESS) {
    return;
  }
  data.resize(size);
  std::error_code error;
  std::filesystem::create_directories(native_pipeline_cache_path_.parent_path(), error);
  if (error) {
    REXLOG_WARN("gta4-native: could not create pipeline cache directory: {}", error.message());
    return;
  }
  std::filesystem::path temporary = native_pipeline_cache_path_;
  temporary += ".tmp";
  {
    std::ofstream stream(temporary, std::ios::binary | std::ios::trunc);
    if (!stream || !stream.write(reinterpret_cast<const char*>(data.data()), data.size()) ||
        !stream.flush()) {
      REXLOG_WARN("gta4-native: failed writing pipeline cache {}", temporary.string());
      return;
    }
  }
  std::filesystem::rename(temporary, native_pipeline_cache_path_, error);
  if (error) {
    // Never remove the last known-good cache before installing its replacement.
    // Platforms without atomic replace retain the prior cache for this run.
    std::error_code cleanup_error;
    std::filesystem::remove(temporary, cleanup_error);
    REXLOG_WARN("gta4-native: failed installing pipeline cache: {}", error.message());
    return;
  }
  native_pipeline_cache_dirty_ = false;
}

bool Gta4NativeGraphicsSystem::AllocateUpload(VkDeviceSize size, VkDeviceSize alignment,
                                              NativeUploadAllocation& allocation) {
  if (!upload_buffer_.mapping || !alignment || !rex::is_pow2(alignment)) {
    return false;
  }
  const VkDeviceSize offset = rex::align(upload_buffer_.write_offset, alignment);
  if (offset > upload_buffer_.capacity || size > upload_buffer_.capacity - offset) {
    return false;
  }
  allocation.offset = offset;
  allocation.device_address = upload_buffer_.device_address + offset;
  allocation.mapping = upload_buffer_.mapping + offset;
  upload_buffer_.write_offset = offset + size;
  return true;
}

bool Gta4NativeGraphicsSystem::EnsureFrameUploadCapacity(
    const std::shared_ptr<const NativeTextureResource>& present_source) {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device || !upload_buffer_.buffer) {
    return false;
  }

  const VkDeviceSize maximum_capacity = vulkan_device->properties().maxStorageBufferRange;
  VkDeviceSize required_capacity = kDefaultVertexDataSize;
  bool overflow = false;
  auto add_allocation = [&](VkDeviceSize size, VkDeviceSize alignment) {
    if (!size || overflow) {
      return;
    }
    const VkDeviceSize aligned_offset = rex::align(required_capacity, alignment);
    if (aligned_offset > maximum_capacity || size > maximum_capacity - aligned_offset) {
      overflow = true;
      return;
    }
    required_capacity = aligned_offset + size;
  };

  std::unordered_set<uint64_t> pending_texture_generations;
  auto add_texture = [&](const std::shared_ptr<const NativeTextureResource>& texture) {
    if (!texture || texture->gpu_produced || texture->payload.empty() ||
        native_texture_images_.contains(texture->generation) ||
        !pending_texture_generations.insert(texture->generation).second) {
      return;
    }
    add_allocation(VkDeviceSize(texture->payload.size()), 16);
  };
  add_texture(present_source);

  using VertexUploadKey =
      std::tuple<const NativeBufferResource*, uint64_t, uint64_t, uint32_t, uint32_t, uint32_t>;
  std::set<VertexUploadKey> pending_vertex_uploads;
  std::set<std::pair<const NativeBufferResource*, bool>> pending_index_uploads;
  uint32_t draw_count = 0;
  for (const NativeCommand& command : current_frame_) {
    add_texture(command.resolve_destination);
    add_texture(command.depth_handoff_source);
    for (const auto& texture : command.textures) {
      add_texture(texture);
    }

    const bool is_draw = command.type == CommandType::kDrawPrimitive ||
                         command.type == CommandType::kDrawPrimitiveUp ||
                         command.type == CommandType::kDrawIndexedPrimitive;
    if (!is_draw) {
      continue;
    }
    ++draw_count;
    add_allocation(kVertexConstantsSize, 16);
    add_allocation(kPixelConstantsSize, 16);
    add_allocation(sizeof(NativeSharedConstants), 16);

    if (command.type == CommandType::kDrawPrimitiveUp) {
      add_allocation(VkDeviceSize(command.payload.size()), 16);
      DrawPrimitiveUpCommand draw{};
      if (command.bytes.size() >= sizeof(draw)) {
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        if (draw.primitive_type == uint32_t(xenos::PrimitiveType::kRectangleList) &&
            draw.vertex_count && draw.stride) {
          const VkDeviceSize rectangle_count = draw.vertex_count / 3;
          const VkDeviceSize expanded_size = rectangle_count * 4 * draw.stride;
          add_allocation(expanded_size, 16);
        }
      }
      continue;
    }

    if (!command.pipeline_state || !command.pipeline_state->vertex_declaration_resource ||
        !command.pipeline_state->vertex_shader_resource) {
      continue;
    }
    const auto& state = *command.pipeline_state;
    const uint64_t declaration_hash = state.vertex_declaration_resource->content_hash;
    const uint64_t shader_hash = state.vertex_shader_resource->hash;
    std::array<bool, kVertexStreamCount> required_streams{};
    if (!GetRequiredVertexStreams(state, required_streams)) {
      continue;
    }
    for (uint32_t stream = 0; stream < kVertexStreamCount; ++stream) {
      if (!required_streams[stream]) {
        continue;
      }
      const auto& resource = command.vertex_buffers[stream];
      const auto& stream_state = state.vertex_streams[stream];
      if (!resource || resource->payload.empty() || !stream_state.stride) {
        continue;
      }
      VertexUploadKey key{resource.get(), declaration_hash, shader_hash, stream,
                          stream_state.offset, stream_state.stride};
      if (pending_vertex_uploads.insert(key).second) {
        add_allocation(VkDeviceSize(resource->payload.size()), 16);
      }
    }

    if (command.type == CommandType::kDrawIndexedPrimitive && command.index_buffer) {
      const bool index32 = (command.index_buffer->flags & kIndex32Flag) != 0;
      if (pending_index_uploads.insert({command.index_buffer.get(), index32}).second) {
        add_allocation(VkDeviceSize(command.index_buffer->payload.size()), 16);
      }
      DrawIndexedPrimitiveCommand draw{};
      if (command.bytes.size() >= sizeof(draw)) {
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        const VkPrimitiveTopology topology = ConvertPrimitiveTopology(draw.primitive_type);
        if (topology == VK_PRIMITIVE_TOPOLOGY_LINE_STRIP ||
            topology == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP) {
          add_allocation(VkDeviceSize(draw.index_count) * sizeof(uint32_t),
                         alignof(uint32_t));
        }
      }
    }
  }

  if (overflow) {
    REXLOG_ERROR(
        "gta4-native-upload: frame requirements exceed maxStorageBufferRange={} draws={} "
        "textures={} vertex-uploads={} index-uploads={}",
        maximum_capacity, draw_count, pending_texture_generations.size(),
        pending_vertex_uploads.size(), pending_index_uploads.size());
    return false;
  }
  if (required_capacity <= upload_buffer_.capacity) {
    return true;
  }

  const VkDeviceSize new_capacity =
      rex::round_up(required_capacity, kNativeUploadGrowthGranularity);
  if (new_capacity > maximum_capacity) {
    REXLOG_ERROR(
        "gta4-native-upload: rounded frame upload capacity {} exceeds "
        "maxStorageBufferRange={} (required={})",
        new_capacity, maximum_capacity, required_capacity);
    return false;
  }
  NativeUploadBuffer replacement;
  if (!CreateNativeUploadBuffer(new_capacity, replacement)) {
    REXLOG_ERROR(
        "gta4-native-upload: failed to grow frame upload buffer from {} to {} bytes "
        "(required={} draws={} textures={} vertex-uploads={} index-uploads={})",
        upload_buffer_.capacity, new_capacity, required_capacity, draw_count,
        pending_texture_generations.size(), pending_vertex_uploads.size(),
        pending_index_uploads.size());
    return false;
  }

  const VkDeviceSize old_capacity = upload_buffer_.capacity;
  DestroyNativeUploadBuffer(upload_buffer_);
  upload_buffer_ = replacement;

  VkDescriptorBufferInfo buffer_info{};
  buffer_info.buffer = upload_buffer_.buffer;
  buffer_info.range = upload_buffer_.capacity;
  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.dstSet = descriptor_sets_[5];
  write.dstBinding = 0;
  write.descriptorCount = 1;
  write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  write.pBufferInfo = &buffer_info;
  vulkan_device->functions().vkUpdateDescriptorSets(vulkan_device->device(), 1, &write, 0,
                                                     nullptr);
  REXLOG_INFO(
      "gta4-native-upload: grew frame upload buffer {} -> {} bytes (required={} draws={} "
      "textures={} vertex-uploads={} index-uploads={})",
      old_capacity, new_capacity, required_capacity, draw_count,
      pending_texture_generations.size(), pending_vertex_uploads.size(),
      pending_index_uploads.size());
  return true;
}

bool Gta4NativeGraphicsSystem::RecordNullImageInitialization(VkCommandBuffer command_buffer) {
  if (null_images_initialized_) {
    return true;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();

  const std::array<VkImage, 4> images = {
      null_texture_2d_.image, null_texture_2d_array_.image, null_texture_3d_.image,
      null_texture_cube_.image};
  const std::array<uint32_t, 4> layer_counts = {1, 1, 1, 6};
  std::array<VkImageMemoryBarrier, 4> barriers{};
  for (size_t index = 0; index < images.size(); ++index) {
    auto& barrier = barriers[index];
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = images[index];
    barrier.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
        VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, layer_counts[index]);
  }
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr,
                           uint32_t(barriers.size()), barriers.data());

  VkClearColorValue clear_color{};
  clear_color.float32[0] = 1.0f;
  clear_color.float32[1] = 1.0f;
  clear_color.float32[2] = 1.0f;
  clear_color.float32[3] = 1.0f;
  for (const auto& barrier : barriers) {
    dfn.vkCmdClearColorImage(command_buffer, barrier.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                             &clear_color, 1, &barrier.subresourceRange);
  }

  for (auto& barrier : barriers) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  }
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
      nullptr, uint32_t(barriers.size()), barriers.data());
  null_images_initialized_ = true;
  return true;
}

Gta4NativeGraphicsSystem::NativeTextureImage* Gta4NativeGraphicsSystem::GetOrCreateTextureImage(
    VkCommandBuffer command_buffer, const std::shared_ptr<const NativeTextureResource>& texture) {
  auto reject = [&texture](const char* reason) -> NativeTextureImage* {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    if (count <= 64 || !(count % 4096)) {
      REXLOG_WARN(
          "gta4-native-diag: texture image reject #{} reason={} handle={:08X} generation={} "
          "gpu-produced={} payload={} format={} dimension={} base={:08X} size={}x{}",
          count, reason, texture ? texture->handle : 0, texture ? texture->generation : 0,
          texture ? texture->gpu_produced : false, texture ? texture->payload.size() : 0,
          texture ? uint32_t(texture->info.format) : 0,
          texture ? uint32_t(texture->info.dimension) : 0,
          texture ? texture->info.memory.base_address : 0, texture ? texture->info.width + 1 : 0,
          texture ? texture->info.height + 1 : 0);
    }
    return nullptr;
  };
  if (!texture) {
    return nullptr;
  }
  if (!native_descriptor_capacity_) {
    return reject("descriptor-capacity-zero");
  }
  auto existing = native_texture_images_.find(texture->generation);
  if (existing != native_texture_images_.end()) {
    return existing->second.get();
  }

  const VkFormat format = ConvertTextureFormat(texture->info.format);
  if (format == VK_FORMAT_UNDEFINED) {
    return reject("format");
  }
  if (!texture->gpu_produced && texture->payload.empty()) {
    return reject("empty-payload");
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  VkFormatProperties format_properties{};
  vulkan_device->vulkan_instance()->functions().vkGetPhysicalDeviceFormatProperties(
      vulkan_device->physical_device(), format, &format_properties);
  constexpr VkFormatFeatureFlags kRequiredTextureFeatures =
      VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
  if ((format_properties.optimalTilingFeatures & kRequiredTextureFeatures) !=
      kRequiredTextureFeatures) {
    return reject("format-capabilities");
  }

  auto image = std::make_unique<NativeTextureImage>();
  image->source = texture;
  image->format = format;
  image->logical_width = texture->info.width + 1;
  image->logical_height = texture->info.height + 1;
  image->width = image->logical_width;
  image->height = image->logical_height;
  const auto reflection_entry = reflection_resources_.find(texture->handle);
  if (reflection_entry != reflection_resources_.end()) {
    image->reflection = reflection_entry->second;
    image->is_reflection = true;
    image->logical_width = reflection_entry->second.logical_width;
    image->logical_height = reflection_entry->second.logical_height;
    const uint32_t maximum_extent = vulkan_device->properties().maxImageDimension2D;
    if (reflection_entry->second.physical_width <= maximum_extent &&
        reflection_entry->second.physical_height <= maximum_extent) {
      image->width = reflection_entry->second.physical_width;
      image->height = reflection_entry->second.physical_height;
    } else {
      REXLOG_WARN(
          "gta4-native-reflection: texture {:08X} requested {}x{} exceeds device limit {}; "
          "using original {}x{}",
          texture->handle, reflection_entry->second.physical_width,
          reflection_entry->second.physical_height, maximum_extent, image->logical_width,
          image->logical_height);
    }
  }
  const bool is_3d = texture->info.dimension == xenos::DataDimension::k3D;
  const bool is_cube = texture->info.dimension == xenos::DataDimension::kCube;
  const bool is_2d_array = texture->info.dimension == xenos::DataDimension::k2DOrStacked &&
                           texture->info.is_stacked;
  const uint32_t array_layer_count = is_cube ? 6 : is_2d_array ? texture->info.depth + 1 : 1;
  if (format == VK_FORMAT_D24_UNORM_S8_UINT || format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
    image->aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
  }
  image->guest_mip_levels = texture->info.mip_max_level + 1;
  image->mip_levels = image->guest_mip_levels;
  if (image->is_reflection &&
      image->reflection.family == ReflectionFamily::kEnvironment &&
      image->reflection.role == ReflectionRole::kColor) {
    image->mip_levels =
        std::max(image->guest_mip_levels,
                 uint32_t(std::bit_width(std::max(image->width, image->height))));
  }
  VkImageCreateInfo image_info{};
  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.flags = is_cube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
  image_info.imageType = is_3d ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D;
  image_info.format = format;
  image_info.extent = {image->width, image->height,
                       is_3d ? texture->info.depth + 1 : 1};
  image_info.mipLevels = image->mip_levels;
  image_info.arrayLayers = array_layer_count;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;
  image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  if (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT) {
    image_info.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  }
  if (image->aspect == VK_IMAGE_ASPECT_COLOR_BIT &&
      (format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)) {
    image_info.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  } else if ((image->aspect & VK_IMAGE_ASPECT_DEPTH_BIT) &&
             (format_properties.optimalTilingFeatures &
              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)) {
    image_info.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  }
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (!ui::vulkan::util::CreateDedicatedAllocationImage(
          vulkan_device, image_info, ui::vulkan::util::MemoryPurpose::kDeviceLocal,
          image->resource.image, image->resource.memory)) {
    if (!image->is_reflection ||
        (image->width == image->logical_width && image->height == image->logical_height)) {
      return reject("create-image");
    }
    REXLOG_WARN(
        "gta4-native-reflection: resolved texture {:08X} allocation failed at {}x{}; "
        "retrying original {}x{}",
        texture->handle, image->width, image->height, image->logical_width,
        image->logical_height);
    image->width = image->logical_width;
    image->height = image->logical_height;
    image->mip_levels = image->guest_mip_levels;
    if (image->reflection.family == ReflectionFamily::kEnvironment &&
        image->reflection.role == ReflectionRole::kColor) {
      image->mip_levels = std::max(
          image->guest_mip_levels,
          uint32_t(std::bit_width(std::max(image->width, image->height))));
    }
    image_info.extent.width = image->width;
    image_info.extent.height = image->height;
    image_info.mipLevels = image->mip_levels;
    if (!ui::vulkan::util::CreateDedicatedAllocationImage(
            vulkan_device, image_info, ui::vulkan::util::MemoryPurpose::kDeviceLocal,
            image->resource.image, image->resource.memory)) {
      return reject("create-image-original-fallback");
    }
  }

  VkImageViewCreateInfo view_info{};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.image = image->resource.image;
  view_info.viewType = is_3d       ? VK_IMAGE_VIEW_TYPE_3D
                       : is_cube    ? VK_IMAGE_VIEW_TYPE_CUBE
                       : is_2d_array ? VK_IMAGE_VIEW_TYPE_2D_ARRAY
                                     : VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = format;
  uint32_t host_swizzle = GuestToNativeHostSwizzle(
      texture->fetch.swizzle, GetNativeHostFormatSwizzle(texture->info.format));
  if (texture->vector_font_replacement) {
    view_info.components.r = VK_COMPONENT_SWIZZLE_ONE;
    view_info.components.g = VK_COMPONENT_SWIZZLE_ONE;
    view_info.components.b = VK_COMPONENT_SWIZZLE_ONE;
    view_info.components.a = VK_COMPONENT_SWIZZLE_R;
  } else if (!vulkan_device->properties().imageViewFormatSwizzle) {
    host_swizzle = xenos::XE_GPU_TEXTURE_SWIZZLE_RGBA;
    view_info.components.r = GetNativeComponentSwizzle(host_swizzle, 0);
    view_info.components.g = GetNativeComponentSwizzle(host_swizzle, 1);
    view_info.components.b = GetNativeComponentSwizzle(host_swizzle, 2);
    view_info.components.a = GetNativeComponentSwizzle(host_swizzle, 3);
  } else {
    view_info.components.r = GetNativeComponentSwizzle(host_swizzle, 0);
    view_info.components.g = GetNativeComponentSwizzle(host_swizzle, 1);
    view_info.components.b = GetNativeComponentSwizzle(host_swizzle, 2);
    view_info.components.a = GetNativeComponentSwizzle(host_swizzle, 3);
  }
  view_info.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      VK_IMAGE_ASPECT_COLOR_BIT, 0, image->mip_levels, 0, array_layer_count);
  // A depth/stencil image may use both aspects for transfers and layout
  // transitions, but Vulkan requires a view installed in a sampled-image
  // descriptor to select exactly one aspect. GTA IV samples these resources as
  // floating-point depth textures, so expose only depth to the shader.
  view_info.subresourceRange.aspectMask =
      image->aspect & VK_IMAGE_ASPECT_DEPTH_BIT ? VK_IMAGE_ASPECT_DEPTH_BIT
                                                : image->aspect;
  if (dfn.vkCreateImageView(device, &view_info, nullptr, &image->resource.view) != VK_SUCCESS) {
    dfn.vkDestroyImage(device, image->resource.image, nullptr);
    dfn.vkFreeMemory(device, image->resource.memory, nullptr);
    return reject("create-view");
  }

  if (!texture->gpu_produced) {
    NativeUploadAllocation upload;
    if (!AllocateUpload(texture->payload.size(), 16, upload)) {
      dfn.vkDestroyImageView(device, image->resource.view, nullptr);
      dfn.vkDestroyImage(device, image->resource.image, nullptr);
      dfn.vkFreeMemory(device, image->resource.memory, nullptr);
      return reject("upload-allocation");
    }
    std::memcpy(upload.mapping, texture->payload.data(), texture->payload.size());

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image->resource.image;
    barrier.subresourceRange = view_info.subresourceRange;
    dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);

    std::vector<VkBufferImageCopy> copies;
    copies.reserve(texture->mip_levels.size());
    for (const NativeTextureResource::MipLevel& mip : texture->mip_levels) {
      VkBufferImageCopy copy{};
      copy.bufferOffset = upload.offset + mip.payload_offset;
      copy.bufferRowLength = mip.buffer_row_length;
      copy.bufferImageHeight = mip.buffer_image_height;
      copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      copy.imageSubresource.mipLevel = mip.level;
      copy.imageSubresource.baseArrayLayer = mip.base_array_layer;
      copy.imageSubresource.layerCount = mip.layer_count;
      copy.imageExtent = {mip.width, mip.height, mip.depth};
      copies.push_back(copy);
    }
    if (copies.empty()) {
      dfn.vkDestroyImageView(device, image->resource.view, nullptr);
      dfn.vkDestroyImage(device, image->resource.image, nullptr);
      dfn.vkFreeMemory(device, image->resource.memory, nullptr);
      return reject("mip-payloads-empty");
    }
    if (texture->vector_font_replacement && REXCVAR_GET(gta4_trace_vector_fonts)) {
      const VkBufferImageCopy& first_copy = copies.front();
      REXLOG_INFO(
          "gta4-native-font-debug: image-upload font={} handle={:08X} generation={} "
          "format={} logical={}x{} physical={}x{} guest-mips={} image-mips={} "
          "payload={} payload-hash={:016X} copy-count={} copy-offset={} "
          "row-length={} image-height={} extent={}x{}x{} swizzle={},{},{},{}",
          texture->vector_font_id, texture->handle, texture->generation, uint32_t(image->format),
          image->logical_width, image->logical_height, image->width, image->height,
          image->guest_mip_levels, image->mip_levels, texture->payload.size(),
          XXH3_64bits(texture->payload.data(), texture->payload.size()), copies.size(),
          first_copy.bufferOffset, first_copy.bufferRowLength, first_copy.bufferImageHeight,
          first_copy.imageExtent.width, first_copy.imageExtent.height, first_copy.imageExtent.depth,
          uint32_t(view_info.components.r), uint32_t(view_info.components.g),
          uint32_t(view_info.components.b), uint32_t(view_info.components.a));
    }
    dfn.vkCmdCopyBufferToImage(command_buffer, upload_buffer_.buffer, image->resource.image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, uint32_t(copies.size()),
                               copies.data());

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
        0, nullptr, 1, &barrier);
    image->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  } else {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image->resource.image;
    barrier.subresourceRange = view_info.subresourceRange;
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
        0, nullptr, 1, &barrier);
    image->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  }

  NativeTextureImage* result = image.get();
  native_texture_images_.emplace(texture->generation, std::move(image));
  if (result->is_reflection) {
    REXLOG_INFO(
        "gta4-native-reflection: created resolved texture {:08X} logical={}x{} physical={}x{} "
        "guest-mips={} physical-mips={} format={}",
        texture->handle, result->logical_width, result->logical_height, result->width,
        result->height, result->guest_mip_levels, result->mip_levels,
        uint32_t(result->format));
  }
  return result;
}

VkSampler Gta4NativeGraphicsSystem::GetOrCreateSampler(
    const xenos::xe_gpu_texture_fetch_t& fetch, const NativeTextureImage* image,
    NativeSamplerKey* effective_key) {
  if (effective_key) {
    *effective_key = {};
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device || !native_descriptor_capacity_) {
    return 0;
  }

  auto normalize_clamp = [vulkan_device](xenos::ClampMode mode) {
    if (mode == xenos::ClampMode::kClampToHalfway) {
      return xenos::ClampMode::kClampToEdge;
    }
    if (mode == xenos::ClampMode::kMirrorClampToEdge ||
        mode == xenos::ClampMode::kMirrorClampToHalfway ||
        mode == xenos::ClampMode::kMirrorClampToBorder) {
      return vulkan_device->properties().samplerMirrorClampToEdge
                 ? xenos::ClampMode::kMirrorClampToEdge
                 : xenos::ClampMode::kMirroredRepeat;
    }
    return mode;
  };

  NativeSamplerKey key{};
  texture_util::GetClampModesForDimension(fetch, key.clamp_u, key.clamp_v, key.clamp_w);
  key.clamp_u = normalize_clamp(key.clamp_u);
  key.clamp_v = normalize_clamp(key.clamp_v);
  key.clamp_w = normalize_clamp(key.clamp_w);
  key.min_filter = fetch.min_filter == xenos::TextureFilter::kUseFetchConst
                       ? xenos::TextureFilter::kPoint
                       : fetch.min_filter;
  key.mag_filter = fetch.mag_filter == xenos::TextureFilter::kUseFetchConst
                       ? xenos::TextureFilter::kPoint
                       : fetch.mag_filter;
  key.mip_filter = fetch.mip_filter == xenos::TextureFilter::kUseFetchConst
                       ? xenos::TextureFilter::kPoint
                       : fetch.mip_filter;
  key.aniso_filter = fetch.aniso_filter == xenos::AnisoFilter::kUseFetchConst
                         ? xenos::AnisoFilter::kDisabled
                         : fetch.aniso_filter;
  key.border_color = xenos::ClampModeUsesBorder(key.clamp_u) ||
                             xenos::ClampModeUsesBorder(key.clamp_v) ||
                             xenos::ClampModeUsesBorder(key.clamp_w)
                         ? fetch.border_color
                         : xenos::BorderColor::k_ABGR_Black;
  key.lod_bias = fetch.lod_bias;
  texture_util::GetSubresourcesFromFetchConstant(fetch, nullptr, nullptr, nullptr, nullptr, nullptr,
                                                 &key.mip_min_level, &key.mip_max_level);
  if (key.mip_filter == xenos::TextureFilter::kBaseMap) {
    key.mip_max_level = key.mip_min_level;
  } else if (key.mip_min_level != key.mip_max_level && image && image->is_reflection &&
             image->reflection.family == ReflectionFamily::kEnvironment &&
             image->reflection.role == ReflectionRole::kColor) {
    // Preserve exact guest mip clamps used while constructing the environment
    // reflection mip chain. Only sampling ranges may opt into the native tail.
    key.mip_max_level = std::max(key.mip_max_level, image->mip_levels - 1);
  }

  const VkFormat sampled_format = image ? image->format : ConvertTextureFormat(fetch.format);
  if (sampled_format == VK_FORMAT_UNDEFINED) {
    return 0;
  }
  VkFormatProperties format_properties{};
  vulkan_device->vulkan_instance()->functions().vkGetPhysicalDeviceFormatProperties(
      vulkan_device->physical_device(), sampled_format, &format_properties);
  if (image && image->source && image->source->vector_font_replacement) {
    key.min_filter = xenos::TextureFilter::kLinear;
    key.mag_filter = xenos::TextureFilter::kLinear;
    key.mip_filter = xenos::TextureFilter::kLinear;
    key.mip_min_level = 0;
    key.mip_max_level = image->mip_levels - 1;
  }
  if (!(format_properties.optimalTilingFeatures &
        VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    key.min_filter = xenos::TextureFilter::kPoint;
    key.mag_filter = xenos::TextureFilter::kPoint;
    key.mip_filter = key.mip_filter == xenos::TextureFilter::kBaseMap
                         ? xenos::TextureFilter::kBaseMap
                         : xenos::TextureFilter::kPoint;
    key.aniso_filter = xenos::AnisoFilter::kDisabled;
  }

  if (image) {
    const uint32_t maximum_image_mip = image->mip_levels ? image->mip_levels - 1 : 0;
    key.mip_min_level = std::min(key.mip_min_level, maximum_image_mip);
    key.mip_max_level =
        std::min(std::max(key.mip_min_level, key.mip_max_level), maximum_image_mip);
  }

  if (image && image->source && image->source->vector_font_replacement &&
      REXCVAR_GET(gta4_trace_vector_fonts)) {
    static std::atomic<uint64_t> font_sampler_trace_count{0};
    const uint64_t trace = ++font_sampler_trace_count;
    if (trace <= 96) {
      REXLOG_INFO(
          "gta4-native-font-debug: sampler #{} font={} handle={:08X} generation={} "
          "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
          "min={} mag={} mip={} aniso={} clamp={},{},{} border={} lod-bias={} mips={}-{}",
          trace, image->source->vector_font_id, image->source->handle,
          image->source->generation, fetch.dword_0, fetch.dword_1, fetch.dword_2, fetch.dword_3,
          fetch.dword_4, fetch.dword_5, uint32_t(key.min_filter), uint32_t(key.mag_filter),
          uint32_t(key.mip_filter), uint32_t(key.aniso_filter), uint32_t(key.clamp_u),
          uint32_t(key.clamp_v), uint32_t(key.clamp_w), uint32_t(key.border_color), key.lod_bias,
          key.mip_min_level, key.mip_max_level);
    }
  }

  const bool material_filter_eligible =
      image && image->aspect == VK_IMAGE_ASPECT_COLOR_BIT && image->mip_levels > 1 &&
      !image->is_reflection && image->source && !image->source->gpu_produced &&
      !image->source->vector_font_replacement &&
      key.mip_filter != xenos::TextureFilter::kBaseMap &&
      (format_properties.optimalTilingFeatures &
       VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT);
  const std::string texture_filtering = REXCVAR_GET(gta4_texture_filtering);
  if (material_filter_eligible && texture_filtering != "original") {
    key.min_filter = xenos::TextureFilter::kLinear;
    key.mag_filter = xenos::TextureFilter::kLinear;
    if (texture_filtering == "trilinear") {
      key.mip_filter = xenos::TextureFilter::kLinear;
    }
  }

  const std::string anisotropic_filtering = REXCVAR_GET(gta4_anisotropic_filtering);
  if (anisotropic_filtering == "off") {
    key.aniso_filter = xenos::AnisoFilter::kDisabled;
  } else if (material_filter_eligible && anisotropic_filtering != "original") {
    key.aniso_filter = anisotropic_filtering == "16x"  ? xenos::AnisoFilter::kMax_16_1
                       : anisotropic_filtering == "8x" ? xenos::AnisoFilter::kMax_8_1
                       : anisotropic_filtering == "4x" ? xenos::AnisoFilter::kMax_4_1
                                                       : xenos::AnisoFilter::kMax_2_1;
    key.min_filter = xenos::TextureFilter::kLinear;
    key.mag_filter = xenos::TextureFilter::kLinear;
    key.mip_filter = xenos::TextureFilter::kLinear;
  }
  if (!vulkan_device->properties().samplerAnisotropy) {
    if (key.aniso_filter != xenos::AnisoFilter::kDisabled) {
      static std::atomic<bool> logged_missing_anisotropy{false};
      if (!logged_missing_anisotropy.exchange(true)) {
        REXLOG_WARN(
            "gta4-native-filtering: sampler anisotropy is unavailable; "
            "falling back to non-anisotropic filtering");
      }
    }
    key.aniso_filter = xenos::AnisoFilter::kDisabled;
  }

  if (effective_key) {
    *effective_key = key;
  }

  for (const NativeSampler& existing : native_samplers_) {
    if (existing.key == key) {
      return existing.sampler;
    }
  }

  static const VkSamplerAddressMode kAddressModes[] = {
      VK_SAMPLER_ADDRESS_MODE_REPEAT,          VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,   VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,   VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER, VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
  };
  VkSamplerCreateInfo sampler_info{};
  sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  sampler_info.minFilter =
      key.min_filter == xenos::TextureFilter::kLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
  sampler_info.magFilter =
      key.mag_filter == xenos::TextureFilter::kLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
  sampler_info.mipmapMode = key.mip_filter == xenos::TextureFilter::kLinear
                                ? VK_SAMPLER_MIPMAP_MODE_LINEAR
                                : VK_SAMPLER_MIPMAP_MODE_NEAREST;
  sampler_info.addressModeU = kAddressModes[uint32_t(key.clamp_u)];
  sampler_info.addressModeV = kAddressModes[uint32_t(key.clamp_v)];
  sampler_info.addressModeW = kAddressModes[uint32_t(key.clamp_w)];
  sampler_info.mipLodBias = static_cast<float>(key.lod_bias) / 32.0f;
  sampler_info.minLod = float(key.mip_min_level);
  sampler_info.maxLod = float(key.mip_max_level);
  if (key.aniso_filter != xenos::AnisoFilter::kDisabled) {
    sampler_info.anisotropyEnable = VK_TRUE;
    const float requested_anisotropy =
        std::ldexp(1.0f, int32_t(key.aniso_filter) - int32_t(xenos::AnisoFilter::kMax_1_1));
    sampler_info.maxAnisotropy =
        std::min(requested_anisotropy, vulkan_device->properties().maxSamplerAnisotropy);
    sampler_info.minFilter = VK_FILTER_LINEAR;
    sampler_info.magFilter = VK_FILTER_LINEAR;
    sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  }
  sampler_info.borderColor = key.border_color == xenos::BorderColor::k_ABGR_White
                                 ? VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE
                                 : VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

  VkSampler sampler = VK_NULL_HANDLE;
  const auto& dfn = vulkan_device->functions();
  if (dfn.vkCreateSampler(vulkan_device->device(), &sampler_info, nullptr, &sampler) !=
      VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  native_samplers_.push_back({key, sampler});
  return sampler;
}

bool Gta4NativeGraphicsSystem::PrepareFrameDescriptorPool(uint32_t draw_count,
                                                          uint32_t combined_descriptor_count,
                                                          uint32_t combined_set_count) {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  if (frame_descriptor_pool_ && frame_descriptor_draw_capacity_ >= draw_count &&
      frame_descriptor_resolve_capacity_ >= combined_descriptor_count &&
      frame_descriptor_combined_set_capacity_ >= combined_set_count) {
    return dfn.vkResetDescriptorPool(device, frame_descriptor_pool_, 0) == VK_SUCCESS;
  }
  if (frame_descriptor_pool_) {
    dfn.vkDestroyDescriptorPool(device, frame_descriptor_pool_, nullptr);
    frame_descriptor_pool_ = VK_NULL_HANDLE;
    frame_descriptor_draw_capacity_ = 0;
    frame_descriptor_resolve_capacity_ = 0;
    frame_descriptor_combined_set_capacity_ = 0;
  }
  if (!draw_count && !combined_descriptor_count && !combined_set_count) {
    return true;
  }

  std::array<VkDescriptorPoolSize, 3> pool_sizes{};
  pool_sizes[0] = {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, draw_count * kShaderTextureCount * 4};
  pool_sizes[1] = {VK_DESCRIPTOR_TYPE_SAMPLER, draw_count * kShaderTextureCount};
  pool_sizes[2] = {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, combined_descriptor_count};
  VkDescriptorPoolCreateInfo pool_info{};
  pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_info.maxSets = draw_count * kDrawDescriptorSetCount + combined_set_count;
  pool_info.poolSizeCount = uint32_t(pool_sizes.size());
  pool_info.pPoolSizes = pool_sizes.data();
  if (dfn.vkCreateDescriptorPool(device, &pool_info, nullptr, &frame_descriptor_pool_) !=
      VK_SUCCESS) {
    return false;
  }
  frame_descriptor_draw_capacity_ = draw_count;
  frame_descriptor_resolve_capacity_ = combined_descriptor_count;
  frame_descriptor_combined_set_capacity_ = combined_set_count;
  return true;
}

bool Gta4NativeGraphicsSystem::PrepareFrameTextures(VkCommandBuffer command_buffer,
                                                     bool prepare_present,
                                                     uint32_t submitted_frame,
                                                     bool trace_reflections) {
  SCOPE_profile_cpu_i("gpu", "GTA4 Native PrepareFrameTextures");
  uint32_t draw_count = 0;
  uint32_t resolve_count = 0;
  for (const NativeCommand& command : current_frame_) {
    if (command.type == CommandType::kDrawPrimitive ||
        command.type == CommandType::kDrawPrimitiveUp ||
        command.type == CommandType::kDrawIndexedPrimitive) {
      ++draw_count;
    } else if (command.type == CommandType::kResolve) {
      ++resolve_count;
      ResolveCommand resolve{};
      if (command.resolve_destination && command.bytes.size() >= sizeof(resolve)) {
        std::memcpy(&resolve, command.bytes.data(), sizeof(resolve));
        const auto reflection =
            reflection_resources_.find(command.resolve_destination->handle);
        if (reflection != reflection_resources_.end() &&
            reflection->second.family == ReflectionFamily::kEnvironment &&
            reflection->second.role == ReflectionRole::kColor &&
            resolve.destination_level ==
                command.resolve_destination->info.mip_max_level) {
          const uint32_t physical_mip_levels = uint32_t(std::bit_width(std::max(
              reflection->second.physical_width, reflection->second.physical_height)));
          const uint32_t guest_mip_levels =
              command.resolve_destination->info.mip_max_level + 1;
          if (physical_mip_levels > guest_mip_levels) {
            resolve_count += physical_mip_levels - guest_mip_levels;
          }
        }
      }
    }
  }
  uint32_t combined_descriptor_count = resolve_count;
  uint32_t combined_set_count = resolve_count;
  const uint32_t placement_materialization_capacity =
      draw_count * (kRenderTargetCount + 1u);
  combined_descriptor_count += placement_materialization_capacity;
  combined_set_count += placement_materialization_capacity;
  if (prepare_present) {
    ++combined_descriptor_count;
    ++combined_set_count;
  }
  combined_descriptor_count += kSplitPostFxCombinedDescriptorCount;
  combined_set_count += kSplitPostFxDescriptorSetCount;
  combined_descriptor_count += kSunShaftCombinedDescriptorCount;
  combined_set_count += kSunShaftDescriptorSetCount;
  if (prepare_present && IsNativeSmaaEnabled()) {
    combined_descriptor_count += SmaaPipeline::kCombinedImageSamplerDescriptorCount;
    combined_set_count += SmaaPipeline::kDescriptorSetCount;
  }
  if (!PrepareFrameDescriptorPool(draw_count, combined_descriptor_count, combined_set_count)) {
    return false;
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  const std::array<VkDescriptorSetLayout, kDrawDescriptorSetCount> draw_layouts = {
      descriptor_set_layouts_[0], descriptor_set_layouts_[1], descriptor_set_layouts_[2],
      descriptor_set_layouts_[3], descriptor_set_layouts_[4]};

  for (size_t command_index = 0; command_index < current_frame_.size(); ++command_index) {
    NativeCommand& command = current_frame_[command_index];
    if (command.resolve_destination &&
        !GetOrCreateTextureImage(command_buffer, command.resolve_destination)) {
      return false;
    }
    if (command.depth_handoff_source &&
        !GetOrCreateTextureImage(command_buffer, command.depth_handoff_source)) {
      return false;
    }
    const bool is_draw = command.type == CommandType::kDrawPrimitive ||
                         command.type == CommandType::kDrawPrimitiveUp ||
                         command.type == CommandType::kDrawIndexedPrimitive;
    if (!is_draw) {
      continue;
    }

    VkDescriptorSetAllocateInfo allocate_info{};
    allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocate_info.descriptorPool = frame_descriptor_pool_;
    allocate_info.descriptorSetCount = kDrawDescriptorSetCount;
    allocate_info.pSetLayouts = draw_layouts.data();
    if (dfn.vkAllocateDescriptorSets(device, &allocate_info, command.draw_descriptor_sets.data()) !=
        VK_SUCCESS) {
      return false;
    }

    std::array<VkDescriptorImageInfo, kShaderTextureCount> images_2d{};
    std::array<VkDescriptorImageInfo, kShaderTextureCount> images_2d_array{};
    std::array<VkDescriptorImageInfo, kShaderTextureCount> images_3d{};
    std::array<VkDescriptorImageInfo, kShaderTextureCount> images_cube{};
    std::array<VkDescriptorImageInfo, kShaderTextureCount> samplers{};
    command.texture_descriptor_indices.fill(0);
    command.sampler_descriptor_indices.fill(0);
    for (uint32_t stage = 0; stage < kShaderTextureCount; ++stage) {
      images_2d[stage] = {VK_NULL_HANDLE, null_texture_2d_.view,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
      images_2d_array[stage] = {VK_NULL_HANDLE, null_texture_2d_array_.view,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
      images_3d[stage] = {VK_NULL_HANDLE, null_texture_3d_.view,
                          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
      images_cube[stage] = {VK_NULL_HANDLE, null_texture_cube_.view,
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
      samplers[stage] = {null_sampler_, VK_NULL_HANDLE, VK_IMAGE_LAYOUT_UNDEFINED};
      command.texture_descriptor_indices[stage] = stage;
      command.sampler_descriptor_indices[stage] = stage;

      NativeTextureImage* image = GetOrCreateTextureImage(command_buffer, command.textures[stage]);
      if (image) {
        switch (image->source->info.dimension) {
          case xenos::DataDimension::k3D:
            images_3d[stage].imageView = image->resource.view;
            break;
          case xenos::DataDimension::kCube:
            images_cube[stage].imageView = image->resource.view;
            break;
          case xenos::DataDimension::k2DOrStacked:
            if (image->source->info.is_stacked) {
              images_2d_array[stage].imageView = image->resource.view;
            } else {
              images_2d[stage].imageView = image->resource.view;
            }
            break;
          default:
            images_2d[stage].imageView = image->resource.view;
            break;
        }
        NativeSamplerKey effective_sampler_key{};
        const VkSampler sampler = GetOrCreateSampler(command.texture_fetches[stage], image,
                                                     &effective_sampler_key);
        samplers[stage].sampler = sampler ? sampler : null_sampler_;
        if (trace_reflections && image->is_reflection) {
          uint32_t guest_mip_min = 0;
          uint32_t guest_mip_max = 0;
          texture_util::GetSubresourcesFromFetchConstant(
              command.texture_fetches[stage], nullptr, nullptr, nullptr, nullptr, nullptr,
              &guest_mip_min, &guest_mip_max);
          const NativeShader* vertex_shader =
              command.pipeline_state ? command.pipeline_state->vertex_shader_resource : nullptr;
          const NativeShader* pixel_shader =
              command.pipeline_state ? command.pipeline_state->pixel_shader_resource : nullptr;
          const auto& fetch = command.texture_fetches[stage];
          REXLOG_INFO(
              "gta4-native-reflection-trace: point=sample-bind frame={} cmd={} phase={} "
              "object={:08X} stage={} handle={:08X} generation={} family={} role={} "
              "logical={}x{} physical={}x{} guest-mips={} physical-mips={} dimension={} "
              "stacked={} layout={} vs={:016X}:{} ps={:016X}:{} constants={:016X}/{:016X} "
              "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
              "guest-mip-range={}-{} effective-filter={}/{}/{} effective-aniso={} "
              "effective-clamp={}/{}/{} effective-lod-bias={} effective-mip-range={}-{}",
              submitted_frame, command_index, RenderPhaseName(command.render_phase),
              command.render_phase_object, stage,
              image->source ? image->source->handle : 0,
              image->source ? image->source->generation : 0,
              uint32_t(image->reflection.family), uint32_t(image->reflection.role),
              image->logical_width, image->logical_height, image->width, image->height,
              image->guest_mip_levels, image->mip_levels,
              image->source ? uint32_t(image->source->info.dimension) : 0,
              image->source ? image->source->info.is_stacked : false, uint32_t(image->layout),
              vertex_shader ? vertex_shader->hash : 0,
              vertex_shader ? vertex_shader->filename : std::string{},
              pixel_shader ? pixel_shader->hash : 0,
              pixel_shader ? pixel_shader->filename : std::string{},
              command.vertex_constants_hash, command.pixel_constants_hash,
              fetch.dword_0, fetch.dword_1, fetch.dword_2, fetch.dword_3, fetch.dword_4,
              fetch.dword_5, guest_mip_min, guest_mip_max,
              uint32_t(effective_sampler_key.min_filter),
              uint32_t(effective_sampler_key.mag_filter),
              uint32_t(effective_sampler_key.mip_filter),
              uint32_t(effective_sampler_key.aniso_filter),
              uint32_t(effective_sampler_key.clamp_u),
              uint32_t(effective_sampler_key.clamp_v),
              uint32_t(effective_sampler_key.clamp_w), effective_sampler_key.lod_bias,
              effective_sampler_key.mip_min_level, effective_sampler_key.mip_max_level);
        }
        if (image->source->vector_font_replacement &&
            REXCVAR_GET(gta4_trace_vector_fonts)) {
          static std::atomic<uint64_t> font_descriptor_trace_count{0};
          const uint64_t trace = ++font_descriptor_trace_count;
          if (trace <= 96) {
            REXLOG_INFO(
                "gta4-native-font-debug: descriptor #{} stage={} font={} handle={:08X} "
                "generation={} dimension={} view={} sampler={} fallback-sampler={} "
                "texture-index={} sampler-index={}",
                trace, stage, image->source->vector_font_id, image->source->handle,
                image->source->generation, uint32_t(image->source->info.dimension),
                fmt::ptr(image->resource.view), fmt::ptr(samplers[stage].sampler), !sampler,
                command.texture_descriptor_indices[stage],
                command.sampler_descriptor_indices[stage]);
          }
        }
      }
    }

    std::array<VkWriteDescriptorSet, kDrawDescriptorSetCount> writes{};
    const std::array<const VkDescriptorImageInfo*, kDrawDescriptorSetCount> descriptor_infos = {
        images_2d.data(), images_2d_array.data(), images_3d.data(), images_cube.data(),
        samplers.data()};
    for (uint32_t set = 0; set < kDrawDescriptorSetCount; ++set) {
      writes[set].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writes[set].dstSet = command.draw_descriptor_sets[set];
      writes[set].dstBinding = 0;
      writes[set].descriptorCount = kShaderTextureCount;
      writes[set].descriptorType =
          set < 4 ? VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE : VK_DESCRIPTOR_TYPE_SAMPLER;
      writes[set].pImageInfo = descriptor_infos[set];
    }
    dfn.vkUpdateDescriptorSets(device, uint32_t(writes.size()), writes.data(), 0, nullptr);
  }
  return true;
}

void Gta4NativeGraphicsSystem::ReleaseUnusedTextureImages() {
  std::unordered_set<uint64_t> used_generations;
  {
    std::lock_guard lock(texture_resource_mutex_);
    for (const auto& [handle, resource] : texture_resources_) {
      (void)handle;
      if (resource) {
        used_generations.insert(resource->generation);
      }
    }
  }
  for (const NativeCommand& command : current_frame_) {
    if (command.resolve_destination) {
      used_generations.insert(command.resolve_destination->generation);
    }
    if (command.depth_handoff_source) {
      used_generations.insert(command.depth_handoff_source->generation);
    }
    for (const auto& texture : command.textures) {
      if (texture) {
        used_generations.insert(texture->generation);
      }
    }
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  for (auto image = native_texture_images_.begin(); image != native_texture_images_.end();) {
    if (used_generations.contains(image->first)) {
      ++image;
      continue;
    }
    for (VkImageView mip_view : image->second->mip_views) {
      if (mip_view) {
        dfn.vkDestroyImageView(device, mip_view, nullptr);
      }
    }
    if (image->second->resource.view) {
      dfn.vkDestroyImageView(device, image->second->resource.view, nullptr);
    }
    if (image->second->resource.image) {
      dfn.vkDestroyImage(device, image->second->resource.image, nullptr);
    }
    if (image->second->resource.memory) {
      dfn.vkFreeMemory(device, image->second->resource.memory, nullptr);
    }
    image = native_texture_images_.erase(image);
  }
}

Gta4NativeGraphicsSystem::NativeSurfaceImage* Gta4NativeGraphicsSystem::GetOrCreateSurfaceImage(
    const SurfaceDescriptor& descriptor, bool depth) {
  auto reject = [&descriptor, depth](const char* reason) -> NativeSurfaceImage* {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    if (count <= 32 || !(count % 1024)) {
      REXLOG_WARN(
          "gta4-native-diag: surface reject #{} reason={} depth={} handle={:08X} flags={:08X} "
          "base={:08X} address={:08X} packed={:08X} format={} size={}x{} samples={}",
          count, reason, depth, descriptor.handle, descriptor.flags, descriptor.base,
          descriptor.address, descriptor.packed_dimensions, descriptor.format, descriptor.width,
          descriptor.height, descriptor.sample_type);
    }
    return nullptr;
  };

  if (!descriptor.handle || !descriptor.width || !descriptor.height) {
    return reject("missing-handle-or-dimensions");
  }
  const VkImageAspectFlags aspect =
      depth ? (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) : VK_IMAGE_ASPECT_COLOR_BIT;
  uint32_t image_width = descriptor.width;
  uint32_t image_height = descriptor.height;
  NativeReflectionTarget reflection{};
  bool is_reflection = false;
  const auto reflection_entry = reflection_resources_.find(descriptor.handle);
  if (reflection_entry != reflection_resources_.end()) {
    reflection = reflection_entry->second;
    is_reflection = true;
    image_width = reflection.physical_width;
    image_height = reflection.physical_height;
  }

  const VkFormat format = ConvertSurfaceFormat(descriptor.format, depth);
  VkSampleCountFlagBits samples = ConvertSurfaceSamples(descriptor.sample_type);
  if (format == VK_FORMAT_UNDEFINED || samples == VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM) {
    return reject(format == VK_FORMAT_UNDEFINED ? "unsupported-format" : "unsupported-samples");
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  if (is_reflection && reflection.sample_count_override) {
    const VkSampleCountFlagBits requested_samples =
        ConvertNativeSampleCount(reflection.sample_count_override);
    const auto& properties = vulkan_device->properties();
    // Choose from the intersection for the entire color/depth family so the
    // two attachments can never independently settle on different counts.
    const VkSampleCountFlags supported_samples =
        properties.framebufferColorSampleCounts &
        properties.framebufferDepthSampleCounts &
        properties.framebufferStencilSampleCounts &
        properties.sampledImageColorSampleCounts &
        properties.sampledImageDepthSampleCounts;
    samples = requested_samples;
    while (samples != VK_SAMPLE_COUNT_1_BIT && !(supported_samples & samples)) {
      samples = samples == VK_SAMPLE_COUNT_4_BIT ? VK_SAMPLE_COUNT_2_BIT
                                                 : VK_SAMPLE_COUNT_1_BIT;
    }
    if (!(supported_samples & samples)) {
      return reject("reflection-sample-capabilities");
    }
  }
  for (const auto& image : native_surface_images_) {
    if (image->aspect == aspect && image->width == image_width && image->height == image_height &&
        image->samples == samples && SurfaceDescriptorsEqual(image->descriptor, descriptor)) {
      if (is_reflection) {
        using CacheSignature =
            std::tuple<uint32_t, VkImageAspectFlags, uint32_t, uint32_t, VkSampleCountFlagBits>;
        static std::set<CacheSignature> logged_reflection_cache_hits;
        if (logged_reflection_cache_hits
                .emplace(descriptor.handle, aspect, image_width, image_height, samples)
                .second) {
          REXLOG_INFO(
              "gta4-native-cause: point=reflection-surface-cache-hit handle={:08X} depth={} "
              "family={} role={} guest={}x{} logical={}x{} host={}x{} samples={} written={}",
              descriptor.handle, depth, uint32_t(reflection.family), uint32_t(reflection.role),
              descriptor.width, descriptor.height, image->logical_width, image->logical_height,
              image->width, image->height, uint32_t(image->samples), image->ever_written);
        }
      }
      return image.get();
    }
  }
  const uint32_t maximum_extent = vulkan_device->properties().maxImageDimension2D;
  if (image_width > maximum_extent || image_height > maximum_extent) {
    if (!is_reflection) {
      return reject("dimension-limit");
    }
    REXLOG_WARN(
        "gta4-native-reflection: surface {:08X} requested {}x{} exceeds device limit {}; "
        "using original {}x{}",
        descriptor.handle, image_width, image_height, maximum_extent, descriptor.width,
        descriptor.height);
    image_width = descriptor.width;
    image_height = descriptor.height;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  auto image = std::make_unique<NativeSurfaceImage>();
  image->descriptor = descriptor;
  image->format = format;
  image->aspect = aspect;
  image->samples = samples;
  image->width = image_width;
  image->height = image_height;
  image->logical_width = is_reflection ? reflection.logical_width : descriptor.width;
  image->logical_height = is_reflection ? reflection.logical_height : descriptor.height;
  image->reflection = reflection;
  image->is_reflection = is_reflection;

  std::string same_address_aliases;
  bool has_written_same_address_alias = false;
  if (descriptor.address) {
    for (const auto& existing : native_surface_images_) {
      if (!existing || existing->aspect != aspect ||
          existing->descriptor.address != descriptor.address ||
          existing->descriptor.handle == descriptor.handle) {
        continue;
      }
      same_address_aliases += fmt::format(
          "{}{:08X}:guest={}x{}:host={}x{}:format={}:samples={}:layout={}:written={}",
          same_address_aliases.empty() ? "" : ",", existing->descriptor.handle,
          existing->descriptor.width, existing->descriptor.height, existing->width,
          existing->height, uint32_t(existing->format), uint32_t(existing->samples),
          uint32_t(existing->layout), existing->ever_written);
      has_written_same_address_alias |= existing->ever_written;
    }
  }

  VkImageCreateInfo image_info{};
  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.format = format;
  image_info.extent = {image_width, image_height, 1};
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.samples = samples;
  image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_info.usage =
      depth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  image_info.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                      VK_IMAGE_USAGE_SAMPLED_BIT;
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (!ui::vulkan::util::CreateDedicatedAllocationImage(
          vulkan_device, image_info, ui::vulkan::util::MemoryPurpose::kDeviceLocal,
          image->resource.image, image->resource.memory)) {
    if (!is_reflection ||
        (image_width == image->logical_width && image_height == image->logical_height)) {
      return reject("image-allocation");
    }
    REXLOG_WARN(
        "gta4-native-reflection: surface {:08X} allocation failed at {}x{}; retrying "
        "original {}x{}",
        descriptor.handle, image_width, image_height, image->logical_width,
        image->logical_height);
    image_width = image->logical_width;
    image_height = image->logical_height;
    image->width = image_width;
    image->height = image_height;
    image_info.extent.width = image_width;
    image_info.extent.height = image_height;
    if (!ui::vulkan::util::CreateDedicatedAllocationImage(
            vulkan_device, image_info, ui::vulkan::util::MemoryPurpose::kDeviceLocal,
            image->resource.image, image->resource.memory)) {
      return reject("image-allocation-original-fallback");
    }
  }

  VkImageViewCreateInfo view_info{};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.image = image->resource.image;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = format;
  view_info.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(aspect, 0, 1, 0, 1);
  if (dfn.vkCreateImageView(device, &view_info, nullptr, &image->resource.view) != VK_SUCCESS) {
    dfn.vkDestroyImage(device, image->resource.image, nullptr);
    dfn.vkFreeMemory(device, image->resource.memory, nullptr);
    return reject("image-view");
  }
  if (depth) {
    view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (dfn.vkCreateImageView(device, &view_info, nullptr, &image->sampled_view) != VK_SUCCESS) {
      dfn.vkDestroyImageView(device, image->resource.view, nullptr);
      dfn.vkDestroyImage(device, image->resource.image, nullptr);
      dfn.vkFreeMemory(device, image->resource.memory, nullptr);
      return reject("sampled-depth-view");
    }
  }

  NativeSurfaceImage* result = image.get();
  native_surface_images_.push_back(std::move(image));
  if (!same_address_aliases.empty()) {
    REXLOG_ERROR(
        "gta4-native-cause: point=surface-address-alias-create handle={:08X} "
        "address={:08X} depth={} guest={}x{} host={}x{} format={} samples={} "
        "written-alias={} aliases=[{}]",
        descriptor.handle, descriptor.address, depth, descriptor.width, descriptor.height,
        image_width, image_height, uint32_t(format), uint32_t(samples),
        has_written_same_address_alias, same_address_aliases);
  }
  REXLOG_INFO(
      "gta4-native: created {} surface {:08X} guest={}x{} host={}x{} format {} samples {} "
      "reflection={} family={} role={} logical={}x{}",
      depth ? "depth" : "color", descriptor.handle, descriptor.width, descriptor.height,
      image_width, image_height, uint32_t(format), uint32_t(samples), is_reflection,
      uint32_t(reflection.family), uint32_t(reflection.role), result->logical_width,
      result->logical_height);
  return result;
}

bool Gta4NativeGraphicsSystem::ResolveRenderingTarget(const NativePipelineState& state,
                                                      VkImageView presenter_view,
                                                      uint32_t presenter_width,
                                                      uint32_t presenter_height,
                                                      NativeRenderingTarget& target) {
  target = {};
  auto reject = [this, &state, &target, presenter_view, presenter_width,
                 presenter_height](const char* reason) {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    using RejectionSignature = std::tuple<std::string, uint32_t, uint32_t>;
    static std::set<RejectionSignature> logged_signatures;
    const bool first_signature =
        logged_signatures.emplace(reason, state.render_targets[0].handle,
                                  state.depth_stencil.handle).second;
    const auto rt0_registration = reflection_resources_.find(state.render_targets[0].handle);
    const auto depth_registration = reflection_resources_.find(state.depth_stencil.handle);
    const bool rt0_registered = rt0_registration != reflection_resources_.end();
    const bool depth_registered = depth_registration != reflection_resources_.end();
    const NativeSurfaceImage* rt0_image = target.color_surfaces[0];
    const NativeSurfaceImage* depth_image = target.depth_surface;
    if (first_signature || count <= 32 || !(count % 1024)) {
      std::fprintf(
          stderr,
          "[TargetRejectReason] count=%llu reason=%s presenter=%u/%ux%u "
          "rt0=%08X/%08X/f%08X/%ux%u/s%u rt1=%08X rt2=%08X rt3=%08X "
          "depth=%08X/%08X/f%08X/%ux%u/s%u\n",
          static_cast<unsigned long long>(count), reason,
          unsigned(presenter_view != VK_NULL_HANDLE), presenter_width, presenter_height,
          state.render_targets[0].handle, state.render_targets[0].address,
          state.render_targets[0].format, state.render_targets[0].width,
          state.render_targets[0].height, state.render_targets[0].sample_type,
          state.render_targets[1].handle, state.render_targets[2].handle,
          state.render_targets[3].handle, state.depth_stencil.handle,
          state.depth_stencil.address, state.depth_stencil.format, state.depth_stencil.width,
          state.depth_stencil.height, state.depth_stencil.sample_type);
      std::fflush(stderr);
      REXLOG_WARN(
          "gta4-native-cause: point=target-reject frame={} cmd={} count={} reason={} "
          "presenter-view={} presenter={}x{} "
          "rt0={:08X}/{}x{}/f{}/s{} rt0-image={}x{}/logical={}x{}/samples={}/reflection={} "
          "rt0-reg={}/family={}/role={}/logical={}x{}/host={}x{}/samples={} "
          "depth={:08X}/{}x{}/f{}/s{} depth-image={}x{}/logical={}x{}/samples={}/reflection={} "
          "depth-reg={}/family={}/role={}/logical={}x{}/host={}x{}/samples={}",
          diagnostic_submitted_frame_, diagnostic_command_index_, count, reason,
          presenter_view != VK_NULL_HANDLE, presenter_width, presenter_height,
          state.render_targets[0].handle, state.render_targets[0].width,
          state.render_targets[0].height, state.render_targets[0].format,
          state.render_targets[0].sample_type, rt0_image ? rt0_image->width : 0,
          rt0_image ? rt0_image->height : 0, rt0_image ? rt0_image->logical_width : 0,
          rt0_image ? rt0_image->logical_height : 0,
          rt0_image ? uint32_t(rt0_image->samples) : 0,
          rt0_image ? rt0_image->is_reflection : false, rt0_registered,
          rt0_registered ? uint32_t(rt0_registration->second.family) : 0,
          rt0_registered ? uint32_t(rt0_registration->second.role) : 0,
          rt0_registered ? rt0_registration->second.logical_width : 0,
          rt0_registered ? rt0_registration->second.logical_height : 0,
          rt0_registered ? rt0_registration->second.physical_width : 0,
          rt0_registered ? rt0_registration->second.physical_height : 0,
          rt0_registered ? rt0_registration->second.sample_count_override : 0,
          state.depth_stencil.handle, state.depth_stencil.width, state.depth_stencil.height,
          state.depth_stencil.format, state.depth_stencil.sample_type,
          depth_image ? depth_image->width : 0, depth_image ? depth_image->height : 0,
          depth_image ? depth_image->logical_width : 0,
          depth_image ? depth_image->logical_height : 0,
          depth_image ? uint32_t(depth_image->samples) : 0,
          depth_image ? depth_image->is_reflection : false, depth_registered,
          depth_registered ? uint32_t(depth_registration->second.family) : 0,
          depth_registered ? uint32_t(depth_registration->second.role) : 0,
          depth_registered ? depth_registration->second.logical_width : 0,
          depth_registered ? depth_registration->second.logical_height : 0,
          depth_registered ? depth_registration->second.physical_width : 0,
          depth_registered ? depth_registration->second.physical_height : 0,
          depth_registered ? depth_registration->second.sample_count_override : 0);
    }
    return false;
  };

  bool samples_initialized = false;
  for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
    const SurfaceDescriptor& descriptor = state.render_targets[index];
    if (!descriptor.handle) {
      continue;
    }
    NativeSurfaceImage* image = GetOrCreateSurfaceImage(descriptor, false);
    if (!image) {
      return reject("color-surface");
    }
    if (samples_initialized && target.samples != image->samples) {
      return reject("color-sample-mismatch");
    }
    target.samples = image->samples;
    samples_initialized = true;
    target.color_surfaces[index] = image;
    target.color_views[index] = image->resource.view;
    target.color_formats[index] = image->format;
    if (!target.width) {
      target.width = image->width;
      target.height = image->height;
      target.logical_width = image->logical_width;
      target.logical_height = image->logical_height;
      target.reflection = image->reflection;
      target.is_reflection = image->is_reflection;
    } else if (target.width != image->width || target.height != image->height) {
      return reject("color-size-mismatch");
    } else if (target.logical_width != image->logical_width ||
               target.logical_height != image->logical_height ||
               target.is_reflection != image->is_reflection ||
               (target.is_reflection && target.reflection.family != image->reflection.family)) {
      return reject("color-logical-size-or-family-mismatch");
    }
  }

  if (state.depth_stencil.handle) {
    target.depth_surface = GetOrCreateSurfaceImage(state.depth_stencil, true);
    if (!target.depth_surface ||
        (samples_initialized && target.samples != target.depth_surface->samples)) {
      return reject(target.depth_surface ? "depth-sample-mismatch" : "depth-surface");
    }
    target.samples = target.depth_surface->samples;
    samples_initialized = true;
    target.depth_view = target.depth_surface->resource.view;
    target.depth_format = target.depth_surface->format;
    if (!target.width) {
      target.width = target.depth_surface->width;
      target.height = target.depth_surface->height;
      target.logical_width = target.depth_surface->logical_width;
      target.logical_height = target.depth_surface->logical_height;
      target.reflection = target.depth_surface->reflection;
      target.is_reflection = target.depth_surface->is_reflection;
    } else if (target.width != target.depth_surface->width ||
               target.height != target.depth_surface->height) {
      return reject("depth-size-mismatch");
    } else if (target.logical_width != target.depth_surface->logical_width ||
               target.logical_height != target.depth_surface->logical_height ||
               target.is_reflection != target.depth_surface->is_reflection ||
               (target.is_reflection &&
                target.reflection.family != target.depth_surface->reflection.family)) {
      return reject("depth-logical-size-or-family-mismatch");
    }
  }

  const bool has_color = std::any_of(target.color_views.begin(), target.color_views.end(),
                                     [](VkImageView view) { return view != VK_NULL_HANDLE; });
  if (!has_color && !target.depth_view) {
    target.color_views[0] = presenter_view;
    target.color_formats[0] = ui::vulkan::VulkanPresenter::kGuestOutputFormat;
    target.width = presenter_width;
    target.height = presenter_height;
    target.logical_width = presenter_width;
    target.logical_height = presenter_height;
    target.samples = VK_SAMPLE_COUNT_1_BIT;
    target.uses_presenter = true;
  }
  return (target.width && target.height) || reject("zero-target-dimensions");
}

bool Gta4NativeGraphicsSystem::TransitionRenderingTarget(VkCommandBuffer command_buffer,
                                                         NativeRenderingTarget& target) {
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto& dfn = vulkan_provider->vulkan_device()->functions();
  for (NativeSurfaceImage* image : target.color_surfaces) {
    if (!image || image->layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
      continue;
    }
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask = image->ever_written ? VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT : 0;
    barrier.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.oldLayout = image->layout;
    barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image->resource.image;
    barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(command_buffer,
                             image->ever_written ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                                                 : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0,
                             nullptr, 1, &barrier);
    image->layout = barrier.newLayout;
  }
  if (target.depth_surface &&
      target.depth_surface->layout != VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    NativeSurfaceImage& image = *target.depth_surface;
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.srcAccessMask = image.ever_written ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT : 0;
    barrier.dstAccessMask =
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    barrier.oldLayout = image.layout;
    barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image.resource.image;
    barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(image.aspect, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(
        command_buffer,
        image.ever_written ? VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
                           : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, 0,
        0, nullptr, 0, nullptr, 1, &barrier);
    image.layout = barrier.newLayout;
  }
  return true;
}

VkPipeline Gta4NativeGraphicsSystem::GetOrCreatePipeline(
    const NativePipelineState& state, const NativeFixedFunctionState& fixed_function_state,
    uint32_t primitive_type, const NativeRenderingTarget& target, uint32_t user_pointer_stride,
    bool primitive_restart_enable) {
  SCOPE_profile_cpu_i("gpu", "GTA4 Native GetOrCreatePipeline");
  auto reject = [&state, primitive_type, &target](const char* reason) {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = ++rejection_count;
    if (count <= 32 || !(count % 1024)) {
      REXLOG_WARN(
          "gta4-native-diag: pipeline reject #{} reason={} primitive={} vs={:08X}/{} "
          "ps={:08X}/{} vdecl={:08X}/{} rt0={:08X} depth={:08X} target={}x{} presenter={}",
          count, reason, primitive_type, state.vertex_shader,
          state.vertex_shader_resource ? "ready" : "missing", state.pixel_shader,
          state.pixel_shader_resource ? "ready" : "missing", state.vertex_declaration,
          state.vertex_declaration_resource ? "ready" : "missing", state.render_targets[0].handle,
          state.depth_stencil.handle, target.width, target.height, target.uses_presenter);
    }
    return VK_NULL_HANDLE;
  };

  const bool has_color_target =
      std::any_of(target.color_formats.begin(), target.color_formats.end(),
                  [](VkFormat format) { return format != VK_FORMAT_UNDEFINED; });
  const bool depth_only_without_pixel_shader =
      !state.pixel_shader_resource && target.depth_format != VK_FORMAT_UNDEFINED &&
      (!has_color_target || ConvertColorWriteMask(fixed_function_state.color_write_mask) == 0);
  if (!state.vertex_shader_resource || !state.vertex_shader_resource->early_module ||
      (!state.pixel_shader_resource && !depth_only_without_pixel_shader) ||
      (state.pixel_shader_resource && !state.pixel_shader_resource->early_module) ||
      !state.vertex_declaration_resource || !pipeline_layout_) {
    return reject("missing-state");
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  if (!vulkan_device) {
    return reject("missing-vulkan-device");
  }
  const bool user_pointer = user_pointer_stride != 0;
  NativePipelineKey key{};
  key.vertex_shader_hash = state.vertex_shader_resource->hash;
  key.pixel_shader_hash =
      state.pixel_shader_resource ? state.pixel_shader_resource->hash : 0;
  key.vertex_declaration_hash = state.vertex_declaration_resource->content_hash;
  key.primitive_type = primitive_type;
  key.color_formats = target.color_formats;
  key.depth_format = target.depth_format;
  key.samples = target.samples;
  key.user_pointer = user_pointer;
  key.depth_enable = fixed_function_state.depth_enable;
  key.depth_function = fixed_function_state.depth_function;
  key.depth_write_enable = fixed_function_state.depth_write_enable;
  key.cull_mode = fixed_function_state.cull_mode;
  key.blend_enable = fixed_function_state.blend_enable;
  key.source_blend = fixed_function_state.source_blend;
  key.destination_blend = fixed_function_state.destination_blend;
  key.blend_operation = fixed_function_state.blend_operation;
  key.source_blend_alpha = fixed_function_state.source_blend_alpha;
  key.destination_blend_alpha = fixed_function_state.destination_blend_alpha;
  key.blend_operation_alpha = fixed_function_state.blend_operation_alpha;
  key.alpha_test_enable = fixed_function_state.alpha_test_enable;
  key.alpha_function = fixed_function_state.alpha_function;
  key.stencil_enable = fixed_function_state.stencil_enable;
  key.two_sided_stencil = fixed_function_state.two_sided_stencil;
  key.stencil_fail = fixed_function_state.stencil_fail;
  key.stencil_depth_fail = fixed_function_state.stencil_depth_fail;
  key.stencil_pass = fixed_function_state.stencil_pass;
  key.stencil_function = fixed_function_state.stencil_function;
  key.stencil_mask = fixed_function_state.stencil_mask;
  key.stencil_write_mask = fixed_function_state.stencil_write_mask;
  key.ccw_stencil_fail = fixed_function_state.ccw_stencil_fail;
  key.ccw_stencil_depth_fail = fixed_function_state.ccw_stencil_depth_fail;
  key.ccw_stencil_pass = fixed_function_state.ccw_stencil_pass;
  key.ccw_stencil_function = fixed_function_state.ccw_stencil_function;
  key.color_write_mask = fixed_function_state.color_write_mask;
  key.depth_bias_enable = fixed_function_state.depth_bias_enable;
  key.primitive_restart_enable = primitive_restart_enable;
  if (user_pointer) {
    key.vertex_strides[0] = user_pointer_stride;
  } else {
    for (uint32_t stream = 0; stream < kVertexStreamCount; ++stream) {
      key.vertex_strides[stream] = state.vertex_streams[stream].stride;
    }
  }
  for (const auto& native_pipeline : native_pipelines_) {
    if (native_pipeline.key == key) {
      return native_pipeline.pipeline;
    }
  }

  const VkPrimitiveTopology topology = ConvertPrimitiveTopology(primitive_type);
  if (topology == VK_PRIMITIVE_TOPOLOGY_MAX_ENUM) {
    return reject("unsupported-topology");
  }

  std::vector<VkVertexInputAttributeDescription> attributes;
  std::vector<VkVertexInputBindingDescription> bindings;
  std::array<bool, kVertexStreamCount> binding_used{};
  bool default_binding_used = false;
  attributes.reserve(state.vertex_shader_resource->vertex_inputs.size());
  for (const NativeVertexInput& shader_input : state.vertex_shader_resource->vertex_inputs) {
    const VertexElement* matching_element = nullptr;
    for (const VertexElement& element : state.vertex_declaration_resource->elements) {
      if (ConvertVertexUsageToLocation(element.usage, element.usage_index) ==
          shader_input.location) {
        matching_element = &element;
        break;
      }
    }

    VkVertexInputAttributeDescription attribute{};
    attribute.location = shader_input.location;
    if (!matching_element) {
      attribute.binding = kDefaultVertexBinding;
      attribute.format =
          GetDefaultVertexFormat(shader_input.numeric_type, shader_input.component_count);
      attribute.offset = 0;
      if (attribute.format == VK_FORMAT_UNDEFINED) {
        return reject("unsupported-default-vertex-format");
      }
      default_binding_used = true;
      attributes.push_back(attribute);
      continue;
    }

    const uint32_t stream = user_pointer ? 0 : matching_element->stream;
    if ((user_pointer && matching_element->stream != 0) || stream >= kVertexStreamCount) {
      return reject("required-vertex-stream-range");
    }
    if (!key.vertex_strides[stream] || matching_element->offset >= key.vertex_strides[stream]) {
      return reject("required-vertex-stream-layout");
    }
    attribute.binding = stream;
    attribute.format =
        GetCompatibleVertexFormat(matching_element->type, shader_input.numeric_type);
    attribute.offset = matching_element->offset;
    if (attribute.format == VK_FORMAT_UNDEFINED) {
      static std::atomic<uint64_t> vertex_format_rejection_count{0};
      const uint64_t count = ++vertex_format_rejection_count;
      if (count <= 64 || !(count % 1024)) {
        REXLOG_WARN(
            "gta4-native-diag: vertex format reject #{} vs={:08X} vdecl={:08X} "
            "location={} numeric={} components={} stream={} offset={} type={:08X} "
            "usage={}/{} converted={}",
            count, state.vertex_shader, state.vertex_declaration, shader_input.location,
            uint32_t(shader_input.numeric_type), shader_input.component_count,
            matching_element->stream, matching_element->offset, matching_element->type,
            matching_element->usage, matching_element->usage_index,
            uint32_t(ConvertVertexElementFormat(matching_element->type)));
      }
      return reject("vertex-format-numeric-type");
    }
    attributes.push_back(attribute);
    binding_used[stream] = true;
  }

  if (default_binding_used && binding_used[kDefaultVertexBinding]) {
    return reject("default-vertex-binding-conflict");
  }

  for (uint32_t stream = 0; stream < kVertexStreamCount; ++stream) {
    if (!binding_used[stream]) {
      continue;
    }
    VkVertexInputBindingDescription binding{};
    binding.binding = stream;
    binding.stride = key.vertex_strides[stream];
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    bindings.push_back(binding);
  }
  if (default_binding_used) {
    VkVertexInputBindingDescription binding{};
    binding.binding = kDefaultVertexBinding;
    binding.stride = 0;
    binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    bindings.push_back(binding);
  }
  VkPipelineVertexInputStateCreateInfo vertex_input{};
  vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input.vertexBindingDescriptionCount = uint32_t(bindings.size());
  vertex_input.pVertexBindingDescriptions = bindings.data();
  vertex_input.vertexAttributeDescriptionCount = uint32_t(attributes.size());
  vertex_input.pVertexAttributeDescriptions = attributes.data();

  VkPipelineInputAssemblyStateCreateInfo input_assembly{};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = topology;
  const bool moltenvk_strip_restart =
      vulkan_device->properties().driverID == VK_DRIVER_ID_MOLTENVK &&
      (topology == VK_PRIMITIVE_TOPOLOGY_LINE_STRIP ||
       topology == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP ||
       topology == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN);
  input_assembly.primitiveRestartEnable =
      (primitive_restart_enable || moltenvk_strip_restart) ? VK_TRUE : VK_FALSE;

  VkPipelineViewportStateCreateInfo viewport_state{};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterization{};
  rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization.polygonMode = VK_POLYGON_MODE_FILL;
  switch (fixed_function_state.cull_mode) {
    case 1:
      rasterization.cullMode = VK_CULL_MODE_FRONT_BIT;
      rasterization.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
      break;
    case 2:
      rasterization.cullMode = VK_CULL_MODE_BACK_BIT;
      rasterization.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
      break;
    case 5:
      rasterization.cullMode = VK_CULL_MODE_FRONT_BIT;
      rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
      break;
    case 6:
      rasterization.cullMode = VK_CULL_MODE_BACK_BIT;
      rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
      break;
    case 4:
      rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
      rasterization.cullMode = VK_CULL_MODE_NONE;
      break;
    case 0:
    default:
      rasterization.cullMode = VK_CULL_MODE_NONE;
      rasterization.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
      break;
  }
  rasterization.depthBiasEnable = key.depth_bias_enable;
  rasterization.lineWidth = 1.0f;

  VkPipelineMultisampleStateCreateInfo multisample{};
  multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample.rasterizationSamples = target.samples;

  std::array<VkPipelineColorBlendAttachmentState, kRenderTargetCount> color_attachments{};
  uint32_t color_attachment_count = 0;
  for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
    if (target.color_formats[index] != VK_FORMAT_UNDEFINED) {
      color_attachment_count = index + 1;
    }
    color_attachments[index].blendEnable = fixed_function_state.blend_enable != 0;
    color_attachments[index].srcColorBlendFactor =
        ConvertBlendFactor(fixed_function_state.source_blend);
    color_attachments[index].dstColorBlendFactor =
        ConvertBlendFactor(fixed_function_state.destination_blend);
    color_attachments[index].colorBlendOp =
        ConvertBlendOperation(fixed_function_state.blend_operation);
    color_attachments[index].srcAlphaBlendFactor =
        ConvertBlendFactor(fixed_function_state.source_blend_alpha);
    color_attachments[index].dstAlphaBlendFactor =
        ConvertBlendFactor(fixed_function_state.destination_blend_alpha);
    color_attachments[index].alphaBlendOp =
        ConvertBlendOperation(fixed_function_state.blend_operation_alpha);
    if (!vulkan_device->properties().constantAlphaColorBlendFactors) {
      if (fixed_function_state.source_blend == 14) {
        color_attachments[index].srcColorBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
      } else if (fixed_function_state.source_blend == 15) {
        color_attachments[index].srcColorBlendFactor =
            VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
      }
      if (fixed_function_state.destination_blend == 14) {
        color_attachments[index].dstColorBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR;
      } else if (fixed_function_state.destination_blend == 15) {
        color_attachments[index].dstColorBlendFactor =
            VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
      }
    }
    color_attachments[index].colorWriteMask =
        ConvertColorWriteMask(fixed_function_state.color_write_mask);
  }
  VkPipelineColorBlendStateCreateInfo color_blend{};
  color_blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend.attachmentCount = color_attachment_count;
  color_blend.pAttachments = color_attachments.data();

  VkPipelineDepthStencilStateCreateInfo depth_stencil{};
  depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  const bool has_depth = target.depth_format != VK_FORMAT_UNDEFINED;
  depth_stencil.depthTestEnable = has_depth && fixed_function_state.depth_enable != 0;
  depth_stencil.depthWriteEnable = has_depth && fixed_function_state.depth_write_enable != 0;
  depth_stencil.depthCompareOp = ConvertCompareFunction(fixed_function_state.depth_function);
  depth_stencil.stencilTestEnable = has_depth && fixed_function_state.stencil_enable != 0;
  depth_stencil.front.failOp = ConvertStencilOperation(fixed_function_state.stencil_fail);
  depth_stencil.front.passOp = ConvertStencilOperation(fixed_function_state.stencil_pass);
  depth_stencil.front.depthFailOp =
      ConvertStencilOperation(fixed_function_state.stencil_depth_fail);
  depth_stencil.front.compareOp = ConvertCompareFunction(fixed_function_state.stencil_function);
  depth_stencil.front.compareMask = fixed_function_state.stencil_mask;
  depth_stencil.front.writeMask = fixed_function_state.stencil_write_mask;
  depth_stencil.front.reference = fixed_function_state.stencil_reference;
  depth_stencil.back = depth_stencil.front;
  if (fixed_function_state.two_sided_stencil) {
    depth_stencil.back.failOp = ConvertStencilOperation(fixed_function_state.ccw_stencil_fail);
    depth_stencil.back.passOp = ConvertStencilOperation(fixed_function_state.ccw_stencil_pass);
    depth_stencil.back.depthFailOp =
        ConvertStencilOperation(fixed_function_state.ccw_stencil_depth_fail);
    depth_stencil.back.compareOp =
        ConvertCompareFunction(fixed_function_state.ccw_stencil_function);
  }

  const std::array<VkDynamicState, 7> dynamic_states = {
      VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_DEPTH_BIAS,
      VK_DYNAMIC_STATE_STENCIL_REFERENCE, VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
      VK_DYNAMIC_STATE_STENCIL_WRITE_MASK, VK_DYNAMIC_STATE_BLEND_CONSTANTS};
  VkPipelineDynamicStateCreateInfo dynamic_state{};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount = uint32_t(dynamic_states.size());
  dynamic_state.pDynamicStates = dynamic_states.data();

  uint32_t specialization_value = 0;
  const bool alpha_coverage_requested = IsAlphaCoverageRequested(
      fixed_function_state.alpha_test_enable, fixed_function_state.alpha_function);
  const bool alpha_test_capable =
      state.pixel_shader_resource && HasAlphaTestCapability(
          state.pixel_shader_resource->specialization_constants_mask);
  const bool late_module_available =
      state.pixel_shader_resource && state.pixel_shader_resource->late_module;
  const bool use_late_module =
      alpha_coverage_requested && alpha_test_capable && late_module_available;
  if (use_late_module) {
    specialization_value =
        PackAlphaTestSpecialization(fixed_function_state.alpha_function);
  }
  if (state.pixel_shader_resource) {
    const std::string_view diagnostic_category =
        ClassifyTranslucentDiagnosticShader(state.pixel_shader_resource->filename);
    if (fixed_function_state.alpha_test_enable || !diagnostic_category.empty()) {
      REXLOG_INFO(
          "gta4-native-cause: point=alpha-pipeline-semantics frame={} cmd={} draw={:016X} "
          "category={} shader={} ps={:016X} requested={}:{}:{} capability={:08X} "
          "capable={} packed={:08X} coverage-depends-alpha={} module={} "
          "early-module={} late-module={} rejected={} depth={}:{}:{} stencil={}:{}",
          diagnostic_submitted_frame_, diagnostic_command_index_, diagnostic_draw_id_,
          diagnostic_category, state.pixel_shader_resource->filename,
          state.pixel_shader_resource->hash, fixed_function_state.alpha_test_enable,
          fixed_function_state.alpha_function, fixed_function_state.alpha_reference,
          state.pixel_shader_resource->specialization_constants_mask,
          alpha_test_capable, specialization_value, alpha_coverage_requested,
          use_late_module ? "late" : "early",
          state.pixel_shader_resource->early_module != VK_NULL_HANDLE,
          late_module_available,
          alpha_coverage_requested && !use_late_module,
          fixed_function_state.depth_enable, fixed_function_state.depth_function,
          fixed_function_state.depth_write_enable, fixed_function_state.stencil_enable,
          fixed_function_state.stencil_write_mask);
    }
  }
  if (alpha_coverage_requested && !use_late_module) {
    REXLOG_ERROR(
        "gta4-native: rejecting alpha coverage pipeline ps={:016X} function={} "
        "capability={:08X} late-module={}",
        state.pixel_shader_resource ? state.pixel_shader_resource->hash : 0,
        fixed_function_state.alpha_function,
        state.pixel_shader_resource
            ? state.pixel_shader_resource->specialization_constants_mask
            : 0,
        late_module_available);
    return reject(alpha_test_capable ? "alpha-late-module-missing"
                                     : "alpha-capability-missing");
  }
  VkSpecializationMapEntry specialization_entry{};
  specialization_entry.constantID = 0;
  specialization_entry.size = sizeof(specialization_value);
  VkSpecializationInfo specialization_info{};
  specialization_info.mapEntryCount = 1;
  specialization_info.pMapEntries = &specialization_entry;
  specialization_info.dataSize = sizeof(specialization_value);
  specialization_info.pData = &specialization_value;

  std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages{};
  shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  shader_stages[0].module = state.vertex_shader_resource->early_module;
  shader_stages[0].pName = "shaderMain";
  if (state.vertex_shader_resource->specialization_constants_mask) {
    shader_stages[0].pSpecializationInfo = &specialization_info;
  }
  uint32_t shader_stage_count = 1;
  if (state.pixel_shader_resource) {
    shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stages[1].module = use_late_module
                                  ? state.pixel_shader_resource->late_module
                                  : state.pixel_shader_resource->early_module;
    shader_stages[1].pName = "shaderMain";
    if (state.pixel_shader_resource->specialization_constants_mask) {
      shader_stages[1].pSpecializationInfo = &specialization_info;
    }
    shader_stage_count = 2;
  }

  VkPipelineRenderingCreateInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
  rendering_info.colorAttachmentCount = color_attachment_count;
  rendering_info.pColorAttachmentFormats =
      color_attachment_count ? target.color_formats.data() : nullptr;
  rendering_info.depthAttachmentFormat = target.depth_format;
  rendering_info.stencilAttachmentFormat = target.depth_format;

  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.pNext = &rendering_info;
  pipeline_info.stageCount = shader_stage_count;
  pipeline_info.pStages = shader_stages.data();
  pipeline_info.pVertexInputState = &vertex_input;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterization;
  pipeline_info.pMultisampleState = &multisample;
  pipeline_info.pDepthStencilState = &depth_stencil;
  pipeline_info.pColorBlendState = &color_blend;
  pipeline_info.pDynamicState = &dynamic_state;
  pipeline_info.layout = pipeline_layout_;
  pipeline_info.renderPass = VK_NULL_HANDLE;
  pipeline_info.subpass = 0;

  VkPipeline pipeline = VK_NULL_HANDLE;
  const bool trace_pipeline = transition::IsEnabled() && transition::ActiveTransitionId();
  const uint64_t pipeline_start = trace_pipeline ? rex::chrono::Clock::QueryHostTickCount() : 0;
  if (trace_pipeline) {
    transition::Record(transition::EventSource::kRenderer,
                       transition::EventType::kPipelineMissBegin, 0, 0,
                       diagnostic_submitted_frame_, transition::kFlagBefore,
                       diagnostic_draw_id_, key.vertex_shader_hash, key.pixel_shader_hash);
  }
  const VkResult pipeline_result = vulkan_device->functions().vkCreateGraphicsPipelines(
      vulkan_device->device(), native_pipeline_cache_, 1, &pipeline_info, nullptr, &pipeline);
  if (trace_pipeline) {
    transition::Record(
        transition::EventSource::kRenderer, transition::EventType::kPipelineMissEnd, 0, 0,
        diagnostic_submitted_frame_,
        pipeline_result == VK_SUCCESS ? transition::kFlagAfter : transition::kFlagError,
        diagnostic_draw_id_, rex::chrono::Clock::QueryHostTickCount() - pipeline_start,
        static_cast<uint64_t>(static_cast<int64_t>(pipeline_result)));
  }
  if (pipeline_result != VK_SUCCESS) {
    return reject("vk-create-pipeline");
  }

  if (REXCVAR_GET(gta4_trace_startup_content) && native_pipelines_.size() < 32) {
    REXLOG_INFO(
        "gta4-native-diag: pipeline-create index={} handle={} primitive={} topology={} driver={} "
        "restart={}",
        native_pipelines_.size(), fmt::ptr(pipeline), primitive_type, uint32_t(topology),
        uint32_t(vulkan_device->properties().driverID),
        input_assembly.primitiveRestartEnable == VK_TRUE);
  }

  native_pipelines_.push_back({key, pipeline});
  native_pipeline_cache_dirty_ = true;
  return pipeline;
}

bool Gta4NativeGraphicsSystem::UploadBufferResource(
    const std::shared_ptr<const NativeBufferResource>& resource, bool index_buffer, bool index32,
    const NativePipelineState* vertex_state, uint32_t vertex_stream,
    NativeFrameResources& resources, NativeUploadAllocation& allocation) {
  if (!resource || resource->payload.empty()) {
    return false;
  }

  if (index_buffer) {
    auto& uploads = index32 ? resources.index32_uploads : resources.index16_uploads;
    auto existing = uploads.find(resource.get());
    if (existing != uploads.end()) {
      allocation = existing->second;
      return true;
    }
    if (!AllocateUpload(resource->payload.size(), 16, allocation)) {
      return false;
    }
    CopyGuestIndicesToHost(allocation.mapping, resource->payload.data(), resource->payload.size(),
                           index32);
    uploads.emplace(resource.get(), allocation);
    return true;
  }

  if (!vertex_state || !vertex_state->vertex_declaration_resource ||
      !vertex_state->vertex_shader_resource || vertex_stream >= kVertexStreamCount) {
    return false;
  }
  const NativeVertexDeclaration& declaration = *vertex_state->vertex_declaration_resource;
  const NativeShader& shader = *vertex_state->vertex_shader_resource;
  const NativePipelineState::VertexStream& stream_state =
      vertex_state->vertex_streams[vertex_stream];
  auto& uploads = resources.vertex_uploads[resource.get()];
  for (const NativeVertexUpload& upload : uploads) {
    if (upload.declaration_hash == declaration.content_hash &&
        upload.shader_hash == shader.hash && upload.stream == vertex_stream &&
        upload.stream_offset == stream_state.offset && upload.stride == stream_state.stride) {
      allocation = upload.allocation;
      return true;
    }
  }

  if (!stream_state.stride || stream_state.offset >= resource->payload.size() ||
      !AllocateUpload(resource->payload.size(), 16, allocation)) {
    return false;
  }
  const VertexPayloadConversionCounts conversions = ConvertGuestVertexPayload(
      allocation.mapping, resource->payload.data(), resource->payload.size(), declaration, shader,
      vertex_stream, stream_state.offset, stream_state.stride);

  if (conversions.components_16 || conversions.dec3n || conversions.color_uint) {
    static std::atomic<uint64_t> conversion_count{0};
    const uint64_t count = ++conversion_count;
    if (count <= 64) {
      REXLOG_INFO(
          "gta4-native-vertex-convert: #{} resource={:08X} declaration={:08X} "
          "shader={:016X} stream={} offset={} stride={} components16={} dec3n={} "
          "color-uint={}",
          count, resource->handle, declaration.handle, shader.hash, vertex_stream,
          stream_state.offset, stream_state.stride, conversions.components_16, conversions.dec3n,
          conversions.color_uint);
    }
  }
  uploads.push_back({declaration.content_hash, shader.hash, vertex_stream, stream_state.offset,
                     stream_state.stride, allocation});
  return true;
}

bool Gta4NativeGraphicsSystem::BindCommonDrawState(VkCommandBuffer command_buffer,
                                                   const NativeCommand& command,
                                                   VkPipeline pipeline, uint32_t width,
                                                   uint32_t height, uint32_t logical_width,
                                                   uint32_t logical_height) {
  if (!pipeline || command.device_snapshot.size() != kGuestDeviceSize) {
    return false;
  }

  const NativeFixedFunctionState& fixed = command.fixed_function_state;
  const float requested_viewport_x = std::bit_cast<float>(fixed.viewport_bits[0]);
  const float requested_viewport_y = std::bit_cast<float>(fixed.viewport_bits[1]);
  const float requested_viewport_width = std::bit_cast<float>(fixed.viewport_bits[2]);
  const float requested_viewport_height = std::bit_cast<float>(fixed.viewport_bits[3]);
  const float requested_min_depth = std::bit_cast<float>(fixed.viewport_bits[4]);
  const float requested_max_depth = std::bit_cast<float>(fixed.viewport_bits[5]);
  NativeUploadAllocation vertex_constants_allocation;
  NativeUploadAllocation pixel_constants_allocation;
  NativeUploadAllocation shared_constants_allocation;
  if (!AllocateUpload(kVertexConstantsSize, 16, vertex_constants_allocation) ||
      !AllocateUpload(kPixelConstantsSize, 16, pixel_constants_allocation) ||
      !AllocateUpload(sizeof(NativeSharedConstants), 16, shared_constants_allocation)) {
    return false;
  }
  CopyGuestWordsToHost(vertex_constants_allocation.mapping,
                       command.device_snapshot.data() + kVertexConstantsOffset,
                       kVertexConstantsSize);
  CopyGuestWordsToHost(pixel_constants_allocation.mapping,
                       command.device_snapshot.data() + kPixelConstantsOffset, kPixelConstantsSize);
  NativeSharedConstants shared_constants{};
  for (uint32_t stage = 0; stage < kShaderTextureCount; ++stage) {
    shared_constants.texture_2d_indices[stage] = command.texture_descriptor_indices[stage];
    shared_constants.texture_2d_array_indices[stage] = command.texture_descriptor_indices[stage];
    shared_constants.texture_3d_indices[stage] = command.texture_descriptor_indices[stage];
    shared_constants.texture_cube_indices[stage] = command.texture_descriptor_indices[stage];
    shared_constants.sampler_indices[stage] = command.sampler_descriptor_indices[stage];
  }
  shared_constants.alpha_threshold = command.fixed_function_state.alpha_reference;
  const uint32_t vertex_booleans = LoadGuestWord(command.device_snapshot, 0x2780);
  const uint32_t pixel_booleans = LoadGuestWord(command.device_snapshot, 0x2790);
  shared_constants.booleans = (vertex_booleans & 0xFF) | ((pixel_booleans & 0xFF) << 16);
  shared_constants.half_pixel_offset_x = 1.0f / static_cast<float>(width);
  shared_constants.half_pixel_offset_y = -1.0f / static_cast<float>(height);
  shared_constants.motion_blur_time_scale = MotionBlurTimeScale(command.environmental_data.get());
  if (command.environmental_data) {
    const EnvironmentalDataV1& environment = *command.environmental_data;
    shared_constants.environmental_valid_fields = environment.valid_fields;
    shared_constants.fog_parameters[0] = environment.fog_density;
    shared_constants.fog_parameters[1] = environment.fog_height_falloff;
    shared_constants.fog_parameters[2] = environment.fog_altitude_tweak;
    shared_constants.fog_parameters[3] = environment.fog_power;
    std::copy(environment.camera_position.begin(), environment.camera_position.end(),
              shared_constants.camera_position);
    std::copy(environment.view_inverse_matrix.begin(), environment.view_inverse_matrix.end(),
              shared_constants.view_inverse_matrix);
    shared_constants.projection_scale[0] = environment.projection_matrix[0];
    shared_constants.projection_scale[1] = environment.projection_matrix[5];
  }
  std::memcpy(shared_constants_allocation.mapping, &shared_constants, sizeof(shared_constants));

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto& dfn = vulkan_provider->vulkan_device()->functions();
  const std::array<VkDescriptorSet, kDescriptorSetCount> draw_descriptor_sets = {
      command.draw_descriptor_sets[0], command.draw_descriptor_sets[1],
      command.draw_descriptor_sets[2], command.draw_descriptor_sets[3],
      command.draw_descriptor_sets[4], descriptor_sets_[5]};
  for (VkDescriptorSet descriptor_set : draw_descriptor_sets) {
    if (!descriptor_set) {
      return false;
    }
  }
  dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  dfn.vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout_, 0,
                              kDescriptorSetCount, draw_descriptor_sets.data(), 0, nullptr);
  VkViewport viewport{};
  const float viewport_scale_x = logical_width ? float(width) / float(logical_width) : 1.0f;
  const float viewport_scale_y = logical_height ? float(height) / float(logical_height) : 1.0f;
  if (std::isfinite(requested_viewport_width) && requested_viewport_width > 0.0f &&
      std::isfinite(requested_viewport_height) && requested_viewport_height > 0.0f) {
    viewport.x =
        std::clamp(requested_viewport_x * viewport_scale_x, 0.0f, static_cast<float>(width));
    viewport.y =
        std::clamp(requested_viewport_y * viewport_scale_y, 0.0f, static_cast<float>(height));
    viewport.width = std::min(requested_viewport_width * viewport_scale_x,
                              static_cast<float>(width) - viewport.x);
    viewport.height = std::min(requested_viewport_height * viewport_scale_y,
                               static_cast<float>(height) - viewport.y);
    viewport.minDepth = std::clamp(requested_min_depth, 0.0f, 1.0f);
    viewport.maxDepth = std::clamp(requested_max_depth, 0.0f, 1.0f);
  } else {
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.maxDepth = 1.0f;
  }
  dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
  VkRect2D scissor{};
  if (fixed.scissor_enable) {
    const int32_t logical_left =
        std::clamp(fixed.scissor[0], int32_t(0), int32_t(logical_width));
    const int32_t logical_top =
        std::clamp(fixed.scissor[1], int32_t(0), int32_t(logical_height));
    const int32_t logical_right =
        std::clamp(fixed.scissor[2], logical_left, int32_t(logical_width));
    const int32_t logical_bottom =
        std::clamp(fixed.scissor[3], logical_top, int32_t(logical_height));
    const int32_t left = ScaleCoordinateFloor(logical_left, logical_width, width);
    const int32_t top = ScaleCoordinateFloor(logical_top, logical_height, height);
    const int32_t right = ScaleCoordinateCeil(logical_right, logical_width, width);
    const int32_t bottom = ScaleCoordinateCeil(logical_bottom, logical_height, height);
    scissor.offset = {left, top};
    scissor.extent = {uint32_t(right - left), uint32_t(bottom - top)};
  } else {
    scissor.extent.width = width;
    scissor.extent.height = height;
  }
  dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
  if (deterministic_trace_active_ && command.pipeline_state) {
    const SurfaceDescriptor& color_target = command.pipeline_state->render_targets[0];
    const auto reflection = reflection_resources_.find(color_target.handle);
    if (reflection != reflection_resources_.end()) {
      const NativeShader* vertex_shader = command.pipeline_state->vertex_shader_resource;
      const NativeShader* pixel_shader = command.pipeline_state->pixel_shader_resource;
      TraceNativeRendererEvent(
          "reflection-draw-state",
          fmt::format(
              "target={:08X}/{:08X} family={} role={} logical={}x{} physical={}x{} "
              "requested-viewport={:.9g},{:.9g},{:.9g},{:.9g},{:.9g},{:.9g} "
              "effective-viewport={:.9g},{:.9g},{:.9g},{:.9g},{:.9g},{:.9g} "
              "requested-scissor={}:{},{},{},{} effective-scissor={},{},{}x{} "
              "vs={:016X}:{} ps={:016X}:{} constants={:016X}/{:016X}",
              color_target.handle, color_target.address, uint32_t(reflection->second.family),
              uint32_t(reflection->second.role), logical_width, logical_height, width, height,
              requested_viewport_x, requested_viewport_y, requested_viewport_width,
              requested_viewport_height, requested_min_depth, requested_max_depth, viewport.x,
              viewport.y, viewport.width, viewport.height, viewport.minDepth, viewport.maxDepth,
              fixed.scissor_enable, fixed.scissor[0], fixed.scissor[1], fixed.scissor[2],
              fixed.scissor[3], scissor.offset.x, scissor.offset.y, scissor.extent.width,
              scissor.extent.height, vertex_shader ? vertex_shader->hash : 0,
              vertex_shader ? vertex_shader->filename : std::string{},
              pixel_shader ? pixel_shader->hash : 0,
              pixel_shader ? pixel_shader->filename : std::string{},
              command.vertex_constants_hash, command.pixel_constants_hash));
    }
  }
  const float depth_bias_constant = std::bit_cast<float>(fixed.depth_bias_bits) *
                                    draw_util::kD3D10PolygonOffsetFactorFloat24;
  const float depth_bias_slope = std::bit_cast<float>(fixed.slope_scaled_depth_bias_bits) *
                                 xenos::kPolygonOffsetScaleSubpixelUnit;
  dfn.vkCmdSetDepthBias(command_buffer, depth_bias_constant, 0.0f, depth_bias_slope);
  uint32_t stencil_reference_back = fixed.stencil_reference;
  uint32_t stencil_mask_back = fixed.stencil_mask;
  uint32_t stencil_write_mask_back = fixed.stencil_write_mask;
  const bool separate_stencil = fixed.two_sided_stencil &&
                                vulkan_provider->vulkan_device()
                                    ->properties()
                                    .separateStencilMaskRef;
  if (separate_stencil) {
    stencil_reference_back = fixed.back_stencil_reference;
    stencil_mask_back = fixed.back_stencil_mask;
    stencil_write_mask_back = fixed.back_stencil_write_mask;
  } else if (fixed.two_sided_stencil && (fixed.cull_mode == 1 || fixed.cull_mode == 5)) {
    stencil_reference_back = fixed.back_stencil_reference;
    stencil_mask_back = fixed.back_stencil_mask;
    stencil_write_mask_back = fixed.back_stencil_write_mask;
    dfn.vkCmdSetStencilReference(command_buffer, VK_STENCIL_FACE_FRONT_AND_BACK,
                                 stencil_reference_back);
    dfn.vkCmdSetStencilCompareMask(command_buffer, VK_STENCIL_FACE_FRONT_AND_BACK,
                                   stencil_mask_back);
    dfn.vkCmdSetStencilWriteMask(command_buffer, VK_STENCIL_FACE_FRONT_AND_BACK,
                                 stencil_write_mask_back);
  } else {
    dfn.vkCmdSetStencilReference(command_buffer, VK_STENCIL_FACE_FRONT_AND_BACK,
                                 fixed.stencil_reference);
    dfn.vkCmdSetStencilCompareMask(command_buffer, VK_STENCIL_FACE_FRONT_AND_BACK,
                                   fixed.stencil_mask);
    dfn.vkCmdSetStencilWriteMask(command_buffer, VK_STENCIL_FACE_FRONT_AND_BACK,
                                 fixed.stencil_write_mask);
  }
  if (separate_stencil) {
    dfn.vkCmdSetStencilReference(command_buffer, VK_STENCIL_FACE_FRONT_BIT,
                                 fixed.stencil_reference);
    dfn.vkCmdSetStencilReference(command_buffer, VK_STENCIL_FACE_BACK_BIT,
                                 stencil_reference_back);
    dfn.vkCmdSetStencilCompareMask(command_buffer, VK_STENCIL_FACE_FRONT_BIT,
                                   fixed.stencil_mask);
    dfn.vkCmdSetStencilCompareMask(command_buffer, VK_STENCIL_FACE_BACK_BIT,
                                   stencil_mask_back);
    dfn.vkCmdSetStencilWriteMask(command_buffer, VK_STENCIL_FACE_FRONT_BIT,
                                 fixed.stencil_write_mask);
    dfn.vkCmdSetStencilWriteMask(command_buffer, VK_STENCIL_FACE_BACK_BIT,
                                 stencil_write_mask_back);
  }
  dfn.vkCmdSetBlendConstants(command_buffer, fixed.blend_constants.data());
  NativePushConstants push_constants{};
  push_constants.vertex_constants = vertex_constants_allocation.device_address;
  push_constants.pixel_constants = pixel_constants_allocation.device_address;
  push_constants.shared_constants = shared_constants_allocation.device_address;
  dfn.vkCmdPushConstants(command_buffer, pipeline_layout_,
                         VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                         sizeof(push_constants), &push_constants);
  return true;
}

bool Gta4NativeGraphicsSystem::RecordPrimitiveUp(VkCommandBuffer command_buffer,
                                                 const NativeCommand& command, uint32_t width,
                                                 uint32_t height,
                                                 const NativeRenderingTarget& target) {
  auto fail = [&command, &target](const char* reason) {
    static std::atomic<uint64_t> failure_count{0};
    const uint64_t count = ++failure_count;
    if (count <= 24 || !(count % 1024)) {
      REXLOG_WARN(
          "gta4-native-diag: draw-up failure #{} reason={} snapshot={} payload={} "
          "pipeline-state={} target={}x{} presenter={}",
          count, reason, command.device_snapshot.size(), command.payload.size(),
          command.pipeline_state ? "yes" : "no", target.width, target.height,
          target.uses_presenter);
    }
    return false;
  };

  if (!command.pipeline_state || command.payload.empty() ||
      command.device_snapshot.size() != kGuestDeviceSize) {
    return fail("command-state-or-payload");
  }
  DrawPrimitiveUpCommand draw{};
  std::memcpy(&draw, command.bytes.data(), sizeof(draw));
  if (!draw.vertex_count || !draw.stride) {
    return fail("zero-count-or-stride");
  }

  VkPipeline pipeline = GetOrCreatePipeline(*command.pipeline_state, command.fixed_function_state,
                                            draw.primitive_type, target, draw.stride);
  NativeUploadAllocation vertex_allocation;
  if (!pipeline) {
    return fail("pipeline");
  }
  if (!AllocateUpload(command.payload.size(), 16, vertex_allocation)) {
    return fail("vertex-upload-allocation");
  }
  if (!command.pipeline_state->vertex_declaration_resource ||
      !command.pipeline_state->vertex_shader_resource) {
    return fail("vertex-declaration-or-shader");
  }
  const NativeVertexDeclaration& up_declaration =
      *command.pipeline_state->vertex_declaration_resource;
  const NativeShader& up_shader = *command.pipeline_state->vertex_shader_resource;
  const VertexPayloadConversionCounts up_conversions = ConvertGuestVertexPayload(
      vertex_allocation.mapping, command.payload.data(), command.payload.size(), up_declaration,
      up_shader, 0, 0, draw.stride);
  if (up_conversions.components_16 || up_conversions.dec3n || up_conversions.color_uint) {
    static std::atomic<uint64_t> up_conversion_count{0};
    const uint64_t count = ++up_conversion_count;
    if (count <= 64) {
      REXLOG_INFO(
          "gta4-native-up-vertex-convert: #{} declaration={:08X} shader={:016X} "
          "stride={} components16={} dec3n={} color-uint={}",
          count, up_declaration.handle, up_shader.hash, draw.stride,
          up_conversions.components_16, up_conversions.dec3n, up_conversions.color_uint);
    }
  }

  uint32_t host_vertex_count = draw.vertex_count;
  NativeUploadAllocation quad_list_indices;
  uint32_t quad_list_index_count = 0;
  if (draw.primitive_type == uint32_t(xenos::PrimitiveType::kRectangleList)) {
    if (!command.pipeline_state->vertex_declaration_resource || draw.vertex_count % 3) {
      return fail("rectangle-list-layout");
    }
    const uint32_t rectangle_count = draw.vertex_count / 3;
    host_vertex_count = rectangle_count * 4;
    const uint64_t expanded_size = uint64_t(host_vertex_count) * uint64_t(draw.stride);
    if (!expanded_size || expanded_size > kMaximumUpPayloadSize) {
      return fail("rectangle-list-size");
    }

    NativeUploadAllocation expanded_allocation;
    if (!AllocateUpload(VkDeviceSize(expanded_size), 16, expanded_allocation)) {
      return fail("rectangle-list-upload-allocation");
    }
    const NativeVertexDeclaration& declaration =
        *command.pipeline_state->vertex_declaration_resource;
    const VertexElement* position_element = nullptr;
    for (const VertexElement& element : declaration.elements) {
      if (element.stream == 0 && ConvertVertexUsageToLocation(element.usage, element.usage_index) == 0 &&
          GetFloat32VertexElementComponentCount(element.type) >= 2 &&
          element.offset + sizeof(float) * 2 <= draw.stride) {
        position_element = &element;
        break;
      }
    }

    const uint8_t* input = vertex_allocation.mapping;
    uint8_t* output = expanded_allocation.mapping;
    for (uint32_t rectangle = 0; rectangle < rectangle_count; ++rectangle) {
      std::array<const uint8_t*, 3> vertices = {
          input + size_t(rectangle * 3) * draw.stride,
          input + size_t(rectangle * 3 + 1) * draw.stride,
          input + size_t(rectangle * 3 + 2) * draw.stride,
      };
      uint32_t first_corner = 0;
      if (position_element) {
        std::array<std::array<float, 2>, 3> positions{};
        for (uint32_t vertex = 0; vertex < 3; ++vertex) {
          std::memcpy(&positions[vertex][0], vertices[vertex] + position_element->offset,
                      sizeof(float));
          std::memcpy(&positions[vertex][1],
                      vertices[vertex] + position_element->offset + sizeof(float), sizeof(float));
        }
        float longest_opposite_edge = -1.0f;
        for (uint32_t candidate = 0; candidate < 3; ++candidate) {
          const uint32_t edge_begin = (candidate + 1) % 3;
          const uint32_t edge_end = (candidate + 2) % 3;
          const float delta_x = positions[edge_end][0] - positions[edge_begin][0];
          const float delta_y = positions[edge_end][1] - positions[edge_begin][1];
          const float edge_length = delta_x * delta_x + delta_y * delta_y;
          if (edge_length > longest_opposite_edge) {
            longest_opposite_edge = edge_length;
            first_corner = candidate;
          }
        }
      }

      const uint8_t* corner_0 = vertices[first_corner];
      const uint8_t* corner_1 = vertices[(first_corner + 1) % 3];
      const uint8_t* corner_2 = vertices[(first_corner + 2) % 3];
      uint8_t* rectangle_output = output + size_t(rectangle * 4) * draw.stride;
      std::memcpy(rectangle_output, corner_0, draw.stride);
      std::memcpy(rectangle_output + draw.stride, corner_1, draw.stride);
      std::memcpy(rectangle_output + size_t(draw.stride) * 2, corner_2, draw.stride);
      uint8_t* synthetic = rectangle_output + size_t(draw.stride) * 3;
      std::memcpy(synthetic, corner_2, draw.stride);

      for (const VertexElement& element : declaration.elements) {
        if (element.stream != 0) {
          continue;
        }
        const uint32_t component_count = GetFloat32VertexElementComponentCount(element.type);
        if (!component_count || element.offset + component_count * sizeof(float) > draw.stride) {
          continue;
        }
        for (uint32_t component = 0; component < component_count; ++component) {
          const size_t component_offset = element.offset + size_t(component) * sizeof(float);
          float value_0;
          float value_1;
          float value_2;
          std::memcpy(&value_0, corner_0 + component_offset, sizeof(value_0));
          std::memcpy(&value_1, corner_1 + component_offset, sizeof(value_1));
          std::memcpy(&value_2, corner_2 + component_offset, sizeof(value_2));
          const float synthetic_value = value_1 - value_0 + value_2;
          std::memcpy(synthetic + component_offset, &synthetic_value, sizeof(synthetic_value));
        }
      }
    }
    vertex_allocation = expanded_allocation;
  } else if (draw.primitive_type == uint32_t(xenos::PrimitiveType::kQuadList)) {
    if (draw.vertex_count % 4) {
      return fail("quad-list-count");
    }
    quad_list_index_count = GetQuadListTriangleListIndexCount(draw.vertex_count);
    const VkDeviceSize index_size =
        VkDeviceSize(quad_list_index_count) * sizeof(uint32_t);
    if (!quad_list_index_count ||
        !AllocateUpload(index_size, alignof(uint32_t), quad_list_indices)) {
      return fail("quad-list-upload-allocation");
    }
    uint32_t* indices = reinterpret_cast<uint32_t*>(quad_list_indices.mapping);
    for (uint32_t quad = 0; quad < draw.vertex_count / 4; ++quad) {
      const uint32_t vertex = quad * 4;
      *(indices++) = vertex;
      *(indices++) = vertex + 1;
      *(indices++) = vertex + 2;
      *(indices++) = vertex;
      *(indices++) = vertex + 2;
      *(indices++) = vertex + 3;
    }
  }
  if (!BindCommonDrawState(command_buffer, command, pipeline, width, height,
                           target.logical_width, target.logical_height)) {
    return fail("common-state");
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto& dfn = vulkan_provider->vulkan_device()->functions();
  const VkBuffer default_vertex_buffer = upload_buffer_.buffer;
  const VkDeviceSize default_vertex_offset = 0;
  dfn.vkCmdBindVertexBuffers(command_buffer, kDefaultVertexBinding, 1,
                             &default_vertex_buffer, &default_vertex_offset);
  const VkBuffer vertex_buffer = upload_buffer_.buffer;
  const VkDeviceSize vertex_offset = vertex_allocation.offset;
  dfn.vkCmdBindVertexBuffers(command_buffer, 0, 1, &vertex_buffer, &vertex_offset);
  if (draw.primitive_type == uint32_t(xenos::PrimitiveType::kRectangleList)) {
    const uint32_t rectangle_count = host_vertex_count / 4;
    for (uint32_t rectangle = 0; rectangle < rectangle_count; ++rectangle) {
      dfn.vkCmdDraw(command_buffer, 4, 1, rectangle * 4, 0);
    }
  } else if (draw.primitive_type == uint32_t(xenos::PrimitiveType::kQuadList)) {
    dfn.vkCmdBindIndexBuffer(command_buffer, upload_buffer_.buffer, quad_list_indices.offset,
                             VK_INDEX_TYPE_UINT32);
    dfn.vkCmdDrawIndexed(command_buffer, quad_list_index_count, 1, 0, 0, 0);
  } else {
    dfn.vkCmdDraw(command_buffer, host_vertex_count, 1, 0, 0);
  }
  return true;
}

bool Gta4NativeGraphicsSystem::GetRequiredVertexStreams(
    const NativePipelineState& state,
    std::array<bool, kVertexStreamCount>& required_streams) const {
  required_streams.fill(false);
  if (!state.vertex_shader_resource || !state.vertex_declaration_resource) {
    return false;
  }

  for (const NativeVertexInput& shader_input : state.vertex_shader_resource->vertex_inputs) {
    const VertexElement* matching_element = nullptr;
    for (const VertexElement& element : state.vertex_declaration_resource->elements) {
      if (ConvertVertexUsageToLocation(element.usage, element.usage_index) ==
          shader_input.location) {
        matching_element = &element;
        break;
      }
    }
    if (!matching_element) {
      continue;
    }
    if (matching_element->stream >= kVertexStreamCount) {
      return false;
    }
    required_streams[matching_element->stream] = true;
  }
  return true;
}

bool Gta4NativeGraphicsSystem::RecordPrimitive(VkCommandBuffer command_buffer,
                                               const NativeCommand& command, uint32_t width,
                                               uint32_t height,
                                               const NativeRenderingTarget& target,
                                               NativeFrameResources& resources) {
  auto fail = [&command, &target](const char* reason) {
    static std::atomic<uint64_t> failure_count{0};
    const uint64_t count = ++failure_count;
    if (count <= 24 || !(count % 1024)) {
      REXLOG_WARN(
          "gta4-native-diag: draw failure #{} reason={} snapshot={} pipeline-state={} "
          "target={}x{} presenter={}",
          count, reason, command.device_snapshot.size(), command.pipeline_state ? "yes" : "no",
          target.width, target.height, target.uses_presenter);
    }
    return false;
  };

  if (!command.pipeline_state || command.device_snapshot.size() != kGuestDeviceSize) {
    return fail("command-state");
  }
  DrawPrimitiveCommand draw{};
  std::memcpy(&draw, command.bytes.data(), sizeof(draw));
  if (!draw.vertex_count) {
    return fail("zero-count");
  }

  VkPipeline pipeline = GetOrCreatePipeline(*command.pipeline_state, command.fixed_function_state,
                                            draw.primitive_type, target);
  if (!pipeline) {
    return fail("pipeline");
  }
  if (!BindCommonDrawState(command_buffer, command, pipeline, width, height,
                           target.logical_width, target.logical_height)) {
    return fail("common-state");
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto& dfn = vulkan_provider->vulkan_device()->functions();
  const VkBuffer upload_buffer = upload_buffer_.buffer;
  const VkDeviceSize default_vertex_offset = 0;
  dfn.vkCmdBindVertexBuffers(command_buffer, kDefaultVertexBinding, 1, &upload_buffer,
                             &default_vertex_offset);
  std::array<bool, kVertexStreamCount> required_streams{};
  if (!GetRequiredVertexStreams(*command.pipeline_state, required_streams)) {
    return fail("vertex-input-layout");
  }
  for (uint32_t stream = 0; stream < kVertexStreamCount; ++stream) {
    if (!required_streams[stream]) {
      continue;
    }
    const auto& stream_state = command.pipeline_state->vertex_streams[stream];
    const auto& resource = command.vertex_buffers[stream];
    if (!stream_state.stride || !resource || stream_state.offset >= resource->payload.size()) {
      static std::atomic<uint64_t> stream_rejection_count{0};
      const uint64_t count = ++stream_rejection_count;
      if (count <= 64 || !(count % 1024)) {
        REXLOG_WARN(
            "gta4-native-diag: vertex stream reject #{} indexed=false stream={} "
            "state-handle={:08X} snapshot-handle={:08X} stride={} offset={} "
            "resource={} resource-size={}",
            count, stream, stream_state.buffer,
            LoadGuestWord(command.device_snapshot,
                          kStreamBufferBase + stream * sizeof(uint32_t)),
            stream_state.stride, stream_state.offset, resource ? "yes" : "no",
            resource ? resource->payload.size() : 0);
      }
      return fail("vertex-stream-resource");
    }
    const uint64_t vertex_end = uint64_t(draw.start_vertex) + uint64_t(draw.vertex_count);
    const uint64_t required_size =
        uint64_t(stream_state.offset) + vertex_end * uint64_t(stream_state.stride);
    if (required_size > resource->payload.size()) {
      return fail("vertex-stream-bounds");
    }
    NativeUploadAllocation stream_allocation;
    if (!UploadBufferResource(resource, false, false, command.pipeline_state.get(), stream,
                              resources, stream_allocation)) {
      return fail("vertex-stream-upload");
    }
    const VkDeviceSize stream_offset = stream_allocation.offset + stream_state.offset;
    dfn.vkCmdBindVertexBuffers(command_buffer, stream, 1, &upload_buffer, &stream_offset);
  }

  if (draw.primitive_type == uint32_t(xenos::PrimitiveType::kQuadList)) {
    if (draw.vertex_count % 4) {
      return fail("quad-list-count");
    }
    const uint32_t index_count = GetQuadListTriangleListIndexCount(draw.vertex_count);
    NativeUploadAllocation indices_allocation;
    const VkDeviceSize index_size = VkDeviceSize(index_count) * sizeof(uint32_t);
    if (!index_count ||
        !AllocateUpload(index_size, alignof(uint32_t), indices_allocation)) {
      return fail("quad-list-upload-allocation");
    }
    uint32_t* indices = reinterpret_cast<uint32_t*>(indices_allocation.mapping);
    for (uint32_t quad = 0; quad < draw.vertex_count / 4; ++quad) {
      const uint32_t vertex = quad * 4;
      *(indices++) = vertex;
      *(indices++) = vertex + 1;
      *(indices++) = vertex + 2;
      *(indices++) = vertex;
      *(indices++) = vertex + 2;
      *(indices++) = vertex + 3;
    }
    dfn.vkCmdBindIndexBuffer(command_buffer, upload_buffer, indices_allocation.offset,
                             VK_INDEX_TYPE_UINT32);
    dfn.vkCmdDrawIndexed(command_buffer, index_count, 1, 0,
                         int32_t(draw.start_vertex), 0);
  } else {
    dfn.vkCmdDraw(command_buffer, draw.vertex_count, 1, draw.start_vertex, 0);
  }
  return true;
}

bool Gta4NativeGraphicsSystem::RecordIndexedPrimitive(VkCommandBuffer command_buffer,
                                                      const NativeCommand& command, uint32_t width,
                                                      uint32_t height,
                                                      const NativeRenderingTarget& target,
                                                      NativeFrameResources& resources) {
  auto fail = [&command, &target](const char* reason) {
    static std::atomic<uint64_t> failure_count{0};
    const uint64_t count = ++failure_count;
    if (count <= 24 || !(count % 1024)) {
      REXLOG_WARN(
          "gta4-native-diag: draw-indexed failure #{} reason={} snapshot={} index={} "
          "pipeline-state={} target={}x{} presenter={}",
          count, reason, command.device_snapshot.size(), command.index_buffer ? "yes" : "no",
          command.pipeline_state ? "yes" : "no", target.width, target.height,
          target.uses_presenter);
    }
    return false;
  };

  if (!command.pipeline_state || !command.index_buffer ||
      command.device_snapshot.size() != kGuestDeviceSize) {
    return fail("command-state-or-index");
  }
  DrawIndexedPrimitiveCommand draw{};
  std::memcpy(&draw, command.bytes.data(), sizeof(draw));
  if (!draw.index_count) {
    return fail("zero-index-count");
  }

  const bool index32 = (command.index_buffer->flags & kIndex32Flag) != 0;
  const VkPrimitiveTopology topology = ConvertPrimitiveTopology(draw.primitive_type);
  const bool strip = topology == VK_PRIMITIVE_TOPOLOGY_LINE_STRIP ||
                     topology == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP ||
                     topology == VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
  const bool guest_restart_enabled =
      draw.primitive_restart_enabled && strip &&
      (index32 || draw.primitive_restart_index <= UINT16_MAX);

  VkPipeline pipeline = GetOrCreatePipeline(*command.pipeline_state, command.fixed_function_state,
                                            draw.primitive_type, target, 0,
                                            guest_restart_enabled);
  if (!pipeline) {
    return fail("pipeline");
  }
  if (!BindCommonDrawState(command_buffer, command, pipeline, width, height,
                           target.logical_width, target.logical_height)) {
    return fail("common-state");
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto& dfn = vulkan_provider->vulkan_device()->functions();
  const VkBuffer upload_buffer = upload_buffer_.buffer;
  const VkDeviceSize default_vertex_offset = 0;
  dfn.vkCmdBindVertexBuffers(command_buffer, kDefaultVertexBinding, 1, &upload_buffer,
                             &default_vertex_offset);
  std::array<bool, kVertexStreamCount> required_streams{};
  if (!GetRequiredVertexStreams(*command.pipeline_state, required_streams)) {
    return fail("vertex-input-layout");
  }
  for (uint32_t stream = 0; stream < kVertexStreamCount; ++stream) {
    if (!required_streams[stream]) {
      continue;
    }
    const auto& stream_state = command.pipeline_state->vertex_streams[stream];
    const auto& resource = command.vertex_buffers[stream];
    if (!stream_state.stride || !resource || stream_state.offset >= resource->payload.size()) {
      static std::atomic<uint64_t> stream_rejection_count{0};
      const uint64_t count = ++stream_rejection_count;
      if (count <= 64 || !(count % 1024)) {
        REXLOG_WARN(
            "gta4-native-diag: vertex stream reject #{} indexed=true stream={} "
            "state-handle={:08X} snapshot-handle={:08X} stride={} offset={} "
            "resource={} resource-size={}",
            count, stream, stream_state.buffer,
            LoadGuestWord(command.device_snapshot,
                          kStreamBufferBase + stream * sizeof(uint32_t)),
            stream_state.stride, stream_state.offset, resource ? "yes" : "no",
            resource ? resource->payload.size() : 0);
      }
      return fail("vertex-stream-resource");
    }
    NativeUploadAllocation stream_allocation;
    if (!UploadBufferResource(resource, false, false, command.pipeline_state.get(), stream,
                              resources, stream_allocation)) {
      return fail("vertex-stream-upload");
    }
    const VkDeviceSize stream_offset = stream_allocation.offset + stream_state.offset;
    dfn.vkCmdBindVertexBuffers(command_buffer, stream, 1, &upload_buffer, &stream_offset);
  }

  const uint64_t element_size = index32 ? sizeof(uint32_t) : sizeof(uint16_t);
  const uint64_t index_end = uint64_t(draw.start_index) + uint64_t(draw.index_count);
  const uint64_t required_size = index_end * element_size;
  if (required_size > command.index_buffer->payload.size()) {
    return fail("index-bounds");
  }
  NativeUploadAllocation index_allocation;
  if (!UploadBufferResource(command.index_buffer, true, index32, nullptr, 0, resources,
                            index_allocation)) {
    return fail("index-upload");
  }

  NativeUploadAllocation quad_list_indices;
  NativeUploadAllocation primitive_restart_indices;
  const uint8_t* selected_index_bytes =
      index_allocation.mapping + size_t(draw.start_index) * size_t(element_size);
  uint32_t host_index_count = draw.index_count;
  uint32_t host_start_index = draw.start_index;
  VkDeviceSize host_index_offset = index_allocation.offset;
  if (draw.primitive_type == uint32_t(xenos::PrimitiveType::kQuadList)) {
    if (draw.index_count % 4) {
      return fail("quad-list-count");
    }
    host_index_count = GetQuadListTriangleListIndexCount(draw.index_count);
    const VkDeviceSize expanded_size = VkDeviceSize(host_index_count) * element_size;
    if (!host_index_count ||
        !AllocateUpload(expanded_size, size_t(element_size), quad_list_indices)) {
      return fail("quad-list-upload-allocation");
    }
    if (index32) {
      const uint32_t* source = reinterpret_cast<const uint32_t*>(selected_index_bytes);
      uint32_t* destination = reinterpret_cast<uint32_t*>(quad_list_indices.mapping);
      for (uint32_t quad = 0; quad < draw.index_count / 4; ++quad) {
        const uint32_t* vertices = source + quad * 4;
        *(destination++) = vertices[0];
        *(destination++) = vertices[1];
        *(destination++) = vertices[2];
        *(destination++) = vertices[0];
        *(destination++) = vertices[2];
        *(destination++) = vertices[3];
      }
    } else {
      const uint16_t* source = reinterpret_cast<const uint16_t*>(selected_index_bytes);
      uint16_t* destination = reinterpret_cast<uint16_t*>(quad_list_indices.mapping);
      for (uint32_t quad = 0; quad < draw.index_count / 4; ++quad) {
        const uint16_t* vertices = source + quad * 4;
        *(destination++) = vertices[0];
        *(destination++) = vertices[1];
        *(destination++) = vertices[2];
        *(destination++) = vertices[0];
        *(destination++) = vertices[2];
        *(destination++) = vertices[3];
      }
    }
    selected_index_bytes = quad_list_indices.mapping;
    host_start_index = 0;
    host_index_offset = quad_list_indices.offset;
  }
  if (guest_restart_enabled) {
    const VkDeviceSize restart_size = VkDeviceSize(host_index_count) * element_size;
    if (!AllocateUpload(restart_size, size_t(element_size), primitive_restart_indices)) {
      return fail("primitive-restart-upload-allocation");
    }
    if (index32) {
      const uint32_t* source = reinterpret_cast<const uint32_t*>(selected_index_bytes);
      uint32_t* destination =
          reinterpret_cast<uint32_t*>(primitive_restart_indices.mapping);
      for (uint32_t index = 0; index < host_index_count; ++index) {
        const uint32_t value = source[index];
        destination[index] =
            (value & xenos::kVertexIndexMask) == draw.primitive_restart_index ? UINT32_MAX : value;
      }
    } else {
      const uint16_t* source = reinterpret_cast<const uint16_t*>(selected_index_bytes);
      uint16_t* destination =
          reinterpret_cast<uint16_t*>(primitive_restart_indices.mapping);
      const uint16_t guest_restart_index = uint16_t(draw.primitive_restart_index);
      for (uint32_t index = 0; index < host_index_count; ++index) {
        destination[index] = source[index] == guest_restart_index ? UINT16_MAX : source[index];
      }
    }
    selected_index_bytes = primitive_restart_indices.mapping;
    host_start_index = 0;
    host_index_offset = primitive_restart_indices.offset;
  }
  uint32_t minimum_index = UINT32_MAX;
  uint32_t maximum_index = 0;
  for (uint32_t index = 0; index < host_index_count; ++index) {
    const uint32_t value =
        index32 ? reinterpret_cast<const uint32_t*>(selected_index_bytes)[index]
                : reinterpret_cast<const uint16_t*>(selected_index_bytes)[index];
    if (guest_restart_enabled && value == (index32 ? UINT32_MAX : UINT16_MAX)) {
      continue;
    }
    minimum_index = std::min(minimum_index, value);
    maximum_index = std::max(maximum_index, value);
  }
  if (minimum_index == UINT32_MAX) {
    minimum_index = 0;
    maximum_index = 0;
  }

  bool vertex_range_invalid = false;
  std::string stream_summary;
  for (uint32_t stream = 0; stream < kVertexStreamCount; ++stream) {
    if (!required_streams[stream]) {
      continue;
    }
    const auto& stream_state = command.pipeline_state->vertex_streams[stream];
    const auto& resource = command.vertex_buffers[stream];
    const int64_t first_vertex = int64_t(minimum_index) + int64_t(draw.base_vertex);
    const int64_t last_vertex = int64_t(maximum_index) + int64_t(draw.base_vertex);
    const int64_t byte_begin =
        int64_t(stream_state.offset) + first_vertex * int64_t(stream_state.stride);
    const int64_t byte_end = int64_t(stream_state.offset) +
                             (last_vertex + 1) * int64_t(stream_state.stride);
    const bool range_valid = first_vertex >= 0 && last_vertex >= first_vertex &&
                             byte_begin >= 0 && byte_end >= byte_begin && resource &&
                             uint64_t(byte_end) <= uint64_t(resource->payload.size());
    vertex_range_invalid |= !range_valid;
    stream_summary += fmt::format(
        "{}s{}[handle={:08X} flags={:08X} address={:08X} size={} offset={} stride={} "
        "bytes={}..{} valid={}]",
        stream_summary.empty() ? "" : " ", stream, resource ? resource->handle : 0,
        resource ? resource->flags : 0, resource ? resource->guest_address : 0,
        resource ? resource->payload.size() : 0, stream_state.offset, stream_state.stride,
        byte_begin, byte_end, range_valid);
  }
  if (diagnostic_draw_id_ && transition::IsEnabled() && transition::ActiveTransitionId()) {
    transition::Record(
        transition::EventSource::kRenderer, transition::EventType::kVertexRange,
        0x82A3E348, draw.caller, diagnostic_submitted_frame_,
        vertex_range_invalid ? transition::kFlagError : transition::kFlagNone,
        diagnostic_draw_id_, (uint64_t(minimum_index) << 32) | maximum_index,
        static_cast<uint64_t>(static_cast<int64_t>(draw.base_vertex)));
  }

  const auto& declaration = command.pipeline_state->vertex_declaration_resource;
  const bool skinned_layout = declaration &&
                              (declaration->maximum_stream != 0 ||
                               std::any_of(declaration->elements.begin(),
                                           declaration->elements.end(),
                                           [](const VertexElement& element) {
                                             return element.usage == 1 || element.usage == 2;
                                           }));
  const uint64_t vertex_shader_hash = command.pipeline_state->vertex_shader_resource
                                          ? command.pipeline_state->vertex_shader_resource->hash
                                          : 0;
  bool log_unique_layout = false;
  if (skinned_layout) {
    const std::array<uint64_t, 2> key_data = {declaration->content_hash, vertex_shader_hash};
    const uint64_t key = XXH3_64bits(key_data.data(), sizeof(key_data));
    static std::mutex indexed_vertex_diagnostic_mutex;
    static std::unordered_set<uint64_t> logged_indexed_vertex_layouts;
    std::lock_guard lock(indexed_vertex_diagnostic_mutex);
    if (logged_indexed_vertex_layouts.size() < 256) {
      log_unique_layout = logged_indexed_vertex_layouts.insert(key).second;
    }
  }

  bool log_invalid_range = false;
  if (vertex_range_invalid) {
    static std::atomic<uint64_t> invalid_vertex_range_count{0};
    log_invalid_range = ++invalid_vertex_range_count <= 64;
  }
  if (log_unique_layout || log_invalid_range) {
    static std::atomic<uint64_t> indexed_vertex_diagnostic_count{0};
    const uint64_t count = ++indexed_vertex_diagnostic_count;
    std::string shader_inputs_summary;
    if (command.pipeline_state->vertex_shader_resource) {
      for (const NativeVertexInput& input :
           command.pipeline_state->vertex_shader_resource->vertex_inputs) {
        shader_inputs_summary += fmt::format(
            "{}l{}:{}/{}", shader_inputs_summary.empty() ? "" : ",", input.location,
            uint32_t(input.numeric_type), input.component_count);
      }
    }
    REXLOG_WARN(
        "gta4-native-indexed-vertices: #{} invalid={} declaration={:08X} vs={:016X} "
        "inputs=[{}] index-handle={:08X} index-flags={:08X} index32={} base={} start={} "
        "count={} range={}..{} {}",
        count, vertex_range_invalid, declaration ? declaration->handle : 0,
        vertex_shader_hash, shader_inputs_summary, command.index_buffer->handle,
        command.index_buffer->flags, index32, draw.base_vertex, draw.start_index,
        draw.index_count, minimum_index, maximum_index, stream_summary);
  }

  VkIndexType bound_index_type = index32 ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16;
  const bool moltenvk_strip_restart =
      vulkan_provider->vulkan_device()->properties().driverID == VK_DRIVER_ID_MOLTENVK && strip;
  if (moltenvk_strip_restart && !guest_restart_enabled) {
    const bool disabled_restart_sentinel_used =
        index32 ? primitive_restart::IsDisabledRestartSentinelUsed(
                      reinterpret_cast<const uint32_t*>(selected_index_bytes), host_index_count)
                : primitive_restart::IsDisabledRestartSentinelUsed(
                      reinterpret_cast<const uint16_t*>(selected_index_bytes), host_index_count);
    if (disabled_restart_sentinel_used) {
      NativeUploadAllocation sanitized_indices;
      const VkDeviceSize sanitized_size = VkDeviceSize(host_index_count) * sizeof(uint32_t);
      if (!AllocateUpload(sanitized_size, alignof(uint32_t), sanitized_indices)) {
        return fail("primitive-restart-sanitize-upload");
      }
      if (index32) {
        primitive_restart::ConvertDisabledRestartIndices(
            reinterpret_cast<uint32_t*>(sanitized_indices.mapping),
            reinterpret_cast<const uint32_t*>(selected_index_bytes), host_index_count,
            xenos::Endian::kNone);
      } else {
        primitive_restart::ConvertDisabledRestartIndices(
            reinterpret_cast<uint32_t*>(sanitized_indices.mapping),
            reinterpret_cast<const uint16_t*>(selected_index_bytes), host_index_count,
            xenos::Endian::kNone);
      }
      host_index_offset = sanitized_indices.offset;
      bound_index_type = VK_INDEX_TYPE_UINT32;
      host_start_index = 0;
    }
  }

  dfn.vkCmdBindIndexBuffer(command_buffer, upload_buffer, host_index_offset, bound_index_type);
  dfn.vkCmdDrawIndexed(command_buffer, host_index_count, 1, host_start_index, draw.base_vertex, 0);
  return true;
}

bool Gta4NativeGraphicsSystem::RecordClear(VkCommandBuffer command_buffer,
                                           const NativeCommand& command,
                                           const NativeRenderingTarget& target) {
  ClearCommand clear{};
  std::memcpy(&clear, command.bytes.data(), sizeof(clear));

  const int32_t logical_left =
      std::clamp(clear.left, int32_t(0), int32_t(target.logical_width));
  const int32_t logical_top =
      std::clamp(clear.top, int32_t(0), int32_t(target.logical_height));
  const int32_t logical_right =
      std::clamp(clear.right, logical_left, int32_t(target.logical_width));
  const int32_t logical_bottom =
      std::clamp(clear.bottom, logical_top, int32_t(target.logical_height));
  const int32_t left =
      ScaleCoordinateFloor(logical_left, target.logical_width, target.width);
  const int32_t top =
      ScaleCoordinateFloor(logical_top, target.logical_height, target.height);
  const int32_t right =
      ScaleCoordinateCeil(logical_right, target.logical_width, target.width);
  const int32_t bottom =
      ScaleCoordinateCeil(logical_bottom, target.logical_height, target.height);
  if (right <= left || bottom <= top) {
    return false;
  }

  std::array<VkClearAttachment, kRenderTargetCount + 1> attachments{};
  uint32_t attachment_count = 0;
  for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
    if (!(clear.flags & (uint32_t(1) << index)) || !target.color_views[index]) {
      continue;
    }
    VkClearAttachment& attachment = attachments[attachment_count++];
    attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    attachment.colorAttachment = index;
    for (uint32_t component = 0; component < 4; ++component) {
      attachment.clearValue.color.float32[component] =
          std::bit_cast<float>(clear.color_bits[component]);
    }
  }

  VkImageAspectFlags depth_stencil_aspects = 0;
  if ((clear.flags & 0x10) && target.depth_surface) {
    depth_stencil_aspects |= VK_IMAGE_ASPECT_DEPTH_BIT;
  }
  if ((clear.flags & 0x20) && target.depth_surface &&
      (target.depth_surface->aspect & VK_IMAGE_ASPECT_STENCIL_BIT)) {
    depth_stencil_aspects |= VK_IMAGE_ASPECT_STENCIL_BIT;
  }
  if (depth_stencil_aspects) {
    VkClearAttachment& attachment = attachments[attachment_count++];
    attachment.aspectMask = depth_stencil_aspects;
    attachment.clearValue.depthStencil.depth = float(std::bit_cast<double>(clear.depth_bits));
    attachment.clearValue.depthStencil.stencil = clear.stencil;
  }
  if (!attachment_count) {
    return false;
  }

  VkClearRect rectangle{};
  rectangle.rect.offset.x = left;
  rectangle.rect.offset.y = top;
  rectangle.rect.extent.width = uint32_t(right - left);
  rectangle.rect.extent.height = uint32_t(bottom - top);
  rectangle.baseArrayLayer = 0;
  rectangle.layerCount = 1;

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  vulkan_provider->vulkan_device()->functions().vkCmdClearAttachments(
      command_buffer, attachment_count, attachments.data(), 1, &rectangle);
  return true;
}

VkImageView Gta4NativeGraphicsSystem::GetOrCreateTextureMipView(NativeTextureImage& image,
                                                                uint32_t mip_level) {
  if (mip_level >= image.mip_levels) {
    return VK_NULL_HANDLE;
  }
  if (image.mip_views.size() < image.mip_levels) {
    image.mip_views.resize(image.mip_levels, VK_NULL_HANDLE);
  }
  if (image.mip_views[mip_level]) {
    return image.mip_views[mip_level];
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  VkImageViewCreateInfo view_info{};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.image = image.resource.image;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = image.format;
  view_info.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(image.aspect, mip_level, 1, 0, 1);
  if (vulkan_device->functions().vkCreateImageView(vulkan_device->device(), &view_info, nullptr,
                                                   &image.mip_views[mip_level]) != VK_SUCCESS) {
    image.mip_views[mip_level] = VK_NULL_HANDLE;
  }
  return image.mip_views[mip_level];
}

VkPipeline Gta4NativeGraphicsSystem::GetOrCreateFullscreenPipeline(
    VkFormat destination_format, NativeResolveConversionPipeline::Kind kind,
    VkSampleCountFlagBits destination_samples) {
  for (const NativeResolveConversionPipeline& existing : resolve_conversion_pipelines_) {
    if (existing.destination_format == destination_format && existing.kind == kind &&
        existing.destination_samples == destination_samples) {
      return existing.pipeline;
    }
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  if (!vulkan_device || !resolve_conversion_pipeline_layout_) {
    return VK_NULL_HANDLE;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  VkShaderModule vertex_shader = ui::vulkan::util::CreateShaderModule(
      vulkan_device, fullscreen_cw_vs, sizeof(fullscreen_cw_vs));
  const bool multisampled_source =
      kind == NativeResolveConversionPipeline::Kind::kResolveMultisampled;
  const bool depth_resolve =
      kind == NativeResolveConversionPipeline::Kind::kDepthResolveMultisampled;
  const bool reflection_mip =
      kind == NativeResolveConversionPipeline::Kind::kReflectionMip;
  const uint32_t* pixel_shader_code =
      depth_resolve ? gta4_native_resolve_depth_msaa_ps
      : reflection_mip ? gta4_native_reflection_mip_filter_ps
      : multisampled_source ? gta4_native_resolve_convert_msaa_ps
                            : gta4_native_resolve_convert_ps;
  const size_t pixel_shader_size =
      depth_resolve ? sizeof(gta4_native_resolve_depth_msaa_ps)
      : reflection_mip ? sizeof(gta4_native_reflection_mip_filter_ps)
      : multisampled_source ? sizeof(gta4_native_resolve_convert_msaa_ps)
                            : sizeof(gta4_native_resolve_convert_ps);
  VkShaderModule pixel_shader =
      ui::vulkan::util::CreateShaderModule(vulkan_device, pixel_shader_code, pixel_shader_size);
  if (!vertex_shader || !pixel_shader) {
    if (vertex_shader) {
      dfn.vkDestroyShaderModule(device, vertex_shader, nullptr);
    }
    if (pixel_shader) {
      dfn.vkDestroyShaderModule(device, pixel_shader, nullptr);
    }
    return VK_NULL_HANDLE;
  }

  std::array<VkPipelineShaderStageCreateInfo, 2> stages{};
  stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = vertex_shader;
  stages[0].pName = "main";
  stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = pixel_shader;
  stages[1].pName = "main";
  VkPipelineVertexInputStateCreateInfo vertex_input{};
  vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  VkPipelineInputAssemblyStateCreateInfo input_assembly{};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkPipelineViewportStateCreateInfo viewport_state{};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.scissorCount = 1;
  VkPipelineRasterizationStateCreateInfo rasterization{};
  rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization.polygonMode = VK_POLYGON_MODE_FILL;
  rasterization.cullMode = VK_CULL_MODE_NONE;
  rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterization.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{};
  multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample.rasterizationSamples = destination_samples;
  multisample.sampleShadingEnable = destination_samples != VK_SAMPLE_COUNT_1_BIT;
  multisample.minSampleShading = 1.0f;
  VkPipelineColorBlendAttachmentState color_attachment{};
  color_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  VkPipelineColorBlendStateCreateInfo color_blend{};
  color_blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend.attachmentCount = depth_resolve ? 0 : 1;
  color_blend.pAttachments = depth_resolve ? nullptr : &color_attachment;
  VkPipelineDepthStencilStateCreateInfo depth_stencil{};
  depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depth_stencil.depthTestEnable = depth_resolve;
  depth_stencil.depthWriteEnable = depth_resolve;
  depth_stencil.depthCompareOp = VK_COMPARE_OP_ALWAYS;
  const std::array<VkDynamicState, 2> dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT,
                                                        VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamic_state{};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount = uint32_t(dynamic_states.size());
  dynamic_state.pDynamicStates = dynamic_states.data();
  VkPipelineRenderingCreateInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
  rendering_info.colorAttachmentCount = depth_resolve ? 0 : 1;
  rendering_info.pColorAttachmentFormats = depth_resolve ? nullptr : &destination_format;
  rendering_info.depthAttachmentFormat =
      depth_resolve ? destination_format : VK_FORMAT_UNDEFINED;
  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.pNext = &rendering_info;
  pipeline_info.stageCount = uint32_t(stages.size());
  pipeline_info.pStages = stages.data();
  pipeline_info.pVertexInputState = &vertex_input;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterization;
  pipeline_info.pMultisampleState = &multisample;
  pipeline_info.pDepthStencilState = &depth_stencil;
  pipeline_info.pColorBlendState = &color_blend;
  pipeline_info.pDynamicState = &dynamic_state;
  pipeline_info.layout = resolve_conversion_pipeline_layout_;
  VkPipeline pipeline = VK_NULL_HANDLE;
  const VkResult result = dfn.vkCreateGraphicsPipelines(
      device, native_pipeline_cache_, 1, &pipeline_info, nullptr, &pipeline);
  dfn.vkDestroyShaderModule(device, pixel_shader, nullptr);
  dfn.vkDestroyShaderModule(device, vertex_shader, nullptr);
  if (result != VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  resolve_conversion_pipelines_.push_back(
      {destination_format, kind, destination_samples, pipeline});
  native_pipeline_cache_dirty_ = true;
  return pipeline;
}

VkPipeline Gta4NativeGraphicsSystem::GetOrCreateResolveConversionPipeline(
    VkFormat destination_format, bool multisampled_source,
    VkSampleCountFlagBits destination_samples) {
  return GetOrCreateFullscreenPipeline(
      destination_format,
      multisampled_source ? NativeResolveConversionPipeline::Kind::kResolveMultisampled
                          : NativeResolveConversionPipeline::Kind::kResolve,
      destination_samples);
}

VkPipeline Gta4NativeGraphicsSystem::GetOrCreateDepthResolvePipeline(
    VkFormat destination_format) {
  return GetOrCreateFullscreenPipeline(
      destination_format,
      NativeResolveConversionPipeline::Kind::kDepthResolveMultisampled);
}

VkPipeline Gta4NativeGraphicsSystem::GetOrCreateReflectionMipPipeline(
    VkFormat destination_format) {
  return GetOrCreateFullscreenPipeline(
      destination_format, NativeResolveConversionPipeline::Kind::kReflectionMip);
}

VkPipeline Gta4NativeGraphicsSystem::GetOrCreateHDRPresentPipeline() {
  if (hdr_present_pipeline_) {
    return hdr_present_pipeline_;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device || !resolve_conversion_pipeline_layout_) {
    return VK_NULL_HANDLE;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  VkShaderModule vertex_shader = ui::vulkan::util::CreateShaderModule(
      vulkan_device, fullscreen_cw_vs, sizeof(fullscreen_cw_vs));
  VkShaderModule pixel_shader = ui::vulkan::util::CreateShaderModule(
      vulkan_device, gta4_native_hdr_present_ps, sizeof(gta4_native_hdr_present_ps));
  if (!vertex_shader || !pixel_shader) {
    if (vertex_shader) {
      dfn.vkDestroyShaderModule(device, vertex_shader, nullptr);
    }
    if (pixel_shader) {
      dfn.vkDestroyShaderModule(device, pixel_shader, nullptr);
    }
    return VK_NULL_HANDLE;
  }

  std::array<VkPipelineShaderStageCreateInfo, 2> stages{};
  stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = vertex_shader;
  stages[0].pName = "main";
  stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = pixel_shader;
  stages[1].pName = "main";
  VkPipelineVertexInputStateCreateInfo vertex_input{};
  vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  VkPipelineInputAssemblyStateCreateInfo input_assembly{};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkPipelineViewportStateCreateInfo viewport_state{};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.scissorCount = 1;
  VkPipelineRasterizationStateCreateInfo rasterization{};
  rasterization.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization.polygonMode = VK_POLYGON_MODE_FILL;
  rasterization.cullMode = VK_CULL_MODE_NONE;
  rasterization.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterization.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{};
  multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineColorBlendAttachmentState color_attachment{};
  color_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  VkPipelineColorBlendStateCreateInfo color_blend{};
  color_blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend.attachmentCount = 1;
  color_blend.pAttachments = &color_attachment;
  const std::array<VkDynamicState, 2> dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT,
                                                        VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamic_state{};
  dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state.dynamicStateCount = uint32_t(dynamic_states.size());
  dynamic_state.pDynamicStates = dynamic_states.data();
  const VkFormat destination_format = ui::vulkan::VulkanPresenter::kGuestOutputFormat;
  VkPipelineRenderingCreateInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
  rendering_info.colorAttachmentCount = 1;
  rendering_info.pColorAttachmentFormats = &destination_format;
  VkGraphicsPipelineCreateInfo pipeline_info{};
  pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_info.pNext = &rendering_info;
  pipeline_info.stageCount = uint32_t(stages.size());
  pipeline_info.pStages = stages.data();
  pipeline_info.pVertexInputState = &vertex_input;
  pipeline_info.pInputAssemblyState = &input_assembly;
  pipeline_info.pViewportState = &viewport_state;
  pipeline_info.pRasterizationState = &rasterization;
  pipeline_info.pMultisampleState = &multisample;
  pipeline_info.pColorBlendState = &color_blend;
  pipeline_info.pDynamicState = &dynamic_state;
  pipeline_info.layout = resolve_conversion_pipeline_layout_;
  const VkResult result = dfn.vkCreateGraphicsPipelines(
      device, native_pipeline_cache_, 1, &pipeline_info, nullptr, &hdr_present_pipeline_);
  dfn.vkDestroyShaderModule(device, pixel_shader, nullptr);
  dfn.vkDestroyShaderModule(device, vertex_shader, nullptr);
  if (result != VK_SUCCESS) {
    hdr_present_pipeline_ = VK_NULL_HANDLE;
  } else {
    native_pipeline_cache_dirty_ = true;
  }
  return hdr_present_pipeline_;
}

bool Gta4NativeGraphicsSystem::RecordResolveConversion(
    VkCommandBuffer command_buffer, NativeSurfaceImage& source, NativeTextureImage& destination,
    uint32_t destination_level, int32_t source_left, int32_t source_top, int32_t destination_x,
    int32_t destination_y, uint32_t copy_width, uint32_t copy_height,
    const GuestSurfaceView& source_view, const GuestSurfaceView& requested_view,
    xenos::CopySampleSelect sample_select) {
  if (source.aspect != VK_IMAGE_ASPECT_COLOR_BIT ||
      destination.aspect != VK_IMAGE_ASPECT_COLOR_BIT || !frame_descriptor_pool_ ||
      !resolve_conversion_descriptor_set_layout_) {
    return false;
  }
  VkImageView destination_view = GetOrCreateTextureMipView(destination, destination_level);
  VkPipeline pipeline = GetOrCreateResolveConversionPipeline(
      destination.format, source.samples != VK_SAMPLE_COUNT_1_BIT);
  if (!destination_view || !pipeline) {
    return false;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  VkDescriptorSet resolve_descriptor_set = VK_NULL_HANDLE;
  VkDescriptorSetAllocateInfo descriptor_allocate_info{};
  descriptor_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  descriptor_allocate_info.descriptorPool = frame_descriptor_pool_;
  descriptor_allocate_info.descriptorSetCount = 1;
  descriptor_allocate_info.pSetLayouts = &resolve_conversion_descriptor_set_layout_;
  if (dfn.vkAllocateDescriptorSets(device, &descriptor_allocate_info, &resolve_descriptor_set) !=
      VK_SUCCESS) {
    return false;
  }
  VkDescriptorImageInfo descriptor_image{};
  descriptor_image.imageView = source.resource.view;
  descriptor_image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  VkWriteDescriptorSet descriptor_write{};
  descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptor_write.dstSet = resolve_descriptor_set;
  descriptor_write.dstBinding = 0;
  descriptor_write.descriptorCount = 1;
  descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptor_write.pImageInfo = &descriptor_image;
  dfn.vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);

  std::array<VkImageMemoryBarrier, 2> barriers{};
  barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
  barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[0].oldLayout = source.layout;
  barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].image = source.resource.image;
  barriers[0].subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(source.aspect, 0, 1, 0, 1);
  barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[1].srcAccessMask = destination.layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                                  ? VK_ACCESS_SHADER_READ_BIT
                                  : 0;
  barriers[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  barriers[1].oldLayout = destination.layout;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].image = destination.resource.image;
  barriers[1].subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(destination.aspect, destination_level, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0,
      nullptr, 0, nullptr, uint32_t(barriers.size()), barriers.data());

  // All coordinates passed by RecordResolve are already scaled from the guest
  // extent to the native image extent. Dynamic rendering must cover that same
  // physical mip extent. Using the guest fetch-constant dimensions here left
  // most of a high-resolution reflection attachment outside the viewport and
  // render area, so later sampling observed undefined texels.
  const uint32_t mip_width = CalculateNativeMipExtent(destination.width, destination_level);
  const uint32_t mip_height = CalculateNativeMipExtent(destination.height, destination_level);
  const uint32_t logical_mip_width =
      CalculateNativeMipExtent(destination.logical_width, destination_level);
  const uint32_t logical_mip_height =
      CalculateNativeMipExtent(destination.logical_height, destination_level);
  const bool xenos_float16_pack =
      destination.format == VK_FORMAT_R16G16B16A16_SFLOAT;
  TraceNativeRendererEvent(
      "resolve-conversion-target",
      fmt::format(
          "destination={:08X} level={} logical={}x{} physical={}x{} "
          "offset={},{} extent={}x{} xenos-float16-pack={}",
          destination.source ? destination.source->handle : 0, destination_level,
          logical_mip_width, logical_mip_height, mip_width, mip_height,
          destination_x, destination_y, copy_width, copy_height,
          xenos_float16_pack));
  VkRenderingAttachmentInfo color_attachment{};
  color_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  color_attachment.imageView = destination_view;
  color_attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  color_attachment.loadOp = destination.layout == VK_IMAGE_LAYOUT_UNDEFINED
                                ? VK_ATTACHMENT_LOAD_OP_DONT_CARE
                                : VK_ATTACHMENT_LOAD_OP_LOAD;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  VkRenderingInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
  rendering_info.renderArea.extent = {mip_width, mip_height};
  rendering_info.layerCount = 1;
  rendering_info.colorAttachmentCount = 1;
  rendering_info.pColorAttachments = &color_attachment;
  dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
  VkViewport viewport{};
  viewport.width = float(mip_width);
  viewport.height = float(mip_height);
  viewport.maxDepth = 1.0f;
  dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
  VkRect2D scissor{};
  scissor.offset = {destination_x, destination_y};
  scissor.extent = {copy_width, copy_height};
  dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
  dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  dfn.vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              resolve_conversion_pipeline_layout_, 0, 1, &resolve_descriptor_set, 0,
                              nullptr);
  NativeResolveConversionConstants constants{};
  constants.source_x = source_left;
  constants.source_y = source_top;
  constants.destination_x = destination_x;
  constants.destination_y = destination_y;
  constants.source_sample_type = uint32_t(source_view.msaa_samples);
  constants.requested_sample_type = uint32_t(requested_view.msaa_samples);
  constants.sample_select = uint32_t(sample_select);
  constants.reserved[0] = xenos_float16_pack;
  dfn.vkCmdPushConstants(command_buffer, resolve_conversion_pipeline_layout_,
                         VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(constants), &constants);
  dfn.vkCmdDraw(command_buffer, 3, 1, 0, 0);
  dfn.vkCmdEndRendering(command_buffer);

  VkImageMemoryBarrier destination_barrier{};
  destination_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  destination_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  destination_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  destination_barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  destination_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  destination_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.image = destination.resource.image;
  destination_barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(destination.aspect, destination_level, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
      nullptr, 1, &destination_barrier);
  source.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  destination.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  return true;
}

bool Gta4NativeGraphicsSystem::RecordDepthResolveConversion(
    VkCommandBuffer command_buffer, NativeSurfaceImage& source,
    NativeTextureImage& destination, uint32_t destination_level,
    int32_t source_left, int32_t source_top, int32_t destination_x,
    int32_t destination_y, uint32_t copy_width, uint32_t copy_height,
    const GuestSurfaceView& source_view, const GuestSurfaceView& requested_view,
    xenos::CopySampleSelect sample_select) {
  if (!(source.aspect & VK_IMAGE_ASPECT_DEPTH_BIT) ||
      !(destination.aspect & VK_IMAGE_ASPECT_DEPTH_BIT) ||
      source.samples == VK_SAMPLE_COUNT_1_BIT) {
    return false;
  }
  VkImageView destination_view = GetOrCreateTextureMipView(destination, destination_level);
  if (!destination_view) {
    return false;
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  // The guest depth resolve carries stencil coverage as well as depth. Deferred
  // lighting relies on that coverage to reject background pixels before its
  // reciprocal depth reconstruction. A fragment shader can write depth, but it
  // cannot portably write stencil, so prefer Vulkan's depth/stencil attachment
  // resolve whenever the guest requests the directly corresponding sample-zero
  // full-surface resolve.
  {
    VkPhysicalDeviceDepthStencilResolveProperties resolve_properties{};
    resolve_properties.sType =
        VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES;
    VkPhysicalDeviceProperties2 properties{};
    properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    properties.pNext = &resolve_properties;
    const auto& ifn = vulkan_device->vulkan_instance()->functions();
    ifn.vkGetPhysicalDeviceProperties2(vulkan_device->physical_device(), &properties);

    const uint32_t mip_width =
        CalculateNativeMipExtent(destination.width, destination_level);
    const uint32_t mip_height =
        CalculateNativeMipExtent(destination.height, destination_level);
    const bool combined_depth_stencil =
        (source.aspect & VK_IMAGE_ASPECT_STENCIL_BIT) &&
        (destination.aspect & VK_IMAGE_ASPECT_STENCIL_BIT);
    const bool sample_zero_supported =
        (resolve_properties.supportedDepthResolveModes &
         VK_RESOLVE_MODE_SAMPLE_ZERO_BIT) &&
        (resolve_properties.supportedStencilResolveModes &
         VK_RESOLVE_MODE_SAMPLE_ZERO_BIT);
    const bool matching_full_surface =
        source.format == destination.format && source.width == mip_width &&
        source.height == mip_height && source_left == 0 && source_top == 0 &&
        destination_x == 0 && destination_y == 0 && copy_width == mip_width &&
        copy_height == mip_height;
    const bool sample_zero_requested = sample_select == xenos::CopySampleSelect::k0;
    TraceNativeRendererEvent(
        "depth-stencil-resolve-capability",
        fmt::format(
            "source={:08X} destination={:08X} combined={} full-surface={} "
            "sample-zero-requested={} depth-modes={:08X} stencil-modes={:08X} "
            "independent-none={} independent={}",
            source.descriptor.handle,
            destination.source ? destination.source->handle : 0,
            combined_depth_stencil, matching_full_surface, sample_zero_requested,
            resolve_properties.supportedDepthResolveModes,
            resolve_properties.supportedStencilResolveModes,
            resolve_properties.independentResolveNone != VK_FALSE,
            resolve_properties.independentResolve != VK_FALSE));
    if (combined_depth_stencil && sample_zero_supported && matching_full_surface &&
        sample_zero_requested) {
      std::array<VkImageMemoryBarrier, 2> barriers{};
      barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barriers[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
                                  VK_ACCESS_TRANSFER_WRITE_BIT |
                                  VK_ACCESS_SHADER_READ_BIT;
      barriers[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
      barriers[0].oldLayout = source.layout;
      barriers[0].newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
      barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barriers[0].image = source.resource.image;
      barriers[0].subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
          source.aspect, 0, 1, 0, 1);
      barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barriers[1].srcAccessMask =
          destination.layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
              ? VK_ACCESS_SHADER_READ_BIT
              : 0;
      barriers[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      barriers[1].oldLayout = destination.layout;
      barriers[1].newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
      barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barriers[1].image = destination.resource.image;
      barriers[1].subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
          destination.aspect, destination_level, 1, 0, 1);
      dfn.vkCmdPipelineBarrier(
          command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
          VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
              VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT |
              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
          0, 0, nullptr, 0, nullptr, uint32_t(barriers.size()), barriers.data());

      VkRenderingAttachmentInfo depth_attachment{};
      depth_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
      depth_attachment.imageView = source.resource.view;
      depth_attachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
      depth_attachment.resolveMode = VK_RESOLVE_MODE_SAMPLE_ZERO_BIT;
      depth_attachment.resolveImageView = destination_view;
      depth_attachment.resolveImageLayout =
          VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
      depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
      depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      VkRenderingAttachmentInfo stencil_attachment = depth_attachment;
      VkRenderingInfo rendering_info{};
      rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
      rendering_info.renderArea.extent = {mip_width, mip_height};
      rendering_info.layerCount = 1;
      rendering_info.pDepthAttachment = &depth_attachment;
      rendering_info.pStencilAttachment = &stencil_attachment;
      dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
      dfn.vkCmdEndRendering(command_buffer);

      std::array<VkImageMemoryBarrier, 2> read_barriers{};
      for (VkImageMemoryBarrier& barrier : read_barriers) {
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      }
      read_barriers[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
      read_barriers[0].image = source.resource.image;
      read_barriers[0].subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
          source.aspect, 0, 1, 0, 1);
      read_barriers[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      read_barriers[1].image = destination.resource.image;
      read_barriers[1].subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
          destination.aspect, destination_level, 1, 0, 1);
      dfn.vkCmdPipelineBarrier(
          command_buffer,
          VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
              VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT |
              VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
          VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
          0, 0, nullptr, 0, nullptr, uint32_t(read_barriers.size()),
          read_barriers.data());
      source.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      destination.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      TraceNativeRendererEvent(
          "depth-stencil-resolve-target",
          fmt::format(
              "source={:08X} destination={:08X} level={} extent={}x{} "
              "mode=sample-zero depth=true stencil=true",
              source.descriptor.handle,
              destination.source ? destination.source->handle : 0,
              destination_level, mip_width, mip_height));
      return true;
    }
  }

  if (!source.sampled_view || !frame_descriptor_pool_ ||
      !resolve_conversion_descriptor_set_layout_) {
    return false;
  }
  VkPipeline pipeline = GetOrCreateDepthResolvePipeline(destination.format);
  if (!pipeline) {
    return false;
  }

  VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
  VkDescriptorSetAllocateInfo allocate_info{};
  allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocate_info.descriptorPool = frame_descriptor_pool_;
  allocate_info.descriptorSetCount = 1;
  allocate_info.pSetLayouts = &resolve_conversion_descriptor_set_layout_;
  if (dfn.vkAllocateDescriptorSets(device, &allocate_info, &descriptor_set) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorImageInfo image_info{};
  image_info.imageView = source.sampled_view;
  image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  VkWriteDescriptorSet descriptor_write{};
  descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptor_write.dstSet = descriptor_set;
  descriptor_write.dstBinding = 0;
  descriptor_write.descriptorCount = 1;
  descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  descriptor_write.pImageInfo = &image_info;
  dfn.vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);

  std::array<VkImageMemoryBarrier, 2> barriers{};
  barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
                              VK_ACCESS_TRANSFER_WRITE_BIT;
  barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[0].oldLayout = source.layout;
  barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].image = source.resource.image;
  barriers[0].subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(source.aspect, 0, 1, 0, 1);
  barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[1].srcAccessMask = destination.layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                                  ? VK_ACCESS_SHADER_READ_BIT
                                  : 0;
  barriers[1].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  barriers[1].oldLayout = destination.layout;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].image = destination.resource.image;
  barriers[1].subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      destination.aspect, destination_level, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
          VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      0, 0, nullptr, 0, nullptr, uint32_t(barriers.size()), barriers.data());

  const uint32_t mip_width = CalculateNativeMipExtent(destination.width, destination_level);
  const uint32_t mip_height = CalculateNativeMipExtent(destination.height, destination_level);
  const uint32_t logical_mip_width =
      CalculateNativeMipExtent(destination.logical_width, destination_level);
  const uint32_t logical_mip_height =
      CalculateNativeMipExtent(destination.logical_height, destination_level);
  TraceNativeRendererEvent(
      "depth-resolve-target",
      fmt::format(
          "destination={:08X} level={} logical={}x{} physical={}x{} "
          "offset={},{} extent={}x{} source={:08X} source-samples={} requested-samples={} "
          "select={}",
          destination.source ? destination.source->handle : 0, destination_level,
          logical_mip_width, logical_mip_height, mip_width, mip_height,
          destination_x, destination_y, copy_width, copy_height, source.descriptor.handle,
          uint32_t(source_view.msaa_samples), uint32_t(requested_view.msaa_samples),
          uint32_t(sample_select)));

  VkRenderingAttachmentInfo depth_attachment{};
  depth_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  depth_attachment.imageView = destination_view;
  depth_attachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  depth_attachment.loadOp = destination.layout == VK_IMAGE_LAYOUT_UNDEFINED
                                ? VK_ATTACHMENT_LOAD_OP_DONT_CARE
                                : VK_ATTACHMENT_LOAD_OP_LOAD;
  depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  VkRenderingInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
  rendering_info.renderArea.extent = {mip_width, mip_height};
  rendering_info.layerCount = 1;
  rendering_info.pDepthAttachment = &depth_attachment;
  dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
  VkViewport viewport{};
  viewport.width = float(mip_width);
  viewport.height = float(mip_height);
  viewport.maxDepth = 1.0f;
  dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
  VkRect2D scissor{};
  scissor.offset = {destination_x, destination_y};
  scissor.extent = {copy_width, copy_height};
  dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
  dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  dfn.vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              resolve_conversion_pipeline_layout_, 0, 1, &descriptor_set, 0,
                              nullptr);
  NativeResolveConversionConstants constants{};
  constants.source_x = source_left;
  constants.source_y = source_top;
  constants.destination_x = destination_x;
  constants.destination_y = destination_y;
  constants.source_sample_type = uint32_t(source_view.msaa_samples);
  constants.requested_sample_type = uint32_t(requested_view.msaa_samples);
  constants.sample_select = uint32_t(sample_select);
  dfn.vkCmdPushConstants(command_buffer, resolve_conversion_pipeline_layout_,
                         VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(constants), &constants);
  dfn.vkCmdDraw(command_buffer, 3, 1, 0, 0);
  dfn.vkCmdEndRendering(command_buffer);

  VkImageMemoryBarrier destination_barrier{};
  destination_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  destination_barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  destination_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  destination_barrier.oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  destination_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  destination_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.image = destination.resource.image;
  destination_barrier.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      destination.aspect, destination_level, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(
      command_buffer,
      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
      VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
      nullptr, 0, nullptr, 1, &destination_barrier);
  source.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  destination.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  return true;
}

const Gta4NativeGraphicsSystem::NativePlacementOwner*
Gta4NativeGraphicsSystem::FindPlacementOwner(const SurfaceDescriptor& descriptor,
                                             bool depth) const {
  if (reflection_resources_.contains(descriptor.handle)) {
    return nullptr;
  }
  GuestSurfaceView view{};
  if (!DecodeGuestSurfaceView(descriptor, depth, view)) {
    return nullptr;
  }
  const GuestPlacementKey key = GetGuestPlacementKey(view);
  const auto owner = native_placement_owners_.find(key);
  if (owner == native_placement_owners_.end() || !owner->second.image ||
      owner->second.image->materialized_serial != owner->second.serial ||
      owner->second.image->materialized_placement != key) {
    return nullptr;
  }
  return &owner->second;
}

bool Gta4NativeGraphicsSystem::HasCurrentPlacementContent(
    const NativeSurfaceImage& surface, const SurfaceDescriptor& descriptor, bool depth) const {
  if (surface.is_reflection) {
    return surface.ever_written;
  }
  GuestSurfaceView view{};
  if (!DecodeGuestSurfaceView(descriptor, depth, view)) {
    return false;
  }
  const GuestPlacementKey key = GetGuestPlacementKey(view);
  const NativePlacementOwner* owner = FindPlacementOwner(descriptor, depth);
  return owner && owner->image == &surface && surface.materialized_serial == owner->serial &&
         surface.materialized_placement == key;
}

void Gta4NativeGraphicsSystem::ClaimSurfaceContent(NativeSurfaceImage& surface,
                                                   const SurfaceDescriptor& descriptor,
                                                   bool depth, uint32_t submitted_frame,
                                                   RenderPhase render_phase,
                                                   NativePlacementOwner::WriteKind write_kind) {
  surface.ever_written = true;
  if (surface.is_reflection) {
    return;
  }
  GuestSurfaceView view{};
  if (!DecodeGuestSurfaceView(descriptor, depth, view)) {
    return;
  }
  const GuestPlacementKey key = GetGuestPlacementKey(view);
  const uint64_t serial = next_surface_write_serial_++;
  surface.materialized_placement = key;
  surface.materialized_serial = serial;
  native_placement_owners_[key] =
      {&surface, view, serial, submitted_frame, diagnostic_command_index_, render_phase,
       write_kind};
  TraceNativeRendererEvent(
      "placement-owner-claimed",
      fmt::format(
          "handle={:08X} placement={} pitch={} samplespace={}x{} guest-samples={} "
          "depth={} serial={} phase={} write-kind={}",
          descriptor.handle, key.placement_base_tiles, key.sample_pitch, key.sample_width,
          key.sample_height, uint32_t(view.msaa_samples), depth, serial,
          RenderPhaseName(render_phase), uint32_t(write_kind)));
}

bool Gta4NativeGraphicsSystem::RecordSurfaceMaterialization(
    VkCommandBuffer command_buffer, const NativePlacementOwner& owner,
    NativeSurfaceImage& destination, const GuestSurfaceView& destination_view) {
  NativeSurfaceImage* source = owner.image;
  if (!source || source == &destination || source->is_reflection || destination.is_reflection ||
      source->aspect != VK_IMAGE_ASPECT_COLOR_BIT ||
      destination.aspect != VK_IMAGE_ASPECT_COLOR_BIT || !source->ever_written ||
      !frame_descriptor_pool_ || !resolve_conversion_descriptor_set_layout_ ||
      GetGuestPlacementKey(owner.view) != GetGuestPlacementKey(destination_view)) {
    return false;
  }

  const VkPipeline pipeline = GetOrCreateResolveConversionPipeline(
      destination.format, source->samples != VK_SAMPLE_COUNT_1_BIT, destination.samples);
  if (!pipeline) {
    return false;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
  VkDescriptorSetAllocateInfo allocate_info{};
  allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocate_info.descriptorPool = frame_descriptor_pool_;
  allocate_info.descriptorSetCount = 1;
  allocate_info.pSetLayouts = &resolve_conversion_descriptor_set_layout_;
  if (dfn.vkAllocateDescriptorSets(device, &allocate_info, &descriptor_set) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorImageInfo image_info{};
  image_info.imageView = source->resource.view;
  image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  VkWriteDescriptorSet write{};
  write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  write.dstSet = descriptor_set;
  write.dstBinding = 0;
  write.descriptorCount = 1;
  write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  write.pImageInfo = &image_info;
  dfn.vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);

  std::array<VkImageMemoryBarrier, 2> barriers{};
  barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
  barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barriers[0].oldLayout = source->layout;
  barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[0].image = source->resource.image;
  barriers[0].subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barriers[1].srcAccessMask = destination.ever_written
                                  ? VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                        VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT
                                  : 0;
  barriers[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  barriers[1].oldLayout = destination.layout;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barriers[1].image = destination.resource.image;
  barriers[1].subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
      0, nullptr, 0, nullptr, uint32_t(barriers.size()), barriers.data());

  VkRenderingAttachmentInfo attachment{};
  attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  attachment.imageView = destination.resource.view;
  attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  VkRenderingInfo rendering_info{};
  rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
  rendering_info.renderArea.extent = {destination.width, destination.height};
  rendering_info.layerCount = 1;
  rendering_info.colorAttachmentCount = 1;
  rendering_info.pColorAttachments = &attachment;
  dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
  VkViewport viewport{};
  viewport.width = float(destination.width);
  viewport.height = float(destination.height);
  viewport.maxDepth = 1.0f;
  dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
  VkRect2D scissor{};
  scissor.extent = {destination.width, destination.height};
  dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
  dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  dfn.vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              resolve_conversion_pipeline_layout_, 0, 1, &descriptor_set, 0,
                              nullptr);
  NativeResolveConversionConstants constants{};
  constants.source_sample_type = uint32_t(owner.view.msaa_samples);
  constants.requested_sample_type = uint32_t(destination_view.msaa_samples);
  constants.destination_sample_type = uint32_t(destination_view.msaa_samples);
  constants.mode = 1;
  dfn.vkCmdPushConstants(command_buffer, resolve_conversion_pipeline_layout_,
                         VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(constants), &constants);
  dfn.vkCmdDraw(command_buffer, 3, 1, 0, 0);
  dfn.vkCmdEndRendering(command_buffer);

  source->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  destination.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  destination.ever_written = true;
  destination.materialized_placement = GetGuestPlacementKey(destination_view);
  destination.materialized_serial = owner.serial;
  native_placement_owners_[destination.materialized_placement] =
      {&destination, destination_view, owner.serial, owner.frame,
       owner.command_index, owner.render_phase, owner.write_kind};
  TraceNativeRendererEvent(
      "placement-view-materialized",
      fmt::format(
          "source={:08X} destination={:08X} placement={} pitch={} samplespace={}x{} "
          "source-samples={} destination-samples={} serial={}",
          source->descriptor.handle, destination.descriptor.handle,
          destination.materialized_placement.placement_base_tiles,
          destination.materialized_placement.sample_pitch,
          destination.materialized_placement.sample_width,
          destination.materialized_placement.sample_height, uint32_t(owner.view.msaa_samples),
          uint32_t(destination_view.msaa_samples), owner.serial));
  return true;
}

bool Gta4NativeGraphicsSystem::EnsureDepthHandoffStencilScratch(
    NativeSurfaceImage& destination) {
  if (destination.depth_handoff_stencil_scratch_buffer) {
    return true;
  }

  auto* vulkan_provider =
      static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    return false;
  }

  // A stencil-aspect copy contains one byte per D32S8 texel. Use a buffer so
  // MoltenVK cannot fold the preservation copy into a combined Metal
  // depth/stencil texture blit that also restores the old depth plane.
  const VkDeviceSize scratch_size =
      VkDeviceSize(destination.width) * VkDeviceSize(destination.height);
  if (!scratch_size ||
      !ui::vulkan::util::CreateDedicatedAllocationBuffer(
          vulkan_device, scratch_size,
          VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
          ui::vulkan::util::MemoryPurpose::kDeviceLocal,
          destination.depth_handoff_stencil_scratch_buffer,
          destination.depth_handoff_stencil_scratch_memory)) {
    destination.depth_handoff_stencil_scratch_buffer = VK_NULL_HANDLE;
    destination.depth_handoff_stencil_scratch_memory = VK_NULL_HANDLE;
    destination.depth_handoff_stencil_scratch_size = 0;
    destination.depth_handoff_stencil_scratch_initialized = false;
    return false;
  }

  destination.depth_handoff_stencil_scratch_size = scratch_size;
  destination.depth_handoff_stencil_scratch_initialized = false;
  REXLOG_INFO(
      "gta4-native-cause: point=explicit-depth-handoff-stencil-scratch "
      "result=created transport=buffer destination={:08X}/{:08X} "
      "extent={}x{} format={} bytes={}",
      destination.descriptor.handle, destination.descriptor.address,
      destination.width, destination.height, uint32_t(destination.format),
      scratch_size);
  return true;
}

bool Gta4NativeGraphicsSystem::RecordDepthSurfaceHandoff(
    VkCommandBuffer command_buffer, const NativeCommand& native_command,
    uint32_t submitted_frame) {
  DepthSurfaceHandoffCommand handoff{};
  std::memcpy(&handoff, native_command.bytes.data(), sizeof(handoff));

  auto reject = [&](const char* reason, const NativeSurfaceImage* source,
                    const NativeSurfaceImage* destination) {
    REXLOG_ERROR(
        "gta4-native-cause: point=explicit-depth-handoff frame={} cmd={} result=rejected "
        "reason={} source-wrapper={:08X} source={:08X}/{:08X} "
        "destination-wrapper={:08X} destination={:08X}/{:08X} "
        "source-texture={:08X}@{} source-host={}x{}:s{} "
        "destination-host={}x{}:s{}",
        submitted_frame, diagnostic_command_index_, reason, handoff.source_wrapper,
        handoff.source.handle, handoff.source.address, handoff.destination_wrapper,
        handoff.destination.handle, handoff.destination.address,
        handoff.source_texture,
        native_command.depth_handoff_source
            ? native_command.depth_handoff_source->generation
            : 0,
        source ? source->width : 0, source ? source->height : 0,
        source ? uint32_t(source->samples) : 0,
        destination ? destination->width : 0,
        destination ? destination->height : 0,
        destination ? uint32_t(destination->samples) : 0);
    return false;
  };

  NativeSurfaceImage* source = GetOrCreateSurfaceImage(handoff.source, true);
  NativeTextureImage* source_texture = nullptr;
  if (native_command.depth_handoff_source) {
    const auto source_texture_entry = native_texture_images_.find(
        native_command.depth_handoff_source->generation);
    if (source_texture_entry != native_texture_images_.end()) {
      source_texture = source_texture_entry->second.get();
    }
  }
  NativeSurfaceImage* destination =
      GetOrCreateSurfaceImage(handoff.destination, true);
  constexpr VkImageAspectFlags kDepthStencilAspects =
      VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
  if (!source || !source_texture || !destination) {
    return reject("surface-create", source, destination);
  }
  if (source_texture->resource.image == destination->resource.image) {
    return reject("same-object", source, destination);
  }
  if (source_texture->is_reflection || destination->is_reflection) {
    return reject("reflection", source, destination);
  }
  if (source_texture->layout == VK_IMAGE_LAYOUT_UNDEFINED) {
    return reject("source-unwritten", source, destination);
  }
  if (source_texture->aspect != kDepthStencilAspects ||
      destination->aspect != kDepthStencilAspects ||
      source_texture->format != destination->format) {
    return reject("format-or-aspect", source, destination);
  }
  if (destination->samples != VK_SAMPLE_COUNT_1_BIT) {
    return reject("sample-count", source, destination);
  }
  if (source_texture->width != destination->width ||
      source_texture->height != destination->height ||
      source_texture->logical_width != destination->logical_width ||
      source_texture->logical_height != destination->logical_height) {
    return reject("extent", source, destination);
  }
  if (!source_texture->resource.view || !destination->resource.view) {
    return reject("depth-stencil-view", source, destination);
  }

  auto* vulkan_provider =
      static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider->vulkan_device();
  const auto& dfn = vulkan_device->functions();
  const bool destination_stencil_initialized = destination->ever_written;
  if (destination_stencil_initialized &&
      !EnsureDepthHandoffStencilScratch(*destination)) {
    return reject("stencil-scratch-allocation", source, destination);
  }
  RecordDepthStencilDiagnosticProbe(
      command_buffer, *source_texture, 19, 20,
      "explicit-resolved-handoff-source", handoff.source_wrapper,
      native_command.depth_handoff_source->generation);
  if (destination_stencil_initialized) {
    RecordDepthStencilDiagnosticProbe(command_buffer, *destination, 34, 35);
  }

  if (destination_stencil_initialized) {
    VkImageMemoryBarrier preserve_image_barrier{};
    preserve_image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    preserve_image_barrier.srcAccessMask =
        VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
    preserve_image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    preserve_image_barrier.oldLayout = destination->layout;
    preserve_image_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    preserve_image_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserve_image_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserve_image_barrier.image = destination->resource.image;
    preserve_image_barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
    VkBufferMemoryBarrier preserve_buffer_barrier{};
    preserve_buffer_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    preserve_buffer_barrier.srcAccessMask =
        destination->depth_handoff_stencil_scratch_initialized
            ? VK_ACCESS_TRANSFER_READ_BIT
            : 0;
    preserve_buffer_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    preserve_buffer_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserve_buffer_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserve_buffer_barrier.buffer =
        destination->depth_handoff_stencil_scratch_buffer;
    preserve_buffer_barrier.size =
        destination->depth_handoff_stencil_scratch_size;
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 1,
        &preserve_buffer_barrier, 1, &preserve_image_barrier);

    VkBufferImageCopy stencil_copy{};
    stencil_copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
    stencil_copy.imageSubresource.layerCount = 1;
    stencil_copy.imageExtent = {destination->width, destination->height, 1};
    dfn.vkCmdCopyImageToBuffer(
        command_buffer, destination->resource.image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        destination->depth_handoff_stencil_scratch_buffer, 1, &stencil_copy);

    VkImageMemoryBarrier preserved_image_barrier{};
    preserved_image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    preserved_image_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    preserved_image_barrier.dstAccessMask =
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    preserved_image_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    preserved_image_barrier.newLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    preserved_image_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserved_image_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserved_image_barrier.image = destination->resource.image;
    preserved_image_barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
    VkBufferMemoryBarrier preserved_buffer_barrier{};
    preserved_buffer_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    preserved_buffer_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    preserved_buffer_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    preserved_buffer_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserved_buffer_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    preserved_buffer_barrier.buffer =
        destination->depth_handoff_stencil_scratch_buffer;
    preserved_buffer_barrier.size =
        destination->depth_handoff_stencil_scratch_size;
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT |
            VK_PIPELINE_STAGE_TRANSFER_BIT,
        0, 0, nullptr, 1, &preserved_buffer_barrier, 1,
        &preserved_image_barrier);
    destination->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    destination->depth_handoff_stencil_scratch_initialized = true;
    REXLOG_INFO(
        "gta4-native-cause: point=explicit-depth-handoff-stencil-preserve "
        "frame={} cmd={} phase=saved transport=buffer "
        "destination={:08X}/{:08X} extent={}x{} bytes={}",
        submitted_frame, diagnostic_command_index_, destination->descriptor.handle,
        destination->descriptor.address, destination->width, destination->height,
        destination->depth_handoff_stencil_scratch_size);
  } else {
    VkImageMemoryBarrier destination_barrier{};
    destination_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    destination_barrier.dstAccessMask =
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    destination_barrier.oldLayout = destination->layout;
    destination_barrier.newLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    destination_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_barrier.image = destination->resource.image;
    destination_barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        0, 0, nullptr, 0, nullptr, 1, &destination_barrier);
    destination->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkRenderingAttachmentInfo stencil_initialization{};
    stencil_initialization.sType =
        VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    stencil_initialization.imageView = destination->resource.view;
    stencil_initialization.imageLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    stencil_initialization.resolveMode = VK_RESOLVE_MODE_NONE;
    stencil_initialization.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    stencil_initialization.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    stencil_initialization.clearValue.depthStencil = {0.0f, 0};
    VkRenderingInfo stencil_rendering{};
    stencil_rendering.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    stencil_rendering.renderArea.extent = {destination->width,
                                           destination->height};
    stencil_rendering.layerCount = 1;
    stencil_rendering.pStencilAttachment = &stencil_initialization;
    dfn.vkCmdBeginRendering(command_buffer, &stencil_rendering);
    dfn.vkCmdEndRendering(command_buffer);

    VkImageMemoryBarrier stencil_to_resolve{};
    stencil_to_resolve.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    stencil_to_resolve.srcAccessMask =
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    stencil_to_resolve.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    stencil_to_resolve.oldLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    stencil_to_resolve.newLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    stencil_to_resolve.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    stencil_to_resolve.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    stencil_to_resolve.image = destination->resource.image;
    stencil_to_resolve.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(
        command_buffer,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0,
        nullptr, 1, &stencil_to_resolve);
  }

  // The title's explicit depth resolve is the persistent pre-clear scene-depth
  // resource. Copy only its depth aspect into the forward attachment; stencil
  // remains owned by the destination and is restored below when initialized.
  std::array<VkImageMemoryBarrier, 2> depth_copy_barriers{};
  depth_copy_barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  depth_copy_barriers[0].srcAccessMask =
      VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  depth_copy_barriers[0].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  depth_copy_barriers[0].oldLayout = source_texture->layout;
  depth_copy_barriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  depth_copy_barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  depth_copy_barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  depth_copy_barriers[0].image = source_texture->resource.image;
  depth_copy_barriers[0].subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(source_texture->aspect, 0, 1, 0, 1);
  depth_copy_barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  depth_copy_barriers[1].srcAccessMask =
      VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  depth_copy_barriers[1].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  depth_copy_barriers[1].oldLayout = destination->layout;
  depth_copy_barriers[1].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  depth_copy_barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  depth_copy_barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  depth_copy_barriers[1].image = destination->resource.image;
  depth_copy_barriers[1].subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr,
      uint32_t(depth_copy_barriers.size()), depth_copy_barriers.data());
  source_texture->layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  destination->layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

  VkImageCopy depth_copy{};
  depth_copy.srcSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  depth_copy.srcSubresource.layerCount = 1;
  depth_copy.dstSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  depth_copy.dstSubresource.layerCount = 1;
  depth_copy.extent = {destination->width, destination->height, 1};
  dfn.vkCmdCopyImage(
      command_buffer, source_texture->resource.image,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destination->resource.image,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &depth_copy);

  VkImageMemoryBarrier source_post_barrier{};
  source_post_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  source_post_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  source_post_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  source_post_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  source_post_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  source_post_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_post_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_post_barrier.image = source_texture->resource.image;
  source_post_barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(source_texture->aspect, 0, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
      0, 0, nullptr, 0, nullptr, 1, &source_post_barrier);
  source_texture->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

  if (destination_stencil_initialized) {
    VkImageMemoryBarrier destination_to_restore{};
    destination_to_restore.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    destination_to_restore.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    destination_to_restore.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    destination_to_restore.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    destination_to_restore.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    destination_to_restore.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_to_restore.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_to_restore.image = destination->resource.image;
    destination_to_restore.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
        &destination_to_restore);
    destination->layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

    VkBufferImageCopy stencil_copy{};
    stencil_copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
    stencil_copy.imageSubresource.layerCount = 1;
    stencil_copy.imageExtent = {destination->width, destination->height, 1};
    dfn.vkCmdCopyBufferToImage(
        command_buffer, destination->depth_handoff_stencil_scratch_buffer,
        destination->resource.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
        &stencil_copy);

    VkImageMemoryBarrier destination_restored{};
    destination_restored.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    destination_restored.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    destination_restored.dstAccessMask =
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    destination_restored.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    destination_restored.newLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    destination_restored.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_restored.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_restored.image = destination->resource.image;
    destination_restored.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        0, 0, nullptr, 0, nullptr, 1, &destination_restored);
    destination->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    REXLOG_INFO(
        "gta4-native-cause: point=explicit-depth-handoff-stencil-preserve "
        "frame={} cmd={} phase=restored transport=buffer "
        "destination={:08X}/{:08X} extent={}x{} bytes={}",
        submitted_frame, diagnostic_command_index_, destination->descriptor.handle,
        destination->descriptor.address, destination->width, destination->height,
        destination->depth_handoff_stencil_scratch_size);
  } else {
    VkImageMemoryBarrier destination_post_barrier{};
    destination_post_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    destination_post_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    destination_post_barrier.dstAccessMask =
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    destination_post_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    destination_post_barrier.newLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    destination_post_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_post_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_post_barrier.image = destination->resource.image;
    destination_post_barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(destination->aspect, 0, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        0, 0, nullptr, 0, nullptr, 1, &destination_post_barrier);
    destination->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  }

  ClaimSurfaceContent(
      *destination, handoff.destination, true, submitted_frame,
      native_command.render_phase,
      NativePlacementOwner::WriteKind::kExplicitDepthHandoff);
  RecordDepthStencilDiagnosticProbe(command_buffer, *destination, 36, 37);
  TraceNativeRendererEvent(
      "explicit-depth-handoff",
      fmt::format(
          "source-wrapper={:08X} source={:08X}/{:08X} source-texture={:08X}@{} "
          "destination-wrapper={:08X} destination={:08X}/{:08X} "
          "extent={}x{} source-samples={} destination-samples={} "
          "mode=copy depth=true stencil=false stencil-state={} "
          "destination-serial={} caller={:08X}",
          handoff.source_wrapper, handoff.source.handle, handoff.source.address,
          handoff.source_texture, native_command.depth_handoff_source->generation,
          handoff.destination_wrapper, handoff.destination.handle,
          handoff.destination.address, destination->width, destination->height,
          uint32_t(VK_SAMPLE_COUNT_1_BIT), uint32_t(destination->samples),
          destination_stencil_initialized ? "preserved-copy" : "initialized-zero",
          destination->materialized_serial, handoff.trace_caller));
  REXLOG_WARN(
      "gta4-native-cause: point=explicit-depth-handoff frame={} cmd={} result=ok "
      "source-wrapper={:08X} source={:08X}/{:08X} source-texture={:08X}@{} "
      "destination-wrapper={:08X} destination={:08X}/{:08X} extent={}x{} "
      "source-samples={} destination-samples={} mode=copy depth=true "
      "stencil=false stencil-state={} serial={}",
      submitted_frame, diagnostic_command_index_, handoff.source_wrapper,
      handoff.source.handle, handoff.source.address, handoff.source_texture,
      native_command.depth_handoff_source->generation,
      handoff.destination_wrapper, handoff.destination.handle,
      handoff.destination.address, destination->width, destination->height,
      uint32_t(VK_SAMPLE_COUNT_1_BIT), uint32_t(destination->samples),
      destination_stencil_initialized ? "preserved-copy" : "initialized-zero",
      destination->materialized_serial);
  return true;
}

bool Gta4NativeGraphicsSystem::PrepareSurfaceContent(
    VkCommandBuffer command_buffer, NativeSurfaceImage& surface,
    const SurfaceDescriptor& descriptor, bool depth, uint32_t submitted_frame,
    RenderPhase render_phase) {
  if (surface.is_reflection) {
    return true;
  }
  GuestSurfaceView view{};
  if (!DecodeGuestSurfaceView(descriptor, depth, view)) {
    return false;
  }
  if (HasCurrentPlacementContent(surface, descriptor, depth)) {
    return true;
  }
  const NativePlacementOwner* owner = FindPlacementOwner(descriptor, depth);
  if (!owner) {
    return true;
  }
  if (depth) {
    REXLOG_ERROR(
        "gta4-native-cause: point=placement-view-rejected frame={} cmd={} reason=depth-view "
        "source={:08X} destination={:08X} serial={}",
        submitted_frame, diagnostic_command_index_, owner->image->descriptor.handle,
        descriptor.handle, owner->serial);
    return false;
  }
  const bool materialized = RecordSurfaceMaterialization(command_buffer, *owner, surface, view);
  if (!materialized) {
    REXLOG_ERROR(
        "gta4-native-cause: point=placement-view-rejected frame={} cmd={} reason=recording "
        "source={:08X} destination={:08X} phase={} serial={}",
        submitted_frame, diagnostic_command_index_, owner->image->descriptor.handle,
        descriptor.handle, RenderPhaseName(render_phase), owner->serial);
  }
  return materialized;
}

bool Gta4NativeGraphicsSystem::RecordResolveClears(VkCommandBuffer command_buffer,
                                                   const NativeCommand& command,
                                                   const ResolveCommand& resolve,
                                                   uint32_t submitted_frame) {
  const bool clear_color = (resolve.flags & (1u << 8)) != 0;
  const bool clear_depth = (resolve.flags & (1u << 9)) != 0;
  if (!clear_color && !clear_depth) {
    return true;
  }

  auto clear_surface = [&](const SurfaceDescriptor& descriptor, bool depth) {
    NativeSurfaceImage* surface = GetOrCreateSurfaceImage(descriptor, depth);
    if (!surface || !PrepareSurfaceContent(command_buffer, *surface, descriptor, depth,
                                           submitted_frame, command.render_phase)) {
      return false;
    }
    const bool had_current_content = HasCurrentPlacementContent(*surface, descriptor, depth);
    if (depth && resolve.trace_origin == 3) {
      const NativePlacementOwner* owner = FindPlacementOwner(descriptor, true);
      RecordDepthStencilDiagnosticProbe(
          command_buffer, *surface, 25, 26, "title-clear-source-before",
          command.pipeline_state
              ? command.pipeline_state->depth_stencil_trace_wrapper
              : 0,
          0, owner);
      REXLOG_WARN(
          "gta4-native-cause: point=depth-lifecycle-probe-scheduled role=title-clear-source-before "
          "frame={} cmd={} wrapper={:08X} surface={:08X}/{:08X} samples={} "
          "host={}x{} provenance={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
          submitted_frame, diagnostic_command_index_,
          command.pipeline_state
              ? command.pipeline_state->depth_stencil_trace_wrapper
              : 0,
          surface->descriptor.handle, surface->descriptor.address,
          uint32_t(surface->samples), surface->width, surface->height,
          owner && owner->image ? owner->image->descriptor.handle : 0,
          owner ? owner->frame : 0,
          owner ? owner->command_index : SIZE_MAX,
          owner ? owner->serial : 0,
          owner ? uint32_t(owner->render_phase) : 0,
          owner ? uint32_t(owner->write_kind) : 0);
    }
    const int32_t logical_width = int32_t(surface->logical_width);
    const int32_t logical_height = int32_t(surface->logical_height);
    const int32_t requested_left = resolve.source_rectangle_valid
                                       ? resolve.source_rectangle.left
                                       : 0;
    const int32_t requested_top = resolve.source_rectangle_valid
                                      ? resolve.source_rectangle.top
                                      : 0;
    const int32_t requested_right = resolve.source_rectangle_valid
                                        ? resolve.source_rectangle.right
                                        : logical_width;
    const int32_t requested_bottom = resolve.source_rectangle_valid
                                         ? resolve.source_rectangle.bottom
                                         : logical_height;
    const int32_t logical_left = std::clamp(requested_left, 0, logical_width);
    const int32_t logical_top = std::clamp(requested_top, 0, logical_height);
    const int32_t logical_right = std::clamp(requested_right, logical_left, logical_width);
    const int32_t logical_bottom =
        std::clamp(requested_bottom, logical_top, logical_height);
    const int32_t left =
        ScaleCoordinateFloor(logical_left, surface->logical_width, surface->width);
    const int32_t top =
        ScaleCoordinateFloor(logical_top, surface->logical_height, surface->height);
    const int32_t right =
        ScaleCoordinateCeil(logical_right, surface->logical_width, surface->width);
    const int32_t bottom =
        ScaleCoordinateCeil(logical_bottom, surface->logical_height, surface->height);
    if (right <= left || bottom <= top) {
      return false;
    }

    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const auto& dfn = vulkan_provider->vulkan_device()->functions();
    const VkImageLayout attachment_layout =
        depth ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
              : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    if (surface->layout != attachment_layout) {
      VkImageMemoryBarrier barrier{};
      barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
                              VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
      barrier.dstAccessMask = depth ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                                         VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
                                   : VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                                         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
      barrier.oldLayout = surface->layout;
      barrier.newLayout = attachment_layout;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = surface->resource.image;
      barrier.subresourceRange =
          ui::vulkan::util::InitializeSubresourceRange(surface->aspect, 0, 1, 0, 1);
      dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                               depth ? VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                                           VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
                                     : VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                               0, 0, nullptr, 0, nullptr, 1, &barrier);
      surface->layout = attachment_layout;
    }

    VkRenderingAttachmentInfo attachment{};
    attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    attachment.imageView = surface->resource.view;
    attachment.imageLayout = attachment_layout;
    attachment.loadOp = had_current_content ? VK_ATTACHMENT_LOAD_OP_LOAD
                                            : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    VkRenderingInfo rendering_info{};
    rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    rendering_info.renderArea.extent = {surface->width, surface->height};
    rendering_info.layerCount = 1;
    if (depth && resolve.trace_origin == 3) {
      rendering_info.pDepthAttachment = &attachment;
      if (surface->aspect & VK_IMAGE_ASPECT_STENCIL_BIT) {
        rendering_info.pStencilAttachment = &attachment;
      }
    } else {
      rendering_info.colorAttachmentCount = 1;
      rendering_info.pColorAttachments = &attachment;
    }
    dfn.vkCmdBeginRendering(command_buffer, &rendering_info);

    VkClearAttachment clear_attachment{};
    if (depth) {
      clear_attachment.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      if (surface->aspect & VK_IMAGE_ASPECT_STENCIL_BIT) {
        clear_attachment.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
      }
      clear_attachment.clearValue.depthStencil.depth =
          float(std::bit_cast<double>(resolve.clear_depth_bits));
      clear_attachment.clearValue.depthStencil.stencil = resolve.clear_stencil;
    } else {
      clear_attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      for (uint32_t component = 0; component < 4; ++component) {
        clear_attachment.clearValue.color.float32[component] =
            std::bit_cast<float>(resolve.clear_color_bits[component]);
      }
    }
    VkClearRect clear_rectangle{};
    clear_rectangle.rect.offset = {left, top};
    clear_rectangle.rect.extent = {uint32_t(right - left), uint32_t(bottom - top)};
    clear_rectangle.baseArrayLayer = 0;
    clear_rectangle.layerCount = 1;
    dfn.vkCmdClearAttachments(command_buffer, 1, &clear_attachment, 1, &clear_rectangle);
    dfn.vkCmdEndRendering(command_buffer);
    surface->ever_written = true;
    ClaimSurfaceContent(*surface, descriptor, depth, submitted_frame, command.render_phase,
                        NativePlacementOwner::WriteKind::kResolveClear);
    if (depth) {
      const NativePlacementOwner* owner = FindPlacementOwner(descriptor, true);
      RecordDepthStencilDiagnosticProbe(
          command_buffer, *surface, 27, 28, "title-clear-source-after",
          command.pipeline_state
              ? command.pipeline_state->depth_stencil_trace_wrapper
              : 0,
          0, owner);
      REXLOG_WARN(
          "gta4-native-cause: point=depth-lifecycle-probe-scheduled role=title-clear-source-after "
          "frame={} cmd={} wrapper={:08X} surface={:08X}/{:08X} samples={} "
          "host={}x{} provenance={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
          submitted_frame, diagnostic_command_index_,
          command.pipeline_state
              ? command.pipeline_state->depth_stencil_trace_wrapper
              : 0,
          surface->descriptor.handle, surface->descriptor.address,
          uint32_t(surface->samples), surface->width, surface->height,
          owner && owner->image ? owner->image->descriptor.handle : 0,
          owner ? owner->frame : 0,
          owner ? owner->command_index : SIZE_MAX,
          owner ? owner->serial : 0,
          owner ? uint32_t(owner->render_phase) : 0,
          owner ? uint32_t(owner->write_kind) : 0);
    }
    TraceNativeRendererEvent(
        "resolve-clear-applied",
        fmt::format(
            "handle={:08X} placement={} depth={} rect={},{},{},{} clear={:08X},{:08X},"
            "{:08X},{:08X}:{:016X}:{} origin={} caller={:08X} owner={:08X}",
            descriptor.handle, descriptor.address & 0x7FFu, depth, logical_left, logical_top,
            logical_right, logical_bottom, resolve.clear_color_bits[0],
            resolve.clear_color_bits[1], resolve.clear_color_bits[2],
            resolve.clear_color_bits[3], resolve.clear_depth_bits, resolve.clear_stencil,
            resolve.trace_origin, resolve.trace_caller, resolve.trace_owner));
    return true;
  };

  bool result = true;
  if (clear_color) {
    const uint32_t source_index = resolve.flags & 7u;
    const SurfaceDescriptor& color_descriptor =
        source_index < kRenderTargetCount && command.pipeline_state
            ? command.pipeline_state->render_targets[source_index]
            : resolve.source;
    result &= clear_surface(color_descriptor, false);
  }
  if (clear_depth) {
    if (!command.pipeline_state || !command.pipeline_state->depth_stencil.handle) {
      result = false;
    } else {
      result &= clear_surface(command.pipeline_state->depth_stencil, true);
    }
  }
  return result;
}

bool Gta4NativeGraphicsSystem::RecordResolve(VkCommandBuffer command_buffer,
                                             const NativeCommand& command,
                                             uint32_t submitted_frame) {
  SCOPE_profile_cpu_i("gpu", "GTA4 Native RecordResolve");
  ResolveCommand resolve;
  std::memcpy(&resolve, command.bytes.data(), sizeof(resolve));
  resolve.flags = NormalizeResolveSampleFlags(resolve.flags, resolve.source.sample_type);
  NativeSurfaceImage* source = nullptr;
  NativeSurfaceImage* content_source = nullptr;
  NativeTextureImage* destination = nullptr;
  auto log_result = [&](const char* result, const char* reason) {
    TraceNativeRendererEvent(
        "resolve-result",
        fmt::format(
            "result={} operation={} source={:08X}:{:08X}:vk{}:{}x{}:layout{}:written{} "
            "destination={:08X}@{}:vk{}:{}x{}:layout{} "
            "origin={} caller={:08X} owner={:08X}",
            result, reason, resolve.source.handle, resolve.source.address,
            source ? uint32_t(source->format) : uint32_t(VK_FORMAT_UNDEFINED),
            source ? source->width : 0, source ? source->height : 0,
            source ? uint32_t(source->layout) : 0, source ? source->ever_written : false,
            resolve.destination_texture,
            command.resolve_destination ? command.resolve_destination->generation : 0,
            destination ? uint32_t(destination->format) : uint32_t(VK_FORMAT_UNDEFINED),
            destination ? destination->width : 0, destination ? destination->height : 0,
            destination ? uint32_t(destination->layout) : 0, resolve.trace_origin,
            resolve.trace_caller, resolve.trace_owner));
    if (!ShouldLogDiagnosticFrame(submitted_frame)) {
      return;
    }
    REXLOG_WARN(
        "gta4-native-resolve: frame={} result={} reason={} source={:08X}/{:08X} "
        "source-format={} source-guest-size={}x{} source-host-size={}x{} "
        "source-layout={} source-written={} "
        "destination={:08X}@{} destination-format={} destination-size={}x{} "
        "destination-layout={} flags={:08X} level={} slice={} "
        "source-rect={}({},{},{},{}) destination-point={}({},{}) "
        "origin={} caller={:08X} owner={:08X}",
        submitted_frame, result, reason, resolve.source.handle, resolve.source.address,
        source ? uint32_t(source->format) : uint32_t(VK_FORMAT_UNDEFINED), resolve.source.width,
        resolve.source.height, source ? source->width : 0, source ? source->height : 0,
        source ? uint32_t(source->layout) : 0,
        source ? source->ever_written : false, resolve.destination_texture,
        command.resolve_destination ? command.resolve_destination->generation : 0,
        destination ? uint32_t(destination->format) : uint32_t(VK_FORMAT_UNDEFINED),
        command.resolve_destination ? command.resolve_destination->info.width + 1 : 0,
        command.resolve_destination ? command.resolve_destination->info.height + 1 : 0,
        destination ? uint32_t(destination->layout) : 0, resolve.flags,
        resolve.destination_level, resolve.destination_slice_or_face,
        bool(resolve.source_rectangle_valid), resolve.source_rectangle.left,
        resolve.source_rectangle.top, resolve.source_rectangle.right, resolve.source_rectangle.bottom,
        bool(resolve.destination_point_valid), resolve.destination_point.x,
        resolve.destination_point.y, resolve.trace_origin, resolve.trace_caller,
        resolve.trace_owner);
    std::fprintf(
        stderr,
        "[ResolveE2E] frame=%u result=%s operation=%s source=%08X/%08X "
        "source-format=%u source-size=%ux%u source-host=%ux%u source-sample=%u "
        "source-base=%08X source-dim=%08X source-written=%u source-image=%p "
        "destination=%08X@%llu destination-format=%u destination-size=%ux%u "
        "flags=%08X level=%u slice=%u rect=%u:%d,%d,%d,%d point=%u:%d,%d\n",
        submitted_frame, result, reason, resolve.source.handle, resolve.source.address,
        source ? uint32_t(source->format) : uint32_t(VK_FORMAT_UNDEFINED),
        resolve.source.width, resolve.source.height, source ? source->width : 0,
        source ? source->height : 0, resolve.source.sample_type, resolve.source.base,
        resolve.source.packed_dimensions, unsigned(source ? source->ever_written : false),
        static_cast<void*>(source),
        resolve.destination_texture,
        static_cast<unsigned long long>(
            command.resolve_destination ? command.resolve_destination->generation : 0),
        destination ? uint32_t(destination->format) : uint32_t(VK_FORMAT_UNDEFINED),
        command.resolve_destination ? command.resolve_destination->info.width + 1 : 0,
        command.resolve_destination ? command.resolve_destination->info.height + 1 : 0,
        resolve.flags, resolve.destination_level, resolve.destination_slice_or_face,
        unsigned(resolve.source_rectangle_valid), resolve.source_rectangle.left,
        resolve.source_rectangle.top, resolve.source_rectangle.right,
        resolve.source_rectangle.bottom, unsigned(resolve.destination_point_valid),
        resolve.destination_point.x, resolve.destination_point.y);
    std::fflush(stderr);
  };
  if (!command.resolve_destination) {
    log_result("fail", "missing-destination-resource");
    return false;
  }
  const bool depth = (resolve.flags & 7) == 4;
  source = GetOrCreateSurfaceImage(resolve.source, depth);
  auto destination_entry = native_texture_images_.find(command.resolve_destination->generation);
  if (!source || destination_entry == native_texture_images_.end()) {
    log_result("fail", !source ? "source-image" : "destination-image");
    if (source) {
      RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    }
    return false;
  }
  destination = destination_entry->second.get();
  GuestSurfaceView requested_view{};
  if (!DecodeGuestSurfaceView(resolve.source, depth, requested_view)) {
    log_result("fail", "guest-source-view");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }
  GuestSurfaceView content_view = requested_view;
  content_source = source;
  const NativePlacementOwner* placement_owner = FindPlacementOwner(resolve.source, depth);
  if (!source->is_reflection && placement_owner) {
    content_source = placement_owner->image;
    content_view = placement_owner->view;
  }
  const xenos::CopySampleSelect sample_select = SanitizeGuestCopySampleSelect(
      DecodeResolveSampleSelect(resolve.flags), requested_view.msaa_samples, depth);
  const bool source_content_available =
      source->is_reflection ? source->ever_written
                            : content_source && content_source->ever_written;
  if (!source_content_available) {
    std::string same_address_aliases;
    bool has_written_same_address_alias = false;
    for (const auto& candidate : native_surface_images_) {
      if (!candidate || candidate.get() == source || candidate->aspect != source->aspect ||
          candidate->descriptor.address != resolve.source.address) {
        continue;
      }
      same_address_aliases +=
          fmt::format("{}{:08X}:guest={}x{}:host={}x{}:format={}:samples={}:layout={}:written={}",
                      same_address_aliases.empty() ? "" : ",", candidate->descriptor.handle,
                      candidate->descriptor.width, candidate->descriptor.height, candidate->width,
                      candidate->height, uint32_t(candidate->format), uint32_t(candidate->samples),
                      uint32_t(candidate->layout), candidate->ever_written);
      has_written_same_address_alias |= candidate->ever_written;
    }
    static std::set<std::pair<uint32_t, uint32_t>> logged_unwritten_resolves;
    if (logged_unwritten_resolves
            .emplace(resolve.source.handle, resolve.destination_texture)
            .second) {
      const auto reflection = reflection_resources_.find(resolve.source.handle);
      const bool registered_reflection = reflection != reflection_resources_.end();
      REXLOG_ERROR(
          "gta4-native-cause: point=first-unwritten-resolve frame={} cmd={} "
          "source={:08X}/{:08X} guest={}x{} logical={}x{} host={}x{} layout={} "
          "reflection={} family={} role={} destination={:08X}@{} format={} size={}x{} "
          "written-alias={} aliases=[{}]",
          submitted_frame, diagnostic_command_index_, resolve.source.handle,
          resolve.source.address, resolve.source.width, resolve.source.height,
          source->logical_width, source->logical_height, source->width, source->height,
          uint32_t(source->layout), registered_reflection,
          registered_reflection ? uint32_t(reflection->second.family) : 0,
          registered_reflection ? uint32_t(reflection->second.role) : 0,
          resolve.destination_texture,
          command.resolve_destination ? command.resolve_destination->generation : 0,
          uint32_t(destination->format), destination->width, destination->height,
          has_written_same_address_alias, same_address_aliases);
    }
  }
  if (!source_content_available) {
    log_result("fail", "source-placement-unwritten");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }
  if (resolve.destination_level >= destination->mip_levels ||
      resolve.destination_slice_or_face != 0) {
    REXLOG_ERROR("gta4-native: unsupported resolve destination level {} slice {}",
                 resolve.destination_level, resolve.destination_slice_or_face);
    log_result("fail", "destination-subresource");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }

  const int32_t logical_source_width = int32_t(source->logical_width);
  const int32_t logical_source_height = int32_t(source->logical_height);
  int32_t requested_left = resolve.source_rectangle_valid ? resolve.source_rectangle.left : 0;
  int32_t requested_top = resolve.source_rectangle_valid ? resolve.source_rectangle.top : 0;
  int32_t requested_right =
      resolve.source_rectangle_valid ? resolve.source_rectangle.right : logical_source_width;
  int32_t requested_bottom =
      resolve.source_rectangle_valid ? resolve.source_rectangle.bottom : logical_source_height;

  const int32_t logical_source_left =
      std::clamp(requested_left, 0, logical_source_width);
  const int32_t logical_source_top =
      std::clamp(requested_top, 0, logical_source_height);
  const int32_t logical_source_right =
      std::clamp(requested_right, logical_source_left, logical_source_width);
  const int32_t logical_source_bottom =
      std::clamp(requested_bottom, logical_source_top, logical_source_height);
  const int32_t source_left =
      ScaleCoordinateFloor(logical_source_left, source->logical_width, source->width);
  const int32_t source_top =
      ScaleCoordinateFloor(logical_source_top, source->logical_height, source->height);
  const int32_t source_right =
      ScaleCoordinateCeil(logical_source_right, source->logical_width, source->width);
  const int32_t source_bottom =
      ScaleCoordinateCeil(logical_source_bottom, source->logical_height, source->height);
  if (source_right <= source_left || source_bottom <= source_top) {
    log_result("fail", "empty-source-rectangle");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }

  const uint32_t logical_mip_width =
      CalculateNativeMipExtent(destination->logical_width, resolve.destination_level);
  const uint32_t logical_mip_height =
      CalculateNativeMipExtent(destination->logical_height, resolve.destination_level);
  const uint32_t mip_width =
      CalculateNativeMipExtent(destination->width, resolve.destination_level);
  const uint32_t mip_height =
      CalculateNativeMipExtent(destination->height, resolve.destination_level);
  const int32_t logical_destination_x =
      resolve.destination_point_valid ? resolve.destination_point.x : 0;
  const int32_t logical_destination_y =
      resolve.destination_point_valid ? resolve.destination_point.y : 0;
  if (logical_destination_x < 0 || logical_destination_y < 0 ||
      uint32_t(logical_destination_x) >= logical_mip_width ||
      uint32_t(logical_destination_y) >= logical_mip_height) {
    log_result("fail", "destination-point");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }
  const int32_t destination_x =
      ScaleCoordinateFloor(logical_destination_x, logical_mip_width, mip_width);
  const int32_t destination_y =
      ScaleCoordinateFloor(logical_destination_y, logical_mip_height, mip_height);
  const uint32_t copy_width =
      std::min(uint32_t(source_right - source_left), mip_width - uint32_t(destination_x));
  const uint32_t copy_height =
      std::min(uint32_t(source_bottom - source_top), mip_height - uint32_t(destination_y));
  if (!copy_width || !copy_height) {
    log_result("fail", "empty-copy-extent");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }

  enum class ResolveOperation {
    kCopy,
    kBlit,
    kMultisampleResolve,
    kConvert,
    kDepthConvert,
  };
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  VkFormatProperties source_properties{};
  VkFormatProperties destination_properties{};
  const auto& ifn = vulkan_device->vulkan_instance()->functions();
  ifn.vkGetPhysicalDeviceFormatProperties(vulkan_device->physical_device(), content_source->format,
                                          &source_properties);
  ifn.vkGetPhysicalDeviceFormatProperties(vulkan_device->physical_device(), destination->format,
                                          &destination_properties);
  const bool transfer_supported =
      (source_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT) &&
      (destination_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_DST_BIT);
  const bool blit_supported =
      (source_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT) &&
      (destination_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);
  ResolveOperation operation;
  const bool matching_aspects = content_source->aspect == destination->aspect;
  const bool color_resolve = content_source->aspect == VK_IMAGE_ASPECT_COLOR_BIT &&
                             destination->aspect == VK_IMAGE_ASPECT_COLOR_BIT;
  const bool conversion_supported =
      color_resolve &&
      (source_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) &&
      (destination_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT);
  const bool depth_conversion_supported =
      depth && content_source->samples != VK_SAMPLE_COUNT_1_BIT &&
      (content_source->aspect & VK_IMAGE_ASPECT_DEPTH_BIT) &&
      (destination->aspect & VK_IMAGE_ASPECT_DEPTH_BIT) && content_source->sampled_view &&
      (source_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) &&
      (destination_properties.optimalTilingFeatures &
       VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  // Color resolves with guest or host multisampling need the programmable path. Besides
  // preserving the Xenos sample selector for ordinary guest surfaces, this avoids relying on
  // driver-internal transfer images for high-resolution reflection resolves. Those reflection
  // images may have host MSAA even when the guest view is single-sampled.
  const bool programmable_color_resolve =
      !depth && (content_source != source ||
                 content_source->samples != VK_SAMPLE_COUNT_1_BIT ||
                 requested_view.msaa_samples != xenos::MsaaSamples::k1X);
  if (depth_conversion_supported) {
    operation = ResolveOperation::kDepthConvert;
  } else if (programmable_color_resolve && conversion_supported) {
    operation = ResolveOperation::kConvert;
  } else if (transfer_supported && content_source->samples == VK_SAMPLE_COUNT_1_BIT &&
             matching_aspects && content_source->format == destination->format) {
    operation = ResolveOperation::kCopy;
  } else if (blit_supported && content_source->samples == VK_SAMPLE_COUNT_1_BIT && !depth &&
             color_resolve) {
    operation = ResolveOperation::kBlit;
  } else if (transfer_supported && content_source->samples != VK_SAMPLE_COUNT_1_BIT && !depth &&
             color_resolve && content_source->format == destination->format) {
    operation = ResolveOperation::kMultisampleResolve;
  } else if (conversion_supported) {
    operation = ResolveOperation::kConvert;
  } else {
    REXLOG_ERROR("gta4-native: unsupported resolve formats {} -> {} samples {} depth {}",
                 uint32_t(content_source->format), uint32_t(destination->format),
                 uint32_t(content_source->samples), depth);
    log_result("fail", "unsupported-operation");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }

  if (source->is_reflection || destination->is_reflection) {
    const char* reflection_operation_name =
        operation == ResolveOperation::kCopy                 ? "copy"
        : operation == ResolveOperation::kBlit               ? "blit"
        : operation == ResolveOperation::kMultisampleResolve ? "msaa"
        : operation == ResolveOperation::kDepthConvert       ? "depth-convert"
                                                              : "convert";
    TraceNativeRendererEvent(
        "reflection-resolve-geometry",
        fmt::format(
            "family={} role={} source={:08X}/{:08X} source-logical={}x{} "
            "source-physical={}x{} source-samples={} source-layout={} "
            "destination={:08X}@{} level={} slice={} guest-mips={} physical-mips={} "
            "mip-logical={}x{} mip-physical={}x{} "
            "requested-source={},{},{},{} physical-source={},{},{},{} "
            "requested-destination={},{} physical-destination={},{} copy={}x{} operation={}",
            uint32_t(destination->reflection.family), uint32_t(destination->reflection.role),
            resolve.source.handle, resolve.source.address, source->logical_width,
            source->logical_height, source->width, source->height,
            uint32_t(content_source->samples), uint32_t(content_source->layout),
            resolve.destination_texture,
            command.resolve_destination ? command.resolve_destination->generation : 0,
            resolve.destination_level, resolve.destination_slice_or_face,
            destination->guest_mip_levels, destination->mip_levels, logical_mip_width,
            logical_mip_height, mip_width, mip_height, logical_source_left,
            logical_source_top, logical_source_right, logical_source_bottom, source_left,
            source_top, source_right, source_bottom, logical_destination_x,
            logical_destination_y, destination_x, destination_y, copy_width, copy_height,
            reflection_operation_name));
  }

  if (operation == ResolveOperation::kConvert) {
    if (REXCVAR_GET(gta4_trace_startup_content)) {
      static std::atomic<uint64_t> conversion_count{0};
      const uint64_t count = ++conversion_count;
      if (count <= 32 || !(count % 1024)) {
        REXLOG_INFO(
            "gta4-native-diag: resolve conversion #{} formats {} -> {} samples {} extent {}x{}",
            count, uint32_t(content_source->format), uint32_t(destination->format),
            uint32_t(content_source->samples), copy_width, copy_height);
      }
    }
    GuestSurfaceView conversion_content_view = content_view;
    GuestSurfaceView conversion_requested_view = requested_view;
    xenos::CopySampleSelect conversion_sample_select = sample_select;
    if (source->is_reflection) {
      const xenos::MsaaSamples host_samples =
          ConvertHostSamplesToGuestSamples(content_source->samples);
      if (host_samples != conversion_content_view.msaa_samples) {
        // Reflection MSAA is a host quality override, not a second guest placement view. Resolve
        // directly in host pixel/sample space and consume every host sample so the override is
        // transparent to the guest's single-sampled surface description.
        conversion_content_view.msaa_samples = host_samples;
        conversion_requested_view.msaa_samples = host_samples;
        conversion_sample_select = ResolveAllHostSamples(host_samples);
      }
    }
    bool converted = RecordResolveConversion(
        command_buffer, *content_source, *destination, resolve.destination_level, source_left,
        source_top, destination_x, destination_y, copy_width, copy_height,
        conversion_content_view, conversion_requested_view, conversion_sample_select);
    if (converted && resolve.destination_level + 1 == destination->guest_mip_levels) {
      converted = GenerateReflectionTailMips(command_buffer, *destination);
    }
    const bool cleared = RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    TraceNativeRendererEvent(
        "placement-resolve",
        fmt::format(
            "source={:08X} owner={:08X} requested-samples={} owner-samples={} select={} "
            "serial={} converted={} cleared={}",
            resolve.source.handle, content_source->descriptor.handle,
            uint32_t(requested_view.msaa_samples), uint32_t(content_view.msaa_samples),
            uint32_t(sample_select), placement_owner ? placement_owner->serial : 0, converted,
            cleared));
    log_result(converted && cleared ? "ok" : "fail",
               !converted ? "conversion-recording" : !cleared ? "resolve-clear" : "convert");
    return converted && cleared;
  }

  if (operation == ResolveOperation::kDepthConvert) {
    const NativePlacementOwner* source_owner =
        FindPlacementOwner(content_source->descriptor, true);
    if (resolve.trace_origin == 3) {
      RecordDepthStencilDiagnosticProbe(
          command_buffer, *content_source, 38, 39,
          "explicit-title-resolve-source-before", resolve.trace_source_wrapper, 0,
          source_owner);
      REXLOG_WARN(
          "gta4-native-cause: point=depth-lifecycle-probe-scheduled role=explicit-resolve-source-before "
          "frame={} cmd={} source-wrapper={:08X} surface={:08X}/{:08X} "
          "texture={:08X} destination-wrapper={:08X} samples={} host={}x{} "
          "provenance={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
          submitted_frame, diagnostic_command_index_, resolve.trace_source_wrapper,
          content_source->descriptor.handle, content_source->descriptor.address,
          resolve.destination_texture, resolve.trace_owner,
          uint32_t(content_source->samples), content_source->width,
          content_source->height,
          source_owner && source_owner->image
              ? source_owner->image->descriptor.handle
              : 0,
          source_owner ? source_owner->frame : 0,
          source_owner ? source_owner->command_index : SIZE_MAX,
          source_owner ? source_owner->serial : 0,
          source_owner ? uint32_t(source_owner->render_phase) : 0,
          source_owner ? uint32_t(source_owner->write_kind) : 0);
    }
    const bool converted = RecordDepthResolveConversion(
        command_buffer, *content_source, *destination, resolve.destination_level,
        source_left, source_top, destination_x, destination_y, copy_width, copy_height,
        content_view, requested_view, sample_select);
    if (converted && resolve.trace_origin == 3) {
      RecordDepthStencilDiagnosticProbe(
          command_buffer, *destination, 29, 30,
          "explicit-title-resolve-destination-after", resolve.trace_owner,
          command.resolve_destination ? command.resolve_destination->generation : 0,
          nullptr);
      REXLOG_WARN(
          "gta4-native-cause: point=depth-lifecycle-probe-scheduled role=explicit-resolve-destination-after "
          "frame={} cmd={} source-wrapper={:08X} surface={:08X}/{:08X} "
          "texture={:08X} destination-wrapper={:08X} samples={} host={}x{} "
          "provenance=explicit-title-depth-resolve",
          submitted_frame, diagnostic_command_index_,
          resolve.trace_source_wrapper, content_source->descriptor.handle,
          content_source->descriptor.address, resolve.destination_texture,
          resolve.trace_owner, uint32_t(VK_SAMPLE_COUNT_1_BIT),
          destination->width, destination->height);
    }
    const bool cleared = RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    TraceNativeRendererEvent(
        "depth-placement-resolve",
        fmt::format(
            "source={:08X} owner={:08X} requested-samples={} owner-samples={} select={} "
            "serial={} converted={} cleared={}",
            resolve.source.handle, content_source->descriptor.handle,
            uint32_t(requested_view.msaa_samples), uint32_t(content_view.msaa_samples),
            uint32_t(sample_select), placement_owner ? placement_owner->serial : 0,
            converted, cleared));
    log_result(converted && cleared ? "ok" : "fail",
               !converted ? "depth-conversion-recording"
                          : !cleared ? "resolve-clear" : "depth-convert");
    return converted && cleared;
  }

  const auto& dfn = vulkan_device->functions();
  std::array<VkImageMemoryBarrier, 2> barriers{};
  VkImageMemoryBarrier& source_barrier = barriers[0];
  source_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  source_barrier.srcAccessMask =
      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
      VK_ACCESS_TRANSFER_WRITE_BIT;
  source_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  source_barrier.oldLayout = content_source->layout;
  source_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  source_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_barrier.image = content_source->resource.image;
  source_barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(content_source->aspect, 0, 1, 0, 1);

  VkImageMemoryBarrier& destination_barrier = barriers[1];
  destination_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  destination_barrier.srcAccessMask =
      destination->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ? VK_ACCESS_SHADER_READ_BIT
                                                                      : 0;
  destination_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  destination_barrier.oldLayout = destination->layout;
  destination_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  destination_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.image = destination->resource.image;
  destination_barrier.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      destination->aspect, resolve.destination_level, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr,
                           uint32_t(barriers.size()), barriers.data());

  VkImageSubresourceLayers source_layers{};
  source_layers.aspectMask = content_source->aspect;
  source_layers.layerCount = 1;
  VkImageSubresourceLayers destination_layers{};
  destination_layers.aspectMask = destination->aspect;
  destination_layers.mipLevel = resolve.destination_level;
  destination_layers.layerCount = 1;

  switch (operation) {
    case ResolveOperation::kCopy: {
      VkImageCopy copy{};
      copy.srcSubresource = source_layers;
      copy.srcOffset = {source_left, source_top, 0};
      copy.dstSubresource = destination_layers;
      copy.dstOffset = {destination_x, destination_y, 0};
      copy.extent = {copy_width, copy_height, 1};
      dfn.vkCmdCopyImage(command_buffer, content_source->resource.image,
                         VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destination->resource.image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
      break;
    }
    case ResolveOperation::kBlit: {
      VkImageBlit blit{};
      blit.srcSubresource = source_layers;
      blit.srcOffsets[0] = {source_left, source_top, 0};
      blit.srcOffsets[1] = {source_left + int32_t(copy_width), source_top + int32_t(copy_height),
                            1};
      blit.dstSubresource = destination_layers;
      blit.dstOffsets[0] = {destination_x, destination_y, 0};
      blit.dstOffsets[1] = {destination_x + int32_t(copy_width),
                            destination_y + int32_t(copy_height), 1};
      dfn.vkCmdBlitImage(command_buffer, content_source->resource.image,
                         VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destination->resource.image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_NEAREST);
      break;
    }
    case ResolveOperation::kMultisampleResolve: {
      VkImageResolve image_resolve{};
      image_resolve.srcSubresource = source_layers;
      image_resolve.srcOffset = {source_left, source_top, 0};
      image_resolve.dstSubresource = destination_layers;
      image_resolve.dstOffset = {destination_x, destination_y, 0};
      image_resolve.extent = {copy_width, copy_height, 1};
      dfn.vkCmdResolveImage(command_buffer, content_source->resource.image,
                            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destination->resource.image,
                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &image_resolve);
      break;
    }
    case ResolveOperation::kConvert:
    case ResolveOperation::kDepthConvert:
      break;
  }

  destination_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  destination_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  destination_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  destination_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
      nullptr, 1, &destination_barrier);
  content_source->layout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  destination->layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  if (resolve.destination_level + 1 == destination->guest_mip_levels &&
      !GenerateReflectionTailMips(command_buffer, *destination)) {
    log_result("fail", "reflection-tail-mips");
    RecordResolveClears(command_buffer, command, resolve, submitted_frame);
    return false;
  }
  const char* operation_name = operation == ResolveOperation::kCopy
                                   ? "copy"
                                   : operation == ResolveOperation::kBlit ? "blit" : "msaa";
  const bool cleared = RecordResolveClears(command_buffer, command, resolve, submitted_frame);
  log_result(cleared ? "ok" : "fail", cleared ? operation_name : "resolve-clear");
  return cleared;
}

bool Gta4NativeGraphicsSystem::GenerateReflectionTailMips(
    VkCommandBuffer command_buffer, NativeTextureImage& destination) {
  if (!destination.is_reflection ||
      destination.reflection.family != ReflectionFamily::kEnvironment ||
      destination.reflection.role != ReflectionRole::kColor ||
      destination.guest_mip_levels >= destination.mip_levels) {
    return true;
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  VkFormatProperties properties{};
  vulkan_device->vulkan_instance()->functions().vkGetPhysicalDeviceFormatProperties(
      vulkan_device->physical_device(), destination.format, &properties);
  constexpr VkFormatFeatureFlags kRequiredFeatures =
      VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
  if ((properties.optimalTilingFeatures & kRequiredFeatures) != kRequiredFeatures) {
    REXLOG_ERROR(
        "gta4-native-reflection: cannot filter native tail mips format={} features={:08X}",
        uint32_t(destination.format), uint32_t(properties.optimalTilingFeatures));
    return false;
  }

  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();
  const VkPipeline pipeline = GetOrCreateReflectionMipPipeline(destination.format);
  if (!pipeline || !frame_descriptor_pool_ ||
      !resolve_conversion_descriptor_set_layout_ ||
      !resolve_conversion_pipeline_layout_) {
    return false;
  }

  for (uint32_t destination_level = destination.guest_mip_levels;
       destination_level < destination.mip_levels; ++destination_level) {
    const uint32_t source_level = destination_level - 1;
    const VkImageView source_view =
        GetOrCreateTextureMipView(destination, source_level);
    const VkImageView destination_view =
        GetOrCreateTextureMipView(destination, destination_level);
    if (!source_view || !destination_view) {
      return false;
    }

    VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
    VkDescriptorSetAllocateInfo descriptor_allocate_info{};
    descriptor_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_allocate_info.descriptorPool = frame_descriptor_pool_;
    descriptor_allocate_info.descriptorSetCount = 1;
    descriptor_allocate_info.pSetLayouts =
        &resolve_conversion_descriptor_set_layout_;
    if (dfn.vkAllocateDescriptorSets(device, &descriptor_allocate_info,
                                     &descriptor_set) != VK_SUCCESS) {
      return false;
    }
    VkDescriptorImageInfo descriptor_image{};
    descriptor_image.imageView = source_view;
    descriptor_image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    VkWriteDescriptorSet descriptor_write{};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = descriptor_set;
    descriptor_write.dstBinding = 0;
    descriptor_write.descriptorCount = 1;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptor_write.pImageInfo = &descriptor_image;
    dfn.vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);

    VkImageMemoryBarrier destination_barrier{};
    destination_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    destination_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    destination_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    destination_barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    destination_barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    destination_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    destination_barrier.image = destination.resource.image;
    destination_barrier.subresourceRange =
        ui::vulkan::util::InitializeSubresourceRange(
            destination.aspect, destination_level, 1, 0, 1);
    dfn.vkCmdPipelineBarrier(
        command_buffer,
        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0,
        nullptr, 1, &destination_barrier);

    const uint32_t mip_width =
        std::max(1u, destination.width >> destination_level);
    const uint32_t mip_height =
        std::max(1u, destination.height >> destination_level);
    VkRenderingAttachmentInfo color_attachment{};
    color_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    color_attachment.imageView = destination_view;
    color_attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    VkRenderingInfo rendering_info{};
    rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    rendering_info.renderArea.extent = {mip_width, mip_height};
    rendering_info.layerCount = 1;
    rendering_info.colorAttachmentCount = 1;
    rendering_info.pColorAttachments = &color_attachment;
    dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
    VkViewport viewport{};
    viewport.width = float(mip_width);
    viewport.height = float(mip_height);
    viewport.maxDepth = 1.0f;
    dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    VkRect2D scissor{};
    scissor.extent = {mip_width, mip_height};
    dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline);
    dfn.vkCmdBindDescriptorSets(
        command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        resolve_conversion_pipeline_layout_, 0, 1, &descriptor_set, 0, nullptr);
    NativeReflectionMipConstants constants{};
    constants.destination_level = destination_level;
    dfn.vkCmdPushConstants(command_buffer,
                           resolve_conversion_pipeline_layout_,
                           VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(constants),
                           &constants);
    dfn.vkCmdDraw(command_buffer, 3, 1, 0, 0);
    dfn.vkCmdEndRendering(command_buffer);

    destination_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    destination_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    destination_barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    destination_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    dfn.vkCmdPipelineBarrier(
        command_buffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, nullptr, 0, nullptr, 1, &destination_barrier);

    TraceNativeRendererEvent(
        "reflection-tail-mip-generated",
        fmt::format(
            "handle={:08X}@{} source-level={} destination-level={} source={}x{} "
            "destination={}x{} format={} filter=box-2x2 level-weighted=1",
            destination.source ? destination.source->handle : 0,
            destination.source ? destination.source->generation : 0, source_level,
            destination_level, CalculateNativeMipExtent(destination.width, source_level),
            CalculateNativeMipExtent(destination.height, source_level), mip_width, mip_height,
            uint32_t(destination.format)));
  }

  if (deterministic_trace_active_ && InitializeContentProbeBuffer()) {
    if (content_probe_buffer_.pending_frame != diagnostic_submitted_frame_) {
      std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
      content_probe_buffer_.pending_frame = diagnostic_submitted_frame_;
      content_probe_buffer_.stages = {};
    }
    for (uint32_t mip_level = destination.guest_mip_levels;
         mip_level < destination.mip_levels; ++mip_level) {
      uint32_t probe_stage = 0;
      while (probe_stage < content_probe_buffer_.stages.size() &&
             content_probe_buffer_.stages[probe_stage].valid) {
        ++probe_stage;
      }
      const uint32_t mip_width = CalculateNativeMipExtent(destination.width, mip_level);
      const uint32_t mip_height = CalculateNativeMipExtent(destination.height, mip_level);
      if (RecordContentProbeImage(
              command_buffer, probe_stage, destination.resource.image, destination.format,
              destination.layout, mip_width, mip_height,
              destination.source ? destination.source->handle : 0,
              destination.source ? destination.source->info.memory.base_address : 0, 14,
              mip_level)) {
        NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
        stage.command_index = uint32_t(diagnostic_command_index_);
        stage.render_phase = uint32_t(RenderPhase::kUnknown);
      }
    }
  }

  static std::atomic<uint64_t> generation_count{0};
  const uint64_t count = ++generation_count;
  if (count <= 16 || !(count % 1024)) {
    REXLOG_INFO(
        "gta4-native-reflection: filtered native tail mips #{} handle={:08X} "
        "guest-mips={} physical-mips={} base={}x{}",
        count, destination.source ? destination.source->handle : 0,
        destination.guest_mip_levels, destination.mip_levels, destination.width,
        destination.height);
  }
  return true;
}

bool Gta4NativeGraphicsSystem::RecordPresent(
    VkCommandBuffer command_buffer, VkImage presenter_image, VkImageView presenter_view,
    uint32_t presenter_width, uint32_t presenter_height,
    const std::shared_ptr<const NativeTextureResource>& present_source,
    NativeSurfaceImage* high_precision_source, bool hdr_output, float hdr_headroom,
    bool& transfer_written) {
  SCOPE_profile_cpu_i("gpu", "GTA4 Native RecordPresent");
  transfer_written = false;
  if (!present_source || !presenter_image || !presenter_width || !presenter_height) {
    TraceNativeRendererEvent("present-rejected", "reason=missing-input-or-extent");
    return false;
  }
  auto source_entry = native_texture_images_.find(present_source->generation);
  if (source_entry == native_texture_images_.end() || !source_entry->second) {
    TraceNativeRendererEvent("present-rejected", "reason=source-generation-not-prepared");
    return false;
  }
  NativeTextureImage& source = *source_entry->second;
  if (source.aspect != VK_IMAGE_ASPECT_COLOR_BIT || source.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
    TraceNativeRendererEvent("present-rejected", "reason=source-aspect-or-layout");
    return false;
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device) {
    TraceNativeRendererEvent("present-rejected", "reason=vulkan-device-missing");
    return false;
  }

  // GTA IV's resolved frontbuffer is the authoritative display-ready image.
  // The matching FP16 resolve source is an internal render-graph surface whose
  // transfer function and color grading are not part of the presentation ABI;
  // bypassing the final frontbuffer loses the game's authored saturation and
  // contrast. HDR therefore decodes this final sRGB image into the swapchain's
  // extended-linear color space instead of substituting an internal surface.
  VkImage shader_source_image = source.resource.image;
  VkImageView shader_source_view = source.resource.view;
  VkImageLayout* shader_source_layout = &source.layout;
  uint32_t shader_source_width = present_source->info.width + 1;
  uint32_t shader_source_height = present_source->info.height + 1;
  SmaaPipeline::Output smaa_output{};
  VkImageLayout smaa_output_layout = VK_IMAGE_LAYOUT_UNDEFINED;
  bool smaa_applied = false;
  if (IsNativeSmaaEnabled()) {
    const SmaaQuality quality = ParseSmaaQuality(
        rex::cvar::Query<std::string>("gta4_native_smaa_quality"));
    smaa_applied = smaa_pipeline_.Record(
        command_buffer, vulkan_device, frame_descriptor_pool_, native_pipeline_cache_,
        shader_source_image, shader_source_view, *shader_source_layout,
        {shader_source_width, shader_source_height}, quality, smaa_output);
    if (smaa_applied) {
      shader_source_image = smaa_output.image;
      shader_source_view = smaa_output.view;
      smaa_output_layout = smaa_output.layout;
      shader_source_layout = &smaa_output_layout;
      native_pipeline_cache_dirty_ = true;
    } else {
      static std::atomic<bool> logged_smaa_failure{false};
      if (!logged_smaa_failure.exchange(true)) {
        REXLOG_ERROR(
            "gta4-native-smaa: full three-pass chain unavailable; presenting without SMAA");
      }
    }
  }
  (void)high_precision_source;
  static std::atomic<uint64_t> hdr_present_count{0};
  const uint64_t present_count = ++hdr_present_count;
  if (present_count <= 16 || !(present_count % 1024)) {
    REXLOG_INFO(
        "gta4-native-hdr: present={} path={} source-format={} source-size={}x{} "
        "guest-format={} source-transfer={} HDR={} headroom={}",
        present_count, smaa_applied ? "frontbuffer-smaa" : "frontbuffer",
        uint32_t(smaa_applied ? smaa_output.format : source.format),
        shader_source_width, shader_source_height,
        uint32_t(ui::vulkan::VulkanPresenter::kGuestOutputFormat),
        "srgb", hdr_output, hdr_headroom);
  }

  VkPipeline present_pipeline = GetOrCreateHDRPresentPipeline();
  if (present_pipeline && frame_descriptor_pool_ && presenter_view &&
      shader_source_view) {
    const auto& dfn = vulkan_device->functions();
    const VkDevice device = vulkan_device->device();
    VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
    VkDescriptorSetAllocateInfo descriptor_allocate_info{};
    descriptor_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptor_allocate_info.descriptorPool = frame_descriptor_pool_;
    descriptor_allocate_info.descriptorSetCount = 1;
    descriptor_allocate_info.pSetLayouts = &resolve_conversion_descriptor_set_layout_;
    if (dfn.vkAllocateDescriptorSets(device, &descriptor_allocate_info, &descriptor_set) ==
        VK_SUCCESS) {
      VkImageMemoryBarrier source_barrier{};
      source_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      source_barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
      source_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      source_barrier.oldLayout = *shader_source_layout;
      source_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      source_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      source_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      source_barrier.image = shader_source_image;
      source_barrier.subresourceRange = ui::vulkan::util::InitializeSubresourceRange();
      dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                               &source_barrier);

      VkDescriptorImageInfo descriptor_image{};
      descriptor_image.imageView = shader_source_view;
      descriptor_image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      VkWriteDescriptorSet descriptor_write{};
      descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptor_write.dstSet = descriptor_set;
      descriptor_write.dstBinding = 0;
      descriptor_write.descriptorCount = 1;
      descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      descriptor_write.pImageInfo = &descriptor_image;
      dfn.vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);

      VkRenderingAttachmentInfo color_attachment{};
      color_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
      color_attachment.imageView = presenter_view;
      color_attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
      color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      VkRenderingInfo rendering_info{};
      rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
      rendering_info.renderArea.extent = {presenter_width, presenter_height};
      rendering_info.layerCount = 1;
      rendering_info.colorAttachmentCount = 1;
      rendering_info.pColorAttachments = &color_attachment;
      dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
      VkViewport viewport{};
      viewport.width = float(presenter_width);
      viewport.height = float(presenter_height);
      viewport.maxDepth = 1.0f;
      dfn.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
      VkRect2D scissor{};
      scissor.extent = {presenter_width, presenter_height};
      dfn.vkCmdSetScissor(command_buffer, 0, 1, &scissor);
      dfn.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, present_pipeline);
      dfn.vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                  resolve_conversion_pipeline_layout_, 0, 1, &descriptor_set, 0,
                                  nullptr);
      NativeHDRPresentConstants constants{};
      constants.source_width = int32_t(shader_source_width);
      constants.source_height = int32_t(shader_source_height);
      constants.destination_width = int32_t(presenter_width);
      constants.destination_height = int32_t(presenter_height);
      constants.hdr_headroom = std::max(1.0f, hdr_headroom);
      const uint32_t aa_bits = GetNativePresentationAABits();
      constants.output_mode = (hdr_output ? 1u : 0u) | aa_bits | 4u |
                              (REXCVAR_GET(gta4_native_output_dither) ? 8u : 0u);
      TraceNativeRendererEvent(
          "present-shader",
          fmt::format(
              "path=fullscreen-srgb-decode source-vk={} source-layout={} source={}x{} "
              "destination={}x{} hdr={} headroom={} aa-bits={:08X} mode={:08X}",
              uint32_t(source.format), uint32_t(*shader_source_layout), shader_source_width,
              shader_source_height, presenter_width, presenter_height, hdr_output,
              constants.hdr_headroom, aa_bits, constants.output_mode));
      if (!aa_bits && rex::cvar::Query<std::string>("gta4_native_upscaler") == "fsr1") {
        static std::atomic<bool> logged_fsr_without_aa{false};
        if (!logged_fsr_without_aa.exchange(true)) {
          REXLOG_WARN(
              "gta4-native-upscaler: FSR 1 input anti-aliasing is disabled; "
              "edge quality may be unstable");
        }
      }
      dfn.vkCmdPushConstants(command_buffer, resolve_conversion_pipeline_layout_,
                             VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(constants), &constants);
      dfn.vkCmdDraw(command_buffer, 3, 1, 0, 0);
      dfn.vkCmdEndRendering(command_buffer);
      *shader_source_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      TraceNativeRendererEvent("present-shader-result", "recorded=1");
      return true;
    }
  }

  VkFormatProperties source_properties{};
  VkFormatProperties destination_properties{};
  const auto& ifn = vulkan_device->vulkan_instance()->functions();
  const VkPhysicalDevice physical_device = vulkan_device->physical_device();
  ifn.vkGetPhysicalDeviceFormatProperties(physical_device, source.format, &source_properties);
  ifn.vkGetPhysicalDeviceFormatProperties(
      physical_device, ui::vulkan::VulkanPresenter::kGuestOutputFormat, &destination_properties);
  if (!(source_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT) ||
      !(destination_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT)) {
    TraceNativeRendererEvent("present-rejected", "reason=fallback-blit-format-unsupported");
    REXLOG_ERROR("gta4-native: unsupported frontbuffer blit formats {} -> {}",
                 uint32_t(source.format),
                 uint32_t(ui::vulkan::VulkanPresenter::kGuestOutputFormat));
    return false;
  }

  const auto& dfn = vulkan_device->functions();
  std::array<VkImageMemoryBarrier, 2> barriers{};
  VkImageMemoryBarrier& source_barrier = barriers[0];
  source_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  source_barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  source_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  source_barrier.oldLayout = source.layout;
  source_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  source_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  source_barrier.image = source.resource.image;
  source_barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);

  VkImageMemoryBarrier& destination_barrier = barriers[1];
  destination_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  destination_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  destination_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  destination_barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  destination_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  destination_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  destination_barrier.image = presenter_image;
  destination_barrier.subresourceRange =
      ui::vulkan::util::InitializeSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr,
                           uint32_t(barriers.size()), barriers.data());

  VkImageBlit blit{};
  blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  blit.srcSubresource.layerCount = 1;
  blit.srcOffsets[1] = {int32_t(present_source->info.width + 1),
                        int32_t(present_source->info.height + 1), 1};
  blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  blit.dstSubresource.layerCount = 1;
  blit.dstOffsets[1] = {int32_t(presenter_width), int32_t(presenter_height), 1};
  dfn.vkCmdBlitImage(command_buffer, source.resource.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                     presenter_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
                     VK_FILTER_NEAREST);

  source_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  source_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  source_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  source_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  dfn.vkCmdPipelineBarrier(
      command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
      nullptr, 1, &source_barrier);
  source.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  transfer_written = true;
  TraceNativeRendererEvent(
      "present-blit-result",
      fmt::format("recorded=1 source-vk={} source={}x{} destination={}x{}",
                  uint32_t(source.format), present_source->info.width + 1,
                  present_source->info.height + 1, presenter_width, presenter_height));
  return true;
}

bool Gta4NativeGraphicsSystem::ReadbackTextureToGuest(const TextureLockCommand& command,
                                                      TextureLockResult& result) {
  std::shared_ptr<const NativeTextureResource> texture;
  {
    std::lock_guard lock(texture_resource_mutex_);
    auto texture_entry = texture_resources_.find(command.texture);
    if (texture_entry == texture_resources_.end()) {
      return true;
    }
    texture = texture_entry->second;
  }
  result.generation = texture->generation;
  if (!texture->gpu_produced) {
    return true;
  }
  if (command.array_index || command.dimension != TextureLockDimension::k2D) {
    REXLOG_ERROR("gta4-native: unresolved native texture lock dimension {} array {}",
                 uint32_t(command.dimension), command.array_index);
    return false;
  }

  auto image_entry = native_texture_images_.find(texture->generation);
  if (image_entry == native_texture_images_.end() || !image_entry->second ||
      command.level >= image_entry->second->mip_levels) {
    REXLOG_ERROR("gta4-native: native texture generation {} is unavailable for lock level {}",
                 texture->generation, command.level);
    return false;
  }
  NativeTextureImage& image = *image_entry->second;
  if (image.aspect != VK_IMAGE_ASPECT_COLOR_BIT || image.layout == VK_IMAGE_LAYOUT_UNDEFINED) {
    REXLOG_ERROR("gta4-native: native texture generation {} has unsupported readback aspect {}",
                 texture->generation, uint32_t(image.aspect));
    return false;
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device =
      vulkan_provider ? vulkan_provider->vulkan_device() : nullptr;
  if (!vulkan_device || !submission_tracker_ ||
      (command_buffer_submission_ &&
       !submission_tracker_->AwaitSubmissionCompletion(command_buffer_submission_))) {
    return false;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  const FormatInfo* format_info = texture->info.format_info();
  if (!format_info) {
    return false;
  }
  const uint32_t bytes_per_block = format_info->bytes_per_block();
  uint32_t mip_width = 0;
  uint32_t mip_height = 0;
  texture->info.GetMipSize(command.level, &mip_width, &mip_height);
  const TextureExtent host_extent =
      TextureExtent::Calculate(format_info, mip_width, mip_height, 1, false, false);
  const VkDeviceSize readback_size = VkDeviceSize(host_extent.visible_blocks()) * bytes_per_block;
  if (!bytes_per_block || !readback_size) {
    return false;
  }

  VkBuffer readback_buffer = VK_NULL_HANDLE;
  VkDeviceMemory readback_memory = VK_NULL_HANDLE;
  uint32_t readback_memory_type = UINT32_MAX;
  VkDeviceSize readback_memory_size = 0;
  if (!ui::vulkan::util::CreateDedicatedAllocationBuffer(
          vulkan_device, readback_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
          ui::vulkan::util::MemoryPurpose::kReadback, readback_buffer, readback_memory,
          &readback_memory_type, &readback_memory_size)) {
    return false;
  }

  VkCommandPool readback_pool = VK_NULL_HANDLE;
  VkCommandBuffer readback_command_buffer = VK_NULL_HANDLE;
  auto destroy_readback_objects = [&]() {
    if (readback_pool) {
      dfn.vkDestroyCommandPool(device, readback_pool, nullptr);
      readback_pool = VK_NULL_HANDLE;
    }
    if (readback_buffer) {
      dfn.vkDestroyBuffer(device, readback_buffer, nullptr);
      readback_buffer = VK_NULL_HANDLE;
    }
    if (readback_memory) {
      dfn.vkFreeMemory(device, readback_memory, nullptr);
      readback_memory = VK_NULL_HANDLE;
    }
  };

  VkCommandPoolCreateInfo pool_info{};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
  pool_info.queueFamilyIndex = vulkan_device->queue_family_graphics_compute();
  if (dfn.vkCreateCommandPool(device, &pool_info, nullptr, &readback_pool) != VK_SUCCESS) {
    destroy_readback_objects();
    return false;
  }

  VkCommandBufferAllocateInfo allocate_info{};
  allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocate_info.commandPool = readback_pool;
  allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocate_info.commandBufferCount = 1;
  if (dfn.vkAllocateCommandBuffers(device, &allocate_info, &readback_command_buffer) !=
      VK_SUCCESS) {
    destroy_readback_objects();
    return false;
  }

  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  if (dfn.vkBeginCommandBuffer(readback_command_buffer, &begin_info) != VK_SUCCESS) {
    destroy_readback_objects();
    return false;
  }

  const VkImageLayout previous_layout = image.layout;
  VkImageMemoryBarrier image_barrier{};
  image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  image_barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  image_barrier.oldLayout = previous_layout;
  image_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  image_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  image_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  image_barrier.image = image.resource.image;
  image_barrier.subresourceRange = ui::vulkan::util::InitializeSubresourceRange(
      VK_IMAGE_ASPECT_COLOR_BIT, command.level, 1, 0, 1);
  dfn.vkCmdPipelineBarrier(readback_command_buffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &image_barrier);

  VkBufferImageCopy copy{};
  copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  copy.imageSubresource.mipLevel = command.level;
  copy.imageSubresource.layerCount = 1;
  copy.imageExtent = {mip_width, mip_height, 1};
  dfn.vkCmdCopyImageToBuffer(readback_command_buffer, image.resource.image,
                             VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, readback_buffer, 1, &copy);

  VkBufferMemoryBarrier buffer_barrier{};
  buffer_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
  buffer_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  buffer_barrier.dstAccessMask = VK_ACCESS_HOST_READ_BIT;
  buffer_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  buffer_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  buffer_barrier.buffer = readback_buffer;
  buffer_barrier.size = VK_WHOLE_SIZE;
  image_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  image_barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  image_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  image_barrier.newLayout = previous_layout;
  dfn.vkCmdPipelineBarrier(readback_command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_HOST_BIT | VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0,
                           nullptr, 1, &buffer_barrier, 1, &image_barrier);

  if (dfn.vkEndCommandBuffer(readback_command_buffer) != VK_SUCCESS) {
    destroy_readback_objects();
    return false;
  }

  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &readback_command_buffer;
  ui::vulkan::VulkanSubmissionTracker readback_submission(vulkan_device);
  const uint64_t submission = readback_submission.GetCurrentSubmission();
  {
    ui::vulkan::VulkanSubmissionTracker::FenceAcquisition fence_acquisition(
        readback_submission.AcquireFenceToAdvanceSubmission());
    if (!fence_acquisition.fence()) {
      fence_acquisition.SubmissionFailedOrDropped();
      destroy_readback_objects();
      return false;
    }
    VkResult submit_result;
    {
      auto queue = vulkan_device->AcquireQueue(vulkan_device->queue_family_graphics_compute(), 0);
      submit_result = dfn.vkQueueSubmit(queue.queue(), 1, &submit_info, fence_acquisition.fence());
    }
    if (submit_result != VK_SUCCESS) {
      fence_acquisition.SubmissionFailedOrDropped();
      destroy_readback_objects();
      return false;
    }
  }
  if (!readback_submission.AwaitSubmissionCompletion(submission)) {
    destroy_readback_objects();
    return false;
  }
  dfn.vkDestroyCommandPool(device, readback_pool, nullptr);
  readback_pool = VK_NULL_HANDLE;

  void* readback_mapping = nullptr;
  if (dfn.vkMapMemory(device, readback_memory, 0, VK_WHOLE_SIZE, 0, &readback_mapping) !=
      VK_SUCCESS) {
    destroy_readback_objects();
    return false;
  }
  if (!(vulkan_device->memory_types().host_coherent & (uint32_t(1) << readback_memory_type))) {
    VkMappedMemoryRange range{};
    range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    range.memory = readback_memory;
    range.size =
        std::min(rex::round_up(readback_size, vulkan_device->properties().nonCoherentAtomSize),
                 readback_memory_size);
    dfn.vkInvalidateMappedMemoryRanges(device, 1, &range);
  }

  uint32_t packed_offset_x = 0;
  uint32_t packed_offset_y = 0;
  const uint32_t guest_mip_address =
      texture->info.GetMipLocation(command.level, &packed_offset_x, &packed_offset_y, true);
  uint8_t* guest_mip = memory_->TranslatePhysical<uint8_t*>(guest_mip_address);
  if (!guest_mip || !rex::is_pow2(bytes_per_block)) {
    dfn.vkUnmapMemory(device, readback_memory);
    destroy_readback_objects();
    return false;
  }

  const TextureExtent guest_extent = texture->info.GetMipExtent(command.level, true);
  const uint32_t bytes_per_block_log2 = rex::log2_floor(bytes_per_block);
  const auto* source = static_cast<const uint8_t*>(readback_mapping);
  bool copy_succeeded = true;
  for (uint32_t block_y = 0; block_y < host_extent.block_height && copy_succeeded; ++block_y) {
    for (uint32_t block_x = 0; block_x < host_extent.block_width; ++block_x) {
      const uint32_t destination_x = packed_offset_x + block_x;
      const uint32_t destination_y = packed_offset_y + block_y;
      int32_t destination_offset;
      if (texture->info.is_tiled) {
        destination_offset =
            texture_util::GetTiledOffset2D(int32_t(destination_x), int32_t(destination_y),
                                           guest_extent.block_pitch_h, bytes_per_block_log2);
      } else {
        destination_offset =
            int32_t((destination_y * guest_extent.block_pitch_h + destination_x) * bytes_per_block);
      }
      if (destination_offset < 0) {
        copy_succeeded = false;
        break;
      }
      const size_t source_offset =
          size_t(block_y * host_extent.block_width + block_x) * bytes_per_block;
      texture_conversion::CopySwapBlock(texture->info.endianness,
                                        guest_mip + uint32_t(destination_offset),
                                        source + source_offset, bytes_per_block);
    }
  }

  dfn.vkUnmapMemory(device, readback_memory);
  destroy_readback_objects();
  if (!copy_succeeded) {
    return false;
  }
  result.copied_to_guest = 1;
  return true;
}

void Gta4NativeGraphicsSystem::LogVectorFontDraw(const NativeCommand& command,
                                                 uint32_t submitted_frame,
                                                 size_t command_index) const {
  if (!REXCVAR_GET(gta4_trace_vector_fonts) || !command.pipeline_state) {
    return;
  }

  bool uses_vector_font = false;
  for (const auto& texture : command.textures) {
    if (texture && texture->vector_font_replacement) {
      uses_vector_font = true;
      break;
    }
  }
  if (!uses_vector_font) {
    return;
  }

  static std::atomic<uint64_t> font_draw_trace_count{0};
  const uint64_t trace = ++font_draw_trace_count;
  if (trace > 96) {
    return;
  }

  const NativePipelineState& state = *command.pipeline_state;
  uint32_t primitive_type = 0;
  uint32_t vertex_count = 0;
  uint32_t start_vertex = 0;
  int32_t base_vertex = 0;
  uint32_t stride = 0;
  uint32_t start_index = 0;
  uint32_t index_count = 0;
  if (command.type == CommandType::kDrawPrimitiveUp) {
    DrawPrimitiveUpCommand draw{};
    std::memcpy(&draw, command.bytes.data(), sizeof(draw));
    primitive_type = draw.primitive_type;
    vertex_count = draw.vertex_count;
    stride = draw.stride;
  } else if (command.type == CommandType::kDrawPrimitive) {
    DrawPrimitiveCommand draw{};
    std::memcpy(&draw, command.bytes.data(), sizeof(draw));
    primitive_type = draw.primitive_type;
    vertex_count = draw.vertex_count;
    start_vertex = draw.start_vertex;
  } else if (command.type == CommandType::kDrawIndexedPrimitive) {
    DrawIndexedPrimitiveCommand draw{};
    std::memcpy(&draw, command.bytes.data(), sizeof(draw));
    primitive_type = draw.primitive_type;
    base_vertex = draw.base_vertex;
    start_index = draw.start_index;
    index_count = draw.index_count;
  }

  const NativeShader* vertex_shader = state.vertex_shader_resource;
  const NativeShader* pixel_shader = state.pixel_shader_resource;
  const NativeVertexDeclaration* declaration = state.vertex_declaration_resource.get();
  REXLOG_INFO(
      "gta4-native-font-debug: draw #{} frame={} command-index={} type={} primitive={} "
      "vertices={} start-vertex={} base-vertex={} indices={} start-index={} up-stride={} "
      "vs={:08X}/{:016X}/{} ps={:08X}/{:016X}/{} vdecl={:08X}/{:016X} "
      "constants={:016X}/{:016X} target={:08X}/{}x{} blend={} alpha-test={} "
      "viewport={:08X},{:08X},{:08X},{:08X}",
      trace, submitted_frame, command_index, CommandTypeName(command.type), primitive_type,
      vertex_count, start_vertex, base_vertex, index_count, start_index, stride,
      state.vertex_shader, vertex_shader ? vertex_shader->hash : 0,
      vertex_shader ? vertex_shader->filename : std::string{}, state.pixel_shader,
      pixel_shader ? pixel_shader->hash : 0,
      pixel_shader ? pixel_shader->filename : std::string{}, state.vertex_declaration,
      declaration ? declaration->content_hash : 0, command.vertex_constants_hash,
      command.pixel_constants_hash, state.render_targets[0].handle,
      state.render_targets[0].width, state.render_targets[0].height,
      command.fixed_function_state.blend_enable, command.fixed_function_state.alpha_test_enable,
      command.fixed_function_state.viewport_bits[0], command.fixed_function_state.viewport_bits[1],
      command.fixed_function_state.viewport_bits[2], command.fixed_function_state.viewport_bits[3]);

  for (uint32_t stage = 0; stage < command.textures.size(); ++stage) {
    const auto& texture = command.textures[stage];
    if (!texture || !texture->vector_font_replacement) {
      continue;
    }
    const xenos::xe_gpu_texture_fetch_t& fetch = command.texture_fetches[stage];
    REXLOG_INFO(
        "gta4-native-font-debug: draw-texture #{} stage={} font={} handle={:08X} "
        "generation={} resource-hash={:016X} size={}x{} format={} payload={} "
        "payload-hash={:016X} descriptor-indices={}/{} "
        "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X}",
        trace, stage, texture->vector_font_id, texture->handle, texture->generation,
        texture->content_hash, texture->info.width + 1, texture->info.height + 1,
        uint32_t(texture->info.format), texture->payload.size(),
        XXH3_64bits(texture->payload.data(), texture->payload.size()),
        command.texture_descriptor_indices[stage], command.sampler_descriptor_indices[stage],
        fetch.dword_0, fetch.dword_1, fetch.dword_2, fetch.dword_3, fetch.dword_4, fetch.dword_5);
  }

  auto format_hex_prefix = [](const uint8_t* data, size_t size) {
    std::string output;
    if (!data || !size) {
      return output;
    }
    const size_t prefix_size = std::min<size_t>(size, 128);
    output.reserve(prefix_size * 2);
    for (size_t index = 0; index < prefix_size; ++index) {
      output += fmt::format("{:02X}", data[index]);
    }
    return output;
  };

  if (command.type == CommandType::kDrawPrimitiveUp) {
    REXLOG_INFO("gta4-native-font-debug: draw-vertex-bytes #{} source=up size={} hash={:016X} "
                "prefix={}",
                trace, command.payload.size(),
                XXH3_64bits(command.payload.data(), command.payload.size()),
                format_hex_prefix(command.payload.data(), command.payload.size()));
  } else {
    for (uint32_t stream = 0; stream < command.vertex_buffers.size(); ++stream) {
      const auto& resource = command.vertex_buffers[stream];
      if (!resource || !state.vertex_streams[stream].stride) {
        continue;
      }
      const auto& stream_state = state.vertex_streams[stream];
      const size_t offset = std::min<size_t>(stream_state.offset, resource->payload.size());
      const uint8_t* prefix_data =
          offset < resource->payload.size() ? resource->payload.data() + offset : nullptr;
      REXLOG_INFO(
          "gta4-native-font-debug: draw-vertex-bytes #{} source=stream{} handle={:08X} "
          "generation={} guest={:08X}/{} state-offset={} stride={} payload={} hash={:016X} "
          "prefix={}",
          trace, stream, resource->handle, resource->generation, resource->guest_address,
          resource->guest_size, stream_state.offset, stream_state.stride, resource->payload.size(),
          resource->content_hash, format_hex_prefix(prefix_data,
                                                    resource->payload.size() - offset));
    }
    if (command.index_buffer) {
      REXLOG_INFO(
          "gta4-native-font-debug: draw-index-bytes #{} handle={:08X} generation={} "
          "flags={:08X} payload={} hash={:016X} prefix={}",
          trace, command.index_buffer->handle, command.index_buffer->generation,
          command.index_buffer->flags, command.index_buffer->payload.size(),
          command.index_buffer->content_hash,
          format_hex_prefix(command.index_buffer->payload.data(),
                            command.index_buffer->payload.size()));
    }
  }

  if (!declaration) {
    REXLOG_WARN("gta4-native-font-debug: draw-layout #{} missing vertex declaration", trace);
    return;
  }
  for (size_t element_index = 0; element_index < declaration->elements.size(); ++element_index) {
    const VertexElement& element = declaration->elements[element_index];
    const uint32_t component_count = GetFloat32VertexElementComponentCount(element.type);
    std::string values;
    const uint8_t* bytes = nullptr;
    size_t byte_size = 0;
    size_t stream_base = 0;
    uint32_t element_stride = 0;
    uint32_t available_vertices = vertex_count;
    if (command.type == CommandType::kDrawPrimitiveUp && element.stream == 0) {
      bytes = command.payload.data();
      byte_size = command.payload.size();
      element_stride = stride;
    } else if (command.type != CommandType::kDrawPrimitiveUp &&
               element.stream < command.vertex_buffers.size() &&
               command.vertex_buffers[element.stream]) {
      const auto& resource = command.vertex_buffers[element.stream];
      bytes = resource->payload.data();
      byte_size = resource->payload.size();
      element_stride = state.vertex_streams[element.stream].stride;
      stream_base = state.vertex_streams[element.stream].offset;
      if (command.type == CommandType::kDrawPrimitive) {
        stream_base += size_t(start_vertex) * element_stride;
      }
      if (command.type == CommandType::kDrawIndexedPrimitive) {
        available_vertices = 4;
      }
    }
    if (bytes && component_count && element_stride) {
      const uint32_t logged_vertices = std::min<uint32_t>(available_vertices, 4);
      for (uint32_t vertex = 0; vertex < logged_vertices; ++vertex) {
        values += fmt::format("v{}=[", vertex);
        for (uint32_t component = 0; component < component_count; ++component) {
          const size_t component_offset = stream_base + size_t(vertex) * element_stride +
                                          element.offset + size_t(component) * sizeof(uint32_t);
          if (component_offset > byte_size || sizeof(uint32_t) > byte_size - component_offset) {
            values += "oob";
          } else {
            uint32_t bits = 0;
            std::memcpy(&bits, bytes + component_offset, sizeof(bits));
            bits = __builtin_bswap32(bits);
            values += fmt::format("{}{:g}/{:08X}", component ? "," : "",
                                  std::bit_cast<float>(bits), bits);
          }
        }
        values += "]";
      }
    }
    REXLOG_INFO(
        "gta4-native-font-debug: draw-layout #{} element={} stream={} offset={} type={:08X} "
        "method={} usage={}/{} location={} float-components={} stride={} values={}",
        trace, element_index, element.stream, element.offset, element.type, element.method,
        element.usage, element.usage_index,
        ConvertVertexUsageToLocation(element.usage, element.usage_index), component_count,
        element_stride, values);
  }
}

bool Gta4NativeGraphicsSystem::RecordNativeFrame(
    VkCommandBuffer command_buffer, uint32_t width, uint32_t height, VkImage presenter_image,
    VkImageView presenter_view, uint32_t submitted_frame,
    const std::shared_ptr<const NativeTextureResource>& present_source,
    const std::shared_ptr<const EnvironmentalDataV1>& environmental_data,
    bool hdr_output, float hdr_headroom, bool& presenter_transfer_written,
    bool trace_stages, uint32_t trace_sequence) {
  SCOPE_profile_cpu_i("gpu", "GTA4 Native RecordNativeFrame");
  presenter_transfer_written = false;
  (void)environmental_data;
  static std::atomic<uint64_t> invocation_counter{0};
  const uint64_t invocation = ++invocation_counter;
  const bool legacy_diagnostics =
      REXCVAR_GET(gta4_trace_startup_content) || REXCVAR_GET(gta4_trace_native_renderer);
  const bool transition_trace = transition::IsEnabled() && transition::ActiveTransitionId();
  uint32_t queued_draws = 0;
  uint32_t queued_draws_up = 0;
  uint32_t queued_draws_indexed = 0;
  uint32_t queued_resolves = 0;
  uint32_t queued_clears = 0;
  uint32_t missing_pipeline_state = 0;
  uint32_t target_failures = 0;
  uint32_t presenter_target_commands = 0;
  uint32_t offscreen_target_commands = 0;
  uint32_t successful_draws = 0;
  uint32_t failed_draws = 0;
  uint32_t successful_resolves = 0;
  uint32_t failed_resolves = 0;
  uint32_t successful_clears = 0;
  uint32_t failed_clears = 0;
  uint32_t successful_presenter_commands = 0;
  uint32_t texture_requested_bindings = 0;
  uint32_t texture_captured_bindings = 0;
  uint32_t texture_bound_bindings = 0;
  uint32_t textured_draws = 0;
  uint32_t draws_with_missing_textures = 0;
  uint32_t alpha_blend_draws = 0;
  uint32_t alpha_test_draws = 0;
  uint32_t z_disabled_draws = 0;
  uint32_t z_write_disabled_draws = 0;
  uint32_t nondefault_color_write_draws = 0;
  uint32_t texture_failure_examples = 0;
  uint32_t fixed_state_examples = 0;
  std::array<std::array<uint32_t, 16>, 3> primitive_counts{};
  uint32_t first_render_target_handle = 0;
  uint32_t first_render_target_address = 0;
  uint32_t last_render_target_handle = 0;
  uint32_t last_render_target_address = 0;
  uint32_t first_resolve_source_handle = 0;
  uint32_t first_resolve_source_address = 0;
  uint32_t first_resolve_destination = 0;
  uint64_t first_resolve_generation = 0;
  uint32_t last_resolve_source_handle = 0;
  uint32_t last_resolve_source_address = 0;
  uint32_t last_resolve_destination = 0;
  uint64_t last_resolve_generation = 0;
  bool saw_render_target = false;
  bool saw_resolve = false;
  bool saw_traced_render_target = false;
  uint32_t traced_render_target_handle = 0;
  bool present_matches_frame_resolve = false;
  NativeSurfaceImage* high_precision_present_source = nullptr;
  NativeTextureImage* final_composite_input = nullptr;
  bool recorded_draw = false;
  bool rendering = false;
  bool presenter_written = false;
  NativeRenderingTarget active_target;
  NativeFrameResources resources;
  PostFxScheduler postfx_scheduler;
  postfx_scheduler.BeginFrame();
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const auto& dfn = vulkan_provider->vulkan_device()->functions();
  XXH3_state_t* semantic_hash_state =
      (legacy_diagnostics || transition_trace) ? XXH3_createState() : nullptr;
  if (semantic_hash_state && XXH3_64bits_reset(semantic_hash_state) != XXH_OK) {
    XXH3_freeState(semantic_hash_state);
    semantic_hash_state = nullptr;
  }
  auto hash_bytes = [semantic_hash_state](const void* data, size_t size) {
    if (semantic_hash_state && data && size) {
      XXH3_64bits_update(semantic_hash_state, data, size);
    }
  };

  auto targets_equal = [](const NativeRenderingTarget& left, const NativeRenderingTarget& right) {
    return left.color_views == right.color_views && left.depth_view == right.depth_view &&
           left.color_formats == right.color_formats && left.depth_format == right.depth_format &&
           left.samples == right.samples && left.width == right.width &&
           left.height == right.height && left.uses_presenter == right.uses_presenter;
  };

  diagnostic_submitted_frame_ = submitted_frame;
  diagnostic_command_index_ = SIZE_MAX;
  deterministic_trace_event_ = 0;
  deterministic_trace_active_ = trace_stages;
  TraceNativeRendererEvent(
      "frame-begin",
      fmt::format(
          "commands={} output={}x{} present={:08X}@{} hdr={}:{} environment={}:{}:{:016X}",
          current_frame_.size(), width, height, present_source ? present_source->handle : 0,
          present_source ? present_source->generation : 0, hdr_output, hdr_headroom,
          environmental_data ? environmental_data->source_sequence : 0,
          environmental_data ? environmental_data->version : 0,
          environmental_data ? environmental_data->valid_fields : 0));
  if (environmental_data) {
    TraceNativeRendererEvent(
        "environment",
        fmt::format(
            "dt={} motion-blur={}:{} fog={}:{}:{}:{}:{} fog-color={},{},{},{} "
            "sun-color={},{},{},{} camera-altitude={} effects={:08X}:{}",
            environmental_data->time_step_seconds, environmental_data->motion_blur_scale,
            environmental_data->directional_motion_blur_length, environmental_data->fog_start,
            environmental_data->fog_density, environmental_data->fog_height_falloff,
            environmental_data->fog_altitude_tweak, environmental_data->fog_power,
            environmental_data->fog_color[0], environmental_data->fog_color[1],
            environmental_data->fog_color[2], environmental_data->fog_color[3],
            environmental_data->sun_color[0], environmental_data->sun_color[1],
            environmental_data->sun_color[2], environmental_data->sun_color[3],
            environmental_data->camera_altitude,
            environmental_data->effect_settings.enabled_effects,
            environmental_data->effect_settings.motion_blur_quality));
  }

  if (trace_stages) {
    REXLOG_WARN(
        "gta4-native-stage: trace={} frame={} stage=record-begin commands={} present-source={} "
        "generation={}",
        trace_sequence, submitted_frame, current_frame_.size(), bool(present_source),
        present_source ? present_source->generation : 0);
  }
  if (transition_trace) {
    transition::Record(transition::EventSource::kRenderer,
                       transition::EventType::kFrameRecordBegin, 0, 0, submitted_frame,
                       transition::kFlagBefore, invocation, current_frame_.size(),
                       present_source ? present_source->generation : 0);
  }
  const bool diagnostic_frame = ShouldLogDiagnosticFrame(submitted_frame);
  const bool translucent_queries_active =
      diagnostic_frame && InitializeTranslucentQueryPool();
  std::unordered_map<std::string_view, uint32_t> translucent_category_draw_counts;
  std::unordered_set<std::string_view> translucent_depth_probe_categories;
  uint32_t forward_depth_handle = 0;
  uint32_t forward_depth_wrapper = 0;
  size_t depth_handoff_command_index = SIZE_MAX;
  for (size_t pending_index = 0; pending_index < current_frame_.size();
       ++pending_index) {
    const NativeCommand& pending = current_frame_[pending_index];
    if (pending.type != CommandType::kDepthSurfaceHandoff) {
      continue;
    }
    DepthSurfaceHandoffCommand handoff{};
    std::memcpy(&handoff, pending.bytes.data(), sizeof(handoff));
    forward_depth_handle = handoff.destination.handle;
    forward_depth_wrapper = handoff.destination_wrapper;
    depth_handoff_command_index = pending_index;
  }
  size_t first_opaque_depth_consumer_index = SIZE_MAX;
  size_t first_translucent_depth_consumer_index = SIZE_MAX;
  std::string_view first_translucent_depth_category;
  if (diagnostic_frame && forward_depth_handle &&
      depth_handoff_command_index != SIZE_MAX) {
    for (size_t pending_index = 0; pending_index < current_frame_.size();
         ++pending_index) {
      if (pending_index <= depth_handoff_command_index) {
        continue;
      }
      const NativeCommand& pending = current_frame_[pending_index];
      const bool draw = pending.type == CommandType::kDrawPrimitive ||
                        pending.type == CommandType::kDrawPrimitiveUp ||
                        pending.type == CommandType::kDrawIndexedPrimitive;
      if (!draw || !pending.pipeline_state ||
          pending.pipeline_state->depth_stencil.handle != forward_depth_handle ||
          !ConvertColorWriteMask(
              pending.fixed_function_state.color_write_mask)) {
        continue;
      }
      const NativeShader* pixel_shader =
          pending.pipeline_state->pixel_shader_resource;
      const std::string_view category =
          pixel_shader
              ? ClassifyTranslucentDiagnosticShader(pixel_shader->filename)
              : std::string_view{};
      if (category.empty() && first_opaque_depth_consumer_index == SIZE_MAX) {
        first_opaque_depth_consumer_index = pending_index;
      }
      const bool target_translucent_family =
          category == "water-surface" || category == "vehicle-glass" ||
          category == "glass" || category == "alpha-reflect" ||
          category == "vehicle-light" || category == "light-sprite";
      if (target_translucent_family &&
          first_translucent_depth_consumer_index == SIZE_MAX) {
        first_translucent_depth_consumer_index = pending_index;
        first_translucent_depth_category = category;
      }
    }
    REXLOG_WARN(
        "gta4-native-cause: point=depth-lifecycle-consumer-plan frame={} "
        "handoff-cmd={} destination={:08X} opaque-cmd={} translucent-cmd={} "
        "translucent-category={}",
        submitted_frame, depth_handoff_command_index, forward_depth_handle,
        first_opaque_depth_consumer_index,
        first_translucent_depth_consumer_index,
        first_translucent_depth_category.empty()
            ? "none"
            : first_translucent_depth_category);
  }
  bool forward_phase_entry_probed = false;
  if (translucent_queries_active) {
    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
    vulkan_device->functions().vkCmdResetQueryPool(
        command_buffer, translucent_query_state_.pool, 0,
        kTranslucentQueryCapacity);
    translucent_query_state_.pending_frame = submitted_frame;
    translucent_query_state_.pending_count = 0;
    translucent_query_state_.queries = {};
    REXLOG_INFO(
        "gta4-native-cause: point=translucent-query-frame-begin frame={} capacity={}",
        submitted_frame, kTranslucentQueryCapacity);
  }
  uint32_t final_present_source_handle = 0;
  if (present_source) {
    for (const NativeCommand& pending : current_frame_) {
      if (pending.type != CommandType::kResolve || !pending.resolve_destination ||
          pending.resolve_destination->generation != present_source->generation) {
        continue;
      }
      ResolveCommand resolve{};
      std::memcpy(&resolve, pending.bytes.data(), sizeof(resolve));
      final_present_source_handle = resolve.source.handle;
    }
  }
  bool have_previous_final_draw = false;
  bool have_last_final_draw = false;
  size_t previous_final_draw_index = 0;
  size_t last_final_draw_index = 0;
  std::unordered_set<size_t> resolve_predecessor_draw_indices;
  if (final_present_source_handle) {
    for (size_t index = 0; index < current_frame_.size(); ++index) {
      const NativeCommand& pending = current_frame_[index];
      const bool draw = pending.type == CommandType::kDrawPrimitive ||
                        pending.type == CommandType::kDrawPrimitiveUp ||
                        pending.type == CommandType::kDrawIndexedPrimitive;
      if (!draw || !pending.pipeline_state ||
          pending.pipeline_state->render_targets[0].handle != final_present_source_handle) {
        continue;
      }
      if (have_last_final_draw) {
        previous_final_draw_index = last_final_draw_index;
        have_previous_final_draw = true;
      }
      last_final_draw_index = index;
      have_last_final_draw = true;
    }
  }
  {
    std::vector<size_t> recent_draw_indices;
    for (size_t index = 0; index < current_frame_.size(); ++index) {
      const NativeCommand& pending = current_frame_[index];
      const bool draw = pending.type == CommandType::kDrawPrimitive ||
                        pending.type == CommandType::kDrawPrimitiveUp ||
                        pending.type == CommandType::kDrawIndexedPrimitive;
      if (draw && pending.pipeline_state) {
        recent_draw_indices.push_back(index);
        continue;
      }
      if (pending.type != CommandType::kResolve) {
        continue;
      }
      ResolveCommand resolve{};
      std::memcpy(&resolve, pending.bytes.data(), sizeof(resolve));
      if (!(resolve.flags & 0x04000000u)) {
        continue;
      }
      uint32_t matches = 0;
      for (auto draw_index = recent_draw_indices.rbegin();
           draw_index != recent_draw_indices.rend() && matches < 3; ++draw_index) {
        const NativeCommand& candidate = current_frame_[*draw_index];
        if (candidate.pipeline_state->render_targets[0].handle != resolve.source.handle) {
          continue;
        }
        resolve_predecessor_draw_indices.insert(*draw_index);
        ++matches;
      }
    }
  }
  std::unordered_map<size_t, uint32_t> scene_write_checkpoint_ordinals;
  uint32_t scene_write_checkpoint_count = 0;
  size_t first_scene_resolve_index = SIZE_MAX;
  if (diagnostic_frame && final_present_source_handle) {
    for (size_t index = 0; index < current_frame_.size(); ++index) {
      const NativeCommand& pending = current_frame_[index];
      if (pending.type != CommandType::kResolve) {
        continue;
      }
      ResolveCommand resolve{};
      std::memcpy(&resolve, pending.bytes.data(), sizeof(resolve));
      if (resolve.source.handle == final_present_source_handle &&
          (resolve.flags & 0x04000000u)) {
        first_scene_resolve_index = index;
        break;
      }
    }
  }
  if (first_scene_resolve_index != SIZE_MAX) {
    std::vector<size_t> scene_write_candidates;
    for (size_t index = 0; index < first_scene_resolve_index; ++index) {
      const NativeCommand& pending = current_frame_[index];
      if (!pending.pipeline_state ||
          pending.pipeline_state->render_targets[0].handle != final_present_source_handle) {
        continue;
      }
      const bool draw = pending.type == CommandType::kDrawPrimitive ||
                        pending.type == CommandType::kDrawPrimitiveUp ||
                        pending.type == CommandType::kDrawIndexedPrimitive;
      bool writes_color = draw &&
                          ConvertColorWriteMask(pending.fixed_function_state.color_write_mask);
      if (pending.type == CommandType::kClear) {
        ClearCommand clear{};
        std::memcpy(&clear, pending.bytes.data(), sizeof(clear));
        writes_color = (clear.flags & 1u) != 0;
      }
      if (writes_color) {
        scene_write_candidates.push_back(index);
      }
    }
    scene_write_checkpoint_count = uint32_t(std::min<size_t>(
        scene_write_candidates.size(), kSceneWriteCheckpointMaximumStages));
    if (scene_write_checkpoint_count) {
      const size_t first_scene_write_index = scene_write_candidates.front();
      const NativeCommand& first_scene_write = current_frame_[first_scene_write_index];
      const SurfaceDescriptor& first_scene_target =
          first_scene_write.pipeline_state->render_targets[0];
      for (size_t predecessor_index = first_scene_write_index;
           predecessor_index != 0; --predecessor_index) {
        const size_t candidate_index = predecessor_index - 1;
        const NativeCommand& candidate = current_frame_[candidate_index];
        if (candidate.type != CommandType::kResolve) {
          continue;
        }
        ResolveCommand predecessor_resolve{};
        std::memcpy(&predecessor_resolve, candidate.bytes.data(), sizeof(predecessor_resolve));
        const bool clears_color = (predecessor_resolve.flags & (1u << 8)) != 0;
        const bool clears_depth = (predecessor_resolve.flags & (1u << 9)) != 0;
        if ((!clears_color && !clears_depth) ||
            predecessor_resolve.source.base != first_scene_target.base) {
          continue;
        }
        REXLOG_ERROR(
            "gta4-native-cause: point=scene-prewrite-resolve-clear-link frame={} "
            "scene-cmd={} target={:08X}/{:08X}/base{:08X}/f{:08X}/{}x{} "
            "resolve-cmd={} source={:08X}/{:08X}/base{:08X}/f{:08X}/{}x{} "
            "flags={:08X} clear-color={} clear-depth={} "
            "clear={:08X},{:08X},{:08X},{:08X}:{:016X}:{} implementation=unapplied",
            submitted_frame, first_scene_write_index, first_scene_target.handle,
            first_scene_target.address, first_scene_target.base, first_scene_target.format,
            first_scene_target.width, first_scene_target.height, candidate_index,
            predecessor_resolve.source.handle, predecessor_resolve.source.address,
            predecessor_resolve.source.base, predecessor_resolve.source.format,
            predecessor_resolve.source.width, predecessor_resolve.source.height,
            predecessor_resolve.flags, clears_color, clears_depth,
            predecessor_resolve.clear_color_bits[0], predecessor_resolve.clear_color_bits[1],
            predecessor_resolve.clear_color_bits[2], predecessor_resolve.clear_color_bits[3],
            predecessor_resolve.clear_depth_bits, predecessor_resolve.clear_stencil);
        break;
      }
      for (uint32_t checkpoint = 0; checkpoint < scene_write_checkpoint_count; ++checkpoint) {
        const size_t candidate_position = scene_write_checkpoint_count == 1
                                              ? 0
                                              : (size_t(checkpoint) *
                                                 (scene_write_candidates.size() - 1)) /
                                                    (scene_write_checkpoint_count - 1);
        scene_write_checkpoint_ordinals.emplace(scene_write_candidates[candidate_position],
                                                checkpoint + 1);
      }
      REXLOG_WARN(
          "gta4-native-cause: point=scene-checkpoint-plan frame={} target={:08X} "
          "resolve-cmd={} candidates={} checkpoints={} first-cmd={} last-cmd={}",
          submitted_frame, final_present_source_handle, first_scene_resolve_index,
          scene_write_candidates.size(), scene_write_checkpoint_ordinals.size(),
          scene_write_candidates.front(), scene_write_candidates.back());
    }
  }
  size_t native_profile_phase_span = SIZE_MAX;
  RenderPhase native_profile_phase = RenderPhase::kUnknown;
  for (size_t command_index = 0; command_index < current_frame_.size(); ++command_index) {
    const NativeCommand& command = current_frame_[command_index];
    const bool profile_gpu_command = IsProfiledNativeGpuCommand(command.type);
    if (native_gpu_profile_state_.active &&
        !native_gpu_profile_state_.command_detail && profile_gpu_command &&
        (native_profile_phase_span == SIZE_MAX ||
         native_profile_phase != command.render_phase)) {
      EndNativeGpuProfileSpan(command_buffer, native_profile_phase_span);
      native_profile_phase = command.render_phase;
      native_profile_phase_span = BeginNativeGpuProfileSpan(
          command_buffer, NativeGpuProfileScopeKind::kRenderPhase,
          command.render_phase);
    }
    const uint64_t native_profile_cpu_begin =
        native_gpu_profile_state_.active &&
                native_gpu_profile_state_.command_detail && profile_gpu_command
            ? rex::chrono::Clock::QueryHostTickCount()
            : 0;
    const size_t native_profile_command_span =
        native_gpu_profile_state_.active &&
                native_gpu_profile_state_.command_detail && profile_gpu_command
            ? BeginNativeGpuProfileSpan(
                  command_buffer, NativeGpuProfileScopeKind::kCommand,
                  command.render_phase, command.type, uint32_t(command_index),
                  &command)
            : SIZE_MAX;
    auto native_profile_command_scope = MakeScopeExit([&]() {
      EndNativeGpuProfileSpan(command_buffer, native_profile_command_span,
                              native_profile_cpu_begin);
    });
    diagnostic_submitted_frame_ = submitted_frame;
    diagnostic_command_index_ = command_index;
    const uint32_t command_trace_limit =
        std::max(1u, REXCVAR_GET(gta4_trace_native_command_limit));
    const bool causally_significant_command =
        command.type == CommandType::kResolve || command.type == CommandType::kClear ||
        command.type == CommandType::kRenderPhaseMarker ||
        command.type == CommandType::kDepthSurfaceHandoff;
    deterministic_trace_active_ =
        trace_stages && (command_index < command_trace_limit || causally_significant_command);
    diagnostic_draw_id_ = 0;
    uint32_t diagnostic_guest_pc = 0;
    uint32_t diagnostic_guest_lr = 0;
    uint32_t diagnostic_primitive_type = 0;
    if (command.type == CommandType::kDrawIndexedPrimitive) {
      DrawIndexedPrimitiveCommand draw{};
      std::memcpy(&draw, command.bytes.data(), sizeof(draw));
      diagnostic_draw_id_ = draw.draw_id;
      diagnostic_guest_pc = 0x82A3E348;
      diagnostic_guest_lr = draw.caller;
      diagnostic_primitive_type = draw.primitive_type;
    } else if (command.type == CommandType::kDrawPrimitive) {
      DrawPrimitiveCommand draw{};
      std::memcpy(&draw, command.bytes.data(), sizeof(draw));
      diagnostic_draw_id_ = (uint64_t(submitted_frame) << 32) | uint32_t(command_index);
      diagnostic_primitive_type = draw.primitive_type;
    } else if (command.type == CommandType::kDrawPrimitiveUp) {
      DrawPrimitiveUpCommand draw{};
      std::memcpy(&draw, command.bytes.data(), sizeof(draw));
      diagnostic_draw_id_ = (uint64_t(submitted_frame) << 32) | uint32_t(command_index);
      diagnostic_primitive_type = draw.primitive_type;
    }
    LogVectorFontDraw(command, submitted_frame, command_index);
    TraceNativeRendererEvent(
        "command-begin",
        fmt::format("type={} phase={} object={:08X} state-version={}",
                    CommandTypeName(command.type), RenderPhaseName(command.render_phase),
                    command.render_phase_object,
                    command.pipeline_state ? command.pipeline_state->version : 0));
    const bool trace_command = trace_stages && (command_index < kIndexedFrameDetailedCommandLimit ||
                                                !(command_index % kIndexedFrameProgressInterval));
    if (trace_command) {
      REXLOG_WARN(
          "gta4-native-stage: trace={} frame={} stage=command-begin index={} total={} type={} "
          "pipelines={} textures={} surfaces={}",
          trace_sequence, submitted_frame, command_index, current_frame_.size(),
          CommandTypeName(command.type), native_pipelines_.size(), native_texture_images_.size(),
          native_surface_images_.size());
    }
    hash_bytes(&command.type, sizeof(command.type));
    hash_bytes(command.bytes.data(), command.bytes.size());
    if (command.type == CommandType::kRenderPhaseMarker) {
      RenderPhaseMarkerCommand marker{};
      std::memcpy(&marker, command.bytes.data(), sizeof(marker));
      TraceNativeRendererEvent(
          "phase-marker",
          fmt::format("phase={} event={} object={:08X} caller={:08X}",
                      RenderPhaseName(marker.phase),
                      marker.event == RenderPhaseEvent::kBegin ? "begin" : "end",
                      marker.object, marker.caller));
      postfx_scheduler.ObserveMarker(marker.phase, marker.event);
      continue;
    }
    if (command.pipeline_state) {
      const NativePipelineState& state = *command.pipeline_state;
      const uint64_t vertex_shader_hash =
          state.vertex_shader_resource ? state.vertex_shader_resource->hash : 0;
      const uint64_t pixel_shader_hash =
          state.pixel_shader_resource ? state.pixel_shader_resource->hash : 0;
      const uint64_t vertex_declaration_hash =
          state.vertex_declaration_resource ? state.vertex_declaration_resource->content_hash : 0;
      hash_bytes(&vertex_shader_hash, sizeof(vertex_shader_hash));
      hash_bytes(&pixel_shader_hash, sizeof(pixel_shader_hash));
      hash_bytes(&vertex_declaration_hash, sizeof(vertex_declaration_hash));
      hash_bytes(state.render_targets.data(), sizeof(state.render_targets));
      hash_bytes(&state.depth_stencil, sizeof(state.depth_stencil));
      hash_bytes(state.vertex_streams.data(), sizeof(state.vertex_streams));
      hash_bytes(state.textures.data(), sizeof(state.textures));
      hash_bytes(&state.index_buffer, sizeof(state.index_buffer));
      hash_bytes(&command.fixed_function_state, sizeof(command.fixed_function_state));

      if (deterministic_trace_active_ && diagnostic_draw_id_) {
        const auto summarize_constants = [&](size_t begin, size_t size) {
          uint32_t finite = 0;
          uint32_t nonfinite = 0;
          float minimum = std::numeric_limits<float>::infinity();
          float maximum = -std::numeric_limits<float>::infinity();
          float maximum_absolute = 0.0f;
          for (size_t offset = begin; offset < begin + size; offset += sizeof(uint32_t)) {
            const float value =
                std::bit_cast<float>(LoadGuestWord(command.device_snapshot, offset));
            if (!std::isfinite(value)) {
              ++nonfinite;
              continue;
            }
            ++finite;
            minimum = std::min(minimum, value);
            maximum = std::max(maximum, value);
            maximum_absolute = std::max(maximum_absolute, std::abs(value));
          }
          if (!finite) {
            minimum = 0.0f;
            maximum = 0.0f;
          }
          return std::tuple{finite, nonfinite, minimum, maximum, maximum_absolute};
        };
        const auto [vertex_finite, vertex_nonfinite, vertex_minimum, vertex_maximum,
                    vertex_maximum_absolute] =
            summarize_constants(kVertexConstantsOffset, kVertexConstantsSize);
        const auto [pixel_finite, pixel_nonfinite, pixel_minimum, pixel_maximum,
                    pixel_maximum_absolute] =
            summarize_constants(kPixelConstantsOffset, kPixelConstantsSize);
        if (pixel_shader_hash == 0x97AC30BA76D5515Full) {
          std::string selected_constants;
          constexpr std::array<uint32_t, 5> selected_registers = {37, 38, 39, 208, 209};
          for (uint32_t register_index : selected_registers) {
            selected_constants += fmt::format("{}c{}=[",
                                              selected_constants.empty() ? "" : ",",
                                              register_index);
            for (uint32_t component = 0; component < 4; ++component) {
              const size_t offset =
                  kPixelConstantsOffset +
                  (size_t(register_index) * 4 + component) * sizeof(uint32_t);
              const uint32_t word = LoadGuestWord(command.device_snapshot, offset);
              selected_constants += fmt::format(
                  "{}{:08X}:{:.9g}", component ? "," : "", word,
                  std::bit_cast<float>(word));
            }
            selected_constants += "]";
          }
          std::string nonfinite_constants;
          for (size_t offset = 0; offset < kPixelConstantsSize; offset += sizeof(uint32_t)) {
            const uint32_t word = LoadGuestWord(
                command.device_snapshot, kPixelConstantsOffset + offset);
            if (std::isfinite(std::bit_cast<float>(word))) {
              continue;
            }
            nonfinite_constants += fmt::format(
                "{}{}:{:08X}", nonfinite_constants.empty() ? "" : ",",
                offset / sizeof(uint32_t), word);
          }
          TraceNativeRendererEvent(
              "deferred-ps2-constants",
              fmt::format("selected=[{}] nonfinite=[{}]", selected_constants,
                          nonfinite_constants));
        }
        std::string textures;
        uint32_t requested_textures = 0;
        uint32_t captured_textures = 0;
        for (uint32_t stage = 0; stage < kShaderTextureCount; ++stage) {
          if (!state.textures[stage] && !command.textures[stage]) {
            continue;
          }
          ++requested_textures;
          captured_textures += command.textures[stage] != nullptr;
          textures += fmt::format(
              "{}{}:{:08X}@{}:{:016X}:f{}:{}x{}:m{}-{}", textures.empty() ? "" : ",",
              stage, state.textures[stage],
              command.textures[stage] ? command.textures[stage]->generation : 0,
              command.textures[stage] ? command.textures[stage]->content_hash : 0,
              command.textures[stage] ? uint32_t(command.textures[stage]->info.format) : 0,
              command.textures[stage] ? command.textures[stage]->info.width + 1 : 0,
              command.textures[stage] ? command.textures[stage]->info.height + 1 : 0,
              command.textures[stage] ? command.textures[stage]->info.mip_min_level : 0,
              command.textures[stage] ? command.textures[stage]->info.mip_max_level : 0);
        }
        const NativeFixedFunctionState& fixed = command.fixed_function_state;
        TraceNativeRendererEvent(
            "draw-state",
            fmt::format(
                "draw-id={:016X} guest={:08X}:{:08X} primitive={} "
                "vs={:08X}:{:016X} ps={:08X}:{:016X} declaration={:08X}:{:016X} "
                "constants={:016X}:{:016X} "
                "vstats={}:{}:{:.9g}:{:.9g}:{:.9g} pstats={}:{}:{:.9g}:{:.9g}:{:.9g} "
                "textures={}:{}:[{}] rt={:08X}:{:08X}:f{:08X}:{}x{}:s{} "
                "depth={:08X}:{:08X}:f{:08X}:{}x{}:s{}:wrapper{:08X}:caller{:08X} "
                "blend={}:{}:{}:{}:{}:{}:{} alpha={}:{}:{:.9g} "
                "z={}:{}:{} stencil={}:{} color-mask={:08X} "
                "viewport={:.9g},{:.9g},{:.9g},{:.9g},{:.9g},{:.9g} "
                "scissor={}:{},{},{},{}",
                diagnostic_draw_id_, diagnostic_guest_pc, diagnostic_guest_lr,
                diagnostic_primitive_type, state.vertex_shader, vertex_shader_hash,
                state.pixel_shader, pixel_shader_hash, state.vertex_declaration,
                vertex_declaration_hash, command.vertex_constants_hash,
                command.pixel_constants_hash, vertex_finite, vertex_nonfinite, vertex_minimum,
                vertex_maximum, vertex_maximum_absolute, pixel_finite, pixel_nonfinite,
                pixel_minimum, pixel_maximum, pixel_maximum_absolute, requested_textures,
                captured_textures, textures, state.render_targets[0].handle,
                state.render_targets[0].address, state.render_targets[0].format,
                state.render_targets[0].width, state.render_targets[0].height,
                state.render_targets[0].sample_type, state.depth_stencil.handle,
                state.depth_stencil.address, state.depth_stencil.format,
                state.depth_stencil.width, state.depth_stencil.height,
                state.depth_stencil.sample_type, state.depth_stencil_trace_wrapper,
                state.depth_stencil_trace_caller,
                fixed.blend_enable, fixed.source_blend,
                fixed.destination_blend, fixed.blend_operation, fixed.source_blend_alpha,
                fixed.destination_blend_alpha, fixed.blend_operation_alpha,
                fixed.alpha_test_enable, fixed.alpha_function, fixed.alpha_reference,
                fixed.depth_enable, fixed.depth_function, fixed.depth_write_enable,
                fixed.stencil_enable, fixed.stencil_function, fixed.color_write_mask,
                std::bit_cast<float>(fixed.viewport_bits[0]),
                std::bit_cast<float>(fixed.viewport_bits[1]),
                std::bit_cast<float>(fixed.viewport_bits[2]),
                std::bit_cast<float>(fixed.viewport_bits[3]),
                std::bit_cast<float>(fixed.viewport_bits[4]),
                std::bit_cast<float>(fixed.viewport_bits[5]), fixed.scissor_enable,
                fixed.scissor[0], fixed.scissor[1], fixed.scissor[2], fixed.scissor[3]));
        if (fixed.stencil_enable) {
          const uint32_t effective_back_function =
              fixed.two_sided_stencil ? fixed.ccw_stencil_function
                                      : fixed.stencil_function;
          const uint32_t effective_back_fail =
              fixed.two_sided_stencil ? fixed.ccw_stencil_fail
                                      : fixed.stencil_fail;
          const uint32_t effective_back_depth_fail =
              fixed.two_sided_stencil ? fixed.ccw_stencil_depth_fail
                                      : fixed.stencil_depth_fail;
          const uint32_t effective_back_pass =
              fixed.two_sided_stencil ? fixed.ccw_stencil_pass
                                      : fixed.stencil_pass;
          const uint32_t effective_back_reference =
              fixed.two_sided_stencil ? fixed.back_stencil_reference
                                      : fixed.stencil_reference;
          const uint32_t effective_back_mask =
              fixed.two_sided_stencil ? fixed.back_stencil_mask
                                      : fixed.stencil_mask;
          const uint32_t effective_back_write_mask =
              fixed.two_sided_stencil ? fixed.back_stencil_write_mask
                                      : fixed.stencil_write_mask;
          REXLOG_WARN(
              "gta4-native-cause: point=stencil-draw-state frame={} cmd={} "
              "draw={:016X} depth={}:{}:{} cull={} two-sided={} "
              "front=guest{}/{}/{}/{}:ref{:02X}:compare{:02X}:write{:02X}:"
              "vk{}/{}/{}/{} "
              "back=guest{}/{}/{}/{}:ref{:02X}:compare{:02X}:write{:02X}:"
              "vk{}/{}/{}/{}",
              submitted_frame, command_index, diagnostic_draw_id_,
              fixed.depth_enable, fixed.depth_function,
              fixed.depth_write_enable, fixed.cull_mode, fixed.two_sided_stencil,
              fixed.stencil_function, fixed.stencil_fail,
              fixed.stencil_depth_fail, fixed.stencil_pass,
              fixed.stencil_reference, fixed.stencil_mask,
              fixed.stencil_write_mask,
              uint32_t(ConvertCompareFunction(fixed.stencil_function)),
              uint32_t(ConvertStencilOperation(fixed.stencil_fail)),
              uint32_t(ConvertStencilOperation(fixed.stencil_depth_fail)),
              uint32_t(ConvertStencilOperation(fixed.stencil_pass)),
              effective_back_function, effective_back_fail,
              effective_back_depth_fail, effective_back_pass,
              effective_back_reference, effective_back_mask,
              effective_back_write_mask,
              uint32_t(ConvertCompareFunction(effective_back_function)),
              uint32_t(ConvertStencilOperation(effective_back_fail)),
              uint32_t(ConvertStencilOperation(effective_back_depth_fail)),
              uint32_t(ConvertStencilOperation(effective_back_pass)));
        }
      }

      if (diagnostic_draw_id_ && transition_trace) {
        transition::Record(
            transition::EventSource::kRenderer, transition::EventType::kDrawMetadata,
            diagnostic_guest_pc, diagnostic_guest_lr, submitted_frame, transition::kFlagNone,
            diagnostic_draw_id_, vertex_shader_hash, pixel_shader_hash);
        if (command.type == CommandType::kDrawIndexedPrimitive) {
          DrawIndexedPrimitiveCommand draw{};
          std::memcpy(&draw, command.bytes.data(), sizeof(draw));
          transition::Record(
              transition::EventSource::kRenderer,
              transition::EventType::kDrawMetadata, diagnostic_guest_pc,
              diagnostic_guest_lr, submitted_frame, transition::kFlagStateChanged,
              diagnostic_draw_id_,
              (uint64_t(draw.origin_flags) << 32) | draw.command_list,
              command_index);
        }
        transition::Record(
            transition::EventSource::kRenderer, transition::EventType::kDrawMetadata,
            diagnostic_guest_pc, diagnostic_guest_lr, submitted_frame, transition::kFlagAfter,
            diagnostic_draw_id_, vertex_declaration_hash,
            command.pipeline_state->vertex_declaration_resource
                ? command.pipeline_state->vertex_declaration_resource->generation
                : 0);
        for (uint32_t stream = 0; stream < command.vertex_buffers.size(); ++stream) {
          const auto& vertex_buffer = command.vertex_buffers[stream];
          if (vertex_buffer) {
            const uint64_t identity =
                (uint64_t(stream) << 32) | vertex_buffer->handle;
            transition::Record(
                transition::EventSource::kRenderer,
                transition::EventType::kResourceGeneration, diagnostic_guest_pc,
                diagnostic_guest_lr, submitted_frame, transition::kFlagBefore,
                diagnostic_draw_id_, identity, vertex_buffer->generation);
            transition::Record(
                transition::EventSource::kRenderer,
                transition::EventType::kResourceGeneration, diagnostic_guest_pc,
                diagnostic_guest_lr, submitted_frame, transition::kFlagAfter,
                diagnostic_draw_id_, identity, vertex_buffer->content_hash);
          }
        }
        if (command.index_buffer) {
          const uint64_t identity =
              (uint64_t(UINT32_MAX) << 32) | command.index_buffer->handle;
          transition::Record(
              transition::EventSource::kRenderer,
              transition::EventType::kResourceGeneration, diagnostic_guest_pc,
              diagnostic_guest_lr, submitted_frame, transition::kFlagBefore,
              diagnostic_draw_id_, identity, command.index_buffer->generation);
          transition::Record(
              transition::EventSource::kRenderer,
              transition::EventType::kResourceGeneration, diagnostic_guest_pc,
              diagnostic_guest_lr, submitted_frame, transition::kFlagAfter,
              diagnostic_draw_id_, identity, command.index_buffer->content_hash);
        }
        transition::Record(
            transition::EventSource::kRenderer, transition::EventType::kFirstDrawState,
            diagnostic_guest_pc, diagnostic_guest_lr, submitted_frame, transition::kFlagNone,
            diagnostic_draw_id_,
            (uint64_t(uint32_t(command.type)) << 32) | diagnostic_primitive_type,
            command.vertex_constants_hash ^ command.pixel_constants_hash);
        uint32_t finite_constant_count = 0;
        uint32_t nonfinite_constant_count = 0;
        float maximum_absolute_constant = 0.0f;
        for (size_t offset = kVertexConstantsOffset;
             offset < kVertexConstantsOffset + kVertexConstantsSize;
             offset += sizeof(uint32_t)) {
          const float value = std::bit_cast<float>(LoadGuestWord(command.device_snapshot, offset));
          if (std::isfinite(value)) {
            ++finite_constant_count;
            maximum_absolute_constant = std::max(maximum_absolute_constant, std::abs(value));
          } else {
            ++nonfinite_constant_count;
          }
        }
        transition::Record(
            transition::EventSource::kRenderer, transition::EventType::kVertexConstants,
            diagnostic_guest_pc, diagnostic_guest_lr, submitted_frame,
            transition::kFlagBefore,
            diagnostic_draw_id_,
            (uint64_t(finite_constant_count) << 32) | nonfinite_constant_count,
            command.vertex_constants_hash);
        transition::Record(
            transition::EventSource::kRenderer, transition::EventType::kVertexConstants,
            diagnostic_guest_pc, diagnostic_guest_lr, submitted_frame, transition::kFlagAfter,
            diagnostic_draw_id_, std::bit_cast<uint32_t>(maximum_absolute_constant),
            command.pixel_constants_hash);
      }

      const SurfaceDescriptor& color_target = state.render_targets[0];
      if (!saw_render_target) {
        first_render_target_handle = color_target.handle;
        first_render_target_address = color_target.address;
        saw_render_target = true;
      }
      last_render_target_handle = color_target.handle;
      last_render_target_address = color_target.address;

      const bool writes_render_target =
          command.type == CommandType::kDrawPrimitive ||
          command.type == CommandType::kDrawPrimitiveUp ||
          command.type == CommandType::kDrawIndexedPrimitive || command.type == CommandType::kClear;
      if (diagnostic_frame && writes_render_target &&
          (!saw_traced_render_target || color_target.handle != traced_render_target_handle)) {
        std::fprintf(
            stderr,
            "[RTTrace] frame=%u index=%zu event=target-transition type=%s phase=%s "
            "rt0=%08X/%08X/f%08X/%ux%u/s%u/base%08X/dim%08X "
            "rt1=%08X/%08X rt2=%08X/%08X rt3=%08X/%08X\n",
            submitted_frame, command_index, CommandTypeName(command.type),
            RenderPhaseName(command.render_phase), state.render_targets[0].handle,
            state.render_targets[0].address, state.render_targets[0].format,
            state.render_targets[0].width, state.render_targets[0].height,
            state.render_targets[0].sample_type, state.render_targets[0].base,
            state.render_targets[0].packed_dimensions, state.render_targets[1].handle,
            state.render_targets[1].address, state.render_targets[2].handle,
            state.render_targets[2].address, state.render_targets[3].handle,
            state.render_targets[3].address);
        std::fflush(stderr);
        saw_traced_render_target = true;
        traced_render_target_handle = color_target.handle;
      }
    }
    if (command.type == CommandType::kDepthSurfaceHandoff) {
      DepthSurfaceHandoffCommand handoff{};
      std::memcpy(&handoff, command.bytes.data(), sizeof(handoff));
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      TraceNativeRendererEvent(
          "explicit-depth-handoff-begin",
          fmt::format(
              "source-wrapper={:08X} source={:08X}/{:08X}:{}x{}:s{} "
              "destination-wrapper={:08X} destination={:08X}/{:08X}:{}x{}:s{} "
              "caller={:08X}",
              handoff.source_wrapper, handoff.source.handle, handoff.source.address,
              handoff.source.width, handoff.source.height, handoff.source.sample_type,
              handoff.destination_wrapper, handoff.destination.handle,
              handoff.destination.address, handoff.destination.width,
              handoff.destination.height, handoff.destination.sample_type,
              handoff.trace_caller));
      const bool handoff_recorded =
          RecordDepthSurfaceHandoff(command_buffer, command, submitted_frame);
      TraceNativeRendererEvent(
          "command-result",
          fmt::format("type={} recorded={} depth=true stencil=false",
                      CommandTypeName(command.type), handoff_recorded));
      if (!handoff_recorded) {
        REXLOG_ERROR(
            "gta4-native-cause: point=explicit-depth-handoff frame={} cmd={} "
            "result=rejected source-wrapper={:08X} source={:08X} "
            "destination-wrapper={:08X} destination={:08X}",
            submitted_frame, command_index, handoff.source_wrapper,
            handoff.source.handle, handoff.destination_wrapper,
            handoff.destination.handle);
      }
      continue;
    }
    if (command.type == CommandType::kResolve) {
      ++queued_resolves;
      ResolveCommand resolve{};
      std::memcpy(&resolve, command.bytes.data(), sizeof(resolve));
      TraceNativeRendererEvent(
          "resolve-begin",
          fmt::format(
              "source={:08X}:{:08X}:f{:08X}:{}x{}:s{}:base{:08X}:dim{:08X} "
              "destination={:08X}@{} flags={:08X} formats={:08X}:{}:{:08X} "
              "rect={}:{},{},{},{} point={}:{},{} level={} slice={} "
              "clear={:08X},{:08X},{:08X},{:08X}:{:016X}:{} "
              "origin={} caller={:08X} owner={:08X} source-wrapper={:08X} "
              "destination-meta=hash{:016X}:gpu{}:f{}:{}x{}:pitch{}:base{:08X}",
              resolve.source.handle, resolve.source.address, resolve.source.format,
              resolve.source.width, resolve.source.height, resolve.source.sample_type,
              resolve.source.base, resolve.source.packed_dimensions,
              resolve.destination_texture,
              command.resolve_destination ? command.resolve_destination->generation : 0,
              resolve.flags, resolve.color_format, resolve.color_exp_bias,
              resolve.depth_format, resolve.source_rectangle_valid,
              resolve.source_rectangle.left, resolve.source_rectangle.top,
              resolve.source_rectangle.right, resolve.source_rectangle.bottom,
              resolve.destination_point_valid, resolve.destination_point.x,
              resolve.destination_point.y, resolve.destination_level,
              resolve.destination_slice_or_face, resolve.clear_color_bits[0],
              resolve.clear_color_bits[1], resolve.clear_color_bits[2],
              resolve.clear_color_bits[3], resolve.clear_depth_bits,
              resolve.clear_stencil, resolve.trace_origin, resolve.trace_caller,
              resolve.trace_owner, resolve.trace_source_wrapper,
              command.resolve_destination ? command.resolve_destination->content_hash : 0,
              command.resolve_destination ? command.resolve_destination->gpu_produced : false,
              command.resolve_destination
                  ? uint32_t(command.resolve_destination->info.format)
                  : 0,
              command.resolve_destination ? command.resolve_destination->info.width + 1 : 0,
              command.resolve_destination ? command.resolve_destination->info.height + 1 : 0,
              command.resolve_destination ? command.resolve_destination->info.pitch : 0,
              command.resolve_destination
                  ? command.resolve_destination->info.memory.base_address
                  : 0));
      if (diagnostic_frame) {
        const SurfaceDescriptor empty_target{};
        const SurfaceDescriptor& state_target =
            command.pipeline_state ? command.pipeline_state->render_targets[0] : empty_target;
        std::fprintf(
            stderr,
            "[RTTrace] frame=%u index=%zu event=resolve phase=%s state-rt0=%08X/%08X "
            "source=%08X/%08X destination=%08X flags=%08X\n",
            submitted_frame, command_index, RenderPhaseName(command.render_phase),
            state_target.handle, state_target.address, resolve.source.handle, resolve.source.address,
            resolve.destination_texture, resolve.flags);
        std::fflush(stderr);
      }
      const uint64_t destination_generation =
          command.resolve_destination ? command.resolve_destination->generation : 0;
      if (!saw_resolve) {
        first_resolve_source_handle = resolve.source.handle;
        first_resolve_source_address = resolve.source.address;
        first_resolve_destination = resolve.destination_texture;
        first_resolve_generation = destination_generation;
        saw_resolve = true;
      }
      last_resolve_source_handle = resolve.source.handle;
      last_resolve_source_address = resolve.source.address;
      last_resolve_destination = resolve.destination_texture;
      last_resolve_generation = destination_generation;
      const bool resolve_matches_present =
          present_source && command.resolve_destination &&
          present_source->generation == command.resolve_destination->generation;
      const auto reflection_registration = command.resolve_destination
                                               ? reflection_resources_.find(
                                                     command.resolve_destination->handle)
                                               : reflection_resources_.end();
      const bool reflection_resolve =
          reflection_registration != reflection_resources_.end();
      if (resolve_matches_present) {
        present_matches_frame_resolve = true;
      }
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      if (diagnostic_frame && (resolve.flags & 0x04000000u) &&
          InitializeContentProbeBuffer()) {
        if (content_probe_buffer_.pending_frame != submitted_frame) {
          std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
          content_probe_buffer_.pending_frame = submitted_frame;
          content_probe_buffer_.stages = {};
        }
        uint32_t probe_stage = 0;
        while (probe_stage < content_probe_buffer_.stages.size() &&
               content_probe_buffer_.stages[probe_stage].valid) {
          ++probe_stage;
        }
        NativeSurfaceImage* resolve_source = GetOrCreateSurfaceImage(resolve.source, false);
        if (const NativePlacementOwner* owner = FindPlacementOwner(resolve.source, false)) {
          resolve_source = owner->image;
        }
        if (resolve_source) {
          RecordContentProbeImage(
              command_buffer, probe_stage, resolve_source->resource.image,
              resolve_source->format, resolve_source->layout, resolve_source->width,
              resolve_source->height, resolve.source.handle, resolve.source.address, 5);
        }
      }
      if (diagnostic_frame && reflection_resolve && InitializeContentProbeBuffer()) {
        if (content_probe_buffer_.pending_frame != submitted_frame) {
          std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
          content_probe_buffer_.pending_frame = submitted_frame;
          content_probe_buffer_.stages = {};
        }
        uint32_t probe_stage = 0;
        while (probe_stage < content_probe_buffer_.stages.size() &&
               content_probe_buffer_.stages[probe_stage].valid) {
          ++probe_stage;
        }
        const bool depth_resolve = (resolve.flags & 7u) == 4u;
        NativeSurfaceImage* resolve_source =
            GetOrCreateSurfaceImage(resolve.source, depth_resolve);
        if (const NativePlacementOwner* owner = FindPlacementOwner(resolve.source, depth_resolve)) {
          resolve_source = owner->image;
        }
        if (resolve_source &&
            RecordContentProbeImage(
                command_buffer, probe_stage, resolve_source->resource.image,
                resolve_source->format, resolve_source->layout, resolve_source->width,
                resolve_source->height, resolve.source.handle, resolve.source.address, 12)) {
          NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
          stage.command_index = uint32_t(command_index);
          stage.render_phase = uint32_t(command.render_phase);
        }
      }
      const bool resolve_recorded = RecordResolve(command_buffer, command, submitted_frame);
      if (resolve_recorded) {
        recorded_draw = true;
        ++successful_resolves;
        if (diagnostic_frame && reflection_resolve && InitializeContentProbeBuffer()) {
          const auto destination_image = command.resolve_destination
                                             ? native_texture_images_.find(
                                                   command.resolve_destination->generation)
                                             : native_texture_images_.end();
          if (destination_image != native_texture_images_.end() && destination_image->second &&
              resolve.destination_level < destination_image->second->mip_levels) {
            NativeTextureImage& image = *destination_image->second;
            uint32_t probe_stage = 0;
            while (probe_stage < content_probe_buffer_.stages.size() &&
                   content_probe_buffer_.stages[probe_stage].valid) {
              ++probe_stage;
            }
            const uint32_t mip_width =
                CalculateNativeMipExtent(image.width, resolve.destination_level);
            const uint32_t mip_height =
                CalculateNativeMipExtent(image.height, resolve.destination_level);
            if (RecordContentProbeImage(
                    command_buffer, probe_stage, image.resource.image, image.format,
                    image.layout, mip_width, mip_height,
                    image.source ? image.source->handle : 0,
                    image.source ? image.source->info.memory.base_address : 0, 13,
                    resolve.destination_level)) {
              NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
              stage.command_index = uint32_t(command_index);
              stage.render_phase = uint32_t(command.render_phase);
            }
          }
        }
        if (resolve_matches_present && (resolve.flags & 7) != 4) {
          NativeSurfaceImage* resolved_source = GetOrCreateSurfaceImage(resolve.source, false);
          if (resolved_source && resolved_source->format == VK_FORMAT_R16G16B16A16_SFLOAT &&
              resolved_source->samples == VK_SAMPLE_COUNT_1_BIT) {
            high_precision_present_source = resolved_source;
          }
        }
      } else {
        ++failed_resolves;
      }
      if (trace_command) {
        REXLOG_WARN(
            "gta4-native-stage: trace={} frame={} stage=command-end index={} type={} "
            "resolve-ok={}",
            trace_sequence, submitted_frame, command_index, CommandTypeName(command.type),
            failed_resolves == 0);
      }
      continue;
    }
    switch (command.type) {
      case CommandType::kDrawPrimitive:
        ++queued_draws;
        break;
      case CommandType::kDrawPrimitiveUp:
        ++queued_draws_up;
        break;
      case CommandType::kDrawIndexedPrimitive:
        ++queued_draws_indexed;
        break;
      case CommandType::kClear:
        ++queued_clears;
        break;
      default:
        break;
    }
    const bool is_draw = command.type == CommandType::kDrawPrimitive ||
                         command.type == CommandType::kDrawPrimitiveUp ||
                         command.type == CommandType::kDrawIndexedPrimitive;
    const bool trace_final_draw =
        diagnostic_frame && is_draw &&
        ((have_previous_final_draw && command_index == previous_final_draw_index) ||
         (have_last_final_draw && command_index == last_final_draw_index));
    if (trace_final_draw && command.pipeline_state) {
      const NativePipelineState& state = *command.pipeline_state;
      const NativeShader* vertex_shader = state.vertex_shader_resource;
      const NativeShader* pixel_shader = state.pixel_shader_resource;
      const NativeTextureResource* texture_two = command.textures[2].get();
      std::fprintf(
          stderr,
          "[FinalDrawTrace] frame=%u index=%zu type=%s phase=%s target=%08X/%08X "
          "vs=%08X/%016llX ps=%08X/%016llX constants=%016llX/%016llX "
          "textures=%08X,%08X,%08X,%08X tex2-generation=%llu tex2-hash=%016llX "
          "blend=%u depth=%u/%u color-mask=%08X\n",
          submitted_frame, command_index, CommandTypeName(command.type),
          RenderPhaseName(command.render_phase), state.render_targets[0].handle,
          state.render_targets[0].address, state.vertex_shader,
          static_cast<unsigned long long>(vertex_shader ? vertex_shader->hash : 0),
          state.pixel_shader,
          static_cast<unsigned long long>(pixel_shader ? pixel_shader->hash : 0),
          static_cast<unsigned long long>(command.vertex_constants_hash),
          static_cast<unsigned long long>(command.pixel_constants_hash), state.textures[0],
          state.textures[1], state.textures[2], state.textures[3],
          static_cast<unsigned long long>(texture_two ? texture_two->generation : 0),
          static_cast<unsigned long long>(texture_two ? texture_two->content_hash : 0),
          command.fixed_function_state.blend_enable, command.fixed_function_state.depth_enable,
          command.fixed_function_state.depth_write_enable,
          command.fixed_function_state.color_write_mask);
      std::fflush(stderr);
    }
    if (have_last_final_draw && command_index == last_final_draw_index && command.textures[2]) {
      const auto input_image = native_texture_images_.find(command.textures[2]->generation);
      if (input_image != native_texture_images_.end()) {
        final_composite_input = input_image->second.get();
      }
    }
    if (is_draw && postfx_scheduler.scene_capture_pending()) {
      constexpr uint32_t kStippleMaskTextureStage = 0;
      constexpr uint32_t kCompositeDepthTextureStage = 1;
      constexpr uint32_t kCompositeInputTextureStage = 2;
      NativeTextureImage* composite_input =
          GetOrCreateTextureImage(command_buffer, command.textures[kCompositeInputTextureStage]);
      bool captured = false;
      if (composite_input && composite_input->aspect == VK_IMAGE_ASPECT_COLOR_BIT &&
          composite_input->source->info.dimension == xenos::DataDimension::k2DOrStacked &&
          !composite_input->source->info.is_stacked &&
          composite_input->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        if (rendering) {
          dfn.vkCmdEndRendering(command_buffer);
          rendering = false;
        }
        captured = postfx_resource_pool_.RecordSceneSnapshot(
            command_buffer, vulkan_provider->vulkan_device(), composite_input->resource.image,
            composite_input->format, composite_input->layout,
            {composite_input->width, composite_input->height});
        NativeTextureImage* depth_input = GetOrCreateTextureImage(
            command_buffer, command.textures[kCompositeDepthTextureStage]);
        NativeTextureImage* stipple_mask = GetOrCreateTextureImage(
            command_buffer, command.textures[kStippleMaskTextureStage]);
        const bool compatible_depth =
            depth_input && depth_input->source->info.dimension == xenos::DataDimension::k2DOrStacked &&
            !depth_input->source->info.is_stacked &&
            depth_input->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        const bool compatible_mask =
            stipple_mask && stipple_mask->aspect == VK_IMAGE_ASPECT_COLOR_BIT &&
            stipple_mask->source->info.dimension == xenos::DataDimension::k2DOrStacked &&
            !stipple_mask->source->info.is_stacked &&
            stipple_mask->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        if (captured && compatible_depth && compatible_mask) {
          auto load_vector = [&command](size_t offset) {
            std::array<float, 4> result{};
            for (uint32_t component = 0; component < result.size(); ++component) {
              result[component] = std::bit_cast<float>(
                  LoadGuestWord(command.device_snapshot, offset + component * sizeof(uint32_t)));
            }
            return result;
          };
          SplitPostFxParameters parameters{};
          parameters.dof_projection = load_vector(kDofProjectionSnapshotOffset);
          parameters.dof_distance = load_vector(kDofDistanceSnapshotOffset);
          parameters.dof_blur = load_vector(kDofBlurSnapshotOffset);
          parameters.depth_source = PostFxDepthSource::kCurrentCompositeTexture;
          captured = split_postfx_pass_.Record(
              command_buffer, vulkan_provider->vulkan_device(), frame_descriptor_pool_,
              native_pipeline_cache_, composite_input->resource.image,
              composite_input->resource.view, depth_input->resource.view,
              stipple_mask->resource.view, composite_input->format,
              {composite_input->width, composite_input->height}, parameters,
              postfx_resource_pool_);
          if (captured) {
            const SunShaftParameters sun_parameters =
                BuildSunShaftParameters(environmental_data.get());
            captured = sun_shafts_pass_.Record(
                command_buffer, vulkan_provider->vulkan_device(), frame_descriptor_pool_,
                native_pipeline_cache_, composite_input->resource.image,
                composite_input->resource.view, depth_input->resource.view,
                composite_input->format, {composite_input->width, composite_input->height},
                sun_parameters, postfx_resource_pool_);
          }
        } else if (captured) {
          captured = false;
        }
      }
      postfx_scheduler.FinishSceneCapture(captured);
      static std::atomic<uint64_t> composite_capture_count{0};
      const uint64_t capture_index = ++composite_capture_count;
      if (capture_index <= 16 || !(capture_index % 4096)) {
        REXLOG_INFO(
            "gta4-native-postfx: composite input capture={} result={} texture={:08X}@{} "
            "format={} extent={}x{}",
            capture_index, captured ? "ok" : "unavailable",
            composite_input && composite_input->source ? composite_input->source->handle : 0,
            composite_input && composite_input->source ? composite_input->source->generation : 0,
            composite_input ? uint32_t(composite_input->format) : 0,
            composite_input ? composite_input->width : 0,
            composite_input ? composite_input->height : 0);
      }
    }
    if (is_draw) {
      uint32_t primitive_type = 0;
      size_t route_index = 0;
      if (command.type == CommandType::kDrawPrimitive) {
        DrawPrimitiveCommand draw{};
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        primitive_type = draw.primitive_type;
      } else if (command.type == CommandType::kDrawPrimitiveUp) {
        DrawPrimitiveUpCommand draw{};
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        primitive_type = draw.primitive_type;
        route_index = 1;
      } else {
        DrawIndexedPrimitiveCommand draw{};
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        primitive_type = draw.primitive_type;
        route_index = 2;
      }
      if (primitive_type < primitive_counts[route_index].size()) {
        ++primitive_counts[route_index][primitive_type];
      }
      uint32_t requested_for_draw = 0;
      uint32_t captured_for_draw = 0;
      uint32_t bound_for_draw = 0;
      uint32_t first_missing_stage = kShaderTextureCount;
      uint32_t first_missing_handle = 0;
      for (uint32_t stage = 0; stage < kShaderTextureCount; ++stage) {
        const uint32_t handle =
            LoadGuestWord(command.device_snapshot, kTextureHandleBase + stage * sizeof(uint32_t));
        if (!handle) {
          continue;
        }
        ++requested_for_draw;
        if (command.textures[stage]) {
          ++captured_for_draw;
        }
        if (command.textures[stage] && command.draw_descriptor_sets[0]) {
          ++bound_for_draw;
        } else if (first_missing_stage == kShaderTextureCount) {
          first_missing_stage = stage;
          first_missing_handle = handle;
        }
      }
      texture_requested_bindings += requested_for_draw;
      texture_captured_bindings += captured_for_draw;
      texture_bound_bindings += bound_for_draw;
      if (requested_for_draw) {
        ++textured_draws;
      }
      if (bound_for_draw != requested_for_draw) {
        ++draws_with_missing_textures;
        const NativeShader* vertex_shader =
            command.pipeline_state ? command.pipeline_state->vertex_shader_resource : nullptr;
        const NativeShader* pixel_shader =
            command.pipeline_state ? command.pipeline_state->pixel_shader_resource : nullptr;
        static std::mutex missing_texture_cause_mutex;
        static std::set<std::tuple<uint64_t, uint64_t, uint32_t, uint32_t>>
            logged_missing_texture_causes;
        {
          std::lock_guard lock(missing_texture_cause_mutex);
          const auto cause_key = std::make_tuple(
              vertex_shader ? vertex_shader->hash : 0,
              pixel_shader ? pixel_shader->hash : 0, first_missing_stage,
              first_missing_handle);
          if (logged_missing_texture_causes.insert(cause_key).second) {
            const SurfaceDescriptor target =
                command.pipeline_state ? command.pipeline_state->render_targets[0]
                                       : SurfaceDescriptor{};
            const auto missing_resource =
                first_missing_stage < command.textures.size()
                    ? command.textures[first_missing_stage]
                    : std::shared_ptr<const NativeTextureResource>{};
            REXLOG_ERROR(
                "gta4-native-cause: point=missing-texture-draw frame={} cmd={} "
                "draw-id={:016X} requested={} captured={} bound={} missing-stage={} "
                "missing-handle={:08X} missing-generation={} target={:08X}/{:08X}:{}x{} "
                "vs={:016X}({}) ps={:016X}({})",
                submitted_frame, command_index, diagnostic_draw_id_, requested_for_draw,
                captured_for_draw, bound_for_draw, first_missing_stage, first_missing_handle,
                missing_resource ? missing_resource->generation : 0, target.handle,
                target.address, target.width, target.height,
                vertex_shader ? vertex_shader->hash : 0,
                vertex_shader ? vertex_shader->filename : "missing",
                pixel_shader ? pixel_shader->hash : 0,
                pixel_shader ? pixel_shader->filename : "missing");
          }
        }
        if (ShouldLogDiagnosticFrame(submitted_frame) && texture_failure_examples < 2) {
          ++texture_failure_examples;
          REXLOG_WARN(
              "gta4-native-diag: frame={} missing texture draw #{} requested={} captured={} "
              "bound={} first-stage={} first-handle={:08X} vs={:016X}({}) ps={:016X}({})",
              submitted_frame, texture_failure_examples, requested_for_draw, captured_for_draw,
              bound_for_draw, first_missing_stage, first_missing_handle,
              vertex_shader ? vertex_shader->hash : 0,
              vertex_shader ? vertex_shader->filename : "missing",
              pixel_shader ? pixel_shader->hash : 0,
              pixel_shader ? pixel_shader->filename : "missing");
        }
      }
      alpha_blend_draws += command.fixed_function_state.blend_enable != 0;
      alpha_test_draws += command.fixed_function_state.alpha_test_enable != 0;
      z_disabled_draws += command.fixed_function_state.depth_enable == 0;
      z_write_disabled_draws += command.fixed_function_state.depth_write_enable == 0;
      nondefault_color_write_draws += command.fixed_function_state.color_write_mask != 0xF;
      if (legacy_diagnostics && invocation <= 16 && fixed_state_examples < 2) {
        ++fixed_state_examples;
        const NativeFixedFunctionState& fixed = command.fixed_function_state;
        REXLOG_INFO(
            "gta4-native-state: frame={} sample={} depth={}/{}/{} cull={} "
            "blend={}/{}/{}/{} alpha={}/{}/{} stencil={}/{} color-mask={:08X}->{:X} "
            "viewport-bits={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
            "viewport={},{},{},{},{},{} scissor-enable={} scissor={},{},{},{}",
            submitted_frame, fixed_state_examples, fixed.depth_enable, fixed.depth_function,
            fixed.depth_write_enable, fixed.cull_mode, fixed.blend_enable, fixed.source_blend,
            fixed.destination_blend, fixed.blend_operation, fixed.alpha_test_enable,
            fixed.alpha_function, fixed.alpha_reference, fixed.stencil_enable,
            fixed.stencil_function, fixed.color_write_mask,
            uint32_t(ConvertColorWriteMask(fixed.color_write_mask)), fixed.viewport_bits[0],
            fixed.viewport_bits[1], fixed.viewport_bits[2], fixed.viewport_bits[3],
            fixed.viewport_bits[4], fixed.viewport_bits[5],
            std::bit_cast<float>(fixed.viewport_bits[0]),
            std::bit_cast<float>(fixed.viewport_bits[1]),
            std::bit_cast<float>(fixed.viewport_bits[2]),
            std::bit_cast<float>(fixed.viewport_bits[3]),
            std::bit_cast<float>(fixed.viewport_bits[4]),
            std::bit_cast<float>(fixed.viewport_bits[5]), fixed.scissor_enable, fixed.scissor[0],
            fixed.scissor[1], fixed.scissor[2], fixed.scissor[3]);
      }
    }
    if (!command.pipeline_state) {
      ++missing_pipeline_state;
      TraceNativeRendererEvent("command-rejected", "reason=missing-pipeline-state");
      if (trace_command) {
        REXLOG_WARN(
            "gta4-native-stage: trace={} frame={} stage=command-end index={} type={} "
            "result=missing-pipeline-state",
            trace_sequence, submitted_frame, command_index, CommandTypeName(command.type));
      }
      continue;
    }
    NativeRenderingTarget target;
    if (!ResolveRenderingTarget(*command.pipeline_state, presenter_view, width, height, target)) {
      ++target_failures;
      TraceNativeRendererEvent("command-rejected", "reason=render-target-resolution-failed");
      if (diagnostic_frame && command.type == CommandType::kClear) {
        ClearCommand clear{};
        std::memcpy(&clear, command.bytes.data(), sizeof(clear));
        const SurfaceDescriptor& rt0 = command.pipeline_state->render_targets[0];
        const SurfaceDescriptor& depth = command.pipeline_state->depth_stencil;
        std::fprintf(
            stderr,
            "[ClearTargetReject] frame=%u index=%zu flags=%08X rect=%d,%d,%d,%d "
            "rt0=%08X/%08X/f%08X/%ux%u/s%u/base%08X/dim%08X "
            "depth=%08X/%08X/f%08X/%ux%u/s%u/base%08X/dim%08X\n",
            submitted_frame, command_index, clear.flags, clear.left, clear.top, clear.right,
            clear.bottom, rt0.handle, rt0.address, rt0.format, rt0.width, rt0.height,
            rt0.sample_type, rt0.base, rt0.packed_dimensions, depth.handle, depth.address,
            depth.format, depth.width, depth.height, depth.sample_type, depth.base,
            depth.packed_dimensions);
        std::fflush(stderr);
        REXLOG_ERROR(
            "gta4-native-cause: point=clear-target-rejected frame={} cmd={} flags={:08X} "
            "rect={},{},{},{} rt0={:08X}/{:08X}/f{:08X}/{}x{}/s{}/base{:08X}/dim{:08X} "
            "depth={:08X}/{:08X}/f{:08X}/{}x{}/s{}/base{:08X}/dim{:08X}",
            submitted_frame, command_index, clear.flags, clear.left, clear.top, clear.right,
            clear.bottom, rt0.handle, rt0.address, rt0.format, rt0.width, rt0.height,
            rt0.sample_type, rt0.base, rt0.packed_dimensions, depth.handle, depth.address,
            depth.format, depth.width, depth.height, depth.sample_type, depth.base,
            depth.packed_dimensions);
      }
      if (trace_command) {
        REXLOG_WARN(
            "gta4-native-stage: trace={} frame={} stage=command-end index={} type={} "
            "result=target-rejected",
            trace_sequence, submitted_frame, command_index, CommandTypeName(command.type));
      }
      continue;
    }
    if (target.uses_presenter) {
      ++presenter_target_commands;
    } else {
      ++offscreen_target_commands;
    }
    const auto pending_scene_checkpoint =
        scene_write_checkpoint_ordinals.find(command_index);
    if (pending_scene_checkpoint != scene_write_checkpoint_ordinals.end() &&
        pending_scene_checkpoint->second == 1 && target.color_surfaces[0] &&
        target.color_surfaces[0]->format == VK_FORMAT_R16G16B16A16_SFLOAT &&
        InitializeContentProbeBuffer()) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      if (content_probe_buffer_.pending_frame != submitted_frame) {
        std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
        content_probe_buffer_.pending_frame = submitted_frame;
        content_probe_buffer_.stages = {};
      }
      uint32_t probe_stage = 0;
      while (probe_stage < content_probe_buffer_.stages.size() &&
             content_probe_buffer_.stages[probe_stage].valid) {
        ++probe_stage;
      }
      NativeSurfaceImage* draw_target = target.color_surfaces[0];
      const bool checkpoint_recorded = RecordContentProbeImage(
          command_buffer, probe_stage, draw_target->resource.image, draw_target->format,
          draw_target->layout, draw_target->width, draw_target->height,
          draw_target->descriptor.handle,
          command.pipeline_state->render_targets[0].address, 9);
      if (checkpoint_recorded) {
        NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
        const NativeShader* vertex_shader = command.pipeline_state->vertex_shader_resource;
        const NativeShader* pixel_shader = command.pipeline_state->pixel_shader_resource;
        stage.command_type = uint8_t(command.type);
        stage.command_index = uint32_t(command_index);
        stage.checkpoint_ordinal = 0;
        stage.checkpoint_count = scene_write_checkpoint_count;
        stage.render_phase = uint32_t(command.render_phase);
        stage.draw_id = diagnostic_draw_id_;
        stage.vertex_shader_hash = vertex_shader ? vertex_shader->hash : 0;
        stage.pixel_shader_hash = pixel_shader ? pixel_shader->hash : 0;
        for (uint32_t texture_stage = 0; texture_stage < stage.texture_handles.size();
             ++texture_stage) {
          stage.texture_handles[texture_stage] =
              command.textures[texture_stage] ? command.textures[texture_stage]->handle : 0;
        }
        REXLOG_INFO(
            "gta4-native-cause: point=scene-before-first-write-recorded frame={} "
            "cmd={} target={:08X}/{:08X} draw={:016X} vs={:016X} ps={:016X}",
            submitted_frame, stage.command_index, stage.handle, stage.address,
            stage.draw_id, stage.vertex_shader_hash, stage.pixel_shader_hash);
      }
      const NativeShader* vertex_shader = command.pipeline_state->vertex_shader_resource;
      const NativeShader* pixel_shader = command.pipeline_state->pixel_shader_resource;
      for (uint32_t texture_stage = 0; texture_stage < kShaderTextureCount; ++texture_stage) {
        const std::shared_ptr<const NativeTextureResource>& texture =
            command.textures[texture_stage];
        if (!texture || !command.pipeline_state->textures[texture_stage]) {
          continue;
        }
        NativeTextureImage* image = GetOrCreateTextureImage(command_buffer, texture);
        const bool probe_format_supported =
            image && (image->format == VK_FORMAT_R16G16B16A16_SFLOAT ||
                      image->format == VK_FORMAT_R8G8B8A8_UNORM ||
                      image->format == VK_FORMAT_B8G8R8A8_UNORM ||
                      image->format == VK_FORMAT_R32_SFLOAT ||
                      image->format == VK_FORMAT_D32_SFLOAT_S8_UINT);
        if (!probe_format_supported ||
            image->layout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ||
            image->source->info.dimension != xenos::DataDimension::k2DOrStacked ||
            image->source->info.is_stacked) {
          REXLOG_INFO(
              "gta4-native-scene-input-skipped: frame={} cmd={} texture-stage={} "
              "handle={:08X} native-format={} layout={} dimension={} stacked={}",
              submitted_frame, command_index, texture_stage, texture->handle,
              image ? uint32_t(image->format) : 0,
              image ? uint32_t(image->layout) : 0,
              image ? uint32_t(image->source->info.dimension) : 0,
              image ? image->source->info.is_stacked : false);
          continue;
        }
        uint32_t input_probe_stage = 0;
        while (input_probe_stage < content_probe_buffer_.stages.size() &&
               content_probe_buffer_.stages[input_probe_stage].valid) {
          ++input_probe_stage;
        }
        if (!RecordContentProbeImage(
                command_buffer, input_probe_stage, image->resource.image, image->format,
                image->layout, image->width, image->height, texture->handle,
                texture->info.memory.base_address, 10)) {
          continue;
        }
        NativeContentProbeStage& input_stage =
            content_probe_buffer_.stages[input_probe_stage];
        input_stage.command_type = uint8_t(command.type);
        input_stage.command_index = uint32_t(command_index);
        input_stage.texture_stage = texture_stage;
        input_stage.render_phase = uint32_t(command.render_phase);
        input_stage.draw_id = diagnostic_draw_id_;
        input_stage.vertex_shader_hash = vertex_shader ? vertex_shader->hash : 0;
        input_stage.pixel_shader_hash = pixel_shader ? pixel_shader->hash : 0;
      }
    }
    if (diagnostic_frame && is_draw &&
        resolve_predecessor_draw_indices.contains(command_index) &&
        InitializeContentProbeBuffer()) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      if (content_probe_buffer_.pending_frame != submitted_frame) {
        std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
        content_probe_buffer_.pending_frame = submitted_frame;
        content_probe_buffer_.stages = {};
      }
      const NativeShader* vertex_shader = command.pipeline_state->vertex_shader_resource;
      const NativeShader* pixel_shader = command.pipeline_state->pixel_shader_resource;
      for (uint32_t texture_stage = 0; texture_stage < 4; ++texture_stage) {
        const std::shared_ptr<const NativeTextureResource>& texture =
            command.textures[texture_stage];
        NativeTextureImage* image = texture ? GetOrCreateTextureImage(command_buffer, texture)
                                            : nullptr;
        bool sampled = false;
        const bool probe_format_supported =
            image && (image->format == VK_FORMAT_R16G16B16A16_SFLOAT ||
                      image->format == VK_FORMAT_R8G8B8A8_UNORM ||
                      image->format == VK_FORMAT_B8G8R8A8_UNORM ||
                      image->format == VK_FORMAT_R32_SFLOAT ||
                      image->format == VK_FORMAT_D32_SFLOAT_S8_UINT);
        if (probe_format_supported &&
            image->layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
            image->source->info.dimension == xenos::DataDimension::k2DOrStacked &&
            !image->source->info.is_stacked) {
          uint32_t probe_stage = 0;
          while (probe_stage < content_probe_buffer_.stages.size() &&
                 content_probe_buffer_.stages[probe_stage].valid) {
            ++probe_stage;
          }
          sampled = RecordContentProbeImage(
              command_buffer, probe_stage, image->resource.image, image->format,
              image->layout, image->width, image->height, texture->handle,
              texture_stage, 7);
        }
        std::fprintf(
            stderr,
            "[DeferredInputTrace] frame=%u draw=%zu stage=%u sampled=%u "
            "vs=%016llX ps=%016llX handle=%08X generation=%llu hash=%016llX "
            "gpu=%u guest-format=%u native-format=%u size=%ux%u pitch=%u "
            "dimension=%u tiled=%u endian=%u base=%08X layout=%u\n",
            submitted_frame, command_index, texture_stage, unsigned(sampled),
            static_cast<unsigned long long>(vertex_shader ? vertex_shader->hash : 0),
            static_cast<unsigned long long>(pixel_shader ? pixel_shader->hash : 0),
            texture ? texture->handle : 0,
            static_cast<unsigned long long>(texture ? texture->generation : 0),
            static_cast<unsigned long long>(texture ? texture->content_hash : 0),
            unsigned(texture ? texture->gpu_produced : false),
            texture ? uint32_t(texture->info.format) : 0,
            image ? uint32_t(image->format) : 0,
            image ? image->width : 0, image ? image->height : 0,
            texture ? texture->info.pitch : 0,
            texture ? uint32_t(texture->info.dimension) : 0,
            unsigned(texture ? texture->info.is_tiled : false),
            texture ? uint32_t(texture->info.endianness) : 0,
            texture ? texture->info.memory.base_address : 0,
            image ? uint32_t(image->layout) : 0);
      }
      std::fflush(stderr);
    }
    if (is_draw && command.render_phase == RenderPhase::kRadarMap) {
      static std::atomic<uint64_t> radar_draw_count{0};
      const uint64_t radar_draw_index = ++radar_draw_count;
      if (radar_draw_index <= 512 || !(radar_draw_index % 4096)) {
        const NativePipelineState& state = *command.pipeline_state;
        const NativeFixedFunctionState& fixed = command.fixed_function_state;
        std::string draw_summary;
        std::string vertex_payload_summary;
        if (command.type == CommandType::kDrawPrimitiveUp) {
          DrawPrimitiveUpCommand draw{};
          std::memcpy(&draw, command.bytes.data(), sizeof(draw));
          draw_summary = fmt::format(
              "primitive={}/vertices={}/stride={}/guest-bytes={}/captured-bytes={}",
              draw.primitive_type, draw.vertex_count, draw.stride, draw.vertex_data_size,
              command.payload.size());

          if (draw.vertex_count && draw.stride && !command.payload.empty()) {
            std::vector<uint8_t> host_payload(command.payload.size());
            CopyGuestWordsToHost(host_payload.data(), command.payload.data(),
                                 command.payload.size());
            const size_t sampled_vertices = std::min<size_t>(draw.vertex_count, 4);
            const size_t sampled_words = std::min<size_t>(draw.stride / sizeof(uint32_t), 12);
            for (size_t vertex = 0; vertex < sampled_vertices; ++vertex) {
              vertex_payload_summary += fmt::format("{}v{}=[",
                                                    vertex_payload_summary.empty() ? "" : ",",
                                                    vertex);
              for (size_t word_index = 0; word_index < sampled_words; ++word_index) {
                const size_t byte_offset = vertex * size_t(draw.stride) +
                                           word_index * sizeof(uint32_t);
                if (byte_offset + sizeof(uint32_t) > host_payload.size()) {
                  break;
                }
                uint32_t word;
                std::memcpy(&word, host_payload.data() + byte_offset, sizeof(word));
                vertex_payload_summary +=
                    fmt::format("{}{:08X}/{:.6g}", word_index ? "," : "", word,
                                std::bit_cast<float>(word));
              }
              vertex_payload_summary += "]";
            }
          }
        } else if (command.type == CommandType::kDrawPrimitive) {
          DrawPrimitiveCommand draw{};
          std::memcpy(&draw, command.bytes.data(), sizeof(draw));
          draw_summary = fmt::format("primitive={}/start={}/vertices={}", draw.primitive_type,
                                     draw.start_vertex, draw.vertex_count);
        } else {
          DrawIndexedPrimitiveCommand draw{};
          std::memcpy(&draw, command.bytes.data(), sizeof(draw));
          draw_summary = fmt::format(
              "primitive={}/base={}/start-index={}/indices={}", draw.primitive_type,
              draw.base_vertex, draw.start_index, draw.index_count);
        }

        std::string vertex_matrix_summary;
        for (uint32_t row = 0; row < 4; ++row) {
          vertex_matrix_summary +=
              fmt::format("{}c{}=[", vertex_matrix_summary.empty() ? "" : ",", row + 8);
          for (uint32_t component = 0; component < 4; ++component) {
            const size_t word_offset = kVertexConstantsOffset + 128 +
                                       size_t(row * 4 + component) * sizeof(uint32_t);
            const uint32_t word = LoadGuestWord(command.device_snapshot, word_offset);
            vertex_matrix_summary +=
                fmt::format("{}{:.7g}", component ? "," : "", std::bit_cast<float>(word));
          }
          vertex_matrix_summary += "]";
        }

        std::string declaration_summary;
        if (state.vertex_declaration_resource) {
          for (const VertexElement& element : state.vertex_declaration_resource->elements) {
            declaration_summary += fmt::format(
                "{}s{}:o{}:t{:08X}:u{}:{}", declaration_summary.empty() ? "" : ",",
                element.stream, element.offset, element.type, element.usage,
                element.usage_index);
          }
        }
        std::string texture_summary;
        for (uint32_t stage = 0; stage < kShaderTextureCount; ++stage) {
          if (!state.textures[stage] && !command.textures[stage]) {
            continue;
          }
          texture_summary += fmt::format(
              "{}s{}={:08X}/g{}/h{:016X}/f{}/{}x{}/p{}/m{}-{}/t{}/e{}",
              texture_summary.empty() ? "" : ",", stage,
              state.textures[stage], command.textures[stage] ? command.textures[stage]->generation : 0,
              command.textures[stage] ? command.textures[stage]->content_hash : 0,
              command.textures[stage] ? uint32_t(command.textures[stage]->info.format) : 0,
              command.textures[stage] ? command.textures[stage]->info.width + 1 : 0,
              command.textures[stage] ? command.textures[stage]->info.height + 1 : 0,
              command.textures[stage] ? command.textures[stage]->info.pitch : 0,
              command.textures[stage] ? command.textures[stage]->info.mip_min_level : 0,
              command.textures[stage] ? command.textures[stage]->info.mip_max_level : 0,
              command.textures[stage] ? command.textures[stage]->info.is_tiled : false,
              command.textures[stage] ? uint32_t(command.textures[stage]->info.endianness) : 0);
        }
        REXLOG_INFO(
            "gta4-native-radar-draw: index={} object={:08X} type={} "
            "vs={:016X} ps={:016X} constants={:016X}/{:016X} "
            "blend={}/{}/{}/{} alpha-blend={}/{}/{} alpha-test={}/{}/{} "
            "color-mask={:08X} rt0={:08X}/{:08X}/f{}/{}x{} target={}x{} "
            "presenter={} draw=[{}] declaration={:08X}/h{:016X}/[{}] "
            "vertices=[{}] vertex-matrix=[{}] textures=[{}]",
            radar_draw_index, command.render_phase_object, CommandTypeName(command.type),
            state.vertex_shader_resource ? state.vertex_shader_resource->hash : 0,
            state.pixel_shader_resource ? state.pixel_shader_resource->hash : 0,
            command.vertex_constants_hash, command.pixel_constants_hash, fixed.blend_enable,
            fixed.source_blend, fixed.destination_blend, fixed.blend_operation,
            fixed.source_blend_alpha, fixed.destination_blend_alpha,
            fixed.blend_operation_alpha, fixed.alpha_test_enable, fixed.alpha_function,
            fixed.alpha_reference, fixed.color_write_mask, state.render_targets[0].handle,
            state.render_targets[0].address, state.render_targets[0].format,
            state.render_targets[0].width, state.render_targets[0].height, target.width,
            target.height, target.uses_presenter, draw_summary,
            state.vertex_declaration_resource ? state.vertex_declaration_resource->handle : 0,
            state.vertex_declaration_resource ? state.vertex_declaration_resource->content_hash : 0,
            declaration_summary, vertex_payload_summary, vertex_matrix_summary, texture_summary);
      }
    }
    const bool draw_command = command.type == CommandType::kDrawPrimitive ||
                              command.type == CommandType::kDrawPrimitiveUp ||
                              command.type == CommandType::kDrawIndexedPrimitive;
    const NativeShader* diagnostic_pixel_shader =
        command.pipeline_state->pixel_shader_resource;
    const std::string_view translucent_category =
        diagnostic_pixel_shader
            ? ClassifyTranslucentDiagnosticShader(diagnostic_pixel_shader->filename)
            : std::string_view{};
    uint32_t translucent_draw_ordinal = 0;
    if (translucent_queries_active && draw_command && !translucent_category.empty()) {
      translucent_draw_ordinal = ++translucent_category_draw_counts[translucent_category];
    }
    const bool deep_translucent_capture =
        translucent_draw_ordinal &&
        translucent_draw_ordinal <= TranslucentDeepCaptureLimit(translucent_category);
    const bool lifecycle_opaque_consumer =
        diagnostic_frame &&
        command_index == first_opaque_depth_consumer_index;
    const bool lifecycle_translucent_consumer =
        diagnostic_frame &&
        command_index == first_translucent_depth_consumer_index;

    auto record_lifecycle_target_probe =
        [&](uint8_t kind, std::string_view role) {
          NativeSurfaceImage* surface = target.color_surfaces[0];
          if (!surface || surface->samples != VK_SAMPLE_COUNT_1_BIT ||
              !InitializeContentProbeBuffer()) {
            return false;
          }
          if (content_probe_buffer_.pending_frame != submitted_frame) {
            std::memset(content_probe_buffer_.mapping, 0,
                        size_t(kContentProbeBufferSize));
            content_probe_buffer_.pending_frame = submitted_frame;
            content_probe_buffer_.stages = {};
          }
          uint32_t probe_stage = 0;
          while (probe_stage < content_probe_buffer_.stages.size() &&
                 content_probe_buffer_.stages[probe_stage].valid) {
            ++probe_stage;
          }
          if (probe_stage >= content_probe_buffer_.stages.size() ||
              !RecordContentProbeImage(
                  command_buffer, probe_stage, surface->resource.image,
                  surface->format, surface->layout, surface->width,
                  surface->height, surface->descriptor.handle,
                  command.pipeline_state->render_targets[0].address, kind)) {
            return false;
          }
          NativeContentProbeStage& stage =
              content_probe_buffer_.stages[probe_stage];
          stage.command_index = uint32_t(command_index);
          stage.render_phase = uint32_t(command.render_phase);
          stage.draw_id = diagnostic_draw_id_;
          stage.vertex_shader_hash =
              command.pipeline_state->vertex_shader_resource
                  ? command.pipeline_state->vertex_shader_resource->hash
                  : 0;
          stage.pixel_shader_hash =
              diagnostic_pixel_shader ? diagnostic_pixel_shader->hash : 0;
          stage.sample_count = uint32_t(surface->samples);
          stage.diagnostic_category =
              translucent_category.empty() ? "opaque-unclassified"
                                           : translucent_category;
          stage.diagnostic_role = role;
          return true;
        };

    if ((lifecycle_opaque_consumer || lifecycle_translucent_consumer) &&
        target.depth_surface) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      const NativePlacementOwner* owner =
          FindPlacementOwner(command.pipeline_state->depth_stencil, true);
      const std::string_view role = lifecycle_opaque_consumer
                                        ? "first-opaque-consumer-before"
                                        : "first-translucent-consumer-before";
      RecordDepthStencilDiagnosticProbe(
          command_buffer, *target.depth_surface,
          lifecycle_opaque_consumer ? 42 : 46,
          lifecycle_opaque_consumer ? 43 : 47, role,
          command.pipeline_state->depth_stencil_trace_wrapper
              ? command.pipeline_state->depth_stencil_trace_wrapper
              : forward_depth_wrapper,
          0, owner);
      record_lifecycle_target_probe(lifecycle_opaque_consumer ? 44 : 48,
                                    lifecycle_opaque_consumer
                                        ? "first-opaque-target-before"
                                        : "first-translucent-target-before");
      REXLOG_WARN(
          "gta4-native-cause: point=depth-lifecycle-consumer role={} frame={} "
          "cmd={} draw={:016X} category={} phase={} wrapper={:08X} "
          "depth={:08X}/{:08X} depth-samples={} target={:08X}/{:08X} "
          "target-samples={} vs={:016X} ps={:016X} "
          "provenance={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
          role, submitted_frame, command_index, diagnostic_draw_id_,
          translucent_category.empty() ? "opaque-unclassified"
                                       : translucent_category,
          uint32_t(command.render_phase),
          command.pipeline_state->depth_stencil_trace_wrapper,
          target.depth_surface->descriptor.handle,
          command.pipeline_state->depth_stencil.address,
          uint32_t(target.depth_surface->samples),
          target.color_surfaces[0]
              ? target.color_surfaces[0]->descriptor.handle
              : 0,
          command.pipeline_state->render_targets[0].address,
          uint32_t(target.samples),
          command.pipeline_state->vertex_shader_resource
              ? command.pipeline_state->vertex_shader_resource->hash
              : 0,
          diagnostic_pixel_shader ? diagnostic_pixel_shader->hash : 0,
          owner && owner->image ? owner->image->descriptor.handle : 0,
          owner ? owner->frame : 0,
          owner ? owner->command_index : SIZE_MAX,
          owner ? owner->serial : 0,
          owner ? uint32_t(owner->render_phase) : 0,
          owner ? uint32_t(owner->write_kind) : 0);
    }

    const bool forward_depth_consumer =
        diagnostic_frame && draw_command && !forward_phase_entry_probed &&
        forward_depth_handle && target.depth_surface &&
        command.pipeline_state->depth_stencil.handle == forward_depth_handle;
    if (forward_depth_consumer) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      const NativePlacementOwner* owner =
          FindPlacementOwner(command.pipeline_state->depth_stencil, true);
      RecordDepthStencilDiagnosticProbe(
          command_buffer, *target.depth_surface, 40, 41,
          "forward-phase-entry",
          command.pipeline_state->depth_stencil_trace_wrapper
              ? command.pipeline_state->depth_stencil_trace_wrapper
              : forward_depth_wrapper,
          0, owner);
      REXLOG_WARN(
          "gta4-native-cause: point=depth-lifecycle-consumer role=forward-phase-entry "
          "frame={} cmd={} draw={:016X} category={} phase={} wrapper={:08X} "
          "surface={:08X}/{:08X} samples={} host={}x{} "
          "vs={:016X} ps={:016X} provenance={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
          submitted_frame, command_index, diagnostic_draw_id_,
          translucent_category.empty() ? "unclassified" : translucent_category,
          uint32_t(command.render_phase),
          command.pipeline_state->depth_stencil_trace_wrapper
              ? command.pipeline_state->depth_stencil_trace_wrapper
              : forward_depth_wrapper,
          target.depth_surface->descriptor.handle,
          command.pipeline_state->depth_stencil.address,
          uint32_t(target.depth_surface->samples), target.depth_surface->width,
          target.depth_surface->height,
          command.pipeline_state->vertex_shader_resource
              ? command.pipeline_state->vertex_shader_resource->hash
              : 0,
          diagnostic_pixel_shader ? diagnostic_pixel_shader->hash : 0,
          owner && owner->image ? owner->image->descriptor.handle : 0,
          owner ? owner->frame : 0,
          owner ? owner->command_index : SIZE_MAX,
          owner ? owner->serial : 0,
          owner ? uint32_t(owner->render_phase) : 0,
          owner ? uint32_t(owner->write_kind) : 0);
      forward_phase_entry_probed = true;
    }

    auto record_translucent_surface_probe =
        [&](NativeSurfaceImage* surface, uint8_t kind, uint32_t texture_stage,
            uint32_t address) {
          if (!deep_translucent_capture || !surface ||
              surface->samples != VK_SAMPLE_COUNT_1_BIT ||
              !InitializeContentProbeBuffer()) {
            return false;
          }
          if (content_probe_buffer_.pending_frame != submitted_frame) {
            std::memset(content_probe_buffer_.mapping, 0,
                        size_t(kContentProbeBufferSize));
            content_probe_buffer_.pending_frame = submitted_frame;
            content_probe_buffer_.stages = {};
          }
          uint32_t probe_stage = 0;
          while (probe_stage < content_probe_buffer_.stages.size() &&
                 content_probe_buffer_.stages[probe_stage].valid) {
            ++probe_stage;
          }
          if (probe_stage >= content_probe_buffer_.stages.size() ||
              !RecordContentProbeImage(
                  command_buffer, probe_stage, surface->resource.image, surface->format,
                  surface->layout, surface->width, surface->height,
                  surface->descriptor.handle, address, kind)) {
            REXLOG_WARN(
                "gta4-native-cause: point=translucent-content-probe-rejected frame={} "
                "cmd={} draw={:016X} category={} kind={} handle={:08X} layout={} "
                "samples={} stage={}",
                submitted_frame, command_index, diagnostic_draw_id_,
                translucent_category, kind, surface->descriptor.handle,
                uint32_t(surface->layout), uint32_t(surface->samples), probe_stage);
            return false;
          }
          NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
          stage.command_index = uint32_t(command_index);
          stage.texture_stage = texture_stage;
          stage.render_phase = uint32_t(command.render_phase);
          stage.draw_id = diagnostic_draw_id_;
          stage.vertex_shader_hash =
              command.pipeline_state->vertex_shader_resource
                  ? command.pipeline_state->vertex_shader_resource->hash
                  : 0;
          stage.pixel_shader_hash = diagnostic_pixel_shader ? diagnostic_pixel_shader->hash : 0;
          stage.diagnostic_category = translucent_category;
          return true;
        };

    auto record_translucent_texture_probe =
        [&](uint32_t texture_stage) {
          if (!deep_translucent_capture || texture_stage >= command.textures.size() ||
              !command.textures[texture_stage] || !InitializeContentProbeBuffer()) {
            return false;
          }
          NativeTextureImage* image =
              GetOrCreateTextureImage(command_buffer, command.textures[texture_stage]);
          if (!image || image->layout == VK_IMAGE_LAYOUT_UNDEFINED ||
              image->source->info.dimension != xenos::DataDimension::k2DOrStacked ||
              image->source->info.is_stacked) {
            return false;
          }
          const bool supported_format =
              image->format == VK_FORMAT_R16G16B16A16_SFLOAT ||
              image->format == VK_FORMAT_R16G16_SFLOAT ||
              image->format == VK_FORMAT_R8G8B8A8_UNORM ||
              image->format == VK_FORMAT_B8G8R8A8_UNORM ||
              image->format == VK_FORMAT_R32_SFLOAT ||
              image->format == VK_FORMAT_D32_SFLOAT_S8_UINT;
          if (!supported_format) {
            REXLOG_WARN(
                "gta4-native-cause: point=translucent-input-probe-skipped frame={} "
                "cmd={} draw={:016X} category={} texture-stage={} handle={:08X} "
                "reason=unsupported-readback-format format={}",
                submitted_frame, command_index, diagnostic_draw_id_,
                translucent_category, texture_stage,
                command.textures[texture_stage]->handle, uint32_t(image->format));
            return false;
          }
          if (content_probe_buffer_.pending_frame != submitted_frame) {
            std::memset(content_probe_buffer_.mapping, 0,
                        size_t(kContentProbeBufferSize));
            content_probe_buffer_.pending_frame = submitted_frame;
            content_probe_buffer_.stages = {};
          }
          uint32_t probe_stage = 0;
          while (probe_stage < content_probe_buffer_.stages.size() &&
                 content_probe_buffer_.stages[probe_stage].valid) {
            ++probe_stage;
          }
          if (probe_stage >= content_probe_buffer_.stages.size() ||
              !RecordContentProbeImage(
                  command_buffer, probe_stage, image->resource.image, image->format,
                  image->layout, image->width, image->height,
                  command.textures[texture_stage]->handle,
                  command.textures[texture_stage]->info.memory.base_address, 18)) {
            return false;
          }
          NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
          stage.command_index = uint32_t(command_index);
          stage.texture_stage = texture_stage;
          stage.render_phase = uint32_t(command.render_phase);
          stage.draw_id = diagnostic_draw_id_;
          stage.vertex_shader_hash =
              command.pipeline_state->vertex_shader_resource
                  ? command.pipeline_state->vertex_shader_resource->hash
                  : 0;
          stage.pixel_shader_hash = diagnostic_pixel_shader ? diagnostic_pixel_shader->hash : 0;
          stage.diagnostic_category = translucent_category;
          return true;
        };

    if (deep_translucent_capture) {
      std::string geometry;
      if (command.type == CommandType::kDrawPrimitive) {
        DrawPrimitiveCommand draw{};
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        geometry = fmt::format("primitive={} start={} vertices={}", draw.primitive_type,
                               draw.start_vertex, draw.vertex_count);
      } else if (command.type == CommandType::kDrawPrimitiveUp) {
        DrawPrimitiveUpCommand draw{};
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        geometry = fmt::format("primitive={} vertices={} stride={} payload={}",
                               draw.primitive_type, draw.vertex_count, draw.stride,
                               draw.vertex_data_size);
      } else if (command.type == CommandType::kDrawIndexedPrimitive) {
        DrawIndexedPrimitiveCommand draw{};
        std::memcpy(&draw, command.bytes.data(), sizeof(draw));
        geometry = fmt::format(
            "primitive={} base={} start-index={} indices={}", draw.primitive_type,
            draw.base_vertex, draw.start_index, draw.index_count);
      }
      std::string vertex_streams;
      for (uint32_t stream = 0; stream < command.vertex_buffers.size(); ++stream) {
        if (!command.vertex_buffers[stream]) {
          continue;
        }
        const NativePipelineState::VertexStream& stream_state =
            command.pipeline_state->vertex_streams[stream];
        vertex_streams += fmt::format(
            "{}s{}={:08X}/h{:016X}/off{}/stride{}", vertex_streams.empty() ? "" : ",",
            stream, command.vertex_buffers[stream]->handle,
            command.vertex_buffers[stream]->content_hash, stream_state.offset,
            stream_state.stride);
      }
      const NativeFixedFunctionState& diagnostic_fixed = command.fixed_function_state;
      REXLOG_WARN(
          "gta4-native-cause: point=translucent-geometry-state frame={} cmd={} "
          "draw={:016X} ordinal={} category={} type={} geometry=[{}] target={}x{} "
          "logical={}x{} viewport={:.9g},{:.9g},{:.9g},{:.9g},{:.9g},{:.9g} "
          "scissor={}:{},{},{},{} cull={} vdecl={:08X}/h{:016X} streams=[{}]",
          submitted_frame, command_index, diagnostic_draw_id_, translucent_draw_ordinal,
          translucent_category, CommandTypeName(command.type), geometry, target.width,
          target.height, target.logical_width, target.logical_height,
          std::bit_cast<float>(diagnostic_fixed.viewport_bits[0]),
          std::bit_cast<float>(diagnostic_fixed.viewport_bits[1]),
          std::bit_cast<float>(diagnostic_fixed.viewport_bits[2]),
          std::bit_cast<float>(diagnostic_fixed.viewport_bits[3]),
          std::bit_cast<float>(diagnostic_fixed.viewport_bits[4]),
          std::bit_cast<float>(diagnostic_fixed.viewport_bits[5]),
          diagnostic_fixed.scissor_enable, diagnostic_fixed.scissor[0],
          diagnostic_fixed.scissor[1], diagnostic_fixed.scissor[2],
          diagnostic_fixed.scissor[3], diagnostic_fixed.cull_mode,
          command.pipeline_state->vertex_declaration_resource
              ? command.pipeline_state->vertex_declaration_resource->handle
              : 0,
          command.pipeline_state->vertex_declaration_resource
              ? command.pipeline_state->vertex_declaration_resource->content_hash
              : 0,
          vertex_streams);
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      record_translucent_surface_probe(
          target.color_surfaces[0], 15, UINT32_MAX,
          command.pipeline_state->render_targets[0].address);

      const bool first_depth_probe =
          translucent_depth_probe_categories.emplace(translucent_category).second;
      if (first_depth_probe && target.depth_surface) {
        record_translucent_surface_probe(
            target.depth_surface, 17, UINT32_MAX,
            command.pipeline_state->depth_stencil.address);
        GuestSurfaceView destination_view{};
        const bool decoded = DecodeGuestSurfaceView(
            command.pipeline_state->depth_stencil, true, destination_view);
        const NativePlacementOwner* exact_owner =
            FindPlacementOwner(command.pipeline_state->depth_stencil, true);
        REXLOG_WARN(
            "gta4-native-cause: point=translucent-depth-lineage frame={} cmd={} "
            "draw={:016X} category={} destination={:08X}/{:08X} wrapper={:08X} decoded={} "
            "placement={} pitch={} samplespace={}x{} guest-samples={} host={}x{} "
            "logical={}x{} layout={} ever-written={} materialized-serial={} "
            "exact-owner={:08X}:frame{}:cmd{}:serial{}:phase{}:kind{}",
            submitted_frame, command_index, diagnostic_draw_id_, translucent_category,
            target.depth_surface->descriptor.handle,
            command.pipeline_state->depth_stencil.address,
            command.pipeline_state->depth_stencil_trace_wrapper, decoded,
            destination_view.placement_base_tiles, destination_view.sample_pitch,
            destination_view.sample_width, destination_view.sample_height,
            uint32_t(destination_view.msaa_samples), target.depth_surface->width,
            target.depth_surface->height, target.depth_surface->logical_width,
            target.depth_surface->logical_height, uint32_t(target.depth_surface->layout),
            target.depth_surface->ever_written, target.depth_surface->materialized_serial,
            exact_owner && exact_owner->image ? exact_owner->image->descriptor.handle : 0,
            exact_owner ? exact_owner->frame : 0,
            exact_owner ? exact_owner->command_index : SIZE_MAX,
            exact_owner ? exact_owner->serial : 0,
            exact_owner ? uint32_t(exact_owner->render_phase) : 0,
            exact_owner ? uint32_t(exact_owner->write_kind) : 0);
        if (decoded) {
          for (const auto& [placement_key, owner] : native_placement_owners_) {
            if (!placement_key.depth ||
                placement_key.placement_base_tiles !=
                    destination_view.placement_base_tiles ||
                !owner.image) {
              continue;
            }
            REXLOG_WARN(
                "gta4-native-cause: point=translucent-depth-lineage-candidate frame={} "
                "cmd={} draw={:016X} category={} source={:08X}/{:08X} "
                "owner-frame={} owner-cmd={} serial={} owner-phase={} placement={} "
                "pitch={} samplespace={}x{} guest-samples={} host={}x{} logical={}x{} "
                "layout={} ever-written={} serial-current={}",
                submitted_frame, command_index, diagnostic_draw_id_,
                translucent_category, owner.image->descriptor.handle,
                owner.image->descriptor.address, owner.frame, owner.command_index,
                owner.serial, uint32_t(owner.render_phase),
                placement_key.placement_base_tiles, placement_key.sample_pitch,
                placement_key.sample_width, placement_key.sample_height,
                uint32_t(owner.view.msaa_samples), owner.image->width,
                owner.image->height, owner.image->logical_width,
                owner.image->logical_height, uint32_t(owner.image->layout),
                owner.image->ever_written,
                owner.image->materialized_serial == owner.serial);
          }
        }

        std::string texture_bindings;
        for (uint32_t texture_stage = 0; texture_stage < kShaderTextureCount;
             ++texture_stage) {
          const auto& texture = command.textures[texture_stage];
          if (!texture) {
            continue;
          }
          texture_bindings += fmt::format(
              "{}s{}={:08X}/g{}/h{:016X}/gpu{}/f{}/{}x{}/p{}",
              texture_bindings.empty() ? "" : ",", texture_stage, texture->handle,
              texture->generation, texture->content_hash, texture->gpu_produced,
              uint32_t(texture->info.format), texture->info.width + 1,
              texture->info.height + 1, texture->info.pitch);
        }
        REXLOG_WARN(
            "gta4-native-cause: point=translucent-resource-bindings frame={} cmd={} "
            "draw={:016X} category={} vs={:016X} ps={:016X} constants={:016X}/{:016X} "
            "textures=[{}]",
            submitted_frame, command_index, diagnostic_draw_id_, translucent_category,
            command.pipeline_state->vertex_shader_resource
                ? command.pipeline_state->vertex_shader_resource->hash
                : 0,
            diagnostic_pixel_shader ? diagnostic_pixel_shader->hash : 0,
            command.vertex_constants_hash, command.pixel_constants_hash,
            texture_bindings);

        if (translucent_category == "water-surface") {
          record_translucent_texture_probe(0);
          record_translucent_texture_probe(2);
          record_translucent_texture_probe(14);
        } else if (translucent_category == "light-sprite" ||
                   translucent_category == "vehicle-light" ||
                   translucent_category == "emissive") {
          record_translucent_texture_probe(0);
          record_translucent_texture_probe(16);
        } else {
          record_translucent_texture_probe(0);
          record_translucent_texture_probe(1);
          record_translucent_texture_probe(2);
        }
      }
    }

    if (!rendering || !targets_equal(active_target, target)) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
      }
      bool content_ready = true;
      for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
        if (!target.color_surfaces[index]) {
          continue;
        }
        content_ready &= PrepareSurfaceContent(
            command_buffer, *target.color_surfaces[index],
            command.pipeline_state->render_targets[index], false, submitted_frame,
            command.render_phase);
      }
      if (target.depth_surface) {
        if (diagnostic_frame) {
          REXLOG_WARN(
              "gta4-native-cause: point=depth-bind-transition frame={} cmd={} "
              "wrapper={:08X} caller={:08X} surface={:08X}/{:08X} phase={} target={}x{} "
              "depth-test={}:{}:{} stencil={}:{}:ref{}:mask{:08X}",
              submitted_frame, command_index,
              command.pipeline_state->depth_stencil_trace_wrapper,
              command.pipeline_state->depth_stencil_trace_caller,
              command.pipeline_state->depth_stencil.handle,
              command.pipeline_state->depth_stencil.address,
              RenderPhaseName(command.render_phase), target.width, target.height,
              command.fixed_function_state.depth_enable,
              command.fixed_function_state.depth_function,
              command.fixed_function_state.depth_write_enable,
              command.fixed_function_state.stencil_enable,
              command.fixed_function_state.stencil_function,
              command.fixed_function_state.stencil_reference,
              command.fixed_function_state.stencil_mask);
        }
        content_ready &= PrepareSurfaceContent(
            command_buffer, *target.depth_surface, command.pipeline_state->depth_stencil, true,
            submitted_frame, command.render_phase);
      }
      if (!content_ready) {
        rendering = false;
        TraceNativeRendererEvent("command-result",
                                 "recorded=false reason=placement-materialization");
        continue;
      }
      if (pending_scene_checkpoint != scene_write_checkpoint_ordinals.end() &&
          pending_scene_checkpoint->second == 1 && target.color_surfaces[0] &&
          target.color_surfaces[0]->format == VK_FORMAT_R16G16B16A16_SFLOAT &&
          InitializeContentProbeBuffer()) {
        if (content_probe_buffer_.pending_frame != submitted_frame) {
          std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
          content_probe_buffer_.pending_frame = submitted_frame;
          content_probe_buffer_.stages = {};
        }
        uint32_t probe_stage = 0;
        while (probe_stage < content_probe_buffer_.stages.size() &&
               content_probe_buffer_.stages[probe_stage].valid) {
          ++probe_stage;
        }
        NativeSurfaceImage* draw_target = target.color_surfaces[0];
        const bool checkpoint_recorded = RecordContentProbeImage(
            command_buffer, probe_stage, draw_target->resource.image, draw_target->format,
            draw_target->layout, draw_target->width, draw_target->height,
            draw_target->descriptor.handle,
            command.pipeline_state->render_targets[0].address, 11);
        if (checkpoint_recorded) {
          NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
          const NativeShader* vertex_shader = command.pipeline_state->vertex_shader_resource;
          const NativeShader* pixel_shader = command.pipeline_state->pixel_shader_resource;
          stage.command_type = uint8_t(command.type);
          stage.command_index = uint32_t(command_index);
          stage.checkpoint_ordinal = 0;
          stage.checkpoint_count = scene_write_checkpoint_count;
          stage.render_phase = uint32_t(command.render_phase);
          stage.draw_id = diagnostic_draw_id_;
          stage.vertex_shader_hash = vertex_shader ? vertex_shader->hash : 0;
          stage.pixel_shader_hash = pixel_shader ? pixel_shader->hash : 0;
          REXLOG_INFO(
              "gta4-native-cause: point=scene-after-materialization-recorded frame={} "
              "cmd={} target={:08X}/{:08X} draw={:016X} vs={:016X} ps={:016X}",
              submitted_frame, stage.command_index, stage.handle, stage.address,
              stage.draw_id, stage.vertex_shader_hash, stage.pixel_shader_hash);
        }
      }
      if (!TransitionRenderingTarget(command_buffer, target)) {
        rendering = false;
        continue;
      }

      std::array<VkRenderingAttachmentInfo, kRenderTargetCount> color_attachments{};
      uint32_t color_attachment_count = 0;
      for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
        if (target.color_formats[index] != VK_FORMAT_UNDEFINED) {
          color_attachment_count = index + 1;
        }
        VkRenderingAttachmentInfo& attachment = color_attachments[index];
        attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        attachment.imageView = target.color_views[index];
        attachment.imageLayout = attachment.imageView ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                                      : VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.storeOp =
            attachment.imageView ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
        NativeSurfaceImage* surface = target.color_surfaces[index];
        const bool initialized =
            surface ? HasCurrentPlacementContent(
                          *surface, command.pipeline_state->render_targets[index], false)
                    : presenter_written;
        if (command.render_phase == RenderPhase::kRadarMap && attachment.imageView &&
            !initialized) {
          static std::atomic<uint64_t> radar_implicit_clear_count{0};
          const uint64_t clear_index = ++radar_implicit_clear_count;
          if (clear_index <= 64 || !(clear_index % 4096)) {
            REXLOG_WARN(
                "gta4-native-radar-clear: index={} object={:08X} attachment={} "
                "surface={:08X} target={}x{} implicit-rgba=0,0,0,1",
                clear_index, command.render_phase_object, index,
                surface ? surface->descriptor.handle : 0, target.width, target.height);
          }
        }
        attachment.loadOp = !attachment.imageView ? VK_ATTACHMENT_LOAD_OP_DONT_CARE
                            : initialized         ? VK_ATTACHMENT_LOAD_OP_LOAD
                                                  : VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.clearValue.color.float32[3] = 1.0f;
        if (attachment.imageView) {
          TraceNativeRendererEvent(
              "attachment",
              fmt::format(
                  "slot={} handle={:08X} address={:08X} vk-format={} size={}x{} logical={}x{} "
                  "samples={} prior-written={} load={} store={} layout={}",
                  index, surface ? surface->descriptor.handle : 0,
                  surface ? surface->descriptor.address : 0, uint32_t(target.color_formats[index]),
                  target.width, target.height, target.logical_width, target.logical_height,
                  uint32_t(target.samples), initialized, uint32_t(attachment.loadOp),
                  uint32_t(attachment.storeOp), uint32_t(attachment.imageLayout)));
        }
        if (surface) {
          surface->ever_written = true;
        }
      }

      VkRenderingAttachmentInfo depth_attachment{};
      if (target.depth_surface) {
        depth_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        depth_attachment.imageView = target.depth_view;
        depth_attachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        const bool depth_initialized = HasCurrentPlacementContent(
            *target.depth_surface, command.pipeline_state->depth_stencil, true);
        depth_attachment.loadOp = depth_initialized ? VK_ATTACHMENT_LOAD_OP_LOAD
                                                    : VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depth_attachment.clearValue.depthStencil.depth = 1.0f;
        TraceNativeRendererEvent(
            "depth-attachment",
            fmt::format(
                "handle={:08X} address={:08X} vk-format={} size={}x{} samples={} "
                "prior-written={} load={} store={} layout={}",
                target.depth_surface->descriptor.handle,
                target.depth_surface->descriptor.address, uint32_t(target.depth_format),
                target.width, target.height, uint32_t(target.samples),
                depth_initialized, uint32_t(depth_attachment.loadOp),
                uint32_t(depth_attachment.storeOp), uint32_t(depth_attachment.imageLayout)));
        target.depth_surface->ever_written = true;
      }

      VkRenderingInfo rendering_info{};
      rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
      rendering_info.renderArea.extent.width = target.width;
      rendering_info.renderArea.extent.height = target.height;
      rendering_info.layerCount = 1;
      rendering_info.colorAttachmentCount = color_attachment_count;
      rendering_info.pColorAttachments =
          color_attachment_count ? color_attachments.data() : nullptr;
      rendering_info.pDepthAttachment = target.depth_surface ? &depth_attachment : nullptr;
      rendering_info.pStencilAttachment = target.depth_surface ? &depth_attachment : nullptr;
      TraceNativeRendererEvent(
          "rendering-begin",
          fmt::format("extent={}x{} logical={}x{} colors={} depth={} presenter={} reflection={}",
                      target.width, target.height, target.logical_width,
                      target.logical_height, color_attachment_count,
                      target.depth_surface != nullptr, target.uses_presenter,
                      target.is_reflection));
      dfn.vkCmdBeginRendering(command_buffer, &rendering_info);
      rendering = true;
      if (target.uses_presenter) {
        presenter_written = true;
      }
      active_target = target;
    }

    uint32_t translucent_query_index = UINT32_MAX;
    auto begin_translucent_query = [&](const NativeCommand& query_command,
                                       std::string_view variant) {
      if (!translucent_queries_active || !draw_command || translucent_category.empty() ||
          translucent_query_state_.pending_count >= kTranslucentQueryCapacity) {
        return UINT32_MAX;
      }
      const uint32_t query_index = translucent_query_state_.pending_count++;
      NativeTranslucentQuery& query =
          translucent_query_state_.queries[query_index];
      query.valid = true;
      query.frame = submitted_frame;
      query.command_index = uint32_t(command_index);
      query.render_phase = uint32_t(query_command.render_phase);
      query.draw_id = diagnostic_draw_id_;
      query.vertex_shader_hash = query_command.pipeline_state->vertex_shader_resource
                                     ? query_command.pipeline_state->vertex_shader_resource->hash
                                     : 0;
      query.pixel_shader_hash = diagnostic_pixel_shader->hash;
      query.pixel_shader_specialization_constants_mask =
          diagnostic_pixel_shader->specialization_constants_mask;
      const bool alpha_coverage_requested = IsAlphaCoverageRequested(
          query_command.fixed_function_state.alpha_test_enable,
          query_command.fixed_function_state.alpha_function);
      query.pixel_shader_uses_late_module =
          alpha_coverage_requested &&
          HasAlphaTestCapability(diagnostic_pixel_shader->specialization_constants_mask) &&
          diagnostic_pixel_shader->late_module;
      query.pixel_shader_specialization_value =
          query.pixel_shader_uses_late_module
              ? PackAlphaTestSpecialization(query_command.fixed_function_state.alpha_function)
              : 0;
      query.target_handle = query_command.pipeline_state->render_targets[0].handle;
      query.target_address = query_command.pipeline_state->render_targets[0].address;
      query.depth_handle = query_command.pipeline_state->depth_stencil.handle;
      query.depth_address = query_command.pipeline_state->depth_stencil.address;
      query.fixed_function = query_command.fixed_function_state;
      query.category = translucent_category;
      query.variant = variant;
      query.shader_filename = diagnostic_pixel_shader->filename;
      dfn.vkCmdBeginQuery(command_buffer, translucent_query_state_.pool,
                          query_index, 0);
      return query_index;
    };
    auto end_translucent_query = [&](uint32_t query_index, bool query_draw_recorded) {
      if (query_index == UINT32_MAX) {
        return;
      }
      dfn.vkCmdEndQuery(command_buffer, translucent_query_state_.pool, query_index);
      NativeTranslucentQuery& query = translucent_query_state_.queries[query_index];
      query.draw_recorded = query_draw_recorded;
      REXLOG_INFO(
          "gta4-native-cause: point=translucent-query-recorded frame={} cmd={} "
          "query={} draw={:016X} category={} variant={} shader={} recorded={} "
          "alpha={}:{} capability={:08X} packed={:08X} module={}",
          submitted_frame, command_index, query_index, diagnostic_draw_id_,
          query.category, query.variant, query.shader_filename, query_draw_recorded,
          query.fixed_function.alpha_test_enable, query.fixed_function.alpha_function,
          query.pixel_shader_specialization_constants_mask,
          query.pixel_shader_specialization_value,
          query.pixel_shader_uses_late_module ? "late" : "early");
    };
    translucent_query_index = begin_translucent_query(command, "original");

    bool command_recorded = false;
    if (command.type == CommandType::kDrawPrimitive) {
      command_recorded = RecordPrimitive(command_buffer, command, target.width, target.height,
                                         target, resources);
      command_recorded ? ++successful_draws : ++failed_draws;
    } else if (command.type == CommandType::kDrawPrimitiveUp) {
      command_recorded =
          RecordPrimitiveUp(command_buffer, command, target.width, target.height, target);
      command_recorded ? ++successful_draws : ++failed_draws;
    } else if (command.type == CommandType::kDrawIndexedPrimitive) {
      command_recorded = RecordIndexedPrimitive(command_buffer, command, target.width,
                                                target.height, target, resources);
      command_recorded ? ++successful_draws : ++failed_draws;
      if (command_recorded) {
        transition::NoteFirstIndexedDraw(diagnostic_guest_pc, diagnostic_guest_lr,
                                         submitted_frame, diagnostic_draw_id_);
      }
    } else if (command.type == CommandType::kClear) {
      ClearCommand clear{};
      std::memcpy(&clear, command.bytes.data(), sizeof(clear));
      TraceNativeRendererEvent(
          "clear-begin",
          fmt::format(
              "flags={:08X} rect={},{},{},{} color={:08X},{:08X},{:08X},{:08X} "
              "depth={:016X} stencil={} target={:08X}:{:08X}",
              clear.flags, clear.left, clear.top, clear.right, clear.bottom,
              clear.color_bits[0], clear.color_bits[1], clear.color_bits[2],
              clear.color_bits[3], clear.depth_bits, clear.stencil,
              command.pipeline_state->render_targets[0].handle,
              command.pipeline_state->render_targets[0].address));
      if (diagnostic_frame) {
        std::fprintf(stderr,
                     "[ClearExecute] frame=%u index=%zu phase=before target=%08X image=%p "
                     "written=%u rendering=%u\n",
                     submitted_frame, command_index,
                     command.pipeline_state->render_targets[0].handle,
                     static_cast<void*>(target.color_surfaces[0]),
                     unsigned(target.color_surfaces[0] ? target.color_surfaces[0]->ever_written
                                                       : false),
                     unsigned(rendering));
        std::fflush(stderr);
      }
      command_recorded = RecordClear(command_buffer, command, target);
      command_recorded ? ++successful_clears : ++failed_clears;
      if (diagnostic_frame) {
        std::fprintf(
            stderr,
            "[ClearExecute] frame=%u index=%zu phase=after target=%08X image=%p "
            "written=%u recorded=%u\n",
            submitted_frame, command_index, command.pipeline_state->render_targets[0].handle,
            static_cast<void*>(target.color_surfaces[0]),
            unsigned(target.color_surfaces[0] ? target.color_surfaces[0]->ever_written : false),
            unsigned(command_recorded));
        std::fflush(stderr);
      }
    }
    end_translucent_query(translucent_query_index, command_recorded);
    if (command_recorded && translucent_queries_active && draw_command &&
        !translucent_category.empty() && deep_translucent_capture) {
      auto record_diagnostic_draw = [&](const NativeCommand& diagnostic_command) {
        if (diagnostic_command.type == CommandType::kDrawPrimitive) {
          return RecordPrimitive(command_buffer, diagnostic_command, target.width,
                                 target.height, target, resources);
        }
        if (diagnostic_command.type == CommandType::kDrawPrimitiveUp) {
          return RecordPrimitiveUp(command_buffer, diagnostic_command, target.width,
                                   target.height, target);
        }
        return RecordIndexedPrimitive(command_buffer, diagnostic_command, target.width,
                                      target.height, target, resources);
      };
      NativeCommand diagnostic_command = command;
      NativeFixedFunctionState& diagnostic_fixed =
          diagnostic_command.fixed_function_state;
      diagnostic_fixed.color_write_mask = 0;
      diagnostic_fixed.blend_enable = 0;
      diagnostic_fixed.alpha_test_enable = 0;
      diagnostic_fixed.depth_write_enable = 0;
      diagnostic_fixed.stencil_fail = 0;
      diagnostic_fixed.stencil_depth_fail = 0;
      diagnostic_fixed.stencil_pass = 0;
      diagnostic_fixed.ccw_stencil_fail = 0;
      diagnostic_fixed.ccw_stencil_depth_fail = 0;
      diagnostic_fixed.ccw_stencil_pass = 0;
      diagnostic_fixed.stencil_write_mask = 0;
      diagnostic_fixed.back_stencil_write_mask = 0;

      diagnostic_fixed.stencil_enable = 0;
      uint32_t split_query = begin_translucent_query(diagnostic_command, "depth-only");
      bool split_recorded = record_diagnostic_draw(diagnostic_command);
      end_translucent_query(split_query, split_recorded);

      diagnostic_fixed.depth_enable = 0;
      diagnostic_fixed.stencil_enable = command.fixed_function_state.stencil_enable;
      split_query = begin_translucent_query(diagnostic_command, "stencil-only");
      split_recorded = record_diagnostic_draw(diagnostic_command);
      end_translucent_query(split_query, split_recorded);

      diagnostic_fixed.stencil_enable = 0;
      split_query = begin_translucent_query(diagnostic_command, "raster-only");
      split_recorded = record_diagnostic_draw(diagnostic_command);
      end_translucent_query(split_query, split_recorded);
    }
    if (command_recorded &&
        (lifecycle_opaque_consumer || lifecycle_translucent_consumer) &&
        target.color_surfaces[0]) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      record_lifecycle_target_probe(
          lifecycle_opaque_consumer ? 45 : 49,
          lifecycle_opaque_consumer ? "first-opaque-target-after"
                                    : "first-translucent-target-after");
    }
    if (command_recorded && deep_translucent_capture && target.color_surfaces[0]) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      record_translucent_surface_probe(
          target.color_surfaces[0], 16, UINT32_MAX,
          command.pipeline_state->render_targets[0].address);
    }
    if (command_recorded) {
      recorded_draw = true;
      if (draw_command && command.fixed_function_state.color_write_mask) {
        for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
          if (target.color_surfaces[index]) {
            ClaimSurfaceContent(*target.color_surfaces[index],
                                command.pipeline_state->render_targets[index], false,
                                submitted_frame, command.render_phase,
                                NativePlacementOwner::WriteKind::kDrawColor);
          }
        }
      }
      if (draw_command && command.fixed_function_state.depth_enable &&
          command.fixed_function_state.depth_write_enable && target.depth_surface) {
        ClaimSurfaceContent(*target.depth_surface, command.pipeline_state->depth_stencil, true,
                            submitted_frame, command.render_phase,
                            NativePlacementOwner::WriteKind::kDrawDepth);
      }
      if (command.type == CommandType::kClear) {
        ClearCommand clear{};
        std::memcpy(&clear, command.bytes.data(), sizeof(clear));
        for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
          if ((clear.flags & (uint32_t(1) << index)) && target.color_surfaces[index]) {
            ClaimSurfaceContent(*target.color_surfaces[index],
                                command.pipeline_state->render_targets[index], false,
                                submitted_frame, command.render_phase,
                                NativePlacementOwner::WriteKind::kExplicitClear);
          }
        }
        if ((clear.flags & 0x30u) && target.depth_surface) {
          ClaimSurfaceContent(*target.depth_surface, command.pipeline_state->depth_stencil, true,
                              submitted_frame, command.render_phase,
                              NativePlacementOwner::WriteKind::kExplicitClear);
        }
      }
      if (target.uses_presenter) {
        ++successful_presenter_commands;
      }
    }
    if (diagnostic_frame && command_recorded && target.depth_surface &&
        (target.depth_surface->aspect & VK_IMAGE_ASPECT_STENCIL_BIT)) {
      const NativeFixedFunctionState& fixed = command.fixed_function_state;
      const bool front_writes_stencil =
          draw_command && fixed.stencil_enable && fixed.stencil_write_mask &&
          (fixed.stencil_fail || fixed.stencil_depth_fail || fixed.stencil_pass);
      const bool back_writes_stencil =
          draw_command && fixed.stencil_enable && fixed.two_sided_stencil &&
          fixed.back_stencil_write_mask &&
          (fixed.ccw_stencil_fail || fixed.ccw_stencil_depth_fail ||
           fixed.ccw_stencil_pass);
      bool clears_stencil = false;
      if (command.type == CommandType::kClear) {
        ClearCommand clear{};
        std::memcpy(&clear, command.bytes.data(), sizeof(clear));
        clears_stencil = (clear.flags & 0x20u) != 0;
      }
      if (clears_stencil || front_writes_stencil || back_writes_stencil) {
        if (rendering) {
          dfn.vkCmdEndRendering(command_buffer);
          rendering = false;
        }
        RecordStencilDiagnosticProbe(command_buffer, *target.depth_surface,
                                     clears_stencil ? 31 : 32);
      }
    }
    const auto scene_checkpoint = scene_write_checkpoint_ordinals.find(command_index);
    if (command_recorded && scene_checkpoint != scene_write_checkpoint_ordinals.end() &&
        target.color_surfaces[0] &&
        target.color_surfaces[0]->format == VK_FORMAT_R16G16B16A16_SFLOAT &&
        InitializeContentProbeBuffer()) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      if (content_probe_buffer_.pending_frame != submitted_frame) {
        std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
        content_probe_buffer_.pending_frame = submitted_frame;
        content_probe_buffer_.stages = {};
      }
      uint32_t probe_stage = 0;
      while (probe_stage < content_probe_buffer_.stages.size() &&
             content_probe_buffer_.stages[probe_stage].valid) {
        ++probe_stage;
      }
      NativeSurfaceImage* draw_target = target.color_surfaces[0];
      const bool checkpoint_recorded = RecordContentProbeImage(
          command_buffer, probe_stage, draw_target->resource.image, draw_target->format,
          draw_target->layout, draw_target->width, draw_target->height,
          draw_target->descriptor.handle,
          command.pipeline_state->render_targets[0].address, 8);
      if (checkpoint_recorded) {
        NativeContentProbeStage& stage = content_probe_buffer_.stages[probe_stage];
        const NativeShader* vertex_shader = command.pipeline_state->vertex_shader_resource;
        const NativeShader* pixel_shader = command.pipeline_state->pixel_shader_resource;
        stage.command_type = uint8_t(command.type);
        stage.command_index = uint32_t(command_index);
        stage.checkpoint_ordinal = scene_checkpoint->second;
        stage.checkpoint_count = scene_write_checkpoint_count;
        stage.render_phase = uint32_t(command.render_phase);
        stage.draw_id = diagnostic_draw_id_;
        stage.vertex_shader_hash = vertex_shader ? vertex_shader->hash : 0;
        stage.pixel_shader_hash = pixel_shader ? pixel_shader->hash : 0;
        for (uint32_t texture_stage = 0; texture_stage < stage.texture_handles.size();
             ++texture_stage) {
          stage.texture_handles[texture_stage] =
              command.textures[texture_stage] ? command.textures[texture_stage]->handle : 0;
        }
        REXLOG_INFO(
            "gta4-native-cause: point=scene-write-checkpoint-recorded frame={} "
            "checkpoint={}/{} cmd={} target={:08X}/{:08X} draw={:016X} "
            "vs={:016X} ps={:016X}",
            submitted_frame, stage.checkpoint_ordinal, stage.checkpoint_count,
            stage.command_index, stage.handle, stage.address, stage.draw_id,
            stage.vertex_shader_hash, stage.pixel_shader_hash);
      }
    }
    TraceNativeRendererEvent(
        "command-result",
        fmt::format("type={} recorded={} target={}x{} presenter={}", CommandTypeName(command.type),
                    command_recorded, target.width, target.height, target.uses_presenter));
    if (diagnostic_frame && command_recorded &&
        resolve_predecessor_draw_indices.contains(command_index) && target.color_surfaces[0] &&
        InitializeContentProbeBuffer()) {
      if (rendering) {
        dfn.vkCmdEndRendering(command_buffer);
        rendering = false;
      }
      if (content_probe_buffer_.pending_frame != submitted_frame) {
        std::memset(content_probe_buffer_.mapping, 0, size_t(kContentProbeBufferSize));
        content_probe_buffer_.pending_frame = submitted_frame;
        content_probe_buffer_.stages = {};
      }
      uint32_t probe_stage = 0;
      while (probe_stage < content_probe_buffer_.stages.size() &&
             content_probe_buffer_.stages[probe_stage].valid) {
        ++probe_stage;
      }
      NativeSurfaceImage* draw_target = target.color_surfaces[0];
      RecordContentProbeImage(command_buffer, probe_stage, draw_target->resource.image,
                              draw_target->format, draw_target->layout, draw_target->width,
                              draw_target->height, draw_target->descriptor.handle,
                              command.pipeline_state->render_targets[0].address, 6);
      const NativeShader* vertex_shader = command.pipeline_state->vertex_shader_resource;
      const NativeShader* pixel_shader = command.pipeline_state->pixel_shader_resource;
      const NativeFixedFunctionState& fixed = command.fixed_function_state;
      std::fprintf(stderr,
                   "[ResolvePredecessorDraw] frame=%u index=%zu target=%08X/%08X "
                   "vs=%016llX ps=%016llX textures=%08X,%08X,%08X,%08X "
                   "constants=%016llX/%016llX "
                   "blend=%u/%u/%u/%u alpha=%u/%u/%u write=%08X "
                   "depth=%u/%u/%u stencil=%u/%u viewport=%g,%g,%g,%g,%g,%g "
                   "scissor=%u:%u,%u,%u,%u\n",
                   submitted_frame, command_index, draw_target->descriptor.handle,
                   command.pipeline_state->render_targets[0].address,
                   static_cast<unsigned long long>(vertex_shader ? vertex_shader->hash : 0),
                   static_cast<unsigned long long>(pixel_shader ? pixel_shader->hash : 0),
                   command.pipeline_state->textures[0], command.pipeline_state->textures[1],
                   command.pipeline_state->textures[2], command.pipeline_state->textures[3],
                   static_cast<unsigned long long>(command.vertex_constants_hash),
                   static_cast<unsigned long long>(command.pixel_constants_hash),
                   fixed.blend_enable, fixed.source_blend, fixed.destination_blend,
                   fixed.blend_operation, fixed.source_blend_alpha,
                   fixed.destination_blend_alpha, fixed.blend_operation_alpha,
                   fixed.color_write_mask, fixed.depth_enable, fixed.depth_function,
                   fixed.depth_write_enable, fixed.stencil_enable, fixed.stencil_function,
                   std::bit_cast<float>(fixed.viewport_bits[0]),
                   std::bit_cast<float>(fixed.viewport_bits[1]),
                   std::bit_cast<float>(fixed.viewport_bits[2]),
                   std::bit_cast<float>(fixed.viewport_bits[3]),
                   std::bit_cast<float>(fixed.viewport_bits[4]),
                   std::bit_cast<float>(fixed.viewport_bits[5]),
                   fixed.scissor_enable, fixed.scissor[0], fixed.scissor[1],
                   fixed.scissor[2], fixed.scissor[3]);
      std::fflush(stderr);
    }
    if (trace_final_draw) {
      std::fprintf(stderr,
                   "[FinalDrawResult] frame=%u index=%zu recorded=%u target=%08X/%08X\n",
                   submitted_frame, command_index, unsigned(command_recorded),
                   command.pipeline_state->render_targets[0].handle,
                   command.pipeline_state->render_targets[0].address);
      std::fflush(stderr);
    }
    if (trace_command) {
      REXLOG_WARN(
          "gta4-native-stage: trace={} frame={} stage=command-end index={} type={} "
          "recorded={} pipelines={} textures={} surfaces={}",
          trace_sequence, submitted_frame, command_index, CommandTypeName(command.type),
          command_recorded, native_pipelines_.size(), native_texture_images_.size(),
          native_surface_images_.size());
    }
  }
  EndNativeGpuProfileSpan(command_buffer, native_profile_phase_span);
  if (rendering) {
    dfn.vkCmdEndRendering(command_buffer);
  }
  deterministic_trace_active_ = trace_stages;
  diagnostic_command_index_ = SIZE_MAX;
  if (present_source) {
    TraceNativeRendererEvent(
        "present-begin",
        fmt::format(
            "source={:08X}@{} format={} size={}x{} high-precision={:08X}:vk{} hdr={}:{}",
            present_source->handle, present_source->generation,
            uint32_t(present_source->info.format), present_source->info.width + 1,
            present_source->info.height + 1,
            high_precision_present_source
                ? high_precision_present_source->descriptor.handle
                : 0,
            high_precision_present_source ? uint32_t(high_precision_present_source->format) : 0,
            hdr_output, hdr_headroom));
    if (trace_stages) {
      REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=present-copy-begin generation={}",
                  trace_sequence, submitted_frame, present_source->generation);
    }
    bool present_transfer_written = false;
    const uint64_t present_cpu_begin =
        native_gpu_profile_state_.active
            ? rex::chrono::Clock::QueryHostTickCount()
            : 0;
    const size_t present_profile_span = BeginNativeGpuProfileSpan(
        command_buffer, NativeGpuProfileScopeKind::kPresent,
        RenderPhase::kCompositePostFx);
    const bool present_recorded =
        RecordPresent(command_buffer, presenter_image, presenter_view, width, height,
                      present_source, high_precision_present_source, hdr_output, hdr_headroom,
                      present_transfer_written);
    EndNativeGpuProfileSpan(command_buffer, present_profile_span,
                            present_cpu_begin);
    presenter_transfer_written = present_transfer_written;
    if (present_recorded) {
      presenter_written = true;
      recorded_draw = true;
      ++successful_presenter_commands;
    }
    TraceNativeRendererEvent(
        "present-result",
        fmt::format("recorded={} transfer-written={} presenter-written={}", present_recorded,
                    present_transfer_written, presenter_written));
    if (trace_stages) {
      REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=present-copy-end result={}",
                  trace_sequence, submitted_frame, present_recorded);
    }
  }
  NativeSurfaceImage* final_surface = high_precision_present_source;
  if (!final_surface && active_target.color_surfaces[0]) {
    final_surface = active_target.color_surfaces[0];
  }
  RecordContentProbe(command_buffer, submitted_frame, final_surface, final_composite_input,
                     present_source,
                     presenter_image,
                     presenter_transfer_written ? VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
                                                : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                     width, height);
  if (upload_buffer_.write_offset) {
    ui::vulkan::util::FlushMappedMemoryRange(
        vulkan_provider->vulkan_device(), upload_buffer_.memory, upload_buffer_.memory_type, 0,
        upload_buffer_.allocation_size, upload_buffer_.write_offset);
  }
  const uint64_t semantic_hash = semantic_hash_state ? XXH3_64bits_digest(semantic_hash_state) : 0;
  if (semantic_hash_state) {
    XXH3_freeState(semantic_hash_state);
  }
  if (trace_stages) {
    REXLOG_WARN(
        "gta4-native-stage: trace={} frame={} stage=record-end any-recorded={} present-copy={}",
        trace_sequence, submitted_frame, recorded_draw, presenter_transfer_written);
  }
  if (legacy_diagnostics &&
      (invocation <= 16 || ShouldLogDiagnosticFrame(submitted_frame))) {
    REXLOG_INFO(
        "gta4-native-diag: frame={} invocation={} queued={} draw={}/{}/{} clear={} resolve={} "
        "missing-state={} target-fail={} targets[presenter={},offscreen={}] "
        "recorded[draw-ok={},draw-fail={},clear-ok={},clear-fail={},resolve-ok={},resolve-fail={}] "
        "texture[draws={},missing-draws={},requested={},captured={},bound={}] "
        "state[alpha-blend={},alpha-test={},z-disabled={},zwrite-disabled={},color-mask={}] "
        "presenter-opened={} presenter-success={} surfaces={} textures={} pipelines={} "
        "present-source={} present-generation={} present-copy={} any-recorded={}",
        submitted_frame, invocation, current_frame_.size(), queued_draws, queued_draws_up,
        queued_draws_indexed, queued_clears, queued_resolves, missing_pipeline_state,
        target_failures, presenter_target_commands, offscreen_target_commands, successful_draws,
        failed_draws, successful_clears, failed_clears, successful_resolves, failed_resolves,
        textured_draws, draws_with_missing_textures, texture_requested_bindings,
        texture_captured_bindings, texture_bound_bindings, alpha_blend_draws, alpha_test_draws,
        z_disabled_draws, z_write_disabled_draws, nondefault_color_write_draws, presenter_written,
        successful_presenter_commands, native_surface_images_.size(), native_texture_images_.size(),
        native_pipelines_.size(), bool(present_source),
        present_source ? present_source->generation : 0, presenter_transfer_written, recorded_draw);
    REXLOG_INFO(
        "gta4-native-transition: frame={} semantic={:016X} "
        "rt[first={:08X}/{:08X},last={:08X}/{:08X}] "
        "resolve[first={:08X}/{:08X}->{:08X}@{},last={:08X}/{:08X}->{:08X}@{}] "
        "present={:08X}/{:08X}@{} match-frame-resolve={} commands={}",
        submitted_frame, semantic_hash, first_render_target_handle, first_render_target_address,
        last_render_target_handle, last_render_target_address, first_resolve_source_handle,
        first_resolve_source_address, first_resolve_destination, first_resolve_generation,
        last_resolve_source_handle, last_resolve_source_address, last_resolve_destination,
        last_resolve_generation, present_source ? present_source->handle : 0,
        present_source ? present_source->info.memory.base_address : 0,
        present_source ? present_source->generation : 0, present_matches_frame_resolve,
        current_frame_.size());
    REXLOG_INFO(
        "gta4-native-primitives: frame={} nonup[4={},6={},8={},13={}] "
        "up[4={},6={},8={},13={}] indexed[4={},6={},8={},13={}]",
        submitted_frame, primitive_counts[0][4], primitive_counts[0][6],
        primitive_counts[0][8], primitive_counts[0][13], primitive_counts[1][4],
        primitive_counts[1][6], primitive_counts[1][8], primitive_counts[1][13],
        primitive_counts[2][4], primitive_counts[2][6], primitive_counts[2][8],
        primitive_counts[2][13]);
  }
  deterministic_trace_active_ = trace_stages;
  diagnostic_command_index_ = SIZE_MAX;
  const char* causal_focus =
      missing_pipeline_state || target_failures ? "command-state-or-target"
      : failed_draws || failed_clears           ? "draw-or-clear-recording"
      : failed_resolves                         ? "resolve-recording"
      : draws_with_missing_textures             ? "missing-texture-bindings"
      : !present_matches_frame_resolve           ? "frontbuffer-lineage"
      : successful_presenter_commands == 0       ? "presentation-recording"
                                                  : "gpu-luminance-readback";
  TraceNativeRendererEvent(
      "frame-cause-summary",
      fmt::format(
          "focus={} semantic={:016X} commands={} "
          "draw={}:{} clear={}:{} resolve={}:{} missing-state={} target-fail={} "
          "textures={}:{}:{} missing-draws={} present-match={} presenter-ok={} "
          "rt-first={:08X}:{:08X} rt-last={:08X}:{:08X} "
          "resolve-last={:08X}:{:08X}->{:08X}@{} present={:08X}@{}",
          causal_focus, semantic_hash, current_frame_.size(), successful_draws, failed_draws,
          successful_clears, failed_clears, successful_resolves, failed_resolves,
          missing_pipeline_state, target_failures, texture_requested_bindings,
          texture_captured_bindings, texture_bound_bindings, draws_with_missing_textures,
          present_matches_frame_resolve, successful_presenter_commands,
          first_render_target_handle, first_render_target_address, last_render_target_handle,
          last_render_target_address, last_resolve_source_handle,
          last_resolve_source_address, last_resolve_destination, last_resolve_generation,
          present_source ? present_source->handle : 0,
          present_source ? present_source->generation : 0));
  TraceNativeRendererEvent("frame-end", fmt::format("recorded={}", recorded_draw));
  deterministic_trace_active_ = false;
  diagnostic_draw_id_ = 0;
  diagnostic_submitted_frame_ = 0;
  diagnostic_command_index_ = SIZE_MAX;
  if (transition_trace) {
    transition::Record(
        transition::EventSource::kRenderer, transition::EventType::kFrameRecordEnd, 0, 0,
        submitted_frame, recorded_draw ? transition::kFlagAfter : transition::kFlagError,
        successful_draws, failed_draws,
        (uint64_t(successful_resolves) << 32) | failed_resolves);
  }
  return recorded_draw;
}

bool Gta4NativeGraphicsSystem::PublishFrame(
    const PresentCommand& present,
    const std::shared_ptr<const NativeTextureResource>& present_source,
    const std::shared_ptr<const EnvironmentalDataV1>& environmental_data) {
  SCOPE_profile_cpu_i("gpu", "GTA4 Native PublishFrame");
  const uint32_t width = present.width    ? present.width
                         : present_source ? present_source->info.width + 1
                                          : kDefaultOutputWidth;
  const uint32_t height = present.height   ? present.height
                          : present_source ? present_source->info.height + 1
                                           : kDefaultOutputHeight;
  const uint32_t display_width = present.display_width ? present.display_width : width;
  const uint32_t display_height = present.display_height ? present.display_height : height;
  static std::atomic<uint64_t> present_id{0};
  const uint64_t current_present_id = present_id.fetch_add(1, std::memory_order_relaxed) + 1;
  const bool transition_trace = transition::IsEnabled() && transition::ActiveTransitionId();
  if (transition_trace) {
    transition::Record(transition::EventSource::kRenderer,
                       transition::EventType::kPresentBegin, 0, 0,
                       present.submitted_frame, transition::kFlagBefore,
                       current_present_id,
                       (uint64_t(width) << 32) | height,
                       present_source ? present_source->generation : 0);
  }
  if (!present_source && present.submitted_frame &&
      ShouldLogDiagnosticFrame(present.submitted_frame)) {
    REXLOG_WARN(
        "gta4-native-diag: frame={} frontbuffer={:08X} has no matching native texture generation",
        present.submitted_frame, present.frontbuffer_texture);
  }
  if (ShouldLogDiagnosticFrame(present.submitted_frame)) {
    const uint64_t fetch_hash =
        XXH3_64bits(present.frontbuffer_fetch, sizeof(present.frontbuffer_fetch));
    REXLOG_INFO(
        "gta4-native-present: frame={} requested={:08X} fetch={:016X} "
        "selected={:08X} base={:08X} generation={} render={}x{} display={}x{} "
        "gpu-produced={} environment-sequence={} environment-valid={:016X}",
        present.submitted_frame, present.frontbuffer_texture, fetch_hash,
        present_source ? present_source->handle : 0,
        present_source ? present_source->info.memory.base_address : 0,
        present_source ? present_source->generation : 0,
        width, height, display_width, display_height,
        present_source ? present_source->gpu_produced : false,
        environmental_data ? environmental_data->source_sequence : 0,
        environmental_data ? environmental_data->valid_fields : 0);
  }
  const bool result = ClearGuestOutput(width, height, display_width, display_height,
                                       present.submitted_frame, present_source,
                                       environmental_data);
  if (transition_trace) {
    transition::Record(transition::EventSource::kRenderer,
                       transition::EventType::kPresentEnd, 0, 0,
                       present.submitted_frame,
                       result ? transition::kFlagAfter : transition::kFlagError,
                       current_present_id, result,
                       present_source ? present_source->generation : 0);
  }
  if (result) {
    transition::NotePresent(present.submitted_frame, current_present_id);
  }
  return result;
}

bool Gta4NativeGraphicsSystem::ClearGuestOutput(
    uint32_t width, uint32_t height, uint32_t display_width, uint32_t display_height,
    uint32_t submitted_frame,
    const std::shared_ptr<const NativeTextureResource>& present_source,
    const std::shared_ptr<const EnvironmentalDataV1>& environmental_data) {
  if (!presenter_ || !provider_) {
    return false;
  }

  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  if (!vulkan_device) {
    return false;
  }
  if (!InitializeNativeRendererObjects()) {
    REXLOG_ERROR("gta4-native: failed to initialize native Vulkan renderer objects");
    return false;
  }

  uint32_t indexed_draw_count = 0;
  uint32_t primitive_draw_count = 0;
  uint32_t primitive_up_draw_count = 0;
  uint32_t resolve_count = 0;
  uint32_t clear_count = 0;
  for (const NativeCommand& command : current_frame_) {
    switch (command.type) {
      case CommandType::kDrawIndexedPrimitive:
        ++indexed_draw_count;
        break;
      case CommandType::kDrawPrimitive:
        ++primitive_draw_count;
        break;
      case CommandType::kDrawPrimitiveUp:
        ++primitive_up_draw_count;
        break;
      case CommandType::kResolve:
        ++resolve_count;
        break;
      case CommandType::kClear:
        ++clear_count;
        break;
      default:
        break;
    }
  }
  static std::atomic<uint32_t> indexed_frame_trace_counter{0};
  const bool legacy_diagnostics =
      REXCVAR_GET(gta4_trace_startup_content) || REXCVAR_GET(gta4_trace_native_renderer);
  const bool selected_trace_frame = ShouldLogDiagnosticFrame(submitted_frame);
  const uint32_t trace_sequence =
      legacy_diagnostics && selected_trace_frame
          ? indexed_frame_trace_counter.fetch_add(1, std::memory_order_relaxed)
          : 0;
  const bool trace_stages = legacy_diagnostics && selected_trace_frame;
  if (trace_stages) {
    REXLOG_WARN(
        "gta4-native-stage: trace={} frame={} stage=publish-enter commands={} draw={}/{}/{} "
        "clear={} resolve={} present-source={} generation={}",
        trace_sequence, submitted_frame, current_frame_.size(), primitive_draw_count,
        primitive_up_draw_count, indexed_draw_count, clear_count, resolve_count,
        bool(present_source), present_source ? present_source->generation : 0);
  }

  return presenter_->RefreshGuestOutput(
      width, height, display_width, display_height,
      [this, vulkan_device, width, height, submitted_frame, present_source, environmental_data,
       trace_stages, trace_sequence](ui::Presenter::GuestOutputRefreshContext& context) -> bool {
        auto& vulkan_context =
            static_cast<ui::vulkan::VulkanPresenter::VulkanGuestOutputRefreshContext&>(context);
        const auto& dfn = vulkan_device->functions();
        const VkDevice device = vulkan_device->device();
        const uint64_t native_profile_callback_begin =
            rex::chrono::Clock::QueryHostTickCount();
        uint64_t native_profile_wait_ticks = 0;
        bool native_profile_frame_started = false;
        bool native_profile_submission_committed = false;
        auto native_profile_failure_cleanup = MakeScopeExit([&]() {
          if (native_profile_frame_started &&
              !native_profile_submission_committed) {
            CancelNativeGpuProfileFrame();
          }
        });

        if (trace_stages) {
          REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=refresh-callback-enter",
                      trace_sequence, submitted_frame);
        }

        if (!submission_tracker_) {
          submission_tracker_ =
              std::make_unique<ui::vulkan::VulkanSubmissionTracker>(vulkan_device);
        }
        if (command_buffer_submission_) {
          const uint64_t native_profile_wait_begin =
              rex::chrono::Clock::QueryHostTickCount();
          if (trace_stages) {
            REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=fence-wait-begin submission={}",
                        trace_sequence, submitted_frame, command_buffer_submission_);
          }
          const bool wait_succeeded =
              submission_tracker_->AwaitSubmissionCompletion(command_buffer_submission_);
          native_profile_wait_ticks =
              rex::chrono::Clock::QueryHostTickCount() -
              native_profile_wait_begin;
          if (trace_stages) {
            REXLOG_WARN(
                "gta4-native-stage: trace={} frame={} stage=fence-wait-end submission={} result={}",
                trace_sequence, submitted_frame, command_buffer_submission_, wait_succeeded);
          }
          if (!wait_succeeded) {
            return false;
          }
        }
        AnalyzePendingNativeGpuProfile();
        AnalyzePendingContentProbe();
        AnalyzePendingTranslucentQueries();
        ReleaseUnusedTextureImages();
        if (!EnsureFrameUploadCapacity(present_source)) {
          return false;
        }

        if (!command_pool_) {
          VkCommandPoolCreateInfo pool_info{};
          pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
          pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
          pool_info.queueFamilyIndex = vulkan_device->queue_family_graphics_compute();
          if (dfn.vkCreateCommandPool(device, &pool_info, nullptr, &command_pool_) != VK_SUCCESS) {
            return false;
          }

          VkCommandBufferAllocateInfo allocate_info{};
          allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
          allocate_info.commandPool = command_pool_;
          allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
          allocate_info.commandBufferCount = 1;
          if (dfn.vkAllocateCommandBuffers(device, &allocate_info, &command_buffer_) !=
              VK_SUCCESS) {
            dfn.vkDestroyCommandPool(device, command_pool_, nullptr);
            command_pool_ = VK_NULL_HANDLE;
            return false;
          }
        } else if (dfn.vkResetCommandPool(device, command_pool_, 0) != VK_SUCCESS) {
          return false;
        }

        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        if (dfn.vkBeginCommandBuffer(command_buffer_, &begin_info) != VK_SUCCESS) {
          return false;
        }
        native_profile_frame_started = BeginNativeGpuProfileFrame(
            command_buffer_, submitted_frame, native_profile_wait_ticks);
        if (trace_stages) {
          REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=command-buffer-begun",
                      trace_sequence, submitted_frame);
        }

        VkImageMemoryBarrier acquire_barrier{};
        acquire_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        acquire_barrier.srcAccessMask =
            vulkan_context.image_ever_written_previously()
                ? ui::vulkan::VulkanPresenter::kGuestOutputInternalAccessMask
                : 0;
        acquire_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        acquire_barrier.oldLayout = vulkan_context.image_ever_written_previously()
                                        ? ui::vulkan::VulkanPresenter::kGuestOutputInternalLayout
                                        : VK_IMAGE_LAYOUT_UNDEFINED;
        acquire_barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        acquire_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        acquire_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        acquire_barrier.image = vulkan_context.image();
        acquire_barrier.subresourceRange = ui::vulkan::util::InitializeSubresourceRange();
        dfn.vkCmdPipelineBarrier(command_buffer_,
                                 vulkan_context.image_ever_written_previously()
                                     ? ui::vulkan::VulkanPresenter::kGuestOutputInternalStageMask
                                     : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                 VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0,
                                 nullptr, 1, &acquire_barrier);

        const uint64_t native_profile_texture_begin =
            native_profile_frame_started
                ? rex::chrono::Clock::QueryHostTickCount()
                : 0;
        const size_t native_profile_texture_span = BeginNativeGpuProfileSpan(
            command_buffer_, NativeGpuProfileScopeKind::kTexturePreparation);
        if (!RecordNullImageInitialization(command_buffer_)) {
          return false;
        }
        std::memset(upload_buffer_.mapping, 0, size_t(kDefaultVertexDataSize));
        upload_buffer_.write_offset = kDefaultVertexDataSize;
        if (trace_stages) {
          REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=texture-prepare-begin",
                      trace_sequence, submitted_frame);
        }
        const bool textures_prepared = PrepareFrameTextures(
            command_buffer_, bool(present_source), submitted_frame, trace_stages);
        if (trace_stages) {
          REXLOG_WARN(
              "gta4-native-stage: trace={} frame={} stage=texture-prepare-end result={} "
              "images={} samplers={}",
              trace_sequence, submitted_frame, textures_prepared, native_texture_images_.size(),
              native_samplers_.size());
        }
        if (!textures_prepared) {
          return false;
        }
        if (present_source) {
          if (trace_stages) {
            REXLOG_WARN(
                "gta4-native-stage: trace={} frame={} stage=frontbuffer-prepare-begin "
                "generation={}",
                trace_sequence, submitted_frame, present_source->generation);
          }
          const bool frontbuffer_prepared =
              GetOrCreateTextureImage(command_buffer_, present_source) != nullptr;
          if (trace_stages) {
            REXLOG_WARN(
                "gta4-native-stage: trace={} frame={} stage=frontbuffer-prepare-end result={}",
                trace_sequence, submitted_frame, frontbuffer_prepared);
          }
          if (!frontbuffer_prepared) {
            REXLOG_ERROR("gta4-native: failed to prepare frontbuffer texture generation {}",
                         present_source->generation);
            return false;
          }
        }
        EndNativeGpuProfileSpan(command_buffer_, native_profile_texture_span,
                                native_profile_texture_begin);
        if (native_profile_frame_started) {
          native_gpu_profile_state_.cpu_texture_prepare_ticks =
              rex::chrono::Clock::QueryHostTickCount() -
              native_profile_texture_begin;
        }

        bool presenter_transfer_written = false;
        const uint64_t native_profile_record_begin =
            native_profile_frame_started
                ? rex::chrono::Clock::QueryHostTickCount()
                : 0;
        const size_t native_profile_record_span = BeginNativeGpuProfileSpan(
            command_buffer_, NativeGpuProfileScopeKind::kNativeFrame);
        const bool frame_recorded =
            RecordNativeFrame(command_buffer_, width, height, vulkan_context.image(),
                              vulkan_context.image_view(), submitted_frame, present_source,
                              environmental_data,
                              vulkan_context.hdr_output(), vulkan_context.hdr_headroom(),
                              presenter_transfer_written, trace_stages, trace_sequence);
        EndNativeGpuProfileSpan(command_buffer_, native_profile_record_span,
                                native_profile_record_begin);
        if (native_profile_frame_started) {
          native_gpu_profile_state_.cpu_record_ticks =
              rex::chrono::Clock::QueryHostTickCount() -
              native_profile_record_begin;
        }
        if (trace_stages) {
          REXLOG_WARN(
              "gta4-native-stage: trace={} frame={} stage=record-return result={} present-copy={}",
              trace_sequence, submitted_frame, frame_recorded, presenter_transfer_written);
        }

        VkImageMemoryBarrier release_barrier{};
        release_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        release_barrier.srcAccessMask = presenter_transfer_written
                                            ? VK_ACCESS_TRANSFER_WRITE_BIT
                                            : VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        release_barrier.dstAccessMask = ui::vulkan::VulkanPresenter::kGuestOutputInternalAccessMask;
        release_barrier.oldLayout = presenter_transfer_written
                                        ? VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
                                        : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        release_barrier.newLayout = ui::vulkan::VulkanPresenter::kGuestOutputInternalLayout;
        release_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        release_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        release_barrier.image = vulkan_context.image();
        release_barrier.subresourceRange = ui::vulkan::util::InitializeSubresourceRange();
        dfn.vkCmdPipelineBarrier(command_buffer_,
                                 presenter_transfer_written
                                     ? VK_PIPELINE_STAGE_TRANSFER_BIT
                                     : VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                 ui::vulkan::VulkanPresenter::kGuestOutputInternalStageMask, 0, 0,
                                 nullptr, 0, nullptr, 1, &release_barrier);
        EndNativeGpuProfileFrame(command_buffer_);

        if (trace_stages) {
          REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=command-buffer-end-begin",
                      trace_sequence, submitted_frame);
        }
        const VkResult end_result = dfn.vkEndCommandBuffer(command_buffer_);
        if (trace_stages) {
          REXLOG_WARN(
              "gta4-native-stage: trace={} frame={} stage=command-buffer-end-result result={}",
              trace_sequence, submitted_frame, int32_t(end_result));
        }
        if (end_result != VK_SUCCESS) {
          return false;
        }

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer_;
        const uint64_t submission = submission_tracker_->GetCurrentSubmission();
        ui::vulkan::VulkanSubmissionTracker::FenceAcquisition fence_acquisition(
            submission_tracker_->AcquireFenceToAdvanceSubmission());
        if (!fence_acquisition.fence()) {
          fence_acquisition.SubmissionFailedOrDropped();
          return false;
        }

        VkResult submit_result;
        if (trace_stages) {
          REXLOG_WARN("gta4-native-stage: trace={} frame={} stage=queue-submit-begin submission={}",
                      trace_sequence, submitted_frame, submission);
        }
        const bool transition_submit =
            transition::IsEnabled() && transition::ActiveTransitionId();
        if (transition_submit) {
          transition::Record(transition::EventSource::kRenderer,
                             transition::EventType::kQueueSubmitBegin, 0, 0,
                             submitted_frame, transition::kFlagBefore,
                             submission, command_buffer_submission_, frame_recorded);
        }
        const uint64_t native_profile_submit_begin =
            native_profile_frame_started
                ? rex::chrono::Clock::QueryHostTickCount()
                : 0;
        {
          auto queue =
              vulkan_device->AcquireQueue(vulkan_device->queue_family_graphics_compute(), 0);
          submit_result =
              dfn.vkQueueSubmit(queue.queue(), 1, &submit_info, fence_acquisition.fence());
        }
        if (native_profile_frame_started) {
          native_gpu_profile_state_.cpu_submit_ticks =
              rex::chrono::Clock::QueryHostTickCount() -
              native_profile_submit_begin;
        }
        if (transition_submit) {
          transition::Record(
              transition::EventSource::kRenderer,
              transition::EventType::kQueueSubmitEnd, 0, 0, submitted_frame,
              submit_result == VK_SUCCESS ? transition::kFlagAfter : transition::kFlagError,
              submission, static_cast<uint64_t>(static_cast<int64_t>(submit_result)),
              frame_recorded);
        }
        if (trace_stages) {
          REXLOG_WARN(
              "gta4-native-stage: trace={} frame={} stage=queue-submit-end submission={} result={}",
              trace_sequence, submitted_frame, submission, int32_t(submit_result));
        }
        if (submit_result != VK_SUCCESS) {
          fence_acquisition.SubmissionFailedOrDropped();
          return false;
        }

        command_buffer_submission_ = submission;
        if (native_profile_frame_started) {
          native_gpu_profile_state_.cpu_callback_ticks =
              rex::chrono::Clock::QueryHostTickCount() -
              native_profile_callback_begin;
          native_profile_submission_committed = true;
        }
        context.SetIs8bpc(false);
        return true;
      });
}

void Gta4NativeGraphicsSystem::DestroyNativeRendererObjects() {
  if (!provider_) {
    return;
  }
  auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
  const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
  if (!vulkan_device) {
    return;
  }
  const auto& dfn = vulkan_device->functions();
  const VkDevice device = vulkan_device->device();

  SaveNativePipelineCache();

  smaa_pipeline_.Destroy(vulkan_device);
  split_postfx_pass_.Destroy(vulkan_device);
  sun_shafts_pass_.Destroy(vulkan_device);
  postfx_resource_pool_.Destroy(vulkan_device);

  for (const NativePipeline& native_pipeline : native_pipelines_) {
    if (native_pipeline.pipeline) {
      dfn.vkDestroyPipeline(device, native_pipeline.pipeline, nullptr);
    }
  }
  native_pipelines_.clear();
  for (const NativeResolveConversionPipeline& conversion_pipeline : resolve_conversion_pipelines_) {
    if (conversion_pipeline.pipeline) {
      dfn.vkDestroyPipeline(device, conversion_pipeline.pipeline, nullptr);
    }
  }
  resolve_conversion_pipelines_.clear();
  if (hdr_present_pipeline_) {
    dfn.vkDestroyPipeline(device, hdr_present_pipeline_, nullptr);
    hdr_present_pipeline_ = VK_NULL_HANDLE;
  }
  for (const NativeSampler& native_sampler : native_samplers_) {
    if (native_sampler.sampler) {
      dfn.vkDestroySampler(device, native_sampler.sampler, nullptr);
    }
  }
  native_samplers_.clear();
  if (native_pipeline_cache_) {
    dfn.vkDestroyPipelineCache(device, native_pipeline_cache_, nullptr);
    native_pipeline_cache_ = VK_NULL_HANDLE;
  }
  native_pipeline_cache_dirty_ = false;
  for (auto& [generation, image] : native_texture_images_) {
    for (VkImageView mip_view : image->mip_views) {
      if (mip_view) {
        dfn.vkDestroyImageView(device, mip_view, nullptr);
      }
    }
    if (image->resource.view) {
      dfn.vkDestroyImageView(device, image->resource.view, nullptr);
    }
    if (image->resource.image) {
      dfn.vkDestroyImage(device, image->resource.image, nullptr);
    }
    if (image->resource.memory) {
      dfn.vkFreeMemory(device, image->resource.memory, nullptr);
    }
  }
  native_texture_images_.clear();
  for (const auto& image : native_surface_images_) {
    if (image->depth_handoff_stencil_scratch_buffer) {
      dfn.vkDestroyBuffer(device,
                          image->depth_handoff_stencil_scratch_buffer, nullptr);
    }
    if (image->depth_handoff_stencil_scratch_memory) {
      dfn.vkFreeMemory(device, image->depth_handoff_stencil_scratch_memory,
                       nullptr);
    }
    if (image->sampled_view) {
      dfn.vkDestroyImageView(device, image->sampled_view, nullptr);
    }
    if (image->resource.view) {
      dfn.vkDestroyImageView(device, image->resource.view, nullptr);
    }
    if (image->resource.image) {
      dfn.vkDestroyImage(device, image->resource.image, nullptr);
    }
    if (image->resource.memory) {
      dfn.vkFreeMemory(device, image->resource.memory, nullptr);
    }
  }
  native_surface_images_.clear();
  native_placement_owners_.clear();
  next_surface_write_serial_ = 1;
  reflection_resources_.clear();
  if (resolve_conversion_pipeline_layout_) {
    dfn.vkDestroyPipelineLayout(device, resolve_conversion_pipeline_layout_, nullptr);
  }
  if (resolve_conversion_descriptor_set_layout_) {
    dfn.vkDestroyDescriptorSetLayout(device, resolve_conversion_descriptor_set_layout_, nullptr);
  }
  if (pipeline_layout_) {
    dfn.vkDestroyPipelineLayout(device, pipeline_layout_, nullptr);
  }
  if (descriptor_pool_) {
    dfn.vkDestroyDescriptorPool(device, descriptor_pool_, nullptr);
  }
  if (frame_descriptor_pool_) {
    dfn.vkDestroyDescriptorPool(device, frame_descriptor_pool_, nullptr);
  }
  for (VkDescriptorSetLayout layout : descriptor_set_layouts_) {
    if (layout) {
      dfn.vkDestroyDescriptorSetLayout(device, layout, nullptr);
    }
  }
  if (null_sampler_) {
    dfn.vkDestroySampler(device, null_sampler_, nullptr);
  }

  for (NativeImageResource* resource :
       {&null_texture_2d_, &null_texture_2d_array_, &null_texture_3d_,
        &null_texture_cube_}) {
    if (resource->view) {
      dfn.vkDestroyImageView(device, resource->view, nullptr);
    }
    if (resource->image) {
      dfn.vkDestroyImage(device, resource->image, nullptr);
    }
    if (resource->memory) {
      dfn.vkFreeMemory(device, resource->memory, nullptr);
    }
    *resource = {};
  }

  DestroyNativeUploadBuffer(upload_buffer_);
  DestroyContentProbeBuffer();
  DestroyTranslucentQueryPool();
  DestroyNativeGpuProfiler();
  pipeline_layout_ = VK_NULL_HANDLE;
  resolve_conversion_pipeline_layout_ = VK_NULL_HANDLE;
  resolve_conversion_descriptor_set_layout_ = VK_NULL_HANDLE;
  descriptor_pool_ = VK_NULL_HANDLE;
  frame_descriptor_pool_ = VK_NULL_HANDLE;
  frame_descriptor_draw_capacity_ = 0;
  frame_descriptor_resolve_capacity_ = 0;
  frame_descriptor_combined_set_capacity_ = 0;
  descriptor_set_layouts_.fill(VK_NULL_HANDLE);
  descriptor_sets_.fill(VK_NULL_HANDLE);
  null_sampler_ = VK_NULL_HANDLE;
  native_descriptor_capacity_ = 0;
  null_images_initialized_ = false;
}

void Gta4NativeGraphicsSystem::DestroyVulkanWorkerObjects() {
  if (submission_tracker_ && command_buffer_submission_) {
    submission_tracker_->AwaitSubmissionCompletion(command_buffer_submission_);
  }
  AnalyzePendingNativeGpuProfile();
  DestroyNativeRendererObjects();
  DestroyShaderResources();
  submission_tracker_.reset();
  command_buffer_submission_ = 0;
  command_buffer_ = VK_NULL_HANDLE;

  if (provider_) {
    auto* vulkan_provider = static_cast<ui::vulkan::VulkanProvider*>(provider_.get());
    const ui::vulkan::VulkanDevice* vulkan_device = vulkan_provider->vulkan_device();
    if (vulkan_device) {
      const auto& dfn = vulkan_device->functions();
      const VkDevice device = vulkan_device->device();
      if (clear_framebuffer_) {
        dfn.vkDestroyFramebuffer(device, clear_framebuffer_, nullptr);
      }
      if (clear_render_pass_) {
        dfn.vkDestroyRenderPass(device, clear_render_pass_, nullptr);
      }
      if (command_pool_) {
        dfn.vkDestroyCommandPool(device, command_pool_, nullptr);
      }
    }
  }
  clear_framebuffer_ = VK_NULL_HANDLE;
  clear_framebuffer_image_version_ = 0;
  clear_render_pass_ = VK_NULL_HANDLE;
  command_pool_ = VK_NULL_HANDLE;
  vertex_declarations_.clear();
  next_vertex_declaration_generation_ = 1;
  {
    std::lock_guard lock(texture_resource_mutex_);
    texture_resources_.clear();
    dirty_texture_handles_.clear();
    next_texture_generation_ = 1;
  }
}

void Gta4NativeGraphicsSystem::Shutdown() {
  if (render_worker_running_.exchange(false)) {
    render_condition_.notify_all();
    if (render_worker_.joinable()) {
      render_worker_.join();
    }
  }

  if (presenter_) {
    if (app_context_) {
      app_context_->CallInUIThreadSynchronous([this]() { presenter_.reset(); });
    } else {
      presenter_.reset();
    }
  }
  provider_.reset();
  app_context_ = nullptr;
  memory_ = nullptr;
}

}  // namespace rex::graphics::gta4_native
