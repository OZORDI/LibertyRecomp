/**
 ******************************************************************************
 * ReXGlue : native macOS CoreAudio system                                    *
 ******************************************************************************
 */

#pragma once

#include <memory>

#include <rex/audio/audio_system.h>

namespace rex::audio::coreaudio {

class CoreAudioOutput;

class CoreAudioAudioSystem final : public AudioSystem {
 public:
  explicit CoreAudioAudioSystem(runtime::FunctionDispatcher* function_dispatcher);
  ~CoreAudioAudioSystem() override;

  static bool IsAvailable();
  static std::unique_ptr<AudioSystem> Create(runtime::FunctionDispatcher* function_dispatcher);

  X_STATUS CreateDriver(size_t index, rex::thread::Semaphore* semaphore,
                        AudioDriver** out_driver) override;
  void DestroyDriver(AudioDriver* driver) override;

 protected:
  void Initialize() override;

 private:
  std::shared_ptr<CoreAudioOutput> output_;
  bool use_sdl_fallback_ = false;
};

}  // namespace rex::audio::coreaudio
