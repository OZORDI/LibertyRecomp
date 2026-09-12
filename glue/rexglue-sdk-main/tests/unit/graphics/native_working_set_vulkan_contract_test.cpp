#include <catch2/catch_test_macros.hpp>
#include <rex/ui/vulkan/api.h>
#include <array>
#include <cstdint>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>
#include "graphics/gta4_native/native_working_set.h"
#include "graphics/gta4_native/native_descriptor_backend.h"

// Compile the production Vulkan binding integration in a command-recording
// fixture. The planner and all descriptor publication code are the real source;
// only allocation/provider/driver calls are replaced by inspected test doubles.
namespace descriptor_runtime_contract {
using rex::graphics::gta4_native::NativeDescriptorStatus;
using rex::graphics::gta4_native::NativeDescriptorWorkingSet;
constexpr size_t kShaderTextureCount = 4;
struct NativeFrameContextRing {
  static constexpr uint32_t kSlotCount = 2;
};
template <typename T>
uint64_t NativeVulkanHandleIdentity(T value) {
  if constexpr (std::is_pointer_v<T>)
    return reinterpret_cast<uintptr_t>(value);
  else
    return uint64_t(value);
}
template <typename T>
T Handle(uint64_t n) {
  if constexpr (std::is_pointer_v<T>)
    return reinterpret_cast<T>(uintptr_t(n));
  else
    return T(n);
}
namespace performance {
enum class Counter { kDescriptorEntriesWritten };
}
struct Driver {
  std::map<uint64_t, std::vector<VkDescriptorImageInfo>> descriptors;
  uint64_t calls = 0, writes = 0;
  void vkUpdateDescriptorSets(VkDevice, uint32_t count, const VkWriteDescriptorSet* updates,
                              uint32_t, const VkCopyDescriptorSet*) {
    ++calls;
    writes += count;
    for (uint32_t i = 0; i < count; ++i) {
      const auto& update = updates[i];
      REQUIRE(update.sType == VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
      REQUIRE(update.dstArrayElement == 0);
      REQUIRE(update.descriptorCount > 0);
      auto& copy = descriptors[NativeVulkanHandleIdentity(update.dstSet)];
      // Vulkan updates only the requested range; an unwritten tail retains
      // its previous resource references. Do not let the fake driver erase it.
      copy.resize(std::max(copy.size(), size_t(update.descriptorCount)));
      std::copy_n(update.pImageInfo, update.descriptorCount, copy.begin());
      for (const auto& value : copy) {
        if (update.descriptorType == VK_DESCRIPTOR_TYPE_SAMPLER)
          REQUIRE(value.sampler != VK_NULL_HANDLE);
        else {
          REQUIRE(value.imageView != VK_NULL_HANDLE);
          REQUIRE(value.imageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
      }
    }
  }
};
namespace ui::vulkan {
struct VulkanDevice {
  mutable Driver driver;
  Driver& functions() const { return driver; }
  VkDevice device() const { return Handle<VkDevice>(1); }
};
struct VulkanProvider {
  VulkanDevice gpu;
  const VulkanDevice* vulkan_device() const { return &gpu; }
};
}  // namespace ui::vulkan
struct Gta4NativeGraphicsSystem {
  struct NativeCommand {
    uint32_t descriptor_page = UINT32_MAX, descriptor_copy = UINT32_MAX;
    std::array<uint32_t, kShaderTextureCount> texture_descriptor_indices{},
        sampler_descriptor_indices{};
    uint64_t image_descriptor_epoch = 0, sampler_descriptor_epoch = 0;
    bool bindings_prepared = false;
  };
  struct NativeDescriptorImageSlot {
    uint64_t generation = 0;
    std::array<VkImageView, 4> views{};
    bool operator==(const NativeDescriptorImageSlot&) const = default;
  };
  struct NativeDescriptorSamplerSlot {
    uint64_t generation = 0;
    VkSampler sampler = VK_NULL_HANDLE;
    bool operator==(const NativeDescriptorSamplerSlot&) const = default;
  };
  struct Page {
    std::array<VkDescriptorSet, 5> descriptor_sets{};
    std::vector<NativeDescriptorImageSlot> image_slots;
    std::vector<NativeDescriptorSamplerSlot> sampler_slots;
    uint64_t frame_epoch = 0;
    uint32_t published_image_count = 0, published_sampler_count = 0;
    bool images_dirty = false, samplers_dirty = false;
    std::vector<VkDescriptorImageInfo> update_scratch;
  };
  struct View {
    VkImageView view;
  };
  std::unique_ptr<ui::vulkan::VulkanProvider> provider_ =
      std::make_unique<ui::vulkan::VulkanProvider>();
  std::array<NativeDescriptorWorkingSet<kShaderTextureCount>, 2> native_descriptor_working_sets_;
  uint32_t active_frame_slot_ = 0, active_descriptor_copy_ = 0;
  uint64_t command_buffer_submission_ = 0, completed_command_buffer_submission_ = 0;
  uint32_t native_descriptor_capacity_ = 9, native_sampler_descriptor_capacity_ = 8,
           native_descriptor_maximum_page_count_ = 8;
  uint32_t frame_descriptor_entries_written_ = 0, frame_descriptor_unique_draws_ = 0;
  View null_texture_2d_{Handle<VkImageView>(11)}, null_texture_2d_array_{Handle<VkImageView>(12)},
      null_texture_3d_{Handle<VkImageView>(13)}, null_texture_cube_{Handle<VkImageView>(14)};
  VkSampler null_sampler_ = Handle<VkSampler>(21);
  std::vector<Page> native_descriptor_pages_;
  bool fallback = false;
  bool BeginIndexedWorkingSet();
  template <typename Key>
  bool AssignIndexedWorkingSet(NativeCommand&, const Key&);
  bool PublishIndexedWorkingSet();
  bool ActivateCachedDescriptorFallback(NativeDescriptorStatus, const char*) {
    fallback = true;
    return true;
  }
  bool EnsureIndexedDescriptorPages(uint32_t count) {
    if (count > native_descriptor_maximum_page_count_)
      return false;
    for (; native_descriptor_pages_.size() < count;) {
      Page p;
      const auto id = native_descriptor_pages_.size();
      for (size_t i = 0; i < 5; ++i)
        p.descriptor_sets[i] = Handle<VkDescriptorSet>(100 + id * 5 + i);
      p.image_slots.resize(native_descriptor_capacity_);
      p.sampler_slots.resize(native_sampler_descriptor_capacity_);
      native_descriptor_pages_.push_back(std::move(p));
    }
    return true;
  }
  void AddNativeGpuProfileCounter(performance::Counter, uint64_t) {}
};
#include "graphics/gta4_native/native_descriptor_working_set.inc"
struct Key {
  std::array<uint64_t, kShaderTextureCount> image_lifetimes{};
  std::array<VkImageView, kShaderTextureCount> images_2d{}, images_2d_array{}, images_3d{},
      images_cube{};
  std::array<VkSampler, kShaderTextureCount> samplers{};
};
Key MakeKey(uint64_t draw, uint64_t epoch) {
  Key k;
  for (size_t stage = 0; stage < kShaderTextureCount; ++stage) {
    k.image_lifetimes[stage] = 1000 + epoch * 10000 + draw * 4 + stage;
    // Handles intentionally repeat across epochs. The lifetime identity must
    // still force publication when an allocator reuses an old view address.
    k.images_2d[stage] = Handle<VkImageView>(5000 + draw * 4 + stage);
    k.images_2d_array[stage] = Handle<VkImageView>(12);
    k.images_3d[stage] = Handle<VkImageView>(13);
    k.images_cube[stage] = Handle<VkImageView>(14);
    k.samplers[stage] = Handle<VkSampler>(stage ? 22 : 21);
  }
  return k;
}
}  // namespace descriptor_runtime_contract

TEST_CASE("production indexed binding code publishes complete page-local Vulkan descriptors",
          "[renderer-performance]") {
  using namespace descriptor_runtime_contract;
  Gta4NativeGraphicsSystem renderer;
  for (uint64_t frame = 0; frame < 500; ++frame) {
    renderer.active_frame_slot_ = renderer.active_descriptor_copy_ = uint32_t(frame % 2);
    renderer.completed_command_buffer_submission_ = frame;
    REQUIRE(renderer.BeginIndexedWorkingSet());
    std::vector<Gta4NativeGraphicsSystem::NativeCommand> commands(7);
    std::vector<Key> keys;
    for (uint64_t draw = 0; draw < commands.size(); ++draw) {
      keys.push_back(MakeKey(draw, frame));
      REQUIRE(renderer.AssignIndexedWorkingSet(commands[draw], keys.back()));
    }
    REQUIRE_FALSE(renderer.fallback);
    REQUIRE(renderer.PublishIndexedWorkingSet());
    REQUIRE(renderer.frame_descriptor_unique_draws_ == 4);
    for (size_t i = 0; i < commands.size(); ++i) {
      const auto& command = commands[i];
      const auto& key = keys[i];
      REQUIRE(command.bindings_prepared);
      REQUIRE(command.descriptor_page % 2 == frame % 2);
      const auto& page = renderer.native_descriptor_pages_.at(command.descriptor_page);
      auto& driver = renderer.provider_->gpu.driver;
      for (size_t stage = 0; stage < 4; ++stage) {
        const VkImageView expected[] = {key.images_2d[stage], key.images_2d_array[stage],
                                        key.images_3d[stage], key.images_cube[stage]};
        for (size_t dimension = 0; dimension < 4; ++dimension) {
          const auto& set =
              driver.descriptors.at(NativeVulkanHandleIdentity(page.descriptor_sets[dimension]));
          REQUIRE(set.at(command.texture_descriptor_indices[stage]).imageView ==
                  expected[dimension]);
        }
        const auto& set =
            driver.descriptors.at(NativeVulkanHandleIdentity(page.descriptor_sets[4]));
        REQUIRE(set.at(command.sampler_descriptor_indices[stage]).sampler == key.samplers[stage]);
      }
    }
    REQUIRE(renderer.native_descriptor_working_sets_[frame % 2].MarkSubmitted(frame + 1));
    const auto calls = renderer.provider_->gpu.driver.calls;
    REQUIRE_FALSE(renderer.BeginIndexedWorkingSet());
    REQUIRE(renderer.provider_->gpu.driver.calls == calls);
  }
  REQUIRE(renderer.native_descriptor_pages_.size() == 8);
}
TEST_CASE("production indexed binding reuses descriptor bytes and exposes true budget failure",
          "[renderer-performance]") {
  using namespace descriptor_runtime_contract;
  Gta4NativeGraphicsSystem renderer;
  for (uint64_t frame = 0; frame < 2; ++frame) {
    renderer.completed_command_buffer_submission_ = frame;
    REQUIRE(renderer.BeginIndexedWorkingSet());
    Gta4NativeGraphicsSystem::NativeCommand command;
    auto key = MakeKey(0, 0);
    REQUIRE(renderer.AssignIndexedWorkingSet(command, key));
    const auto before = renderer.provider_->gpu.driver.calls;
    REQUIRE(renderer.PublishIndexedWorkingSet());
    if (frame)
      REQUIRE(renderer.provider_->gpu.driver.calls == before);
    REQUIRE(renderer.native_descriptor_working_sets_[0].MarkSubmitted(frame + 1));
  }
  renderer.command_buffer_submission_ = 99;
  REQUIRE_FALSE(renderer.BeginIndexedWorkingSet());
  renderer.command_buffer_submission_ = 0;
  renderer.completed_command_buffer_submission_ = 100;
  renderer.native_descriptor_maximum_page_count_ = 2;
  REQUIRE(renderer.BeginIndexedWorkingSet());
  for (unsigned i = 0; i < 2; ++i) {
    Gta4NativeGraphicsSystem::NativeCommand c;
    REQUIRE(renderer.AssignIndexedWorkingSet(c, MakeKey(i, 1)));
    REQUIRE(c.bindings_prepared);
  }
  Gta4NativeGraphicsSystem::NativeCommand overflow;
  REQUIRE(renderer.AssignIndexedWorkingSet(overflow, MakeKey(2, 1)));
  REQUIRE(renderer.fallback);
  REQUIRE_FALSE(overflow.bindings_prepared);
}

TEST_CASE("completed indexed pages clear stale tails and unused pages without touching the other slot",
          "[renderer-performance][native-memory-fix]") {
  using namespace descriptor_runtime_contract;
  Gta4NativeGraphicsSystem renderer;
  auto& driver = renderer.provider_->gpu.driver;
  for (uint32_t slot = 0; slot < 2; ++slot) {
    renderer.active_frame_slot_ = renderer.active_descriptor_copy_ = slot;
    REQUIRE(renderer.BeginIndexedWorkingSet());
    for (uint64_t draw = 0; draw < 7; ++draw) {
      Gta4NativeGraphicsSystem::NativeCommand command;
      REQUIRE(renderer.AssignIndexedWorkingSet(command, MakeKey(draw, slot)));
    }
    REQUIRE(renderer.PublishIndexedWorkingSet());
    REQUIRE(renderer.native_descriptor_working_sets_[slot].MarkSubmitted(slot + 1));
  }
  auto other_slot = driver.descriptors;
  renderer.active_frame_slot_ = renderer.active_descriptor_copy_ = 0;
  const auto calls = driver.calls;
  REQUIRE_FALSE(renderer.BeginIndexedWorkingSet());
  REQUIRE(driver.calls == calls);
  renderer.completed_command_buffer_submission_ = 1;
  REQUIRE(renderer.BeginIndexedWorkingSet());
  Gta4NativeGraphicsSystem::NativeCommand command;
  const auto key = MakeKey(0, 9);
  REQUIRE(renderer.AssignIndexedWorkingSet(command, key));
  REQUIRE(renderer.PublishIndexedWorkingSet());
  for (size_t page_index = 0; page_index < renderer.native_descriptor_pages_.size(); ++page_index) {
    const auto& page = renderer.native_descriptor_pages_[page_index];
    for (size_t dimension = 0; dimension < 5; ++dimension) {
      const auto set_id = NativeVulkanHandleIdentity(page.descriptor_sets[dimension]);
      if (!driver.descriptors.contains(set_id)) continue;
      const auto& values = driver.descriptors.at(set_id);
      if (page_index % 2) {
        const auto& original = other_slot.at(set_id);
        REQUIRE(values.size() == original.size());
        for (size_t i = 0; i < values.size(); ++i) {
          REQUIRE(values[i].imageView == original[i].imageView);
          REQUIRE(values[i].sampler == original[i].sampler);
        }
        continue;
      }
      const size_t first_unused = page_index == 0
          ? (dimension < 4 ? size_t(5) : size_t(2)) : 0;
      for (size_t i = first_unused; i < values.size(); ++i) {
        if (dimension == 4) REQUIRE(values[i].sampler == renderer.null_sampler_);
        else REQUIRE(values[i].imageView == Handle<VkImageView>(11 + dimension));
      }
    }
  }
  // A frame with no indexed draws releases even the last page's references.
  REQUIRE(renderer.BeginIndexedWorkingSet());
  REQUIRE(renderer.PublishIndexedWorkingSet());
  for (size_t page_index = 0; page_index < renderer.native_descriptor_pages_.size(); page_index += 2) {
    const auto& page = renderer.native_descriptor_pages_[page_index];
    for (size_t dimension = 0; dimension < 5; ++dimension) {
      const auto& values = driver.descriptors.at(NativeVulkanHandleIdentity(page.descriptor_sets[dimension]));
      for (const auto& value : values) {
        if (dimension == 4) REQUIRE(value.sampler == renderer.null_sampler_);
        else REQUIRE(value.imageView == Handle<VkImageView>(11 + dimension));
      }
    }
  }
  const auto stable_calls = driver.calls;
  REQUIRE(renderer.BeginIndexedWorkingSet());
  REQUIRE(renderer.PublishIndexedWorkingSet());
  REQUIRE(driver.calls == stable_calls);
}
