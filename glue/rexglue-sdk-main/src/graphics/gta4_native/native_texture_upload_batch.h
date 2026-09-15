#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <span>
#include <utility>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace rex::graphics::gta4_native {

// Records uploads to distinct, newly initialized images. Image and staging
// allocation ownership remains with the renderer's submission retirement path.
// Consumers must flush before reading any queued image. No image is published
// here, and queued handles must never be destroyed before Flush.
class NativeTextureUploadBatch {
 public:
  static constexpr size_t kCapacity = 128;
  struct Upload {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkImage image = VK_NULL_HANDLE;
    VkImageSubresourceRange range{};
    std::vector<VkBufferImageCopy> copies;
  };

  bool full() const { return uploads_.size() == kCapacity; }
  bool empty() const { return uploads_.empty(); }
  void Add(Upload upload) {
    assert(!full() && upload.image && upload.buffer && !upload.copies.empty());
    uploads_.push_back(std::move(upload));
  }

  template <typename EmitBarriers, typename EmitCopy>
  void Flush(EmitBarriers&& emit_barriers, EmitCopy&& emit_copy) {
    if (empty()) return;
    std::array<VkImageMemoryBarrier, kCapacity> barriers;
    for (size_t i = 0; i < uploads_.size(); ++i) {
      auto& barrier = barriers[i];
      barrier = {};
      barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = uploads_[i].image;
      barrier.subresourceRange = uploads_[i].range;
    }
    const std::span<const VkImageMemoryBarrier> span(barriers.data(), uploads_.size());
    emit_barriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, span);
    for (const auto& upload : uploads_) emit_copy(upload);
    for (size_t i = 0; i < uploads_.size(); ++i) {
      auto& barrier = barriers[i];
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    emit_barriers(VK_PIPELINE_STAGE_TRANSFER_BIT,
                  VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                  span);
    uploads_.clear();
  }

 private:
  std::vector<Upload> uploads_;
};

}  // namespace rex::graphics::gta4_native
