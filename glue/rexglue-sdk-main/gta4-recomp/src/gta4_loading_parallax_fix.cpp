#include "gta4_loading_parallax_fix.h"

#include <array>
#include <bit>
#include <cstdint>

#include "gta4_init.h"

namespace {

// Layout and limits are taken from the generated sub_82143DC8 animator and
// sub_82145968 loading-screen definition parser.
constexpr uint32_t kCumulativeElapsedGlobal = 0x831D5338;
constexpr uint32_t kFrameDeltaGlobal = 0x831D533C;
constexpr uint32_t kCurrentScreenGlobal = 0x831D5340;
constexpr uint32_t kDefinitionsBase = 0x831D5498;
constexpr uint32_t kScreenStride = 400;
constexpr uint32_t kLayerCountOffset = 4;
constexpr uint32_t kLayerStride = 96;
constexpr uint32_t kScaleAccumulatorOffset = 56;
constexpr uint32_t kMovementAccumulatorOffset = 60;
constexpr uint32_t kScreenCount = 14;
constexpr uint32_t kLayersPerScreen = 4;
constexpr uint32_t kNegativeInfinityBits = 0xFF800000;

struct LayerSnapshot {
  uint32_t scale_address;
  uint32_t movement_address;
  float scale;
  float movement;
};

thread_local gta4::loading_parallax::LogicalAnimationClock animation_clock;

float LoadGuestFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<float>(REX_LOAD_U32(address));
}

void StoreGuestFloat(uint8_t* base, uint32_t address, float value) {
  REX_STORE_U32(address, std::bit_cast<uint32_t>(value));
}

}  // namespace

extern "C" void sub_82143DC8(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = REX_LOAD_U32(kCurrentScreenGlobal);
  if (screen >= kScreenCount) {
    __imp__sub_82143DC8(ctx, base);
    return;
  }

  const uint32_t screen_base = kDefinitionsBase + screen * kScreenStride;
  const uint32_t layer_count = REX_LOAD_U32(screen_base + kLayerCountOffset);
  if (layer_count > kLayersPerScreen) {
    __imp__sub_82143DC8(ctx, base);
    return;
  }

  const float cumulative_elapsed = LoadGuestFloat(base, kCumulativeElapsedGlobal);
  const float frame_delta = LoadGuestFloat(base, kFrameDeltaGlobal);
  const bool advance_animation = animation_clock.Observe(cumulative_elapsed, frame_delta);
  std::array<LayerSnapshot, kLayersPerScreen> snapshots = {};
  for (uint32_t layer = 0; layer < layer_count; ++layer) {
    const uint32_t layer_base = screen_base + layer * kLayerStride;
    const uint32_t scale_address = layer_base + kScaleAccumulatorOffset;
    const uint32_t movement_address = layer_base + kMovementAccumulatorOffset;
    snapshots[layer] = {scale_address, movement_address, LoadGuestFloat(base, scale_address),
                        LoadGuestFloat(base, movement_address)};
    if (!advance_animation) {
      REX_STORE_U32(scale_address, kNegativeInfinityBits);
      REX_STORE_U32(movement_address, kNegativeInfinityBits);
    }
  }

  __imp__sub_82143DC8(ctx, base);

  if (!advance_animation) {
    // The sentinels keep the original routine on both non-trigger branches,
    // allowing it to render without mutating layer position or scale. Restore
    // the exact timer bits before any other guest code can observe them.
    for (uint32_t layer = 0; layer < layer_count; ++layer) {
      const LayerSnapshot& snapshot = snapshots[layer];
      StoreGuestFloat(base, snapshot.scale_address, snapshot.scale);
      StoreGuestFloat(base, snapshot.movement_address, snapshot.movement);
    }
    return;
  }

  // If the original call changed screens or rebuilt this definition, leave
  // all resulting state untouched.
  if (REX_LOAD_U32(kCurrentScreenGlobal) != screen ||
      REX_LOAD_U32(screen_base + kLayerCountOffset) != layer_count) {
    return;
  }

  for (uint32_t layer = 0; layer < layer_count; ++layer) {
    const LayerSnapshot& snapshot = snapshots[layer];
    const float scale_after = LoadGuestFloat(base, snapshot.scale_address);
    const float corrected_scale =
        gta4::loading_parallax::CorrectAccumulator(snapshot.scale, scale_after, frame_delta);
    if (std::bit_cast<uint32_t>(corrected_scale) != std::bit_cast<uint32_t>(scale_after)) {
      StoreGuestFloat(base, snapshot.scale_address, corrected_scale);
    }

    const float movement_after = LoadGuestFloat(base, snapshot.movement_address);
    const float corrected_movement = gta4::loading_parallax::CorrectAccumulator(
        snapshot.movement, movement_after, cumulative_elapsed);
    if (std::bit_cast<uint32_t>(corrected_movement) != std::bit_cast<uint32_t>(movement_after)) {
      StoreGuestFloat(base, snapshot.movement_address, corrected_movement);
    }
  }
}
