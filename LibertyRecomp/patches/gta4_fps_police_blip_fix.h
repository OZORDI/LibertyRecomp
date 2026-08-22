#pragma once

#include <atomic>
#include <cstdint>

namespace gta4::fps::police_blip
{
class LogicalFrameCounter30Hz final
{
public:
    void Reset() noexcept;
    void Advance(float elapsedSeconds) noexcept;

    [[nodiscard]] std::uint32_t Value() const noexcept;

private:
    double elapsedRemainderMilliseconds_ = 0.0;
    std::atomic<std::uint32_t> value_{0};
};

// Shared 30 Hz counter for other proven raw-frame-counter fixes.
[[nodiscard]] std::uint32_t GetLogicalFrameCounter() noexcept;
}
