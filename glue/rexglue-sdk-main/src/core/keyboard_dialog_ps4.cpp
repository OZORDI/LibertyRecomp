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

#include <orbis/ImeDialog.h>

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
  KeyboardDialogResult result;
  result.accepted = false;
  result.text = params.default_text;

  const uint32_t max_length =
      params.max_length > 0 ? params.max_length : 256;

  std::vector<uint16_t> title16, desc16, default16;
  Utf8ToUtf16(params.title, &title16);
  Utf8ToUtf16(params.description, &desc16);
  Utf8ToUtf16(params.default_text, &default16);

  std::vector<uint16_t> buffer(max_length + 1, 0);
  if (default16.size() > 0 && default16.size() - 1 <= max_length) {
    std::memcpy(buffer.data(), default16.data(),
                (default16.size() - 1) * sizeof(uint16_t));
  }

  OrbisImeDialogParam ime{};
  sceImeDialogParamInit(&ime);
  ime.supportedLanguages = 0;  // all
  ime.languagesForced = 0;
  ime.type = (params.flags & kKeyboardDialogFlag_Numeric)
                 ? ORBIS_IME_TYPE_NUMBER
                 : ORBIS_IME_TYPE_DEFAULT;
  ime.option = 0;
  if (params.flags & kKeyboardDialogFlag_Password) {
    ime.option |= ORBIS_IME_OPTION_PASSWORD;
  }
  ime.maxTextLength = max_length;
  ime.inputTextBuffer = buffer.data();
  ime.title = title16.empty() ? nullptr : title16.data();
  ime.placeholder = desc16.empty() ? nullptr : desc16.data();

  int rc = sceImeDialogInit(&ime, nullptr);
  if (rc < 0) {
    REXKRNL_WARN("[keyboard_dialog][ps4] sceImeDialogInit failed: 0x{:08X}",
                 static_cast<unsigned>(rc));
    return result;
  }

  for (;;) {
    OrbisImeDialogStatus status = sceImeDialogGetStatus();
    if (status == ORBIS_IME_DIALOG_STATUS_FINISHED) {
      OrbisImeDialogResult r{};
      sceImeDialogGetResult(&r);
      if (r.endstatus == ORBIS_IME_DIALOG_END_STATUS_OK) {
        result.accepted = true;
        result.text = Utf16ToUtf8(buffer.data());
      }
      break;
    }
    if (status == ORBIS_IME_DIALOG_STATUS_NONE) {
      break;  // Dialog gone without finishing — treat as cancel.
    }
    rex::thread::Sleep(std::chrono::milliseconds(16));
  }

  sceImeDialogTerm();
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
