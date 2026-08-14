#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <span>
#include <string>

#include <fmt/format.h>
#include <vulkan/vulkan.h>

namespace rex::graphics::gta4_native {

// Increment whenever native shader translation, fixed-function decoding, or
// pipeline layout compatibility changes. This is deliberately independent of
// the guest title-command ABI.
inline constexpr uint32_t kNativePipelineCacheAbi = 1;

struct NativePipelineCacheIdentity {
  uint32_t title_id = 0;
  uint32_t vendor_id = 0;
  uint32_t device_id = 0;
  uint32_t driver_version = 0;
  uint32_t api_version = 0;
  std::array<uint8_t, VK_UUID_SIZE> uuid{};
  uint32_t renderer_abi = kNativePipelineCacheAbi;
};

struct NativePipelineCacheHeaderV1 {
  uint32_t header_length = 0;
  uint32_t header_version = 0;
  uint32_t vendor_id = 0;
  uint32_t device_id = 0;
  std::array<uint8_t, VK_UUID_SIZE> uuid{};
};
static_assert(sizeof(NativePipelineCacheHeaderV1) == 32);

inline bool ValidateNativePipelineCacheData(
    std::span<const uint8_t> data, const NativePipelineCacheIdentity& identity) {
  if (data.size() < sizeof(NativePipelineCacheHeaderV1)) {
    return false;
  }
  NativePipelineCacheHeaderV1 header{};
  std::memcpy(&header, data.data(), sizeof(header));
  return header.header_length >= sizeof(header) && header.header_length <= data.size() &&
         header.header_version == VK_PIPELINE_CACHE_HEADER_VERSION_ONE &&
         header.vendor_id == identity.vendor_id && header.device_id == identity.device_id &&
         header.uuid == identity.uuid;
}

inline std::filesystem::path GetNativePipelineCachePath(
    const std::filesystem::path& cache_root, const NativePipelineCacheIdentity& identity) {
  std::string uuid;
  uuid.reserve(identity.uuid.size() * 2);
  for (uint8_t byte : identity.uuid) {
    uuid += fmt::format("{:02x}", byte);
  }
  return cache_root / "gta4-native" / fmt::format("{:08x}", identity.title_id) /
         fmt::format("pipelines-v{}-api{:08x}-{:04x}-{:04x}-drv{:08x}-{}.bin",
                     identity.renderer_abi, identity.api_version, identity.vendor_id,
                     identity.device_id, identity.driver_version, uuid);
}

}  // namespace rex::graphics::gta4_native
