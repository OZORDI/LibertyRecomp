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
#include <switch/kernel/svc.h>
#include <switch/kernel/thread.h>
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
  // Configure audio renderer. Follows the libnx `audren_user` sample: minimal
  // voice/mix/sink counts. 1 voice (stereo PCM16 downmix of the guest 5.1),
  // 1 final mix with 2 mix buffers for stereo output, 1 device sink.
  // NOTE: AudioRendererConfig in vendored libnx has no `revision` field —
  // the renderer revision is negotiated internally by audrenInitialize()
  // (uses the latest supported rev, currently AUDREN_REVISION_6).
  const AudioRendererConfig ar_config = {
      .output_rate     = AudioRendererOutputRate_48kHz,
      .num_voices      = 1,
      .num_effects     = 0,
      .num_sinks       = 1,
      .num_mix_objs    = 1,
      .num_mix_buffers = static_cast<int>(kOutputChannels),
  };

  // STEP 1: Connect to the audren service. `audrvCreate` cannot be called
  // before this — it assumes the renderer service session exists.
  Result rc = audrenInitialize(&ar_config);
  if (R_FAILED(rc)) {
    REXAPU_ERROR("audrenInitialize failed: 0x{:08X}", rc);
    return false;
  }
  audren_service_initialized_ = true;

  // STEP 2: Create the high-level audio driver wrapper over audren.
  rc = audrvCreate(&audren_driver_, &ar_config, static_cast<int>(kOutputChannels));
  if (R_FAILED(rc)) {
    REXAPU_ERROR("audrvCreate failed: 0x{:08X}", rc);
    return false;
  }
  audren_driver_initialized_ = true;

  // Compute aligned memory pool size for double-buffered PCM16 output.
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

  // STEP 3 + 4: Register the PCM memory pool with audren and attach it.
  mempool_id_ = audrvMemPoolAdd(&audren_driver_, mempool_ptr_, mempool_size_);
  if (mempool_id_ < 0) {
    REXAPU_ERROR("audrvMemPoolAdd failed");
    return false;
  }
  if (!audrvMemPoolAttach(&audren_driver_, mempool_id_)) {
    REXAPU_ERROR("audrvMemPoolAttach failed");
    return false;
  }

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

  // STEP 5: Add a device sink for the default audio output.
  const u8 sink_channels[] = {0, 1};
  int sink_id = audrvDeviceSinkAdd(&audren_driver_, AUDREN_DEFAULT_DEVICE_NAME,
                                   static_cast<int>(kOutputChannels), sink_channels);
  if (sink_id < 0) {
    REXAPU_ERROR("audrvDeviceSinkAdd failed");
    return false;
  }

  // STEP 6: Push initial state to the renderer.
  rc = audrvUpdate(&audren_driver_);
  if (R_FAILED(rc)) {
    REXAPU_ERROR("Initial audrvUpdate failed: 0x{:08X}", rc);
    return false;
  }

  // STEP 7: Start the audio renderer.
  rc = audrenStartAudioRenderer();
  if (R_FAILED(rc)) {
    REXAPU_ERROR("audrenStartAudioRenderer failed: 0x{:08X}", rc);
    return false;
  }

  // Raise thread priority for the audio worker that calls SubmitFrame.
  // Switch default is 0x2C; audio glitches under rendering load unless we
  // bump to 0x28 (higher priority on Switch = lower numeric value).
  Result prio_rc = svcSetThreadPriority(CUR_THREAD_HANDLE, 0x28);
  if (R_FAILED(prio_rc)) {
    REXAPU_WARN("svcSetThreadPriority(0x28) failed: 0x{:08X} (non-fatal)", prio_rc);
  }

  // Start the voice.
  audrvVoiceStart(&audren_driver_, 0);

  return true;
}

// Returns true if the current-slot wave buffer is no longer owned by the
// renderer (Free/Done) and may be safely overwritten. Caller must hold
// `audren_mutex_` before invoking audrvUpdate.
bool SwitchAudioDriver::CurrentSlotIsReusable() {
  const auto state = wavebufs_[current_buf_].state;
  return state == AudioDriverWaveBufState_Done ||
         state == AudioDriverWaveBufState_Free;
}

// Wait for the current wave-buffer slot to transition to Done/Free before
// returning. Polls audrvUpdate, gated on audrenWaitFrame() to avoid a busy
// loop. Caller passes the unique_lock owning `audren_mutex_`; this function
// drops and reacquires it around the blocking frame wait. On return the
// caller's lock is re-held.
void SwitchAudioDriver::WaitForCurrentSlotReusable(
    std::unique_lock<std::mutex>& drv_lock) {
  while (!CurrentSlotIsReusable()) {
    drv_lock.unlock();
    audrenWaitFrame();
    drv_lock.lock();
    audrvUpdate(&audren_driver_);
  }
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

  // Try to submit directly; if the current slot is still owned by the
  // renderer (Queued/Waiting/Playing), queue the frame for drain below.
  bool submitted_directly = false;
  {
    std::unique_lock<std::mutex> drv_guard(audren_mutex_);
    // Refresh renderer-side buffer state.
    audrvUpdate(&audren_driver_);
    if (CurrentSlotIsReusable()) {
      ConvertAndSubmit(output_frame);  // holds audren_mutex_
      submitted_directly = true;
    }
  }

  if (submitted_directly) {
    {
      std::unique_lock<std::mutex> guard(frames_mutex_);
      frames_unused_.push(output_frame);
    }
    if (semaphore_) {
      auto ret = semaphore_->Release(1, nullptr);
      assert_true(ret);
    }
  } else {
    std::unique_lock<std::mutex> guard(frames_mutex_);
    frames_queued_.push(output_frame);
  }

  // Drain any queued frames into free buffer slots. CRITICAL: we must not
  // overwrite pcm_buffers_[idx] while the hardware is still referencing it.
  // WaitForCurrentSlotReusable blocks until state transitions to Done/Free.
  while (true) {
    float* queued = nullptr;
    {
      std::unique_lock<std::mutex> guard(frames_mutex_);
      if (frames_queued_.empty()) break;
      queued = frames_queued_.front();
      frames_queued_.pop();
    }

    {
      std::unique_lock<std::mutex> drv_guard(audren_mutex_);
      // Block until the current slot is safe to overwrite (per libnx
      // `audren_user` sample: do NOT write a wave buffer whose state is
      // Queued / Waiting / Playing).
      WaitForCurrentSlotReusable(drv_guard);
      ConvertAndSubmit(queued);
    }

    {
      std::unique_lock<std::mutex> guard(frames_mutex_);
      frames_unused_.push(queued);
    }
    if (semaphore_) {
      auto ret = semaphore_->Release(1, nullptr);
      assert_true(ret);
    }
  }
}

// Convert a single guest 5.1 float frame to stereo PCM16 and submit to the
// current wave-buffer slot. Caller MUST hold `audren_mutex_` and MUST have
// verified that the current slot is Done/Free (see WaitForCurrentSlotReusable).
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
  // Teardown order per libnx `audren_user` sample:
  //   audrvVoiceStop -> audrvClose -> audrenStopAudioRenderer -> audrenExit
  // Serialize against the audio worker: anything touching audrv_* needs
  // audren_mutex_.
  {
    std::unique_lock<std::mutex> drv_guard(audren_mutex_);

    if (voice_initialized_) {
      audrvVoiceStop(&audren_driver_, 0);
      audrvVoiceDrop(&audren_driver_, 0);
      voice_initialized_ = false;
    }

    if (audren_driver_initialized_) {
      if (mempool_id_ >= 0) {
        audrvMemPoolDetach(&audren_driver_, mempool_id_);
        audrvMemPoolRemove(&audren_driver_, mempool_id_);
        mempool_id_ = -1;
      }
      audrvClose(&audren_driver_);
      audren_driver_initialized_ = false;
    }

    if (audren_service_initialized_) {
      audrenStopAudioRenderer();
      audrenExit();
      audren_service_initialized_ = false;
    }
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
