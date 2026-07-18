/**
 * @file        rexglue/commands/template_utils.h
 * @brief       Shared utilities for init command
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 * @license     BSD 3-Clause License
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include <nlohmann/json.hpp>
#include <rex/logging.h>

namespace rexglue::cli {

struct AppNameParts {
  std::string snake_case;
  std::string pascal_case;
  std::string upper_case;
};

inline bool validate_app_name(const std::string& input, std::string& error) {
  if (input.empty()) {
    error = "App name must not be empty";
    return false;
  }
  if (!std::isalpha(static_cast<unsigned char>(input[0]))) {
    error = "App name must start with a letter";
    return false;
  }
  for (char c : input) {
    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-' && c != ' ') {
      error = "App name contains invalid character '" + std::string(1, c) +
              "'. Only alphanumeric, space, underscore, and dash are allowed";
      return false;
    }
  }
  return true;
}

inline AppNameParts parse_app_name(const std::string& input) {
  std::vector<std::string> words;
  std::string current;
  for (char c : input) {
    if (c == ' ' || c == '_' || c == '-') {
      if (!current.empty()) {
        words.push_back(current);
        current.clear();
      }
    } else {
      current += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
  }
  if (!current.empty()) {
    words.push_back(current);
  }

  AppNameParts parts;
  for (size_t i = 0; i < words.size(); ++i) {
    if (i > 0)
      parts.snake_case += '_';
    parts.snake_case += words[i];
  }
  for (const auto& w : words) {
    std::string word = w;
    if (!word.empty() && std::isalpha(static_cast<unsigned char>(word[0]))) {
      word[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
    }
    parts.pascal_case += word;
  }
  for (const auto& w : words) {
    for (char c : w) {
      parts.upper_case += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
  }
  return parts;
}

inline nlohmann::json names_to_json(const AppNameParts& names) {
  return {{"snake_case", names.snake_case},
          {"pascal_case", names.pascal_case},
          {"upper_case", names.upper_case}};
}

inline bool write_file_atomic(const std::filesystem::path& path, const std::string& content) {
  std::filesystem::path tmp = path;
  tmp += ".tmp";
  {
    std::ofstream out(tmp, std::ios::binary);
    if (!out) {
      REXLOG_ERROR("Failed to open tmp file for write: {}", tmp.string());
      return false;
    }
    out << content;
    if (!out.good()) {
      std::error_code ignore;
      std::filesystem::remove(tmp, ignore);
      REXLOG_ERROR("Failed while writing tmp file: {}", tmp.string());
      return false;
    }
  }
  std::error_code ec;
  std::filesystem::rename(tmp, path, ec);
  if (ec) {
    std::error_code ignore;
    std::filesystem::remove(tmp, ignore);
    REXLOG_ERROR("Failed to rename {} to {}: {}", tmp.string(), path.string(), ec.message());
    return false;
  }
  return true;
}

inline std::string read_file(const std::filesystem::path& path) {
  std::ifstream in(path, std::ios::binary);
  if (!in)
    return {};
  return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

}  // namespace rexglue::cli
