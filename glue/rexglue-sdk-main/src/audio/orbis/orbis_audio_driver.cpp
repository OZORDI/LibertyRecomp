/**
 ******************************************************************************
 * ReXGlue : PS4 Audio Backend (sceAudioOut)                                  *
 ******************************************************************************
 */

#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include "orbis_audio_driver.h"

#include <cstring>

#include <rex/assert.h>
#include <rex/audio/conversion.h>
#include <rex/logging.h>

#include <orbis/AudioOut.h>

namespace rex::audio::orbis {

OrbisAudioDriver::OrbisAudioDriver(memory::Memory* memory, rex::thread::Semaphore* semaphore)
    : AudioDriver(memory), semaphore_(semaphore) {
  std::memset(output_buffer_, 0, sizeof(output_buffer_));
}

OrbisAudioDriver::~OrbisAudioDriver() {
  assert_true(frames_queued_.empty());
  assert_true(frames_unused_.empty());
}

bool OrbisAudioDriver::Initialize() {
  int32_t ret = sceAudioOutInit();
  if (ret < 0 && ret != (int32_t)0x8026000E /* SCE_AUDIO_OUT_ERROR_ALREADY_INIT */) {
    REXAPU_ERROR("sceAudioOutInit() failed: 0x{:08X}", static_cast<uint32_t>(ret));
    return false;
  }

  // Open a main audio port: stereo float, 256 samples per frame, 48 kHz.
  audio_handle_ = sceAudioOutOpen(
      0xFF,  // SCE_USER_SERVICE_USER_ID_SYSTEM
      ORBIS_AUDIO_OUT_PORT_TYPE_MAIN,
      0,     // index (unused for MAIN)
      kSamplesPerChannel,
      kFrequency,
      ORBIS_AUDIO_OUT_PARAM_FORMAT_FLOAT_STEREO);

  if (audio_handle_ < 0) {
    REXAPU_ERROR("sceAudioOutOpen() failed: 0x{:08X}", static_cast<uint32_t>(audio_handle_));
    return false;
  }

  REXAPU_INFO("OrbisAudioDriver: opened audio handle {}", audio_handle_);
  return true;
}

void OrbisAudioDriver::SubmitFrame(uint32_t frame_ptr) {
  // Translate guest 5.1 big-endian float buffer to host address.
  const auto input_frame = memory_->TranslateVirtual<float*>(frame_ptr);

  // Downmix 6ch BE to interleaved stereo LE.
  conversion::sequential_6_BE_to_interleaved_2_LE(output_buffer_, input_frame, kSamplesPerChannel);

  // sceAudioOutOutput is blocking -- it waits until the hardware consumes the
  // previous buffer, which naturally paces the audio thread at ~5.33 ms per
  // frame (256 samples / 48000 Hz).
  int32_t ret = sceAudioOutOutput(audio_handle_, output_buffer_);
  if (ret < 0) {
    static uint32_t error_count = 0;
    if (error_count < 10) {
      REXAPU_ERROR("sceAudioOutOutput() failed: 0x{:08X}", static_cast<uint32_t>(ret));
      error_count++;
    }
  }

  // Signal the semaphore so the audio system knows this client consumed a frame.
  auto ok = semaphore_->Release(1, nullptr);
  assert_true(ok);
}

void OrbisAudioDriver::Shutdown() {
  if (audio_handle_ >= 0) {
    sceAudioOutClose(audio_handle_);
    audio_handle_ = -1;
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

}  // namespace rex::audio::orbis

#endif  // REX_PLATFORM_PS4
