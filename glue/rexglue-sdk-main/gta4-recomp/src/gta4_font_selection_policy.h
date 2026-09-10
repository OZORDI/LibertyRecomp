#pragma once
#include <cstdint>
#include <string_view>

namespace gta4::font_selection {
struct Selection {
  uint32_t font;
  uint32_t bank;
};
// sub_821F2ED0, generated gta4_recomp.6.cpp:2390-2541. FONTS.DAT IDs
// are not texture filename suffixes: IDs 0,1,2 address font1,font3,font2.
constexpr Selection DecodeStyle(int32_t style) noexcept {
  switch (style) {
    case 1:
      return {1, 0};
    case 2:
      return {2, 0};
    case 3:
      return {0, 1};
    case 4:
      return {1, 1};
    case 5:
      return {1, 2};
    case 6:
      return {2, 0};
    case 7:
      return {2, 1};
    default:
      return {0, 0};
  }
}
constexpr uint32_t LogicalAtlas(uint32_t font) noexcept {
  return font == 0 ? 1 : font == 1 ? 3 : font == 2 ? 2 : 0;
}
constexpr std::string_view Bank(uint32_t bank) noexcept {
  return bank == 0 ? "main" : bank == 1 ? "sub1" : bank == 2 ? "sub2" : "unknown";
}
constexpr std::string_view Kind(uint32_t caller) noexcept {
  // Only named sites supported by inspected generated callers are classified.
  switch (caller) {
    case 0x8214498C:
      return "legal";
    case 0x82224C20:
      return "help-tutorial";
    case 0x8229E044:
    case 0x8229E200:
    case 0x8229E7C0:
      return "frontend-widget";
    case 0x8225468C:
    case 0x82254A00:
      return "frontend-controls";
    default:
      return "unclassified-caller";
  }
}
constexpr std::string_view ProfileHint(uint32_t episode, uint32_t logical_atlas) noexcept {
  if (logical_atlas == 1)
    return "gta4";
  if (episode == 1 && logical_atlas == 3)
    return "tlad";
  if (episode == 2 && (logical_atlas == 2 || logical_atlas == 3))
    return "tbogt";
  return "gta4";
}
constexpr uint32_t GlyphLookupOffset(uint32_t font, uint32_t bank, uint32_t character) noexcept {
  return (font * 3 + bank) * 256 + (character & 255u);
}
}  // namespace gta4::font_selection
