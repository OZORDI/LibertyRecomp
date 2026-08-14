/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <string>

#include <rex/assert.h>
#include <rex/audio/conversion.h>
#include <rex/audio/flags.h>
#include <rex/audio/sdl/sdl_audio_driver.h>
#include <rex/cvar.h>
#include <rex/dbg.h>
#include <rex/logging.h>
#include <rex/perf/counter.h>
#include <SDL3/SDL.h>

REXCVAR_DEFINE_BOOL(audio_mute, false, "Audio", "Mute audio output");

namespace rex::audio::sdl {

namespace {

struct SDLSubsystemResult {
  bool success = false;
  std::string error;
};

void SDLCALL InitializeAudioSubsystem(void* userdata) {
  auto* result = static_cast<SDLSubsystemResult*>(userdata);
  result->success = SDL_InitSubSystem(SDL_INIT_AUDIO);
  if (!result->success) {
    result->error = SDL_GetError();
  }
}

void SDLCALL ShutdownAudioSubsystem(void*) {
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

}  // namespace

SDLAudioDriver::SDLAudioDriver(memory::Memory* memory, rex::thread::Semaphore* semaphore)
    : AudioDriver(memory), semaphore_(semaphore) {}

SDLAudioDriver::~SDLAudioDriver() {
  assert_true(frames_queued_.empty());
  assert_true(frames_unused_.empty());
}

bool SDLAudioDriver::Initialize() {
  // Prevent SDL from interfering with timer resolution (causes FPS drops)
  SDL_SetHintWithPriority(SDL_HINT_TIMER_RESOLUTION, "0", SDL_HINT_OVERRIDE);

  // Set audio category for proper OS audio handling
  SDL_SetHint(SDL_HINT_AUDIO_CATEGORY, "playback");

  // Set app name for audio device identification
  SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, "rexglue");

  SDLSubsystemResult subsystem_result;
  if (SDL_IsMainThread()) {
    InitializeAudioSubsystem(&subsystem_result);
  } else if (!SDL_RunOnMainThread(InitializeAudioSubsystem, &subsystem_result, true)) {
    REXAPU_ERROR(
        "SDL_RunOnMainThread() for audio initialization failed: {}; using paced silent "
        "fallback",
        SDL_GetError());
    silent_fallback_ = true;
    return true;
  }
  if (!subsystem_result.success) {
    REXAPU_ERROR("SDL_InitSubSystem(SDL_INIT_AUDIO) failed: {}; using paced silent fallback",
                 subsystem_result.error);
    silent_fallback_ = true;
    return true;
  }
  sdl_initialized_ = true;

  SDL_AudioSpec desired_spec = {};
  SDL_AudioSpec obtained_spec = {};
  SDL_AudioSpec preferred_spec = {};
  desired_spec.freq = frame_frequency_;
  desired_spec.format = SDL_AUDIO_F32LE;
  if (SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &preferred_spec, nullptr)) {
    REXAPU_INFO("Default playback device '{}' prefers {} Hz, {} channels, format {}",
                SDL_GetAudioDeviceName(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK), preferred_spec.freq,
                preferred_spec.channels, uint32_t(preferred_spec.format));
  } else {
    REXAPU_WARN("SDL_GetAudioDeviceFormat() before open failed: {}", SDL_GetError());
  }

  // The callback can provide either the guest's six channels or its existing
  // stereo downmix. Select stereo before opening stereo-only devices so
  // CoreAudio never has to start an unsupported six-channel queue merely to
  // discover the physical layout.
  const bool prefer_stereo = preferred_spec.channels > 0 && preferred_spec.channels < 6;
  desired_spec.channels = prefer_stereo ? 2 : frame_channels_;
  sdl_device_channels_ = desired_spec.channels;
  sdl_stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desired_spec,
                                          SDLCallback, this);
  if (!sdl_stream_) {
    if (desired_spec.channels != 2) {
      REXAPU_WARN("SDL_OpenAudioDeviceStream() for {} channels failed: {}; retrying stereo",
                  desired_spec.channels, SDL_GetError());
      desired_spec.channels = 2;
      sdl_device_channels_ = 2;
      sdl_stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desired_spec,
                                              SDLCallback, this);
    }
    if (!sdl_stream_) {
      REXAPU_ERROR(
          "SDL_OpenAudioDeviceStream() stereo fallback failed: {}; using paced silent "
          "fallback",
          SDL_GetError());
      silent_fallback_ = true;
      return true;
    }
  }

  SDL_AudioDeviceID sdl_device = SDL_GetAudioStreamDevice(sdl_stream_);
  if (!sdl_device) {
    REXAPU_ERROR("SDL_GetAudioStreamDevice() failed: {}; using paced silent fallback",
                 SDL_GetError());
    SDL_DestroyAudioStream(sdl_stream_);
    sdl_stream_ = nullptr;
    silent_fallback_ = true;
    return true;
  }

  if (!SDL_GetAudioDeviceFormat(sdl_device, &obtained_spec, NULL)) {
    REXAPU_WARN("SDL_GetAudioDeviceFormat() failed: {}", SDL_GetError());
    obtained_spec = desired_spec;
  }

  REXAPU_INFO(
      "Opened playback stream with {} input channels; device is {} Hz, {} channels, "
      "format {}",
      desired_spec.channels, obtained_spec.freq, obtained_spec.channels,
      uint32_t(obtained_spec.format));

  if (!SDL_ResumeAudioStreamDevice(sdl_stream_)) {
    REXAPU_ERROR("SDL_ResumeAudioStreamDevice() failed: {}; using paced silent fallback",
                 SDL_GetError());
    SDL_DestroyAudioStream(sdl_stream_);
    sdl_stream_ = nullptr;
    silent_fallback_ = true;
    return true;
  }

  return true;
}

void SDLAudioDriver::SubmitFrame(uint32_t frame_ptr) {
  if (silent_fallback_) {
    rex::thread::Sleep(std::chrono::microseconds(silent_frame_duration_microseconds_));
    const bool released = semaphore_->Release(1, nullptr);
    assert_true(released);
    return;
  }

  const auto input_frame = memory_->TranslateVirtual<float*>(frame_ptr);
  float* output_frame;
  {
    std::unique_lock<std::mutex> guard(frames_mutex_);
    if (frames_unused_.empty()) {
      output_frame = new float[frame_samples_];
    } else {
      output_frame = frames_unused_.top();
      frames_unused_.pop();
    }
  }

  std::memcpy(output_frame, input_frame, frame_samples_ * sizeof(float));

  static uint32_t sdl_submit_count = 0;
  if (sdl_submit_count < 10) {
    REXAPU_DEBUG("SDLAudioDriver::SubmitFrame: frame_ptr={:08X} queued_count={}", frame_ptr,
                 frames_queued_.size() + 1);
    sdl_submit_count++;
  }

  {
    std::unique_lock<std::mutex> guard(frames_mutex_);
    frames_queued_.push(output_frame);
    PROFILE_BUFFER_QUEUE_DEPTH(static_cast<int64_t>(frames_queued_.size()));
  }
}

void SDLAudioDriver::Pause() {
  if (!sdl_stream_) {
    FlushQueuedFrames();
    return;
  }
  if (!SDL_PauseAudioStreamDevice(sdl_stream_)) {
    REXAPU_WARN("SDL_PauseAudioStreamDevice() failed: {}", SDL_GetError());
  }
  Flush();
}

void SDLAudioDriver::Resume() {
  if (sdl_stream_ && !SDL_ResumeAudioStreamDevice(sdl_stream_)) {
    REXAPU_WARN("SDL_ResumeAudioStreamDevice() failed: {}", SDL_GetError());
  }
}

void SDLAudioDriver::Flush() {
  if (!sdl_stream_) {
    FlushQueuedFrames();
    return;
  }
  if (!SDL_LockAudioStream(sdl_stream_)) {
    REXAPU_WARN("SDL_LockAudioStream() failed while flushing: {}", SDL_GetError());
    return;
  }
  FlushQueuedFrames();
  if (!SDL_ClearAudioStream(sdl_stream_)) {
    REXAPU_WARN("SDL_ClearAudioStream() failed: {}", SDL_GetError());
  }
  SDL_UnlockAudioStream(sdl_stream_);
}

void SDLAudioDriver::FlushQueuedFrames() {
  std::unique_lock<std::mutex> guard(frames_mutex_);
  while (!frames_queued_.empty()) {
    frames_unused_.push(frames_queued_.front());
    frames_queued_.pop();
  }
}

void SDLAudioDriver::Shutdown() {
  silent_fallback_ = false;
  if (sdl_stream_) {
    SDL_DestroyAudioStream(sdl_stream_);
    sdl_stream_ = nullptr;
  }
  if (sdl_initialized_) {
    if (SDL_IsMainThread()) {
      ShutdownAudioSubsystem(nullptr);
    } else if (!SDL_RunOnMainThread(ShutdownAudioSubsystem, nullptr, true)) {
      REXAPU_ERROR("SDL_RunOnMainThread() for audio shutdown failed: {}", SDL_GetError());
    }
    sdl_initialized_ = false;
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

void SDLAudioDriver::SDLCallback(void* userdata, SDL_AudioStream* stream, int additional_amount,
                                 [[maybe_unused]] int total_amount) {
  SCOPE_profile_cpu_f("apu");
  if (!userdata || !stream) {
    REXAPU_ERROR("SDLAudioDriver::SDLCallback called with nullptr.");
    return;
  }
  const auto driver = static_cast<SDLAudioDriver*>(userdata);
  const int sample_count =
      static_cast<int>(channel_samples_ * std::max<uint8_t>(driver->sdl_device_channels_, 1));
  const int len = static_cast<int>(sizeof(float) * sample_count);
  float* data = SDL_stack_alloc(float, sample_count);
  if (!data) {
    REXAPU_ERROR("SDLAudioDriver::SDLCallback failed to allocate {} samples", sample_count);
    return;
  }
  while (additional_amount > 0) {
    static uint32_t sdl_callback_count = 0;
    std::unique_lock<std::mutex> guard(driver->frames_mutex_);
    if (driver->frames_queued_.empty()) {
      if (sdl_callback_count < 10) {
        REXAPU_DEBUG("SDLCallback: no frames queued (silence)");
        sdl_callback_count++;
      }
      std::memset(data, 0, len);
      if (!SDL_PutAudioStreamData(stream, data, len)) {
        REXAPU_ERROR("SDL_PutAudioStreamData() failed while filling silence: {}", SDL_GetError());
        break;
      }
      additional_amount -= len;
    } else {
      auto buffer = driver->frames_queued_.front();
      driver->frames_queued_.pop();
      if (REXCVAR_GET(audio_mute)) {
        std::memset(data, 0, len);
      } else {
        switch (driver->sdl_device_channels_) {
          case 2:
            conversion::sequential_6_BE_to_interleaved_2_LE(data, buffer, channel_samples_);
            break;
          case 6:
            conversion::sequential_6_BE_to_interleaved_6_LE(data, buffer, channel_samples_);
            break;
          default:
            assert_unhandled_case(driver->sdl_device_channels_);
            break;
        }
      }
      if (!SDL_PutAudioStreamData(stream, data, len)) {
        REXAPU_ERROR("SDL_PutAudioStreamData() failed: {}", SDL_GetError());
        driver->frames_unused_.push(buffer);
        break;
      }
      driver->frames_unused_.push(buffer);

      auto ret = driver->semaphore_->Release(1, nullptr);
      assert_true(ret);
      additional_amount -= len;
    }
  }
  SDL_stack_free(data);
}

}  // namespace rex::audio::sdl
