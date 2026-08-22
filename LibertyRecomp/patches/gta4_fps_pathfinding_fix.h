#pragma once

namespace gta4::fps::pathfinding
{
class StaticCounterAccumulator30Hz final
{
public:
    [[nodiscard]] bool Advance(float timeStepSeconds) noexcept;

private:
    float remainderSeconds_ = 0.0f;
};
}
