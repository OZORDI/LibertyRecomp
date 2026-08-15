/**
 ******************************************************************************
 * ReXGlue : native macOS CoreAudio logical client                            *
 ******************************************************************************
 */

#pragma once

#include <array>
#include <chrono>
#include <memory>

#include <rex/audio/audio_driver.h>
#include <rex/audio/coreaudio/coreaudio_output.h>
#include <rex/audio/rational_frame_clock.h>

namespace rex::audio::coreaudio {

class CoreAudioAudioDriver final : public AudioDriver {
 public:
  CoreAudioAudioDriver(memory::Memory* memory, rex::thread::Semaphore* semaphore,
                       std::shared_ptr<CoreAudioOutput> output);
  ~CoreAudioAudioDriver() override;

  bool Initialize();
  void Shutdown();
  void SubmitFrame(uint32_t frame_ptr) override;
  uint32_t RecommendedInitialCredits(uint32_t configured_maximum) const override;
  void Pause() override;
  void Resume() override;
  void Flush() override;

 private:
  void SubmitSilent();

  rex::thread::Semaphore* semaphore_ = nullptr;
  std::shared_ptr<CoreAudioOutput> output_;
  std::unique_ptr<CoreAudioClientState> state_;
  std::array<float, kGuestAudioFramesPerBlock * kGuestAudioChannels> converted_frame_{};
  RationalFrameClock silent_clock_{kGuestAudioSampleRate, kGuestAudioFramesPerBlock};
  std::chrono::steady_clock::time_point silent_deadline_{};
  bool attached_ = false;
};

}  // namespace rex::audio::coreaudio
