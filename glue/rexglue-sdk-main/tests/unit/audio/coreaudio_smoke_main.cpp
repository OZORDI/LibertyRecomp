#include <array>
#include <chrono>
#include <cstdio>
#include <memory>

#include <rex/audio/coreaudio/coreaudio_output.h>
#include <rex/cvar.h>
#include <rex/thread.h>

REXCVAR_DEFINE_BOOL(audio_mute, false, "Audio", "Mute audio output");
REXCVAR_DEFINE_INT32(audio_maxqframes, 64, "Audio", "Maximum buffered guest audio blocks");

int main() {
  using namespace rex::audio::coreaudio;

  auto output = std::make_shared<CoreAudioOutput>();
  if (!output->Initialize()) {
    std::fprintf(stderr, "CoreAudio smoke: no live default output device\n");
    return 2;
  }

  auto semaphore = rex::thread::Semaphore::Create(0, kMaximumCoreAudioGuestBlocks);
  if (!semaphore) {
    std::fprintf(stderr, "CoreAudio smoke: semaphore creation failed\n");
    return 3;
  }
  CoreAudioClientState client(semaphore.get());
  if (!output->AttachClient(&client)) {
    std::fprintf(stderr, "CoreAudio smoke: client attach failed\n");
    return 4;
  }

  const uint32_t channels = client.channels.load(std::memory_order_acquire);
  const uint32_t credits = output->RecommendedInitialCredits(kMaximumCoreAudioGuestBlocks);
  std::array<float, kGuestAudioFramesPerBlock * kGuestAudioChannels> silence{};
  for (uint32_t block = 0; block < credits; ++block) {
    if (!client.ring.Write(silence.data(), kGuestAudioFramesPerBlock, channels)) {
      std::fprintf(stderr, "CoreAudio smoke: preroll write failed\n");
      return 5;
    }
    client.submitted_blocks.fetch_add(1, std::memory_order_release);
  }
  output->NotifyProducerWork();

  const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
  rex::thread::WaitResult result = rex::thread::WaitResult::kSuccess;
  while (std::chrono::steady_clock::now() < deadline) {
    result = rex::thread::Wait(semaphore.get(), false, std::chrono::milliseconds(100));
    if (result != rex::thread::WaitResult::kSuccess) {
      break;
    }
    if (!client.ring.Write(silence.data(), kGuestAudioFramesPerBlock, channels)) {
      std::fprintf(stderr, "CoreAudio smoke: steady-state write failed\n");
      return 6;
    }
    client.submitted_blocks.fetch_add(1, std::memory_order_release);
    output->NotifyProducerWork();
  }
  const CoreAudioOutputMetrics metrics = output->SnapshotMetrics();
  const uint64_t retired = client.retired_blocks_total.load(std::memory_order_acquire);
  std::fprintf(stderr,
               "CoreAudio smoke: channels=%u credits=%u retired=%llu wait_result=%u callbacks=%llu "
               "underrun=%llu dropped=%llu buffer_errors=%llu device_overloads=%llu "
               "rebuffer_events=%llu credit_failures=%llu\n",
               channels, credits, static_cast<unsigned long long>(retired),
               static_cast<unsigned int>(result),
               static_cast<unsigned long long>(metrics.callback_count),
               static_cast<unsigned long long>(metrics.underrun_frames),
               static_cast<unsigned long long>(metrics.dropped_blocks),
               static_cast<unsigned long long>(metrics.callback_buffer_errors),
               static_cast<unsigned long long>(metrics.device_overloads),
               static_cast<unsigned long long>(metrics.rebuffer_events),
               static_cast<unsigned long long>(metrics.credit_release_failures));

  output->DetachClient(&client);
  output->Shutdown();
  const bool clean = result == rex::thread::WaitResult::kSuccess && retired != 0 &&
                     metrics.underrun_frames == 0 && metrics.dropped_blocks == 0 &&
                     metrics.callback_buffer_errors == 0 && metrics.device_overloads == 0 &&
                     metrics.rebuffer_events == 0 && metrics.credit_release_failures == 0;
  return clean ? 0 : 7;
}
