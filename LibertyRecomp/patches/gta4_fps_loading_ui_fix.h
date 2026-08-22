#pragma once

#include <cstdint>

struct PPCContext;

// HUD text-message draw-state setter. The hook only changes the CD-spinner
// frame gate at sub_82223CF8's proven callsite.
extern "C" void sub_821F2FD8(PPCContext& ctx, std::uint8_t* base);
