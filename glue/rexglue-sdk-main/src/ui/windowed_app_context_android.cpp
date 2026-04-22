// Android windowed app context stub — LibertyRecomp uses SDL for the event
// loop on Android, so these are minimal no-op implementations.

#include <rex/ui/windowed_app_context.h>

namespace rex::ui {

namespace {

class AndroidWindowedAppContext final : public WindowedAppContext {
 public:
  AndroidWindowedAppContext() = default;
  ~AndroidWindowedAppContext() override = default;

 protected:
  void NotifyUILoopOfPendingFunctions() override {
    // SDL event loop handles this on Android
  }
  void PlatformQuitFromUIThread() override {
    // SDL_Quit handles teardown on Android
  }
};

}  // namespace

}  // namespace rex::ui
