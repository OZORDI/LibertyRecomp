#pragma once

#include <array>
#include <cstdint>

struct PPCContext;

using GTA4GuestFunction = void (*)(PPCContext& context, uint8_t* base);

namespace rex::input {
struct AbsolutePointerEvent;
}

struct GTA4TouchExtension {
  // Prepare context and edge latches before draining this poll's pointers.
  void (*begin_poll)(PPCContext& context, uint8_t* base, uint64_t epoch,
                      bool frontend, bool map) = nullptr;
  // Called on the guest input thread for gameplay pointers not reserved by
  // frontend, map, or minimap semantics. Returning true claims the event.
  bool (*on_pointer_event)(const rex::input::AbsolutePointerEvent& event,
                           PPCContext& context, uint8_t* base,
                           uint64_t epoch) = nullptr;
  // Explicit pressed latches preserve a complete Down+Up tap drained within
  // one guest poll. Both arrays are frozen with the coordinator epoch.
  void (*collect_virtual_keys)(uint64_t epoch, std::array<uint8_t, 256>& down,
                               std::array<uint8_t, 256>& pressed) = nullptr;
  void (*on_controls_disabled)(PPCContext& context, uint8_t* base,
                               uint64_t epoch) = nullptr;
  // control is the original sub_822B7DD0 r3 value. The callback runs after
  // retail and keyboard/mouse action injection and must only merge values.
  void (*on_control_replay)(PPCContext& context, uint8_t* base, uint32_t control,
                            uint32_t caller, uint64_t epoch) = nullptr;
};

void GTA4_RegisterTouchExtension(GTA4TouchExtension extension) noexcept;
void GTA4_TouchConsumePoll(PPCContext& context, uint8_t* base, uint64_t epoch);
void GTA4_TouchObserveControlReplay(PPCContext& context, uint8_t* base,
                                    uint32_t control, uint32_t caller,
                                    uint64_t epoch);
// Multiplexes the frontend list draw hook so touch geometry capture and other
// frontend extensions can share the single guest hook implementation.
void GTA4_TouchCaptureFrontendDraw(PPCContext& context, uint8_t* base,
                                   GTA4GuestFunction draw_function);
bool GTA4_TouchVirtualKeyDown(uint16_t key) noexcept;
bool GTA4_TouchVirtualKeyPressed(uint64_t epoch, uint16_t key) noexcept;
// UI-thread-safe ownership signal. It only publishes host state; cancellation
// and all guest interaction remain on the next guest input poll.
void GTA4_SetTouchTitleInputOwned(bool owned) noexcept;
bool GTA4_TouchTitleInputOwned() noexcept;

// Multiplexed by legal_screen_trace.cpp's existing HUD text hook.
void GTA4_TouchObserveHudSubmit(const PPCContext& context, uint8_t* base) noexcept;
