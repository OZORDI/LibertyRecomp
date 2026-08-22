#pragma once

#include <cstdint>

namespace gta4::fps::ladder
{
// Return address of CTaskComplexClimbLadder's CreateSubTask call to the
// CTaskSimpleMoveAchieveHeading constructor in the retail Xbox executable.
inline constexpr std::uint32_t kMoveAchieveHeadingReturnAddress = 0x826A1B68;

// Exact binary32 value used by CTaskSimpleSlideToCoord's heading check.
inline constexpr float kHeadingAngleThreshold = 0x1.99999ap-4f;
}
