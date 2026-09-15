#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

#include "input/context_touch_layout.h"

struct PPCContext;

namespace gta4::input {

struct TouchWeaponSlot {
  uint32_t slot = 0;
  uint32_t type = 0;
  uint16_t ammo = 0;
  std::array<char, 32> native_identifier{};
  bool owned = false;
  bool selectable = false;
};

// Copied facts only. Identities are opaque on the host; never dereference them
// from the UI thread. A task state is not permission to start that task.
struct TouchContextSnapshot {
  uint64_t epoch = 0;
  uint64_t generation = 0;
  uint64_t presentation_revision = 0;
  uint32_t player_identity = 0;
  uint32_t player_generation = 0;
  uint32_t vehicle_identity = 0;
  uint32_t input_user = 0;
  ContextTouchMode base_mode = ContextTouchMode::kDisabled;
  uint32_t weapon_type = 0;
  uint32_t weapon_slot = 0;
  uint32_t clip_ammo = 0;
  uint32_t zoom_action = 24;
  std::array<TouchWeaponSlot, 11> weapons{};
  // Final normalized sprite rectangle, after native alignment and host aspect
  // layout. Absent unless the native weapon icon was actually submitted.
  std::optional<ContextTouchHudBounds> weapon_hud_bounds;
  uint8_t aim_threshold = 0;
  bool alternate_aim_setting = false;
  bool aim_settings_known = false;
  bool valid = false;
  bool input_user_known = false;
  bool inventory_known = false;
  bool playing = false;
  bool gameplay_allowed = false;
  // Native/script input admission is separate from gameplay control
  // permission. Scripts may accept input while player control is disabled.
  bool native_input_allowed = false;
  bool frontend = false;
  bool map = false;
  bool phone_visible = false;
  bool phone_visibility_known = false;
  bool minigame_active = false;
  bool loading = false;
  // Includes native preparation while the main cutscene state is still idle.
  bool cutscene = false;
  bool armed = false;
  bool weapon_known = false;
  bool clip_known = false;
  bool aiming = false;  // Input request, not proof of a lock-on target.
  bool in_cover = false;
  bool cover_known = false;
  bool melee = false;
  bool melee_known = false;
  // Native GET_VEHICLE_PLAYER_WOULD_ENTER selection, before an Enter input.
  bool can_enter_vehicle = false;
  bool scoped_zoom = false;  // Supplied by the native camera observation.
};

// The checked reader also allows deterministic tests with sparse guest memory.
// Bytes returned by read retain the guest's big-endian representation.
struct TouchContextMemory {
  void* opaque = nullptr;
  bool (*read)(void*, uint32_t, void*, size_t) = nullptr;
};

// Executes on the guest thread. The selector owns distance, door, lock and
// vehicle suitability decisions; a copied task pointer is not eligibility.
struct TouchContextQueries {
  void* opaque = nullptr;
  uint32_t (*vehicle_player_would_enter)(void*, uint32_t ped,
                                       uint32_t forward_vector) = nullptr;
};

TouchContextSnapshot ReadTouchContextFacts(const TouchContextMemory& memory,
                                          bool frontend, bool map,
                                          TouchContextQueries queries = {}) noexcept;

class TouchContextGeneration {
 public:
  uint64_t Advance(const TouchContextSnapshot& snapshot) noexcept;

 private:
  TouchContextSnapshot previous_{};
  uint64_t generation_ = 0;
};

void CaptureTouchContext(PPCContext& context, uint8_t* base, uint64_t epoch,
                         bool frontend, bool map) noexcept;
TouchContextSnapshot GetTouchContextSnapshot() noexcept;

struct TouchWeaponHudSnapshot {
  uint64_t epoch = 0;
  uint64_t generation = 0;
  uint64_t presentation_revision = 0;
  uint64_t presentation_generation = 0;
  uint32_t weapon_type = 0;
  uint32_t weapon_slot = 0;
  std::optional<ContextTouchHudBounds> bounds;
};

// A matching native HUD pass always publishes, including an empty result when
// HUD suppression, fading to zero or a missing texture prevented submission.
void PublishTouchWeaponHudSnapshot(TouchWeaponHudSnapshot value) noexcept;
TouchWeaponHudSnapshot GetTouchWeaponHudSnapshot() noexcept;
// Coordinates have already passed through native alignment and the current
// host aspect transform. Keep only the visible portion of a nontransparent draw.
std::optional<ContextTouchHudBounds> ClipTouchWeaponHudBounds(
    double left, double top, double right, double bottom, uint32_t color) noexcept;
std::optional<ContextTouchHudBounds> ReadTouchWeaponHudBounds(
    const TouchWeaponHudSnapshot& hud, const TouchContextSnapshot& context,
    uint64_t presentation_generation) noexcept;
// Composed inside the existing aspect scope on modern hosts and directly in
// the shared sprite hook on legacy hosts. Original is invoked exactly once.
void DrawTouchWeaponHudSprite(PPCContext& context, uint8_t* base,
                              void (*original)(PPCContext&, uint8_t*));

struct TouchVisiblePromptSnapshot {
  static constexpr size_t kMaximumControls = 16;
  uint64_t epoch = 0;
  uint64_t generation = 0;
  uint32_t help_object = 0;
  std::array<TouchScriptControl, kMaximumControls> controls{};
  size_t control_count = 0;
  bool visible = false;
};

// Resolves input identity only. The binding action was observed in retail's
// INPUT_ resolver; its binding category is not a script input group.
std::optional<TouchScriptControl> DecodeTouchHelpToken(
    std::string_view token, uint32_t native_glyph,
    std::optional<uint32_t> binding_action = std::nullopt) noexcept;
TouchVisiblePromptSnapshot GetTouchVisiblePromptSnapshot(uint64_t epoch,
                                                        uint64_t generation) noexcept;
// Native GET_ID_OF_THIS_THREAD identity, not its reusable allocation address.
uint32_t ReadTouchScriptThread(const TouchContextMemory& memory) noexcept;
uint32_t ReadTouchScriptThread(uint8_t* base) noexcept;
std::optional<uint32_t> ReadTouchParachuteState(const TouchContextMemory& memory) noexcept;
std::optional<uint32_t> ReadTouchParachuteState(uint8_t* base) noexcept;

// Narrow production help observer, composed with the existing text hooks.
// Tokens remain native input identities; localized prose never becomes an
// inferred action. The observer does not write guest state.
void BeginTouchHelpDraw(uint8_t* base, uint32_t help_object) noexcept;
void ObserveTouchHelpText(uint8_t* base, uint32_t text) noexcept;
void EndTouchHelpText() noexcept;
void EndTouchHelpDraw() noexcept;
void ResolveTouchHelpToken(PPCContext& context, uint8_t* base,
                           void (*original)(PPCContext&, uint8_t*));
void ResolveTouchHelpBinding(PPCContext& context, uint8_t* base,
                             void (*original)(PPCContext&, uint8_t*));

// Small bounded observer shared by production hooks and deterministic tests.
class TouchHelpObserver {
 public:
  void Begin(uint32_t help_object, uint64_t epoch, uint64_t generation) noexcept;
  void Text(bool submitted) noexcept;
  void Token(const TouchScriptControl& control) noexcept;
  TouchVisiblePromptSnapshot Finish() noexcept;

 private:
  TouchVisiblePromptSnapshot snapshot_{};
  bool text_submitted_ = false;
};

}  // namespace gta4::input
