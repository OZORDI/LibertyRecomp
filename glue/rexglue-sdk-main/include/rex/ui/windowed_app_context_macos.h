#pragma once

#include <rex/ui/windowed_app_context.h>

namespace rex::ui {

class MacWindowedAppContext final : public WindowedAppContext {
 public:
  MacWindowedAppContext() = default;
  ~MacWindowedAppContext() override;

  void NotifyUILoopOfPendingFunctions() override;
  void PlatformQuitFromUIThread() override;

  void RunMainCocoaLoop();

 private:
  void* run_loop_ = nullptr;
};

}  // namespace rex::ui
