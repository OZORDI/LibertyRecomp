/**
 ******************************************************************************
 * ReXGlue : PS4 Audio Backend (sceAudioOut)                                  *
 ******************************************************************************
 */

#pragma once

#if REX_PLATFORM_PS4

#include <rex/audio/audio_system.h>

namespace rex::audio::orbis {

class OrbisAudioSystem : public AudioSystem {
 public:
  explicit OrbisAudioSystem(runtime::FunctionDispatcher* function_dispatcher);
  ~OrbisAudioSystem() override;

  static bool IsAvailable() { return true; }

  static std::unique_ptr<AudioSystem> Create(runtime::FunctionDispatcher* function_dispatcher);

  X_STATUS CreateDriver(size_t index, rex::thread::Semaphore* semaphore,
                        AudioDriver** out_driver) override;
  void DestroyDriver(AudioDriver* driver) override;

 protected:
  void Initialize() override;
};

}  // namespace rex::audio::orbis

#endif  // REX_PLATFORM_PS4
