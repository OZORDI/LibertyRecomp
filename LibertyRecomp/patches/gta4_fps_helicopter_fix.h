#pragma once

#include <cstdint>

namespace gta4::fps::helicopter
{
using LogicalFrameCounterProvider = std::uint32_t (*)() noexcept;

// Supplies the shared FusionFix-style 30 Hz frame counter. Passing nullptr
// leaves the retail frame-counter inputs unchanged.
void SetLogicalFrameCounterProvider(
    LogicalFrameCounterProvider provider) noexcept;
}
