/**
 ******************************************************************************
 * ReXGlue Runtime                                                            *
 ******************************************************************************
 * Copyright 2026. Released under the BSD license.                            *
 ******************************************************************************
 *
 * Switch audio driver using libnx audio renderer (audren).
 *
 * Architecture:
 *   - Uses the libnx AudioDriver high-level wrapper around audren.
 *   - A single voice outputs stereo PCM16 at 48kHz.
 *   - Double-buffered AudioDriverWaveBuf for gapless playback:
 *     while one buffer plays, the next is filled and queued.
 *   - SubmitFrame receives 6-channel big-endian float from the guest,
 *     down-mixes to stereo, byte-swaps, and converts to int16.
 */

#if REX_PLATFORM_NX

#include <algorithm>
#include <cmath>
#include <cstring>

#include <rex/assert.h>
#include <rex/audio/conversion.h>
#include <rex/audio/flags.h>
#include <rex/audio/switch/switch_audio_driver.h>
#include <rex/logging.h>

extern "C" {
#include <switch/audio/driver.h>
#include <switch/kernel/event.h>
#include <switch/services/audren.h>
}

// Audren memory pool buffers must be aligned to 0x1000.
static constexpr size_t kMempoolAlign = AUDREN_MEMPOOL_ALIGNMENT;
// Wave buffer data must be aligned to 0x40.
static constexpr size_t kBufferAlign = AUDREN_BUFFER_ALIGNMENT;

namespace rex::audio::nx {

SwitchAudioDriver::SwitchAudioDriver(memory::Memory* memory,
                                     rex::thread::Semaphore* semaphore)
    : AudioDriver(memory), semaphore_(semaphore) {}

SwitchAudioDriver::~SwitchAudioDriver() {
  assert_true(frames_queued_.empty());
  assert_true(frames_unused_.empty());
}

bool SwitchAudioDriver::Initialize() {
  // Configure audio renderer: 1 voice, 1 sink, 1 mix (final mix), 2 mix buffers (stereo).
  AudioRendererConfig ar_config = {};
  ar_config.output_rate = AudioRendererOutputRate_48kHz;
  ar_config.num_voices = 1;
  ar_config.num_effects = 0;
  ar_config.num_sinks = 1;
  ar_config.num_mix_objs = 1;
  ar_config.num_mix_buffers = kOutputChannels;

  // Create the high-level audio driver.
  Result rc = audrvCreate(&audren_driver_, &ar_config, static_cast<int>(kOutputChannels));
  if (R_FAILED(rc)) {
    REXAPU_ERROR("audrvCreate failed: 0x{:08X}", rc);
    return false;
  }
  audren_initialized_ = true;

  // Compute aligned memory pool size for double-buffered PCM16 output.
  // Each buffer: kChannelSamples * kOutputChannels * sizeof(int16_t) bytes,
  // aligned up to kBufferAlign.
  size_t per_buf_size = kOutputFrameSize;
  per_buf_size = (per_buf_size + kBufferAlign - 1) & ~(kBufferAlign - 1);

  // Total memory pool must be aligned to kMempoolAlign.
  mempool_size_ = kWaveBufCount * per_buf_size;
  mempool_size_ = (mempool_size_ + kMempoolAlign - 1) & ~(kMempoolAlign - 1);

  // Allocate page-aligned memory for the audio pool.
  mempool_ptr_ = aligned_alloc(kMempoolAlign, mempool_size_);
  if (!mempool_ptr_) {
    REXAPU_ERROR("Failed to allocate audren memory pool ({} bytes)", mempool_size_);
    return false;
  }
  std::memset(mempool_ptr_, 0, mempool_size_);

  // Register the memory pool with audren.
  mempool_id_ = audrvMemPoolAdd(&audren_driver_, mempool_ptr_, mempool_size_);
  if (mempool_id_ < 0) {
    REXAPU_ERROR("audrvMemPoolAdd failed");
    return false;
  }
  audrvMemPoolAttach(&audren_driver_, mempool_id_);

  // Set up pointers into the memory pool for each wave buffer.
  auto* pool_bytes = static_cast<uint8_t*>(mempool_ptr_);
  for (int i = 0; i < kWaveBufCount; i++) {
    pcm_buffers_[i] = reinterpret_cast<int16_t*>(pool_bytes + i * per_buf_size);
    std::memset(&wavebufs_[i], 0, sizeof(AudioDriverWaveBuf));
    wavebufs_[i].data_pcm16 = pcm_buffers_[i];
    wavebufs_[i].size = per_buf_size;
    wavebufs_[i].start_sample_offset = 0;
    wavebufs_[i].end_sample_offset = static_cast<int32_t>(kChannelSamples);
    wavebufs_[i].state = AudioDriverWaveBufState_Done;
  }

  // Initialize a single stereo voice at 48kHz PCM16.
  if (!audrvVoiceInit(&audren_driver_, 0, static_cast<int>(kOutputChannels),
                      PcmFormat_Int16, kFrameFrequency)) {
    REXAPU_ERROR("audrvVoiceInit failed");
    return false;
  }
  voice_initialized_ = true;

  audrvVoiceSetDestinationMix(&audren_driver_, 0, AUDREN_FINAL_MIX_ID);
  audrvVoiceSetMixFactor(&audren_driver_, 0, 1.0f, 0, 0);  // left  -> left
  audrvVoiceSetMixFactor(&audren_driver_, 0, 1.0f, 1, 1);  // right -> right
  audrvVoiceSetVolume(&audren_driver_, 0, 1.0f);

  // Add a device sink for the default audio output.
  const u8 sink_channels[] = {0, 1};
  int sink_id = audrvDeviceSinkAdd(&audren_driver_, AUDREN_DEFAULT_DEVICE_NAME,
                                   static_cast<int>(kOutputChannels), sink_channels);
  if (sink_id < 0) {
    REXAPU_ERROR("audrvDeviceSinkAdd failed");
    return false;
  }

  // Push initial state to the renderer and start it.
  rc = audrvUpdate(&audren_driver_);
  if (R_FAILED(rc)) {
    REXAPU_ERROR("Initial audrvUpdate failed: 0x{:08X}", rc);
    return false;
  }

  rc = audrenStartAudioRenderer();
  if (R_FAILED(rc)) {
    REXAPU_ERROR("audrenStartAudioRenderer failed: 0x{:08X}", rc);
    return false;
  }

  // Start the voice.
  audrvVoiceStart(&audren_driver_, 0);

  return true;
}

void SwitchAudioDriver::SubmitFrame(uint32_t frame_ptr) {
  const auto input_frame = memory_->TranslateVirtual<float*>(frame_ptr);
  float* output_frame;
  {
    std::unique_lock<std::mutex> guard(frames_mutex_);
    if (frames_unused_.empty()) {
      output_frame = new float[kFrameSamples];
    } else {
      output_frame = frames_unused_.top();
      frames_unused_.pop();
    }
  }

  std::memcpy(output_frame, input_frame, kFrameSamples * sizeof(float));

  // Find a free wave buffer slot and submit immediately.
  // If both buffers are still in flight, queue the frame for later.
  {
    std::unique_lock<std::mutex> guard(frames_mutex_);

    // Check if current buffer slot is done playing.
    if (wavebufs_[current_buf_].state == AudioDriverWaveBufState_Done ||
        wavebufs_[current_buf_].state == AudioDriverWaveBufState_Free) {
      // Convert directly into the wave buffer and submit.
      guard.unlock();
      ConvertAndSubmit(output_frame);
      guard.lock();
      frames_unused_.push(output_frame);

      if (semaphore_) {
        auto ret = semaphore_->Release(1, nullptr);
        assert_true(ret);
      }
    } else {
      // Both buffers busy -- queue the frame to be consumed on next opportunity.
      frames_queued_.push(output_frame);
    }
  }

  // Drain any queued frames into free buffer slots.
  {
    std::unique_lock<std::mutex> guard(frames_mutex_);
    while (!frames_queued_.empty()) {
      // Poll audren to update buffer states.
      audrvUpdate(&audren_driver_);

      if (wavebufs_[current_buf_].state != AudioDriverWaveBufState_Done &&
          wavebufs_[current_buf_].state != AudioDriverWaveBufState_Free) {
        break;  // No free slot yet.
      }

      float* queued = frames_queued_.front();
      frames_queued_.pop();
      guard.unlock();
      ConvertAndSubmit(queued);
      guard.lock();
      frames_unused_.push(queued);

      if (semaphore_) {
        auto ret = semaphore_->Release(1, nullptr);
        assert_true(ret);
      }
    }
  }
}

void SwitchAudioDriver::ConvertAndSubmit(const float* input) {
  // Temporary buffer for stereo float conversion.
  float stereo_float[kChannelSamples * kOutputChannels];
  conversion::sequential_6_BE_to_interleaved_2_LE(stereo_float, input, kChannelSamples);

  // Convert float [-1.0, 1.0] to int16 and write into the current PCM buffer.
  int16_t* dest = pcm_buffers_[current_buf_];
  const size_t total_samples = kChannelSamples * kOutputChannels;
  for (size_t i = 0; i < total_samples; i++) {
    float sample = stereo_float[i];
    // Clamp to [-1.0, 1.0] then scale to int16 range.
    sample = std::fmax(-1.0f, std::fmin(1.0f, sample));
    dest[i] = static_cast<int16_t>(sample * 32767.0f);
  }

  // Prepare and submit the wave buffer.
  wavebufs_[current_buf_].state = AudioDriverWaveBufState_Free;
  wavebufs_[current_buf_].start_sample_offset = 0;
  wavebufs_[current_buf_].end_sample_offset = static_cast<int32_t>(kChannelSamples);
  audrvVoiceAddWaveBuf(&audren_driver_, 0, &wavebufs_[current_buf_]);
  audrvUpdate(&audren_driver_);

  // Flip to the other buffer for next submission.
  current_buf_ = (current_buf_ + 1) % kWaveBufCount;
}

void SwitchAudioDriver::Shutdown() {
  if (voice_initialized_) {
    audrvVoiceStop(&audren_driver_, 0);
    audrvVoiceDrop(&audren_driver_, 0);
    voice_initialized_ = false;
  }

  if (audren_initialized_) {
    audrenStopAudioRenderer();
    if (mempool_id_ >= 0) {
      audrvMemPoolDetach(&audren_driver_, mempool_id_);
      audrvMemPoolRemove(&audren_driver_, mempool_id_);
      mempool_id_ = -1;
    }
    audrvClose(&audren_driver_);
    audren_initialized_ = false;
  }

  if (mempool_ptr_) {
    free(mempool_ptr_);
    mempool_ptr_ = nullptr;
  }

  std::unique_lock<std::mutex> guard(frames_mutex_);
  while (!frames_unused_.empty()) {
    delete[] frames_unused_.top();
    frames_unused_.pop();
  }
  while (!frames_queued_.empty()) {
    delete[] frames_queued_.front();
    frames_queued_.pop();
  }
}

}  // namespace rex::audio::nx

#endif  // REX_PLATFORM_NX
