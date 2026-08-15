/**
 ******************************************************************************
 * ReXGlue : native macOS CoreAudio system                                    *
 ******************************************************************************
 */

#include <rex/platform.h>

#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS

#include <string>

#include <rex/audio/coreaudio/coreaudio_audio_driver.h>
#include <rex/audio/coreaudio/coreaudio_audio_system.h>
#include <rex/audio/coreaudio/coreaudio_output.h>
#include <rex/audio/sdl/sdl_audio_driver.h>
#include <rex/cvar.h>
#include <rex/logging.h>

REXCVAR_DEFINE_STRING(audio_backend, "auto", "Audio",
                      "Audio output backend on macOS: auto, coreaudio, or sdl");

namespace rex::audio::coreaudio {

std::unique_ptr<AudioSystem> CoreAudioAudioSystem::Create(
    runtime::FunctionDispatcher* function_dispatcher) {
  return std::make_unique<CoreAudioAudioSystem>(function_dispatcher);
}

CoreAudioAudioSystem::CoreAudioAudioSystem(runtime::FunctionDispatcher* function_dispatcher)
    : AudioSystem(function_dispatcher) {}

CoreAudioAudioSystem::~CoreAudioAudioSystem() {
  if (output_) {
    output_->Shutdown();
  }
}

bool CoreAudioAudioSystem::IsAvailable() {
  return true;
}

void CoreAudioAudioSystem::Initialize() {
  AudioSystem::Initialize();
  const std::string backend = REXCVAR_GET(audio_backend);
  if (backend == "sdl") {
    use_sdl_fallback_ = true;
    REXAPU_INFO("Audio backend: SDL (explicit)");
    return;
  }
  if (backend != "auto" && backend != "coreaudio") {
    REXAPU_WARN("Unknown audio_backend '{}'; using auto", backend);
  }

  output_ = std::make_shared<CoreAudioOutput>();
  const bool initialized = output_->Initialize();
  if (!initialized && backend != "coreaudio") {
    use_sdl_fallback_ = true;
    output_->Shutdown();
    output_.reset();
    REXAPU_WARN("Audio backend: CoreAudio initialization failed; falling back to SDL");
  } else {
    REXAPU_INFO("Audio backend: native CoreAudio{}", initialized ? "" : " (paced silence)");
  }
}

X_STATUS CoreAudioAudioSystem::CreateDriver(size_t, rex::thread::Semaphore* semaphore,
                                            AudioDriver** out_driver) {
  assert_not_null(out_driver);
  if (use_sdl_fallback_) {
    auto* driver = new sdl::SDLAudioDriver(memory_, semaphore);
    if (!driver->Initialize()) {
      driver->Shutdown();
      delete driver;
      return X_STATUS_UNSUCCESSFUL;
    }
    *out_driver = driver;
    return X_STATUS_SUCCESS;
  }

  auto* driver = new CoreAudioAudioDriver(memory_, semaphore, output_);
  if (!driver->Initialize()) {
    driver->Shutdown();
    delete driver;
    return X_STATUS_UNSUCCESSFUL;
  }
  *out_driver = driver;
  return X_STATUS_SUCCESS;
}

void CoreAudioAudioSystem::DestroyDriver(AudioDriver* driver) {
  assert_not_null(driver);
  if (auto* coreaudio_driver = dynamic_cast<CoreAudioAudioDriver*>(driver)) {
    coreaudio_driver->Shutdown();
    delete coreaudio_driver;
    return;
  }
  auto* sdl_driver = dynamic_cast<sdl::SDLAudioDriver*>(driver);
  assert_not_null(sdl_driver);
  sdl_driver->Shutdown();
  delete sdl_driver;
}

}  // namespace rex::audio::coreaudio

#endif  // REX_PLATFORM_MAC && !REX_PLATFORM_IOS
