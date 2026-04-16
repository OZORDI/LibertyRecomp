/**
 ******************************************************************************
 * ReXGlue : Runtime glue layer                                               *
 ******************************************************************************
 * Platform-agnostic keyboard input dialog interface for XamShowKeyboardUI.
 *
 * All platform backends block until the user confirms/cancels. On iOS and
 * Android the native modals are shown asynchronously on the UI thread, but
 * the calling (game) thread blocks on a semaphore/latch until completion,
 * matching the synchronous contract of the console backends (PS4 sceImeDialog
 * poll loop, Switch swkbdShow).
 *
 * Platform backends register themselves via RegisterKeyboardDialogBackend()
 * from a static initializer. If no backend is registered, ShowKeyboardDialog()
 * returns the default_text unchanged (matches the previous empty-return stub
 * behaviour for desktop imgui-served builds that still use the imgui dialog
 * directly from xam_ui.cpp).
 ******************************************************************************/

#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace rex {
namespace kernel {
namespace xam {

struct KeyboardDialogParams {
  std::string title;
  std::string description;
  std::string default_text;
  uint32_t max_length = 0;
  // Bitmask matching XamShowKeyboardUI flags; only the portable bits are
  // honoured by backends (password, numeric, email, uri, gamertag).
  uint32_t flags = 0;
};

// Bits in KeyboardDialogParams::flags. These mirror a subset of Xam keyboard
// flags and are translated per-backend.
enum KeyboardDialogFlag : uint32_t {
  kKeyboardDialogFlag_Default  = 0,
  kKeyboardDialogFlag_Password = 1u << 0,
  kKeyboardDialogFlag_Numeric  = 1u << 1,
  kKeyboardDialogFlag_Email    = 1u << 2,
  kKeyboardDialogFlag_Uri      = 1u << 3,
  kKeyboardDialogFlag_Gamertag = 1u << 4,
};

struct KeyboardDialogResult {
  bool accepted = false;
  std::string text;
};

// Optional async-style callback variant (not used by the blocking wrapper
// below, but exposed so higher layers can build non-blocking flows if needed).
using KeyboardDialogCallback =
    std::function<void(bool accepted, const std::string& text)>;

// Backend function signature. Blocks until the user dismisses the dialog.
using KeyboardDialogBackend =
    std::function<KeyboardDialogResult(const KeyboardDialogParams& params)>;

// Register a platform-native backend. Called once from a TU-scoped static
// initializer in each keyboard_dialog_<platform>.* file. Last registration
// wins.
void RegisterKeyboardDialogBackend(KeyboardDialogBackend backend);

// True if a platform backend has been registered.
bool HasKeyboardDialogBackend();

// Blocking call. Shows the native keyboard dialog and returns the text the
// user entered. Returns params.default_text if the user cancels or if no
// backend is registered.
std::string ShowKeyboardDialog(const KeyboardDialogParams& params);

// Blocking call returning the full result (accepted flag + text).
KeyboardDialogResult ShowKeyboardDialogEx(const KeyboardDialogParams& params);

}  // namespace xam
}  // namespace kernel
}  // namespace rex
