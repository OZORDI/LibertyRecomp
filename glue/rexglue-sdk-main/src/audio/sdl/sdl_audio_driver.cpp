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
#include <cstring>

#include <rex/assert.h>
#include <rex/audio/conversion.h>
#include <rex/audio/flags.h>
#include <rex/audio/sdl/sdl_audio_driver.h>
#include <rex/cvar.h>
#include <rex/dbg.h>
#include <rex/logging.h>

REXCVAR_DEFINE_BOOL(audio_mute, false, "Audio", "Mute audio output");

namespace rex::audio::sdl {

SDLAudioDriver::SDLAudioDriver(memory::Memory* memory, rex::thread::Semaphore* semaphore)
    : AudioDriver(memory), semaphore_(semaphore) {}

SDLAudioDriver::~SDLAudioDriver() {
  assert_true(frames_queued_.empty());
  assert_true(frames_unused_.empty());
}

bool SDLAudioDriver::Initialize() {
  SDL_version ver = {};
  SDL_GetVersion(&ver);
  if ((ver.major < 2) || (ver.major == 2 && ver.minor == 0 && ver.patch < 8)) {
    REXAPU_WARN(
        "SDL library version {}.{}.{} is outdated. "
        "You may experience choppy audio.",
        ver.major, ver.minor, ver.patch);
  }

  // Prevent SDL from interfering with timer resolution (causes FPS drops)
  SDL_SetHintWithPriority(SDL_HINT_TIMER_RESOLUTION, "0", SDL_HINT_OVERRIDE);

  // Set audio category for proper OS audio handling
  SDL_SetHint(SDL_HINT_AUDIO_CATEGORY, "playback");

  // Set app name for audio device identification
#if !SDL_VERSION_ATLEAST(2, 0, 14)
#define SDL_HINT_AUDIO_DEVICE_APP_NAME "SDL_AUDIO_DEVICE_APP_NAME"
#endif
  SDL_SetHint(SDL_HINT_AUDIO_DEVICE_APP_NAME, "rexglue");

  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
    REXAPU_ERROR("SDL_InitSubSystem(SDL_INIT_AUDIO) failed: {}", SDL_GetError());
    return false;
  }
  sdl_initialized_ = true;

  SDL_AudioSpec desired_spec = {};
  SDL_AudioSpec obtained_spec = {};
  desired_spec.freq = frame_frequency_;
  desired_spec.format = AUDIO_F32;
  desired_spec.channels = frame_channels_;
  desired_spec.samples = channel_samples_;
  desired_spec.callback = SDLCallback;
  desired_spec.userdata = this;
  // Set default before opening so an early callback won't see channels==0
  sdl_device_channels_ = frame_channels_;
  // Allow the hardware to decide between 5.1 and stereo
  int allowed_change = SDL_AUDIO_ALLOW_CHANNELS_CHANGE;
  for (int i = 0; i < 2; i++) {
    sdl_device_id_ = SDL_OpenAudioDevice(nullptr, 0, &desired_spec, &obtained_spec, allowed_change);
    if (sdl_device_id_ <= 0) {
      REXAPU_ERROR("SDL_OpenAudioDevice() failed: {}", SDL_GetError());
      return false;
    }
    if (obtained_spec.channels == 2 || obtained_spec.channels == 6) {
      break;
    }
    // If the system is 4 or 7.1, let SDL convert
    allowed_change = 0;
    SDL_CloseAudioDevice(sdl_device_id_);
    sdl_device_id_ = -1;
  }
  if (sdl_device_id_ <= 0) {
    REXAPU_ERROR("Failed to get a compatible SDL Audio Device.");
    return false;
  }
  sdl_device_channels_ = obtained_spec.channels;

  // Validate format detection produced a usable channel count
  if (sdl_device_channels_ != 2 && sdl_device_channels_ != 6) {
    REXAPU_WARN("SDL returned {} channels, falling back to stereo", sdl_device_channels_);
    SDL_CloseAudioDevice(sdl_device_id_);
    sdl_device_id_ = -1;
    desired_spec.channels = 2;
    sdl_device_id_ = SDL_OpenAudioDevice(nullptr, 0, &desired_spec, &obtained_spec, 0);
    if (sdl_device_id_ <= 0) {
      REXAPU_ERROR("SDL_OpenAudioDevice() stereo fallback failed: {}", SDL_GetError());
      return false;
    }
    sdl_device_channels_ = 2;
  }

  SDL_PauseAudioDevice(sdl_device_id_, 0);

  return true;
}

void SDLAudioDriver::SubmitFrame(uint32_t frame_ptr) {
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

  {
    std::unique_lock<std::mutex> guard(frames_mutex_);
    frames_queued_.push(output_frame);
  }
}

void SDLAudioDriver::Shutdown() {
  if (sdl_device_id_ > 0) {
    SDL_CloseAudioDevice(sdl_device_id_);
    sdl_device_id_ = -1;
  }
  if (sdl_initialized_) {
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
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

void SDLAudioDriver::SDLCallback(void* userdata, Uint8* stream, int len) {
  SCOPE_profile_cpu_f("apu");
  if (!userdata || !stream) {
    REXAPU_ERROR("SDLAudioDriver::SDLCallback called with nullptr.");
    return;
  }
  const auto driver = static_cast<SDLAudioDriver*>(userdata);
  const int expected_len =
      static_cast<int>(sizeof(float) * channel_samples_ *
                       std::max<uint8_t>(driver->sdl_device_channels_, 1));
  assert_true(len == expected_len);

  std::unique_lock<std::mutex> guard(driver->frames_mutex_);
  if (driver->frames_queued_.empty()) {
    // Feed silence when no frames are queued to prevent callback stalls
    static uint32_t sdl_silence_count = 0;
    if (sdl_silence_count < 10) {
      REXAPU_DEBUG("SDLCallback: no frames queued (silence)");
      sdl_silence_count++;
    }
    std::memset(stream, 0, len);
  } else {
    auto buffer = driver->frames_queued_.front();
    driver->frames_queued_.pop();
    if (REXCVAR_GET(audio_mute)) {
      // Zero the output buffer when muted (skip conversion work)
      std::memset(stream, 0, len);
    } else {
      switch (driver->sdl_device_channels_) {
        case 2:
          conversion::sequential_6_BE_to_interleaved_2_LE(reinterpret_cast<float*>(stream), buffer,
                                                          channel_samples_);
          break;
        case 6:
          conversion::sequential_6_BE_to_interleaved_6_LE(reinterpret_cast<float*>(stream), buffer,
                                                          channel_samples_);
          break;
        default:
          assert_unhandled_case(driver->sdl_device_channels_);
          std::memset(stream, 0, len);
          break;
      }
    }
    driver->frames_unused_.push(buffer);

    auto ret = driver->semaphore_->Release(1, nullptr);
    assert_true(ret);
  }
}

}  // namespace rex::audio::sdl
