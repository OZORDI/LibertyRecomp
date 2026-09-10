#include "gta4_font_selection_trace.h"
#include "gta4_font_selection_policy.h"

#include <array>
#include <atomic>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

#include <nlohmann/json.hpp>
#include <xxhash.h>
#include <rex/cvar.h>
#include <rex/diagnostics/policy.h>
#include <rex/filesystem.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include "gta4_init.h"

REXCVAR_DEFINE_BOOL(gta4_trace_font_selection, false, "GTA IV/Diagnostics",
                    "Bounded text-role, actual glyph-cell, and font texture binding observations")
    .lifecycle(rex::cvar::Lifecycle::kRequiresRestart);

namespace {
namespace policy = gta4::font_selection;
constexpr uint32_t kStateIndex = 0x82A935A4;
constexpr uint32_t kStates = 0x82B9A118;
constexpr uint32_t kGlyphState = 0x82A94478;
constexpr uint32_t kLookup = 0x82B990C0;
constexpr uint32_t kDescriptors = 0x82B999C0;
constexpr uint32_t kEpisode = 0x82B39504;
std::atomic<uint64_t> next_event{1};
thread_local uint64_t active_glyph = 0;

bool Enabled() {
  return REXCVAR_GET(gta4_trace_font_selection) &&
         rex::diagnostics::IsEnabled(rex::diagnostics::Category::kLogging);
}
bool Readable(uint8_t* base, uint32_t address, size_t bytes) {
  if (!base || !address || !bytes || uint64_t(address) + bytes > uint64_t(UINT32_MAX) + 1)
    return false;
  auto* kernel = REX_KERNEL_STATE();
  auto* memory = kernel ? kernel->memory() : nullptr;
  auto* heap = memory ? memory->LookupHeap(address) : nullptr;
  const uint32_t last = uint32_t(uint64_t(address) + bytes - 1);
  if (!heap || heap != memory->LookupHeap(last))
    return false;
  const auto access = heap->QueryRangeAccess(address, last);
  using rex::memory::PageAccess;
  return access == PageAccess::kReadOnly || access == PageAccess::kReadWrite ||
         access == PageAccess::kExecuteReadOnly || access == PageAccess::kExecuteReadWrite;
}
uint32_t Read32(uint8_t* base, uint32_t address) {
  uint32_t value = 0;
  std::memcpy(&value, rex::memory::GuestPtr(base, address), sizeof(value));
  return __builtin_bswap32(value);
}
uint8_t Read8(uint8_t* base, uint32_t address) {
  return *rex::memory::GuestPtr(base, address);
}
uint32_t Episode(uint8_t* base) {
  return Readable(base, kEpisode, 4) ? Read32(base, kEpisode) : UINT32_MAX;
}
uint32_t Owner(uint8_t* base, uint32_t font) {
  if (font > 2)
    return 0;
  const uint32_t address = kDescriptors + font * 500 + 476;
  return Readable(base, address, 4) ? Read32(base, address) : 0;
}
uint32_t Frame(uint8_t* base) {
  constexpr uint32_t device_slot = 0x831C22A4;
  if (!Readable(base, device_slot, 4))
    return 0;
  const uint32_t device = Read32(base, device_slot);
  if (device > UINT32_MAX - 16544 || !Readable(base, device + 16544, 4))
    return 0;
  return Read32(base, device + 16544);
}
struct Budget {
  std::mutex mutex;
  std::unordered_set<std::string> seen;
  bool exhausted = false;
  bool Take(std::string key, size_t limit, const char* point) {
    std::lock_guard lock(mutex);
    if (seen.contains(key))
      return false;
    if (seen.size() >= limit) {
      if (!exhausted) {
        exhausted = true;
        REXLOG_INFO("gta4-font-selection: point=limit kind={} limit={} coverage=partial", point,
                    limit);
      }
      return false;
    }
    seen.insert(std::move(key));
    return true;
  }
};
Budget texts, glyphs, bindings, selections;
struct FontState {
  uint32_t address = 0, channel = UINT32_MAX, font = UINT32_MAX, bank = UINT32_MAX;
  bool valid = false;
};
FontState SubmissionState(const PPCContext& ctx, uint8_t* base) {
  FontState state;
  if (!Readable(base, kStateIndex, 4))
    return state;
  state.channel = Read32(base, kStateIndex);
  if (state.channel == UINT32_MAX) {
    if (!Readable(base, ctx.r13.u32, 4))
      return state;
    const uint32_t tls = Read32(base, ctx.r13.u32);
    if (tls > UINT32_MAX - 8 || !Readable(base, tls + 8, 4))
      return state;
    state.channel = (Read32(base, tls + 8) >> 2) & 1;
  }
  if (state.channel > 1)
    return state;
  state.address = kStates + state.channel * 68;
  if (!Readable(base, state.address, 68))
    return state;
  state.font = Read8(base, state.address + 40);
  state.bank = Read8(base, state.address + 41);
  state.valid = state.font <= 2 && state.bank <= 2;
  return state;
}
struct Face {
  uint32_t first = 0, last = 0;
  std::string filename, sha;
  std::unordered_set<uint32_t> fallback;
};
struct Atlas {
  bool valid = false;
  std::string file;
  uint64_t png_hash = 0;
  std::vector<Face> faces;
};
const Atlas& Manifest(std::string_view profile, uint32_t logical) {
  static const auto all = [] {
    std::array<std::array<Atlas, 3>, 3> result{};
    const auto root =
        rex::filesystem::GetExecutableFolder().parent_path() / "Resources/font_atlases";
    const std::array<std::string, 3> profiles = {"gta4", "tlad", "tbogt"};
    for (size_t p = 0; p < profiles.size(); ++p) {
      const auto folder = p ? root / profiles[p] : root;
      try {
        std::ifstream stream(folder / "manifest.json");
        const auto doc = nlohmann::json::parse(stream);
        for (size_t index = 0; index < 3; ++index) {
          auto& item = result[p][index];
          const auto name = "font" + std::to_string(index + 1);
          const auto& definition = doc.at("atlases").at(name);
          item.file = (folder / definition.at("file").get<std::string>()).string();
          std::ifstream png(item.file, std::ios::binary);
          std::vector<char> bytes{std::istreambuf_iterator<char>(png),
                                  std::istreambuf_iterator<char>()};
          if (bytes.empty())
            continue;
          item.png_hash = XXH3_64bits(bytes.data(), bytes.size());
          for (const auto& value : definition.at("vector_ranges")) {
            Face face;
            face.first = value.at("first_cell").get<uint32_t>();
            face.last = value.at("last_cell_exclusive").get<uint32_t>();
            face.filename = value.at("font").get<std::string>();
            face.sha = value.value("font_sha256", "not-recorded");
            for (auto cell : value.at("preserved_edge_cells"))
              face.fallback.insert(cell.get<uint32_t>());
            item.faces.push_back(std::move(face));
          }
          item.valid = true;
          REXLOG_INFO(
              "gta4-font-selection: point=atlas-manifest profile={} atlas={} path={} "
              "png-hash={:016X} alpha-source=renderer-atlas-load-log",
              profiles[p], name, item.file, item.png_hash);
        }
      } catch (const std::exception& error) {
        REXLOG_WARN("gta4-font-selection: point=manifest-unavailable profile={} reason={}",
                    profiles[p], error.what());
      }
    }
    return result;
  }();
  static const Atlas missing;
  if (logical < 1 || logical > 3)
    return missing;
  const size_t p = profile == "tlad" ? 1 : profile == "tbogt" ? 2 : 0;
  return all[p][logical - 1];
}
struct GlyphScope {
  uint64_t previous;
  explicit GlyphScope(uint64_t id) : previous(active_glyph) { active_glyph = id; }
  ~GlyphScope() { active_glyph = previous; }
};
}  // namespace

void GTA4_FontSelectionTraceText(const PPCContext& ctx, uint8_t* base) {
  if (!Enabled())
    return;
  const auto state = SubmissionState(ctx, base);
  const uint32_t address = ctx.r5.u32;
  if (!state.valid || !Readable(base, address, 256))
    return;
  std::string raw, preview;
  bool terminated = false;
  for (uint32_t i = 0; i < 256; ++i) {
    const uint8_t c = Read8(base, address + i);
    if (!c) {
      terminated = true;
      break;
    }
    raw.push_back(char(c));
    if (i >= 96)
      continue;
    if (c == '"' || c == '\\')
      preview.push_back('\\');
    if (c >= 32 && c < 127)
      preview.push_back(char(c));
    else
      preview += fmt::format("\\x{:02X}", c);
  }
  if (raw.empty())
    return;
  const uint64_t hash = XXH3_64bits(raw.data(), raw.size());
  const uint32_t caller = ctx.lr;
  if (!texts.Take(fmt::format("{:08X}:{}:{}:{:016X}", caller, state.font, state.bank, hash), 384,
                  "text-submit"))
    return;
  REXLOG_INFO(
      "gta4-font-selection: point=text-submit event={} frame={} kind={} caller={:08X} "
      "channel={} state={:08X} font-id={} atlas=font{} bank={} owner={:08X} "
      "episode={} prefix-hash={:016X} prefix-bytes={} terminated={} text=\"{}\" "
      "evidence=actual-submission-state inline-font-changes-possible=true gpu-verified=false",
      next_event.fetch_add(1), Frame(base), policy::Kind(caller), caller, state.channel,
      state.address, state.font, policy::LogicalAtlas(state.font), policy::Bank(state.bank),
      Owner(base, state.font), Episode(base), hash, raw.size(), terminated, preview);
}

extern "C" void sub_821F2ED0(PPCContext& ctx, uint8_t* base) {
  if (!Enabled()) {
    __imp__sub_821F2ED0(ctx, base);
    return;
  }
  const int32_t requested = ctx.r3.s32;
  const uint32_t caller = ctx.lr;
  __imp__sub_821F2ED0(ctx, base);
  const auto state = SubmissionState(ctx, base);
  if (!state.valid ||
      !selections.Take(fmt::format("{:08X}:{}:{}:{}", caller, requested, state.font, state.bank),
                       128, "style-select"))
    return;
  const auto expected = policy::DecodeStyle(requested);
  REXLOG_INFO(
      "gta4-font-selection: point=style-select caller={:08X} requested={} font-id={} "
      "atlas=font{} bank={} policy-match={} evidence=post-retail-setter",
      caller, requested, state.font, policy::LogicalAtlas(state.font), policy::Bank(state.bank),
      state.font == expected.font && state.bank == expected.bank);
}

extern "C" void sub_821F1EE0(PPCContext& ctx, uint8_t* base) {
  if (!Enabled() || !Readable(base, kGlyphState, 48)) {
    __imp__sub_821F1EE0(ctx, base);
    return;
  }
  const uint32_t font = Read8(base, kGlyphState + 37);
  const uint32_t bank = Read8(base, kGlyphState + 33);
  const uint32_t symbol = Read32(base, kGlyphState + 44);
  const uint32_t character = ctx.r5.u32 & 255;
  const uint32_t episode = Episode(base);
  const auto logical = policy::LogicalAtlas(font);
  // r5 contains the character minus 32 (wrapped to a byte), not a glyph ID.
  const uint32_t lookup = kLookup + policy::GlyphLookupOffset(font, bank, character);
  uint32_t cell = UINT32_MAX;
  if (font <= 2 && bank <= 2 && !symbol && Readable(base, lookup, 1)) {
    cell = Read8(base, lookup);
    if (!Read8(base, kGlyphState + 36) && character == 63)
      cell = 208;
  }
  const uint64_t id = next_event.fetch_add(1, std::memory_order_relaxed);
  const GlyphScope scope(id);
  if (glyphs.Take(fmt::format("{}:{}:{}:{}:{}", episode, font, bank, cell, symbol), 1024,
                  "glyph-dispatch")) {
    const auto profile = policy::ProfileHint(episode, logical);
    const auto& atlas = Manifest(profile, logical);
    std::string face = symbol ? "controller-or-special-symbol" : "stock-contour-or-unmapped-cell";
    std::string font_sha = "none";
    bool fallback = false;
    for (const auto& candidate : atlas.faces) {
      if (cell < candidate.first || cell >= candidate.last)
        continue;
      fallback = candidate.fallback.contains(cell);
      face = fallback ? "stock-contour-fallback" : candidate.filename;
      font_sha = fallback ? "none" : candidate.sha;
      break;
    }
    REXLOG_INFO(
        "gta4-font-selection: point=glyph-dispatch event={} frame={} episode={} "
        "font-id={} atlas=font{} bank={} cell={} codepoint-byte={} symbol={} "
        "owner={:08X} profile-hint={} manifest-valid={} atlas-png-hash={:016X} "
        "source-face={} source-sha256={} stock-fallback={} evidence=retail-glyph-lookup "
        "gpu-verified=false",
        id, Frame(base), episode, font, logical, policy::Bank(bank), cell, (character + 32) & 255u,
        symbol, Owner(base, font), profile, atlas.valid, atlas.png_hash, face, font_sha, fallback);
  }
  __imp__sub_821F1EE0(ctx, base);
}

void GTA4_FontSelectionTraceBinding(uint32_t logical, uint32_t owner, uint32_t texture,
                                    uint32_t stage) {
  if (!Enabled() || !logical)
    return;
  if (!bindings.Take(fmt::format("{}:{:08X}:{:08X}:{}", logical, owner, texture, stage), 128,
                     "texture-binding"))
    return;
  REXLOG_INFO(
      "gta4-font-selection: point=texture-binding glyph-event={} atlas=font{} "
      "owner={:08X} texture={:08X} stage={} evidence=actual-native-set-texture "
      "gpu-image-source=vector-font-capture-log",
      active_glyph, logical, owner, texture, stage);
}
