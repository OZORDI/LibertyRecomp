#pragma once
#include <string_view>
namespace gta4::presentation {
struct ModePolicy {
  bool explicit_mode = false;
  bool vsync = true;
  bool prefer_fifo = true;
  bool immediate = false;
  bool mailbox = false;
};
constexpr ModePolicy ResolveMode(std::string_view mode) {
  if (mode == "immediate") return {true, false, false, true, false};
  if (mode == "mailbox") return {true, true, false, false, true};
  if (mode == "fifo") return {true, true, true, false, false};
  return {};
}
} // namespace gta4::presentation
