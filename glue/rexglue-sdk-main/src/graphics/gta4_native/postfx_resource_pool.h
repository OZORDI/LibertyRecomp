#pragma once

#include <cstdint>

#include <rex/graphics/gta4_native/title_commands.h>
#include <rex/ui/vulkan/api.h>

namespace rex::ui::vulkan {
class VulkanDevice;
}

namespace rex::graphics::gta4_native {

struct PostFxExtent {
  uint32_t width = 0;
  uint32_t height = 0;

  bool operator==(const PostFxExtent&) const = default;
};

class PostFxScheduler {
 public:
  void BeginFrame() {
    composite_open_ = false;
    scene_capture_pending_ = false;
    scene_captured_ = false;
  }
  void ObserveMarker(RenderPhase phase, RenderPhaseEvent event) {
    if (phase != RenderPhase::kCompositePostFx) {
      return;
    }
    if (event == RenderPhaseEvent::kBegin) {
      composite_open_ = true;
      scene_capture_pending_ = true;
      scene_captured_ = false;
    } else {
      composite_open_ = false;
      scene_capture_pending_ = false;
    }
  }
  bool scene_capture_pending() const { return scene_capture_pending_; }
  bool scene_captured() const { return scene_captured_; }
  void FinishSceneCapture(bool succeeded) {
    if (!composite_open_ || !scene_capture_pending_) {
      return;
    }
    scene_capture_pending_ = false;
    scene_captured_ = succeeded;
  }

 private:
  bool composite_open_ = false;
  bool scene_capture_pending_ = false;
  bool scene_captured_ = false;
};

constexpr PostFxExtent CalculatePostFxExtent(uint32_t width, uint32_t height, uint32_t divisor) {
  if (!width || !height || !divisor) {
    return {};
  }
  return {(width + divisor - 1) / divisor, (height + divisor - 1) / divisor};
}

class PostFxResourcePool {
 public:
  struct Image {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    PostFxExtent extent{};
  };

  bool EnsureSceneSnapshot(const ui::vulkan::VulkanDevice* device, VkFormat format,
                           PostFxExtent extent);
  bool RecordSceneSnapshot(VkCommandBuffer command_buffer, const ui::vulkan::VulkanDevice* device,
                           VkImage source, VkFormat format, VkImageLayout source_layout,
                           PostFxExtent extent);
  bool EnsureSplitPostFxImages(const ui::vulkan::VulkanDevice* device, VkFormat format,
                               PostFxExtent extent);
  bool EnsureSunShaftImages(const ui::vulkan::VulkanDevice* device, VkFormat format,
                            PostFxExtent extent);
  void Destroy(const ui::vulkan::VulkanDevice* device);

  Image& scene_snapshot() { return scene_snapshot_; }
  const Image& scene_snapshot() const { return scene_snapshot_; }
  Image& split_full_ping() { return split_full_ping_; }
  Image& split_half_ping() { return split_half_ping_; }
  Image& split_half_pong() { return split_half_pong_; }
  Image& split_full_output() { return split_full_output_; }
  Image& sun_half_prepass() { return sun_half_prepass_; }
  Image& sun_half_ping() { return sun_half_ping_; }
  Image& sun_half_pong() { return sun_half_pong_; }
  Image& sun_full_output() { return sun_full_output_; }

 private:
  static bool EnsureImage(const ui::vulkan::VulkanDevice* device, VkFormat format,
                          PostFxExtent extent, Image& image);
  static void DestroyImage(const ui::vulkan::VulkanDevice* device, Image& image);
  Image scene_snapshot_;
  Image split_full_ping_;
  Image split_half_ping_;
  Image split_half_pong_;
  Image split_full_output_;
  Image sun_half_prepass_;
  Image sun_half_ping_;
  Image sun_half_pong_;
  Image sun_full_output_;
};

}  // namespace rex::graphics::gta4_native
