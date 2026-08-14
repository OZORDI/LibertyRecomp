#include <rex/graphics/gta4_native/pipeline_cache_file.h>

#include <cstring>
#include <vector>

#include <catch2/catch_test_macros.hpp>

namespace rex::graphics::gta4_native {
namespace {

NativePipelineCacheIdentity MakeIdentity() {
  NativePipelineCacheIdentity identity{};
  identity.title_id = 0x545407F2;
  identity.vendor_id = 0x106B;
  identity.device_id = 0x1234;
  identity.driver_version = 0x01020304;
  identity.api_version = VK_MAKE_API_VERSION(0, 1, 3, 0);
  for (size_t i = 0; i < identity.uuid.size(); ++i) {
    identity.uuid[i] = static_cast<uint8_t>(i);
  }
  return identity;
}

std::vector<uint8_t> MakeCacheData(const NativePipelineCacheIdentity& identity) {
  NativePipelineCacheHeaderV1 header{};
  header.header_length = sizeof(header);
  header.header_version = VK_PIPELINE_CACHE_HEADER_VERSION_ONE;
  header.vendor_id = identity.vendor_id;
  header.device_id = identity.device_id;
  header.uuid = identity.uuid;
  std::vector<uint8_t> data(sizeof(header));
  std::memcpy(data.data(), &header, sizeof(header));
  return data;
}

TEST_CASE("GTA4 native pipeline cache validates Vulkan identity",
          "[graphics][gta4_native][pipeline_cache]") {
  const NativePipelineCacheIdentity identity = MakeIdentity();
  const std::vector<uint8_t> valid = MakeCacheData(identity);
  REQUIRE(ValidateNativePipelineCacheData(valid, identity));

  NativePipelineCacheIdentity changed = identity;
  changed.vendor_id ^= 1;
  REQUIRE_FALSE(ValidateNativePipelineCacheData(valid, changed));
  changed = identity;
  changed.device_id ^= 1;
  REQUIRE_FALSE(ValidateNativePipelineCacheData(valid, changed));
  changed = identity;
  changed.uuid.front() ^= 1;
  REQUIRE_FALSE(ValidateNativePipelineCacheData(valid, changed));
}

TEST_CASE("GTA4 native pipeline cache rejects truncated and malformed headers",
          "[graphics][gta4_native][pipeline_cache]") {
  const NativePipelineCacheIdentity identity = MakeIdentity();
  std::vector<uint8_t> data = MakeCacheData(identity);
  REQUIRE_FALSE(ValidateNativePipelineCacheData(
      std::span<const uint8_t>(data.data(), data.size() - 1), identity));

  NativePipelineCacheHeaderV1 header{};
  std::memcpy(&header, data.data(), sizeof(header));
  header.header_length = sizeof(header) + 1;
  std::memcpy(data.data(), &header, sizeof(header));
  REQUIRE_FALSE(ValidateNativePipelineCacheData(data, identity));

  data = MakeCacheData(identity);
  std::memcpy(&header, data.data(), sizeof(header));
  header.header_version = 0;
  std::memcpy(data.data(), &header, sizeof(header));
  REQUIRE_FALSE(ValidateNativePipelineCacheData(data, identity));
}

TEST_CASE("GTA4 native pipeline cache filename keys compatibility inputs",
          "[graphics][gta4_native][pipeline_cache]") {
  const NativePipelineCacheIdentity identity = MakeIdentity();
  const auto original = GetNativePipelineCachePath("cache", identity);

  NativePipelineCacheIdentity changed = identity;
  changed.driver_version ^= 1;
  REQUIRE(GetNativePipelineCachePath("cache", changed) != original);
  changed = identity;
  changed.api_version ^= 1;
  REQUIRE(GetNativePipelineCachePath("cache", changed) != original);
  changed = identity;
  changed.renderer_abi ^= 1;
  REQUIRE(GetNativePipelineCachePath("cache", changed) != original);
  changed = identity;
  changed.title_id ^= 1;
  REQUIRE(GetNativePipelineCachePath("cache", changed) != original);
}

}  // namespace
}  // namespace rex::graphics::gta4_native
