/**
 ******************************************************************************
 * ReXGlue : Runtime glue layer                                               *
 ******************************************************************************
 * Platform-agnostic keyboard dialog dispatcher.
 *
 * Holds the registered backend and routes ShowKeyboardDialog() calls to it.
 * If no backend is registered (e.g. desktop imgui-served builds) this falls
 * back to returning params.default_text, matching the prior stub contract.
 ******************************************************************************/

#include <rex/kernel/xam/keyboard_dialog.h>
#include <rex/logging.h>

#include <mutex>

namespace rex {
namespace kernel {
namespace xam {

namespace {
std::mutex& BackendMutex() {
  static std::mutex m;
  return m;
}
KeyboardDialogBackend& BackendSlot() {
  static KeyboardDialogBackend b;
  return b;
}
}  // namespace

void RegisterKeyboardDialogBackend(KeyboardDialogBackend backend) {
  std::lock_guard<std::mutex> lk(BackendMutex());
  BackendSlot() = std::move(backend);
}

bool HasKeyboardDialogBackend() {
  std::lock_guard<std::mutex> lk(BackendMutex());
  return static_cast<bool>(BackendSlot());
}

KeyboardDialogResult ShowKeyboardDialogEx(const KeyboardDialogParams& params) {
  KeyboardDialogBackend backend;
  {
    std::lock_guard<std::mutex> lk(BackendMutex());
    backend = BackendSlot();
  }
  if (!backend) {
    REXKRNL_WARN(
        "[keyboard_dialog] No platform backend registered; returning "
        "default_text for '{}'",
        params.title);
    KeyboardDialogResult r;
    r.accepted = false;
    r.text = params.default_text;
    return r;
  }
  return backend(params);
}

std::string ShowKeyboardDialog(const KeyboardDialogParams& params) {
  auto result = ShowKeyboardDialogEx(params);
  return result.accepted ? result.text : params.default_text;
}

}  // namespace xam
}  // namespace kernel
}  // namespace rex
