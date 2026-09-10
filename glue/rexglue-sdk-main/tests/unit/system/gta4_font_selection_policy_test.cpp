#include <array>
#include <catch2/catch_test_macros.hpp>
#include "gta4_font_selection_policy.h"

namespace p = gta4::font_selection;
TEST_CASE("Font style selection follows both retail switch tables", "[font-selection]") {
  constexpr std::array<p::Selection, 8> expected{
      {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {1, 2}, {2, 0}, {2, 1}}};
  for (size_t style = 0; style < expected.size(); ++style) {
    const auto result = p::DecodeStyle(int32_t(style));
    CHECK(result.font == expected[style].font);
    CHECK(result.bank == expected[style].bank);
  }
  for (int32_t value : {-2, -1, 8, 255, 32767}) {
    CHECK(p::DecodeStyle(value).font == 0);
    CHECK(p::DecodeStyle(value).bank == 0);
  }
}
TEST_CASE("Font IDs and filenames are separate namespaces", "[font-selection]") {
  CHECK(p::LogicalAtlas(0) == 1);
  CHECK(p::LogicalAtlas(1) == 3);
  CHECK(p::LogicalAtlas(2) == 2);
  CHECK(p::LogicalAtlas(3) == 0);
  CHECK(p::LogicalAtlas(UINT32_MAX) == 0);
  CHECK(p::Bank(0) == "main");
  CHECK(p::Bank(1) == "sub1");
  CHECK(p::Bank(2) == "sub2");
  CHECK(p::Bank(3) == "unknown");
}
TEST_CASE("Text kind does not guess from a shader or font filename", "[font-selection]") {
  CHECK(p::Kind(0x8214498C) == "legal");
  CHECK(p::Kind(0x82224C20) == "help-tutorial");
  for (uint32_t caller : {0x8229E044u, 0x8229E200u, 0x8229E7C0u})
    CHECK(p::Kind(caller) == "frontend-widget");
  CHECK(p::Kind(0x82144988) == "unclassified-caller");
  CHECK(p::Kind(0) == "unclassified-caller");
}
TEST_CASE("Shared atlas hint does not overwrite episode name fonts", "[font-selection]") {
  for (uint32_t episode : {0u, 1u, 2u})
    CHECK(p::ProfileHint(episode, 1) == "gta4");
  CHECK(p::ProfileHint(1, 2) == "gta4");
  CHECK(p::ProfileHint(1, 3) == "tlad");
  CHECK(p::ProfileHint(2, 2) == "tbogt");
  CHECK(p::ProfileHint(2, 3) == "tbogt");
}
TEST_CASE("Glyph lookup keeps all character bytes and bank ranges isolated", "[font-selection]") {
  for (uint32_t font = 0; font < 3; ++font) {
    for (uint32_t bank = 0; bank < 3; ++bank) {
      const uint32_t base = p::GlyphLookupOffset(font, bank, 0);
      for (uint32_t character = 0; character < 256; ++character) {
        CHECK(p::GlyphLookupOffset(font, bank, character) == base + character);
        CHECK(p::GlyphLookupOffset(font, bank, character + 256) == base + character);
      }
      if (bank < 2)
        CHECK(p::GlyphLookupOffset(font, bank + 1, 0) == base + 256);
    }
  }
}
