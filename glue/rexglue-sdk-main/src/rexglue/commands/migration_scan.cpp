/**
 * @file        rexglue/commands/migration_scan.cpp
 * @brief       Project-tree scanners for SDK upgrade-driven migrations
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */

#include "migration_scan.h"
#include "template_utils.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <system_error>
#include <unordered_map>

#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <rex/codegen/template_registry.h>

namespace rexglue::cli {

namespace fs = std::filesystem;

namespace {

std::string ToLower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return s;
}

bool IsSourceExtension(const fs::path& p) {
  static const std::unordered_set<std::string> kExts = {".cpp", ".cc",  ".cxx", ".c",  ".h",
                                                        ".hh",  ".hpp", ".hxx", ".inl"};
  return kExts.contains(ToLower(p.extension().string()));
}

bool IsCmakeFile(const fs::path& p) {
  return p.filename() == "CMakeLists.txt" || p.extension() == ".cmake";
}

bool IsUnderGeneratedTree(const fs::path& project_root, const fs::path& target) {
  std::error_code ec;
  auto rel = fs::relative(target, project_root, ec);
  if (ec)
    return false;
  auto first = rel.begin();
  return first != rel.end() && first->generic_string().starts_with("generated");
}

template <typename Accept, typename Visit>
void WalkProjectFiles(const fs::path& project_root, Accept accept, Visit visit) {
  std::error_code ec;
  if (!fs::is_directory(project_root, ec))
    return;
  fs::recursive_directory_iterator it(project_root, fs::directory_options::skip_permission_denied,
                                      ec);
  if (ec)
    return;
  for (fs::recursive_directory_iterator end; it != end; it.increment(ec)) {
    if (ec) {
      ec.clear();
      continue;
    }
    if (it->is_directory()) {
      if (IsUnderGeneratedTree(project_root, it->path()))
        it.disable_recursion_pending();
      continue;
    }
    if (!it->is_regular_file() || !accept(it->path()))
      continue;
    visit(it->path());
  }
}

bool IsPathTokenChar(char c) {
  unsigned char uc = static_cast<unsigned char>(c);
  return std::isalnum(uc) != 0 || c == '_' || c == '-' || c == '.' || c == '/' || c == '\\';
}

bool ReplaceAllPathTokens(std::string& haystack, std::string_view needle,
                          std::string_view replacement) {
  if (needle.empty())
    return false;
  bool replaced = false;
  std::string::size_type pos = 0;
  while ((pos = haystack.find(needle, pos)) != std::string::npos) {
    bool boundary_before = pos == 0 || !IsPathTokenChar(haystack[pos - 1]);
    bool boundary_after =
        pos + needle.size() >= haystack.size() || !IsPathTokenChar(haystack[pos + needle.size()]);
    if (!boundary_before || !boundary_after) {
      pos += needle.size();
      continue;
    }
    haystack.replace(pos, needle.size(), replacement);
    pos += replacement.size();
    replaced = true;
  }
  return replaced;
}

bool IsIdentStart(char c) {
  unsigned char uc = static_cast<unsigned char>(c);
  return std::isalpha(uc) != 0 || c == '_';
}

bool IsIdentChar(char c) {
  unsigned char uc = static_cast<unsigned char>(c);
  return std::isalnum(uc) != 0 || c == '_';
}

std::string ExtractIncludeBasename(std::string_view target) {
  auto pos = target.find_last_of("/\\");
  return std::string(pos == std::string_view::npos ? target : target.substr(pos + 1));
}

constexpr BreakingChangeRule kRules[] = {
    {"PPC_HOOK", "REX_HOOK", ""},
    {"PPC_STUB", "REX_STUB", ""},
    {"PPC_STUB_LOG", "REX_STUB_LOG", ""},
    {"PPC_STUB_RETURN", "REX_STUB_RETURN", ""},
    {"PPC_FUNC", "REX_FUNC", ""},
    {"PPC_WEAK_FUNC", "REX_WEAK_FUNC", ""},
    {"PPC_FUNC_IMPL", "REX_EXTERN", "PPC_FUNC_IMPL was extern \"C\" PPC_FUNC; use REX_EXTERN"},
    {"PPC_EXTERN_IMPORT", "REX_EXTERN",
     "PPC_EXTERN_IMPORT was extern \"C\" PPC_FUNC; use REX_EXTERN"},
    {"PPC_EXTERN_FUNC", "", "removed; declare with extern REX_FUNC(name)"},
    {"PPC_JOIN", "REX_JOIN", ""},
    {"PPC_XSTRINGIFY", "REX_XSTRINGIFY", ""},
    {"PPC_STRINGIFY", "REX_STRINGIFY", ""},
    {"PPC_ROUND_NEAREST", "rex::ppc::kRoundNearest", ""},
    {"PPC_ROUND_TOWARD_ZERO", "rex::ppc::kRoundTowardZero", ""},
    {"PPC_ROUND_UP", "rex::ppc::kRoundUp", ""},
    {"PPC_ROUND_DOWN", "rex::ppc::kRoundDown", ""},
    {"PPC_ROUND_MASK", "rex::ppc::kRoundMask", ""},
    {"PPC_CONFIG_NON_ARGUMENT_AS_LOCAL", "REX_CONFIG_NON_ARGUMENT_AS_LOCAL", ""},
    {"PPC_CONFIG_NON_VOLATILE_AS_LOCAL", "REX_CONFIG_NON_VOLATILE_AS_LOCAL", ""},
    {"PPC_CONFIG_SKIP_LR", "REX_CONFIG_SKIP_LR", ""},
    {"PPC_CONFIG_CTR_AS_LOCAL", "REX_CONFIG_CTR_AS_LOCAL", ""},
    {"PPC_CONFIG_XER_AS_LOCAL", "REX_CONFIG_XER_AS_LOCAL", ""},
    {"PPC_CONFIG_RESERVED_AS_LOCAL", "REX_CONFIG_RESERVED_AS_LOCAL", ""},
    {"PPC_CONFIG_SKIP_MSR", "REX_CONFIG_SKIP_MSR", ""},
    {"PPC_CONFIG_CR_AS_LOCAL", "REX_CONFIG_CR_AS_LOCAL", ""},
    {"PPC_FUNC_PROLOGUE", "REX_FUNC_PROLOGUE", ""},
    {"PPC_CALL_FUNC", "REX_CALL_FUNC", ""},
    {"PPC_LOOKUP_FUNC", "REX_LOOKUP_FUNC", ""},
    {"PPC_CALL_INDIRECT_FUNC", "REX_CALL_INDIRECT_FUNC", ""},
    {"PPC_SET_FLUSH_MODE", "REX_SET_FLUSH_MODE", ""},
    {"PPC_UNIMPLEMENTED", "REX_UNIMPLEMENTED", ""},
    {"PPC_QUERY_TIMEBASE", "REX_QUERY_TIMEBASE", ""},
    {"PPC_CHECK_GLOBAL_LOCK", "REX_CHECK_GLOBAL_LOCK", ""},
    {"PPC_ENTER_GLOBAL_LOCK", "REX_ENTER_GLOBAL_LOCK", ""},
    {"PPC_LEAVE_GLOBAL_LOCK", "REX_LEAVE_GLOBAL_LOCK", ""},
    {"PPC_PHYS_HOST_OFFSET", "REX_PHYS_HOST_OFFSET", ""},
    {"PPC_RAW_ADDR", "REX_RAW_ADDR", ""},
    {"PPC_LOAD_U8", "REX_LOAD_U8", ""},
    {"PPC_LOAD_U16", "REX_LOAD_U16", ""},
    {"PPC_LOAD_U32", "REX_LOAD_U32", ""},
    {"PPC_LOAD_U64", "REX_LOAD_U64", ""},
    {"PPC_LOAD_STRING", "REX_LOAD_STRING", ""},
    {"PPC_STORE_U8", "REX_STORE_U8", ""},
    {"PPC_STORE_U16", "REX_STORE_U16", ""},
    {"PPC_STORE_U32", "REX_STORE_U32", ""},
    {"PPC_STORE_U64", "REX_STORE_U64", ""},
    {"PPC_MEMORY_SIZE", "REX_MEMORY_SIZE", ""},
    {"cache_path", "cache_root", "renamed: cache_path cvar -> cache_root"},
};

bool ConfirmSingleArgAllocateThunk(std::string_view line) {
  auto name_pos = line.find("AllocateThunk");
  if (name_pos == std::string_view::npos)
    return false;
  auto open = line.find('(', name_pos);
  if (open == std::string_view::npos)
    return false;
  int depth = 0;
  bool any_arg = false;
  for (std::size_t i = open; i < line.size(); ++i) {
    char c = line[i];
    if (c == '(') {
      ++depth;
    } else if (c == ')') {
      --depth;
      if (depth == 0)
        return any_arg;
    } else if (c == ',' && depth == 1) {
      return false;
    } else if (depth == 1 && std::isspace(static_cast<unsigned char>(c)) == 0) {
      any_arg = true;
    }
  }
  return false;
}

constexpr CallSiteRule kCallSiteRules[] = {
    {"GetArgument(\"game_directory\")", "removed positional argument 'game_directory'", "",
     nullptr},
    {"AllocateThunk(", "single-arg AllocateThunk call", "second arg is now caller_address",
     ConfirmSingleArgAllocateThunk},
    {"void RenameEntryInternal(", "Entry::RenameEntryInternal return type changed to X_STATUS",
     "override now returns X_STATUS_SUCCESS on success (and a failure code on rename failure)",
     nullptr},
};

using RuleIndex = std::unordered_map<std::string_view, const BreakingChangeRule*>;

RuleIndex BuildRuleIndex(std::span<const BreakingChangeRule> rules) {
  RuleIndex idx;
  idx.reserve(rules.size());
  for (const auto& r : rules) {
    if (!r.legacy_token.empty())
      idx.emplace(r.legacy_token, &r);
  }
  return idx;
}

std::string RenameReason(const BreakingChangeRule& rule) {
  if (!rule.reason.empty())
    return std::string(rule.reason);
  return fmt::format("renamed: {} -> {}", rule.legacy_token, rule.replacement);
}

struct IdentifierScanResult {
  std::string content;
  std::set<std::string> applied_reasons;
};

IdentifierScanResult ApplyIdentifierRules(const fs::path& path, std::string_view content,
                                          const RuleIndex& rules,
                                          std::vector<MigrationWarning>& warnings) {
  IdentifierScanResult result;
  result.content.reserve(content.size());

  std::size_t i = 0;
  std::size_t line_start = 0;
  std::size_t line_number = 1;
  while (i < content.size()) {
    char c = content[i];
    if (c == '\n') {
      result.content.push_back(c);
      ++i;
      ++line_number;
      line_start = i;
      continue;
    }
    if (!IsIdentStart(c)) {
      result.content.push_back(c);
      ++i;
      continue;
    }
    std::size_t start = i;
    while (i < content.size() && IsIdentChar(content[i]))
      ++i;
    std::string_view tok(content.data() + start, i - start);
    auto it = rules.find(tok);
    if (it == rules.end()) {
      result.content.append(tok);
      continue;
    }
    const BreakingChangeRule& rule = *it->second;
    if (!rule.replacement.empty()) {
      result.content.append(rule.replacement);
      result.applied_reasons.insert(RenameReason(rule));
      continue;
    }
    result.content.append(tok);
    std::size_t line_end = content.find('\n', start);
    if (line_end == std::string_view::npos)
      line_end = content.size();
    while (line_end > line_start && content[line_end - 1] == '\r')
      --line_end;
    warnings.push_back({path, line_number,
                        std::string(content.substr(line_start, line_end - line_start)),
                        fmt::format("legacy identifier: {}", tok), std::string(rule.reason)});
  }
  return result;
}

std::string SummarizeRenames(const std::set<std::string>& reasons) {
  static constexpr std::size_t kMaxListed = 3;
  std::string out;
  std::size_t i = 0;
  for (const auto& reason : reasons) {
    if (i == kMaxListed)
      break;
    if (i > 0)
      out += '\n';
    out += reason;
    ++i;
  }
  if (reasons.size() > kMaxListed)
    out += fmt::format("\n(+{} more)", reasons.size() - kMaxListed);
  return out;
}

}  // namespace

std::span<const BreakingChangeRule> DefaultBreakingChangeRules() {
  return {std::begin(kRules), std::end(kRules)};
}

std::span<const CallSiteRule> DefaultCallSiteRules() {
  return {std::begin(kCallSiteRules), std::end(kCallSiteRules)};
}

std::string RenderRexglueCmake(std::string_view project_name, std::string_view sdk_version,
                               std::string_view entrypoint_out_dir) {
  rex::codegen::TemplateRegistry registry;
  auto names = parse_app_name(std::string(project_name));
  nlohmann::json data = {
      {"names", names_to_json(names)},
      {"sdk_version", std::string(sdk_version)},
      {"entrypoint_out_dir", std::string(entrypoint_out_dir)},
  };
  return registry.render("init/rexglue_cmake", data.dump());
}

std::vector<OverwriteEntry> ScanSdkTemplateDrift(const fs::path& project_root,
                                                 std::string_view project_name,
                                                 std::string_view sdk_version,
                                                 std::string_view entrypoint_out_dir) {
  std::vector<OverwriteEntry> plan;
  fs::path rexglue_cmake = project_root / "generated" / "rexglue.cmake";
  std::string rendered = RenderRexglueCmake(project_name, sdk_version, entrypoint_out_dir);
  std::string on_disk = fs::exists(rexglue_cmake) ? read_file(rexglue_cmake) : std::string{};
  if (rendered != on_disk) {
    plan.push_back({rexglue_cmake, std::move(rendered), OverwriteAction::Write, /*silent=*/true,
                    fmt::format("regenerate SDK helper for v{}", sdk_version)});
  }
  return plan;
}

std::vector<OverwriteEntry> ScanCmakeReferences(const fs::path& project_root,
                                                std::string_view legacy_filename,
                                                std::string_view manifest_filename) {
  std::vector<OverwriteEntry> entries;
  if (legacy_filename.empty() || manifest_filename.empty() || legacy_filename == manifest_filename)
    return entries;

  WalkProjectFiles(project_root, IsCmakeFile, [&](const fs::path& path) {
    std::string content = read_file(path);
    if (content.empty())
      return;
    std::string updated = content;
    if (!ReplaceAllPathTokens(updated, legacy_filename, manifest_filename))
      return;
    entries.push_back({path, std::move(updated), OverwriteAction::Write, /*silent=*/false,
                       fmt::format("{} -> {}", legacy_filename, manifest_filename)});
  });
  return entries;
}

std::vector<OverwriteEntry> ScanSourceIncludeRewrites(const fs::path& project_root,
                                                      std::string_view project_name) {
  std::vector<OverwriteEntry> entries;
  if (project_name.empty())
    return entries;

  auto names = parse_app_name(std::string(project_name));
  std::string old_basename_lc = ToLower(names.snake_case + "_config.h");
  std::string new_basename = names.snake_case + "_init.h";
  std::string new_basename_lc = ToLower(new_basename);

  static const std::regex include_re(R"(^(\s*#\s*include\s*[<"])([^>"]+)([>"].*)$)");
  auto extract_target = [&](const std::string& line) -> std::optional<std::string> {
    std::smatch m;
    if (!std::regex_search(line, m, include_re))
      return std::nullopt;
    return m[2].str();
  };

  WalkProjectFiles(project_root, IsSourceExtension, [&](const fs::path& path) {
    std::string content = read_file(path);
    if (content.empty())
      return;

    std::vector<std::string> lines;
    std::string buf;
    for (char c : content) {
      if (c == '\n') {
        lines.push_back(std::move(buf));
        buf.clear();
      } else {
        buf.push_back(c);
      }
    }
    if (!buf.empty())
      lines.push_back(std::move(buf));
    bool has_trailing_newline = !content.empty() && content.back() == '\n';

    bool has_init_include = false;
    for (const auto& line : lines) {
      auto target = extract_target(line);
      if (target && ToLower(ExtractIncludeBasename(*target)) == new_basename_lc) {
        has_init_include = true;
        break;
      }
    }

    bool changed = false;
    std::string out;
    out.reserve(content.size());
    for (std::size_t li = 0; li < lines.size(); ++li) {
      const std::string& line = lines[li];
      auto target = extract_target(line);
      auto append_line = [&](std::string_view payload) {
        out.append(payload);
        if (li + 1 < lines.size() || has_trailing_newline)
          out.push_back('\n');
      };

      if (!target || ToLower(ExtractIncludeBasename(*target)) != old_basename_lc) {
        append_line(line);
        continue;
      }
      if (has_init_include) {
        changed = true;
        continue;
      }
      std::smatch m;
      if (!std::regex_search(line, m, include_re)) {
        append_line(line);
        continue;
      }
      std::string prefix = m[1].str();
      std::string old_target = m[2].str();
      std::string suffix = m[3].str();
      std::string base = ExtractIncludeBasename(old_target);
      std::string new_target = old_target;
      new_target.replace(new_target.size() - base.size(), base.size(), new_basename);
      append_line(prefix + new_target + suffix);
      has_init_include = true;
      changed = true;
    }

    if (!changed)
      return;
    entries.push_back(
        {path, std::move(out), OverwriteAction::Write, /*silent=*/false,
         fmt::format("{}_config.h -> {}_init.h", names.snake_case, names.snake_case)});
  });
  return entries;
}

MigrationFindings ScanLegacyIdentifiers(const fs::path& project_root,
                                        std::span<const BreakingChangeRule> rules) {
  MigrationFindings out;
  RuleIndex index = BuildRuleIndex(rules);
  if (index.empty())
    return out;

  WalkProjectFiles(project_root, IsSourceExtension, [&](const fs::path& path) {
    std::string content = read_file(path);
    if (content.empty())
      return;

    std::size_t warnings_before = out.warnings.size();
    auto result = ApplyIdentifierRules(path, content, index, out.warnings);
    if (result.applied_reasons.empty())
      return;
    std::string reason = SummarizeRenames(result.applied_reasons);
    if (auto warning_count = out.warnings.size() - warnings_before; warning_count > 0) {
      reason += fmt::format("\n(+{} warn-only)", warning_count);
    }
    out.rewrites.push_back(
        {path, std::move(result.content), OverwriteAction::Write, /*silent=*/false, reason});
  });
  return out;
}

std::vector<MigrationWarning> ScanCallSitePatterns(const fs::path& project_root,
                                                   std::span<const CallSiteRule> rules) {
  std::vector<MigrationWarning> out;
  if (rules.empty())
    return out;

  WalkProjectFiles(project_root, IsSourceExtension, [&](const fs::path& path) {
    std::string content = read_file(path);
    if (content.empty())
      return;

    std::size_t line_start = 0;
    std::size_t line_number = 1;
    for (std::size_t i = 0; i <= content.size(); ++i) {
      bool eol = i == content.size() || content[i] == '\n';
      if (!eol)
        continue;
      std::size_t line_end = i;
      while (line_end > line_start && content[line_end - 1] == '\r')
        --line_end;
      std::string_view line(content.data() + line_start, line_end - line_start);
      for (const auto& rule : rules) {
        if (line.find(rule.pattern) == std::string_view::npos)
          continue;
        if (rule.confirm && !rule.confirm(line))
          continue;
        out.push_back({path, line_number, std::string(line), std::string(rule.detail),
                       std::string(rule.hint)});
      }
      line_start = i + 1;
      ++line_number;
    }
  });
  return out;
}

std::vector<MigrationWarning> ScanStaleIncludes(
    const fs::path& src_dir, const std::unordered_set<std::string>& removed_basenames) {
  std::vector<MigrationWarning> matches;
  if (!fs::is_directory(src_dir))
    return matches;

  std::unordered_set<std::string> removed_lower;
  removed_lower.reserve(removed_basenames.size());
  for (const auto& name : removed_basenames)
    removed_lower.insert(ToLower(name));

  static const std::regex include_re(R"(^\s*#\s*include\s*[<"]([^>"]+)[>"])");

  for (const auto& entry : fs::recursive_directory_iterator(src_dir)) {
    if (!entry.is_regular_file() || !IsSourceExtension(entry.path()))
      continue;
    std::ifstream in(entry.path());
    if (!in)
      continue;
    std::string line;
    std::size_t line_number = 0;
    while (std::getline(in, line)) {
      ++line_number;
      std::smatch m;
      if (!std::regex_search(line, m, include_re))
        continue;
      std::string target = m[1].str();
      auto pos = target.find_last_of("/\\");
      std::string basename = pos == std::string::npos ? target : target.substr(pos + 1);
      if (!removed_lower.contains(ToLower(basename)))
        continue;
      matches.push_back({entry.path(), line_number, line, fmt::format("stale include: {}", target),
                         "header no longer emitted by codegen; update or remove the include"});
    }
  }

  std::sort(matches.begin(), matches.end(), [](const auto& a, const auto& b) {
    if (a.file != b.file)
      return a.file < b.file;
    return a.line_number < b.line_number;
  });
  return matches;
}

}  // namespace rexglue::cli
