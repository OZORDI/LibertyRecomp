#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <unordered_set>
#include <nlohmann/json.hpp>

namespace libserver {

inline bool IsIdentityHex(std::string_view value) {
  return value.size() == 18 && value.substr(0, 2) == "0x" &&
         std::all_of(value.begin() + 2, value.end(), [](unsigned char c) {
           return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
                  (c >= 'A' && c <= 'F');
         });
}
inline std::string CanonicalIdentity(std::string value) {
  if (IsIdentityHex(value)) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
      return static_cast<char>(std::tolower(c));
    });
  }
  return value;
}
inline bool IdentityValueField(std::string_view key) {
  return key == "xuid" || key.ends_with("_xuid") || key == "xuids" ||
         key.ends_with("_xuids") || key == "machine_id" || key == "host_machine_id" ||
         key == "account_id" || key == "session_id" || key.ends_with("_session_id") ||
         key == "nonce" || key == "recipients" || key == "muted" ||
         key == "targets" || key == "muted_xuids";
}

inline bool IdentityMapField(std::string_view field) {
  return field == "player_names" || field == "relationships" || field == "stats" ||
         field == "progression" || field == "mode_stats" || field == "profiles" ||
         field == "title_profile_settings" || field == "prog_ach_records" ||
         field == "achievements" || field == "entitlements" ||
         field == "platform_stat_reports" || field == "ready" ||
         field == "spectators" || field == "kick_votes";
}

// Normalize only structured numeric identities. Text, signatures, device IDs,
// tokens, opaque blobs, and title-specific string values remain byte-exact.
// Reject colliding dictionary keys rather than silently choosing an owner.
inline bool NormalizeIdentities(nlohmann::json& value, std::string& error,
                                std::string_view field = {}, bool durable = false,
                                bool identity_map = false) {
  if (value.is_string()) {
    if (IdentityValueField(field)) {
      value = CanonicalIdentity(value.get<std::string>());
    }
    return true;
  }
  if (value.is_array()) {
    for (auto& item : value) {
      if (!NormalizeIdentities(item, error, field, durable, identity_map)) return false;
    }
    return true;
  }
  if (!value.is_object()) return true;
  nlohmann::json normalized = nlohmann::json::object();
  for (auto& [key, item] : value.items()) {
    std::string canonical = key;
    // An arbitrary title/string dictionary is not an identity map. The
    // relationships map has one further numeric target-key level.
    if ((identity_map || IdentityMapField(field)) && IsIdentityHex(key)) {
      canonical = CanonicalIdentity(key);
    }
    if (durable && (field == "stat_write_receipts" || field == "stat_next_sequences")) {
      std::size_t start = 0;
      for (;;) {
        const auto end = canonical.find(':', start);
        const auto segment = canonical.substr(start, end - start);
        canonical.replace(start, segment.size(), CanonicalIdentity(segment));
        if (end == std::string::npos) break;
        start = end + 1;
      }
    }
    if (normalized.contains(canonical)) {
      error = "numeric identity collision in " + std::string(field);
      return false;
    }
    if (!NormalizeIdentities(item, error, key, durable,
                             field == "relationships")) return false;
    normalized[canonical] = std::move(item);
  }
  value = std::move(normalized);
  return true;
}

// HTTP paths use canonical numeric XUID/session segments, never opaque tokens.
inline std::string CanonicalIdentityPath(std::string path) {
  std::size_t start = 0;
  for (;;) {
    const auto end = path.find('/', start);
    const auto segment = path.substr(start, end - start);
    path.replace(start, segment.size(), CanonicalIdentity(segment));
    if (end == std::string::npos) break;
    start = end + 1;
  }
  return path;
}
}  // namespace libserver
