// Android surface stub — LibertyRecomp uses SDL for windowing on Android.
// This provides the minimal Surface implementation required by rexui.

#include <rex/ui/window.h>

namespace rex::ui {

// Android does not use XCB/X11/GTK surfaces — Vulkan surfaces are created
// by SDL via ANativeWindow. No surface helper is needed.

}  // namespace rex::ui
