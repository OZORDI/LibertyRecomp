#pragma once
#include <cstdint>

struct PPCContext;
// Observation only; the caller continues its existing touch/help/original path.
void GTA4_FontSelectionTraceText(const PPCContext& ctx, uint8_t* base);
void GTA4_FontSelectionTraceBinding(uint32_t logical_font, uint32_t owner, uint32_t texture,
                                    uint32_t stage);
