#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

#include "graphics/gta4_native/frame_constant_arena.h"
#include "graphics/gta4_native/native_constant_upload.h"

using namespace rex::graphics::gta4_native;

namespace {
std::vector<uint8_t> HostBytes(std::span<const uint8_t> source, bool guest) {
  std::vector<uint8_t> expected(source.begin(), source.end());
  if (guest) {
    for (size_t offset = 0; offset < expected.size(); offset += 4) {
      std::reverse(expected.begin() + offset, expected.begin() + offset + 4);
    }
  }
  return expected;
}
}

TEST_CASE("Fresh constant storage is initialized even when unknown bytes happen to match",
          "[native-upload][constants]") {
  NativeConstantUploadTracker uploads;
  std::vector<uint8_t> storage(4096, 0);
  std::vector<uint8_t> source(4096, 0);
  REQUIRE(uploads.Write(storage, 0, source, true) == 4096);
  REQUIRE(uploads.initialized_bytes() == 4096);
  REQUIRE(uploads.written_bytes() == 4096);

  uploads.BeginFrame();
  REQUIRE(uploads.Write(storage, 0, source, true) == 0);
  REQUIRE(uploads.written_bytes() == 0);
  source[23] = 0x80;
  REQUIRE(uploads.Write(storage, 0, source, true) == 16);
  REQUIRE(uploads.written_bytes() == 16);
  REQUIRE(storage == HostBytes(source, true));

  // Replacing a Vulkan allocation invalidates the prior initialization state,
  // even when the replacement happens to contain identical bytes.
  uploads.ResetStorage();
  REQUIRE(uploads.Write(storage, 0, source, true) == 4096);
}

TEST_CASE("Constant refresh preserves NaN payloads signed zero and exact shader words",
          "[native-upload][constants]") {
  NativeConstantUploadTracker uploads;
  std::array<uint8_t, 32> storage{};
  std::array<uint8_t, 32> source{
      0x7F, 0xC0, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00,
      0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x80, 0x00, 0x00,
      0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0xFF, 0xC0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};
  REQUIRE(uploads.Write(storage, 0, source, true) == source.size());
  REQUIRE(std::ranges::equal(storage, HostBytes(source, true)));
  uploads.BeginFrame();
  source[3] = 2;
  source[4] = 0;
  REQUIRE(uploads.Write(storage, 0, source, true) == 16);
  REQUIRE(std::ranges::equal(storage, HostBytes(source, true)));
  REQUIRE(uploads.Write(storage, 0, source, true) == 0);
}

TEST_CASE("Uncached constant storage uses sequential writes without reuse reads",
          "[native-upload][constants]") {
  NativeConstantUploadTracker uploads;
  std::array<uint8_t, 64> storage{}, source{};
  REQUIRE(uploads.Write(storage, 0, source, true, false) == 64);
  uploads.BeginFrame();
  REQUIRE(uploads.Write(storage, 0, source, true, false) == 64);
  REQUIRE(uploads.written_bytes() == 64);
}

TEST_CASE("Invalid constant upload bounds cannot mutate storage or counters",
          "[native-upload][constants]") {
  NativeConstantUploadTracker uploads;
  std::array<uint8_t, 64> storage{}, source{};
  const auto initial = storage;
  CHECK_FALSE(uploads.Write(storage, 16, std::span(source).first(16), false));
  CHECK_FALSE(uploads.Write(storage, 1, std::span(source).first(16), false));
  CHECK_FALSE(uploads.Write(storage, 0, std::span(source).first(15), false));
  CHECK_FALSE(uploads.Write(std::span(storage).first(16), 0, source, false));
  CHECK_FALSE(uploads.Write(storage, std::numeric_limits<size_t>::max(), source, false));
  CHECK_FALSE(uploads.Write(storage, 0, {}, false));
  CHECK(storage == initial);
  CHECK(uploads.initialized_bytes() == 0);
  CHECK(uploads.written_bytes() == 0);
}

TEST_CASE("Partial constant uploads retain every draw snapshot across frame-slot reuse",
          "[native-upload][constants]") {
  struct Slot {
    FrameConstantArenaIndex index;
    NativeConstantUploadTracker uploads;
    std::vector<uint8_t> storage = std::vector<uint8_t>(131072, 0xCC);
  };
  struct Draw {
    FrameConstantReservation allocation;
    std::vector<uint8_t> expected;
  };
  std::array<Slot, 2> slots;
  for (auto& slot : slots) REQUIRE(slot.index.SetByteCapacity(slot.storage.size()));
  constexpr std::array<size_t, 3> sizes{4096, 3584, 1056};
  for (uint64_t frame = 1; frame <= 100; ++frame) {
    auto& slot = slots[frame % slots.size()];
    const auto other_before = slots[(frame + 1) % slots.size()].storage;
    REQUIRE(slot.index.ResetAfterCompletion(frame));
    slot.uploads.BeginFrame();
    std::vector<Draw> draws;
    for (uint64_t draw = 0; draw < 24; ++draw) {
      const size_t kind = (frame + draw) % sizes.size();
      std::vector<uint8_t> source(sizes[kind]);
      for (size_t i = 0; i < source.size(); ++i) source[i] = uint8_t(draw + i);
      source[(frame * 16) % source.size()] ^= uint8_t(frame);
      const FrameConstantIdentity identity{FrameConstantKind(kind), draw + 1};
      const auto allocation = slot.index.FindOrReserve(identity, source.size(), 16);
      REQUIRE(allocation);
      REQUIRE_FALSE(allocation->reused);
      REQUIRE(slot.uploads.Write(slot.storage, allocation->offset, source, kind != 2));
      draws.push_back({*allocation, HostBytes(source, kind != 2)});
      // The producer may mutate its own state immediately after capture.
      std::fill(source.begin(), source.end(), 0xEE);
      for (const auto& prior : draws) {
        const auto actual = std::span(slot.storage).subspan(prior.allocation.offset,
                                                            prior.allocation.byte_size);
        REQUIRE(std::ranges::equal(actual, prior.expected));
      }
      REQUIRE(slot.index.FindOrReserve(identity, sizes[kind], 16)->reused);
    }
    REQUIRE(slots[(frame + 1) % slots.size()].storage == other_before);
    REQUIRE(slot.index.MarkSubmitted(frame + 1));
    const auto in_flight_bytes = slot.storage;
    REQUIRE_FALSE(slot.index.ResetAfterCompletion(frame));
    REQUIRE_FALSE(slot.index.ResetUnsubmitted());
    REQUIRE(slot.storage == in_flight_bytes);
  }
}

TEST_CASE("Constant storage can change layout and extend its initialized region safely",
          "[native-upload][constants]") {
  NativeConstantUploadTracker uploads;
  std::vector<uint8_t> storage(4096, 0x55);
  std::vector<uint8_t> shared(1056, 0xA5);
  REQUIRE(uploads.Write(storage, 0, shared, false) == shared.size());
  std::vector<uint8_t> vertex(4096, 0);
  uploads.BeginFrame();
  REQUIRE(uploads.Write(storage, 0, vertex, true) == vertex.size());
  REQUIRE(storage == vertex);
  uploads.BeginFrame();
  REQUIRE(uploads.Write(storage, 0, shared, false) == shared.size());
  auto tail = std::span(vertex).subspan(shared.size());
  REQUIRE(uploads.Write(storage, shared.size(), tail, true) == 0);
  REQUIRE(std::equal(shared.begin(), shared.end(), storage.begin()));
}
