#pragma once

#include <array>
#include <cstdint>

#include <rex/input/sony_feedback.h>

struct PPCContext;

// Guest addresses are transient comparison tokens for the calling guest hook.
// Never retain this object or dereference its addresses on a host/SDL thread.
struct GTA4SonyLocalPlayer {
  uint32_t user = 0;
  uint64_t generation = 0;
  uint32_t ped = 0;
  uint32_t vehicle = 0;
  bool position_valid = false;
  std::array<float, 3> position{};
  rex::input::sony::GameState state{};
};

// Existing retail input wrappers call these after their original exactly once.
void GTA4_SonyEndPoll(PPCContext& context, uint8_t* base);
void GTA4_SonyReplay(PPCContext& context, uint8_t* base, uint32_t control);

// Resolves the local player under the title's primary-player alias scope. The
// caller's PPC registers are unchanged. False includes menu/dead/control-owned
// states and a disconnected/disabled Sony device.
bool GTA4_SonyReadLocalPlayer(PPCContext& context, uint8_t* base,
                             GTA4SonyLocalPlayer& player);
void GTA4_SonyEmit(const GTA4SonyLocalPlayer& player, rex::input::sony::Event event,
                   float strength);
