/**
 ******************************************************************************
 * ReXGlue Runtime                                                            *
 ******************************************************************************
 * Copyright 2026. Released under the BSD license.                            *
 ******************************************************************************
 *
 * Switch audio driver using libnx audren (audio renderer) API.
 * Uses the high-level AudioDriver wrapper from libnx for voice-based PCM
 * output with double-buffered wave buffers for gapless playback.
 */

#pragma once

#if REX_PLATFORM_NX

#include <mutex>
#include <queue>
#include <stack>

#include <rex/audio/audio_driver.h>
#include <rex/thread.h>

extern "C" {
#include <switch/audio/driver.h>
}

namespace rex::audio::nx {

class SwitchAudioDriver : public AudioDriver {
 public:
  SwitchAudioDriver(memory::Memory* memory, rex::thread::Semaphore* semaphore);
  ~SwitchAudioDriver() override;

  bool Initialize();
  void SubmitFrame(uint32_t frame_ptr) override;
  void Shutdown();

 protected:
  rex::thread::Semaphore* semaphore_ = nullptr;

  // libnx audio renderer state
  ::AudioDriver audren_driver_ = {};
  int mempool_id_ = -1;
  bool audren_initialized_ = false;
  bool voice_initialized_ = false;

  // Audio format constants matching Xbox 360 XMA output:
  // 6-channel (5.1) 48kHz float, 256 samples per channel per frame.
  // Switch audren expects PCM16, so we convert float BE -> int16 LE stereo.
  static const uint32_t kFrameFrequency = 48000;
  static const uint32_t kFrameChannels = 6;
  static const uint32_t kChannelSamples = 256;
  static const uint32_t kFrameSamples = kFrameChannels * kChannelSamples;
  static const uint32_t kFrameSize = sizeof(float) * kFrameSamples;

  // Output is stereo PCM16 for the Switch speaker/headphone sink.
  static const uint32_t kOutputChannels = 2;
  static const uint32_t kOutputSampleBytes = sizeof(int16_t);
  static const uint32_t kOutputFrameSize =
      kChannelSamples * kOutputChannels * kOutputSampleBytes;

  // Double-buffer wave buffers for gapless playback.
  static const int kWaveBufCount = 2;
  AudioDriverWaveBuf wavebufs_[kWaveBufCount] = {};
  int16_t* pcm_buffers_[kWaveBufCount] = {};
  int current_buf_ = 0;

  // Aligned memory pool registered with audren.
  void* mempool_ptr_ = nullptr;
  size_t mempool_size_ = 0;

  // Queue of frames received from the guest, awaiting consumption.
  std::queue<float*> frames_queued_ = {};
  std::stack<float*> frames_unused_ = {};
  std::mutex frames_mutex_ = {};

  void ConvertAndSubmit(const float* input);
};

}  // namespace rex::audio::nx

#endif  // REX_PLATFORM_NX
