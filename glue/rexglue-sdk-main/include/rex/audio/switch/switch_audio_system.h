/**
 ******************************************************************************
 * ReXGlue Runtime                                                            *
 ******************************************************************************
 * Copyright 2026. Released under the BSD license.                            *
 ******************************************************************************
 *
 * Switch audio system backend using libnx audio renderer.
 */

#pragma once

#if REX_PLATFORM_NX

#include <rex/audio/audio_system.h>

namespace rex::audio::nx {

class SwitchAudioSystem : public AudioSystem {
 public:
  explicit SwitchAudioSystem(runtime::FunctionDispatcher* function_dispatcher);
  ~SwitchAudioSystem() override;

  static bool IsAvailable() { return true; }

  static std::unique_ptr<AudioSystem> Create(runtime::FunctionDispatcher* function_dispatcher);

  X_STATUS CreateDriver(size_t index, rex::thread::Semaphore* semaphore,
                        AudioDriver** out_driver) override;
  void DestroyDriver(AudioDriver* driver) override;

 protected:
  void Initialize() override;
};

}  // namespace rex::audio::nx

#endif  // REX_PLATFORM_NX
