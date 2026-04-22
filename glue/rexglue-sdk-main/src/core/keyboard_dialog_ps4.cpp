/**
 ******************************************************************************
 * ReXGlue : Runtime glue layer                                               *
 ******************************************************************************
 * PS4 (Orbis) keyboard dialog backend using sceImeDialog.
 *
 * Blocking poll-loop implementation: Init -> poll GetStatus until FINISHED ->
 * GetResult -> Term. Runs on the calling thread; no UI-thread hop is needed
 * because sceImeDialog is a system modal handled by the compositor.
 ******************************************************************************/

#include <rex/platform.h>

#if REX_PLATFORM_PS4

#include <rex/kernel/xam/keyboard_dialog.h>
#include <rex/logging.h>
#include <rex/thread.h>

// NOTE: OpenOrbis ships only a partial sceImeDialog surface. The struct/enum
// names we'd otherwise use (OrbisImeDialogParam, ORBIS_IME_TYPE_*,
// ORBIS_IME_OPTION_PASSWORD, ORBIS_IME_DIALOG_STATUS_*) are missing. GTA IV
// never raises the system keyboard on disc/save paths, so we stub this
// backend to "cancelled" — games that need a real IME can wire one up once
// the open-toolchain headers cover it.
#include <chrono>
#include <cstring>
#include <vector>

namespace rex {
namespace kernel {
namespace xam {

namespace {

// Convert UTF-8 -> UTF-16 (host LE) for sceImeDialog input buffers. Very small
// implementation that only handles the BMP — sufficient for names/messages.
static void Utf8ToUtf16(const std::string& in, std::vector<uint16_t>* out) {
  out->clear();
  out->reserve(in.size() + 1);
  size_t i = 0;
  while (i < in.size()) {
    uint32_t cp = 0;
    unsigned char c = static_cast<unsigned char>(in[i]);
    if (c < 0x80) {
      cp = c;
      i += 1;
    } else if ((c & 0xE0) == 0xC0 && i + 1 < in.size()) {
      cp = ((c & 0x1F) << 6) | (static_cast<unsigned char>(in[i + 1]) & 0x3F);
      i += 2;
    } else if ((c & 0xF0) == 0xE0 && i + 2 < in.size()) {
      cp = ((c & 0x0F) << 12) |
           ((static_cast<unsigned char>(in[i + 1]) & 0x3F) << 6) |
           (static_cast<unsigned char>(in[i + 2]) & 0x3F);
      i += 3;
    } else {
      cp = '?';
      i += 1;
    }
    if (cp > 0xFFFF) cp = '?';  // BMP only
    out->push_back(static_cast<uint16_t>(cp));
  }
  out->push_back(0);
}

static std::string Utf16ToUtf8(const uint16_t* s) {
  std::string out;
  if (!s) return out;
  for (; *s; ++s) {
    uint32_t cp = *s;
    if (cp < 0x80) {
      out.push_back(static_cast<char>(cp));
    } else if (cp < 0x800) {
      out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
      out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
      out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
      out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
      out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
  }
  return out;
}

static KeyboardDialogResult RunBackend(const KeyboardDialogParams& params) {
  // OpenOrbis does not expose the full sceImeDialog surface (param struct,
  // option flags, status/end-status enums). Rather than maintain a fragile
  // shim that disagrees with the real SDK, return "cancelled, default text"
  // — matches how unsupported-keyboard paths behave on retail too.
  (void)Utf8ToUtf16;   // keep helpers referenced in case of future reuse
  (void)Utf16ToUtf8;
  KeyboardDialogResult result;
  result.accepted = false;
  result.text = params.default_text;
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

#endif  // REX_PLATFORM_PS4
