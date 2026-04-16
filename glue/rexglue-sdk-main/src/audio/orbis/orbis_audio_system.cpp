/**
 ******************************************************************************
 * ReXGlue : PS4 Audio Backend (sceAudioOut)                                  *
 ******************************************************************************
 */

#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include "orbis_audio_system.h"
#include "orbis_audio_driver.h"

#include <rex/audio/flags.h>

namespace rex::audio::orbis {

std::unique_ptr<AudioSystem> OrbisAudioSystem::Create(
    runtime::FunctionDispatcher* function_dispatcher) {
  return std::make_unique<OrbisAudioSystem>(function_dispatcher);
}

OrbisAudioSystem::OrbisAudioSystem(runtime::FunctionDispatcher* function_dispatcher)
    : AudioSystem(function_dispatcher) {}

OrbisAudioSystem::~OrbisAudioSystem() {}

void OrbisAudioSystem::Initialize() {
  AudioSystem::Initialize();
}

X_STATUS OrbisAudioSystem::CreateDriver([[maybe_unused]] size_t index,
                                        rex::thread::Semaphore* semaphore,
                                        AudioDriver** out_driver) {
  assert_not_null(out_driver);
  auto driver = new OrbisAudioDriver(memory_, semaphore);
  if (!driver->Initialize()) {
    driver->Shutdown();
    delete driver;
    return X_STATUS_UNSUCCESSFUL;
  }

  *out_driver = driver;
  return X_STATUS_SUCCESS;
}

void OrbisAudioSystem::DestroyDriver(AudioDriver* driver) {
  assert_not_null(driver);
  auto orbis_driver = dynamic_cast<OrbisAudioDriver*>(driver);
  assert_not_null(orbis_driver);
  orbis_driver->Shutdown();
  delete orbis_driver;
}

}  // namespace rex::audio::orbis

#endif  // REX_PLATFORM_PS4
