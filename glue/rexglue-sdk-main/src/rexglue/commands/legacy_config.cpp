/**
 * @file        rexglue/commands/legacy_config.cpp
 * @brief       Legacy single-file config -> manifest conversion
 *
 * @copyright   Copyright (c) 2026 Tom Clay
 * @license     BSD 3-Clause License
 */

#include "legacy_config.h"
#include "template_utils.h"

#include <array>
#include <string_view>
#include <vector>

#include <toml++/toml.hpp>

#include <rex/logging.h>

namespace rexglue::cli {

namespace {

namespace fs = std::filesystem;

std::optional<std::string> ParseSectionHeader(std::string_view line) {
  auto first = line.find_first_not_of(" \t");
  if (first == std::string_view::npos)
    return std::nullopt;
  if (line[first] != '[' || (first + 1 < line.size() && line[first + 1] == '['))
    return std::nullopt;
  auto end = line.find(']', first + 1);
  if (end == std::string_view::npos)
    return std::nullopt;
  return std::string(line.substr(first + 1, end - first - 1));
}

bool LineSetsKey(std::string_view line, std::string_view key) {
  auto first = line.find_first_not_of(" \t");
  if (first == std::string_view::npos)
    return false;
  if (line.compare(first, key.size(), key) != 0)
    return false;
  auto after = first + key.size();
  while (after < line.size() && (line[after] == ' ' || line[after] == '\t'))
    ++after;
  return after < line.size() && line[after] == '=';
}

size_t FindAssignmentEndLine(const std::vector<std::string>& lines, size_t start_line) {
  int depth = 0;
  bool seen_open = false;
  for (size_t j = start_line; j < lines.size(); ++j) {
    bool in_string = false;
    char quote = 0;
    for (size_t k = 0; k < lines[j].size(); ++k) {
      char c = lines[j][k];
      if (in_string) {
        if (c == '\\' && k + 1 < lines[j].size()) {
          ++k;
          continue;
        }
        if (c == quote)
          in_string = false;
        continue;
      }
      if (c == '#')
        break;
      if (c == '"' || c == '\'') {
        in_string = true;
        quote = c;
        continue;
      }
      if (c == '[') {
        ++depth;
        seen_open = true;
      } else if (c == ']') {
        --depth;
      }
    }
    if (!seen_open)
      return j;
    if (depth <= 0)
      return j;
  }
  return lines.size() - 1;
}

std::string RenderTomlString(std::string_view value) {
  std::string out = "\"";
  for (char c : value) {
    switch (c) {
      case '\\':
        out.append("\\\\");
        break;
      case '"':
        out.append("\\\"");
        break;
      case '\n':
        out.append("\\n");
        break;
      case '\t':
        out.append("\\t");
        break;
      default:
        out.push_back(c);
    }
  }
  out.push_back('"');
  return out;
}

std::string RenderKeyString(std::string_view key, std::string_view value) {
  std::string out;
  out.append(key);
  out.append(" = ");
  out.append(RenderTomlString(value));
  return out;
}

std::vector<std::string> SplitLines(std::string_view content) {
  std::vector<std::string> lines;
  std::string buf;
  for (char c : content) {
    if (c == '\n') {
      lines.push_back(std::move(buf));
      buf.clear();
    } else if (c != '\r') {
      buf.push_back(c);
    }
  }
  if (!buf.empty())
    lines.push_back(std::move(buf));
  return lines;
}

}  // namespace

std::optional<LegacyConversion> ConvertLegacyConfig(const fs::path& legacy_path) {
  toml::table tbl;
  try {
    tbl = toml::parse_file(legacy_path.string());
  } catch (const toml::parse_error& err) {
    REXLOG_ERROR("Failed to parse legacy config {}: {}", legacy_path.string(), err.what());
    return std::nullopt;
  }

  auto project_name = tbl["project_name"].value_or<std::string>("");
  if (project_name.empty() || project_name == "rex") {
    return std::nullopt;
  }
  auto file_path = tbl["file_path"].value_or<std::string>("");
  if (file_path.empty()) {
    REXLOG_ERROR("Legacy config {} missing required file_path", legacy_path.string());
    return std::nullopt;
  }
  auto out_directory_path = tbl["out_directory_path"].value_or<std::string>("");
  if (out_directory_path.empty()) {
    out_directory_path = "generated/" + fs::path(file_path).stem().string();
  }

  std::string legacy_content = read_file(legacy_path);
  if (legacy_content.empty()) {
    return std::nullopt;
  }
  std::vector<std::string> lines = SplitLines(legacy_content);

  size_t first_section_idx = lines.size();
  for (size_t i = 0; i < lines.size(); ++i) {
    if (ParseSectionHeader(lines[i])) {
      first_section_idx = i;
      break;
    }
  }

  static constexpr std::array<std::string_view, 5> kManifestEmittedKeys = {
      "project_name", "file_path", "out_directory_path", "template_dir", "includes",
  };

  static constexpr std::array<std::string_view, 2> kRemovedTopLevelKeys = {
      "patch_file_path",
      "patched_file_path",
  };
  for (auto key : kRemovedTopLevelKeys) {
    if (tbl[key].value<std::string>()) {
      REXLOG_WARN(
          "Legacy key '{}' in {} is no longer used. Patching is now auto-detected from "
          "the '<file>p' sibling at codegen time. Dropping during migration.",
          key, legacy_path.string());
    }
  }

  std::string entrypoint_body;
  size_t i = 0;
  while (i < first_section_idx) {
    auto first_nonws = lines[i].find_first_not_of(" \t");
    if (first_nonws == std::string::npos || lines[i][first_nonws] == '#') {
      ++i;
      continue;
    }
    bool emitted_by_manifest = false;
    for (auto key : kManifestEmittedKeys) {
      if (LineSetsKey(lines[i], key)) {
        emitted_by_manifest = true;
        break;
      }
    }
    if (!emitted_by_manifest) {
      for (auto key : kRemovedTopLevelKeys) {
        if (LineSetsKey(lines[i], key)) {
          emitted_by_manifest = true;
          break;
        }
      }
    }
    size_t end = FindAssignmentEndLine(lines, i);
    if (!emitted_by_manifest) {
      for (size_t j = i; j <= end; ++j) {
        auto fn = lines[j].find_first_not_of(" \t");
        if (fn == std::string::npos)
          continue;
        entrypoint_body.append(lines[j]);
        entrypoint_body.push_back('\n');
      }
    }
    i = end + 1;
  }

  std::string stripped;
  for (size_t j = first_section_idx; j < lines.size(); ++j) {
    auto first_nonws = lines[j].find_first_not_of(" \t");
    if (first_nonws == std::string::npos)
      continue;
    if (lines[j][first_nonws] == '#')
      continue;
    if (ParseSectionHeader(lines[j]) && !stripped.empty())
      stripped.push_back('\n');
    stripped.append(lines[j]);
    stripped.push_back('\n');
  }

  std::vector<std::string> entrypoint_includes;
  if (!stripped.empty())
    entrypoint_includes.push_back(legacy_path.filename().string());
  if (auto* arr = tbl["includes"].as_array()) {
    for (const auto& elem : *arr) {
      if (auto s = elem.value<std::string>()) {
        entrypoint_includes.push_back(*s);
      }
    }
  }

  std::string manifest_content;
  manifest_content += "[project]\n";
  manifest_content += RenderKeyString("name", project_name);
  manifest_content += "\n\n[entrypoint]\n";
  manifest_content += RenderKeyString("file_path", file_path);
  manifest_content += '\n';
  manifest_content += RenderKeyString("out_directory_path", out_directory_path);
  manifest_content += '\n';
  if (auto v = tbl["template_dir"].value<std::string>()) {
    manifest_content += RenderKeyString("template_dir", *v);
    manifest_content += '\n';
  }
  if (!entrypoint_includes.empty()) {
    manifest_content += "includes = [\n";
    for (const auto& inc : entrypoint_includes) {
      manifest_content += "  ";
      manifest_content += RenderTomlString(inc);
      manifest_content += ",\n";
    }
    manifest_content += "]\n";
  }
  if (!entrypoint_body.empty()) {
    manifest_content += '\n';
    manifest_content += entrypoint_body;
    manifest_content += '\n';
  }

  LegacyConversion result;
  result.legacy_path = legacy_path;
  result.project_name = project_name;
  result.out_directory_path = out_directory_path;
  auto names = parse_app_name(project_name);
  result.manifest_path = legacy_path.parent_path() / (names.snake_case + "_manifest.toml");
  result.manifest_content = std::move(manifest_content);
  result.stripped_legacy_content = std::move(stripped);
  return result;
}

}  // namespace rexglue::cli
