#pragma once

#include <array>
#include <cstdint>

#include "input/context_touch_layout.h"

namespace gta4::input {

struct ContextTouchOverlaySnapshot {
  ContextTouchLayout layout{};
  std::array<uint8_t, ContextTouchLayout::kMaximumControls> active{};
  float movement_x = 0.0f;
  float movement_y = 0.0f;
  float movement_origin_x = 0.0f;
  float movement_origin_y = 0.0f;
  float right_x = 0.0f;
  float right_y = 0.0f;
  float fade_alpha = 1.0f;
  bool movement_owned = false;
  bool editor = false;
  bool visible = false;
};

// Host values, not guest-endian XInput fields. Reading never consumes input.
struct TouchNativePadState {
  uint64_t epoch = 0;
  uint16_t buttons = 0;
  uint8_t left_trigger = 0;
  uint8_t right_trigger = 0;
  int16_t left_x = 0;
  int16_t left_y = 0;
  int16_t right_x = 0;
  int16_t right_y = 0;
  bool active = false;
};

void InitializeContextTouchControls() noexcept;
void ShutdownContextTouchControls() noexcept;
// Guest presentation boundaries can stop normal input polling while a loading
// screen or cutscene is drawn. Suppression lasts until a fresh input poll.
void SuspendContextTouchGameplay() noexcept;
uint64_t ContextTouchPresentationRevision() noexcept;
bool ContextTouchGameplayInputAdmitted() noexcept;
// HUD taps use native weapon-cycle input, never direct inventory mutation.
bool ContextTouchWeaponCycleAdmitted() noexcept;
bool QueueContextTouchWeaponCycle(uint64_t epoch, uint64_t pointer_id) noexcept;
class ContextTouchGameplayTransition final {
 public:
  ContextTouchGameplayTransition() noexcept;
  ~ContextTouchGameplayTransition();
  ContextTouchGameplayTransition(const ContextTouchGameplayTransition&) = delete;
  ContextTouchGameplayTransition& operator=(const ContextTouchGameplayTransition&) = delete;
};
// Explicit native menu action. Editing previews a gameplay layout over the
// current screen without changing the saved Off/On/Auto mode.
void RequestContextTouchEditor() noexcept;
bool ContextTouchEditorRequested() noexcept;
bool IsContextTouchEditorActive() noexcept;
// Ownership includes the noninteractive one-second fade after Done, so an
// editor gesture cannot also activate the native menu beneath it.
bool ContextTouchEditorCapturesInput(uint64_t monotonic_ns = 0) noexcept;
ContextTouchOverlaySnapshot GetContextTouchOverlaySnapshot() noexcept;
// Visual state may fade after input has stopped. A zero timestamp uses the
// host monotonic clock; an explicit timestamp supports deterministic testing.
ContextTouchOverlaySnapshot GetContextTouchDrawableOverlaySnapshot(
    uint64_t monotonic_ns = 0) noexcept;
// epoch == 0 selects the latest frozen poll for host adapters.
bool GetTouchNativePadState(uint64_t epoch, TouchNativePadState* output) noexcept;

// Called only at the original, already admitted scoped-camera zoom decoder.
int32_t MergeTouchScopedZoom(uint64_t epoch, uint32_t input_user, uint32_t action,
                            int32_t original) noexcept;

void ObserveTouchScriptQuery(TouchScriptQueryKind kind, uint32_t action, uint64_t epoch,
                             uint32_t input_group = 0, uint32_t script_thread = 0,
                             uint64_t generation = 0) noexcept;
void ObserveTouchParachuteState(uint32_t state, uint64_t epoch,
                                uint32_t script_thread = 0) noexcept;
uint32_t GetTouchScriptQueryValue(TouchScriptQueryKind kind, uint32_t action,
                                  uint64_t epoch, uint32_t input_group = 0,
                                  uint32_t script_thread = 0, uint64_t generation = 0) noexcept;
bool GetTouchScriptAnalogueSticks(uint64_t epoch, std::array<int32_t, 4>* axes,
                                 uint32_t input_group = 0, uint32_t script_thread = 0,
                                 uint64_t generation = 0) noexcept;
bool MergeTouchScriptQueryResult(uint8_t* base, uint32_t call_context, TouchScriptQueryKind kind,
                                 uint64_t epoch, uint32_t script_thread = 0,
                                 uint64_t generation = 0) noexcept;
bool MergeTouchScriptAnalogueStickResults(uint8_t* base, uint32_t call_context,
                                          uint64_t epoch, uint32_t script_thread = 0,
                                          uint64_t generation = 0) noexcept;

}  // namespace gta4::input
