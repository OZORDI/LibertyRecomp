/**
 ******************************************************************************
 * ReXGlue : PS4 Audio Backend (sceAudioOut)                                  *
 ******************************************************************************
 */

#pragma once

#if REX_PLATFORM_PS4

#include <mutex>
#include <queue>
#include <stack>

#include <rex/audio/audio_driver.h>
#include <rex/thread.h>

namespace rex::audio::orbis {

class OrbisAudioDriver : public AudioDriver {
 public:
  OrbisAudioDriver(memory::Memory* memory, rex::thread::Semaphore* semaphore);
  ~OrbisAudioDriver() override;

  bool Initialize();
  void SubmitFrame(uint32_t frame_ptr) override;
  void Shutdown();

 private:
  rex::thread::Semaphore* semaphore_ = nullptr;

  int32_t audio_handle_ = -1;

  // Xbox 360 audio: 6 channels (5.1), 256 samples per channel, big-endian float.
  // PS4 output: stereo float (downmixed from 5.1).
  static const uint32_t kFrequency = 48000;
  static const uint32_t kInputChannels = 6;
  static const uint32_t kOutputChannels = 2;
  static const uint32_t kSamplesPerChannel = 256;
  static const uint32_t kInputFrameSamples = kInputChannels * kSamplesPerChannel;
  static const uint32_t kOutputFrameSamples = kOutputChannels * kSamplesPerChannel;

  // Double-buffer: one frame being submitted to sceAudioOut, one being filled.
  float output_buffer_[kOutputFrameSamples];

  std::queue<float*> frames_queued_;
  std::stack<float*> frames_unused_;
  std::mutex frames_mutex_;
};

}  // namespace rex::audio::orbis

#endif  // REX_PLATFORM_PS4
