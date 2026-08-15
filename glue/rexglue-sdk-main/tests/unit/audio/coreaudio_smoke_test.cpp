#include <array>
#include <chrono>
#include <memory>

#include <catch2/catch_test_macros.hpp>

#include <rex/audio/coreaudio/coreaudio_output.h>
#include <rex/thread.h>

TEST_CASE("CoreAudio consumes a native client ring and returns producer credit",
          "[audio][coreaudio]") {
  using namespace rex::audio::coreaudio;

  auto output = std::make_shared<CoreAudioOutput>();
  if (!output->Initialize()) {
    SKIP("No live CoreAudio default output device is available");
  }

  auto semaphore = rex::thread::Semaphore::Create(0, kMaximumCoreAudioGuestBlocks);
  REQUIRE(semaphore);
  CoreAudioClientState client(semaphore.get());
  REQUIRE(output->AttachClient(&client));

  const uint32_t channels = client.channels.load(std::memory_order_acquire);
  const uint32_t credits = output->RecommendedInitialCredits(kMaximumCoreAudioGuestBlocks);
  std::array<float, kGuestAudioFramesPerBlock * kGuestAudioChannels> silence{};
  for (uint32_t block = 0; block < credits; ++block) {
    REQUIRE(client.ring.Write(silence.data(), kGuestAudioFramesPerBlock, channels));
    client.submitted_blocks.fetch_add(1, std::memory_order_release);
  }
  output->NotifyProducerWork();

  CHECK(rex::thread::Wait(semaphore.get(), false, std::chrono::seconds(2)) ==
        rex::thread::WaitResult::kSuccess);
  CHECK(client.retired_blocks_total.load(std::memory_order_acquire) >= 1);
  const CoreAudioOutputMetrics metrics = output->SnapshotMetrics();
  CHECK(metrics.underrun_frames == 0);
  CHECK(metrics.dropped_blocks == 0);
  CHECK(metrics.callback_buffer_errors == 0);
  CHECK(metrics.rebuffer_events == 0);
  CHECK(metrics.credit_release_failures == 0);

  output->DetachClient(&client);
  output->Shutdown();
}
