// =============================================================================
// Stub implementations for rex::ui symbols referenced by rexkernel (xam_ui.cpp).
// LibertyRecomp provides its own UI; these stubs satisfy the linker.
// =============================================================================

#include <functional>
#include <vector>

// Forward declarations matching the SDK headers to avoid pulling in the full
// rex::ui header tree (which needs Vulkan/graphics includes we don't want).
namespace rex {
namespace thread {
class Fence;
}  // namespace thread
namespace ui {
class ImGuiDrawer;

class ImGuiDialog {
 public:
  ~ImGuiDialog();
  void Then(rex::thread::Fence* fence);
  void Close();

 protected:
  ImGuiDialog(ImGuiDrawer* imgui_drawer);

 private:
  ImGuiDrawer* imgui_drawer_ = nullptr;
  bool has_close_pending_ = false;
  std::vector<rex::thread::Fence*> waiting_fences_;
};

class WindowedAppContext {
 public:
  virtual ~WindowedAppContext();
  bool CallInUIThreadSynchronous(std::function<void()> function);

 protected:
  virtual void NotifyUILoopOfPendingFunctions() = 0;
  virtual void PlatformQuitFromUIThread() = 0;
};

}  // namespace ui
}  // namespace rex

// --- Stub definitions --------------------------------------------------------

namespace rex {
namespace ui {

ImGuiDialog::ImGuiDialog(ImGuiDrawer* /*imgui_drawer*/) {}
ImGuiDialog::~ImGuiDialog() {}

void ImGuiDialog::Then(rex::thread::Fence* /*fence*/) {}
void ImGuiDialog::Close() {}

WindowedAppContext::~WindowedAppContext() {}

bool WindowedAppContext::CallInUIThreadSynchronous(
    std::function<void()> function) {
  if (function) function();
  return true;
}

}  // namespace ui
}  // namespace rex
