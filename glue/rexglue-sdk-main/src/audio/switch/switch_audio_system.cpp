/**
 ******************************************************************************
 * ReXGlue Runtime                                                            *
 ******************************************************************************
 * Copyright 2026. Released under the BSD license.                            *
 ******************************************************************************
 *
 * Switch audio system implementation.
 */

#if REX_PLATFORM_NX

#include <rex/audio/flags.h>
#include <rex/audio/switch/switch_audio_driver.h>
#include <rex/audio/switch/switch_audio_system.h>

namespace rex::audio::nx {

std::unique_ptr<AudioSystem> SwitchAudioSystem::Create(
    runtime::FunctionDispatcher* function_dispatcher) {
  return std::make_unique<SwitchAudioSystem>(function_dispatcher);
}

SwitchAudioSystem::SwitchAudioSystem(runtime::FunctionDispatcher* function_dispatcher)
    : AudioSystem(function_dispatcher) {}

SwitchAudioSystem::~SwitchAudioSystem() {}

void SwitchAudioSystem::Initialize() {
  AudioSystem::Initialize();
}

X_STATUS SwitchAudioSystem::CreateDriver([[maybe_unused]] size_t index,
                                         rex::thread::Semaphore* semaphore,
                                         AudioDriver** out_driver) {
  assert_not_null(out_driver);
  auto driver = new SwitchAudioDriver(memory_, semaphore);
  if (!driver->Initialize()) {
    driver->Shutdown();
    delete driver;
    return X_STATUS_UNSUCCESSFUL;
  }

  *out_driver = driver;
  return X_STATUS_SUCCESS;
}

void SwitchAudioSystem::DestroyDriver(AudioDriver* driver) {
  assert_not_null(driver);
  auto nx_driver = dynamic_cast<SwitchAudioDriver*>(driver);
  assert_not_null(nx_driver);
  nx_driver->Shutdown();
  delete nx_driver;
}

}  // namespace rex::audio::nx

#endif  // REX_PLATFORM_NX
