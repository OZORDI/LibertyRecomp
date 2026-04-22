// Android window stub — LibertyRecomp uses SDL for windowing on Android.
// This provides the minimal Window::Create factory and stub AndroidWindow
// required by rexui when built for Android.

#include <rex/ui/window.h>

#include <cstdint>
#include <memory>
#include <string_view>

namespace rex::ui {

namespace {

class AndroidWindow final : public Window {
 public:
  AndroidWindow(WindowedAppContext& app_context, const std::string_view title,
                uint32_t width, uint32_t height)
      : Window(app_context, title, width, height) {}
  ~AndroidWindow() override = default;

 protected:
  bool OpenImpl() override { return true; }
  void RequestCloseImpl() override {}
  std::unique_ptr<Surface> CreateSurfaceImpl(
      Surface::TypeFlags /*allowed_types*/) override {
    return nullptr;  // SDL creates the Vulkan surface on Android
  }
  void RequestPaintImpl() override {}
};

}  // namespace

// Static factory — each platform provides this.
std::unique_ptr<Window> Window::Create(WindowedAppContext& app_context,
                                       const std::string_view title,
                                       uint32_t desired_logical_width,
                                       uint32_t desired_logical_height) {
  return std::make_unique<AndroidWindow>(app_context, title,
                                         desired_logical_width,
                                         desired_logical_height);
}

}  // namespace rex::ui
