/**
 ******************************************************************************
 * ReXGlue : Runtime glue layer                                               *
 ******************************************************************************
 * Nintendo Switch (libnx) keyboard dialog backend using swkbd.
 *
 * swkbdShow is already synchronous (applet runs on the applet core and the
 * caller blocks until the user confirms). No extra semaphore wiring needed.
 ******************************************************************************/

#include <rex/platform.h>

#if REX_PLATFORM_NX

#include <rex/kernel/xam/keyboard_dialog.h>
#include <rex/logging.h>

#include <switch/applets/swkbd.h>

#include <cstring>
#include <vector>

namespace rex {
namespace kernel {
namespace xam {

namespace {

static KeyboardDialogResult RunBackend(const KeyboardDialogParams& params) {
  KeyboardDialogResult result;
  result.accepted = false;
  result.text = params.default_text;

  SwkbdConfig cfg{};
  Result rc = swkbdCreate(&cfg, 0);
  if (R_FAILED(rc)) {
    REXKRNL_WARN("[keyboard_dialog][switch] swkbdCreate failed: 0x{:08X}",
                 static_cast<unsigned>(rc));
    return result;
  }

  swkbdConfigMakePresetDefault(&cfg);

  if (!params.title.empty()) {
    swkbdConfigSetHeaderText(&cfg, params.title.c_str());
  }
  if (!params.description.empty()) {
    swkbdConfigSetSubText(&cfg, params.description.c_str());
    swkbdConfigSetGuideText(&cfg, params.description.c_str());
  }
  if (!params.default_text.empty()) {
    swkbdConfigSetInitialText(&cfg, params.default_text.c_str());
  }

  const uint32_t max_length =
      params.max_length > 0 ? params.max_length : 256;
  swkbdConfigSetStringLenMax(&cfg, max_length);
  swkbdConfigSetStringLenMin(&cfg, 0);

  if (params.flags & kKeyboardDialogFlag_Numeric) {
    swkbdConfigSetType(&cfg, SwkbdType_NumPad);
  } else {
    swkbdConfigSetType(&cfg, SwkbdType_Normal);
  }
  if (params.flags & kKeyboardDialogFlag_Password) {
    swkbdConfigSetPasswordFlag(&cfg, 1);
  }

  std::vector<char> buffer(max_length + 1, 0);
  rc = swkbdShow(&cfg, buffer.data(), buffer.size());
  swkbdClose(&cfg);

  if (R_SUCCEEDED(rc)) {
    result.accepted = true;
    result.text.assign(buffer.data());
  } else {
    REXKRNL_WARN("[keyboard_dialog][switch] swkbdShow failed/cancelled: 0x{:08X}",
                 static_cast<unsigned>(rc));
  }

  return result;
}

struct Register {
  Register() { RegisterKeyboardDialogBackend(&RunBackend); }
};
static Register s_register;

}  // namespace

}  // namespace xam
}  // namespace kernel
}  // namespace rex

#endif  // REX_PLATFORM_NX
