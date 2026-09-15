#pragma once

#include <algorithm>
#include <span>
#include <rex/ui/vulkan/api.h>

namespace rex::ui::vulkan {

struct PresentModeOptions {
  bool prefer_fifo = false;
  bool allow_mailbox = false;
  bool allow_immediate = false;
  bool allow_fifo_relaxed = false;
  bool operator==(const PresentModeOptions&) const = default;
};

inline VkPresentModeKHR SelectPresentMode(const PresentModeOptions& options,
                                          std::span<const VkPresentModeKHR> supported) {
  const auto has = [&](VkPresentModeKHR mode) {
    return std::find(supported.begin(), supported.end(), mode) != supported.end();
  };
  if (options.prefer_fifo) return VK_PRESENT_MODE_FIFO_KHR;
  if (options.allow_mailbox && has(VK_PRESENT_MODE_MAILBOX_KHR))
    return VK_PRESENT_MODE_MAILBOX_KHR;
  if (options.allow_immediate && has(VK_PRESENT_MODE_IMMEDIATE_KHR))
    return VK_PRESENT_MODE_IMMEDIATE_KHR;
  if (options.allow_fifo_relaxed && has(VK_PRESENT_MODE_FIFO_RELAXED_KHR))
    return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
  // FIFO is mandatory. Never request an unsupported tearing mode.
  return VK_PRESENT_MODE_FIFO_KHR;
}

}  // namespace rex::ui::vulkan
