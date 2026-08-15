#include <array>
#include <atomic>
#include <thread>

#include <catch2/catch_test_macros.hpp>

#include <rex/audio/spsc_frame_ring.h>

TEST_CASE("audio SPSC ring writes reads and wraps without partial publication", "[audio][ring]") {
  rex::audio::SpscFrameRing ring(8, 2);
  REQUIRE(ring.valid());

  const std::array<float, 12> first{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}};
  REQUIRE(ring.Write(first.data(), 6, 2));
  CHECK(ring.available_frames() == 6);

  std::array<float, 8> first_read{};
  CHECK(ring.Read(first_read.data(), 4, 2, false) == 4);
  for (size_t i = 0; i < first_read.size(); ++i) {
    CHECK(first_read[i] == first[i]);
  }

  const std::array<float, 8> wrapped{{12, 13, 14, 15, 16, 17, 18, 19}};
  REQUIRE(ring.Write(wrapped.data(), 4, 2));
  CHECK(ring.available_frames() == 6);

  std::array<float, 12> second_read{};
  CHECK(ring.Read(second_read.data(), 6, 2, false) == 6);
  const std::array<float, 12> expected{{8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}};
  CHECK(second_read == expected);
  CHECK(ring.available_frames() == 0);
}

TEST_CASE("audio SPSC ring rejects whole-frame overflow", "[audio][ring]") {
  rex::audio::SpscFrameRing ring(8, 2);
  std::array<float, 16> full{};
  std::array<float, 2> extra{};
  REQUIRE(ring.Write(full.data(), 8, 2));
  CHECK_FALSE(ring.Write(extra.data(), 1, 2));
  CHECK(ring.available_frames() == 8);
}

TEST_CASE("audio SPSC ring mixes a second client", "[audio][ring]") {
  rex::audio::SpscFrameRing first(8, 2);
  rex::audio::SpscFrameRing second(8, 2);
  const std::array<float, 4> a{{1.0f, 2.0f, 3.0f, 4.0f}};
  const std::array<float, 4> b{{0.5f, 1.0f, 1.5f, 2.0f}};
  REQUIRE(first.Write(a.data(), 2, 2));
  REQUIRE(second.Write(b.data(), 2, 2));
  std::array<float, 4> mixed{};
  REQUIRE(first.Read(mixed.data(), 2, 2, false) == 2);
  REQUIRE(second.Read(mixed.data(), 2, 2, true) == 2);
  CHECK(mixed == std::array<float, 4>{{1.5f, 3.0f, 4.5f, 6.0f}});
}

TEST_CASE("audio SPSC ring publishes ordered frames across concurrent threads", "[audio][ring]") {
  constexpr uint32_t kFrameCount = 4096;
  rex::audio::SpscFrameRing ring(64, 2);
  std::atomic<bool> failed{false};

  std::thread producer([&]() {
    for (uint32_t frame_index = 0; frame_index < kFrameCount; ++frame_index) {
      const std::array<float, 2> frame{
          {static_cast<float>(frame_index), -static_cast<float>(frame_index)}};
      while (!ring.Write(frame.data(), 1, 2)) {
        std::this_thread::yield();
      }
    }
  });
  std::thread consumer([&]() {
    for (uint32_t frame_index = 0; frame_index < kFrameCount; ++frame_index) {
      std::array<float, 2> frame{};
      while (!ring.Read(frame.data(), 1, 2, false)) {
        std::this_thread::yield();
      }
      if (frame[0] != static_cast<float>(frame_index) ||
          frame[1] != -static_cast<float>(frame_index)) {
        failed.store(true, std::memory_order_release);
      }
    }
  });

  producer.join();
  consumer.join();
  CHECK_FALSE(failed.load(std::memory_order_acquire));
  CHECK(ring.available_frames() == 0);
}
