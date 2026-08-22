#pragma once

#include <cstdint>

struct PPCContext;

// CHandShaker::Process. The hook self-registers by overriding the generated
// weak PPC symbol at 0x8236B4D8.
extern "C" void sub_8236B4D8(PPCContext& ctx, std::uint8_t* base);
