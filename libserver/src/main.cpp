#include <algorithm>
#include <array>
#include <atomic>
#include <charconv>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "http_runtime.h"
#include "gta4_stat_schema.h"
#include "multiplayer_64_vectors.h"
#include "persistent_state.h"
#include "identity_contract.h"
#include "deferred_responses.h"
#include "qos_probe_state.h"

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>

#include <nlohmann/json.hpp>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace {

using json = nlohmann::json;
using Clock = std::chrono::steady_clock;
using SystemClock = std::chrono::system_clock;
using libserver::http::Request;
using libserver::http::Socket;
using libserver::http::Connection;

constexpr std::size_t kMaximumSessionMembers = 64;
// One local sender leaves this many remote destinations in a full session.
constexpr std::size_t kMaximumRemoteSessionMembers = 63;
constexpr std::size_t kMaximumRelayDatagramBytes = 65507;
constexpr std::size_t kMaximumRelayBatchBytes = 1048576;
constexpr std::size_t kMaximumProfileClothingItems = 32;
constexpr std::size_t kMaximumMatchmakingFilterEntries = 64;
constexpr std::size_t kMaximumPropertyBytes = 512;
constexpr std::size_t kMaximumStatViews = 16;
constexpr std::size_t kMaximumStatColumnsPerRow = 64;
constexpr std::size_t kMaximumStatReadXuids = 100;
constexpr std::size_t kMaximumStatValueJsonBytes = 2048;
constexpr std::size_t kMaximumStatUnicodeBytes = 1024;
constexpr std::size_t kMaximumStatBinaryBytes = 512;
constexpr std::size_t kMaximumInviteRecipients = 32;
constexpr std::size_t kOversizeInviteRecipients = 33;
constexpr std::size_t kMaximumInviteCustomBytes = 512;
constexpr std::size_t kOversizeInviteCustomEncodedBytes = 684;
constexpr std::size_t kMaximumVoicePayloadBytes = 4096;
constexpr std::size_t kMaximumVoiceEncodedPayloadBytes = 5464;
constexpr std::size_t kMaximumVoiceEncodedPayloadPrefixBytes = 5462;
// Canonical wire size for RandomToken("voice_", 16).
constexpr std::size_t kVoiceRouteTokenCharacters = 38;
constexpr std::size_t kMaximumVoicePacketsPerBatch = 32;
constexpr std::size_t kOversizeVoicePacketsPerBatch = 33;
constexpr std::size_t kMaximumVoiceQueuedPackets = 512;
constexpr std::size_t kMaximumVoiceQueueEncodedBytes = 2797568;
constexpr std::size_t kVoiceQueueFillBatches = 16;
constexpr std::size_t kMaximumTextChatBytes = 256;
constexpr std::size_t kOversizeTextChatBytes = 257;
constexpr std::size_t kMaximumTextChatEventsPerPage = 256;
constexpr std::size_t kMaximumQueuedTextChatEvents = 256;
constexpr std::uint64_t kMaximumTextChatWaitMilliseconds = 30000;
// Derived and boundary-checked by tools/derive_qos_limits.py.
constexpr std::size_t kQosTitleDataBytes = 12;
constexpr std::size_t kQosTitleDataEncodedBytes = 16;
constexpr std::size_t kMaximumQosTargets = 64;
constexpr std::size_t kOversizeQosTargets = 65;
constexpr std::size_t kQosChallengeHexDigits = 32;
constexpr std::size_t kQosChallengeHexCharacters = 34;
// Derived and boundary-checked by tools/derive_resource_leases.py.
constexpr auto kSweepInterval = std::chrono::seconds(1);
constexpr auto kRelayRouteLease = std::chrono::seconds(60);
constexpr auto kVoiceRouteLease = std::chrono::seconds(60);
constexpr auto kVoicePacketLifetime = kVoiceRouteLease;
constexpr auto kPresenceLease = std::chrono::seconds(90);
constexpr auto kSessionLease = std::chrono::seconds(120);
constexpr auto kChallengeLifetime = std::chrono::seconds(120);
constexpr auto kMatchmakingTicketLease = std::chrono::seconds(300);
constexpr auto kAccessTokenLifetime = std::chrono::seconds(3600);
constexpr auto kRefreshTokenLifetime = std::chrono::seconds(2592000);
constexpr auto kInviteLifetime = std::chrono::seconds(604800);
// Session mutation receipts are process-local, just like the sessions they protect. Keep them
// through a full access-token lifetime so a refreshed transport can recover a lost response,
// while the sweeper still bounds receipts from clients that never retry.
constexpr auto kSessionMutationReceiptLifetime = kAccessTokenLifetime;
constexpr std::size_t kSha256Bytes = 32;
constexpr std::size_t kSha256HexCharacters = 64;
constexpr std::uint64_t kLegacyDurablePayloadSchema = 1;
constexpr std::uint64_t kAchievementDurablePayloadSchema = 2;
constexpr std::uint64_t kEntitlementDurablePayloadSchema = 3;
constexpr std::uint64_t kProfileDurablePayloadSchema = 4;
constexpr std::uint64_t kDurablePayloadSchema = 5;
// Derived and checked by tools/derive_social_contract.py.
constexpr std::size_t kMaximumFriendsPageItems = 100;
constexpr std::size_t kMaximumFriendsCheckItems = 100;
constexpr std::size_t kOversizeFriendsCheckItems = 101;
constexpr std::size_t kMaximumIdempotencyKeyBytes = 128;
// SessionLifecycleState::kDeleted is the fifth and final active-client lifecycle value.
constexpr std::uint32_t kMaximumSessionLifecycleState = 4;
// XSession flags observed in the active GTA IV contract.
constexpr std::uint32_t kSessionFlagUsesArbitration = 0x10;
constexpr std::uint32_t kSessionFlagUsesPresence = 0x2;
constexpr std::uint32_t kSessionFlagUsesMatchmaking = 0x8;
constexpr std::uint32_t kSessionFlagJoinViaPresenceDisabled = 0x200;
constexpr std::uint32_t kSessionFlagJoinInProgressDisabled = 0x400;
// GTA IV's generated XSessionArbitrationRegister wrapper hard-codes both.
constexpr std::uint32_t kGta4ArbitrationRegistrationSeconds = 300;
constexpr std::uint32_t kGta4ArbitrationFlags = 0;
// XSESSION_REGISTRANT has room for four local XUIDs per machine. Accepting a
// larger group would produce a roster that the title's fixed 3592-byte result
// buffer cannot consume.
constexpr std::size_t kGta4ArbitrationUsersPerMachine = 4;
// Derived and checked by tools/derive_prog_ach_contract.py.
constexpr std::size_t kGta4ProgAchBytes = 604;
constexpr std::size_t kGta4ProgAchBase64UrlCharacters = 806;
constexpr std::size_t kGta4ProgAchShortBase64UrlCharacters = 804;
constexpr std::size_t kGta4ProgAchLongBase64UrlCharacters = 807;
// Derived and checked by tools/derive_gta4_profile_contract.py.
constexpr std::size_t kMaximumGta4TitleProfileBytes = 1000;
constexpr std::size_t kGta4TitleProfileRecordBytes = 8;
constexpr std::uint32_t kFirstGta4AchievementId = 1;
constexpr std::uint32_t kLastGta4AchievementId = 65;
constexpr std::size_t kGta4AchievementCount = 65;
constexpr std::array<std::string_view, 2> kKnownGta4EpisodePackages = {"TLAD", "TBOGT"};
constexpr std::array<std::int64_t, 11> kRankCashThresholds = {
    0, 1000, 10000, 50000, 100000, 250000, 500000, 750000, 1000000, 2500000, 5000000};

volatile std::sig_atomic_t g_stopping = 0;

enum class TransportMode {
  kPlaintextLoopback,
  kBehindProxy,
  kDirectTls,
};

std::string Lower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return value;
}

bool IsValidListenAddress(std::string_view text, bool& loopback) {
  in_addr address{};
  const std::string value(text);
  if (inet_pton(AF_INET, value.c_str(), &address) != 1) return false;
  const auto* bytes = reinterpret_cast<const unsigned char*>(&address.s_addr);
  loopback = bytes[0] == 127;
  return true;
}

bool IsValidPublicUrlHost(std::string_view host) {
  if (host.empty()) return false;
  if (host.front() == '[') {
    if (host.size() < 3 || host.back() != ']') return false;
    std::array<unsigned char, 16> binary{};
    const std::string address(host.substr(1, host.size() - 2));
    return inet_pton(AF_INET6, address.c_str(), binary.data()) == 1;
  }
  if (host.front() == '.' || host.back() == '.' || host.front() == '-' ||
      host.back() == '-') {
    return false;
  }
  bool label_start = true;
  unsigned char previous = 0;
  for (const unsigned char character : host) {
    if (character == '.') {
      if (label_start || previous == '-') return false;
      label_start = true;
      previous = character;
      continue;
    }
    if (!std::isalnum(character) && character != '-') return false;
    if (label_start && character == '-') return false;
    label_start = false;
    previous = character;
  }
  return !label_start && previous != '-';
}

bool IsValidPublicUrl(std::string_view text, bool require_https, bool require_http) {
  std::string_view authority;
  if (text.starts_with("https://")) {
    if (require_http) return false;
    authority = text.substr(std::string_view("https://").size());
  } else if (text.starts_with("http://")) {
    if (require_https) return false;
    authority = text.substr(std::string_view("http://").size());
  } else {
    return false;
  }
  if (authority.ends_with('/')) authority.remove_suffix(1);
  if (authority.empty() || authority.find_first_of("/?#@ 	\r\n") !=
                               std::string_view::npos) {
    return false;
  }

  std::string_view host = authority;
  std::string_view port;
  if (authority.front() == '[') {
    const std::size_t bracket = authority.find(']');
    if (bracket == std::string_view::npos) return false;
    host = authority.substr(0, bracket + 1);
    const std::string_view suffix = authority.substr(bracket + 1);
    if (!suffix.empty()) {
      if (!suffix.starts_with(':') || suffix.size() == 1) return false;
      port = suffix.substr(1);
    }
  } else {
    const std::size_t colon = authority.find(':');
    if (colon != std::string_view::npos) {
      if (authority.find(':', colon + 1) != std::string_view::npos || colon == 0 ||
          colon + 1 == authority.size()) {
        return false;
      }
      host = authority.substr(0, colon);
      port = authority.substr(colon + 1);
    }
  }
  if (!IsValidPublicUrlHost(host)) return false;
  if (!port.empty()) {
    unsigned value = 0;
    const auto parsed = std::from_chars(port.data(), port.data() + port.size(), value);
    if (parsed.ec != std::errc{} || parsed.ptr != port.data() + port.size() || value == 0 ||
        value > std::numeric_limits<std::uint16_t>::max()) {
      return false;
    }
  }
  return true;
}

std::string RandomToken(std::string_view prefix, std::size_t bytes = 24) {
  static constexpr char kHex[] = "0123456789abcdef";
  std::vector<unsigned char> random(bytes);
  if (RAND_bytes(random.data(), static_cast<int>(random.size())) != 1) return {};
  std::string result(prefix);
  result.reserve(prefix.size() + random.size() * 2);
  for (const unsigned char value : random) {
    result.push_back(kHex[value >> 4]);
    result.push_back(kHex[value & 15]);
  }
  return result;
}

std::optional<std::vector<unsigned char>> DecodeBase64Url(std::string text) {
  std::replace(text.begin(), text.end(), '-', '+');
  std::replace(text.begin(), text.end(), '_', '/');
  while (text.size() % 4 != 0) text.push_back('=');
  std::vector<unsigned char> output((text.size() / 4) * 3);
  const int decoded = EVP_DecodeBlock(output.data(),
                                      reinterpret_cast<const unsigned char*>(text.data()),
                                      static_cast<int>(text.size()));
  if (decoded < 0) return std::nullopt;
  std::size_t padding = 0;
  if (!text.empty() && text.back() == '=') ++padding;
  if (text.size() > 1 && text[text.size() - 2] == '=') ++padding;
  output.resize(static_cast<std::size_t>(decoded) - padding);
  return output;
}

bool VerifyEd25519(std::string_view public_key, std::string_view message,
                   std::string_view signature) {
  auto key = DecodeBase64Url(std::string(public_key));
  auto sig = DecodeBase64Url(std::string(signature));
  if (!key || !sig || key->size() != 32 || sig->size() != 64) return false;
  EVP_PKEY* pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, nullptr, key->data(), key->size());
  if (!pkey) return false;
  EVP_MD_CTX* context = EVP_MD_CTX_new();
  const bool valid = context && EVP_DigestVerifyInit(context, nullptr, nullptr, nullptr, pkey) == 1 &&
                     EVP_DigestVerify(context, sig->data(), sig->size(),
                                      reinterpret_cast<const unsigned char*>(message.data()),
                                      message.size()) == 1;
  if (context) EVP_MD_CTX_free(context);
  EVP_PKEY_free(pkey);
  return valid;
}

struct Response {
  int status = 200;
  json body = json::object();
};

std::string StatusText(int status) {
  switch (status) {
    case 200: return "OK";
    case 201: return "Created";
    case 204: return "No Content";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 408: return "Request Timeout";
    case 409: return "Conflict";
    case 412: return "Precondition Failed";
    case 413: return "Payload Too Large";
    case 414: return "URI Too Long";
    case 431: return "Request Header Fields Too Large";
    case 429: return "Too Many Requests";
    case 202: return "Accepted";
    case 501: return "Not Implemented";
    case 503: return "Service Unavailable";
    case 505: return "HTTP Version Not Supported";
    default: return "Internal Server Error";
  }
}

Response Error(int status, std::string code, std::string message) {
  return {.status = status,
          .body = {{"error", {{"code", std::move(code)}, {"message", std::move(message)}}}}};
}

std::string EncodeResponse(const Response& response) {
  const std::string body = response.status == 204 ? std::string{} : response.body.dump();
  std::ostringstream stream;
  stream << "HTTP/1.1 " << response.status << ' ' << StatusText(response.status) << "\r\n"
         << "Content-Type: application/json\r\n"
         << "Content-Length: " << body.size() << "\r\n"
         << "Cache-Control: no-store\r\n"
         << "X-Content-Type-Options: nosniff\r\n"
         << "Connection: close\r\n\r\n" << body;
  return stream.str();
}

bool SendResponse(Connection& connection, const Response& response) {
  return libserver::http::SendAll(connection, EncodeResponse(response));
}

std::optional<json> ParseBody(const Request& request) {
  try {
    json body = request.body.empty() ? json::object() : json::parse(request.body);
    std::string error;
    if (!libserver::NormalizeIdentities(body, error)) return std::nullopt;
    return body;
  } catch (...) {
    return std::nullopt;
  }
}

bool IsHex(std::string_view text, std::size_t digits) {
  return text.size() == digits + 2 && text.starts_with("0x") &&
         std::all_of(text.begin() + 2, text.end(), [](unsigned char c) { return std::isxdigit(c); });
}

bool IsCanonicalVoiceRouteToken(std::string_view token) {
  static constexpr std::string_view kPrefix = "voice_";
  if (token.size() != kVoiceRouteTokenCharacters || !token.starts_with(kPrefix)) {
    return false;
  }
  token.remove_prefix(kPrefix.size());
  return std::ranges::all_of(token, [](unsigned char value) {
    return std::isdigit(value) || (value >= 'a' && value <= 'f');
  });
}

std::optional<std::uint32_t> ParseHex32(std::string_view text) {
  if (!IsHex(text, 8)) return std::nullopt;
  text.remove_prefix(2);
  std::uint32_t result = 0;
  const auto parsed = std::from_chars(text.data(), text.data() + text.size(), result, 16);
  if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size()) return std::nullopt;
  return result;
}

std::string Hex32Lower(std::uint32_t value) {
  std::ostringstream stream;
  stream << "0x" << std::hex << std::nouppercase << std::setfill('0')
         << std::setw(8) << value;
  return stream.str();
}

std::optional<std::uint64_t> ParseDecimalUint64(std::string_view text) {
  if (text.empty()) return std::nullopt;
  std::uint64_t result = 0;
  const auto parsed = std::from_chars(text.data(), text.data() + text.size(), result);
  if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size()) return std::nullopt;
  return result;
}

bool IsSha256Hex(std::string_view text) {
  return text.size() == kSha256HexCharacters &&
         std::all_of(text.begin(), text.end(), [](unsigned char value) {
           return std::isdigit(value) || (value >= 'a' && value <= 'f');
         });
}

bool IsKnownGta4EpisodePackage(std::string_view package) {
  return std::ranges::find(kKnownGta4EpisodePackages, package) !=
         kKnownGta4EpisodePackages.end();
}

std::optional<std::vector<unsigned char>> DecodeCanonicalProgAchBlob(
    std::string_view blob) {
  if (blob.size() != kGta4ProgAchBase64UrlCharacters ||
      !std::all_of(blob.begin(), blob.end(), [](unsigned char character) {
        return std::isalnum(character) || character == '-' || character == '_';
      }) ||
      std::string_view("AQgw").find(blob.back()) == std::string_view::npos) {
    return std::nullopt;
  }
  auto decoded = DecodeBase64Url(std::string(blob));
  if (!decoded || decoded->size() != kGta4ProgAchBytes) return std::nullopt;
  return decoded;
}

std::string SegmentAfter(std::string_view path, std::string_view prefix) {
  return path.starts_with(prefix) ? std::string(path.substr(prefix.size())) : std::string{};
}

struct Identity {
  std::string device_id;
  std::string xuid;
  std::string machine_id;
  std::string player_name;
};

struct DeviceBinding {
  std::string xuid;
  std::string public_key;
};

struct AccessSession {
  Identity identity;
  Clock::time_point expires;
};

struct RefreshSession {
  Identity identity;
  std::int64_t expires_at_unix = 0;
};

struct Challenge {
  std::string device_id;
  std::string xuid;
  std::string public_key;
  std::string text;
  Clock::time_point expires;
};

struct RelayRoute {
  std::string xuid;
  std::string session_id;
  std::uint16_t port = 0;
  std::string virtual_ipv4;
  Clock::time_point expires = Clock::time_point::max();
};

struct QosListener {
  std::uint64_t generation = 0;
  std::string host_xuid;
  std::string exchange_key;
  bool enabled = false;
  std::optional<std::string> title_data;
  std::uint32_t bits_per_second = 0;
};

struct Datagram {
  std::string source_ipv4;
  std::uint16_t source_port = 0;
  std::string payload;
};

struct VoiceRoute {
  std::string owner_xuid;
  std::string session_id;
  std::string channel;
  std::vector<std::string> targets;
  std::unordered_set<std::string> muted;
  // The client keeps an unacknowledged batch queued across transport errors.
  // Remember a bounded window of accepted sequence/payload pairs so a lost
  // HTTP response cannot produce duplicate decoded audio on retry.
  std::unordered_map<std::uint32_t, std::string> accepted_payload_digests;
  std::deque<std::uint32_t> accepted_sequence_order;
  Clock::time_point expires = Clock::time_point::max();
};

struct VoicePacket {
  std::string id;
  std::uint64_t cursor = 0;
  std::string source_xuid;
  std::string session_id;
  std::uint32_t sequence = 0;
  std::string payload;
  std::size_t encoded_bytes = 0;
  Clock::time_point expires = Clock::time_point::max();
};

struct RealtimeEvent {
  std::uint64_t id = 0;
  std::string session_id;
  std::string source_xuid;
  json wire;
};

struct ArbitrationState {
  json snapshot;
  std::unordered_set<std::string> registered_xuids;
  std::unordered_set<std::string> expected_machine_ids;
  std::unordered_set<std::string> registered_machine_ids;
  std::unordered_map<std::string, std::string> authorized_xuid_machines;
  std::string nonce;
  std::uint32_t duration_seconds = 0;
  std::uint32_t flags = 0;
  Clock::time_point deadline = Clock::time_point::max();
  bool failed = false;
};

struct ProgressionRecord {
  std::int64_t cash = 0;
  std::uint32_t rank = 0;
  std::string updated_at;
};

struct PresenceRecord {
  std::string state;
  std::string session_id;
  std::string episode;
  Clock::time_point expires;
  std::string expires_at;
};

struct NextGameState {
  std::string mode;
  std::string episode;
  bool ranked = false;
  json contexts = json::object();
  json properties = json::object();
};

struct LobbyState {
  std::int64_t revision = 0;
  std::map<std::string, bool> ready;
  std::map<std::string, bool> spectators;
  std::map<std::string, std::unordered_set<std::string>> kick_votes;
  std::optional<NextGameState> next_game;
};

struct MatchmakingTicket {
  std::string id;
  std::string owner_xuid;
  std::uint32_t procedure_index = 0;
  std::string mode;
  std::string episode;
  std::string region;
  bool ranked = false;
  std::size_t party_size = 0;
  json contexts = json::object();
  json properties = json::object();
  std::string state;
  std::string matched_session_id;
  std::string created_at;
  std::string updated_at;
  Clock::time_point expires = Clock::time_point::max();
};

struct StatWriteReceipt {
  std::string request_digest;
  int status = 204;
};

struct InviteAcceptReceipt {
  std::string recipient_xuid;
  std::string invite_id;
  std::string idempotency_key;
  std::string request_digest;
  json response;
};

struct SessionMutationReceipt {
  std::string request_digest;
  std::optional<Response> terminal_response;
  Clock::time_point expires = Clock::time_point::max();
};

struct TitleProfileSettingRecord {
  std::uint64_t revision = 0;
  std::string blob;
  std::string digest;
  std::string updated_at;
};

struct ProgAchRecord {
  std::string blob;
  std::string digest;
  std::string updated_at;
};

struct DurableData {
  std::unordered_map<std::string, DeviceBinding> devices;
  std::unordered_map<std::string, RefreshSession> refresh_sessions;
  std::unordered_map<std::string, std::string> player_names;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> relationships;
  std::unordered_map<std::string, json> invites;
  std::unordered_map<std::string, std::unordered_map<std::string, json>> stats;
  std::unordered_map<std::string, ProgressionRecord> progression;
  std::unordered_map<std::string, std::unordered_map<std::string, json>> mode_stats;
  std::unordered_map<std::string, json> ranked_results;
  std::unordered_map<std::string, std::string> ranked_result_requests;
  std::unordered_map<std::string, json> profiles;
  std::unordered_map<std::string, StatWriteReceipt> stat_write_receipts;
  std::unordered_map<std::string, InviteAcceptReceipt> invite_accept_receipts;
  std::unordered_map<std::string, std::uint64_t> stat_next_sequences;
  std::unordered_map<std::string, TitleProfileSettingRecord> title_profile_settings;
  std::unordered_map<std::string, ProgAchRecord> prog_ach_records;
  std::unordered_map<std::string, std::unordered_set<std::uint32_t>> achievements;
  std::unordered_map<std::string, std::unordered_set<std::string>> entitlements;
};

std::int64_t UnixSecondsAfter(std::chrono::seconds offset = std::chrono::seconds::zero()) {
  return std::chrono::duration_cast<std::chrono::seconds>(
             SystemClock::now().time_since_epoch() + offset)
      .count();
}

std::optional<std::string> Sha256Hex(std::string_view value) {
  std::array<unsigned char, EVP_MAX_MD_SIZE> digest{};
  unsigned int digest_size = 0;
  EVP_MD_CTX* context = EVP_MD_CTX_new();
  if (!context) return std::nullopt;
  const bool okay = EVP_DigestInit_ex(context, EVP_sha256(), nullptr) == 1 &&
                    EVP_DigestUpdate(context, value.data(), value.size()) == 1 &&
                    EVP_DigestFinal_ex(context, digest.data(), &digest_size) == 1;
  EVP_MD_CTX_free(context);
  if (!okay || digest_size != kSha256Bytes) return std::nullopt;
  static constexpr char kHex[] = "0123456789abcdef";
  std::string result;
  result.reserve(kSha256HexCharacters);
  for (unsigned int index = 0; index < digest_size; ++index) {
    result.push_back(kHex[digest[index] >> 4]);
    result.push_back(kHex[digest[index] & 15]);
  }
  return result;
}

std::optional<std::string> InviteAcceptReceiptKey(std::string_view recipient_xuid,
                                                  std::string_view invite_id,
                                                  std::string_view idempotency_key) {
  return Sha256Hex(json::array({recipient_xuid, invite_id, idempotency_key}).dump());
}

json IdentityJson(const Identity& identity) {
  return {{"device_id", identity.device_id}, {"xuid", identity.xuid},
          {"machine_id", identity.machine_id}, {"player_name", identity.player_name}};
}

std::string UtcIsoAfter(std::chrono::seconds offset = std::chrono::seconds::zero()) {
  const std::time_t value = std::chrono::system_clock::to_time_t(
      std::chrono::system_clock::now() + offset);
  std::tm utc{};
#if defined(_WIN32)
  gmtime_s(&utc, &value);
#else
  gmtime_r(&value, &utc);
#endif
  char buffer[32]{};
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &utc);
  return buffer;
}

bool ValidEpisode(std::string_view episode) {
  return episode == "base" || episode == "tlad" || episode == "tbogt";
}

bool ValidIdentifier(std::string_view value, std::size_t maximum_length = 128) {
  if (value.empty() || value.size() > maximum_length ||
      !std::isalnum(static_cast<unsigned char>(value.front()))) return false;
  return std::all_of(value.begin() + 1, value.end(), [](unsigned char c) {
    return std::isalnum(c) || c == '_' || c == '.' || c == ':' || c == '-';
  });
}

struct PreparedSessionMutation {
  bool enabled = false;
  std::string receipt_key;
  std::string request_digest;
  std::optional<Response> error;
};

PreparedSessionMutation PrepareSessionMutation(const Request& request,
                                                const Identity& identity) {
  const auto header = request.headers.find("idempotency-key");
  if (header == request.headers.end()) return {};
  if (!ValidIdentifier(header->second, kMaximumIdempotencyKeyBytes)) {
    return {.error = Error(400, "invalid_idempotency_key",
                           "Idempotency-Key is invalid")};
  }

  // Preconditions select which authoritative revision may perform the mutation, but they do not
  // change the mutation itself. Omitting them from the intent makes a terminal 412 replay stable
  // (the client must use a fresh key) and lets a nonterminal arbitration poll advance from the
  // frozen revision without changing its idempotency domain.
  json canonical_intent = json::object();
  if (auto body = ParseBody(request)) {
    if (body->is_object()) {
      body->erase("expected_revision");
      if (request.method == "POST" && request.path.ends_with("/migration")) {
        body->erase("expected_host_epoch");
      }
    }
    canonical_intent["body"] = std::move(*body);
  } else {
    canonical_intent["invalid_json"] = request.body;
  }
  const auto request_digest = Sha256Hex(canonical_intent.dump());
  // The HTTP parser has already removed the query component. Preserve the routed spelling here:
  // session identifiers and action names are case-sensitive resource paths in this service.
  const std::string& canonical_path = request.path;
  const auto receipt_key = Sha256Hex(
      json::array({identity.device_id, identity.xuid, identity.machine_id,
                   request.method, canonical_path, header->second})
          .dump());
  if (!request_digest || !receipt_key) {
    return {.error = Error(500, "digest_failure",
                           "session mutation receipt could not be hashed")};
  }
  return {.enabled = true,
          .receipt_key = *receipt_key,
          .request_digest = *request_digest};
}

bool ValidEpisodeOrAll(std::string_view episode) {
  return ValidEpisode(episode) || episode == "all";
}

std::optional<std::int64_t> NonNegativeInteger(const json& value) {
  if (value.is_number_unsigned()) {
    const std::uint64_t number = value.get<std::uint64_t>();
    if (number > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) {
      return std::nullopt;
    }
    return static_cast<std::int64_t>(number);
  }
  if (!value.is_number_integer()) return std::nullopt;
  const std::int64_t number = value.get<std::int64_t>();
  return number < 0 ? std::nullopt : std::optional(number);
}

std::optional<std::uint32_t> SessionUint32(const json& value, bool allow_hex_string = false) {
  if (allow_hex_string && value.is_string()) {
    return ParseHex32(Lower(value.get<std::string>()));
  }
  const auto number = NonNegativeInteger(value);
  if (!number || static_cast<std::uint64_t>(*number) >
                     std::numeric_limits<std::uint32_t>::max()) {
    return std::nullopt;
  }
  return static_cast<std::uint32_t>(*number);
}

std::string SessionTraceJsonString(std::string_view value) {
  return json(std::string(value)).dump();
}

std::string SessionTraceScalar(const json& preferred, const json& fallback,
                               std::string_view key) {
  for (const json* source : {&preferred, &fallback}) {
    if (!source->is_object()) continue;
    const auto value = source->find(std::string(key));
    if (value == source->end() || value->is_null() || !value->is_primitive()) continue;
    return value->dump();
  }
  return "-";
}

std::optional<std::int64_t> SessionTraceInteger(const json& preferred, const json& fallback,
                                                std::string_view key) {
  for (const json* source : {&preferred, &fallback}) {
    if (!source->is_object()) continue;
    const auto value = source->find(std::string(key));
    if (value == source->end()) continue;
    if (const auto number = NonNegativeInteger(*value)) return number;
  }
  return std::nullopt;
}

std::string_view SessionLifecycleTraceName(std::int64_t lifecycle) {
  switch (lifecycle) {
    case 0: return "lobby";
    case 1: return "registration";
    case 2: return "in_game";
    case 3: return "reporting";
    case 4: return "deleted";
    default: return "unknown";
  }
}

std::string SessionTraceOperation(const Request& request, const json& request_body) {
  if (request.path == "/api/v2/sessions" && request.method == "POST") return "create";
  if (request.path == "/api/v2/sessions/search" && request.method == "POST") return "search";
  if (request.path.starts_with("/api/v2/sessions/by-xbox-id/") && request.method == "GET") {
    return "lookup";
  }
  if (request.method == "GET") return "get";
  if (request.method == "DELETE") return "delete";
  if (request.method == "PATCH") {
    const auto lifecycle = SessionTraceInteger(json::object(), request_body, "lifecycle_state");
    if (lifecycle == 1) return "arbitration";
    if (lifecycle == 2) return "start";
    if (lifecycle == 3) return "end";
    return "modify";
  }
  for (const std::string_view action : {"heartbeat", "join", "leave", "migration"}) {
    if (request.method == "POST" && request.path.ends_with(std::string("/") + std::string(action))) {
      return std::string(action);
    }
  }
  return "unknown";
}

std::string SessionTraceRequestId(const Request& request, const json& request_body) {
  if (request.path.starts_with("/api/v2/sessions/by-xbox-id/")) {
    return SegmentAfter(request.path, "/api/v2/sessions/by-xbox-id/");
  }
  if (request.path.starts_with("/api/v2/sessions/")) {
    const std::string suffix = SegmentAfter(request.path, "/api/v2/sessions/");
    return suffix.substr(0, suffix.find('/'));
  }
  if (request_body.is_object()) {
    const auto session_id = request_body.find("session_id");
    if (session_id != request_body.end() && session_id->is_string()) {
      return session_id->get<std::string>();
    }
  }
  return {};
}

std::string SessionTraceCount(const json& preferred, const json& fallback) {
  for (const json* source : {&preferred, &fallback}) {
    if (!source->is_object()) continue;
    const auto members = source->find("members");
    if (members != source->end() && members->is_array()) {
      return std::to_string(members->size());
    }
    const auto member_count = source->find("member_count");
    if (member_count != source->end() && NonNegativeInteger(*member_count)) {
      return member_count->dump();
    }
  }
  return "-";
}

std::string FormatSessionOperationTrace(const Request& request, const Identity& identity,
                                        const Response& response) {
  const auto parsed_request = ParseBody(request);
  const json request_body = parsed_request && parsed_request->is_object()
                                ? *parsed_request
                                : json::object();
  const json& response_body = response.body;
  const std::string request_session = SessionTraceRequestId(request, request_body);
  const std::string response_session =
      response_body.is_object() && response_body.contains("session_id") &&
              response_body.at("session_id").is_string()
          ? response_body.at("session_id").get<std::string>()
          : std::string{};
  const auto lifecycle = SessionTraceInteger(response_body, request_body, "lifecycle_state");
  std::string results = "-";
  if (response_body.is_object()) {
    const auto sessions = response_body.find("sessions");
    if (sessions != response_body.end() && sessions->is_array()) {
      results = std::to_string(sessions->size());
    }
  }
  std::string error = "-";
  if (response_body.is_object()) {
    const auto error_object = response_body.find("error");
    if (error_object != response_body.end() && error_object->is_object()) {
      const auto code = error_object->find("code");
      if (code != error_object->end() && code->is_string()) {
        error = SessionTraceJsonString(code->get<std::string>());
      }
    }
  }

  std::ostringstream trace;
  trace << "libserver-session-op: method=" << SessionTraceJsonString(request.method)
        << " path=" << SessionTraceJsonString(request.path)
        << " operation=" << SessionTraceOperation(request, request_body)
        << " xuid=" << SessionTraceJsonString(identity.xuid)
        << " session="
        << (request_session.empty() ? "-" : SessionTraceJsonString(request_session))
        << " response_session="
        << (response_session.empty() ? "-" : SessionTraceJsonString(response_session))
        << " status=" << response.status
        << " result=" << (response.status >= 200 && response.status < 300 ? "ok" : "error")
        << " revision=" << SessionTraceScalar(response_body, request_body, "revision")
        << " expected_revision="
        << SessionTraceScalar(request_body, json::object(), "expected_revision")
        << " state=" << SessionTraceScalar(response_body, request_body, "state")
        << " lifecycle_state=" << (lifecycle ? std::to_string(*lifecycle) : "-")
        << " lifecycle="
        << (lifecycle ? SessionTraceJsonString(SessionLifecycleTraceName(*lifecycle)) : "-")
        << " mode=" << SessionTraceScalar(response_body, request_body, "mode")
        << " ranked=" << SessionTraceScalar(response_body, request_body, "ranked")
        << " members=" << SessionTraceCount(response_body, request_body)
        << " results=" << results << " error=" << error;
  return trace.str();
}

std::mutex& SessionTraceOutputMutex() {
  static std::mutex mutex;
  return mutex;
}

Response TraceSessionOperation(const Request& request, const Identity& identity,
                               Response response) {
  const std::string trace = FormatSessionOperationTrace(request, identity, response);
  {
    std::lock_guard lock(SessionTraceOutputMutex());
    std::cout << trace + '\n';
  }
  return response;
}

bool ValidFilterObject(const json& values, bool encoded_properties) {
  if (!values.is_object() || values.size() > kMaximumMatchmakingFilterEntries) return false;
  for (const auto& [key, value] : values.items()) {
    if (!ValidIdentifier(key)) return false;
    if (!encoded_properties) {
      if (!value.is_number_unsigned() && !value.is_number_integer()) return false;
      if (value.is_number_integer() && value.get<std::int64_t>() < 0) return false;
      if (value.is_number_unsigned() && value.get<std::uint64_t>() > std::numeric_limits<std::uint32_t>::max()) {
        return false;
      }
      if (value.is_number_integer() &&
          static_cast<std::uint64_t>(value.get<std::int64_t>()) > std::numeric_limits<std::uint32_t>::max()) {
        return false;
      }
      continue;
    }
    if (!value.is_string()) return false;
    const auto decoded = DecodeBase64Url(value.get<std::string>());
    if (!decoded || decoded->size() > kMaximumPropertyBytes) return false;
  }
  return true;
}

json LobbyJson(const std::string& session_id, const LobbyState& state) {
  json kick_votes = json::object();
  for (const auto& [target, voters] : state.kick_votes) {
    std::vector<std::string> sorted(voters.begin(), voters.end());
    std::sort(sorted.begin(), sorted.end());
    kick_votes[target] = std::move(sorted);
  }
  json result = {{"session_id", session_id}, {"revision", state.revision},
                 {"ready", state.ready}, {"spectators", state.spectators},
                 {"kick_votes", std::move(kick_votes)}};
  if (state.next_game) {
    result["next_game"] = {{"mode", state.next_game->mode},
                           {"episode", state.next_game->episode},
                           {"ranked", state.next_game->ranked},
                           {"contexts", state.next_game->contexts},
                           {"properties", state.next_game->properties}};
  }
  return result;
}

json TicketJson(const MatchmakingTicket& ticket) {
  json result = {{"id", ticket.id}, {"owner_id", ticket.owner_xuid},
                 {"procedure_index", ticket.procedure_index}, {"mode", ticket.mode},
                 {"episode", ticket.episode}, {"region", ticket.region},
                 {"ranked", ticket.ranked}, {"party_size", ticket.party_size},
                 {"contexts", ticket.contexts}, {"properties", ticket.properties},
                 {"state", ticket.state}, {"created_at", ticket.created_at},
                 {"updated_at", ticket.updated_at}};
  if (!ticket.matched_session_id.empty()) result["matched_session_id"] = ticket.matched_session_id;
  return result;
}

json ProgressionJson(std::string_view xuid, const ProgressionRecord& record) {
  json unlocks = json::array();
  for (std::uint32_t rank = 0; rank <= record.rank; ++rank) {
    unlocks.push_back("rank_" + std::to_string(rank));
  }
  return {{"xuid", xuid}, {"cash", record.cash}, {"rank", record.rank},
          {"clothing_unlocks", std::move(unlocks)}, {"updated_at", record.updated_at}};
}

json TitleProfileSettingJson(const TitleProfileSettingRecord& record) {
  return {{"title_id", "0x545407f2"},
          {"setting_id", "0x63e83fff"},
          {"revision", std::to_string(record.revision)},
          {"blob", record.blob},
          {"digest", record.digest},
          {"updated_at", record.updated_at}};
}

json SingleStatWriteRequest(std::string_view session_id, std::string_view sequence,
                            std::string_view xuid, std::string_view view_id, json columns) {
  json row = json::object();
  row["xuid"] = std::string(xuid);
  row["columns"] = std::move(columns);
  json view = json::object();
  view["view_id"] = std::string(view_id);
  view["rows"] = json::array({std::move(row)});
  json body = json::object();
  body["session_id"] = std::string(session_id);
  body["sequence"] = std::string(sequence);
  body["views"] = json::array({std::move(view)});
  return body;
}

bool JsonInt64(const json& value, std::int64_t& result) {
  if (value.is_number_unsigned()) {
    const std::uint64_t candidate = value.get<std::uint64_t>();
    if (candidate > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) {
      return false;
    }
    result = static_cast<std::int64_t>(candidate);
    return true;
  }
  if (value.is_number_integer()) {
    result = value.get<std::int64_t>();
    return true;
  }
  return false;
}

bool StatValueMatchesDescriptor(
    const json& value, const libserver::Gta4StatFieldDescriptor& descriptor);

using StoredStats =
    std::unordered_map<std::string, std::unordered_map<std::string, json>>;

struct RankedStatCandidate {
  std::string xuid;
  const json* view = nullptr;
  std::int64_t rating = 0;
};

std::optional<std::vector<RankedStatCandidate>> BuildRankedStatCandidates(
    const StoredStats& stats, std::uint32_t view_id) {
  constexpr std::uint16_t kIntrinsicRatingAttribute = UINT16_C(0xFFFE);
  const auto* rating_mapping =
      libserver::FindGta4StatAttributeById(view_id, kIntrinsicRatingAttribute);
  const auto* rating_descriptor =
      rating_mapping
          ? libserver::FindGta4StatField(view_id, rating_mapping->property_id)
          : nullptr;
  if (!rating_mapping || !rating_descriptor ||
      rating_descriptor->wire_type == libserver::StatWireType::kUnicode) {
    return std::nullopt;
  }

  const std::string view_key = Hex32Lower(view_id);
  const std::string rating_key = Hex32Lower(rating_mapping->property_id);
  std::vector<RankedStatCandidate> candidates;
  for (const auto& [xuid, views] : stats) {
    if (!views.contains(view_key) || !views.at(view_key).contains(rating_key)) continue;
    const json& rating_wire = views.at(view_key).at(rating_key);
    std::int64_t rating = 0;
    if (!StatValueMatchesDescriptor(rating_wire, *rating_descriptor) ||
        !JsonInt64(rating_wire.at("value"), rating)) {
      return std::nullopt;
    }
    candidates.push_back({.xuid = xuid, .view = &views.at(view_key), .rating = rating});
  }
  const bool ascending =
      rating_descriptor->aggregation == libserver::StatAggregation::kMin;
  std::ranges::sort(candidates, [ascending](const auto& left, const auto& right) {
    if (left.rating != right.rating) {
      return ascending ? left.rating < right.rating : left.rating > right.rating;
    }
    return left.xuid < right.xuid;
  });
  return candidates;
}

bool JsonUint32(const json& value, std::uint32_t& result) {
  std::uint64_t candidate = 0;
  if (value.is_number_unsigned()) {
    candidate = value.get<std::uint64_t>();
  } else if (value.is_number_integer()) {
    const std::int64_t signed_candidate = value.get<std::int64_t>();
    if (signed_candidate < 0) return false;
    candidate = static_cast<std::uint64_t>(signed_candidate);
  } else {
    return false;
  }
  if (candidate > std::numeric_limits<std::uint32_t>::max()) return false;
  result = static_cast<std::uint32_t>(candidate);
  return true;
}

bool JsonUint64(const json& value, std::uint64_t& result) {
  if (value.is_number_unsigned()) {
    result = value.get<std::uint64_t>();
    return true;
  }
  if (value.is_number_integer()) {
    const std::int64_t candidate = value.get<std::int64_t>();
    if (candidate < 0) return false;
    result = static_cast<std::uint64_t>(candidate);
    return true;
  }
  return false;
}

bool CheckedAddInt64(std::int64_t left, std::int64_t right, std::int64_t& result) {
  if ((right > 0 && left > std::numeric_limits<std::int64_t>::max() - right) ||
      (right < 0 && left < std::numeric_limits<std::int64_t>::min() - right)) {
    return false;
  }
  result = left + right;
  return true;
}

bool CheckedAddUint64(std::uint64_t left, std::uint64_t right, std::uint64_t& result) {
  if (left > std::numeric_limits<std::uint64_t>::max() - right) return false;
  result = left + right;
  return true;
}

bool ValidStatValue(const json& value) {
  if (!value.is_object() || value.size() != 2 || !value.contains("type") ||
      !value.at("type").is_string() || !value.contains("value") ||
      value.at("value").dump().size() > kMaximumStatValueJsonBytes) {
    return false;
  }
  const std::string type = value.at("type").get<std::string>();
  const json& payload = value.at("value");
  if (type == "i32") {
    std::int64_t integer = 0;
    return JsonInt64(payload, integer) && integer >= std::numeric_limits<std::int32_t>::min() &&
           integer <= std::numeric_limits<std::int32_t>::max();
  }
  if (type == "i64") {
    std::int64_t integer = 0;
    return JsonInt64(payload, integer);
  }
  if (type == "f64") {
    if (!payload.is_number()) return false;
    const double number = payload.get<double>();
    return std::isfinite(number);
  }
  if (type == "unicode") {
    return payload.is_string() && payload.get_ref<const std::string&>().size() <= kMaximumStatUnicodeBytes;
  }
  if (type == "binary") {
    if (!payload.is_string()) return false;
    const auto decoded = DecodeBase64Url(payload.get<std::string>());
    return decoded && decoded->size() <= kMaximumStatBinaryBytes;
  }
  return false;
}

bool StatValueMatchesDescriptor(const json& value,
                                const libserver::Gta4StatFieldDescriptor& descriptor) {
  if (!ValidStatValue(value)) return false;
  const std::string type = value.at("type").get<std::string>();
  switch (descriptor.wire_type) {
    case libserver::StatWireType::kInt32:
      return type == "i32";
    case libserver::StatWireType::kInt64:
      return type == "i64";
    case libserver::StatWireType::kUnicode:
      return type == "unicode";
  }
  return false;
}

std::optional<json> MergeStatValue(const libserver::Gta4StatFieldDescriptor& descriptor,
                                   const json* current, const json& incoming) {
  if (!StatValueMatchesDescriptor(incoming, descriptor) ||
      (current && !StatValueMatchesDescriptor(*current, descriptor))) {
    return std::nullopt;
  }
  if (!current || descriptor.aggregation == libserver::StatAggregation::kLast) {
    return incoming;
  }
  if (descriptor.wire_type == libserver::StatWireType::kUnicode) return std::nullopt;

  std::int64_t current_value = 0;
  std::int64_t incoming_value = 0;
  if (!JsonInt64(current->at("value"), current_value) ||
      !JsonInt64(incoming.at("value"), incoming_value)) {
    return std::nullopt;
  }
  std::int64_t merged = incoming_value;
  if (descriptor.aggregation == libserver::StatAggregation::kSum) {
    if (!CheckedAddInt64(current_value, incoming_value, merged)) return std::nullopt;
  } else if (descriptor.aggregation == libserver::StatAggregation::kMin) {
    merged = std::min(current_value, incoming_value);
  } else {
    return std::nullopt;
  }
  if (descriptor.wire_type == libserver::StatWireType::kInt32 &&
      (merged < std::numeric_limits<std::int32_t>::min() ||
       merged > std::numeric_limits<std::int32_t>::max())) {
    return std::nullopt;
  }
  json result = incoming;
  result["value"] = merged;
  return result;
}

std::uint32_t RankForCash(std::int64_t cash) {
  std::uint32_t result = 0;
  for (std::uint32_t rank = 0; rank < kRankCashThresholds.size(); ++rank) {
    if (cash >= kRankCashThresholds[rank]) result = rank;
  }
  return result;
}

std::string StatSequenceOwnerKey(std::string_view xuid, std::string_view session_id) {
  return std::string(xuid) + ":" + std::string(session_id);
}

std::string StatWriteReceiptKey(std::string_view xuid, std::string_view session_id,
                                std::uint64_t sequence) {
  return StatSequenceOwnerKey(xuid, session_id) + ":" + std::to_string(sequence);
}

bool IsValidGta4TitleProfileBlob(std::span<const unsigned char> bytes) {
  if (bytes.size() > kMaximumGta4TitleProfileBytes ||
      bytes.size() % kGta4TitleProfileRecordBytes != 0) {
    return false;
  }
  std::unordered_set<std::uint32_t> keys;
  for (std::size_t offset = 0; offset < bytes.size();
       offset += kGta4TitleProfileRecordBytes) {
    const std::uint32_t key =
        (static_cast<std::uint32_t>(bytes[offset]) << 24) |
        (static_cast<std::uint32_t>(bytes[offset + 1]) << 16) |
        (static_cast<std::uint32_t>(bytes[offset + 2]) << 8) |
        static_cast<std::uint32_t>(bytes[offset + 3]);
    if (!keys.insert(key).second) return false;
  }
  return true;
}

bool HasExactKeys(const json& object, std::initializer_list<std::string_view> keys) {
  if (!object.is_object() || object.size() != keys.size()) return false;
  return std::all_of(keys.begin(), keys.end(), [&](std::string_view key) {
    return object.contains(std::string(key));
  });
}

json SerializeDurable(const DurableData& data) {
  json devices = json::object();
  for (const auto& [device_id, binding] : data.devices) {
    devices[device_id] = {{"xuid", binding.xuid}, {"public_key", binding.public_key},
                          {"enrollment_mode", "local_first_binding"}};
  }
  json refresh_sessions = json::object();
  for (const auto& [token_hash, session] : data.refresh_sessions) {
    refresh_sessions[token_hash] = {{"identity", IdentityJson(session.identity)},
                                    {"expires_at_unix", session.expires_at_unix}};
  }
  json progression = json::object();
  for (const auto& [xuid, record] : data.progression) {
    progression[xuid] = {{"cash", record.cash}, {"rank", record.rank},
                         {"updated_at", record.updated_at}};
  }
  json ranked = json::object();
  for (const auto& [result_id, response] : data.ranked_results) {
    const auto request = data.ranked_result_requests.find(result_id);
    if (request != data.ranked_result_requests.end()) {
      ranked[result_id] = {{"canonical_request", request->second}, {"response", response}};
    }
  }
  json achievements = json::object();
  for (const auto& [xuid, unlocked] : data.achievements) {
    std::vector<std::uint32_t> ordered(unlocked.begin(), unlocked.end());
    std::ranges::sort(ordered);
    achievements[xuid] = std::move(ordered);
  }
  json entitlements = json::object();
  for (const auto& [xuid, granted] : data.entitlements) {
    std::vector<std::string> ordered(granted.begin(), granted.end());
    std::ranges::sort(ordered);
    entitlements[xuid] = std::move(ordered);
  }
  json stat_write_receipts = json::object();
  for (const auto& [key, receipt] : data.stat_write_receipts) {
    stat_write_receipts[key] = {
        {"request_digest", receipt.request_digest}, {"status", receipt.status}};
  }
  json invite_accept_receipts = json::object();
  for (const auto& [key, receipt] : data.invite_accept_receipts) {
    invite_accept_receipts[key] = {
        {"recipient_xuid", receipt.recipient_xuid},
        {"invite_id", receipt.invite_id},
        {"idempotency_key", receipt.idempotency_key},
        {"request_digest", receipt.request_digest},
        {"status", 200},
        {"response", receipt.response},
    };
  }
  json stat_next_sequences = json::object();
  for (const auto& [key, next_sequence] : data.stat_next_sequences) {
    stat_next_sequences[key] = std::to_string(next_sequence);
  }
  json title_profile_settings = json::object();
  for (const auto& [xuid, record] : data.title_profile_settings) {
    title_profile_settings[xuid] = {
        {"title_id", "0x545407f2"},
        {"setting_id", "0x63e83fff"},
        {"revision", std::to_string(record.revision)},
        {"blob", record.blob},
        {"digest", record.digest},
        {"updated_at", record.updated_at},
    };
  }
  json prog_ach_records = json::object();
  for (const auto& [xuid, record] : data.prog_ach_records) {
    prog_ach_records[xuid] = {
        {"title_id", "0x545407F2"},
        {"facility", 3},
        {"path", "Prog_ACH"},
        {"blob", record.blob},
        {"digest", record.digest},
        {"updated_at", record.updated_at},
    };
  }
  return {{"payload_schema", kDurablePayloadSchema},
          {"enrollment_mode", "local_first_binding"},
          {"devices", std::move(devices)},
          {"refresh_sessions", std::move(refresh_sessions)},
          {"player_names", data.player_names},
          {"relationships", data.relationships},
          {"invites", data.invites},
          {"stats", data.stats},
          {"progression", std::move(progression)},
          {"mode_stats", data.mode_stats},
          {"ranked_results", std::move(ranked)},
          {"profiles", data.profiles},
          {"stat_write_receipts", std::move(stat_write_receipts)},
          {"invite_accept_receipts", std::move(invite_accept_receipts)},
          {"stat_next_sequences", std::move(stat_next_sequences)},
          {"title_profile_settings", std::move(title_profile_settings)},
          {"prog_ach_records", std::move(prog_ach_records)},
          {"achievements", std::move(achievements)},
          {"entitlements", std::move(entitlements)}};
}

bool ParseIdentity(const json& value, Identity& identity) {
  if (!HasExactKeys(value, {"device_id", "xuid", "machine_id", "player_name"}) ||
      !value.at("device_id").is_string() || !value.at("xuid").is_string() ||
      !value.at("machine_id").is_string() || !value.at("player_name").is_string()) return false;
  identity = {.device_id = value.at("device_id").get<std::string>(),
              .xuid = value.at("xuid").get<std::string>(),
              .machine_id = value.at("machine_id").get<std::string>(),
              .player_name = value.at("player_name").get<std::string>()};
  return ValidIdentifier(identity.device_id) && IsHex(identity.xuid, 16) &&
         IsHex(identity.machine_id, 16) && !identity.player_name.empty() &&
         identity.player_name.size() <= 128;
}

bool DeserializeDurable(const json& wire_payload, DurableData& result, std::string& error) {
  try {
  json payload = wire_payload;
  if (!libserver::NormalizeIdentities(payload, error, {}, true)) return false;
  static constexpr std::array<std::string_view, 12> kLegacyKeys = {
      "payload_schema", "enrollment_mode", "devices", "refresh_sessions", "player_names",
      "relationships", "invites", "stats", "progression", "mode_stats", "ranked_results",
      "profiles"};
  static constexpr std::array<std::string_view, 13> kAchievementKeys = {
      "payload_schema", "enrollment_mode", "devices", "refresh_sessions", "player_names",
      "relationships", "invites", "stats", "progression", "mode_stats", "ranked_results",
      "profiles", "achievements"};
  static constexpr std::array<std::string_view, 14> kEntitlementKeys = {
      "payload_schema", "enrollment_mode", "devices", "refresh_sessions", "player_names",
      "relationships", "invites", "stats", "progression", "mode_stats", "ranked_results",
      "profiles", "achievements", "entitlements"};
  static constexpr std::array<std::string_view, 17> kProfileKeys = {
      "payload_schema", "enrollment_mode", "devices", "refresh_sessions", "player_names",
      "relationships", "invites", "stats", "progression", "mode_stats", "ranked_results",
      "profiles", "stat_write_receipts", "stat_next_sequences", "title_profile_settings",
      "achievements", "entitlements"};
  static constexpr std::array<std::string_view, 19> kCurrentKeys = {
      "payload_schema", "enrollment_mode", "devices", "refresh_sessions", "player_names",
      "relationships", "invites", "stats", "progression", "mode_stats", "ranked_results",
      "profiles", "stat_write_receipts", "invite_accept_receipts", "stat_next_sequences",
      "title_profile_settings", "prog_ach_records", "achievements", "entitlements"};
  if (!payload.is_object() || !payload.contains("payload_schema") ||
      !payload.at("payload_schema").is_number_unsigned()) {
    error = "durable payload schema is invalid";
    return false;
  }
  const std::uint64_t schema = payload.at("payload_schema").get<std::uint64_t>();
  const auto has_exact_schema_keys = [&](const auto& keys) {
    return payload.size() == keys.size() &&
           std::all_of(keys.begin(), keys.end(), [&](std::string_view key) {
             return payload.contains(std::string(key));
           });
  };
  if (!((schema == kLegacyDurablePayloadSchema && has_exact_schema_keys(kLegacyKeys)) ||
        (schema == kAchievementDurablePayloadSchema &&
         has_exact_schema_keys(kAchievementKeys)) ||
        (schema == kEntitlementDurablePayloadSchema &&
         has_exact_schema_keys(kEntitlementKeys)) ||
        (schema == kProfileDurablePayloadSchema && has_exact_schema_keys(kProfileKeys)) ||
        (schema == kDurablePayloadSchema && has_exact_schema_keys(kCurrentKeys))) ||
      payload.value("enrollment_mode", "") != "local_first_binding") {
    error = "durable payload schema is invalid";
    return false;
  }
  for (std::string_view field : {"devices", "refresh_sessions", "player_names", "relationships",
                                 "invites", "stats", "progression", "mode_stats",
                                 "ranked_results", "profiles"}) {
    if (!payload.at(std::string(field)).is_object()) {
      error = "durable payload collection is invalid";
      return false;
    }
  }
  if (schema >= kAchievementDurablePayloadSchema &&
      !payload.at("achievements").is_object()) {
    error = "durable achievement collection is invalid";
    return false;
  }
  if (schema >= kEntitlementDurablePayloadSchema &&
      !payload.at("entitlements").is_object()) {
    error = "durable entitlement collection is invalid";
    return false;
  }
  if (schema >= kProfileDurablePayloadSchema &&
      (!payload.at("stat_write_receipts").is_object() ||
       !payload.at("stat_next_sequences").is_object() ||
       !payload.at("title_profile_settings").is_object())) {
    error = "durable profile or stat receipt collection is invalid";
    return false;
  }
  if (schema == kDurablePayloadSchema &&
      (!payload.at("invite_accept_receipts").is_object() ||
       !payload.at("prog_ach_records").is_object())) {
    error = "durable schema 5 receipt or title storage collection is invalid";
    return false;
  }
  DurableData parsed;
  std::unordered_set<std::string> bound_xuids;
  for (const auto& [device_id, value] : payload.at("devices").items()) {
    if (!ValidIdentifier(device_id) ||
        !HasExactKeys(value, {"xuid", "public_key", "enrollment_mode"}) ||
        !value.at("xuid").is_string() || !IsHex(value.at("xuid").get<std::string>(), 16) ||
        !value.at("public_key").is_string() ||
        value.value("enrollment_mode", "") != "local_first_binding") {
      error = "device binding is invalid";
      return false;
    }
    const auto key = DecodeBase64Url(value.at("public_key").get<std::string>());
    if (!key || key->size() != kSha256Bytes) {
      error = "device public key is invalid";
      return false;
    }
    const std::string xuid = value.at("xuid").get<std::string>();
    if (xuid == "0x0000000000000000" || !bound_xuids.insert(xuid).second) {
      error = "xuid has more than one device owner";
      return false;
    }
    parsed.devices.emplace(device_id,
                           DeviceBinding{xuid, value.at("public_key").get<std::string>()});
  }
  for (const auto& [token_hash, value] : payload.at("refresh_sessions").items()) {
    if (token_hash.size() != kSha256HexCharacters ||
        !std::all_of(token_hash.begin(), token_hash.end(), [](unsigned char c) {
          return std::isxdigit(c) && !std::isupper(c);
        }) || !HasExactKeys(value, {"identity", "expires_at_unix"}) ||
        !value.at("expires_at_unix").is_number_integer()) {
      error = "refresh session is invalid";
      return false;
    }
    Identity identity;
    const std::int64_t expires = value.at("expires_at_unix").get<std::int64_t>();
    if (expires <= 0 || !ParseIdentity(value.at("identity"), identity)) {
      error = "refresh identity is invalid";
      return false;
    }
    const auto device = parsed.devices.find(identity.device_id);
    if (device == parsed.devices.end() || device->second.xuid != identity.xuid) {
      error = "refresh identity has no matching device binding";
      return false;
    }
    parsed.refresh_sessions.emplace(token_hash, RefreshSession{std::move(identity), expires});
  }
  for (const auto& [xuid, value] : payload.at("player_names").items()) {
    if (!IsHex(xuid, 16) || !value.is_string() || value.get<std::string>().empty() ||
        value.get_ref<const std::string&>().size() > 128) {
      error = "player name is invalid";
      return false;
    }
    parsed.player_names.emplace(xuid, value.get<std::string>());
  }
  static const std::unordered_set<std::string> kRelationshipStates = {
      "incoming", "outgoing", "accepted", "blocked", "blocked_by", "blocked_mutual"};
  for (const auto& [owner, entries] : payload.at("relationships").items()) {
    if (!IsHex(owner, 16) || !entries.is_object()) {
      error = "relationship owner is invalid";
      return false;
    }
    for (const auto& [target, state] : entries.items()) {
      if (!IsHex(target, 16) || target == owner || !state.is_string() ||
          !kRelationshipStates.contains(state.get<std::string>())) {
        error = "relationship entry is invalid";
        return false;
      }
      parsed.relationships[owner][target] = state.get<std::string>();
    }
  }
  for (const auto& [owner, entries] : parsed.relationships) {
    for (const auto& [target, state] : entries) {
      const auto reverse_owner = parsed.relationships.find(target);
      if (reverse_owner == parsed.relationships.end() ||
          !reverse_owner->second.contains(owner)) {
        error = "relationship pair is incomplete";
        return false;
      }
      const std::string& reverse = reverse_owner->second.at(owner);
      const bool consistent =
          (state == "accepted" && reverse == "accepted") ||
          (state == "outgoing" && reverse == "incoming") ||
          (state == "incoming" && reverse == "outgoing") ||
          (state == "blocked" && reverse == "blocked_by") ||
          (state == "blocked" && reverse == "blocked") ||
          (state == "blocked_by" && reverse == "blocked") ||
          (state == "blocked_mutual" && reverse == "blocked_mutual");
      if (!consistent) {
        error = "relationship pair is inconsistent";
        return false;
      }
    }
  }
  for (const auto& [id, invite] : payload.at("invites").items()) {
    const auto custom = invite.contains("custom_data") && invite.at("custom_data").is_string()
                            ? DecodeBase64Url(invite.at("custom_data").get<std::string>())
                            : std::nullopt;
    const bool valid_keys =
        HasExactKeys(invite, {"id", "sender_xuid", "recipient_xuid", "session_id",
                              "custom_data", "revision", "state", "created_at",
                              "expires_at_unix"}) ||
        HasExactKeys(invite, {"id", "sender_xuid", "recipient_xuid", "session_id",
                              "custom_data", "revision", "state", "acknowledged",
                              "created_at", "expires_at_unix"});
    if (!valid_keys || invite.value("id", "") != id ||
        !IsHex(invite.value("sender_xuid", ""), 16) ||
        !IsHex(invite.value("recipient_xuid", ""), 16) ||
        invite.value("sender_xuid", "") == invite.value("recipient_xuid", "") ||
        !invite.at("session_id").is_string() ||
        !IsHex(invite.at("session_id").get<std::string>(), 16) ||
        !invite.at("custom_data").is_string() || !custom ||
        custom->size() > kMaximumInviteCustomBytes ||
        !invite.at("revision").is_number_integer() ||
        invite.at("revision").get<std::int64_t>() <= 0 ||
        (invite.value("state", "") != "pending" && invite.value("state", "") != "accepted") ||
        (invite.contains("acknowledged") && !invite.at("acknowledged").is_boolean()) ||
        !invite.at("created_at").is_string() || invite.value("created_at", "").empty() ||
        !invite.at("expires_at_unix").is_number_integer() ||
        invite.at("expires_at_unix").get<std::int64_t>() <= 0) {
      error = "invite is invalid";
      return false;
    }
    json normalized = invite;
    normalized["acknowledged"] = invite.value("acknowledged", false);
    parsed.invites.emplace(id, std::move(normalized));
  }
  for (const auto& [xuid, views] : payload.at("stats").items()) {
    if (!IsHex(xuid, 16) || !views.is_object() ||
        views.size() > libserver::Gta4StatViewCount()) {
      error = "stats owner is invalid";
      return false;
    }
    for (const auto& [view_id, columns] : views.items()) {
      if (!IsHex(view_id, 8) || !columns.is_object() ||
          columns.size() > kMaximumStatColumnsPerRow) {
        error = "stats view is invalid";
        return false;
      }
      const auto parsed_view_id = ParseHex32(Lower(view_id));
      if (!parsed_view_id) {
        error = "stats view is invalid";
        return false;
      }
      for (const auto& [stat_id, value] : columns.items()) {
        const auto parsed_stat_id = ParseHex32(Lower(stat_id));
        const auto* descriptor =
            parsed_stat_id
                ? libserver::FindGta4StatField(*parsed_view_id, *parsed_stat_id)
                : nullptr;
        if (!descriptor || !StatValueMatchesDescriptor(value, *descriptor)) {
          error = "stats column is invalid";
          return false;
        }
      }
      parsed.stats[xuid][view_id] = columns;
    }
  }
  for (const auto& [xuid, value] : payload.at("progression").items()) {
    std::int64_t cash = 0;
    std::uint32_t rank = 0;
    if (!IsHex(xuid, 16) || !HasExactKeys(value, {"cash", "rank", "updated_at"}) ||
        !JsonInt64(value.at("cash"), cash) ||
        !JsonUint32(value.at("rank"), rank) || rank >= kRankCashThresholds.size() ||
        rank != RankForCash(cash) ||
        !value.at("updated_at").is_string()) {
      error = "progression record is invalid";
      return false;
    }
    parsed.progression.emplace(xuid, ProgressionRecord{cash, rank, value.at("updated_at")});
  }
  for (const auto& [xuid, modes] : payload.at("mode_stats").items()) {
    if (!IsHex(xuid, 16) || !modes.is_object()) {
      error = "mode stats owner is invalid";
      return false;
    }
    for (const auto& [mode, aggregate] : modes.items()) {
      std::int64_t games = 0, wins = 0, score = 0;
      std::uint64_t kills = 0, deaths = 0;
      if (!ValidIdentifier(mode) ||
          !HasExactKeys(aggregate, {"games", "wins", "score", "kills", "deaths"}) ||
          !JsonInt64(aggregate.at("games"), games) || games < 0 ||
          !JsonInt64(aggregate.at("wins"), wins) || wins < 0 || wins > games ||
          !JsonInt64(aggregate.at("score"), score) ||
          !JsonUint64(aggregate.at("kills"), kills) ||
          !JsonUint64(aggregate.at("deaths"), deaths)) {
        error = "mode stats aggregate is invalid";
        return false;
      }
      parsed.mode_stats[xuid][mode] = aggregate;
    }
  }
  for (const auto& [result_id, value] : payload.at("ranked_results").items()) {
    if (!ValidIdentifier(result_id) ||
        !HasExactKeys(value, {"canonical_request", "response"}) ||
        !value.at("canonical_request").is_string() || !value.at("response").is_object()) {
      error = "ranked result is invalid";
      return false;
    }
    try {
      const json canonical = json::parse(value.at("canonical_request").get<std::string>());
      if (!canonical.is_object() ||
          canonical.dump() != value.at("canonical_request").get<std::string>()) {
        error = "ranked canonical request is invalid";
        return false;
      }
    } catch (...) {
      error = "ranked canonical request is invalid";
      return false;
    }
    parsed.ranked_result_requests[result_id] = value.at("canonical_request").get<std::string>();
    parsed.ranked_results[result_id] = value.at("response");
  }
  for (const auto& [xuid, profile] : payload.at("profiles").items()) {
    if (!IsHex(xuid, 16) ||
        !HasExactKeys(profile, {"xuid", "player_name", "appearances", "updated_at"}) ||
        profile.value("xuid", "") != xuid || !profile.at("player_name").is_string() ||
        !profile.at("appearances").is_object() || !profile.at("updated_at").is_string()) {
      error = "profile is invalid";
      return false;
    }
    for (const auto& [episode, appearance] : profile.at("appearances").items()) {
      if (!ValidEpisode(episode) ||
          !HasExactKeys(appearance, {"gender", "model", "clothing"}) ||
          (appearance.value("gender", "") != "male" &&
           appearance.value("gender", "") != "female") ||
          !appearance.at("model").is_string() || appearance.value("model", "").empty() ||
          !appearance.at("clothing").is_array() ||
          appearance.at("clothing").size() > kMaximumProfileClothingItems) {
        error = "profile appearance is invalid";
        return false;
      }
      for (const auto& item : appearance.at("clothing")) {
        if (!item.is_string() || item.get<std::string>().empty()) {
          error = "profile clothing is invalid";
          return false;
        }
      }
    }
    parsed.profiles[xuid] = profile;
  }
  if (schema >= kAchievementDurablePayloadSchema) {
    for (const auto& [xuid, wire_ids] : payload.at("achievements").items()) {
      if (!IsHex(xuid, 16) || !wire_ids.is_array() ||
          wire_ids.size() > kGta4AchievementCount) {
        error = "achievement owner is invalid";
        return false;
      }
      auto& unlocked = parsed.achievements[xuid];
      for (const auto& wire_id : wire_ids) {
        if (!wire_id.is_number_unsigned()) {
          error = "achievement id is invalid";
          return false;
        }
        const std::uint32_t id = wire_id.get<std::uint32_t>();
        if (id < kFirstGta4AchievementId || id > kLastGta4AchievementId ||
            !unlocked.insert(id).second) {
          error = "achievement id is invalid";
          return false;
        }
      }
    }
  }
  if (schema >= kEntitlementDurablePayloadSchema) {
    for (const auto& [xuid, wire_packages] : payload.at("entitlements").items()) {
      if (!IsHex(xuid, 16) || xuid != Lower(xuid) || !wire_packages.is_array() ||
          wire_packages.size() > kKnownGta4EpisodePackages.size()) {
        error = "entitlement owner is invalid";
        return false;
      }
      auto& granted = parsed.entitlements[xuid];
      for (const auto& wire_package : wire_packages) {
        if (!wire_package.is_string()) {
          error = "entitlement package is invalid";
          return false;
        }
        const std::string package = wire_package.get<std::string>();
        if (!IsKnownGta4EpisodePackage(package) || !granted.insert(package).second) {
          error = "entitlement package is invalid";
          return false;
        }
      }
    }
  }
  if (schema >= kProfileDurablePayloadSchema) {
    for (const auto& [key, value] : payload.at("stat_write_receipts").items()) {
      const std::size_t first_separator = key.find(':');
      const std::size_t second_separator =
          first_separator == std::string::npos ? std::string::npos : key.find(':', first_separator + 1);
      if (first_separator == std::string::npos || second_separator == std::string::npos ||
          key.find(':', second_separator + 1) != std::string::npos ||
          !HasExactKeys(value, {"request_digest", "status"}) ||
          !value.at("request_digest").is_string() ||
          !IsSha256Hex(value.at("request_digest").get<std::string>()) ||
          !value.at("status").is_number_integer() || value.at("status").get<int>() != 204) {
        error = "stat write receipt is invalid";
        return false;
      }
      const std::string xuid = key.substr(0, first_separator);
      const std::string session_id =
          key.substr(first_separator + 1, second_separator - first_separator - 1);
      const auto sequence = ParseDecimalUint64(key.substr(second_separator + 1));
      if (!IsHex(xuid, 16) || !IsHex(session_id, 16) || !sequence ||
          key != StatWriteReceiptKey(xuid, session_id, *sequence)) {
        error = "stat write receipt key is invalid";
        return false;
      }
      parsed.stat_write_receipts.emplace(
          key, StatWriteReceipt{value.at("request_digest").get<std::string>(), 204});
    }
    for (const auto& [key, value] : payload.at("stat_next_sequences").items()) {
      const std::size_t separator = key.find(':');
      if (separator == std::string::npos || key.find(':', separator + 1) != std::string::npos ||
          !value.is_string()) {
        error = "stat next sequence is invalid";
        return false;
      }
      const std::string xuid = key.substr(0, separator);
      const std::string session_id = key.substr(separator + 1);
      const auto next_sequence = ParseDecimalUint64(value.get<std::string>());
      if (!IsHex(xuid, 16) || !IsHex(session_id, 16) || !next_sequence || !*next_sequence ||
          key != StatSequenceOwnerKey(xuid, session_id)) {
        error = "stat next sequence key is invalid";
        return false;
      }
      parsed.stat_next_sequences.emplace(key, *next_sequence);
    }
    for (const auto& [receipt_key, receipt] : parsed.stat_write_receipts) {
      (void)receipt;
      const std::size_t separator = receipt_key.rfind(':');
      const std::string owner_key = receipt_key.substr(0, separator);
      const auto sequence = ParseDecimalUint64(receipt_key.substr(separator + 1));
      const auto next = parsed.stat_next_sequences.find(owner_key);
      if (!sequence || next == parsed.stat_next_sequences.end() || *sequence >= next->second) {
        error = "stat write receipt has no matching allocated sequence";
        return false;
      }
    }
    for (const auto& [xuid, value] : payload.at("title_profile_settings").items()) {
      if (!IsHex(xuid, 16) ||
          !HasExactKeys(value, {"title_id", "setting_id", "revision", "blob", "digest",
                                "updated_at"}) ||
          value.value("title_id", "") != "0x545407f2" ||
          value.value("setting_id", "") != "0x63e83fff" ||
          !value.at("revision").is_string() || !value.at("blob").is_string() ||
          !value.at("digest").is_string() ||
          !IsSha256Hex(value.at("digest").get<std::string>()) ||
          !value.at("updated_at").is_string() || value.value("updated_at", "").empty()) {
        error = "title profile setting is invalid";
        return false;
      }
      const auto revision = ParseDecimalUint64(value.at("revision").get<std::string>());
      const std::string blob = value.at("blob").get<std::string>();
      const auto decoded = DecodeBase64Url(blob);
      const auto digest = decoded
                              ? Sha256Hex(std::string_view(
                                    reinterpret_cast<const char*>(decoded->data()), decoded->size()))
                              : std::nullopt;
      if (!revision || !*revision || !decoded || !IsValidGta4TitleProfileBlob(*decoded) ||
          !digest || *digest != value.at("digest").get<std::string>()) {
        error = "title profile setting payload is invalid";
        return false;
      }
      parsed.title_profile_settings.emplace(
          xuid, TitleProfileSettingRecord{*revision, blob, *digest,
                                          value.at("updated_at").get<std::string>()});
    }
  }
  if (schema == kDurablePayloadSchema) {
    for (const auto& [key, value] : payload.at("invite_accept_receipts").items()) {
      if (!IsSha256Hex(key) ||
          !HasExactKeys(value, {"recipient_xuid", "invite_id", "idempotency_key",
                                "request_digest", "status", "response"}) ||
          !value.at("recipient_xuid").is_string() ||
          !value.at("invite_id").is_string() ||
          !value.at("idempotency_key").is_string() ||
          !value.at("request_digest").is_string() ||
          !value.at("status").is_number_integer() || value.at("status").get<int>() != 200 ||
          !value.at("response").is_object() ||
          !HasExactKeys(value.at("response"), {"invite", "session"})) {
        error = "invite accept receipt is invalid";
        return false;
      }
      const std::string recipient = value.at("recipient_xuid").get<std::string>();
      const std::string invite_id = value.at("invite_id").get<std::string>();
      const std::string idempotency_key = value.at("idempotency_key").get<std::string>();
      const std::string request_digest = value.at("request_digest").get<std::string>();
      const json& response = value.at("response");
      const json& invite = response.at("invite");
      const auto expected_key =
          InviteAcceptReceiptKey(recipient, invite_id, idempotency_key);
      if (!IsHex(recipient, 16) || recipient != Lower(recipient) ||
          !ValidIdentifier(invite_id) ||
          !ValidIdentifier(idempotency_key, kMaximumIdempotencyKeyBytes) ||
          !IsSha256Hex(request_digest) || !expected_key || *expected_key != key ||
          !invite.is_object() || invite.value("id", "") != invite_id ||
          invite.value("recipient_xuid", "") != recipient ||
          !invite.value("acknowledged", false) || !response.at("session").is_object()) {
        error = "invite accept receipt payload is invalid";
        return false;
      }
      parsed.invite_accept_receipts.emplace(
          key, InviteAcceptReceipt{recipient, invite_id, idempotency_key, request_digest,
                                   response});
    }
    for (const auto& [xuid, value] : payload.at("prog_ach_records").items()) {
      if (!IsHex(xuid, 16) || xuid != Lower(xuid) ||
          !HasExactKeys(value, {"title_id", "facility", "path", "blob", "digest",
                                "updated_at"}) ||
          value.value("title_id", "") != "0x545407F2" ||
          !value.at("facility").is_number_integer() || value.at("facility").get<int>() != 3 ||
          value.value("path", "") != "Prog_ACH" || !value.at("blob").is_string() ||
          !value.at("digest").is_string() ||
          !IsSha256Hex(value.at("digest").get<std::string>()) ||
          !value.at("updated_at").is_string() || value.value("updated_at", "").empty()) {
        error = "Prog_ACH title storage record is invalid";
        return false;
      }
      const std::string blob = value.at("blob").get<std::string>();
      const auto decoded = DecodeCanonicalProgAchBlob(blob);
      const auto digest =
          decoded ? Sha256Hex(std::string_view(
                        reinterpret_cast<const char*>(decoded->data()), decoded->size()))
                  : std::nullopt;
      if (!digest || *digest != value.at("digest").get<std::string>()) {
        error = "Prog_ACH title storage payload is invalid";
        return false;
      }
      parsed.prog_ach_records.emplace(
          xuid, ProgAchRecord{blob, *digest, value.at("updated_at").get<std::string>()});
    }
  }

  // Schema 1-3 stored ranked cash in a disconnected progression collection.
  // Migrate it once into the title's authoritative view 109 Score property only
  // when no canonical value exists. Unmapped legacy per-mode aggregates remain
  // intact as audit data rather than being assigned guessed view IDs.
  if (schema < kProfileDurablePayloadSchema) {
    for (const auto& [xuid, record] : parsed.progression) {
      json& view = parsed.stats[xuid]["0x0000006d"];
      if (!view.contains("0x2000000d")) {
        view["0x2000000d"] = {{"type", "i64"}, {"value", record.cash}};
      }
    }
  }
  for (const auto& [xuid, record] : parsed.progression) {
    const auto player = parsed.stats.find(xuid);
    if (player == parsed.stats.end() || !player->second.contains("0x0000006d") ||
        !player->second.at("0x0000006d").contains("0x2000000d")) {
      error = "progression record has no canonical view 109 cash";
      return false;
    }
    std::int64_t cash = 0;
    if (!JsonInt64(player->second.at("0x0000006d").at("0x2000000d").at("value"), cash) ||
        cash != record.cash || RankForCash(cash) != record.rank) {
      error = "progression record disagrees with canonical view 109";
      return false;
    }
  }
  for (const auto& [xuid, views] : parsed.stats) {
    const auto money_view = views.find("0x0000006d");
    if (money_view == views.end() || !money_view->second.contains("0x2000000d")) continue;
    std::int64_t cash = 0;
    if (!JsonInt64(money_view->second.at("0x2000000d").at("value"), cash)) {
      error = "canonical view 109 cash is invalid";
      return false;
    }
    const std::uint32_t rank = RankForCash(cash);
    if (money_view->second.contains("0x10008001")) {
      std::int64_t stored_rank = 0;
      if (!JsonInt64(money_view->second.at("0x10008001").at("value"), stored_rank) ||
          stored_rank != rank) {
        error = "canonical view 109 rank disagrees with cash";
        return false;
      }
    }
    const auto progression = parsed.progression.find(xuid);
    if (progression == parsed.progression.end() || progression->second.cash != cash ||
        progression->second.rank != rank) {
      error = "canonical view 109 has no matching progression projection";
      return false;
    }
  }
  result = std::move(parsed);
  return true;
  } catch (const json::exception&) {
    error = "durable payload contains a value with the wrong type";
    return false;
  }
}

class Service {
  friend struct ServiceRegression;
 public:
  explicit Service(libserver::PersistentState* persistent_state = nullptr,
                   std::string public_url = "http://127.0.0.1:8080")
      : persistent_state_(persistent_state), public_url_(std::move(public_url)) {}

  ~Service() {
    {
      std::lock_guard lock(mutex_);
      sweeper_stopping_ = true;
      realtime_events_stopping_ = true;
    }
    sweeper_condition_.notify_all();
    realtime_event_condition_.notify_all();
    if (sweeper_.joinable()) sweeper_.join();
  }

  bool Initialize() {
    std::lock_guard lock(mutex_);
    if (initialized_) return true;
    if (persistent_state_) {
      DurableData loaded;
      const json& payload = persistent_state_->payload();
      if (!persistent_state_->has_snapshot()) {
        if (!persistent_state_->Save(SerializeDurable(loaded))) return false;
      } else if (!DeserializeDurable(payload, loaded, initialization_error_)) {
        return false;
      } else if (!loaded.invites.empty() || !loaded.invite_accept_receipts.empty() ||
                 !loaded.stat_write_receipts.empty() || !loaded.stat_next_sequences.empty()) {
        // Sessions are intentionally ephemeral. No invite or stat idempotency domain can remain
        // valid when the process restarts without the session that scoped it.
        loaded.invites.clear();
        loaded.invite_accept_receipts.clear();
        loaded.stat_write_receipts.clear();
        loaded.stat_next_sequences.clear();
        if (!persistent_state_->Save(SerializeDurable(loaded))) return false;
      }
      // Canonicalize collision-free legacy identities before serving any account.
      // Deserialize rejects aliases with multiple owners without changing disk.
      const json canonical = SerializeDurable(loaded);
      if (persistent_state_->payload() != canonical && !persistent_state_->Save(canonical)) {
        return false;
      }
      CommitDurableLocked(std::move(loaded));
    }
    const auto cursor_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
                                  SystemClock::now().time_since_epoch())
                                  .count();
    if (cursor_epoch <= 0 ||
        static_cast<std::uint64_t>(cursor_epoch) >=
            static_cast<std::uint64_t>(
                std::numeric_limits<std::int64_t>::max())) {
      initialization_error_ = "system clock cannot seed realtime event cursors";
      return false;
    }
    realtime_event_cursor_floor_ = static_cast<std::uint64_t>(cursor_epoch);
    next_realtime_event_id_ = realtime_event_cursor_floor_;
    try {
      sweeper_stopping_ = false;
      realtime_events_stopping_ = false;
      sweeper_ = std::thread(&Service::SweeperMain, this);
    } catch (...) {
      initialization_error_ = "resource sweeper could not be started";
      return false;
    }
    initialized_ = true;
    return true;
  }

  bool GrantEntitlement(std::string xuid, std::string package) {
    xuid = Lower(std::move(xuid));
    if (!IsHex(xuid, 16) || !IsKnownGta4EpisodePackage(package)) return false;
    std::lock_guard lock(mutex_);
    if (!initialized_) return false;
    const auto owner = entitlements_.find(xuid);
    if (owner != entitlements_.end() && owner->second.contains(package)) return true;
    DurableData staged = CaptureDurableLocked();
    staged.entitlements[std::move(xuid)].insert(std::move(package));
    return SaveAndCommitLocked(std::move(staged));
  }

  std::uint64_t ActivityGeneration() const noexcept { return activity_generation_.load(); }
  std::string InitializationError() const { return initialization_error_; }
  std::optional<std::string> WaitingOwner(const Request& request) {
    const auto identity = Authenticate(request);
    return identity ? std::optional(identity->xuid) : std::nullopt;
  }

  Response Dispatch(const Request& wire_request, bool polling = false) {
    Request request = wire_request;
    request.path = libserver::CanonicalIdentityPath(std::move(request.path));
    for (auto& [key, value] : request.query) {
      if (libserver::IdentityValueField(key) || key == "cursor") {
        value = libserver::CanonicalIdentity(std::move(value));
      }
    }
    Response response = DispatchCanonical(request);
    if (!polling && request.method != "GET") ++activity_generation_;
    return response;
  }

  Response DispatchCanonical(const Request& request) {
    if (request.method == "GET" && request.path == "/health/live") {
      return {.body = {{"status", "live"}, {"service", "libserver"}, {"maximum_session_members", 64}}};
    }
    if (request.method == "GET" && request.path == "/health/ready") {
      std::lock_guard lock(mutex_);
      const bool ready = initialized_ &&
                         (!persistent_state_ || persistent_state_->readable());
      json body = {{"status", ready ? "ready" : "error"}, {"public_url", public_url_}};
      if (persistent_state_) body["storage"] = persistent_state_->HealthJson();
      if (!ready) {
        body["error"] = persistent_state_
                            ? libserver::PersistentState::ErrorCodeName(
                                  persistent_state_->error_code())
                            : "not_initialized";
        if (!initialization_error_.empty()) body["detail"] = initialization_error_;
      }
      return {.status = ready ? 200 : 503, .body = std::move(body)};
    }
    {
      std::lock_guard lock(mutex_);
      if (!initialized_ || (persistent_state_ && !persistent_state_->readable())) {
        return Error(503, "storage_unavailable", "durable storage is not ready");
      }
    }
    if (request.path == "/api/v2/devices/challenge" && request.method == "POST") return ChallengeDevice(request);
    if (request.path == "/api/v2/devices/enroll" && request.method == "POST") return EnrollDevice(request);
    if (request.path == "/api/v2/devices/refresh" && request.method == "POST") return RefreshDevice(request);

    const auto identity = Authenticate(request);
    if (!identity) return Error(401, "unauthorized", "a valid bearer token is required");
    if (request.path == "/api/v2/sessions" && request.method == "POST") {
      return TraceSessionOperation(request, *identity, CreateSession(request, *identity));
    }
    if (request.path == "/api/v2/sessions/search" && request.method == "POST") {
      return TraceSessionOperation(request, *identity, SearchSessions(request, *identity));
    }
    if (request.path.starts_with("/api/v2/sessions/by-xbox-id/") && request.method == "GET") {
      return TraceSessionOperation(
          request, *identity,
          GetSession(SegmentAfter(request.path, "/api/v2/sessions/by-xbox-id/"), *identity));
    }
    if (request.path.starts_with("/api/v2/sessions/")) {
      return TraceSessionOperation(request, *identity, SessionOperation(request, *identity));
    }
    if (request.path.starts_with("/api/v2/lobbies/")) return LobbyOperation(request, *identity);
    if (request.path == "/api/v2/matchmaking/tickets" && request.method == "POST") {
      return CreateMatchmakingTicket(request, *identity);
    }
    if (request.path.starts_with("/api/v2/matchmaking/tickets/")) {
      return MatchmakingTicketOperation(request, *identity);
    }
    if (request.path == "/api/v2/relay/routes") return RelayRoutes(request, *identity);
    if (request.path == "/api/v2/relay/datagrams") return RelayDatagrams(request, *identity);
    if (request.path == "/api/v2/qos/listeners") return QosListeners(request, *identity);
    if (request.path == "/api/v2/qos/lookup" && request.method == "POST") {
      return QosLookup(request, *identity);
    }
    if (request.path == "/api/v3/qos/probes") return QosProbes(request, *identity);
    if (request.path == "/api/v3/qos/ack" && request.method == "POST") {
      return QosAcknowledge(request, *identity);
    }
    if (request.path == "/api/v2/friends/check" && request.method == "POST") return FriendsCheck(request, *identity);
    if (request.path == "/api/v2/friends" && request.method == "GET") {
      return FriendsList(request, *identity);
    }
    if (request.path == "/api/v2/friends/relationships" && request.method == "POST") {
      return FriendRelationship(request, *identity);
    }
    if (request.path == "/api/v2/invites" && request.method == "POST") return CreateInvites(request, *identity);
    if (request.path == "/api/v2/invites" && request.method == "GET") {
      return ListInvites(request, *identity);
    }
    if (request.method == "POST" && request.path.starts_with("/api/v2/invites/") &&
        request.path.ends_with("/accept")) {
      return AcceptInvite(request, *identity);
    }
    if (request.path == "/api/v2/stats/writes" && request.method == "POST") return WriteStats(request, *identity);
    if (request.path == "/api/v2/stats/sequences" && request.method == "POST") {
      return AllocateStatSequence(request, *identity);
    }
    if (request.path == "/api/v2/stats/read" && request.method == "POST") return ReadStats(request);
    if (request.path == "/api/v2/stats/reset" && request.method == "POST") return ResetStats(request, *identity);
    if (request.path == "/api/v2/stats/skill" && request.method == "POST") return Skill(request);
    if (request.path == "/api/v2/progression/results" && request.method == "POST") {
      return RankedResult(request, *identity);
    }
    if (request.path.starts_with("/api/v2/progression/") && request.method == "GET") {
      return ProgressionGet(SegmentAfter(request.path, "/api/v2/progression/"));
    }
    if (request.path == "/api/v2/profiles/me" && request.method == "PUT") {
      return ProfilePut(request, *identity);
    }
    if (request.path.starts_with("/api/v2/profiles/") && request.method == "GET") {
      return ProfileGet(SegmentAfter(request.path, "/api/v2/profiles/"));
    }
    if (request.path == "/api/v3/profile/title-settings/0x63e83fff") {
      return TitleProfileSetting(request, *identity);
    }
    if (request.path == "/api/v3/storage/0x545407F2/3/Prog_ACH") {
      if (request.method == "GET" || request.method == "PUT") {
        return ProgAchStorage(request, *identity);
      }
      return Error(404, "not_found", "the requested endpoint does not exist");
    }
    if (request.path == "/api/v2/achievements") {
      return Achievements(request, *identity);
    }
    if (request.path == "/api/v2/entitlements") {
      return Entitlements(request, *identity);
    }
    if (request.path == "/api/v2/presence" && request.method == "PUT") {
      return PresencePut(request, *identity);
    }
    if (request.path.starts_with("/api/v2/presence/") && request.method == "GET") {
      return PresenceGet(SegmentAfter(request.path, "/api/v2/presence/"));
    }
    if (request.path.starts_with("/api/v2/leaderboards/") && request.method == "GET") return Leaderboard(request, *identity);
    if (request.path == "/api/v2/chat/messages" && request.method == "POST") {
      return ChatMessage(request, *identity);
    }
    if (request.path == "/api/v2/events" && request.method == "GET") {
      return RealtimeEvents(request, *identity);
    }
    if (request.path == "/api/v2/voice/routes") return VoiceRoutes(request, *identity);
    if (request.path == "/api/v2/voice/packets") return VoicePackets(request, *identity);
    return Error(404, "not_found", "the requested endpoint does not exist");
  }

  bool VerifyParitySelfTest() {
    const Identity host{.device_id = "host", .xuid = "0x0000000000000001",
                        .machine_id = "0x0000000000000011", .player_name = "Host"};
    const Identity peer{.device_id = "peer", .xuid = "0x0000000000000002",
                        .machine_id = "0x0000000000000022", .player_name = "Peer"};
    const Identity third{.device_id = "third", .xuid = "0x0000000000000003",
                         .machine_id = "0x0000000000000033", .player_name = "Third"};
    const Identity pending{.device_id = "pending", .xuid = "0x0000000000000004",
                           .machine_id = "0x0000000000000044", .player_name = "Pending"};
    const Identity blocked{.device_id = "blocked", .xuid = "0x0000000000000007",
                           .machine_id = "0x0000000000000077", .player_name = "Blocked"};
    player_names_[host.xuid] = host.player_name;
    player_names_[peer.xuid] = peer.player_name;
    player_names_[third.xuid] = third.player_name;
    player_names_[pending.xuid] = pending.player_name;
    player_names_[blocked.xuid] = blocked.player_name;

    json ranked_public_policy = {{"flags", 62}, {"ranked", false},
                                 {"join_in_progress", false}, {"visibility", "private"}};
    json unranked_public_policy = {{"flags", 46}, {"ranked", true},
                                   {"join_in_progress", false}, {"visibility", "private"}};
    json private_no_join_policy = {{"flags", 1798}, {"ranked", true},
                                   {"join_in_progress", true}, {"visibility", "public"}};
    json lobby_lifecycle = {{"lifecycle_state", 0}};
    json registration_lifecycle = {{"lifecycle_state", 1}};
    json game_lifecycle = {{"lifecycle_state", 2}};
    json reporting_lifecycle = {{"lifecycle_state", 3}};
    json state_only = {{"state", "in_game"}};
    json contradictory_lifecycle = {{"state", "lobby"}, {"lifecycle_state", 2}};
    if (!CanonicalizeSessionFlags(ranked_public_policy) ||
        !ranked_public_policy.value("ranked", false) ||
        !ranked_public_policy.value("join_in_progress", false) ||
        ranked_public_policy.value("visibility", "") != "public" ||
        !CanonicalizeSessionFlags(unranked_public_policy) ||
        unranked_public_policy.value("ranked", true) ||
        !unranked_public_policy.value("join_in_progress", false) ||
        unranked_public_policy.value("visibility", "") != "public" ||
        !CanonicalizeSessionFlags(private_no_join_policy) ||
        private_no_join_policy.value("ranked", true) ||
        private_no_join_policy.value("join_in_progress", true) ||
        private_no_join_policy.value("visibility", "") != "private" ||
        !NormalizeSessionLifecycle(lobby_lifecycle, false, true) ||
        lobby_lifecycle.value("state", "") != "lobby" ||
        !NormalizeSessionLifecycle(registration_lifecycle, false, true) ||
        registration_lifecycle.value("state", "") != "open" ||
        !NormalizeSessionLifecycle(game_lifecycle, false, true) ||
        game_lifecycle.value("state", "") != "in_game" ||
        !NormalizeSessionLifecycle(reporting_lifecycle, false, true) ||
        reporting_lifecycle.value("state", "") != "closed" ||
        !NormalizeSessionLifecycle(state_only, true, false) ||
        state_only.value("lifecycle_state", 0u) != 2 ||
        NormalizeSessionLifecycle(contradictory_lifecycle, true, true)) {
      return false;
    }

    Request friend_request{.method = "POST", .path = "/api/v2/friends/relationships",
                           .body = json({{"target_xuid", peer.xuid}, {"action", "request"}}).dump()};
    Request friend_accept{.method = "POST", .path = "/api/v2/friends/relationships",
                          .body = json({{"target_xuid", host.xuid}, {"action", "accept"}}).dump()};
    if (FriendRelationship(friend_request, host).status != 200 ||
        FriendRelationship(friend_accept, peer).status != 200 ||
        relationships_.at(host.xuid).at(peer.xuid) != "accepted") return false;
    Request pending_request{.method = "POST", .path = "/api/v2/friends/relationships",
                            .body = json({{"target_xuid", pending.xuid}, {"action", "request"}}).dump()};
    if (FriendRelationship(pending_request, host).status != 200) return false;
    const std::string unknown_xuid = "0x0000000000000005";
    Request invalid_relationship{.method = "POST", .path = "/api/v2/friends/relationships",
                                 .body = json({{"target_xuid", unknown_xuid},
                                               {"action", "invalid"}}).dump()};
    if (relationships_.contains(unknown_xuid) ||
        FriendRelationship(invalid_relationship, host).status != 400 ||
        relationships_.contains(unknown_xuid)) return false;
    Request block_third{.method = "POST", .path = "/api/v2/friends/relationships",
                        .body = json({{"target_xuid", blocked.xuid},
                                      {"action", "block"}}).dump()};
    if (FriendRelationship(block_third, host).status != 200 ||
        relationships_.at(host.xuid).at(blocked.xuid) != "blocked" ||
        relationships_.at(blocked.xuid).at(host.xuid) != "blocked_by") {
      return false;
    }
    Request first_friend_page{.query = {{"limit", "1"}}};
    const Response first_page = FriendsList(first_friend_page, host);
    Request accepted_friend_page{
        .query = {{"limit", "1"}, {"state", "accepted"}}};
    const Response accepted_page = FriendsList(accepted_friend_page, host);
    Request remaining_friend_page{
        .query = {{"limit", "2"}, {"cursor", peer.xuid}}};
    const Response remaining_page = FriendsList(remaining_friend_page, host);
    const Response inverse_list = FriendsList({}, blocked);
    Request invalid_friend_state{.query = {{"state", "muted"}}};
    if (first_page.status != 200 || first_page.body["items"].size() != 1 ||
        first_page.body["items"][0].value("xuid", "") != peer.xuid ||
        first_page.body["items"][0].value("state", "") != "accepted" ||
        first_page.body["items"][0].value("is_blocked", true) ||
        first_page.body.value("next_cursor", "") != peer.xuid ||
        accepted_page.status != 200 || accepted_page.body["items"].size() != 1 ||
        accepted_page.body["items"][0].value("xuid", "") != peer.xuid ||
        !accepted_page.body.value("next_cursor", "").empty() ||
        remaining_page.status != 200 || remaining_page.body["items"].size() != 2 ||
        remaining_page.body["items"][0].value("state", "") != "outgoing" ||
        remaining_page.body["items"][1].value("state", "") != "blocked" ||
        !remaining_page.body["items"][1].value("is_blocked", false) ||
        !remaining_page.body.value("next_cursor", "").empty() ||
        inverse_list.body["items"].size() != 1 ||
        inverse_list.body["items"][0].value("state", "") != "blocked_by" ||
        !inverse_list.body["items"][0].value("is_blocked", false) ||
        FriendsList(invalid_friend_state, host).status != 400) {
      std::cerr << "libserver self-test: directional friend pages failed: first="
                << first_page.body.dump() << " accepted=" << accepted_page.body.dump()
                << " remaining=" << remaining_page.body.dump()
                << " inverse=" << inverse_list.body.dump() << '\n';
      return false;
    }
    Request friend_check{
        .body = json({{"xuids", json::array({peer.xuid, pending.xuid,
                                               blocked.xuid})}})
                    .dump()};
    const Response friend_check_response = FriendsCheck(friend_check, host);
    Request empty_friend_check{.body = json({{"xuids", json::array()}}).dump()};
    Request zero_friend_check{
        .body = json({{"xuids", json::array({"0x0000000000000000"})}}).dump()};
    Request duplicate_friend_check{
        .body = json({{"xuids", json::array({peer.xuid, peer.xuid})}}).dump()};
    Request oversize_friend_check{
        .body = json({{"xuids", std::vector<std::string>(
                                      kOversizeFriendsCheckItems, peer.xuid)}})
                    .dump()};
    if (friend_check_response.status != 200 ||
        friend_check_response.body["results"].size() != 3 ||
        !friend_check_response.body["results"][0].value("is_friend", false) ||
        friend_check_response.body["results"][0].value("is_blocked", true) ||
        friend_check_response.body["results"][1].value("is_friend", true) ||
        friend_check_response.body["results"][1].value("is_blocked", true) ||
        friend_check_response.body["results"][2].value("is_friend", true) ||
        !friend_check_response.body["results"][2].value("is_blocked", false) ||
        FriendsCheck(empty_friend_check, host).status != 400 ||
        FriendsCheck(zero_friend_check, host).status != 400 ||
        FriendsCheck(duplicate_friend_check, host).status != 400 ||
        FriendsCheck(oversize_friend_check, host).status != 400) {
      return false;
    }
    Request unblock_test_identity{
        .method = "POST", .path = "/api/v2/friends/relationships",
        .body = json({{"target_xuid", blocked.xuid}, {"action", "unblock"}}).dump()};
    if (FriendRelationship(unblock_test_identity, host).status != 200 ||
        relationships_.at(host.xuid).contains(blocked.xuid) ||
        relationships_.at(blocked.xuid).contains(host.xuid)) {
      return false;
    }
    player_names_.erase(blocked.xuid);
    relationships_.erase(blocked.xuid);
    if (progression_.contains(unknown_xuid) || ProgressionGet(unknown_xuid).status != 200 ||
        progression_.contains(unknown_xuid)) return false;
    Request reset_unknown{.method = "POST", .path = "/api/v2/stats/reset",
                          .body = json({{"view_id", "0x00000001"}}).dump()};
    if (stats_.contains(unknown_xuid) || ResetStats(reset_unknown,
          Identity{.device_id = "unknown", .xuid = unknown_xuid,
                   .machine_id = "0x0000000000000055", .player_name = "Unknown"}).status != 204 ||
        stats_.contains(unknown_xuid)) return false;

    Request profile{.method = "PUT", .path = "/api/v2/profiles/me",
                    .body = json({{"episode", "base"},
                                  {"appearance", {{"gender", "male"}, {"model", "player"},
                                                   {"clothing", json::array({"rank_0"})}}}}).dump()};
    if (ProfilePut(profile, host).status != 200) return false;

    Request title_setting_get{
        .method = "GET", .path = "/api/v3/profile/title-settings/0x63e83fff"};
    if (TitleProfileSetting(title_setting_get, host).status != 404) return false;
    Request title_setting_put{
        .method = "PUT",
        .path = "/api/v3/profile/title-settings/0x63e83fff",
        .body = json({{"title_id", "0x545407f2"},
                      {"setting_id", "0x63e83fff"},
                      {"expected_revision", "0"},
                      {"blob", "AAABkAECAwQAAAH2oLDA0A"}})
                    .dump()};
    const Response stored_title_setting = TitleProfileSetting(title_setting_put, host);
    if (stored_title_setting.status != 200 ||
        stored_title_setting.body.value("revision", "") != "1" ||
        stored_title_setting.body.value("blob", "") != "AAABkAECAwQAAAH2oLDA0A" ||
        TitleProfileSetting(title_setting_put, host).status != 412 ||
        TitleProfileSetting(title_setting_get, host).body != stored_title_setting.body) {
      return false;
    }
    Request duplicate_title_keys{
        .method = "PUT",
        .path = "/api/v3/profile/title-settings/0x63e83fff",
        .body = json({{"title_id", "0x545407f2"},
                      {"setting_id", "0x63e83fff"},
                      {"expected_revision", "1"},
                      {"blob", "AAABkAECAwQAAAGQoLDA0A"}})
                    .dump()};
    if (TitleProfileSetting(duplicate_title_keys, host).status != 400 ||
        ProfileGet(host.xuid).body["appearances"].contains("base") == false) {
      return false;
    }

    Request presence{.method = "PUT", .path = "/api/v2/presence",
                     .body = json({{"state", "online"}, {"session_id", ""},
                                   {"episode", "base"}}).dump()};
    Request accepted_relationships{
        .query = {{"limit", "1"}, {"state", "accepted"}}};
    const Response stored_online_presence = PresencePut(presence, host);
    const Response online_friend_presence = FriendsList(accepted_relationships, peer);
    if (stored_online_presence.status != 200 ||
        PresenceGet(host.xuid).status != 200 ||
        online_friend_presence.status != 200 ||
        online_friend_presence.body["items"].size() != 1 ||
        online_friend_presence.body["items"][0].value("presence", "") != "online" ||
        !online_friend_presence.body["items"][0].value("session_id", "").empty()) {
      return false;
    }

    const std::string session_id = "0x0000000000000100";
    sessions_[session_id] = {{"session_id", session_id}, {"host_xuid", host.xuid},
                             {"title_id", "0x545407F2"}, {"media_id", "0x00000000"},
                             {"title_version", "0x00000001"}, {"protocol_version", 2},
                             {"state", "open"}, {"visibility", "public"},
                             {"mode", "gta4"}, {"episode", "all"},
                             {"region", "global"},
                             {"public_slots", 64}, {"private_slots", 0}, {"ranked", true},
                             {"procedure_index", 0}, {"join_in_progress", false},
                             {"flags", 1086}, {"lifecycle_state", 1},
                             {"contexts", {{"0x00000001", 7}}},
                             {"properties", {{"0x00000002", "AQ=="}}},
                             {"revision", 1}, {"host_epoch", 1},
                             {"members", json::array({{{"xuid", host.xuid}}, {{"xuid", peer.xuid}},
                                                       {{"xuid", third.xuid}}})}};
    Request session_presence{
        .method = "PUT",
        .path = "/api/v2/presence",
        .body = json({{"state", "in_game"}, {"session_id", session_id},
                      {"episode", "base"}})
                    .dump()};
    const Response stored_session_presence = PresencePut(session_presence, host);
    const Response playing_friend_presence = FriendsList(accepted_relationships, peer);
    if (stored_session_presence.status != 200 ||
        playing_friend_presence.status != 200 ||
        playing_friend_presence.body["items"].size() != 1 ||
        playing_friend_presence.body["items"][0].value("presence", "") != "playing" ||
        playing_friend_presence.body["items"][0].value("session_id", "") != session_id ||
        playing_friend_presence.body["items"][0].value("episode", "") != "base") {
      return false;
    }
    invites_["invite_missing_session"] = {
        {"id", "invite_missing_session"}, {"sender_xuid", host.xuid},
        {"recipient_xuid", peer.xuid}, {"session_id", "0x0000000000000999"},
        {"custom_data", ""}, {"revision", 1}, {"state", "pending"},
        {"created_at", UtcIsoAfter()},
        {"expires_at_unix", UnixSecondsAfter(kInviteLifetime)}};
    Request missing_invite_session{
        .method = "POST", .path = "/api/v2/invites/invite_missing_session/accept"};
    if (AcceptInvite(missing_invite_session, peer).status != 404 ||
        invites_.contains("invite_missing_session")) return false;
    invites_["invite_second_pending"] = {
        {"id", "invite_second_pending"}, {"sender_xuid", host.xuid},
        {"recipient_xuid", peer.xuid}, {"session_id", session_id},
        {"custom_data", ""}, {"revision", 1}, {"state", "pending"},
        {"acknowledged", false}, {"created_at", UtcIsoAfter()},
        {"expires_at_unix", UnixSecondsAfter(kInviteLifetime)}};
    Request one_pending_invite{.query = {{"limit", "1"}, {"state", "pending"}}};
    const Response pending_invite_page = ListInvites(one_pending_invite, peer);
    if (pending_invite_page.status != 200 || pending_invite_page.body["items"].size() != 1) {
      return false;
    }

    const json valid_stat_column = {{"type", "i32"}, {"value", 7}};
    const json second_stat_column = {{"type", "i64"}, {"value", 8}};
    const json valid_stat_row = {
        {"xuid", host.xuid},
        {"columns", {{"0x1000000e", valid_stat_column},
                     {"0x1000000f", {{"type", "i32"}, {"value", 100}}},
                     {"0x20000037", {{"type", "i64"}, {"value", 50}}}}}};
    const json mismatched_stat_row = {
        {"xuid", peer.xuid}, {"columns", {{"0x2000000d", second_stat_column}}}};
    // Aggregation tests start from an already registered match. HTTP tests
    // independently exercise registration, invalid starts and result admission.
    const json lobby_before_stat_tests = sessions_.at(session_id);
    sessions_.at(session_id)["lifecycle_state"] = 2;
    sessions_.at(session_id)["state"] = "in_game";
    ArbitrationState stats_registration;
    for (const auto& participant : {host, peer, third}) {
      stats_registration.expected_machine_ids.insert(participant.machine_id);
      stats_registration.registered_machine_ids.insert(participant.machine_id);
      stats_registration.registered_xuids.insert(participant.xuid);
      stats_registration.authorized_xuid_machines.emplace(participant.xuid, participant.machine_id);
    }
    stats_registration.snapshot = sessions_.at(session_id);
    arbitration_snapshots_[session_id] = std::move(stats_registration);
    ranked_started_.insert(session_id);
    Request allocate_stats{.method = "POST", .path = "/api/v2/stats/sequences",
                           .body = json({{"session_id", session_id}}).dump()};
    const auto single_stat_write_body = [&](std::string_view sequence_value,
                                            std::string_view view_id,
                                            json columns) {
      return SingleStatWriteRequest(session_id, sequence_value, host.xuid, view_id,
                                    std::move(columns))
          .dump();
    };
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "1") {
      return false;
    }
    const json atomic_stats_body = {
        {"session_id", session_id}, {"sequence", "1"},
        {"views", json::array({
            json{{"view_id", "0x00000001"}, {"rows", json::array({valid_stat_row})}},
            json{{"view_id", "0x00000002"}, {"rows", json::array({mismatched_stat_row})}}})}};
    Request atomic_stats_failure{.method = "POST", .path = "/api/v2/stats/writes",
                                 .body = atomic_stats_body.dump()};
    if (WriteStats(atomic_stats_failure, host).status != 400 || stats_.contains(host.xuid)) {
      return false;
    }
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "2") {
      return false;
    }
    json valid_stats_body = {
        {"session_id", session_id}, {"sequence", "2"},
        {"mode", "deathmatch"}, {"ranked", true}, {"procedure_index", 0},
        {"flags", 1086}, {"lifecycle_state", 2},
        {"expected_revision", 1}, {"expected_host_epoch", 1},
        {"contexts", {{"0x00000001", 7}}},
        {"properties", {{"0x00000002", "AQ=="}}},
        {"views", json::array({
            json{{"view_id", "0x00000001"}, {"rows", json::array({valid_stat_row})}}})}};
    json mismatched_policy_body = valid_stats_body;
    mismatched_policy_body["procedure_index"] = 1;
    Request mismatched_policy_stats{.method = "POST", .path = "/api/v2/stats/writes",
                                    .body = mismatched_policy_body.dump()};
    Request valid_stats{.method = "POST", .path = "/api/v2/stats/writes",
                        .body = valid_stats_body.dump()};
    if (WriteStats(mismatched_policy_stats, host).status != 409 ||
        WriteStats(valid_stats, host).status != 204 ||
        !stats_.at(host.xuid).contains("0x00000001")) return false;
    if (WriteStats(valid_stats, host).status != 204 ||
        stats_.at(host.xuid).at("0x00000001").at("0x1000000e").at("value") != 7) {
      return false;
    }
    json conflicting_stats_body = valid_stats_body;
    conflicting_stats_body["views"][0]["rows"][0]["columns"]["0x1000000e"]["value"] = 8;
    Request conflicting_stats{.method = "POST", .path = "/api/v2/stats/writes",
                              .body = conflicting_stats_body.dump()};
    if (WriteStats(conflicting_stats, host).status != 409 ||
        stats_.at(host.xuid).at("0x00000001").at("0x1000000e").at("value") != 7) {
      return false;
    }
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "3") {
      return false;
    }
    const json invalid_column_stats_body = {
        {"session_id", session_id}, {"sequence", "3"},
        {"views", json::array({
            json{{"view_id", "0x00000002"}, {"rows", json::array({valid_stat_row})}},
            json{{"view_id", "0x00000003"},
                 {"rows", json::array({json{
                     {"xuid", host.xuid},
                     {"columns", {{"0x1000000f", {{"type", "f64"}, {"value", "NaN"}}}}}}})}}})}};
    Request invalid_column_stats{.method = "POST", .path = "/api/v2/stats/writes",
                                 .body = invalid_column_stats_body.dump()};
    if (WriteStats(invalid_column_stats, host).status != 400 ||
        stats_.at(host.xuid).contains("0x00000002") ||
        stats_.at(host.xuid).contains("0x00000003")) return false;
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "4") {
      return false;
    }
    Request aggregate_stats{
        .method = "POST",
        .path = "/api/v2/stats/writes",
        .body = single_stat_write_body(
            "4", "0x00000001",
            {{"0x1000000e", {{"type", "i32"}, {"value", 5}}},
             {"0x2000002c", {{"type", "i64"}, {"value", 900}}},
             {"0x40008002", {{"type", "unicode"}, {"value", "Host A"}}}})};
    if (WriteStats(aggregate_stats, host).status != 204 ||
        stats_.at(host.xuid).at("0x00000001").at("0x1000000e").at("value") != 12 ||
        stats_.at(host.xuid).at("0x00000001").at("0x2000002c").at("value") != 900) {
      return false;
    }
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "5") {
      return false;
    }
    aggregate_stats.body = single_stat_write_body(
        "5", "0x00000001",
        {{"0x2000002c", {{"type", "i64"}, {"value", 1000}}},
         {"0x40008002", {{"type", "unicode"}, {"value", "Host B"}}}});
    if (WriteStats(aggregate_stats, host).status != 204 ||
        stats_.at(host.xuid).at("0x00000001").at("0x2000002c").at("value") != 900 ||
        stats_.at(host.xuid).at("0x00000001").at("0x40008002").at("value") != "Host B") {
      return false;
    }
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "6") {
      return false;
    }
    Request overflowing_stats{
        .method = "POST",
        .path = "/api/v2/stats/writes",
        .body = single_stat_write_body(
            "6", "0x00000001",
            {{"0x1000000e",
              {{"type", "i32"}, {"value", std::numeric_limits<std::int32_t>::max()}}},
             {"0x40008002", {{"type", "unicode"}, {"value", "Bad"}}}})};
    if (WriteStats(overflowing_stats, host).status != 400 ||
        stats_.at(host.xuid).at("0x00000001").at("0x1000000e").at("value") != 12 ||
        stats_.at(host.xuid).at("0x00000001").at("0x40008002").at("value") != "Host B") {
      return false;
    }
    stats_[unknown_xuid]["0x00000001"] = {
        {"0x1000000e", {{"type", "i32"}, {"value", 6}}},
        {"0x1000000f", {{"type", "i32"}, {"value", 1}}},
        {"0x20000037", {{"type", "i64"}, {"value", 40}}}};
    stats_[host.xuid]["0x00000007"]["0x20000037"] =
        {{"type", "i64"}, {"value", 90}};
    stats_[peer.xuid]["0x00000007"]["0x20000037"] =
        {{"type", "i64"}, {"value", 100}};
    stats_[unknown_xuid]["0x00000007"]["0x20000037"] =
        {{"type", "i64"}, {"value", 100}};
    const auto descending_candidates = BuildRankedStatCandidates(stats_, 7);
    if (!descending_candidates || descending_candidates->size() != 3 ||
        descending_candidates->at(0).xuid != peer.xuid ||
        descending_candidates->at(1).xuid != unknown_xuid ||
        descending_candidates->at(2).xuid != host.xuid) {
      return false;
    }
    stats_[host.xuid]["0x0000004f"]["0x20000037"] =
        {{"type", "i64"}, {"value", 90}};
    stats_[peer.xuid]["0x0000004f"]["0x20000037"] =
        {{"type", "i64"}, {"value", 80}};
    stats_[unknown_xuid]["0x0000004f"]["0x20000037"] =
        {{"type", "i64"}, {"value", 80}};
    const auto ascending_candidates = BuildRankedStatCandidates(stats_, 79);
    if (!ascending_candidates || ascending_candidates->size() != 3 ||
        ascending_candidates->at(0).xuid != peer.xuid ||
        ascending_candidates->at(1).xuid != unknown_xuid ||
        ascending_candidates->at(2).xuid != host.xuid) {
      return false;
    }
    const Identity leaderboard_reader{.device_id = "reader",
                                      .xuid = "0x0000000000000006",
                                      .machine_id = "0x0000000000000066",
                                      .player_name = "Reader"};
    Request leaderboard_read{
        .method = "GET", .path = "/api/v2/leaderboards/0x00000001",
        .query = {{"stat_ids", "0x0000fffe,0x00000005,0x00000001,0x00000006"}}};
    const Response leaderboard_response = Leaderboard(leaderboard_read, leaderboard_reader);
    if (relationships_.contains(leaderboard_reader.xuid) ||
        player_names_.contains(unknown_xuid) ||
        leaderboard_response.status != 200 ||
        leaderboard_response.body.value("total", 0) != 2 ||
        leaderboard_response.body["rows"].size() != 2 ||
        leaderboard_response.body["rows"][0].value("xuid", "") != unknown_xuid ||
        leaderboard_response.body["rows"][0].value("rank", 0) != 1 ||
        leaderboard_response.body["rows"][0].value("rating", 0) != 40 ||
        leaderboard_response.body["rows"][0]["columns"].size() != 3 ||
        leaderboard_response.body["rows"][0]["columns"][0].value("stat_id", "") !=
            "0x00000005" ||
        leaderboard_response.body["rows"][0]["columns"][1].value("stat_id", "") !=
            "0x00000001" ||
        leaderboard_response.body["rows"][0]["columns"][1]["value"].value("type", "") !=
            "unset" ||
        leaderboard_response.body["rows"][0]["columns"][2].value("stat_id", "") !=
            "0x00000006" ||
        leaderboard_response.body["rows"][1].value("xuid", "") != host.xuid ||
        leaderboard_response.body["rows"][1].value("rank", 0) != 2 ||
        leaderboard_response.body["rows"][1].value("rating", 0) != 50 ||
        relationships_.contains(leaderboard_reader.xuid) ||
        player_names_.contains(unknown_xuid)) return false;
    Request rating_only_leaderboard{
        .method = "GET", .path = "/api/v2/leaderboards/0x00000001"};
    const Response rating_only_response =
        Leaderboard(rating_only_leaderboard, leaderboard_reader);
    if (rating_only_response.status != 200 ||
        rating_only_response.body.value("total", 0) != 2 ||
        rating_only_response.body["rows"].size() != 2 ||
        !rating_only_response.body["rows"][0]["columns"].empty()) {
      return false;
    }
    Request second_page{
        .method = "GET", .path = "/api/v2/leaderboards/0x00000001",
        .query = {{"offset", "1"}, {"limit", "1"}}};
    const Response second_page_response = Leaderboard(second_page, leaderboard_reader);
    if (second_page_response.status != 200 ||
        second_page_response.body.value("total", 0) != 2 ||
        second_page_response.body["rows"].size() != 1 ||
        second_page_response.body["rows"][0].value("xuid", "") != host.xuid ||
        second_page_response.body["rows"][0].value("rank", 0) != 2) {
      return false;
    }
    Request ranked_direct_read{
        .method = "POST",
        .path = "/api/v2/stats/read",
        .body = json({{"xuids", json::array({host.xuid, unknown_xuid})},
                      {"view_id", "0x00000001"},
                      {"stat_ids", json::array({"0x00000006", "0x00000001",
                                                "0x0000fffe", "0x00000005"})}})
                    .dump()};
    const Response ranked_direct_response = ReadStats(ranked_direct_read);
    if (ranked_direct_response.status != 200 ||
        ranked_direct_response.body["rows"].size() != 2 ||
        ranked_direct_response.body["rows"][0].value("xuid", "") != host.xuid ||
        ranked_direct_response.body["rows"][0].value("rank", 0) != 2 ||
        ranked_direct_response.body["rows"][0].value("rating", 0) != 50 ||
        ranked_direct_response.body["rows"][0]["columns"].size() != 3 ||
        ranked_direct_response.body["rows"][0]["columns"][0].value("stat_id", "") !=
            "0x00000006" ||
        ranked_direct_response.body["rows"][0]["columns"][1].value("stat_id", "") !=
            "0x00000001" ||
        ranked_direct_response.body["rows"][0]["columns"][1]["value"].value("type", "") !=
            "unset" ||
        ranked_direct_response.body["rows"][0]["columns"][2].value("stat_id", "") !=
            "0x00000005" ||
        ranked_direct_response.body["rows"][1].value("xuid", "") != unknown_xuid ||
        ranked_direct_response.body["rows"][1].value("rank", 0) != 1 ||
        ranked_direct_response.body["rows"][1].value("rating", 0) != 40) {
      return false;
    }

    Request result{.method = "POST", .path = "/api/v2/progression/results",
                   .body = json({{"result_id", "result_self_test"}, {"session_id", session_id},
                                 {"expected_revision", 1}, {"expected_host_epoch", 1},
                                 {"mode", "deathmatch"}, {"ranked", true},
                                 {"procedure_index", 0}, {"flags", 1086},
                                 {"lifecycle_state", 2},
                                 {"contexts", {{"0x00000001", 7}}},
                                 {"properties", {{"0x00000002", "AQ=="}}},
                                 {"rows", json::array({{{"xuid", host.xuid}, {"cash_delta", 10000},
                                                        {"score", 100}, {"kills", 5}, {"deaths", 2},
                                                        {"won", true}}})}}).dump()};
    const Response ranked = RankedResult(result, host);
    if (ranked.status != 501 || progression_.contains(host.xuid) ||
        !ProfileGet(host.xuid).body["appearances"].contains("base")) return false;
    const Response replayed_ranked = RankedResult(result, host);
    if (replayed_ranked.status != 501 || progression_.contains(host.xuid) ||
        mode_stats_.contains(host.xuid)) return false;
    Request colliding_result = result;
    json colliding_body = json::parse(colliding_result.body);
    colliding_body["rows"][0]["score"] = 101;
    colliding_result.body = colliding_body.dump();
    if (RankedResult(colliding_result, host).status != 501 ||
        progression_.contains(host.xuid) || mode_stats_.contains(host.xuid)) return false;
    Request malformed_collision{
        .method = "POST", .path = "/api/v2/progression/results",
        .body = json({{"result_id", "result_self_test"}, {"rows", "malformed"}}).dump()};
    if (RankedResult(malformed_collision, host).status != 501 ||
        progression_.contains(host.xuid)) return false;

    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "7") {
      return false;
    }
    Request money_stats{
        .method = "POST",
        .path = "/api/v2/stats/writes",
        .body = single_stat_write_body(
            "7", "0x0000006d",
            {{"0x2000000d", {{"type", "i64"}, {"value", 40000}}},
             {"0x10000026", {{"type", "i32"}, {"value", 1}}},
             {"0x2000003e", {{"type", "i64"}, {"value", 111}}}})};
    if (WriteStats(money_stats, host).status != 204 ||
        progression_.at(host.xuid).cash != 40000 || progression_.at(host.xuid).rank != 2 ||
        stats_.at(host.xuid).at("0x0000006d").at("0x2000003e").at("value") != 111) {
      return false;
    }
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "8") {
      return false;
    }
    money_stats.body = single_stat_write_body(
        "8", "0x0000006d",
        {{"0x2000003e", {{"type", "i64"}, {"value", 222}}}});
    if (WriteStats(money_stats, host).status != 204 ||
        progression_.at(host.xuid).cash != 40000 || progression_.at(host.xuid).rank != 2 ||
        stats_.at(host.xuid).at("0x0000006d").at("0x2000003e").at("value") != 222) {
      return false;
    }
    Request money_read{
        .method = "POST",
        .path = "/api/v2/stats/read",
        .body = json({{"xuids", json::array({host.xuid})},
                      {"view_id", "0x0000006d"},
                      {"stat_ids",
                       json::array({"0x0000fffe", "0x0000ffff"})}})
                    .dump()};
    const Response money_row = ReadStats(money_read);
    if (money_row.status != 200 ||
        money_row.body["rows"][0].value("rank", 0) != 1 ||
        money_row.body["rows"][0].value("rating", 0) != 40000 ||
        money_row.body["rows"][0]["columns"].size() != 1 ||
        money_row.body["rows"][0]["columns"][0].value("stat_id", "") !=
            "0x0000ffff" ||
        money_row.body["rows"][0]["columns"][0]["value"]["value"] != 2) {
      return false;
    }
    stats_[unknown_xuid]["0x0000006d"] = {
        {"0x2000000d", {{"type", "i64"}, {"value", 30000}}},
        {"0x10000026", {{"type", "i32"}, {"value", 100}}}};
    Request cash_leaderboard{
        .method = "GET", .path = "/api/v2/leaderboards/0x0000006d",
        .query = {{"stat_ids", "0x00000001"}}};
    const Response cash_leaderboard_response =
        Leaderboard(cash_leaderboard, leaderboard_reader);
    if (cash_leaderboard_response.status != 200 ||
        cash_leaderboard_response.body.value("total", 0) != 2 ||
        cash_leaderboard_response.body["rows"].size() != 2 ||
        cash_leaderboard_response.body["rows"][0].value("xuid", "") != host.xuid ||
        cash_leaderboard_response.body["rows"][0].value("rank", 0) != 1 ||
        cash_leaderboard_response.body["rows"][0].value("rating", 0) != 40000 ||
        cash_leaderboard_response.body["rows"][0]["columns"][0]["value"]["value"] != 1 ||
        cash_leaderboard_response.body["rows"][1].value("xuid", "") != unknown_xuid ||
        cash_leaderboard_response.body["rows"][1].value("rank", 0) != 2 ||
        cash_leaderboard_response.body["rows"][1].value("rating", 0) != 30000 ||
        cash_leaderboard_response.body["rows"][1]["columns"][0]["value"]["value"] != 100) {
      return false;
    }
    if (AllocateStatSequence(allocate_stats, host).body.value("sequence", "") != "9") {
      return false;
    }
    Request concurrent_stats{
        .method = "POST",
        .path = "/api/v2/stats/writes",
        .body = single_stat_write_body(
            "9", "0x00000001",
            {{"0x1000000e", {{"type", "i32"}, {"value", 1}}}})};
    Response first_concurrent;
    Response second_concurrent;
    std::thread first_writer(
        [&] { first_concurrent = WriteStats(concurrent_stats, host); });
    std::thread second_writer(
        [&] { second_concurrent = WriteStats(concurrent_stats, host); });
    first_writer.join();
    second_writer.join();
    if (first_concurrent.status != 204 || second_concurrent.status != 204 ||
        stats_.at(host.xuid).at("0x00000001").at("0x1000000e").at("value") != 13) {
      return false;
    }

    const Response host_remote_allocation = AllocateStatSequence(allocate_stats, host);
    const std::string host_remote_sequence =
        host_remote_allocation.body.value("sequence", "");
    if (host_remote_allocation.status != 201 || host_remote_sequence.empty()) {
      return false;
    }
    Request host_remote_stats{
        .method = "POST",
        .path = "/api/v2/stats/writes",
        .body = SingleStatWriteRequest(
                    session_id, host_remote_sequence, peer.xuid,
                    "0x00000001",
                    {{"0x1000000e", {{"type", "i32"}, {"value", 2}}}})
                    .dump()};
    if (WriteStats(host_remote_stats, host).status != 204 ||
        stats_.at(peer.xuid).at("0x00000001").at("0x1000000e").at("value") != 2) {
      return false;
    }

    Request allocate_peer_stats{
        .method = "POST", .path = "/api/v2/stats/sequences",
        .body = json({{"session_id", session_id}}).dump()};
    const Response peer_remote_allocation =
        AllocateStatSequence(allocate_peer_stats, peer);
    const std::string peer_remote_sequence =
        peer_remote_allocation.body.value("sequence", "");
    if (peer_remote_allocation.status != 201 || peer_remote_sequence.empty()) {
      return false;
    }
    Request peer_remote_stats{
        .method = "POST",
        .path = "/api/v2/stats/writes",
        .body = SingleStatWriteRequest(
                    session_id, peer_remote_sequence, third.xuid,
                    "0x00000001",
                    {{"0x1000000e", {{"type", "i32"}, {"value", 3}}}})
                    .dump()};
    if (WriteStats(peer_remote_stats, peer).status != 403 ||
        stats_.contains(third.xuid)) {
      return false;
    }

    const json invalid_later_body = {
        {"result_id", "result_invalid_later"}, {"session_id", session_id},
        {"expected_revision", 1}, {"expected_host_epoch", 1}, {"mode", "deathmatch"},
        {"rows", json::array({
            json{{"xuid", host.xuid}, {"cash_delta", 1000}, {"score", 10},
                 {"kills", 1}, {"deaths", 0}, {"won", false}},
            json{{"xuid", unknown_xuid}, {"cash_delta", 1000}, {"score", 10},
                 {"kills", 1}, {"deaths", 0}, {"won", false}}})}};
    Request invalid_later_row{.method = "POST", .path = "/api/v2/progression/results",
                              .body = invalid_later_body.dump()};
    if (RankedResult(invalid_later_row, host).status != 501 ||
        progression_.at(host.xuid).cash != 40000 || progression_.contains(unknown_xuid) ||
        ranked_results_.contains("result_invalid_later") || mode_stats_.contains(host.xuid)) {
      return false;
    }

    mode_stats_[peer.xuid]["overflow_mode"] = {
        {"games", 0}, {"wins", 0}, {"score", std::numeric_limits<std::int64_t>::max()},
        {"kills", std::uint64_t{0}}, {"deaths", std::uint64_t{0}}};
    const json overflowing_later_body = {
        {"result_id", "result_overflow_later"}, {"session_id", session_id},
        {"expected_revision", 1}, {"expected_host_epoch", 1}, {"mode", "overflow_mode"},
        {"rows", json::array({
            json{{"xuid", host.xuid}, {"cash_delta", 50}, {"score", 1},
                 {"kills", 0}, {"deaths", 0}, {"won", false}},
            json{{"xuid", peer.xuid}, {"cash_delta", 50}, {"score", 1},
                 {"kills", 0}, {"deaths", 0}, {"won", false}}})}};
    Request overflowing_later_row{.method = "POST", .path = "/api/v2/progression/results",
                                  .body = overflowing_later_body.dump()};
    if (RankedResult(overflowing_later_row, host).status != 501 ||
        progression_.at(host.xuid).cash != 40000 || progression_.contains(peer.xuid) ||
        (mode_stats_.contains(host.xuid) &&
         mode_stats_.at(host.xuid).contains("overflow_mode")) ||
        mode_stats_.at(peer.xuid).at("overflow_mode").value("score", std::int64_t{0}) !=
            std::numeric_limits<std::int64_t>::max() ||
        ranked_results_.contains("result_overflow_later")) return false;

    sessions_[session_id] = lobby_before_stat_tests;
    ranked_started_.erase(session_id);
    arbitration_snapshots_.erase(session_id);

    Request ready{.method = "POST", .path = "/api/v2/lobbies/" + session_id + "/ready",
                  .body = json({{"expected_session_revision", 1}, {"ready", true}}).dump()};
    Request spectator{.method = "POST", .path = "/api/v2/lobbies/" + session_id + "/spectator",
                      .body = json({{"expected_session_revision", 1}, {"spectating", true}}).dump()};
    Request next_game{.method = "PUT", .path = "/api/v2/lobbies/" + session_id + "/next-game",
                      .body = json({{"expected_session_revision", 1},
                                    {"next_game", {{"mode", "team_deathmatch"},
                                                   {"episode", "base"}, {"ranked", true},
                                                   {"contexts", json::object()},
                                                   {"properties", json::object()}}}}).dump()};
    if (LobbyOperation(ready, peer).body["ready"].value(peer.xuid, false) != true ||
        LobbyOperation(spectator, peer).body["spectators"].value(peer.xuid, false) != true ||
        LobbyOperation(next_game, host).body["next_game"].value("mode", "") != "team_deathmatch") {
      return false;
    }

    Request first_vote{.method = "POST", .path = "/api/v2/lobbies/" + session_id + "/kick-votes",
                       .body = json({{"expected_session_revision", 1},
                                     {"target_xuid", third.xuid}, {"vote", true}}).dump()};
    Request passing_vote = first_vote;
    relay_routes_[third.xuid + ":3074"] = {.xuid = third.xuid, .session_id = session_id,
                                             .port = 3074, .virtual_ipv4 = "192.168.100.3"};
    relay_queues_[third.xuid + ":3074"].push_back({.source_ipv4 = "192.168.100.1",
                                                    .source_port = 3074, .payload = "AA=="});
    voice_routes_["voice_self_test"] = {.owner_xuid = third.xuid, .session_id = session_id};
    if (LobbyOperation(first_vote, peer).body.value("kicked", true)) return false;
    const Response kicked = LobbyOperation(passing_vote, host);
    if (!kicked.body.value("kicked", false) || sessions_[session_id]["members"].size() != 2 ||
        sessions_[session_id].value("revision", 0) != 2 ||
        relay_routes_.contains(third.xuid + ":3074") ||
        relay_queues_.contains(third.xuid + ":3074") ||
        voice_routes_.contains("voice_self_test")) return false;

    Request matched_ticket{.method = "POST", .path = "/api/v2/matchmaking/tickets",
                           .body = json({{"procedure_index", 0}, {"mode", "gta4"},
                                         {"episode", "all"}, {"region", "global"},
                                         {"ranked", true}, {"party_size", 2},
                                         {"contexts", {{"0x00000001", 7}}},
                                         {"properties", {{"0x00000002", "AQ=="}}}}).dump()};
    const Response matched = CreateMatchmakingTicket(matched_ticket, peer);
    if (matched.status != 201 || matched.body.value("state", "") != "matched" ||
        matched.body.value("matched_session_id", "") != session_id) return false;
    const std::string matched_id = matched.body.value("id", "");
    MatchmakingTicket slot_boundary_ticket = tickets_.at(matched_id);
    slot_boundary_ticket.party_size = 0;
    json invalid_public_slots = sessions_.at(session_id);
    invalid_public_slots["public_slots"] = 65;
    invalid_public_slots["private_slots"] = 0;
    json invalid_private_slots = sessions_.at(session_id);
    invalid_private_slots["public_slots"] = 64;
    invalid_private_slots["private_slots"] = 65;
    if (TicketCanMatchSession(slot_boundary_ticket, invalid_public_slots) ||
        TicketCanMatchSession(slot_boundary_ticket, invalid_private_slots)) {
      return false;
    }
    Request wrong_procedure_ticket = matched_ticket;
    json wrong_procedure_ticket_body = json::parse(wrong_procedure_ticket.body);
    wrong_procedure_ticket_body["procedure_index"] = 1;
    wrong_procedure_ticket.body = wrong_procedure_ticket_body.dump();
    const Response procedure_searching = CreateMatchmakingTicket(wrong_procedure_ticket, peer);
    if (procedure_searching.status != 201 ||
        procedure_searching.body.value("state", "") != "searching") {
      return false;
    }
    Request get_matched{.method = "GET", .path = "/api/v2/matchmaking/tickets/" + matched_id};
    Request delete_matched{.method = "DELETE", .path = get_matched.path};
    if (MatchmakingTicketOperation(get_matched, peer).status != 200 ||
        MatchmakingTicketOperation(get_matched, host).status != 403 ||
        MatchmakingTicketOperation(delete_matched, host).status != 403) return false;

    Request unmatched_ticket = matched_ticket;
    unmatched_ticket.body = json({{"procedure_index", 0}, {"mode", "gta4"},
                                   {"episode", "all"}, {"region", "global"},
                                   {"ranked", false}, {"party_size", 2},
                                   {"contexts", json::object()},
                                   {"properties", json::object()}}).dump();
    const Response searching = CreateMatchmakingTicket(unmatched_ticket, peer);
    if (searching.status != 201 || searching.body.value("state", "") != "searching") return false;
    Request malformed_ticket = unmatched_ticket;
    malformed_ticket.body = json({{"procedure_index", "invalid"}, {"party_size", 1},
                                  {"mode", "gta4"}, {"episode", "all"},
                                  {"ranked", false}}).dump();
    if (CreateMatchmakingTicket(malformed_ticket, peer).status != 400) return false;
    Request remove_ticket{.method = "DELETE",
                          .path = "/api/v2/matchmaking/tickets/" + searching.body.value("id", "")};
    Request remove_procedure_ticket{
        .method = "DELETE",
        .path = "/api/v2/matchmaking/tickets/" +
                procedure_searching.body.value("id", "")};
    if (MatchmakingTicketOperation(remove_ticket, peer).status != 200 ||
        MatchmakingTicketOperation(remove_procedure_ticket, peer).status != 200 ||
        MatchmakingTicketOperation(delete_matched, peer).status != 200) return false;

    sessions_.at(session_id)["exchange_key"] = "0x000102030405060708090a0b0c0d0e0f";
    sessions_.at(session_id)["host_ipv4"] = "192.168.100.1";
    const Response member_view = GetSession(session_id, host);
    const Response outsider_view = GetSession(session_id, pending);
    if (member_view.status != 200 ||
        member_view.body.value("exchange_key", "") !=
            "0x000102030405060708090a0b0c0d0e0f" ||
        outsider_view.status != 200 || !outsider_view.body.at("members").empty() ||
        outsider_view.body.value("exchange_key", "") !=
            "0x000102030405060708090a0b0c0d0e0f" ||
        outsider_view.body.value("nonce", "") !=
            sessions_.at(session_id).value("nonce", "") ||
        outsider_view.body.value("host_machine_id", "") !=
            sessions_.at(session_id).value("host_machine_id", "") ||
        outsider_view.body.value("host_ipv4", "") != "192.168.100.1" ||
        outsider_view.body.value("host_port", 0) !=
            sessions_.at(session_id).value("host_port", 0) ||
        outsider_view.body.value("host_ethernet_address", "") !=
            sessions_.at(session_id).value("host_ethernet_address", "") ||
        outsider_view.body.value("host_peer_id", "") !=
            sessions_.at(session_id).value("host_peer_id", "") ||
        outsider_view.body.value("member_count", std::size_t{}) !=
            sessions_.at(session_id).at("members").size()) {
      return false;
    }
    Request heartbeat_missing{.method = "POST", .path = "/api/v2/sessions/" + session_id + "/heartbeat",
                              .body = json::object().dump()};
    Request heartbeat_peer{.method = "POST", .path = heartbeat_missing.path,
                           .body = json({{"expected_revision", 2}}).dump()};
    Request immutable_patch{.method = "PATCH", .path = "/api/v2/sessions/" + session_id,
                            .body = json({{"expected_revision", 2},
                                          {"session_id", "0x000000000000ffff"}}).dump()};
    json injected_members = sessions_.at(session_id).at("members");
    injected_members.push_back(json{{"xuid", host.xuid}, {"private", false}});
    Request membership_injection{.method = "PATCH", .path = "/api/v2/sessions/" + session_id,
                                 .body = json({{"expected_revision", 2},
                                               {"members", injected_members}}).dump()};
    if (SessionOperation(heartbeat_missing, host).status != 400 ||
        SessionOperation(heartbeat_peer, peer).status != 403 ||
        SessionOperation(immutable_patch, host).status != 400 ||
        SessionOperation(membership_injection, host).status != 400) return false;

    const std::string private_id = "0x0000000000000200";
    json private_session = sessions_.at(session_id);
    private_session["session_id"] = private_id;
    private_session["visibility"] = "private";
    private_session["flags"] = 1798;
    private_session["ranked"] = false;
    private_session["join_in_progress"] = false;
    private_session["public_slots"] = 1;
    private_session["private_slots"] = 1;
    private_session["revision"] = 1;
    private_session["host_epoch"] = 1;
    private_session["members"] = json::array({json{{"xuid", host.xuid}, {"private", false}}});
    NormalizeMembers(private_session);
    sessions_[private_id] = private_session;
    if (GetSession(private_id, peer).status != 404) return false;

    json too_many_recipients = json::array();
    for (std::size_t index = 0; index < kOversizeInviteRecipients; ++index) {
      too_many_recipients.push_back(peer.xuid);
    }
    Request too_many_invites{.method = "POST", .path = "/api/v2/invites",
                             .body = json({{"session_id", private_id},
                                           {"recipient_xuids", too_many_recipients},
                                           {"custom_data", ""}}).dump()};
    Request oversized_custom{.method = "POST", .path = "/api/v2/invites",
                             .body = json({{"session_id", private_id},
                                           {"recipient_xuids", json::array({peer.xuid})},
                                           {"custom_data", std::string(kOversizeInviteCustomEncodedBytes, 'A')}}).dump()};
    Request outsider_invite{.method = "POST", .path = "/api/v2/invites",
                            .body = json({{"session_id", private_id},
                                          {"recipient_xuids", json::array({peer.xuid})},
                                          {"custom_data", ""}}).dump()};
    if (CreateInvites(too_many_invites, host).status != 400 ||
        CreateInvites(oversized_custom, host).status != 400 ||
        CreateInvites(outsider_invite, third).status != 403) return false;
    Request valid_invite{.method = "POST", .path = "/api/v2/invites",
                         .body = json({{"session_id", private_id},
                                       {"recipient_xuids", json::array({peer.xuid})},
                                       {"custom_data", "AA=="}, {"expires_in_seconds", 300}}).dump()};
    if (CreateInvites(valid_invite, host).status != 201 || GetSession(private_id, peer).status != 200) {
      return false;
    }
    std::string invite_id;
    for (const auto& [id, invite] : invites_) {
      if (invite.value("session_id", "") == private_id &&
          invite.value("recipient_xuid", "") == peer.xuid) invite_id = id;
    }
    Request accept_private{.method = "POST", .path = "/api/v2/invites/" + invite_id + "/accept",
                           .body = json({{"expected_revision", 1}}).dump()};
    Request unaccepted_private_join{
        .method = "POST",
        .path = "/api/v2/sessions/" + private_id + "/join",
        .body = json({{"expected_revision", 1}, {"private", true},
                      {"invite_id", invite_id},
                      {"member", {{"xuid", peer.xuid}, {"private", true}}}}).dump()};
    if (invite_id.empty() || SessionOperation(unaccepted_private_join, peer).status != 403 ||
        !invites_.contains(invite_id) || sessions_.at(private_id).at("members").size() != 1) {
      return false;
    }
    const Response accepted_private = AcceptInvite(accept_private, peer);
    if (accepted_private.status != 200 ||
        accepted_private.body["invite"].value("state", "") != "pending" ||
        !accepted_private.body["invite"].value("acknowledged", false)) return false;
    Request private_join{.method = "POST", .path = "/api/v2/sessions/" + private_id + "/join",
                         .body = json({{"expected_revision", 1}, {"private", true},
                                       {"invite_id", invite_id},
                                       {"member", {{"xuid", peer.xuid}, {"private", true}}}}).dump()};
    if (SessionOperation(private_join, peer).status != 200 || invites_.contains(invite_id) ||
        sessions_.at(private_id).at("members").size() != 2) return false;

    Request invalid_voice_owner{.method = "POST", .path = "/api/v2/voice/routes",
                               .body = json({{"session_id", session_id}, {"channel", "all"},
                                             {"target_xuids", json::array()},
                                             {"mute_xuids", json::array()}}).dump()};
    if (VoiceRoutes(invalid_voice_owner, third).status != 403) return false;
    Request voice_route_request = invalid_voice_owner;
    voice_route_request.body = json({{"session_id", session_id}, {"channel", "private"},
                                      {"target_xuids", json::array({peer.xuid})},
                                      {"mute_xuids", json::array({peer.xuid})}}).dump();
    Request invalid_all_voice_targets = invalid_voice_owner;
    invalid_all_voice_targets.body =
        json({{"session_id", session_id},
              {"channel", "all"},
              {"target_xuids", json::array({peer.xuid})},
              {"mute_xuids", json::array()}})
            .dump();
    Request invalid_self_voice_mute = invalid_voice_owner;
    invalid_self_voice_mute.body =
        json({{"session_id", session_id},
              {"channel", "all"},
              {"target_xuids", json::array()},
              {"mute_xuids", json::array({host.xuid})}})
            .dump();
    const Response voice_route = VoiceRoutes(voice_route_request, host);
    const std::string voice_token = voice_route.body.value("route_token", "");
    Request delete_voice{.method = "DELETE", .path = "/api/v2/voice/routes",
                         .body = json({{"route_token", voice_token}}).dump()};
    if (voice_route.status != 201 ||
        VoiceRoutes(invalid_all_voice_targets, host).status != 400 ||
        VoiceRoutes(invalid_self_voice_mute, host).status != 400 ||
        VoiceRoutes(delete_voice, peer).status != 403) {
      return false;
    }
    Request invalid_voice_payload{.method = "POST", .path = "/api/v2/voice/packets",
                                 .body = json({{"packets", json::array({json{
                                     {"route_token", voice_token}, {"sequence", 1},
                                     {"payload", std::string(kMaximumVoiceEncodedPayloadBytes, 'A')}}})}}).dump()};
    if (VoicePackets(invalid_voice_payload, host).status != 400) return false;
    Request valid_voice_payload{.method = "POST", .path = "/api/v2/voice/packets",
                               .body = json({{"packets", json::array({json{
                                   {"route_token", voice_token}, {"sequence", 2},
                                   {"payload", "AA=="}}})}}).dump()};
    json oversized_voice_batch = json::array();
    for (std::size_t index = 0; index < kOversizeVoicePacketsPerBatch; ++index) {
      oversized_voice_batch.push_back(json{{"route_token", voice_token}, {"sequence", 2},
                                            {"payload", "AA=="}});
    }
    Request oversized_voice_request{.method = "POST", .path = "/api/v2/voice/packets",
                                    .body = json({{"packets", oversized_voice_batch}}).dump()};
    Request receive_voice{.method = "GET", .path = "/api/v2/voice/packets",
                          .query = {{"route_token", voice_token}}};
    if (VoicePackets(oversized_voice_request, host).status != 400 ||
        VoicePackets(valid_voice_payload, host).status != 204 ||
        VoicePackets(receive_voice, peer).status != 404) return false;
    Request peer_route_request = invalid_voice_owner;
    peer_route_request.body = json({{"session_id", session_id}, {"channel", "all"},
                                     {"target_xuids", json::array()},
                                     {"mute_xuids", json::array()}}).dump();
    const Response peer_route = VoiceRoutes(peer_route_request, peer);
    const std::string peer_voice_token = peer_route.body.value("route_token", "");
    receive_voice.query["route_token"] = peer_voice_token;
    Request delivered_voice_payload = valid_voice_payload;
    delivered_voice_payload.body = json({{"packets", json::array({json{
        {"route_token", voice_token}, {"sequence", 3}, {"payload", "AA=="}}})}}).dump();
    if (VoicePackets(delivered_voice_payload, host).status != 204 ||
        VoicePackets(delivered_voice_payload, host).status != 204) return false;
    const Response received = VoicePackets(receive_voice, peer);
    const std::string received_after = received.body.value("next_after", "");
    Request replay_cursor = receive_voice;
    replay_cursor.query["after"] = received_after;
    const Response no_replay = VoicePackets(replay_cursor, peer);
    Request future_voice_cursor = receive_voice;
    future_voice_cursor.query["after"] = "cursor_18446744073709551615";
    if (peer_route.status != 201 || received.status != 200 ||
        received.body.at("packets").size() != 1 ||
        received_after.empty() || no_replay.status != 200 ||
        !no_replay.body.at("packets").empty() ||
        no_replay.body.value("next_after", "") != received_after ||
        VoicePackets(future_voice_cursor, peer).status != 409 ||
        voice_queue_encoded_bytes_.at(peer_voice_token) != 4) {
      return false;
    }
    const std::string maximum_payload(kMaximumVoiceEncodedPayloadPrefixBytes, 'A');
    std::uint32_t maximum_sequence = 4;
    Request maximum_batch_request{.method = "POST", .path = "/api/v2/voice/packets"};
    for (std::size_t batch = 0; batch < kVoiceQueueFillBatches; ++batch) {
      json maximum_batch = json::array();
      for (std::size_t index = 0; index < kMaximumVoicePacketsPerBatch; ++index) {
        maximum_batch.push_back(json{{"route_token", voice_token},
                                     {"sequence", maximum_sequence++},
                                     {"payload", maximum_payload + "=="}});
      }
      maximum_batch_request.body = json({{"packets", maximum_batch}}).dump();
      if (VoicePackets(maximum_batch_request, host).status != 204) return false;
    }
    if (voice_queues_.at(peer_voice_token).size() != kMaximumVoiceQueuedPackets ||
        voice_queue_encoded_bytes_.at(peer_voice_token) != kMaximumVoiceQueueEncodedBytes ||
        VoicePackets(maximum_batch_request, host).status != 204 ||
        voice_queues_.at(peer_voice_token).size() != kMaximumVoiceQueuedPackets ||
        voice_queue_encoded_bytes_.at(peer_voice_token) != kMaximumVoiceQueueEncodedBytes) {
      return false;
    }
    voice_queues_.erase(peer_voice_token);
    voice_queue_encoded_bytes_.erase(peer_voice_token);
    relationships_[peer.xuid][host.xuid] = "blocked";
    relationships_[host.xuid][peer.xuid] = "blocked_by";
    Request blocked_voice_payload = valid_voice_payload;
    blocked_voice_payload.body = json({{"packets", json::array({json{
        {"route_token", voice_token},
        {"sequence", std::numeric_limits<std::uint32_t>::max()},
        {"payload", "AA=="}}})}}).dump();
    const Response blocked_voice_sent = VoicePackets(blocked_voice_payload, host);
    const Response blocked_voice_received = VoicePackets(receive_voice, peer);
    relationships_[peer.xuid][host.xuid] = "accepted";
    relationships_[host.xuid][peer.xuid] = "accepted";
    if (blocked_voice_sent.status != 204 || blocked_voice_received.status != 200 ||
        !blocked_voice_received.body.at("packets").empty()) {
      return false;
    }
    Request unauthorized_voice_replacement = voice_route_request;
    json unauthorized_voice_replacement_body =
        json::parse(unauthorized_voice_replacement.body);
    unauthorized_voice_replacement_body["replace_route_token"] =
        peer_voice_token;
    unauthorized_voice_replacement.body =
        unauthorized_voice_replacement_body.dump();
    Request recovery_voice_route_request = voice_route_request;
    json recovery_voice_route_body = json::parse(recovery_voice_route_request.body);
    recovery_voice_route_body["replace_route_token"] = voice_token;
    recovery_voice_route_request.body = recovery_voice_route_body.dump();
    const Response recovered_voice_route =
        VoiceRoutes(recovery_voice_route_request, host);
    const std::string recovered_voice_token =
        recovered_voice_route.body.value("route_token", "");
    delete_voice.body = json({{"route_token", recovered_voice_token}}).dump();
    if (VoiceRoutes(unauthorized_voice_replacement, host).status != 403 ||
        recovered_voice_route.status != 201 || recovered_voice_token.empty() ||
        recovered_voice_token == voice_token ||
        VoicePackets(valid_voice_payload, host).status != 403 ||
        VoiceRoutes(delete_voice, host).status != 204) {
      return false;
    }

    Request all_chat{
        .method = "POST",
        .path = "/api/v2/chat/messages",
        .headers = {{"idempotency-key", "chat-all-response-loss"}},
        .body = json({{"session_id", session_id},
                      {"channel", "all"},
                      {"target_xuids", json::array()},
                      {"sequence", 1},
                      {"text", "Hello"}})
                    .dump()};
    const Response all_chat_created = ChatMessage(all_chat, host);
    const Response all_chat_replayed = ChatMessage(all_chat, host);
    Request receive_events{.method = "GET",
                           .path = "/api/v2/events",
                           .query = {{"after", "0"}, {"wait_ms", "0"}}};
    const Response all_events = RealtimeEvents(receive_events, peer);
    if (all_chat_created.status != 201 ||
        all_chat_replayed.status != 201 ||
        all_chat_created.body != all_chat_replayed.body ||
        all_events.status != 200 || all_events.body.at("events").size() != 1 ||
        all_events.body.at("events")[0].value("type", "") != "chat.message" ||
        all_events.body.at("events")[0].value("aggregate_id", "") != session_id ||
        all_events.body.at("events")[0].at("payload").value("source_xuid", "") !=
            host.xuid) {
      return false;
    }
    const std::int64_t all_event_id =
        all_events.body.at("events")[0].at("id").get<std::int64_t>();
    receive_events.query["after"] = std::to_string(all_event_id);
    if (!RealtimeEvents(receive_events, peer).body.at("events").empty()) {
      return false;
    }
    Request conflicting_chat = all_chat;
    conflicting_chat.body = json({{"session_id", session_id},
                                  {"channel", "all"},
                                  {"target_xuids", json::array()},
                                  {"sequence", 1},
                                  {"text", "Different"}})
                                .dump();
    if (ChatMessage(conflicting_chat, host).status != 409) return false;

    Request team_chat{
        .method = "POST",
        .path = "/api/v2/chat/messages",
        .headers = {{"idempotency-key", "chat-team"}},
        .body = json({{"session_id", session_id},
                      {"channel", "team"},
                      {"target_xuids", json::array({peer.xuid})},
                      {"sequence", 2},
                      {"text", "Team"}})
                    .dump()};
    const Response team_chat_created = ChatMessage(team_chat, host);
    const Response team_events = RealtimeEvents(receive_events, peer);
    if (team_chat_created.status != 201 || team_events.status != 200 ||
        team_events.body.at("events").size() != 1 ||
        team_events.body.at("events")[0].at("payload").value("channel", "") !=
            "team") {
      return false;
    }
    const std::int64_t team_event_id =
        team_events.body.at("events")[0].at("id").get<std::int64_t>();
    receive_events.query["after"] = std::to_string(team_event_id);

    Request invalid_all_targets = all_chat;
    invalid_all_targets.headers["idempotency-key"] = "chat-invalid-all-targets";
    json invalid_all_body = json::parse(invalid_all_targets.body);
    invalid_all_body["target_xuids"] = json::array({peer.xuid});
    invalid_all_targets.body = invalid_all_body.dump();
    Request empty_team_targets = team_chat;
    empty_team_targets.headers["idempotency-key"] = "chat-empty-team";
    json empty_team_body = json::parse(empty_team_targets.body);
    empty_team_body["target_xuids"] = json::array();
    empty_team_targets.body = empty_team_body.dump();
    Request missing_chat_key = all_chat;
    missing_chat_key.headers.clear();
    Request oversized_chat = all_chat;
    oversized_chat.headers["idempotency-key"] = "chat-oversized";
    json oversized_chat_body = json::parse(oversized_chat.body);
    oversized_chat_body["text"] = std::string(kOversizeTextChatBytes, 'A');
    oversized_chat.body = oversized_chat_body.dump();
    if (ChatMessage(invalid_all_targets, host).status != 400 ||
        ChatMessage(empty_team_targets, host).status != 400 ||
        ChatMessage(missing_chat_key, host).status != 400 ||
        ChatMessage(oversized_chat, host).status != 400 ||
        ChatMessage(all_chat, third).status != 403) {
      return false;
    }

    relationships_[peer.xuid][host.xuid] = "blocked";
    relationships_[host.xuid][peer.xuid] = "blocked_by";
    Request blocked_chat = all_chat;
    blocked_chat.headers["idempotency-key"] = "chat-blocked";
    json blocked_chat_body = json::parse(blocked_chat.body);
    blocked_chat_body["sequence"] = 3;
    blocked_chat.body = blocked_chat_body.dump();
    const Response blocked_chat_created = ChatMessage(blocked_chat, host);
    const Response blocked_events = RealtimeEvents(receive_events, peer);
    relationships_[peer.xuid][host.xuid] = "accepted";
    relationships_[host.xuid][peer.xuid] = "accepted";
    Request future_events = receive_events;
    future_events.query["after"] =
        std::to_string(std::numeric_limits<std::int64_t>::max());
    if (blocked_chat_created.status != 201 || blocked_events.status != 200 ||
        !blocked_events.body.at("events").empty() ||
        RealtimeEvents(future_events, peer).status != 409) {
      return false;
    }

    const std::string migration_id = "0x0000000000000300";
    json migration_source = sessions_.at(session_id);
    migration_source["session_id"] = migration_id;
    migration_source["mode"] = "deathmatch";
    // Join-in-progress behavior is tested in a Player Match, not by skipping
    // ranked registration or reopening an already certified ranked game.
    migration_source["flags"] = 1070;
    migration_source["ranked"] = false;
    migration_source["revision"] = 1;
    migration_source["host_epoch"] = 1;
    migration_source["contexts"] = {{"migration_policy", 1}};
    migration_source["members"] = json::array(
        {json{{"xuid", host.xuid}, {"peer_id", ""}, {"private", false}},
         json{{"xuid", peer.xuid}, {"peer_id", ""}, {"private", false}},
         json{{"xuid", pending.xuid}, {"peer_id", ""}, {"private", false}}});
    NormalizeMembers(migration_source);
    sessions_[migration_id] = migration_source;
    Request migrating_chat{
        .method = "POST",
        .path = "/api/v2/chat/messages",
        .headers = {{"idempotency-key", "chat-migration"}},
        .body = json({{"session_id", migration_id},
                      {"channel", "team"},
                      {"target_xuids", json::array({pending.xuid})},
                      {"sequence", 4},
                      {"text", "Migrating"}})
                    .dump()};
    if (ChatMessage(migrating_chat, peer).status != 201 ||
        realtime_events_[pending.xuid].empty()) {
      return false;
    }
    json replacement = migration_source;
    replacement["session_id"] = "0x0000000000000301";
    replacement["host_xuid"] = pending.xuid;
    Request arbitrary_migration{.method = "POST", .path = "/api/v2/sessions/" + migration_id + "/migration",
                                .body = json({{"expected_revision", 1}, {"expected_host_epoch", 1},
                                              {"replacement", replacement}}).dump()};
    if (SessionOperation(arbitrary_migration, host).status != 403) return false;
    replacement["host_xuid"] = peer.xuid;
    Request eligible_migration = arbitrary_migration;
    eligible_migration.body = json({{"expected_revision", 1}, {"expected_host_epoch", 1},
                                     {"replacement", replacement}}).dump();
    const Response migrated = SessionOperation(eligible_migration, peer);
    const std::string replacement_id = "0x0000000000000301";
    const Response follower_lookup = GetSession(migration_id, pending);
    if (migrated.status != 200 || !migrated.body.contains("members")) return false;
    const auto migrated_host = std::ranges::find(
        migrated.body.at("members"), peer.xuid,
        [](const json& member) { return member.value("xuid", ""); });
    if (sessions_.contains(migration_id) ||
        !sessions_.contains(replacement_id) || migrated.body.value("host_epoch", 0) != 2 ||
        realtime_events_[pending.xuid].empty() ||
        realtime_events_[pending.xuid].back().session_id != replacement_id ||
        realtime_events_[pending.xuid].back().wire.value("aggregate_id", "") !=
            replacement_id ||
        realtime_events_[pending.xuid].back().wire.at("payload").value("session_id", "") !=
            replacement_id ||
        migrated.body.value("host_peer_id", "") != peer.xuid ||
        migrated_host == migrated.body.at("members").end() ||
        !migrated_host->contains("route") ||
        migrated_host->value("peer_id", "") != peer.xuid ||
        migrated_host->at("route").value("connection_id", "") != peer.xuid ||
        follower_lookup.status != 200 ||
        follower_lookup.body.value("session_id", "") != replacement_id) {
      return false;
    }

    Request unauthorized_removal{
        .method = "POST", .path = "/api/v2/sessions/" + replacement_id + "/leave",
        .body = json({{"expected_revision", 1}, {"xuid", peer.xuid}}).dump()};
    Request host_removal = unauthorized_removal;
    host_removal.body =
        json({{"expected_revision", 1}, {"xuid", pending.xuid}}).dump();
    Request stale_removal = host_removal;
    stale_removal.body =
        json({{"expected_revision", 0}, {"xuid", pending.xuid}}).dump();
    if (SessionOperation(unauthorized_removal, host).status != 403 ||
        SessionOperation(stale_removal, peer).status != 412 ||
        SessionOperation(host_removal, peer).status != 200 ||
        SessionContainsXuid(sessions_.at(replacement_id), pending.xuid)) {
      return false;
    }
    Request missing_removal = host_removal;
    missing_removal.body =
        json({{"expected_revision", 2}, {"xuid", pending.xuid}}).dump();
    if (SessionOperation(missing_removal, peer).status != 403) return false;

    Request enter_game{
        .method = "PATCH", .path = "/api/v2/sessions/" + replacement_id,
        .body = json({{"expected_revision", 2}, {"lifecycle_state", 2}}).dump()};
    const Response entered_game = SessionOperation(enter_game, peer);
    if (entered_game.status != 200 || entered_game.body.value("state", "") != "in_game" ||
        entered_game.body.value("join_in_progress", true)) {
      return false;
    }
    const json search_body = {
        {"maximum_results", 1}, {"title_id", migration_source.at("title_id")},
        {"media_id", migration_source.at("media_id")},
        {"title_version", migration_source.at("title_version")},
        {"protocol_version", migration_source.at("protocol_version")},
        {"procedure_index", 1}, {"contexts", migration_source.at("contexts")},
        {"properties", migration_source.at("properties")}};
    Request search{.method = "POST", .path = "/api/v2/sessions/search",
                   .body = search_body.dump()};
    const Response closed_search = SearchSessions(search, third);
    Request closed_join{
        .method = "POST", .path = "/api/v2/sessions/" + replacement_id + "/join",
        .body = json({{"expected_revision", 3},
                      {"member", {{"xuid", third.xuid}, {"private", false}}}})
                    .dump()};
    if (closed_search.status != 200 || !closed_search.body.at("sessions").empty() ||
        SessionOperation(closed_join, third).status != 409) {
      return false;
    }

    Request allow_in_progress{
        .method = "PATCH", .path = "/api/v2/sessions/" + replacement_id,
        .body = json({{"expected_revision", 3}, {"flags", 46},
                      {"join_in_progress", false}}).dump()};
    if (SessionOperation(allow_in_progress, peer).status != 200) return false;
    json alternate_procedure_body = search_body;
    alternate_procedure_body["procedure_index"] = 2;
    Request alternate_procedure = search;
    alternate_procedure.body = alternate_procedure_body.dump();
    json wrong_mode_body = search_body;
    wrong_mode_body["mode"] = "other_mode";
    Request wrong_mode = search;
    wrong_mode.body = wrong_mode_body.dump();
    json wrong_ranked_body = search_body;
    wrong_ranked_body["ranked"] = true;
    Request wrong_ranked = search;
    wrong_ranked.body = wrong_ranked_body.dump();
    const Response matching_search = SearchSessions(search, third);
    const Response alternate_procedure_search = SearchSessions(alternate_procedure, third);
    const Response nonmatching_mode = SearchSessions(wrong_mode, third);
    const Response nonmatching_ranked = SearchSessions(wrong_ranked, third);
    if (matching_search.body.at("sessions").size() != 1 ||
        matching_search.body.at("sessions").front().value("session_id", "") !=
            replacement_id ||
        matching_search.body.at("sessions").front().value("mode", "") != "deathmatch" ||
        matching_search.body.at("sessions").front().value("ranked", true) ||
        matching_search.body.value("procedure_index", 0u) != 1 ||
        alternate_procedure_search.body.at("sessions").size() != 1 ||
        alternate_procedure_search.body.value("procedure_index", 0u) != 2 ||
        !nonmatching_mode.body.at("sessions").empty() ||
        !nonmatching_ranked.body.at("sessions").empty()) {
      return false;
    }
    closed_join.body = json({{"expected_revision", 4},
                             {"member", {{"xuid", third.xuid}, {"private", false}}}})
                           .dump();
    const Response joined_in_progress = SessionOperation(closed_join, third);
    if (joined_in_progress.status != 200 ||
        !SessionContainsXuid(joined_in_progress.body, third.xuid)) {
      return false;
    }

    Request enter_reporting{
        .method = "PATCH", .path = "/api/v2/sessions/" + replacement_id,
        .body = json({{"expected_revision", 5}, {"lifecycle_state", 3}}).dump()};
    const Response reporting = SessionOperation(enter_reporting, peer);
    if (reporting.status != 200 || reporting.body.value("state", "") != "closed") {
      return false;
    }

    Request empty_delete{.method = "DELETE",
                         .path = "/api/v2/sessions/" + replacement_id,
                         .headers = {{"if-match", "6"}}};
    if (SessionOperation(empty_delete, host).status != 403 ||
        SessionOperation(empty_delete, peer).status != 204 ||
        sessions_.contains(replacement_id)) {
      return false;
    }
    return true;
  }

  bool VerifyDurableAuthSelfTest() {
    if (!persistent_state_) return false;
    const Identity identity{.device_id = "durable-device",
                            .xuid = "0x00000000000000d1",
                            .machine_id = "0x00000000000000e1",
                            .player_name = "Durable Player"};
    if (!GrantEntitlement(identity.xuid, "TLAD") ||
        !GrantEntitlement(identity.xuid, "TLAD") ||
        GrantEntitlement(identity.xuid, "UNKNOWN")) {
      std::cerr << "libserver self-test: entitlement grant contract failed\n";
      return false;
    }
    static constexpr std::string_view kPublicKey =
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    static constexpr std::string_view kConflictKey =
        "AQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQE";
    const Response enrolled = IssueTokens(identity, 201, std::string(kPublicKey), {});
    if (enrolled.status != 201 || !enrolled.body.contains("access_token") ||
        !enrolled.body.contains("refresh_token")) return false;
    const std::string access = enrolled.body.at("access_token").get<std::string>();
    const std::string refresh = enrolled.body.at("refresh_token").get<std::string>();
    Request authenticated;
    authenticated.headers["authorization"] = "Bearer " + access;
    if (!Authenticate(authenticated).has_value()) return false;
    Request entitlement_read = authenticated;
    entitlement_read.method = "GET";
    entitlement_read.path = "/api/v2/entitlements";
    const Response entitlement_snapshot = Dispatch(entitlement_read);
    Request entitlement_write = entitlement_read;
    entitlement_write.method = "POST";
    if (entitlement_snapshot.status != 200 ||
        entitlement_snapshot.body != json{{"packages", json::array({"TLAD"})}} ||
        Dispatch(entitlement_write).status != 404) {
      std::cerr << "libserver self-test: entitlement endpoint contract failed\n";
      std::cerr << "  status=" << entitlement_snapshot.status
                << " body=" << entitlement_snapshot.body.dump()
                << " write_status=" << Dispatch(entitlement_write).status << '\n';
      return false;
    }
    Request achievement_merge = authenticated;
    achievement_merge.method = "POST";
    achievement_merge.path = "/api/v2/achievements";
    achievement_merge.body =
        json({{"achievement_ids", json::array({kFirstGta4AchievementId,
                                                 kLastGta4AchievementId})}})
            .dump();
    const Response first_achievement_merge = Dispatch(achievement_merge);
    const Response repeated_achievement_merge = Dispatch(achievement_merge);
    Request achievement_read = authenticated;
    achievement_read.method = "GET";
    achievement_read.path = "/api/v2/achievements";
    const Response achievement_snapshot = Dispatch(achievement_read);
    Request invalid_achievement_merge = achievement_merge;
    invalid_achievement_merge.body =
        json({{"achievement_ids", json::array({kFirstGta4AchievementId,
                                                 kFirstGta4AchievementId})}})
            .dump();
    if (first_achievement_merge.status != 200 ||
        repeated_achievement_merge.body != first_achievement_merge.body ||
        achievement_snapshot.body != first_achievement_merge.body ||
        first_achievement_merge.body.at("achievement_ids").size() != 2 ||
        Dispatch(invalid_achievement_merge).status != 400) {
      return false;
    }
    const std::string prog_ach_blob(kGta4ProgAchBase64UrlCharacters, 'A');
    Request prog_ach_get = authenticated;
    prog_ach_get.method = "GET";
    prog_ach_get.path = "/api/v3/storage/0x545407F2/3/Prog_ACH";
    Request prog_ach_put{
        .method = "PUT",
        .path = "/api/v3/storage/0x545407F2/3/Prog_ACH",
        .headers = authenticated.headers,
        .body = json({{"blob", prog_ach_blob}}).dump(),
    };
    Request short_prog_ach = prog_ach_put;
    short_prog_ach.body =
        json({{"blob", std::string(kGta4ProgAchShortBase64UrlCharacters, 'A')}}).dump();
    Request long_prog_ach = prog_ach_put;
    long_prog_ach.body =
        json({{"blob", std::string(kGta4ProgAchLongBase64UrlCharacters, 'A')}}).dump();
    Request queried_prog_ach = prog_ach_get;
    queried_prog_ach.query = {{"xuid", identity.xuid}};
    const Identity other_storage_identity{
        .device_id = "other-storage-device",
        .xuid = "0x00000000000000d4",
        .machine_id = "0x00000000000000e4",
        .player_name = "Other Storage Player"};
    if (Dispatch(prog_ach_get).status != 404 ||
        Dispatch(short_prog_ach).status != 400 ||
        Dispatch(long_prog_ach).status != 400 ||
        Dispatch(queried_prog_ach).status != 400 ||
        Dispatch(prog_ach_put).status != 200 ||
        Dispatch(prog_ach_get).body.value("blob", "") != prog_ach_blob ||
        ProgAchStorage(prog_ach_get, other_storage_identity).status != 404) {
      std::cerr << "libserver self-test: Prog_ACH storage contract failed\n";
      return false;
    }
    {
      std::lock_guard lock(mutex_);
      access_tokens_.at(access).expires = Clock::time_point::min();
    }
    if (Authenticate(authenticated).has_value()) return false;
    if (IssueTokens(identity, 201, std::string(kPublicKey), {}).status != 201 ||
        IssueTokens(identity, 201, std::string(kConflictKey), {}).status != 409) return false;
    Request rotate{.method = "POST", .path = "/api/v2/devices/refresh",
                   .body = json({{"refresh_token", refresh}}).dump()};
    if (RefreshDevice(rotate).status != 200 || RefreshDevice(rotate).status != 401) return false;
    if (persistent_state_->payload().dump().find(refresh) != std::string::npos) return false;

    const json canonical = {{"result_id", "durable-idempotency"}};
    const json response = {{"result_id", "durable-idempotency"},
                           {"progression", json::array()}};
    {
      std::lock_guard lock(mutex_);
      DurableData staged = CaptureDurableLocked();
      staged.ranked_results["durable-idempotency"] = response;
      staged.ranked_result_requests["durable-idempotency"] = canonical.dump();
      if (!SaveAndCommitLocked(std::move(staged))) return false;
    }
    Request replay{.method = "POST", .path = "/api/v2/progression/results",
                   .body = canonical.dump()};
    if (RankedResult(replay, identity).status != 501) return false;
    replay.body = json({{"result_id", "durable-idempotency"}, {"different", true}}).dump();
    if (RankedResult(replay, identity).status != 501) return false;

    const Identity invite_recipient{.device_id = "durable-invite-recipient",
                                    .xuid = "0x00000000000000d2",
                                    .machine_id = "0x00000000000000e2",
                                    .player_name = "Invite Recipient"};
    const Identity unrelated_recipient{.device_id = "durable-invite-unrelated",
                                       .xuid = "0x00000000000000d3",
                                       .machine_id = "0x00000000000000e3",
                                       .player_name = "Unrelated Recipient"};
    const std::string durable_invite_id = "invite_durable_replay";
    const std::string durable_invite_session = "0x0000000000000d02";
    const std::string durable_invite_key = "invite-accept-durable-1";
    {
      std::lock_guard lock(mutex_);
      sessions_[durable_invite_session] = {
          {"session_id", durable_invite_session},
          {"members", json::array({json{{"xuid", identity.xuid}},
                                    json{{"xuid", invite_recipient.xuid}}})},
      };
      DurableData staged = CaptureDurableLocked();
      staged.invites[durable_invite_id] = {
          {"id", durable_invite_id},
          {"sender_xuid", identity.xuid},
          {"recipient_xuid", invite_recipient.xuid},
          {"session_id", durable_invite_session},
          {"custom_data", ""},
          {"revision", 1},
          {"state", "pending"},
          {"acknowledged", false},
          {"created_at", UtcIsoAfter()},
          {"expires_at_unix", UnixSecondsAfter(kInviteLifetime)},
      };
      if (!SaveAndCommitLocked(std::move(staged))) return false;
    }
    Request accept_invite{
        .method = "POST",
        .path = "/api/v2/invites/" + durable_invite_id + "/accept",
        .headers = {{"idempotency-key", durable_invite_key}},
        .body = json({{"expected_revision", 1}}).dump(),
    };
    const Response first_accept = AcceptInvite(accept_invite, invite_recipient);
    const Response response_loss_retry = AcceptInvite(accept_invite, invite_recipient);
    Request collision = accept_invite;
    collision.body = json({{"expected_revision", 2}}).dump();
    if (first_accept.status != 200 || response_loss_retry.status != 200 ||
        response_loss_retry.body != first_accept.body ||
        AcceptInvite(collision, invite_recipient).status != 409 ||
        AcceptInvite(accept_invite, unrelated_recipient).status != 404) {
      return false;
    }

    const std::string durable_session = "0x0000000000000d01";
    const json stat_request = SingleStatWriteRequest(
        durable_session, "1", identity.xuid, "0x00000001",
        {{"0x1000000e", {{"type", "i32"}, {"value", 3}}}});
    const auto stat_digest = Sha256Hex(stat_request.dump());
    const std::string title_blob = "AAABkAECAwQAAAH2oLDA0A";
    const auto decoded_title_blob = DecodeBase64Url(title_blob);
    const auto title_digest =
        decoded_title_blob
            ? Sha256Hex(std::string_view(
                  reinterpret_cast<const char*>(decoded_title_blob->data()),
                  decoded_title_blob->size()))
            : std::nullopt;
    if (!stat_digest || !title_digest) return false;
    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    const std::string owner_key = StatSequenceOwnerKey(identity.xuid, durable_session);
    staged.stat_next_sequences[owner_key] = 2;
    staged.stat_write_receipts[StatWriteReceiptKey(identity.xuid, durable_session, 1)] =
        StatWriteReceipt{*stat_digest, 204};
    staged.stats[identity.xuid]["0x00000001"]["0x1000000e"] =
        {{"type", "i32"}, {"value", 3}};
    staged.title_profile_settings[identity.xuid] =
        TitleProfileSettingRecord{1, title_blob, *title_digest, UtcIsoAfter()};
    return SaveAndCommitLocked(std::move(staged));
  }

  bool VerifyRestartedStateSelfTest() {
    const Identity identity{.device_id = "durable-device",
                            .xuid = "0x00000000000000d1",
                            .machine_id = "0x00000000000000e1",
                            .player_name = "Durable Player"};
    const std::string durable_session = "0x0000000000000d01";
    {
      std::lock_guard lock(mutex_);
      if (!devices_.contains("durable-device") ||
          !player_names_.contains(identity.xuid) ||
          !achievements_.contains(identity.xuid) ||
          !achievements_.at(identity.xuid).contains(kFirstGta4AchievementId) ||
          !achievements_.at(identity.xuid).contains(kLastGta4AchievementId) ||
          !entitlements_.contains("0x00000000000000d1") ||
          !entitlements_.at("0x00000000000000d1").contains("TLAD") ||
          !ranked_results_.contains("durable-idempotency") ||
          !invite_accept_receipts_.empty() || !invites_.empty() ||
          !stat_write_receipts_.empty() || !stat_next_sequences_.empty() ||
          !title_profile_settings_.contains(identity.xuid) || !sessions_.empty() ||
          !prog_ach_records_.contains(Lower(identity.xuid)) ||
          !lobbies_.empty() || !tickets_.empty() || !access_tokens_.empty() ||
          !challenges_.empty() || !presence_.empty() || !relay_routes_.empty() ||
          !relay_queues_.empty() || !voice_routes_.empty() || !voice_queues_.empty() ||
          !voice_queue_encoded_bytes_.empty() || !voice_route_next_cursors_.empty() ||
          !voice_route_delivered_cursors_.empty() ||
          !voice_route_replay_after_cursors_.empty() || !realtime_events_.empty()) {
        return false;
      }
    }
    json stat_request = SingleStatWriteRequest(
        durable_session, "1", identity.xuid, "0x00000001",
        {{"0x1000000e", {{"type", "i32"}, {"value", 3}}}});
    Request replay{.method = "POST", .path = "/api/v2/stats/writes",
                   .body = stat_request.dump()};
    if (WriteStats(replay, identity).status != 403) return false;
    stat_request["views"][0]["rows"][0]["columns"]["0x1000000e"]["value"] = 4;
    replay.body = stat_request.dump();
    if (WriteStats(replay, identity).status != 403) return false;
    const Identity invite_recipient{.device_id = "durable-invite-recipient",
                                    .xuid = "0x00000000000000d2",
                                    .machine_id = "0x00000000000000e2",
                                    .player_name = "Invite Recipient"};
    Request invite_replay{
        .method = "POST",
        .path = "/api/v2/invites/invite_durable_replay/accept",
        .headers = {{"idempotency-key", "invite-accept-durable-1"}},
        .body = json({{"expected_revision", 1}}).dump(),
    };
    const Response restarted_replay = AcceptInvite(invite_replay, invite_recipient);
    invite_replay.body = json({{"expected_revision", 2}}).dump();
    if (restarted_replay.status != 404 ||
        restarted_replay.body["error"].value("code", "") != "invite_not_found" ||
        AcceptInvite(invite_replay, invite_recipient).status != 404) {
      return false;
    }
    const Response restarted_prog_ach =
        ProgAchStorage({.method = "GET",
                        .path = "/api/v3/storage/0x545407F2/3/Prog_ACH"},
                       identity);
    if (restarted_prog_ach.status != 200 ||
        restarted_prog_ach.body.value("blob", "") !=
            std::string(kGta4ProgAchBase64UrlCharacters, 'A')) {
      return false;
    }
    return TitleProfileSetting(
               {.method = "GET",
                .path = "/api/v3/profile/title-settings/0x63e83fff"},
               identity)
               .body.value("blob", "") == "AAABkAECAwQAAAH2oLDA0A";
  }

  bool VerifyFaultAtomicitySelfTest(const Identity& identity) {
    const std::string session_id = "0x0000000000000f01";
    {
      std::lock_guard lock(mutex_);
      sessions_[session_id] = {
          {"session_id", session_id},
          {"members", json::array({json{{"xuid", identity.xuid}}})}};
      session_leases_[session_id] = Clock::now() + kSessionLease;
      stat_next_sequences_[StatSequenceOwnerKey(identity.xuid, session_id)] = 2;
    }
    Request stat_write{
        .method = "POST",
        .path = "/api/v2/stats/writes",
        .body = SingleStatWriteRequest(
                    session_id, "1", identity.xuid, "0x0000006d",
                    {{"0x2000000d", {{"type", "i64"}, {"value", 1000}}}})
                    .dump()};
    if (WriteStats(stat_write, identity).status != 503) return false;
    {
      std::lock_guard lock(mutex_);
      if (stats_.contains(identity.xuid) || progression_.contains(identity.xuid) ||
          stat_write_receipts_.contains(StatWriteReceiptKey(identity.xuid, session_id, 1))) {
        return false;
      }
    }
    Request profile_write{
        .method = "PUT",
        .path = "/api/v3/profile/title-settings/0x63e83fff",
        .body = json({{"title_id", "0x545407f2"},
                      {"setting_id", "0x63e83fff"},
                      {"expected_revision", "0"},
                      {"blob", "AAABkAECAwQAAAH2oLDA0A"}})
                    .dump()};
    if (TitleProfileSetting(profile_write, identity).status != 503) return false;
    {
      std::lock_guard lock(mutex_);
      if (title_profile_settings_.contains(identity.xuid)) return false;
    }
    Request prog_ach_write{
        .method = "PUT",
        .path = "/api/v3/storage/0x545407F2/3/Prog_ACH",
        .body = json({{"blob", std::string(kGta4ProgAchBase64UrlCharacters, 'A')}}).dump(),
    };
    if (ProgAchStorage(prog_ach_write, identity).status != 503) return false;
    {
      std::lock_guard lock(mutex_);
      if (prog_ach_records_.contains(Lower(identity.xuid))) return false;
    }
    if (GrantEntitlement(identity.xuid, "TBOGT")) return false;
    {
      std::lock_guard lock(mutex_);
      if (entitlements_.contains(Lower(identity.xuid))) return false;
    }
    Request merge{.method = "POST",
                  .path = "/api/v2/achievements",
                  .body = json({{"achievement_ids",
                                 json::array({kFirstGta4AchievementId})}}).dump()};
    if (Achievements(merge, identity).status != 503 ||
        Achievements({.method = "GET", .path = "/api/v2/achievements"}, identity)
            .body.at("achievement_ids").size() != 0) {
      return false;
    }
    const auto health = Dispatch({.method = "GET", .path = "/health/ready"});
    return health.status == 200 && health.body.at("storage").value("status", "") == "degraded";
  }

  bool VerifyResourceLeaseSelfTest() {
    const Identity host{.device_id = "lease-host",
                        .xuid = "0x0000000000000a01",
                        .machine_id = "0x0000000000000a11",
                        .player_name = "Lease Host"};
    const Identity peer{.device_id = "lease-peer",
                        .xuid = "0x0000000000000a02",
                        .machine_id = "0x0000000000000a12",
                        .player_name = "Lease Peer"};
    const std::string expired_id = "0x0000000000000b01";
    const std::string live_id = "0x0000000000000b02";
    const auto session = [&](const std::string& id) {
      return json{{"session_id", id}, {"host_xuid", host.xuid},
                  {"title_id", "0x545407F2"}, {"media_id", "0x00000000"},
                  {"title_version", "0x00000001"}, {"protocol_version", 2},
                  {"state", "open"}, {"visibility", "public"},
                  {"mode", "gta4"}, {"episode", "all"}, {"region", "global"},
                  {"public_slots", 64}, {"private_slots", 0}, {"ranked", false},
                  {"contexts", json::object()}, {"properties", json::object()},
                  {"revision", 1}, {"host_epoch", 1},
                  {"members", json::array({json{{"xuid", host.xuid}},
                                             json{{"xuid", peer.xuid}}})}};
    };
    const auto now = Clock::now();
    const std::int64_t unix_now = UnixSecondsAfter();
    {
      std::lock_guard lock(mutex_);
      sessions_[expired_id] = session(expired_id);
      session_leases_[expired_id] = now;
      lobbies_[expired_id].ready[host.xuid] = true;
      tickets_["matched-expired"] = {
          .id = "matched-expired", .owner_xuid = peer.xuid, .state = "matched",
          .matched_session_id = expired_id, .expires = now + kMatchmakingTicketLease};
      tickets_["expired-ticket"] = {
          .id = "expired-ticket", .owner_xuid = peer.xuid, .state = "searching",
          .expires = now};
      relay_routes_["expired-session-relay"] = {
          .xuid = host.xuid, .session_id = expired_id, .port = 3074,
          .virtual_ipv4 = "192.168.100.1", .expires = now + kRelayRouteLease};
      relay_queues_["expired-session-relay"].push_back({.payload = "AA=="});
      relay_routes_["expired-standalone-relay"] = {
          .xuid = peer.xuid, .session_id = live_id, .port = 3075,
          .virtual_ipv4 = "192.168.100.2", .expires = now};
      relay_queues_["expired-standalone-relay"].push_back({.payload = "AA=="});
      voice_routes_["expired-session-voice"] = {
          .owner_xuid = host.xuid, .session_id = expired_id,
          .expires = now + kVoiceRouteLease};
      voice_routes_["expired-standalone-voice"] = {
          .owner_xuid = peer.xuid, .session_id = live_id, .expires = now};
      voice_queues_["expired-session-voice"].push_back(
          {.session_id = expired_id, .encoded_bytes = 4});
      voice_queue_encoded_bytes_["expired-session-voice"] = 4;
      voice_route_next_cursors_["expired-session-voice"] = 1;
      realtime_events_[peer.xuid].push_back(
          {.id = 1,
           .session_id = expired_id,
           .source_xuid = host.xuid,
           .wire = {{"id", 1}}});
      challenges_["expired-challenge"] = {.expires = now};
      access_tokens_["expired-access"] = {.identity = host, .expires = now};
      presence_[host.xuid] = {.expires = now};
      refresh_sessions_["expired-refresh"] = {
          .identity = host, .expires_at_unix = unix_now};
      invites_["expired-session-invite"] = {
          {"session_id", expired_id}, {"expires_at_unix", UnixSecondsAfter(kInviteLifetime)}};
      if (!SweepExpiredLocked(now, unix_now)) return false;
      if (sessions_.contains(expired_id) || session_leases_.contains(expired_id) ||
          lobbies_.contains(expired_id) || relay_routes_.contains("expired-session-relay") ||
          relay_queues_.contains("expired-session-relay") ||
          relay_routes_.contains("expired-standalone-relay") ||
          relay_queues_.contains("expired-standalone-relay") ||
          voice_routes_.contains("expired-session-voice") ||
          voice_routes_.contains("expired-standalone-voice") ||
          voice_queues_.contains("expired-session-voice") ||
          voice_queue_encoded_bytes_.contains("expired-session-voice") ||
          voice_route_next_cursors_.contains("expired-session-voice") ||
          realtime_events_.contains(peer.xuid) ||
          tickets_.contains("expired-ticket") || challenges_.contains("expired-challenge") ||
          access_tokens_.contains("expired-access") || presence_.contains(host.xuid) ||
          refresh_sessions_.contains("expired-refresh") ||
          invites_.contains("expired-session-invite")) {
        return false;
      }
      const auto matched = tickets_.find("matched-expired");
      if (matched == tickets_.end() || matched->second.state != "searching" ||
          !matched->second.matched_session_id.empty()) {
        return false;
      }
      sessions_[live_id] = session(live_id);
      session_leases_[live_id] = Clock::now() + kSweepInterval;
    }

    Request peer_heartbeat{
        .method = "POST", .path = "/api/v2/sessions/" + live_id + "/heartbeat",
        .body = json({{"expected_revision", 1}}).dump()};
    if (SessionOperation(peer_heartbeat, peer).status != 403) return false;
    Clock::time_point short_lease;
    {
      std::lock_guard lock(mutex_);
      short_lease = session_leases_.at(live_id);
    }
    Request host_heartbeat = peer_heartbeat;
    if (SessionOperation(host_heartbeat, host).status != 200) return false;
    std::lock_guard lock(mutex_);
    return session_leases_.at(live_id) > short_lease;
  }

  bool VerifyMultiplayer64Simulation() {
    const auto make_identity = [](std::string_view xuid) {
      const std::string value = libserver::CanonicalIdentity(std::string(xuid));
      return Identity{.device_id = "bot-" + value,
                      .xuid = value,
                      .machine_id = value,
                      .player_name = "MP64 Bot"};
    };
    const std::string session_id = "0x6400000000000001";
    const Identity host = make_identity(libserver::multiplayer64::kBotXuids.front());
    const json session = {
        {"session_id", session_id},
        {"host_xuid", host.xuid},
        {"title_id", "0x545407F2"},
        {"media_id", "0x00000000"},
        {"title_version", "0x00000001"},
        {"protocol_version", 2},
        {"state", "open"},
        {"visibility", "public"},
        {"mode", "gta4"},
        {"episode", "all"},
        {"region", "local-simulation"},
        {"public_slots", kMaximumSessionMembers},
        {"private_slots", 0},
        {"ranked", false},
        {"contexts", json::object()},
        {"properties", json::object()},
        {"members", json::array({json{{"xuid", host.xuid},
                                        {"machine_id", host.machine_id},
                                        {"peer_id", ""},
                                        {"private", false}}})}};
    Request create{.method = "POST", .path = "/api/v2/sessions", .body = session.dump()};
    Response current = CreateSession(create, host);
    if (current.status != 201 || !current.body.contains("members") ||
        current.body.at("members").empty()) {
      return false;
    }
    if (current.body.value("host_peer_id", "") != host.xuid ||
        current.body.at("members").front().value("peer_id", "") != host.xuid ||
        !current.body.at("members").front().contains("route") ||
        current.body.at("members").front().at("route").value("connection_id", "") !=
            host.xuid) {
      return false;
    }

    for (std::size_t peer_id = 1; peer_id < libserver::multiplayer64::kBotXuids.size();
         ++peer_id) {
      const Identity bot = make_identity(libserver::multiplayer64::kBotXuids[peer_id]);
      Request join{.method = "POST",
                   .path = "/api/v2/sessions/" + session_id + "/join",
                   .body = json({{"expected_revision", current.body.value("revision", 0)},
                                 {"private", false},
                                 {"member", {{"xuid", bot.xuid},
                                              {"machine_id", bot.machine_id},
                                              {"peer_id", ""},
                                              {"private", false}}}})
                               .dump()};
      current = SessionOperation(join, bot);
      if (current.status != 200) return false;
    }
    if (current.body.at("members").size() != kMaximumSessionMembers) return false;
    for (const std::size_t boundary : libserver::multiplayer64::kPeerBoundaries) {
      const auto& member = current.body.at("members").at(boundary);
      if (member.value("xuid", "") != libserver::multiplayer64::kBotXuids[boundary] ||
          member.value("peer_id", "") != libserver::multiplayer64::kBotXuids[boundary] ||
          !member.contains("route") ||
          member.at("route").value("peer_id", kMaximumSessionMembers) != boundary ||
          member.at("route").value("connection_id", "") !=
              libserver::multiplayer64::kBotXuids[boundary]) {
        return false;
      }
    }

    std::vector<std::string> voice_tokens;
    voice_tokens.reserve(kMaximumSessionMembers);
    for (const std::string_view bot_xuid : libserver::multiplayer64::kBotXuids) {
      const Identity bot = make_identity(bot_xuid);
      Request route_request{
          .method = "POST",
          .path = "/api/v2/voice/routes",
          .body = json({{"session_id", session_id},
                        {"channel", "all"},
                        {"target_xuids", json::array()},
                        {"mute_xuids", json::array()}})
                      .dump()};
      const Response route = VoiceRoutes(route_request, bot);
      const std::string token = route.body.value("route_token", "");
      if (route.status != 201 || token.empty()) return false;
      voice_tokens.push_back(token);
    }
    if (voice_tokens.size() != kMaximumSessionMembers) return false;
    Request full_voice_delivery{
        .method = "POST",
        .path = "/api/v2/voice/packets",
        .body = json({{"packets", json::array({json{
            {"route_token", voice_tokens.front()},
            {"sequence", 1},
            {"payload", "AA=="}}})}})
                    .dump()};
    if (VoicePackets(full_voice_delivery, host).status != 204) return false;
    std::size_t voice_delivery_count = 0;
    for (std::size_t peer_id = 1; peer_id < voice_tokens.size(); ++peer_id) {
      const Identity bot =
          make_identity(libserver::multiplayer64::kBotXuids[peer_id]);
      Request receive_voice{
          .method = "GET",
          .path = "/api/v2/voice/packets",
          .query = {{"route_token", voice_tokens[peer_id]}}};
      const Response received = VoicePackets(receive_voice, bot);
      if (received.status != 200 || received.body.at("packets").size() != 1 ||
          received.body.at("packets")[0].value("source_xuid", "") != host.xuid) {
        return false;
      }
      ++voice_delivery_count;
    }
    if (voice_delivery_count != kMaximumRemoteSessionMembers) return false;

    json full_team_targets = json::array();
    for (std::size_t peer_id = 1;
         peer_id < libserver::multiplayer64::kBotXuids.size(); ++peer_id) {
      full_team_targets.push_back(libserver::multiplayer64::kBotXuids[peer_id]);
    }
    if (full_team_targets.size() != kMaximumRemoteSessionMembers) return false;
    Request full_team_route{
        .method = "POST",
        .path = "/api/v2/voice/routes",
        .body = json({{"session_id", session_id},
                      {"channel", "team"},
                      {"target_xuids", std::move(full_team_targets)},
                      {"mute_xuids", json::array()}})
                    .dump()};
    const Response team_route = VoiceRoutes(full_team_route, host);
    voice_tokens.front() = team_route.body.value("route_token", "");
    full_voice_delivery.body = json({{"packets", json::array({json{
        {"route_token", voice_tokens.front()},
        {"sequence", 2},
        {"payload", "AA=="}}})}}).dump();
    if (team_route.status != 201 || voice_tokens.front().empty() ||
        VoicePackets(full_voice_delivery, host).status != 204) {
      return false;
    }
    for (std::size_t peer_id = 1; peer_id < voice_tokens.size(); ++peer_id) {
      const Identity bot =
          make_identity(libserver::multiplayer64::kBotXuids[peer_id]);
      Request receive_team_voice{
          .method = "GET",
          .path = "/api/v2/voice/packets",
          .query = {{"route_token", voice_tokens[peer_id]}}};
      const Response received = VoicePackets(receive_team_voice, bot);
      if (received.status != 200 || received.body.at("packets").size() != 1 ||
          received.body.at("packets")[0].value("sequence", 0) != 2) {
        return false;
      }
    }

    Request full_chat_delivery{
        .method = "POST",
        .path = "/api/v2/chat/messages",
        .headers = {{"idempotency-key", "chat-mp64-all"}},
        .body = json({{"session_id", session_id},
                      {"channel", "all"},
                      {"target_xuids", json::array()},
                      {"sequence", 1},
                      {"text", "Full lobby"}})
                    .dump()};
    const Response full_chat = ChatMessage(full_chat_delivery, host);
    if (full_chat.status != 201) return false;
    const std::int64_t full_chat_id = full_chat.body.at("id").get<std::int64_t>();
    std::size_t chat_delivery_count = 0;
    for (std::size_t peer_id = 1; peer_id < voice_tokens.size(); ++peer_id) {
      const Identity bot =
          make_identity(libserver::multiplayer64::kBotXuids[peer_id]);
      Request receive_chat{.method = "GET",
                           .path = "/api/v2/events",
                           .query = {{"after", "0"}, {"wait_ms", "0"}}};
      const Response received = RealtimeEvents(receive_chat, bot);
      if (received.status != 200 || received.body.at("events").size() != 1 ||
          received.body.at("events")[0].at("payload").value("source_xuid", "") !=
              host.xuid) {
        return false;
      }
      ++chat_delivery_count;
      receive_chat.query["after"] = std::to_string(full_chat_id);
      if (!RealtimeEvents(receive_chat, bot).body.at("events").empty()) return false;
    }
    Request host_chat{.method = "GET",
                      .path = "/api/v2/events",
                      .query = {{"after", "0"}, {"wait_ms", "0"}}};
    if (chat_delivery_count != kMaximumRemoteSessionMembers ||
        !RealtimeEvents(host_chat, host).body.at("events").empty()) {
      return false;
    }
    for (std::size_t peer_id = 0; peer_id < voice_tokens.size(); ++peer_id) {
      const Identity bot =
          make_identity(libserver::multiplayer64::kBotXuids[peer_id]);
      Request remove_route{
          .method = "DELETE",
          .path = "/api/v2/voice/routes",
          .body = json({{"route_token", voice_tokens[peer_id]}}).dump()};
      if (VoiceRoutes(remove_route, bot).status != 204) return false;
    }

    const Identity overflow = make_identity("0x2000000000000040");
    Request overflow_join{
        .method = "POST",
        .path = "/api/v2/sessions/" + session_id + "/join",
        .body = json({{"expected_revision", current.body.value("revision", 0)},
                      {"private", false},
                      {"member", {{"xuid", overflow.xuid}, {"private", false}}}})
                    .dump()};
    if (SessionOperation(overflow_join, overflow).status != 409) return false;

    constexpr std::size_t kReuseBoundary = 16;
    const Identity leaving = make_identity(libserver::multiplayer64::kBotXuids[kReuseBoundary]);
    Request leave{.method = "POST",
                  .path = "/api/v2/sessions/" + session_id + "/leave",
                  .body = json({{"expected_revision", current.body.value("revision", 0)}}).dump()};
    current = SessionOperation(leave, leaving);
    if (current.status != 200) return false;
    const Identity replacement = make_identity("0x2000000000000010");
    Request reuse{.method = "POST",
                  .path = "/api/v2/sessions/" + session_id + "/join",
                  .body = json({{"expected_revision", current.body.value("revision", 0)},
                                {"private", false},
                                {"member", {{"xuid", replacement.xuid}, {"private", false}}}})
                              .dump()};
    current = SessionOperation(reuse, replacement);
    if (current.status != 200) return false;
    const auto reused = std::find_if(current.body.at("members").begin(),
                                     current.body.at("members").end(), [&](const json& member) {
                                       return member.value("xuid", "") == replacement.xuid;
                                     });
    return reused != current.body.at("members").end() && reused->contains("route") &&
           reused->at("route").value("peer_id", kMaximumSessionMembers) == kReuseBoundary;
  }

  bool VerifyRelaySelfTest() {
    const Identity host{.device_id = "relay-host-device",
                        .xuid = "0x0000000000000c01",
                        .machine_id = "0x0000000000000d01",
                        .player_name = "Relay Host"};
    const Identity peer{.device_id = "relay-peer-device",
                        .xuid = "0x0000000000000c02",
                        .machine_id = "0x0000000000000d02",
                        .player_name = "Relay Peer"};
    const std::string session_id = "0xc100000000000001";
    {
      std::lock_guard lock(mutex_);
      sessions_[session_id] = {
          {"session_id", session_id},
          {"host_xuid", host.xuid},
          {"revision", 1},
          {"members", json::array({json{{"xuid", host.xuid},
                                           {"virtual_ipv4", "192.168.100.1"}},
                                      json{{"xuid", peer.xuid},
                                           {"virtual_ipv4", "192.168.100.2"}}})}};
      session_leases_[session_id] = Clock::now() + kSessionLease;
    }

    const auto route_request = [&](std::string method, std::uint16_t local_port) {
      return Request{.method = std::move(method),
                     .path = "/api/v2/relay/routes",
                     .body = json({{"session_id", session_id},
                                   {"local_port", local_port}})
                                 .dump()};
    };
    if (RelayRoutes(route_request("POST", 41000), host).status != 200 ||
        RelayRoutes(route_request("POST", 41001), peer).status != 200) {
      return false;
    }

    Request send{.method = "POST",
                 .path = "/api/v2/relay/datagrams",
                 .body = json({{"datagrams",
                                json::array({{{"destination_ipv4", "192.168.100.2"},
                                                {"destination_port", 41001},
                                                {"source_port", 41000},
                                                {"payload", "R1RBNA"}}})}})
                             .dump()};
    if (RelayDatagrams(send, host).status != 204) return false;
    Request receive{.method = "GET",
                    .path = "/api/v2/relay/datagrams",
                    .query = {{"local_port", "41001"}}};
    const Response delivered = RelayDatagrams(receive, peer);
    if (delivered.status != 200 || delivered.body.at("datagrams").size() != 1) {
      return false;
    }
    const json& datagram = delivered.body.at("datagrams").at(0);
    if (datagram.value("source_ipv4", "") != "192.168.100.1" ||
        datagram.value("source_port", 0) != 41000 ||
        datagram.value("payload", "") != "R1RBNA") {
      return false;
    }

    if (RelayDatagrams(send, host).status != 204) return false;
    Request leave{.method = "POST",
                  .path = "/api/v2/sessions/" + session_id + "/leave",
                  .body = json({{"expected_revision", 1}}).dump()};
    if (SessionOperation(leave, peer).status != 200 ||
        RelayDatagrams(receive, peer).status != 404 ||
        RelayRoutes(route_request("POST", 41001), peer).status != 403 ||
        RelayRoutes(route_request("DELETE", 41000), host).status != 204) {
      return false;
    }

    std::lock_guard lock(mutex_);
    sessions_.erase(session_id);
    session_leases_.erase(session_id);
    return !relay_routes_.contains(host.xuid + ":41000") &&
           !relay_routes_.contains(peer.xuid + ":41001") &&
           !relay_queues_.contains(peer.xuid + ":41001");
  }

  bool VerifyQosSelfTest() {
    const Identity host{"qos-host", "0x0000000000000a01", "0x0000000000000b01", "Host"};
    const Identity peer{"qos-peer", "0x0000000000000a02", "0x0000000000000b02", "Peer"};
    const std::string id = "0xa100000000000001";
    const std::string key = "0x000102030405060708090a0b0c0d0e0f";
    const std::string title = "AAECAwQFBgcICQoL";
    sessions_[id] = {{"session_id", id}, {"host_xuid", host.xuid},
                    {"host_epoch", 1}, {"exchange_key", key}, {"visibility", "public"},
                    {"members", json::array({{{"xuid", host.xuid},
                                               {"virtual_ipv4", "192.168.100.1"}}})}};
    session_leases_[id] = Clock::now() + kSessionLease;
    relay_routes_["qos-test-route"] = {.xuid = host.xuid, .session_id = id,
        .port = 3074, .virtual_ipv4 = "192.168.100.1", .expires = Clock::now() + kRelayRouteLease};
    Request listen{.method = "PUT", .path = "/api/v2/qos/listeners",
                   .body = json({{"session_id", id}, {"exchange_key", key},
                                 {"enabled", true}, {"title_data", title}}).dump()};
    if (QosListeners(listen, peer).status != 403 || QosListeners(listen, host).status != 204) return false;
    const auto make_lookup = [&] {
      return Request{.method = "POST", .path = "/api/v2/qos/lookup",
          .body = json({{"targets", json::array({{{"session_id", id}, {"exchange_key", key},
                                                 {"challenge", RandomToken("0x", 16)}}})}}).dump()};
    };
    const Request lookup = make_lookup();
    if (QosLookup(lookup, peer).status != 202) return false;
    const auto pending = QosProbes({.method = "GET"}, host);
    if (pending.body.at("probes").size() != 1) return false;
    const auto& probe = pending.body.at("probes").front();
    Request ack{.method = "POST", .body = json({{"probe_id", probe.at("probe_id")},
                                                {"challenge", probe.at("challenge")}}).dump()};
    if (QosAcknowledge(ack, peer).status != 403 || QosAcknowledge(ack, host).status != 204 ||
        QosAcknowledge(ack, host).status != 204) return false;
    const auto reachable = QosLookup(lookup, peer);
    if (reachable.status != 200 || !reachable.body.at("results").front().value("reachable", false) ||
        reachable.body.at("results").front().value("title_data", "") != title ||
        reachable.body.value("measurement", "") != "relay-host-ack-v1") return false;
    auto disable = json::parse(listen.body);
    disable["enabled"] = false;
    Request disabled = listen; disabled.body = disable.dump();
    if (QosListeners(disabled, host).status != 204 || QosAcknowledge(ack, host).status != 403 ||
        QosLookup(lookup, peer).body.at("results").front().value("reachable", true)) return false;
    if (QosListeners(listen, host).status != 204) return false;
    const auto no_response = make_lookup();
    if (QosLookup(no_response, peer).status != 202) return false;
    for (auto& [digest, batch] : qos_probe_batches_) { (void)digest; batch.deadline = Clock::now(); }
    if (QosLookup(no_response, peer).body.at("results").front().value("reachable", true)) return false;
    relay_routes_.at("qos-test-route").expires = Clock::now();
    if (QosLookup(make_lookup(), peer).body.at("results").front().value("reachable", true)) return false;
    json oversize = json::array();
    for (std::size_t i = 0; i < kOversizeQosTargets; ++i) oversize.push_back(json::parse(lookup.body).at("targets").front());
    if (QosLookup({.body = json({{"targets", oversize}}).dump()}, peer).status != 400) return false;
    RemoveSessionResourcesLocked(id);
    return !qos_listeners_.contains(id);
  }

  bool VerifySessionMutationIdempotencySelfTest() {
    const Identity host{.device_id = "receipt-host",
                        .xuid = "0x00000000000000a1",
                        .machine_id = "0x0000000000000a11",
                        .player_name = "Receipt Host"};
    const Identity peer{.device_id = "receipt-peer",
                        .xuid = "0x00000000000000a2",
                        .machine_id = "0x0000000000000a22",
                        .player_name = "Receipt Peer"};
    const auto member = [](const Identity& identity) {
      return json{{"xuid", identity.xuid},
                  {"machine_id", identity.machine_id},
                  {"private", false}};
    };
    const auto session_body = [&](std::string id, const Identity& owner,
                                  json members, std::uint32_t flags,
                                  std::string nonce) {
      return json{{"session_id", std::move(id)},
                  {"host_xuid", owner.xuid},
                  {"title_id", "0x545407f2"},
                  {"media_id", "0x00000000"},
                  {"title_version", "0x00000001"},
                  {"protocol_version", 2},
                  {"state", "lobby"},
                  {"visibility", "public"},
                  {"mode", "gta4"},
                  {"episode", "all"},
                  {"region", "receipt-test"},
                  {"ranked", false},
                  {"public_slots", kMaximumSessionMembers},
                  {"private_slots", 0},
                  {"contexts", json::object()},
                  {"properties", json::object()},
                  {"flags", flags},
                  {"lifecycle_state", 0},
                  {"join_in_progress", true},
                  {"nonce", std::move(nonce)},
                  {"host_machine_id", owner.machine_id},
                  {"members", std::move(members)}};
    };
    const auto mutation_request = [](std::string method, std::string path,
                                     const json& body, std::string key) {
      return Request{.method = std::move(method),
                     .path = std::move(path),
                     .headers = {{"idempotency-key", std::move(key)}},
                     .body = body.dump()};
    };
    const auto same_response = [](const Response& left, const Response& right) {
      return left.status == right.status && left.body == right.body;
    };
    const auto error_code = [](const Response& response) {
      return response.body.contains("error") && response.body.at("error").is_object()
                 ? response.body.at("error").value("code", std::string{})
                 : std::string{};
    };

    const std::string session_id = "0x0000000000000a40";
    const std::string session_path = "/api/v2/sessions/" + session_id;
    json initial = session_body(session_id, host, json::array({member(host)}),
                                0, "0x0000000000000a41");
    Request create = mutation_request("POST", "/api/v2/sessions", initial,
                                      "session-create-response-loss");
    const Response committed_create = CreateSession(create, host);
    // Treat each first result named committed_* as the server-side result whose transport response
    // was lost. The immediate repeat must return that exact status/body from the receipt.
    const Response replayed_create = CreateSession(create, host);
    if (committed_create.status != 201 ||
        !same_response(committed_create, replayed_create)) {
      return false;
    }
    json conflicting_create_body = initial;
    conflicting_create_body["region"] = "receipt-conflict";
    Request conflicting_create = mutation_request(
        "POST", "/api/v2/sessions", conflicting_create_body,
        "session-create-response-loss");
    const Response create_conflict = CreateSession(conflicting_create, host);
    if (create_conflict.status != 409 ||
        error_code(create_conflict) != "idempotency_conflict") {
      return false;
    }
    json peer_owned_session = session_body(
        "0x0000000000000a43", peer, json::array({member(peer)}),
        0, "0x0000000000000a44");
    Request peer_same_key_create = mutation_request(
        "POST", "/api/v2/sessions", peer_owned_session,
        "session-create-response-loss");
    if (CreateSession(peer_same_key_create, peer).status != 201) {
      return false;
    }

    json modify_body = {
        {"expected_revision", committed_create.body.at("revision")},
        {"contexts", {{"0x00000001", 7}}}};
    Request modify = mutation_request("PATCH", session_path, modify_body,
                                      "session-modify-response-loss");
    const Response committed_modify = SessionOperation(modify, host);
    const Response replayed_modify = SessionOperation(modify, host);
    if (committed_modify.status != 200 ||
        !same_response(committed_modify, replayed_modify)) {
      return false;
    }

    json stale_modify_body = {
        {"expected_revision", committed_create.body.at("revision")},
        {"region", "receipt-revision-retry"}};
    Request stale_modify = mutation_request("PATCH", session_path,
                                            stale_modify_body,
                                            "session-explicit-412");
    const Response first_precondition = SessionOperation(stale_modify, host);
    stale_modify_body["expected_revision"] = committed_modify.body.at("revision");
    Request same_key_new_revision = mutation_request(
        "PATCH", session_path, stale_modify_body, "session-explicit-412");
    const Response replayed_precondition =
        SessionOperation(same_key_new_revision, host);
    Request fresh_key_new_revision = mutation_request(
        "PATCH", session_path, stale_modify_body, "session-explicit-412-fresh");
    const Response recovered_modify =
        SessionOperation(fresh_key_new_revision, host);
    if (first_precondition.status != 412 ||
        !same_response(first_precondition, replayed_precondition) ||
        recovered_modify.status != 200) {
      return false;
    }

    json join_body = {{"expected_revision", recovered_modify.body.at("revision")},
                      {"member", member(peer)}};
    Request join = mutation_request("POST", session_path + "/join", join_body,
                                    "session-join-response-loss");
    const Response committed_join = SessionOperation(join, peer);
    const Response replayed_join = SessionOperation(join, peer);
    if (committed_join.status != 200 ||
        !same_response(committed_join, replayed_join) ||
        !SessionContainsXuid(committed_join.body, peer.xuid)) {
      return false;
    }

    json leave_body = {{"expected_revision", committed_join.body.at("revision")}};
    Request leave = mutation_request("POST", session_path + "/leave", leave_body,
                                     "session-leave-response-loss");
    const Response committed_leave = SessionOperation(leave, peer);
    const Response replayed_leave = SessionOperation(leave, peer);
    if (committed_leave.status != 200 ||
        !same_response(committed_leave, replayed_leave) ||
        SessionContainsXuid(committed_leave.body, peer.xuid)) {
      return false;
    }

    join_body["expected_revision"] = committed_leave.body.at("revision");
    Request rejoin = mutation_request("POST", session_path + "/join", join_body,
                                      "session-rejoin-for-migration");
    const Response rejoined = SessionOperation(rejoin, peer);
    if (rejoined.status != 200 || !SessionContainsXuid(rejoined.body, peer.xuid)) {
      return false;
    }

    const auto rejoined_peer = std::find_if(
        rejoined.body.at("members").begin(), rejoined.body.at("members").end(),
        [&](const json& candidate) {
          return candidate.value("xuid", "") == peer.xuid;
        });
    if (rejoined_peer == rejoined.body.at("members").end()) return false;
    const std::string assigned_address =
        rejoined_peer->value("virtual_ipv4", "");
    const auto assigned_route_peer =
        rejoined_peer->at("route").at("peer_id");
    json refreshed_member = member(peer);
    refreshed_member["machine_id"] = "0xffffffffffffffff";
    refreshed_member["online_port"] = 41001;
    json refresh_transport_body = {
        {"expected_revision", rejoined.body.at("revision")},
        {"member", std::move(refreshed_member)}};
    Request refresh_transport = mutation_request(
        "POST", session_path + "/join", refresh_transport_body,
        "session-refresh-transport");
    const Response refreshed_transport =
        SessionOperation(refresh_transport, peer);
    if (refreshed_transport.status != 200) return false;
    const auto refreshed_peer = std::find_if(
        refreshed_transport.body.at("members").begin(),
        refreshed_transport.body.at("members").end(),
        [&](const json& candidate) {
          return candidate.value("xuid", "") == peer.xuid;
        });
    if (refreshed_peer == refreshed_transport.body.at("members").end() ||
        refreshed_peer->value("machine_id", "") != peer.machine_id ||
        refreshed_peer->value("online_port", 0) != 41001 ||
        refreshed_peer->value("virtual_ipv4", "") != assigned_address ||
        refreshed_peer->at("route").at("peer_id") != assigned_route_peer) {
      return false;
    }

    const std::string replacement_id = "0x0000000000000a42";
    json replacement = refreshed_transport.body;
    replacement["session_id"] = replacement_id;
    replacement["host_xuid"] = peer.xuid;
    replacement["host_machine_id"] = peer.machine_id;
    json migration_body = {
        {"expected_revision", refreshed_transport.body.at("revision")},
        {"expected_host_epoch", 0},
        {"replacement", replacement}};
    Request stale_migration = mutation_request(
        "POST", session_path + "/migration", migration_body,
        "session-host-epoch-412");
    const Response first_epoch_precondition =
        SessionOperation(stale_migration, peer);
    migration_body["expected_host_epoch"] =
        refreshed_transport.body.at("host_epoch");
    Request same_key_new_epoch = mutation_request(
        "POST", session_path + "/migration", migration_body,
        "session-host-epoch-412");
    const Response replayed_epoch_precondition =
        SessionOperation(same_key_new_epoch, peer);
    Request migrate = mutation_request(
        "POST", session_path + "/migration", migration_body,
        "session-migrate-response-loss");
    const Response committed_migration = SessionOperation(migrate, peer);
    const Response replayed_migration = SessionOperation(migrate, peer);
    if (first_epoch_precondition.status != 412 ||
        !same_response(first_epoch_precondition, replayed_epoch_precondition) ||
        committed_migration.status != 200 ||
        !same_response(committed_migration, replayed_migration) ||
        committed_migration.body.value("session_id", "") != replacement_id) {
      return false;
    }

    const std::string replacement_path =
        "/api/v2/sessions/" + replacement_id;
    json delete_body = {
        {"expected_revision", committed_migration.body.at("revision")}};
    Request remove = mutation_request("DELETE", replacement_path, delete_body,
                                      "session-delete-response-loss");
    const Response committed_delete = SessionOperation(remove, peer);
    const Response replayed_delete = SessionOperation(remove, peer);
    if (committed_delete.status != 204 ||
        !same_response(committed_delete, replayed_delete) ||
        GetSession(replacement_id, peer).status != 404) {
      return false;
    }

    const std::string arbitration_id = "0x0000000000000a50";
    const std::string arbitration_path =
        "/api/v2/sessions/" + arbitration_id + "/arbitration";
    const std::string arbitration_nonce = "0x0000000000000a51";
    json arbitration_session = session_body(
        arbitration_id, host, json::array({member(host), member(peer)}),
        62, arbitration_nonce);
    Request create_arbitration = mutation_request(
        "POST", "/api/v2/sessions", arbitration_session,
        "session-arbitration-create");
    const Response arbitration_created =
        CreateSession(create_arbitration, host);
    if (arbitration_created.status != 201) return false;
    json arbitration_body = {
        {"expected_revision", arbitration_created.body.at("revision")},
        {"nonce", arbitration_nonce},
        {"registration_duration_seconds", kGta4ArbitrationRegistrationSeconds},
        {"flags", kGta4ArbitrationFlags}};
    Request host_arbitration = mutation_request(
        "POST", arbitration_path, arbitration_body,
        "session-arbitration-pending");
    const Response pending = SessionOperation(host_arbitration, host);
    if (pending.status != 202) return false;
    json conflicting_arbitration_body = arbitration_body;
    conflicting_arbitration_body["nonce"] = "0x0000000000000a52";
    Request conflicting_arbitration = mutation_request(
        "POST", arbitration_path, conflicting_arbitration_body,
        "session-arbitration-pending");
    const Response arbitration_conflict =
        SessionOperation(conflicting_arbitration, host);
    if (arbitration_conflict.status != 409 ||
        error_code(arbitration_conflict) != "idempotency_conflict") {
      return false;
    }
    json peer_arbitration_body = arbitration_body;
    peer_arbitration_body["expected_revision"] = pending.body.at("revision");
    Request peer_arbitration = mutation_request(
        "POST", arbitration_path, peer_arbitration_body,
        // Deliberately reuse the text under a different authenticated identity.
        "session-arbitration-pending");
    const Response completed_by_peer =
        SessionOperation(peer_arbitration, peer);
    const Response completed_for_host =
        SessionOperation(host_arbitration, host);
    const Response replayed_terminal_arbitration =
        SessionOperation(host_arbitration, host);
    if (completed_by_peer.status != 200 || completed_for_host.status != 200 ||
        !same_response(completed_by_peer, completed_for_host) ||
        !same_response(completed_for_host, replayed_terminal_arbitration)) {
      return false;
    }
    return true;
  }

 private:
  template <typename Mutation>
  Response RunSessionMutation(const Request& request, const Identity& identity,
                              Mutation&& mutation) {
    PreparedSessionMutation prepared = PrepareSessionMutation(request, identity);
    if (prepared.error) return std::move(*prepared.error);

    std::lock_guard lock(mutex_);
    bool pending_retry = false;
    if (prepared.enabled) {
      const auto receipt = session_mutation_receipts_.find(prepared.receipt_key);
      if (receipt != session_mutation_receipts_.end()) {
        if (receipt->second.request_digest != prepared.request_digest) {
          return Error(409, "idempotency_conflict",
                       "Idempotency-Key was already used for a different request");
        }
        receipt->second.expires = Clock::now() + kSessionMutationReceiptLifetime;
        if (receipt->second.terminal_response) {
          return *receipt->second.terminal_response;
        }
        pending_retry = true;
      }
    }

    Response response = std::forward<Mutation>(mutation)(pending_retry);
    if (prepared.enabled && (response.status >= 500 || response.status == 429)) {
      // A pre-commit transient failure is not a committed terminal receipt.
      // Retry the identical intent after storage/network recovery. Ambiguous
      // durability still fails closed at Dispatch, before mutation retries.
      return response;
    }
    if (prepared.enabled) {
      SessionMutationReceipt& receipt =
          session_mutation_receipts_[prepared.receipt_key];
      receipt.request_digest = std::move(prepared.request_digest);
      receipt.expires = Clock::now() + kSessionMutationReceiptLifetime;
      if (response.status == 202) {
        // A registration poll has committed this machine, but the frozen roster has not yet
        // converged. Keep only the intent marker so a retry can observe the later terminal result.
        receipt.terminal_response.reset();
      } else {
        receipt.terminal_response = response;
      }
    }
    return response;
  }

  static void RemoveSessionScopedDurable(DurableData& durable,
                                         std::string_view session_id,
                                         bool remove_invites) {
    if (remove_invites) {
      std::erase_if(durable.invites, [&](const auto& entry) {
        return entry.second.value("session_id", "") == session_id;
      });
      std::erase_if(durable.invite_accept_receipts, [&](const auto& entry) {
        return entry.second.response.contains("session") &&
               entry.second.response.at("session").is_object() &&
               entry.second.response.at("session").value("session_id", "") == session_id;
      });
    }
    std::erase_if(durable.stat_next_sequences, [&](const auto& entry) {
      const std::size_t separator = entry.first.find(':');
      return separator != std::string::npos &&
             entry.first.substr(separator + 1) == session_id;
    });
    std::erase_if(durable.stat_write_receipts, [&](const auto& entry) {
      const std::size_t first = entry.first.find(':');
      const std::size_t second = first == std::string::npos
                                     ? std::string::npos
                                     : entry.first.find(':', first + 1);
      return first != std::string::npos && second != std::string::npos &&
             entry.first.substr(first + 1, second - first - 1) == session_id;
    });
    std::vector<std::string> ranked_receipts;
    for (const auto& [result_id, canonical_request] : durable.ranked_result_requests) {
      try {
        const json parsed = json::parse(canonical_request);
        if (parsed.is_object() && parsed.value("session_id", "") == session_id) {
          ranked_receipts.push_back(result_id);
        }
      } catch (const json::exception&) {
        // Durable deserialization already validates canonical requests. Ignore only while
        // cleaning an in-memory legacy receipt that predates this validation.
      }
    }
    for (const std::string& result_id : ranked_receipts) {
      durable.ranked_result_requests.erase(result_id);
      durable.ranked_results.erase(result_id);
    }
  }

  static void RemoveMemberSessionStatState(DurableData& durable,
                                           std::string_view xuid,
                                           std::string_view session_id) {
    durable.stat_next_sequences.erase(StatSequenceOwnerKey(xuid, session_id));
    const std::string prefix = StatSequenceOwnerKey(xuid, session_id) + ":";
    std::erase_if(durable.stat_write_receipts, [&](const auto& entry) {
      return entry.first.starts_with(prefix);
    });
  }

  bool SessionLiveLocked(std::string_view session_id, Clock::time_point now) const {
    const auto lease = session_leases_.find(std::string(session_id));
    return lease == session_leases_.end() || lease->second > now;
  }

  void RemoveSessionResourcesLocked(const std::string& session_id) {
    if (const auto session = sessions_.find(session_id); session != sessions_.end()) {
      for (const auto& member : session->second.value("members", json::array())) {
        TraceServerMembership("server-removal", session_id, member);
      }
    }
    lobbies_.erase(session_id);
    ranked_started_.erase(session_id);
    qos_listeners_.erase(session_id);
    arbitration_snapshots_.erase(session_id);
    for (auto route = relay_routes_.begin(); route != relay_routes_.end();) {
      if (route->second.session_id != session_id) {
        ++route;
        continue;
      }
      relay_queues_.erase(route->first);
      route = relay_routes_.erase(route);
    }
    std::vector<std::string> voice_tokens;
    for (const auto& [token, route] : voice_routes_) {
      if (route.session_id == session_id) voice_tokens.push_back(token);
    }
    for (const std::string& token : voice_tokens) EraseVoiceRouteLocked(token);
    for (auto queue = voice_queues_.begin(); queue != voice_queues_.end();) {
      auto& packets = queue->second;
      std::size_t removed_bytes = 0;
      for (auto packet = packets.begin(); packet != packets.end();) {
        if (packet->session_id != session_id) {
          ++packet;
          continue;
        }
        removed_bytes += packet->encoded_bytes;
        packet = packets.erase(packet);
      }
      auto byte_count = voice_queue_encoded_bytes_.find(queue->first);
      if (byte_count != voice_queue_encoded_bytes_.end()) {
        byte_count->second -= std::min(byte_count->second, removed_bytes);
        if (byte_count->second == 0) voice_queue_encoded_bytes_.erase(byte_count);
      }
      if (packets.empty()) queue = voice_queues_.erase(queue);
      else ++queue;
    }
    for (auto events = realtime_events_.begin();
         events != realtime_events_.end();) {
      std::erase_if(events->second, [&](const RealtimeEvent& event) {
        return event.session_id == session_id;
      });
      if (events->second.empty()) {
        events = realtime_events_.erase(events);
      } else {
        ++events;
      }
    }
    for (auto& [ticket_id, ticket] : tickets_) {
      (void)ticket_id;
      if (ticket.matched_session_id != session_id) continue;
      ticket.state = "searching";
      ticket.matched_session_id.clear();
      ticket.updated_at = UtcIsoAfter();
    }
    std::erase_if(presence_, [&](const auto& entry) {
      return entry.second.session_id == session_id;
    });
    session_leases_.erase(session_id);
    sessions_.erase(session_id);
    relay_condition_.notify_all();
    voice_condition_.notify_all();
    realtime_event_condition_.notify_all();
  }

  bool SweepExpiredLocked(Clock::time_point now, std::int64_t unix_now) {
    std::vector<std::string> expired_sessions;
    for (const auto& [session_id, expires] : session_leases_) {
      if (expires <= now) expired_sessions.push_back(session_id);
    }
    std::unordered_set<std::string> expired_session_ids(expired_sessions.begin(),
                                                        expired_sessions.end());

    expired_session_ids.insert(pending_durable_session_cleanup_.begin(),
                               pending_durable_session_cleanup_.end());
    const bool has_expired_durable = !expired_session_ids.empty() ||
        std::ranges::any_of(refresh_sessions_, [unix_now](const auto& entry) {
          return entry.second.expires_at_unix <= unix_now;
        }) || std::ranges::any_of(invites_, [unix_now](const auto& entry) {
          return entry.second.value("expires_at_unix", std::int64_t{0}) <= unix_now;
        });
    if (has_expired_durable) {
    DurableData durable = CaptureDurableLocked();
    bool durable_changed = false;
    for (auto refresh = durable.refresh_sessions.begin();
         refresh != durable.refresh_sessions.end();) {
      if (refresh->second.expires_at_unix <= unix_now) {
        refresh = durable.refresh_sessions.erase(refresh);
        durable_changed = true;
      } else {
        ++refresh;
      }
    }
    for (auto invite = durable.invites.begin(); invite != durable.invites.end();) {
      if (invite->second.value("expires_at_unix", std::int64_t{0}) <= unix_now ||
          expired_session_ids.contains(invite->second.value("session_id", ""))) {
        invite = durable.invites.erase(invite);
        durable_changed = true;
      } else {
        ++invite;
      }
    }
    for (const std::string& session_id : expired_session_ids) {
      RemoveSessionScopedDurable(durable, session_id, true);
      durable_changed = true;
    }
    if (durable_changed && !SaveAndCommitLocked(std::move(durable))) {
      // Keep the last committed durable snapshot, but do not turn a disk error
      // into renewed membership, authentication, or transport leases.
      for (const std::string& session_id : expired_sessions) {
        pending_durable_session_cleanup_.insert(session_id);
      }
    } else {
      pending_durable_session_cleanup_.clear();
    }
    }

    std::erase_if(qos_probe_batches_, [now](const auto& entry) {
      return entry.second.expires <= now;
    });
    std::erase_if(challenges_, [now](const auto& entry) {
      return entry.second.expires <= now;
    });
    std::erase_if(access_tokens_, [now](const auto& entry) {
      return entry.second.expires <= now;
    });
    std::erase_if(presence_, [now](const auto& entry) {
      return entry.second.expires <= now;
    });
    std::erase_if(tickets_, [now](const auto& entry) {
      return entry.second.expires <= now;
    });
    std::erase_if(session_mutation_receipts_, [now](const auto& entry) {
      return entry.second.expires <= now;
    });

    for (const std::string& session_id : expired_sessions) {
      RemoveSessionResourcesLocked(session_id);
    }
    if (!expired_sessions.empty()) {
      for (auto& [ticket_id, ticket] : tickets_) {
        (void)ticket_id;
        ResolveMatchmakingTicket(ticket);
      }
    }

    bool relay_removed = false;
    for (auto route = relay_routes_.begin(); route != relay_routes_.end();) {
      if (route->second.expires > now) {
        ++route;
        continue;
      }
      relay_queues_.erase(route->first);
      route = relay_routes_.erase(route);
      relay_removed = true;
    }
    std::vector<std::string> expired_voice_routes;
    for (const auto& [token, route] : voice_routes_) {
      if (route.expires <= now) expired_voice_routes.push_back(token);
    }
    for (const std::string& token : expired_voice_routes) EraseVoiceRouteLocked(token);
    for (const auto& [token, route] : voice_routes_) {
      (void)route;
      PruneVoiceQueueLocked(token, now);
    }
    if (relay_removed) relay_condition_.notify_all();
    return true;
  }

  void SweeperMain() {
    std::unique_lock lock(mutex_);
    while (!sweeper_stopping_) {
      if (sweeper_condition_.wait_for(lock, kSweepInterval,
                                      [&] { return sweeper_stopping_; })) {
        break;
      }
      (void)SweepExpiredLocked(Clock::now(), UnixSecondsAfter());
      ++activity_generation_;
    }
  }

  DurableData CaptureDurableLocked() const {
    return {.devices = devices_,
            .refresh_sessions = refresh_sessions_,
            .player_names = player_names_,
            .relationships = relationships_,
            .invites = invites_,
            .stats = stats_,
            .progression = progression_,
            .mode_stats = mode_stats_,
            .ranked_results = ranked_results_,
            .ranked_result_requests = ranked_result_requests_,
            .profiles = profiles_,
            .stat_write_receipts = stat_write_receipts_,
            .invite_accept_receipts = invite_accept_receipts_,
            .stat_next_sequences = stat_next_sequences_,
            .title_profile_settings = title_profile_settings_,
            .prog_ach_records = prog_ach_records_,
            .achievements = achievements_,
            .entitlements = entitlements_};
  }

  void CommitDurableLocked(DurableData data) {
    devices_ = std::move(data.devices);
    refresh_sessions_ = std::move(data.refresh_sessions);
    player_names_ = std::move(data.player_names);
    relationships_ = std::move(data.relationships);
    invites_ = std::move(data.invites);
    stats_ = std::move(data.stats);
    progression_ = std::move(data.progression);
    mode_stats_ = std::move(data.mode_stats);
    ranked_results_ = std::move(data.ranked_results);
    ranked_result_requests_ = std::move(data.ranked_result_requests);
    profiles_ = std::move(data.profiles);
    stat_write_receipts_ = std::move(data.stat_write_receipts);
    invite_accept_receipts_ = std::move(data.invite_accept_receipts);
    stat_next_sequences_ = std::move(data.stat_next_sequences);
    title_profile_settings_ = std::move(data.title_profile_settings);
    prog_ach_records_ = std::move(data.prog_ach_records);
    achievements_ = std::move(data.achievements);
    entitlements_ = std::move(data.entitlements);
  }

  bool SaveAndCommitLocked(DurableData data) {
    if (persistent_state_ && !persistent_state_->Save(SerializeDurable(data))) return false;
    CommitDurableLocked(std::move(data));
    return true;
  }

  static Response StorageFailure() {
    return Error(503, "storage_unavailable", "durable state could not be committed");
  }

  Response ChallengeDevice(const Request& request) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    const std::string device_id = body->value("device_id", "");
    const std::string xuid = body->value("xuid", "");
    const std::string public_key = body->value("public_key", "");
    const auto decoded_key = DecodeBase64Url(public_key);
    if (!ValidIdentifier(device_id) || !IsHex(xuid, 16) || xuid == "0x0000000000000000" || !decoded_key ||
        decoded_key->size() != kSha256Bytes) {
      return Error(400, "invalid_device", "device identity is malformed");
    }
    const std::string id = RandomToken("challenge_");
    const std::string text = RandomToken("libserver_challenge_");
    if (id.empty() || text.empty()) return Error(500, "entropy_failure", "random generation failed");
    std::lock_guard lock(mutex_);
    const auto existing = devices_.find(device_id);
    if (existing != devices_.end() &&
        (existing->second.xuid != xuid || existing->second.public_key != public_key)) {
      return Error(409, "device_binding_conflict", "device is bound to another identity or key");
    }
    for (const auto& [bound_device, binding] : devices_) {
      if (binding.xuid == xuid && bound_device != device_id) {
        return Error(409, "xuid_binding_conflict", "xuid is already owned by another device");
      }
    }
    challenges_[id] = {.device_id = device_id, .xuid = xuid, .public_key = public_key,
                       .text = text, .expires = Clock::now() + kChallengeLifetime};
    return {.status = 201, .body = {{"challenge_id", id}, {"challenge", text}}};
  }

  Response EnrollDevice(const Request& request) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    const std::string challenge_id = body->value("challenge_id", "");
    Challenge challenge;
    {
      std::lock_guard lock(mutex_);
      const auto found = challenges_.find(challenge_id);
      if (found == challenges_.end() || Clock::now() >= found->second.expires) {
        return Error(401, "challenge_expired", "device challenge is missing or expired");
      }
      challenge = found->second;
      challenges_.erase(found);
    }
    if (body->value("device_id", "") != challenge.device_id ||
        body->value("xuid", "") != challenge.xuid ||
        body->value("public_key", "") != challenge.public_key ||
        !VerifyEd25519(challenge.public_key, challenge.text, body->value("signature", ""))) {
      return Error(401, "invalid_signature", "device challenge signature is invalid");
    }
    Identity identity{.device_id = challenge.device_id,
                      .xuid = challenge.xuid,
                      .machine_id = body->value("machine_id", "0x0000000000000000"),
                      .player_name = body->value("player_name", "Player")};
    if (!IsHex(identity.machine_id, 16) || identity.player_name.empty() ||
        identity.player_name.size() > 128) {
      return Error(400, "invalid_identity", "machine id or player name is invalid");
    }
    return IssueTokens(std::move(identity), 201, challenge.public_key, {});
  }

  Response RefreshDevice(const Request& request) {
    const auto body = ParseBody(request);
    if (!body) return Error(400, "invalid_json", "invalid JSON body");
    const std::string refresh = body->value("refresh_token", "");
    const auto token_hash = Sha256Hex(refresh);
    if (!token_hash) return Error(500, "hash_failure", "refresh token could not be hashed");
    return IssueTokens({}, 200, {}, *token_hash);
  }

  Response IssueTokens(Identity identity, int status, const std::string& enrollment_public_key,
                       const std::string& rotated_refresh_hash) {
    identity.xuid = libserver::CanonicalIdentity(std::move(identity.xuid));
    identity.machine_id = libserver::CanonicalIdentity(std::move(identity.machine_id));
    if (rotated_refresh_hash.empty()) {
      const auto key = DecodeBase64Url(enrollment_public_key);
      if (!ValidIdentifier(identity.device_id) || !IsHex(identity.xuid, 16) ||
          identity.xuid == "0x0000000000000000" ||
          !IsHex(identity.machine_id, 16) || identity.player_name.empty() ||
          identity.player_name.size() > 128 || !key || key->size() != kSha256Bytes) {
        return Error(400, "invalid_identity", "enrollment identity is invalid");
      }
    }
    const std::string access = RandomToken("access_");
    const std::string refresh = RandomToken("refresh_");
    if (access.empty() || refresh.empty()) return Error(500, "entropy_failure", "random generation failed");
    const auto refresh_hash = Sha256Hex(refresh);
    if (!refresh_hash) return Error(500, "hash_failure", "refresh token could not be hashed");
    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    if (!rotated_refresh_hash.empty()) {
      const auto existing = staged.refresh_sessions.find(rotated_refresh_hash);
      if (existing == staged.refresh_sessions.end() ||
          existing->second.expires_at_unix <= UnixSecondsAfter()) {
        return Error(401, "invalid_refresh", "refresh token is invalid or expired");
      }
      identity = existing->second.identity;
      staged.refresh_sessions.erase(existing);
    } else {
      const auto existing = staged.devices.find(identity.device_id);
      if (existing != staged.devices.end() &&
          (existing->second.xuid != identity.xuid ||
           existing->second.public_key != enrollment_public_key)) {
        return Error(409, "device_binding_conflict", "device is bound to another identity or key");
      }
      for (const auto& [device_id, binding] : staged.devices) {
        if (binding.xuid == identity.xuid && device_id != identity.device_id) {
          return Error(409, "xuid_binding_conflict", "xuid is already owned by another device");
        }
      }
      staged.devices[identity.device_id] = {identity.xuid, enrollment_public_key};
    }
    staged.player_names[identity.xuid] = identity.player_name;
    staged.refresh_sessions[*refresh_hash] = {
        identity, UnixSecondsAfter(kRefreshTokenLifetime)};
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    access_tokens_[access] = {identity, Clock::now() + kAccessTokenLifetime};
    return {.status = status,
            .body = {{"access_token", access}, {"refresh_token", refresh},
                     {"expires_in", kAccessTokenLifetime.count()},
                     {"enrollment_mode", "local_first_binding"}}};
  }

  std::optional<Identity> Authenticate(const Request& request) {
    const auto found = request.headers.find("authorization");
    if (found == request.headers.end() || !found->second.starts_with("Bearer ")) return std::nullopt;
    std::lock_guard lock(mutex_);
    const auto token = access_tokens_.find(found->second.substr(7));
    if (token == access_tokens_.end()) return std::nullopt;
    if (Clock::now() >= token->second.expires) {
      access_tokens_.erase(token);
      return std::nullopt;
    }
    return token->second.identity;
  }

  static bool ValidSession(const json& session) {
    try {
      if (!session.is_object() || !IsHex(session.value("session_id", ""), 16) ||
          session.value("session_id", "") == "0x0000000000000000" ||
          !IsHex(session.value("host_xuid", ""), 16) ||
          session.value("host_xuid", "") == "0x0000000000000000" ||
          !IsHex(session.value("title_id", ""), 8) ||
          !IsHex(session.value("media_id", ""), 8) ||
          !IsHex(session.value("title_version", ""), 8) ||
          !session.contains("protocol_version") ||
          !NonNegativeInteger(session.at("protocol_version")) ||
          !session.contains("public_slots") || !session.contains("private_slots") ||
          !session.contains("ranked") || !session.at("ranked").is_boolean() ||
          !session.contains("members") || !session.at("members").is_array() ||
          session.at("members").empty() || session.at("members").size() > kMaximumSessionMembers) {
        return false;
      }
      const auto protocol_version = NonNegativeInteger(session.at("protocol_version"));
      const auto public_slots = NonNegativeInteger(session.at("public_slots"));
      const auto private_slots = NonNegativeInteger(session.at("private_slots"));
      const auto procedure_index =
          session.contains("procedure_index")
              ? SessionUint32(session.at("procedure_index"))
              : std::optional<std::uint32_t>{0};
      const auto flags = session.contains("flags")
                             ? SessionUint32(session.at("flags"), true)
                             : std::optional<std::uint32_t>{0};
      const auto lifecycle_state =
          session.contains("lifecycle_state")
              ? SessionUint32(session.at("lifecycle_state"))
              : std::optional<std::uint32_t>{0};
      if (!protocol_version || *protocol_version < 1 ||
          *protocol_version > std::numeric_limits<std::uint32_t>::max() ||
          !public_slots || !private_slots || !procedure_index || !flags || !lifecycle_state ||
          *lifecycle_state > kMaximumSessionLifecycleState ||
          *public_slots > static_cast<std::int64_t>(kMaximumSessionMembers) ||
          *private_slots > static_cast<std::int64_t>(kMaximumSessionMembers) - *public_slots) {
        return false;
      }
      const std::string state = session.value("state", "lobby");
      const std::string visibility = session.value("visibility", "public");
      const std::string mode = session.value("mode", "gta4");
      const std::string episode = session.value("episode", "all");
      const std::string region = session.value("region", "global");
      const bool ranked_from_flags = (*flags & kSessionFlagUsesArbitration) != 0;
      const bool join_in_progress_from_flags =
          (*flags & kSessionFlagJoinInProgressDisabled) == 0;
      std::string_view visibility_from_flags = visibility;
      if (*flags != 0) {
        if ((*flags & kSessionFlagUsesMatchmaking) != 0) {
          visibility_from_flags = "public";
        } else if ((*flags & kSessionFlagUsesPresence) != 0 &&
                   (*flags & kSessionFlagJoinViaPresenceDisabled) == 0) {
          visibility_from_flags = "friends";
        } else {
          visibility_from_flags = "private";
        }
      }
      const bool lifecycle_matches_state =
          (state == "lobby" && *lifecycle_state == 0) ||
          (state == "open" && *lifecycle_state == 1) ||
          (state == "in_game" && *lifecycle_state == 2) ||
          (state == "closed" &&
           (*lifecycle_state == 3 || *lifecycle_state == kMaximumSessionLifecycleState));
      if ((state != "lobby" && state != "open" && state != "in_game" && state != "closed") ||
          !lifecycle_matches_state ||
          (visibility != "public" && visibility != "friends" && visibility != "private") ||
          visibility != visibility_from_flags ||
          session.at("ranked").get<bool>() != ranked_from_flags ||
          !ValidIdentifier(mode, 64) || !ValidEpisodeOrAll(episode) ||
          (!region.empty() && !ValidIdentifier(region, 32)) ||
          (session.contains("join_in_progress") &&
           (!session.at("join_in_progress").is_boolean() ||
            session.at("join_in_progress").get<bool>() != join_in_progress_from_flags)) ||
          (session.contains("previous_session_id") &&
           (!session.at("previous_session_id").is_string() ||
            !IsHex(Lower(session.at("previous_session_id").get<std::string>()), 16))) ||
          (session.contains("nonce") &&
           (!session.at("nonce").is_string() ||
            !IsHex(Lower(session.at("nonce").get<std::string>()), 16))) ||
          (session.contains("exchange_key") &&
           (!session.at("exchange_key").is_string() ||
            !IsHex(Lower(session.at("exchange_key").get<std::string>()), 32))) ||
          (session.contains("host_machine_id") &&
           (!session.at("host_machine_id").is_string() ||
            !IsHex(Lower(session.at("host_machine_id").get<std::string>()), 16))) ||
          (session.contains("host_port") &&
           (!SessionUint32(session.at("host_port")) ||
            *SessionUint32(session.at("host_port")) >
                std::numeric_limits<std::uint16_t>::max())) ||
          !ValidFilterObject(session.value("contexts", json::object()), false) ||
          !ValidFilterObject(session.value("properties", json::object()), true)) {
        return false;
      }
      std::unordered_set<std::string> xuids;
      std::size_t public_members = 0;
      std::size_t private_members = 0;
      bool host_present = false;
      for (const auto& member : session.at("members")) {
        if (!member.is_object() || !member.contains("xuid") || !member.at("xuid").is_string() ||
            !IsHex(member.at("xuid").get<std::string>(), 16) ||
            member.at("xuid").get<std::string>() == "0x0000000000000000" ||
            !xuids.insert(member.at("xuid").get<std::string>()).second ||
            (member.contains("private") && !member.at("private").is_boolean()) ||
            (member.contains("account_id") &&
             (!member.at("account_id").is_string() ||
              member.at("account_id").get<std::string>() !=
                  member.at("xuid").get<std::string>())) ||
            (member.contains("peer_id") && !member.at("peer_id").is_string()) ||
            (member.contains("virtual_ipv4") && !member.at("virtual_ipv4").is_string()) ||
            (member.contains("route") && !member.at("route").is_object())) {
          return false;
        }
        const std::string xuid = member.at("xuid").get<std::string>();
        host_present = host_present || xuid == session.at("host_xuid").get<std::string>();
        if (member.value("private", false)) ++private_members;
        else ++public_members;
        if (member.contains("machine_id") &&
            (!member.at("machine_id").is_string() ||
             !IsHex(member.at("machine_id").get<std::string>(), 16))) return false;
        if (member.contains("online_port")) {
          const auto port = NonNegativeInteger(member.at("online_port"));
          if (!port || *port > 65535) return false;
        }
        if (member.contains("virtual_ipv4")) {
          in_addr address{};
          if (inet_pton(AF_INET, member.at("virtual_ipv4").get<std::string>().c_str(),
                        &address) != 1) {
            return false;
          }
        }
        if (member.contains("route")) {
          const json& route = member.at("route");
          if ((route.contains("peer_id") &&
               (!NonNegativeInteger(route.at("peer_id")) ||
                *NonNegativeInteger(route.at("peer_id")) >=
                    static_cast<std::int64_t>(kMaximumSessionMembers))) ||
              (route.contains("port") &&
               (!NonNegativeInteger(route.at("port")) ||
                *NonNegativeInteger(route.at("port")) > 65535)) ||
              (route.contains("address") && !route.at("address").is_string()) ||
              (route.contains("connection_id") &&
               !route.at("connection_id").is_string())) {
            return false;
          }
          if (route.contains("address")) {
            in_addr address{};
            if (inet_pton(AF_INET, route.at("address").get<std::string>().c_str(),
                          &address) != 1) {
              return false;
            }
          }
        }
      }
      return host_present && public_members <= static_cast<std::size_t>(*public_slots) &&
             private_members <= static_cast<std::size_t>(*private_slots);
    } catch (const json::exception&) {
      return false;
    }
  }

  bool SessionAcceptsJoins(const json& session) const {
    // Registration state is also used by ordinary open sessions. Only an
    // actual atomic arbitration snapshot freezes membership and discovery.
    if (arbitration_snapshots_.contains(
            session.value("session_id", std::string{}))) {
      return false;
    }
    const std::string state = session.value("state", "");
    return state == "lobby" || state == "open" ||
           (state == "in_game" && session.value("join_in_progress", false));
  }

  static std::uint32_t DefaultLifecycleState(std::string_view state) {
    if (state == "open") return 1;
    if (state == "in_game") return 2;
    if (state == "closed") return 3;
    return 0;
  }

  static std::string ServiceStateForLifecycle(std::uint32_t lifecycle_state) {
    if (lifecycle_state == 0) return "lobby";
    if (lifecycle_state == 1) return "open";
    if (lifecycle_state == 2) return "in_game";
    return "closed";
  }

  static bool NormalizeSessionLifecycle(json& session, bool state_supplied,
                                        bool lifecycle_supplied) {
    if (state_supplied && !session.at("state").is_string()) return false;
    const auto lifecycle = lifecycle_supplied
                               ? SessionUint32(session.at("lifecycle_state"))
                               : std::optional<std::uint32_t>{};
    if (lifecycle_supplied &&
        (!lifecycle || *lifecycle > kMaximumSessionLifecycleState)) {
      return false;
    }
    if (state_supplied && lifecycle_supplied) {
      const std::string state = session.at("state").get<std::string>();
      if (state == "open" && *lifecycle == 0) {
        session["state"] = "lobby";
        return true;
      }
      return (state == "lobby" && *lifecycle == 0) ||
             (state == "open" && *lifecycle == 1) ||
             (state == "in_game" && *lifecycle == 2) ||
             (state == "closed" && (*lifecycle == 3 || *lifecycle == 4));
    }
    if (lifecycle_supplied) {
      session["state"] = ServiceStateForLifecycle(*lifecycle);
      return true;
    }
    if (state_supplied) {
      const std::string state = session.at("state").get<std::string>();
      if (state != "lobby" && state != "open" && state != "in_game" && state != "closed") {
        return false;
      }
      session["lifecycle_state"] = DefaultLifecycleState(state);
      return true;
    }
    return true;
  }

  static bool CanonicalizeSessionFlags(json& session) {
    const auto flags = SessionUint32(session.value("flags", json(0)), true);
    if (!flags) return false;
    session["flags"] = *flags;
    session["ranked"] = (*flags & kSessionFlagUsesArbitration) != 0;
    session["join_in_progress"] =
        (*flags & kSessionFlagJoinInProgressDisabled) == 0;
    if (*flags != 0) {
      if ((*flags & kSessionFlagUsesMatchmaking) != 0) {
        session["visibility"] = "public";
      } else if ((*flags & kSessionFlagUsesPresence) != 0 &&
                 (*flags & kSessionFlagJoinViaPresenceDisabled) == 0) {
        session["visibility"] = "friends";
      } else {
        session["visibility"] = "private";
      }
    }
    return true;
  }

  static bool SessionMetadataMatches(const json& request, const json& session) {
    const auto matches_u32 = [&](std::string_view field, bool allow_hex_string = false) {
      const std::string key(field);
      if (!request.contains(key)) return true;
      if (!session.contains(key)) return false;
      const auto requested = SessionUint32(request.at(key), allow_hex_string);
      const auto current = SessionUint32(session.at(key), allow_hex_string);
      return requested && current && *requested == *current;
    };
    const auto matches_json = [&](std::string_view field) {
      const std::string key(field);
      return !request.contains(key) ||
             (session.contains(key) && request.at(key) == session.at(key));
    };
    const bool raw_flags_supplied = request.contains("flags");
    if (!raw_flags_supplied && request.contains("ranked") &&
        (!request.at("ranked").is_boolean() ||
         request.at("ranked").get<bool>() != session.value("ranked", false))) {
      return false;
    }
    if (!raw_flags_supplied && request.contains("join_in_progress") &&
        (!request.at("join_in_progress").is_boolean() ||
         request.at("join_in_progress").get<bool>() !=
             session.value("join_in_progress", false))) {
      return false;
    }
    if (!raw_flags_supplied && request.contains("visibility") &&
        (!request.at("visibility").is_string() ||
         request.at("visibility").get<std::string>() !=
             session.value("visibility", "public"))) {
      return false;
    }
    if (!matches_u32("procedure_index") || !matches_u32("flags", true) ||
        !matches_u32("lifecycle_state") || !matches_json("contexts") ||
        !matches_json("properties")) {
      return false;
    }
    if (request.contains("expected_revision")) {
      const auto expected = NonNegativeInteger(request.at("expected_revision"));
      if (!expected || *expected != session.value("revision", std::int64_t{0})) return false;
    }
    if (request.contains("expected_host_epoch")) {
      const auto expected = NonNegativeInteger(request.at("expected_host_epoch"));
      if (!expected || *expected != session.value("host_epoch", std::int64_t{1})) return false;
    }
    return true;
  }

  bool AcceptedFriendsLocked(std::string_view left, std::string_view right) const {
    const auto owner = relationships_.find(std::string(left));
    return owner != relationships_.end() && owner->second.contains(std::string(right)) &&
           owner->second.at(std::string(right)) == "accepted";
  }

  static bool RelationshipStateBlocks(std::string_view state) {
    return state == "blocked" || state == "blocked_by" ||
           state == "blocked_mutual";
  }

  bool RelationshipBlocksLocked(std::string_view left, std::string_view right) const {
    const auto owner = relationships_.find(std::string(left));
    if (owner != relationships_.end()) {
      const auto target = owner->second.find(std::string(right));
      if (target != owner->second.end() && RelationshipStateBlocks(target->second)) {
        return true;
      }
    }
    const auto reverse_owner = relationships_.find(std::string(right));
    if (reverse_owner == relationships_.end()) return false;
    const auto reverse = reverse_owner->second.find(std::string(left));
    return reverse != reverse_owner->second.end() &&
           RelationshipStateBlocks(reverse->second);
  }

  bool HasPendingInviteLocked(std::string_view session_id, std::string_view recipient) const {
    const std::int64_t now = UnixSecondsAfter();
    return std::any_of(invites_.begin(), invites_.end(), [&](const auto& item) {
      const json& invite = item.second;
      return invite.value("session_id", "") == session_id &&
             invite.value("recipient_xuid", "") == recipient &&
             invite.value("state", "") == "pending" &&
             invite.value("expires_at_unix", std::int64_t{0}) > now;
    });
  }

  bool CanViewSessionLocked(const json& session, const Identity& identity) const {
    if (SessionContainsXuid(session, identity.xuid)) return true;
    const std::string visibility = session.value("visibility", "public");
    if (visibility == "public") return true;
    if (visibility == "friends") {
      return AcceptedFriendsLocked(identity.xuid, session.value("host_xuid", ""));
    }
    return HasPendingInviteLocked(session.value("session_id", ""), identity.xuid);
  }

  bool HasDiscoverableCapacityLocked(const json& session, const Identity& identity) const {
    if (SessionContainsXuid(session, identity.xuid)) return true;
    std::size_t public_members = 0;
    std::size_t private_members = 0;
    for (const auto& member : session.value("members", json::array())) {
      if (member.value("private", false)) ++private_members;
      else ++public_members;
    }
    const bool public_open = public_members < static_cast<std::size_t>(session.value("public_slots", 0));
    const bool private_open = private_members < static_cast<std::size_t>(session.value("private_slots", 0));
    return public_open || (private_open &&
                           HasPendingInviteLocked(session.value("session_id", ""), identity.xuid));
  }

  static json VisibleSession(const json& session, const Identity& identity) {
    if (SessionContainsXuid(session, identity.xuid)) return session;
    json visible = session;
    std::size_t public_members = 0;
    std::size_t private_members = 0;
    for (const auto& member : session.value("members", json::array())) {
      if (member.value("private", false)) {
        ++private_members;
      } else {
        ++public_members;
      }
    }
    const auto public_slots =
        static_cast<std::size_t>(session.value("public_slots", 0));
    const auto private_slots =
        static_cast<std::size_t>(session.value("private_slots", 0));
    visible["member_count"] = public_members + private_members;
    visible["public_member_count"] = public_members;
    visible["private_member_count"] = private_members;
    visible["roster_complete"] = false;
    visible["open_public_slots"] =
        public_members <= public_slots ? public_slots - public_members : 0;
    visible["open_private_slots"] =
        private_members <= private_slots ? private_slots - private_members : 0;
    visible["members"] = json::array();
    visible.erase("platform_stat_reports");
    // XSessionSearch results are join descriptors. GTA copies the nonce and
    // XSESSION_INFO into its own state before XSessionJoinLocal, then reuses
    // that nonce for ranked arbitration. Keep individual roster entries
    // private, but retain the host transport/security descriptor and exact
    // public/private occupancy needed to build the retail search result.
    return visible;
  }

  static std::string VirtualAddress(std::size_t slot) {
    return "192.168.100." + std::to_string(slot + 1);
  }

  static void TraceServerMembership(std::string_view stage, std::string_view session_id,
                                    const json& member) {
    if (!member.is_object() || !member.contains("route") ||
        !member.at("route").is_object()) {
      return;
    }
    const std::size_t peer_id =
        member.at("route").value("peer_id", kMaximumSessionMembers);
    if (peer_id >= kMaximumSessionMembers) return;
    std::ostringstream trace;
    trace << "libserver-mp64-validation: stage=" << stage
          << " session=" << session_id << " xuid=" << member.value("xuid", "")
          << " peer=" << peer_id << '\n';
    std::lock_guard lock(SessionTraceOutputMutex());
    std::cout << trace.str();
  }

  static void NormalizeMembers(json& session) {
    auto& members = session["members"];
    std::array<bool, kMaximumSessionMembers> used_addresses{};
    std::array<bool, kMaximumSessionMembers> used_peer_ids{};
    std::vector<std::optional<std::size_t>> address_slots(members.size());
    std::vector<std::optional<std::size_t>> peer_slots(members.size());

    for (std::size_t member_index = 0; member_index < members.size(); ++member_index) {
      const auto& member = members[member_index];
      const std::string existing_address = member.value("virtual_ipv4", "");
      for (std::size_t slot = 0; slot < kMaximumSessionMembers; ++slot) {
        if (!used_addresses[slot] && existing_address == VirtualAddress(slot)) {
          address_slots[member_index] = slot;
          used_addresses[slot] = true;
          break;
        }
      }
      if (member.contains("route") && member["route"].is_object()) {
        const auto existing_peer_id = member["route"].value("peer_id", kMaximumSessionMembers);
        if (existing_peer_id < kMaximumSessionMembers && !used_peer_ids[existing_peer_id]) {
          peer_slots[member_index] = existing_peer_id;
          used_peer_ids[existing_peer_id] = true;
        }
      }
    }

    for (std::size_t member_index = 0; member_index < members.size(); ++member_index) {
      if (!address_slots[member_index]) {
        const auto free = std::find(used_addresses.begin(), used_addresses.end(), false);
        if (free != used_addresses.end()) {
          address_slots[member_index] = static_cast<std::size_t>(free - used_addresses.begin());
          *free = true;
        }
      }
      if (!peer_slots[member_index]) {
        const auto free = std::find(used_peer_ids.begin(), used_peer_ids.end(), false);
        if (free != used_peer_ids.end()) {
          peer_slots[member_index] = static_cast<std::size_t>(free - used_peer_ids.begin());
          *free = true;
        }
      }
      auto& member = members[member_index];
      const std::size_t address_slot = address_slots[member_index].value();
      const std::size_t peer_slot = peer_slots[member_index].value();
      const std::string address = VirtualAddress(address_slot);
      member["virtual_ipv4"] = address;
      member["account_id"] = member.value("account_id", member.value("xuid", ""));
      member["online_port"] = member.value("online_port", 0);
      std::string connection_id = member.value("peer_id", "");
      if (connection_id.empty()) connection_id = member.value("xuid", "");
      member["peer_id"] = connection_id;
      member["route"] = {{"peer_id", peer_slot}, {"address", address},
                         {"port", member["online_port"]}, {"connection_id", connection_id}};
    }
    const std::string host_xuid = session.value("host_xuid", "");
    for (const auto& member : members) {
      if (member.value("xuid", "") != host_xuid) continue;
      session["host_ipv4"] = member["virtual_ipv4"];
      session["host_port"] = member["online_port"];
      session["host_peer_id"] = member["peer_id"];
      break;
    }
  }

  Response CreateSession(const Request& request, const Identity& identity) {
    return RunSessionMutation(request, identity, [&](bool /*pending_retry*/) -> Response {
    auto body = ParseBody(request);
    bool normalized_policy = false;
    if (body && body->is_object()) {
      const bool state_supplied = body->contains("state");
      const bool lifecycle_supplied = body->contains("lifecycle_state");
      if (!state_supplied && !lifecycle_supplied) {
        (*body)["state"] = "lobby";
        (*body)["lifecycle_state"] = 0;
      }
      (*body)["visibility"] = body->value("visibility", "public");
      (*body)["mode"] = body->value("mode", "gta4");
      (*body)["episode"] = body->value("episode", "all");
      (*body)["region"] = body->value("region", "global");
      (*body)["contexts"] = body->value("contexts", json::object());
      (*body)["properties"] = body->value("properties", json::object());
      (*body)["procedure_index"] = body->value("procedure_index", 0);
      (*body)["join_in_progress"] = body->value("join_in_progress", false);
      (*body)["flags"] = body->value("flags", 0);
      normalized_policy =
          NormalizeSessionLifecycle(*body, state_supplied, lifecycle_supplied) &&
          CanonicalizeSessionFlags(*body);
      const auto public_slots = body->contains("public_slots")
                                    ? NonNegativeInteger(body->at("public_slots"))
                                    : std::optional<std::int64_t>{};
      const auto private_slots = body->contains("private_slots")
                                     ? NonNegativeInteger(body->at("private_slots"))
                                     : std::optional<std::int64_t>{};
      if (public_slots && private_slots && *public_slots == 0 && *private_slots > 0 &&
          body->contains("members") && body->at("members").is_array() &&
          body->at("members").size() == 1 &&
          body->at("members").at(0).value("xuid", "") == identity.xuid) {
        // The active client synthesizes an unclassified public host before GTA issues its
        // private-slot join. Canonicalize that provisional host into the only available class.
        (*body)["members"][0]["private"] = true;
      }
    }
    if (!body || !normalized_policy || !ValidSession(*body) ||
        body->value("host_xuid", "") != identity.xuid) {
      return Error(400, "invalid_session", "session is malformed or host identity does not match");
    }
    NormalizeMembers(*body);
    (*body)["revision"] = 1;
    (*body)["host_epoch"] = 1;
    const std::string id = (*body)["session_id"];
    if (sessions_.contains(id)) return Error(409, "session_exists", "session already exists");
    sessions_[id] = *body;
    session_leases_[id] = Clock::now() + kSessionLease;
    for (const auto& member : body->at("members")) {
      TraceServerMembership("server-membership", id, member);
    }
    for (auto& [ticket_id, ticket] : tickets_) {
      (void)ticket_id;
      ResolveMatchmakingTicket(ticket);
    }
    return {.status = 201, .body = *body};
    });
  }

  Response SearchSessions(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("maximum_results") ||
        !body->contains("title_id") || !body->at("title_id").is_string() ||
        !body->contains("media_id") || !body->at("media_id").is_string() ||
        !body->contains("title_version") || !body->at("title_version").is_string() ||
        !body->contains("protocol_version")) {
      return Error(400, "invalid_search", "session search is malformed");
    }
    const auto requested_maximum = NonNegativeInteger(body->at("maximum_results"));
    const auto protocol_version = NonNegativeInteger(body->at("protocol_version"));
    const auto procedure_index =
        body->contains("procedure_index")
            ? SessionUint32(body->at("procedure_index"))
            : std::optional<std::uint32_t>{0};
    // This selects the title's search schema and is echoed for response
    // correlation. XSessionCreate does not associate it with a session.
    const json contexts = body->value("contexts", json::object());
    const json properties = body->value("properties", json::object());
    if (!requested_maximum || !protocol_version || !procedure_index ||
        !IsHex(body->at("title_id").get<std::string>(), 8) ||
        !IsHex(body->at("media_id").get<std::string>(), 8) ||
        !IsHex(body->at("title_version").get<std::string>(), 8) ||
        (body->contains("mode") &&
         (!body->at("mode").is_string() ||
          !ValidIdentifier(body->at("mode").get<std::string>(), 64))) ||
        (body->contains("ranked") && !body->at("ranked").is_boolean()) ||
        !ValidFilterObject(contexts, false) || !ValidFilterObject(properties, true)) {
      return Error(400, "invalid_search", "session search filters are malformed");
    }
    const std::size_t maximum = std::min<std::size_t>(
        static_cast<std::size_t>(*requested_maximum), 100);
    json result = json::array();
    std::lock_guard lock(mutex_);
    const auto now = Clock::now();
    for (const auto& [id, session] : sessions_) {
      (void)id;
      if (result.size() >= maximum) break;
      if (!SessionLiveLocked(id, now)) continue;
      if (!SessionAcceptsJoins(session) || !CanViewSessionLocked(session, identity) ||
          !HasDiscoverableCapacityLocked(session, identity)) continue;
      if (session.value("title_id", "") != body->value("title_id", "") ||
          session.value("media_id", "") != body->value("media_id", "") ||
          session.value("title_version", "") != body->value("title_version", "") ||
          session.value("protocol_version", 0) != body->value("protocol_version", 0) ||
          (body->contains("mode") &&
           session.value("mode", "") != body->at("mode").get<std::string>()) ||
          (body->contains("ranked") &&
           session.value("ranked", false) != body->at("ranked").get<bool>())) continue;
      bool matches = true;
      for (const auto& [key, value] : contexts.items()) {
        if (!session["contexts"].contains(key) || session["contexts"][key] != value) matches = false;
      }
      for (const auto& [key, value] : properties.items()) {
        if (!session["properties"].contains(key) || session["properties"][key] != value) matches = false;
      }
      if (matches) result.push_back(VisibleSession(session, identity));
    }
    return {.body = {{"procedure_index", *procedure_index}, {"sessions", std::move(result)}}};
  }

  Response GetSession(const std::string& id, const Identity& identity) {
    if (!IsHex(id, 16)) return Error(400, "invalid_session_id", "session id is malformed");
    std::lock_guard lock(mutex_);
    const auto now = Clock::now();
    auto found = sessions_.find(id);
    if (found != sessions_.end() && SessionLiveLocked(id, now) &&
        CanViewSessionLocked(found->second, identity)) {
      return {.body = VisibleSession(found->second, identity)};
    }
    const json* replacement = nullptr;
    std::string replacement_id;
    for (const auto& [candidate_id, candidate] : sessions_) {
      if (candidate.value("previous_session_id", "") != id ||
          !SessionLiveLocked(candidate_id, now) ||
          !CanViewSessionLocked(candidate, identity)) {
        continue;
      }
      if (!replacement || candidate_id < replacement_id) {
        replacement = &candidate;
        replacement_id = candidate_id;
      }
    }
    if (replacement) return {.body = VisibleSession(*replacement, identity)};
    return Error(404, "session_not_found", "session does not exist");
  }

  Response SessionOperation(const Request& request, const Identity& identity) {
    std::string suffix = SegmentAfter(request.path, "/api/v2/sessions/");
    const auto slash = suffix.find('/');
    const std::string id = suffix.substr(0, slash);
    const std::string action = slash == std::string::npos ? std::string{} : suffix.substr(slash + 1);
    if (!IsHex(id, 16)) return Error(400, "invalid_session_id", "session id is malformed");
    if (request.method == "GET" && action.empty()) return GetSession(id, identity);
    return RunSessionMutation(request, identity, [&](bool pending_retry) -> Response {
    auto found = sessions_.find(id);
    if (found == sessions_.end() || !SessionLiveLocked(id, Clock::now())) {
      return Error(404, "session_not_found", "session does not exist");
    }
    json& session = found->second;
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    std::optional<std::int64_t> expected;
    if (body->contains("expected_revision")) {
      expected = NonNegativeInteger(body->at("expected_revision"));
    } else if (const auto if_match = request.headers.find("if-match");
               if_match != request.headers.end()) {
      std::int64_t parsed = 0;
      const auto result = std::from_chars(if_match->second.data(),
                                          if_match->second.data() + if_match->second.size(), parsed);
      if (result.ec == std::errc{} && result.ptr == if_match->second.data() + if_match->second.size() &&
          parsed >= 0) expected = parsed;
    } else {
      return Error(400, "expected_revision_required", "expected session revision is required");
    }
    if (!expected) return Error(400, "invalid_revision", "expected session revision is invalid");
    if (request.method == "POST" && action == "join" && body->contains("member") &&
        body->at("member").is_object()) {
      const json& requested_member = body->at("member");
      if (requested_member.contains("xuid") && requested_member.at("xuid").is_string() &&
          requested_member.at("xuid").get<std::string>() == identity.xuid &&
          (!body->contains("private") || body->at("private").is_boolean()) &&
          (!requested_member.contains("private") ||
           requested_member.at("private").is_boolean())) {
        const auto existing =
            std::find_if(session.at("members").begin(), session.at("members").end(),
                         [&](const json& member) {
                           return member.value("xuid", "") == identity.xuid;
                         });
        if (existing != session.at("members").end()) {
          const bool existing_private = existing->value("private", false);
          const bool requested_private = body->value(
              "private", requested_member.value("private", existing_private));
          std::optional<std::int64_t> requested_port;
          bool requested_port_supplied = false;
          if (requested_member.contains("online_port")) {
            requested_port_supplied = true;
            requested_port = NonNegativeInteger(
                requested_member.at("online_port"));
          } else if (requested_member.contains("route") &&
                     requested_member.at("route").is_object() &&
                     requested_member.at("route").contains("port")) {
            requested_port_supplied = true;
            requested_port = NonNegativeInteger(
                requested_member.at("route").at("port"));
          }
          const bool port_current =
              !requested_port_supplied ||
              (requested_port &&
               (*requested_port == 0 ||
                (*requested_port <= std::numeric_limits<std::uint16_t>::max() &&
                 existing->value("online_port", 0) == *requested_port)));
          if (requested_private == existing_private &&
              (!requested_member.contains("private") ||
               requested_member.at("private").get<bool>() == requested_private) &&
              existing->value("machine_id", "") == identity.machine_id &&
              port_current) {
            return {.body = session};
          }
        }
      }
    }
    if (*expected != session.value("revision", 0ll) &&
        !(pending_retry && request.method == "POST" && action == "arbitration")) {
      return Error(412, "revision_mismatch", "session revision changed");
    }
    if (request.method == "DELETE" && action.empty()) {
      if (session.value("host_xuid", "") != identity.xuid) return Error(403, "not_host", "only host can delete session");
      DurableData staged = CaptureDurableLocked();
      RemoveSessionScopedDurable(staged, id, true);
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      for (const auto& member : session.value("members", json::array())) {
        TraceServerMembership("server-removal", id, member);
        RemoveMemberTransportState(id, member.value("xuid", ""));
      }
      lobbies_.erase(id);
      ranked_started_.erase(id);
      qos_listeners_.erase(id);
      arbitration_snapshots_.erase(id);
      for (auto& [ticket_id, ticket] : tickets_) {
        (void)ticket_id;
        if (ticket.matched_session_id != id) continue;
        ticket.state = "searching";
        ticket.matched_session_id.clear();
        ticket.updated_at = UtcIsoAfter();
      }
      sessions_.erase(found);
      session_leases_.erase(id);
      for (auto& [ticket_id, ticket] : tickets_) {
        (void)ticket_id;
        ResolveMatchmakingTicket(ticket);
      }
      return {.status = 204};
    }
    if (request.method == "PATCH" && action.empty()) {
      if (session.value("host_xuid", "") != identity.xuid) return Error(403, "not_host", "only host can modify session");
      if (const auto arbitration = arbitration_snapshots_.find(id);
          arbitration != arbitration_snapshots_.end()) {
        const bool complete =
            arbitration->second.registered_machine_ids.size() ==
            arbitration->second.expected_machine_ids.size();
        if (!complete) {
          return Error(409, "arbitration_pending",
                       "session cannot change while arbitration is pending");
        }
        if (body->contains("members")) {
          return Error(409, "arbitration_roster_frozen",
                       "ranked arbitration roster is immutable");
        }
      }
      static const std::unordered_set<std::string> kMutableFields = {
          "state", "visibility", "mode", "episode", "region", "ranked", "public_slots",
          "private_slots", "contexts", "properties", "previous_session_id", "nonce", "flags",
          "lifecycle_state", "procedure_index", "join_in_progress", "host_machine_id",
          "host_ipv4", "host_port",
          "host_ethernet_address", "host_peer_id", "members"};
      json replacement = session;
      for (const auto& [key, value] : body->items()) {
        if (key == "expected_revision") continue;
        if (!kMutableFields.contains(key)) {
          return Error(400, "immutable_session_field", "session identity fields cannot be modified");
        }
        replacement[key] = value;
      }
      if (!NormalizeSessionLifecycle(replacement, body->contains("state"),
                                     body->contains("lifecycle_state")) ||
          !CanonicalizeSessionFlags(replacement)) {
        return Error(400, "invalid_session", "session lifecycle or flags are malformed");
      }
      std::unordered_set<std::string> prior_members;
      std::unordered_set<std::string> replacement_members;
      std::vector<json> removed_members;
      if (body->contains("members")) {
        if (!replacement.at("members").is_array()) {
          return Error(400, "invalid_session", "session membership is malformed");
        }
        for (const auto& member : session.at("members")) {
          prior_members.insert(member.value("xuid", ""));
        }
        for (const auto& member : replacement.at("members")) {
          if (!member.is_object() || !member.contains("xuid") ||
              !member.at("xuid").is_string()) {
            return Error(400, "invalid_session", "session member is malformed");
          }
          replacement_members.insert(member.at("xuid").get<std::string>());
        }
      }
      if (!ValidSession(replacement)) {
        return Error(400, "invalid_session", "modified session would violate session invariants");
      }
      const auto next_lifecycle = replacement.value("lifecycle_state", std::uint32_t{0});
      const auto previous_lifecycle = session.value("lifecycle_state", std::uint32_t{0});
      const bool starting_ranked = replacement.value("ranked", false) && next_lifecycle == 2;
      if (starting_ranked && !ranked_started_.contains(id)) {
        const auto arb = arbitration_snapshots_.find(id);
        if (previous_lifecycle > 1 || arb == arbitration_snapshots_.end() ||
            arb->second.failed || arb->second.registered_machine_ids != arb->second.expected_machine_ids) {
          return Error(409, "ranked_registration_required", "ranked start requires completed registration");
        }
      }
      if (ranked_started_.contains(id) &&
          (replacement.value("ranked", false) != session.value("ranked", false) || next_lifecycle < 2)) {
        return Error(409, "ranked_match_frozen", "a started ranked match cannot reopen or change ranking policy");
      }
      NormalizeMembers(replacement);
      replacement["revision"] = session.value("revision", 0ll) + 1;
      if (body->contains("members")) {
        for (const auto& member : session.at("members")) {
          if (!replacement_members.contains(member.value("xuid", ""))) {
            removed_members.push_back(member);
          }
        }
      }
      if (!removed_members.empty()) {
        DurableData staged = CaptureDurableLocked();
        for (const auto& member : removed_members) {
          RemoveMemberSessionStatState(staged, member.value("xuid", ""), id);
        }
        if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      }
      session = std::move(replacement);
      if (starting_ranked) ranked_started_.insert(id);
      if (body->contains("members")) {
        for (const auto& member : removed_members) {
          const std::string removed_xuid = member.value("xuid", "");
          TraceServerMembership("server-removal", id, member);
          if (auto lobby = lobbies_.find(id); lobby != lobbies_.end()) {
            RemoveMemberFromLobby(lobby->second, removed_xuid);
            ++lobby->second.revision;
          }
          RemoveMemberTransportState(id, removed_xuid);
        }
        if (auto lobby = lobbies_.find(id); lobby != lobbies_.end()) {
          for (const auto& member : session.at("members")) {
            const std::string member_xuid = member.value("xuid", "");
            if (prior_members.contains(member_xuid)) continue;
            lobby->second.ready[member_xuid] = false;
            lobby->second.spectators[member_xuid] = false;
            ++lobby->second.revision;
          }
        }
        for (const auto& member : session.at("members")) {
          if (!prior_members.contains(member.value("xuid", ""))) {
            TraceServerMembership("server-membership", id, member);
          }
        }
      }
      for (auto& [ticket_id, ticket] : tickets_) {
        (void)ticket_id;
        ResolveMatchmakingTicket(ticket);
      }
      return {.body = session};
    }
    if (request.method != "POST") return Error(404, "not_found", "session operation does not exist");
    if (action == "heartbeat") {
      if (session.value("host_xuid", "") != identity.xuid) {
        return Error(403, "not_host", "only the host can heartbeat the session");
      }
      session_leases_[id] = Clock::now() + kSessionLease;
      return {.body = session};
    }
    if (action == "arbitration") {
      const auto duration =
          body->contains("registration_duration_seconds")
              ? SessionUint32(body->at("registration_duration_seconds"))
              : std::optional<std::uint32_t>{};
      const auto flags = body->contains("flags")
                             ? SessionUint32(body->at("flags"), true)
                             : std::optional<std::uint32_t>{};
      const std::string nonce =
          body->contains("nonce") && body->at("nonce").is_string()
              ? Lower(body->at("nonce").get<std::string>())
              : std::string{};
      if (!duration || *duration != kGta4ArbitrationRegistrationSeconds ||
          !flags || *flags != kGta4ArbitrationFlags ||
          !IsHex(nonce, 16) || nonce != Lower(session.value("nonce", ""))) {
        return Error(400, "invalid_arbitration_request",
                     "arbitration tuple does not match GTA IV's session contract");
      }
      if (!session.value("ranked", false) ||
          (session.value("flags", std::uint32_t{0}) &
           kSessionFlagUsesArbitration) == 0) {
        return Error(409, "arbitration_not_enabled",
                     "session does not use ranked arbitration");
      }
      auto arbitration = arbitration_snapshots_.find(id);
      if (arbitration == arbitration_snapshots_.end()) {
        const auto registering_member = std::find_if(
            session.at("members").begin(), session.at("members").end(),
            [&](const json& member) {
              return member.value("xuid", "") == identity.xuid;
            });
        if (registering_member == session.at("members").end() ||
            registering_member->value("machine_id", "") !=
                identity.machine_id) {
          return Error(403, "invalid_arbitration_registrant",
                       "authenticated member machine does not match the roster");
        }
        const std::uint32_t lifecycle_state =
            session.value("lifecycle_state", std::uint32_t{0});
        if (lifecycle_state != 0 && lifecycle_state != 1) {
          return Error(409, "invalid_arbitration_lifecycle",
                       "session cannot enter arbitration from its current lifecycle");
        }
        ArbitrationState state;
        state.nonce = nonce;
        state.duration_seconds = *duration;
        state.flags = *flags;
        state.deadline = Clock::now() + std::chrono::seconds(*duration);
        std::unordered_map<std::string, std::size_t> users_per_machine;
        for (const auto& member : session.at("members")) {
          const std::string machine_id = member.value("machine_id", "");
          if (!IsHex(machine_id, 16) ||
              machine_id == "0x0000000000000000") {
            return Error(409, "uncertifiable_arbitration_roster",
                         "every arbitration member needs a machine identity");
          }
          auto& user_count = users_per_machine[machine_id];
          ++user_count;
          if (user_count > kGta4ArbitrationUsersPerMachine) {
            return Error(409, "arbitration_machine_user_limit",
                         "a GTA IV arbitration machine may register at most four users");
          }
          state.expected_machine_ids.insert(machine_id);
          state.authorized_xuid_machines.emplace(
              member.value("xuid", ""), machine_id);
        }
        session["lifecycle_state"] = 1;
        session["state"] = "open";
        session["revision"] =
            session.value("revision", std::int64_t{0}) + 1;
        state.snapshot = session;
        arbitration =
            arbitration_snapshots_.emplace(id, std::move(state)).first;
        for (auto& [ticket_id, ticket] : tickets_) {
          (void)ticket_id;
          ResolveMatchmakingTicket(ticket);
        }
      } else if (arbitration->second.nonce != nonce ||
                 arbitration->second.duration_seconds != *duration ||
                 arbitration->second.flags != *flags) {
        return Error(409, "arbitration_tuple_mismatch",
                     "arbitration registration is already frozen");
      }

      ArbitrationState& state = arbitration->second;
      const auto authorization =
          state.authorized_xuid_machines.find(identity.xuid);
      if (authorization == state.authorized_xuid_machines.end() ||
          authorization->second != identity.machine_id ||
          !state.expected_machine_ids.contains(identity.machine_id)) {
        return Error(403, "invalid_arbitration_registrant",
                     "authenticated machine is absent from the frozen roster");
      }
      if (state.failed) {
        return Error(408, "arbitration_timeout",
                     "not every frozen machine registered before the title deadline");
      }
      const bool already_complete =
          state.registered_machine_ids.size() ==
          state.expected_machine_ids.size();
      if (already_complete) {
        state.registered_xuids.insert(identity.xuid);
        return {.body = state.snapshot};
      }
      if (Clock::now() >= state.deadline) {
        state.failed = true;
        return Error(408, "arbitration_timeout",
                     "not every frozen machine registered before the title deadline");
      }
      state.registered_machine_ids.insert(identity.machine_id);
    state.registered_xuids.insert(identity.xuid);
      const bool registration_complete =
          state.registered_machine_ids.size() ==
          state.expected_machine_ids.size();
      if (!registration_complete) {
        return {.status = 202, .body = state.snapshot};
      }
      return {.body = state.snapshot};
    }
    if (action == "join") {
      if (!SessionAcceptsJoins(session)) {
        return Error(409, "session_closed", "session is not accepting joins");
      }
      json member = body->value("member", json::object());
      if (!member.is_object() || !member.contains("xuid") || !member.at("xuid").is_string()) {
        return Error(400, "invalid_member", "session member is malformed");
      }
      if (member.at("xuid").get<std::string>() != identity.xuid) return Error(403, "identity_mismatch", "member identity does not match token");
      if ((body->contains("private") && !body->at("private").is_boolean()) ||
          (member.contains("private") && !member.at("private").is_boolean())) {
        return Error(400, "invalid_member", "session member is malformed");
      }
      const bool private_slot = body->value("private", member.value("private", false));
      if (member.contains("private") && member.at("private").get<bool>() != private_slot) {
        return Error(400, "invalid_member", "private slot selection is inconsistent");
      }
      member["private"] = private_slot;
      std::optional<std::int64_t> requested_online_port;
      if (member.contains("online_port")) {
        requested_online_port = NonNegativeInteger(member.at("online_port"));
      } else if (member.contains("route") && member.at("route").is_object() &&
                 member.at("route").contains("port")) {
        requested_online_port = NonNegativeInteger(member.at("route").at("port"));
      }
      if ((member.contains("online_port") ||
           (member.contains("route") && member.at("route").is_object() &&
            member.at("route").contains("port"))) &&
          (!requested_online_port ||
           *requested_online_port >
               std::numeric_limits<std::uint16_t>::max())) {
        return Error(400, "invalid_member", "member transport port is malformed");
      }
      // The authenticated device owns its machine and directory connection
      // identity. Never accept spoofed transport identity from the title
      // payload; only the bound UDP port is client-selected.
      member["machine_id"] = identity.machine_id;
      const std::string visibility = session.value("visibility", "public");
      const bool friend_join = AcceptedFriendsLocked(identity.xuid, session.value("host_xuid", ""));
      const bool invite_required = private_slot || visibility == "private";
      if (visibility == "friends" && !friend_join) {
        return Error(403, "friends_only", "session is visible only to accepted friends");
      }
      std::size_t public_members = 0;
      std::size_t private_members = 0;
      for (const auto& existing : session.at("members")) {
        if (existing.value("private", false)) ++private_members;
        else ++public_members;
      }
      auto existing_member = std::find_if(
          session["members"].begin(), session["members"].end(),
          [&](const json& existing) {
            return existing.value("xuid", "") == identity.xuid;
          });
      if (existing_member != session["members"].end()) {
        if (existing_member->value("private", false) == private_slot) {
          bool transport_changed = false;
          if (existing_member->value("machine_id", "") != identity.machine_id) {
            (*existing_member)["machine_id"] = identity.machine_id;
            transport_changed = true;
          }
          if (requested_online_port && *requested_online_port != 0 &&
              existing_member->value("online_port", 0) !=
                  *requested_online_port) {
            (*existing_member)["online_port"] = *requested_online_port;
            transport_changed = true;
          }
          if (!transport_changed) return {.body = session};
          NormalizeMembers(session);
          TraceServerMembership("server-membership-transport", id,
                                *existing_member);
        } else {
          if ((!private_slot &&
               public_members >=
                   static_cast<std::size_t>(session.value("public_slots", 0))) ||
              (private_slot &&
               private_members >=
                   static_cast<std::size_t>(session.value("private_slots", 0)))) {
            return Error(409, "session_full",
                         "requested slot class has no open slots");
          }
          (*existing_member)["private"] = private_slot;
          (*existing_member)["machine_id"] = identity.machine_id;
          if (requested_online_port && *requested_online_port != 0) {
            (*existing_member)["online_port"] = *requested_online_port;
          }
          NormalizeMembers(session);
          TraceServerMembership("server-membership-classification", id,
                                *existing_member);
        }
      } else {
      if ((!private_slot && public_members >= static_cast<std::size_t>(session.value("public_slots", 0))) ||
          (private_slot && private_members >= static_cast<std::size_t>(session.value("private_slots", 0)))) {
        return Error(409, "session_full", "requested slot class has no open slots");
      }
      std::string consumed_invite;
      std::optional<DurableData> staged_invites;
      if (invite_required) {
        const std::string requested_invite = body->value("invite_id", "");
        const std::int64_t now = UnixSecondsAfter();
        staged_invites = CaptureDurableLocked();
        for (auto invite = staged_invites->invites.begin(); invite != staged_invites->invites.end();) {
          if (invite->second.value("expires_at_unix", std::int64_t{0}) <= now) {
            invite = staged_invites->invites.erase(invite);
          } else {
            ++invite;
          }
        }
        for (const auto& [invite_id, invite] : staged_invites->invites) {
          if (!requested_invite.empty() && invite_id != requested_invite) continue;
          if (invite.value("session_id", "") != id ||
              invite.value("recipient_xuid", "") != identity.xuid ||
              invite.value("state", "") != "pending" ||
              !invite.value("acknowledged", false)) continue;
          if (consumed_invite.empty() || invite_id < consumed_invite) consumed_invite = invite_id;
        }
        if (consumed_invite.empty()) {
          if (staged_invites->invites.size() != invites_.size() &&
              !SaveAndCommitLocked(std::move(*staged_invites))) return StorageFailure();
          return Error(403, "invite_required", "a valid pending invite is required");
        }
      }
      member["account_id"] = identity.xuid;
      member["peer_id"] = identity.xuid;
      member.erase("virtual_ipv4");
      member.erase("route");
      json staged_session = session;
      staged_session["members"].push_back(member);
      if (!ValidSession(staged_session)) {
        return Error(400, "invalid_member", "member would violate session invariants");
      }
      if (!consumed_invite.empty()) {
        staged_invites->invites.erase(consumed_invite);
        if (!SaveAndCommitLocked(std::move(*staged_invites))) return StorageFailure();
      }
      session["members"].push_back(std::move(member));
      NormalizeMembers(session);
      const auto joined = std::find_if(session.at("members").begin(),
                                       session.at("members").end(), [&](const json& candidate) {
                                         return candidate.value("xuid", "") == identity.xuid;
                                       });
      if (joined != session.at("members").end()) {
        TraceServerMembership("server-membership", id, *joined);
      }
      if (auto lobby = lobbies_.find(id); lobby != lobbies_.end()) {
        lobby->second.ready[identity.xuid] = false;
        lobby->second.spectators[identity.xuid] = false;
        ++lobby->second.revision;
      }
      }
    } else if (action == "leave") {
      std::string target_xuid = identity.xuid;
      if (body->contains("xuid")) {
        if (!body->at("xuid").is_string() ||
            !IsHex(Lower(body->at("xuid").get<std::string>()), 16)) {
          return Error(400, "invalid_member", "session removal target is malformed");
        }
        target_xuid = Lower(body->at("xuid").get<std::string>());
      }
      if (target_xuid != identity.xuid && session.value("host_xuid", "") != identity.xuid) {
        return Error(403, "not_host", "only the host can remove another member");
      }
      if (!SessionContainsXuid(session, target_xuid)) {
        return Error(403, "not_member", "removal target is not a session member");
      }
      if (session.value("host_xuid", "") == target_xuid) {
        return Error(409, "host_must_migrate", "host must migrate or delete the session");
      }
      DurableData staged = CaptureDurableLocked();
      RemoveMemberSessionStatState(staged, target_xuid, id);
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      auto& members = session["members"];
      const auto leaving = std::find_if(members.begin(), members.end(), [&](const json& member) {
        return member.value("xuid", "") == target_xuid;
      });
      if (leaving != members.end()) TraceServerMembership("server-removal", id, *leaving);
      const auto remove = std::remove_if(members.begin(), members.end(), [&](const json& member) {
        return member.value("xuid", "") == target_xuid;
      });
      members.erase(remove, members.end());
      if (auto lobby = lobbies_.find(id); lobby != lobbies_.end()) {
        RemoveMemberFromLobby(lobby->second, target_xuid);
        ++lobby->second.revision;
      }
      RemoveMemberTransportState(id, target_xuid);
    } else if (action == "migration") {
      if (const auto arbitration = arbitration_snapshots_.find(id);
          arbitration != arbitration_snapshots_.end() &&
          arbitration->second.registered_machine_ids.size() !=
              arbitration->second.expected_machine_ids.size()) {
        return Error(409, "arbitration_pending",
                     "session cannot migrate while arbitration is pending");
      }
      if (!body->contains("expected_host_epoch")) {
        return Error(400, "expected_host_epoch_required", "expected host epoch is required");
      }
      const auto expected_epoch = NonNegativeInteger(body->at("expected_host_epoch"));
      if (!expected_epoch || *expected_epoch != session.value("host_epoch", 1ll)) {
        return Error(412, "host_epoch_mismatch", "session host epoch changed");
      }
      if (!body->contains("replacement") || !body->at("replacement").is_object()) {
        return Error(400, "invalid_migration", "replacement session is malformed");
      }
      json replacement = (*body)["replacement"];
      const bool replacement_state_supplied = replacement.contains("state");
      const bool replacement_lifecycle_supplied = replacement.contains("lifecycle_state");
      if (!replacement_state_supplied && !replacement_lifecycle_supplied) {
        replacement["state"] = session.value("state", "lobby");
        replacement["lifecycle_state"] = session.value("lifecycle_state", 0);
      }
      replacement["procedure_index"] =
          replacement.value("procedure_index", session.value("procedure_index", 0));
      replacement["join_in_progress"] =
          replacement.value("join_in_progress", session.value("join_in_progress", false));
      if (!NormalizeSessionLifecycle(replacement, replacement_state_supplied,
                                     replacement_lifecycle_supplied) ||
          !CanonicalizeSessionFlags(replacement) || !ValidSession(replacement)) {
        return Error(400, "invalid_migration", "replacement session is malformed");
      }
      const std::string previous_host = session.value("host_xuid", "");
      if (identity.xuid == previous_host || !SessionContainsXuid(session, identity.xuid) ||
          replacement.value("host_xuid", "") != identity.xuid) {
        return Error(403, "ineligible_successor",
                     "the title-selected successor must be a live non-host member");
      }
      static constexpr auto kPreservedFields = std::to_array<std::string_view>({
          "title_id", "media_id", "title_version", "protocol_version", "state", "visibility", "mode",
          "episode", "region", "ranked", "public_slots", "private_slots", "contexts", "properties",
          "procedure_index", "join_in_progress", "flags", "lifecycle_state"});
      for (const std::string_view field : kPreservedFields) {
        if (!replacement.contains(field) || replacement.at(field) != session.at(field)) {
          return Error(403, "invalid_migration", "replacement changes immutable session policy");
        }
      }
      std::map<std::string, bool> old_members;
      std::map<std::string, bool> new_members;
      for (const auto& member : session.at("members")) {
        old_members.emplace(member.value("xuid", ""), member.value("private", false));
      }
      for (const auto& member : replacement.at("members")) {
        new_members.emplace(member.value("xuid", ""), member.value("private", false));
      }
      bool membership_preserved = true;
      for (const auto& [xuid, private_slot] : new_members) {
        const auto old = old_members.find(xuid);
        if (old == old_members.end() || old->second != private_slot) {
          membership_preserved = false;
          break;
        }
      }
      for (const auto& [xuid, private_slot] : old_members) {
        (void)private_slot;
        if (xuid != previous_host && !new_members.contains(xuid)) {
          membership_preserved = false;
          break;
        }
      }
      if (!membership_preserved || replacement.value("session_id", "") == id ||
          sessions_.contains(replacement.value("session_id", ""))) {
        return Error(403, "invalid_migration", "replacement membership or identity is invalid");
      }
      replacement["previous_session_id"] = id;
      replacement["revision"] = 1;
      replacement["host_epoch"] = session.value("host_epoch", 1ll) + 1;
      NormalizeMembers(replacement);
      const std::string replacement_id = replacement["session_id"];
      DurableData staged = CaptureDurableLocked();
      for (auto& [invite_id, invite] : staged.invites) {
        (void)invite_id;
        if (invite.value("session_id", "") == id) invite["session_id"] = replacement_id;
      }
      for (auto& [receipt_key, receipt] : staged.invite_accept_receipts) {
        (void)receipt_key;
        if (!receipt.response.is_object() || !receipt.response.contains("invite") ||
            !receipt.response.at("invite").is_object() ||
            receipt.response.at("invite").value("session_id", "") != id) {
          continue;
        }
        receipt.response["invite"]["session_id"] = replacement_id;
        receipt.response["session"] = replacement;
      }
      RemoveSessionScopedDurable(staged, id, false);
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      if (auto lobby = lobbies_.find(id); lobby != lobbies_.end()) {
        lobbies_[replacement_id] = std::move(lobby->second);
        lobbies_.erase(lobby);
      }
      for (auto& [ticket_id, ticket] : tickets_) {
        (void)ticket_id;
        if (ticket.matched_session_id != id) continue;
        ticket.matched_session_id = replacement_id;
        ticket.updated_at = UtcIsoAfter();
      }
      // Removed identities lose transport and pending deliveries in the same
      // locked transaction that changes the roster. Retained sockets adopt the
      // new key domain with empty UDP queues, never packets from the old key.
      for (const auto& [xuid, private_slot] : old_members) {
        (void)private_slot;
        if (!new_members.contains(xuid)) RemoveMemberTransportState(id, xuid);
      }
      for (auto& [route_key, route] : relay_routes_) {
        if (route.session_id != id) continue;
        route.session_id = replacement_id;
        relay_queues_.erase(route_key);
        for (const auto& member : replacement.at("members")) {
          if (member.value("xuid", "") == route.xuid) {
            route.virtual_ipv4 = member.value("virtual_ipv4", "");
            break;
          }
        }
      }
      if (ranked_started_.erase(id)) ranked_started_.insert(replacement_id);
      if (session.contains("platform_stat_reports")) {
        replacement["platform_stat_reports"] = session.at("platform_stat_reports");
      }
      for (auto& [route_token, route] : voice_routes_) {
        (void)route_token;
        if (route.session_id == id) route.session_id = replacement_id;
      }
      for (auto& [route_token, packets] : voice_queues_) {
        (void)route_token;
        for (VoicePacket& packet : packets) {
          if (packet.session_id == id) packet.session_id = replacement_id;
        }
      }
      for (auto& [recipient_xuid, events] : realtime_events_) {
        (void)recipient_xuid;
        for (RealtimeEvent& event : events) {
          if (event.session_id != id) continue;
          event.session_id = replacement_id;
          event.wire["aggregate_id"] = replacement_id;
          event.wire["payload"]["session_id"] = replacement_id;
        }
      }
      std::erase_if(presence_, [&](const auto& entry) {
        return entry.second.session_id == id;
      });
      std::optional<ArbitrationState> migrated_arbitration;
      if (auto arbitration = arbitration_snapshots_.find(id);
          arbitration != arbitration_snapshots_.end()) {
        migrated_arbitration.emplace(std::move(arbitration->second));
        // The arbitration roster remains the exact immutable registration
        // result, while the descriptor and nonce follow the migrated session.
        // This permits deterministic replay without reopening membership.
        json migrated_snapshot = replacement;
        migrated_snapshot["members"] =
            migrated_arbitration->snapshot.at("members");
        migrated_arbitration->snapshot = std::move(migrated_snapshot);
        migrated_arbitration->nonce = replacement.value("nonce", "");
      }
      sessions_.erase(found);
      qos_listeners_.erase(id);
      arbitration_snapshots_.erase(id);
      sessions_[replacement_id] = replacement;
      if (migrated_arbitration) {
        arbitration_snapshots_.emplace(
            replacement_id, std::move(*migrated_arbitration));
      }
      session_leases_.erase(id);
      session_leases_[replacement_id] = Clock::now() + kSessionLease;
      return {.body = replacement};
    } else {
      return Error(404, "not_found", "session action does not exist");
    }
    session["revision"] = session.value("revision", 0) + 1;
    for (auto& [ticket_id, ticket] : tickets_) {
      (void)ticket_id;
      ResolveMatchmakingTicket(ticket);
    }
    return {.body = session};
    });
  }

  static bool SessionContainsXuid(const json& session, std::string_view xuid) {
    if (!session.contains("members") || !session["members"].is_array()) return false;
    return std::any_of(session["members"].begin(), session["members"].end(),
                       [&](const json& member) { return member.value("xuid", "") == xuid; });
  }

  // The strict-majority policy and its boundary vectors are derived by tools/derive_limits.py.
  static std::size_t KickVoteThreshold(std::size_t eligible_voters) {
    return eligible_voters / 2 + 1;
  }

  void EraseVoiceRouteLocked(const std::string& token) {
    voice_routes_.erase(token);
    voice_queues_.erase(token);
    voice_queue_encoded_bytes_.erase(token);
    voice_route_next_cursors_.erase(token);
    voice_route_delivered_cursors_.erase(token);
    voice_route_replay_after_cursors_.erase(token);
    voice_condition_.notify_all();
  }

  void PruneVoiceQueueLocked(const std::string& token, Clock::time_point now) {
    const auto found = voice_queues_.find(token);
    if (found == voice_queues_.end()) return;
    auto& queue = found->second;
    auto& queued_bytes = voice_queue_encoded_bytes_[token];
    while (!queue.empty() && queue.front().expires <= now) {
      queued_bytes -= std::min(queued_bytes, queue.front().encoded_bytes);
      queue.pop_front();
    }
    if (queue.empty()) {
      voice_queues_.erase(found);
      voice_queue_encoded_bytes_.erase(token);
    }
  }

  void RemoveMemberTransportState(const std::string& session_id, const std::string& xuid) {
    for (auto route = relay_routes_.begin(); route != relay_routes_.end();) {
      if (route->second.session_id != session_id || route->second.xuid != xuid) {
        ++route;
        continue;
      }
      relay_queues_.erase(route->first);
      route = relay_routes_.erase(route);
    }
    std::vector<std::string> removed_voice_routes;
    for (auto route = voice_routes_.begin(); route != voice_routes_.end(); ++route) {
      if (route->second.session_id == session_id && route->second.owner_xuid == xuid) {
        removed_voice_routes.push_back(route->first);
        continue;
      }
      if (route->second.session_id == session_id) {
        auto& targets = route->second.targets;
        targets.erase(std::remove(targets.begin(), targets.end(), xuid), targets.end());
        route->second.muted.erase(xuid);
      }
    }
    for (const std::string& token : removed_voice_routes) EraseVoiceRouteLocked(token);
    for (auto& [token, packets] : voice_queues_) {
      std::size_t removed_bytes = 0;
      std::erase_if(packets, [&](const VoicePacket& packet) {
        if (packet.session_id != session_id || packet.source_xuid != xuid) return false;
        removed_bytes += packet.encoded_bytes;
        return true;
      });
      auto bytes = voice_queue_encoded_bytes_.find(token);
      if (bytes != voice_queue_encoded_bytes_.end()) {
        bytes->second -= std::min(bytes->second, removed_bytes);
      }
    }
    if (auto recipient_events = realtime_events_.find(xuid);
        recipient_events != realtime_events_.end()) {
      std::erase_if(recipient_events->second, [&](const RealtimeEvent& event) {
        return event.session_id == session_id;
      });
      if (recipient_events->second.empty()) {
        realtime_events_.erase(recipient_events);
      }
    }
    for (auto events = realtime_events_.begin();
         events != realtime_events_.end();) {
      std::erase_if(events->second, [&](const RealtimeEvent& event) {
        return event.session_id == session_id && event.source_xuid == xuid;
      });
      if (events->second.empty()) {
        events = realtime_events_.erase(events);
      } else {
        ++events;
      }
    }
    if (const auto presence = presence_.find(xuid);
        presence != presence_.end() && presence->second.session_id == session_id) {
      presence_.erase(presence);
    }
    relay_condition_.notify_all();
    voice_condition_.notify_all();
    realtime_event_condition_.notify_all();
  }

  static void RemoveMemberFromLobby(LobbyState& lobby, const std::string& xuid) {
    lobby.ready.erase(xuid);
    lobby.spectators.erase(xuid);
    lobby.kick_votes.erase(xuid);
    for (auto& [target, voters] : lobby.kick_votes) {
      (void)target;
      voters.erase(xuid);
    }
  }

  Response LobbyOperation(const Request& request, const Identity& identity) {
    std::string suffix = SegmentAfter(request.path, "/api/v2/lobbies/");
    const auto slash = suffix.find('/');
    const std::string session_id = suffix.substr(0, slash);
    const std::string action = slash == std::string::npos ? std::string{} : suffix.substr(slash + 1);
    if (!IsHex(session_id, 16)) return Error(400, "invalid_session_id", "session id is malformed");

    std::lock_guard lock(mutex_);
    auto session_it = sessions_.find(session_id);
    if (session_it == sessions_.end()) return Error(404, "session_not_found", "session does not exist");
    json& session = session_it->second;
    if (!SessionContainsXuid(session, identity.xuid)) {
      return Error(403, "not_member", "identity is not a session member");
    }
    LobbyState& lobby = lobbies_[session_id];
    if (request.method == "GET" && action.empty()) return {.body = LobbyJson(session_id, lobby)};

    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    if (!body->contains("expected_session_revision")) {
      return Error(400, "invalid_lobby_request", "expected session revision is required");
    }
    const auto expected_revision = NonNegativeInteger((*body)["expected_session_revision"]);
    if (!expected_revision) {
      return Error(400, "invalid_lobby_request", "expected session revision is invalid");
    }
    if (*expected_revision != session.value("revision", 0ll)) {
      return Error(412, "revision_mismatch", "session revision changed");
    }

    if (request.method == "POST" && action == "ready") {
      if (!body->contains("ready") || !(*body)["ready"].is_boolean()) {
        return Error(400, "invalid_lobby_request", "ready must be boolean");
      }
      lobby.ready[identity.xuid] = (*body)["ready"].get<bool>();
      ++lobby.revision;
      return {.body = LobbyJson(session_id, lobby)};
    }
    if (request.method == "POST" && action == "spectator") {
      if (!body->contains("spectating") || !(*body)["spectating"].is_boolean()) {
        return Error(400, "invalid_lobby_request", "spectating must be boolean");
      }
      lobby.spectators[identity.xuid] = (*body)["spectating"].get<bool>();
      lobby.ready[identity.xuid] = false;
      ++lobby.revision;
      return {.body = LobbyJson(session_id, lobby)};
    }
    if (request.method == "PUT" && action == "next-game") {
      if (session.value("host_xuid", "") != identity.xuid) {
        return Error(403, "not_host", "only the host can select the next game");
      }
      if (!body->contains("next_game") || !(*body)["next_game"].is_object()) {
        return Error(400, "invalid_next_game", "next-game settings are malformed");
      }
      const json& next = (*body)["next_game"];
      if (!next.contains("mode") || !next["mode"].is_string() ||
          !next.contains("episode") || !next["episode"].is_string() ||
          (next.contains("contexts") && !next["contexts"].is_object()) ||
          (next.contains("properties") && !next["properties"].is_object())) {
        return Error(400, "invalid_next_game", "next-game settings are malformed");
      }
      const std::string mode = next.value("mode", "");
      const std::string episode = next.value("episode", "");
      const json contexts = next.value("contexts", json::object());
      const json properties = next.value("properties", json::object());
      if (!next.contains("ranked") || !next["ranked"].is_boolean() ||
          !ValidIdentifier(mode, 64) || !ValidEpisodeOrAll(episode) ||
          !ValidFilterObject(contexts, false) || !ValidFilterObject(properties, true)) {
        return Error(400, "invalid_next_game", "next-game settings are malformed");
      }
      lobby.next_game = NextGameState{.mode = mode,
                                      .episode = episode,
                                      .ranked = next["ranked"].get<bool>(),
                                      .contexts = contexts,
                                      .properties = properties};
      ++lobby.revision;
      return {.body = LobbyJson(session_id, lobby)};
    }
    if (request.method == "POST" && action == "kick-votes") {
      if (!body->contains("vote") || !(*body)["vote"].is_boolean()) {
        return Error(400, "invalid_kick_vote", "vote must be boolean");
      }
      const std::string target_xuid = body->value("target_xuid", "");
      if (!IsHex(target_xuid, 16) || target_xuid == identity.xuid ||
          target_xuid == session.value("host_xuid", "") || !SessionContainsXuid(session, target_xuid)) {
        return Error(403, "invalid_kick_target", "kick target is not eligible");
      }
      auto& voters = lobby.kick_votes[target_xuid];
      if ((*body)["vote"].get<bool>()) voters.insert(identity.xuid);
      else voters.erase(identity.xuid);
      ++lobby.revision;
      const std::size_t threshold =
          KickVoteThreshold(session["members"].size() - 1);
      const bool kicked = voters.size() >= threshold;
      if (!kicked) {
        return {.body = {{"lobby", LobbyJson(session_id, lobby)},
                         {"threshold", threshold}, {"kicked", false}}};
      }
      DurableData staged = CaptureDurableLocked();
      RemoveMemberSessionStatState(staged, target_xuid, session_id);
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      auto& members = session["members"];
      const auto remove = std::remove_if(members.begin(), members.end(), [&](const json& member) {
        return member.value("xuid", "") == target_xuid;
      });
      members.erase(remove, members.end());
      session["revision"] = session.value("revision", 0ll) + 1;
      RemoveMemberFromLobby(lobby, target_xuid);
      RemoveMemberTransportState(session_id, target_xuid);
      for (auto& [ticket_id, ticket] : tickets_) {
        (void)ticket_id;
        ResolveMatchmakingTicket(ticket);
      }
      return {.body = {{"lobby", LobbyJson(session_id, lobby)}, {"session", session},
                       {"threshold", threshold}, {"kicked", true}}};
    }
    return Error(404, "not_found", "lobby operation does not exist");
  }

  bool TicketCanMatchSession(const MatchmakingTicket& ticket, const json& session) {
    if (!SessionAcceptsJoins(session)) return false;
    const auto procedure_index =
        SessionUint32(session.value("procedure_index", json(0)));
    if (!procedure_index ||
        *procedure_index != ticket.procedure_index) {
      return false;
    }
    if (session.value("mode", "") != ticket.mode || session.value("ranked", false) != ticket.ranked) {
      return false;
    }
    const std::string episode = session.value("episode", "");
    if (episode != ticket.episode && episode != "all" && ticket.episode != "all") return false;
    if (!ticket.region.empty() && session.value("region", "") != ticket.region) return false;
    if (session.value("visibility", "public") == "private" &&
        !SessionContainsXuid(session, ticket.owner_xuid)) return false;
    if (session.value("visibility", "public") == "friends" &&
        !SessionContainsXuid(session, ticket.owner_xuid)) {
      const std::string host = session.value("host_xuid", "");
      const auto owner = relationships_.find(ticket.owner_xuid);
      if (owner == relationships_.end() || !owner->second.contains(host) ||
          owner->second.at(host) != "accepted") return false;
    }
    const int public_slots = session.value("public_slots", 0);
    const int private_slots = session.value("private_slots", 0);
    if (public_slots < 0 || private_slots < 0 ||
        public_slots > static_cast<int>(kMaximumSessionMembers) ||
        private_slots > static_cast<int>(kMaximumSessionMembers - public_slots)) return false;
    std::size_t public_members = 0;
    std::size_t private_members = 0;
    if (!session.contains("members") || !session.at("members").is_array()) return false;
    for (const auto& member : session.at("members")) {
      if (member.value("private", false)) ++private_members;
      else ++public_members;
    }
    if (public_members > static_cast<std::size_t>(public_slots) ||
        private_members > static_cast<std::size_t>(private_slots) ||
        ticket.party_size > static_cast<std::size_t>(public_slots) - public_members) return false;
    const json session_contexts = session.value("contexts", json::object());
    const json session_properties = session.value("properties", json::object());
    if (!session_contexts.is_object() || !session_properties.is_object()) return false;
    for (const auto& [key, value] : ticket.contexts.items()) {
      if (!session_contexts.contains(key) || session_contexts.at(key) != value) return false;
    }
    for (const auto& [key, value] : ticket.properties.items()) {
      if (!session_properties.contains(key) || session_properties.at(key) != value) return false;
    }
    return true;
  }

  void ResolveMatchmakingTicket(MatchmakingTicket& ticket) {
    if (ticket.state == "matched") {
      const auto matched = sessions_.find(ticket.matched_session_id);
      if (matched != sessions_.end() &&
          SessionLiveLocked(ticket.matched_session_id, Clock::now()) &&
          TicketCanMatchSession(ticket, matched->second)) return;
      ticket.state = "searching";
      ticket.matched_session_id.clear();
      ticket.updated_at = UtcIsoAfter();
    }
    if (ticket.state != "searching") return;
    for (const auto& [session_id, session] : sessions_) {
      if (!SessionLiveLocked(session_id, Clock::now())) continue;
      if (!TicketCanMatchSession(ticket, session)) continue;
      ticket.state = "matched";
      ticket.matched_session_id = session_id;
      ticket.updated_at = UtcIsoAfter();
      return;
    }
  }

  Response CreateMatchmakingTicket(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    if (!body->contains("procedure_index") || !body->contains("party_size") ||
        !body->contains("mode") || !(*body)["mode"].is_string() ||
        !body->contains("episode") || !(*body)["episode"].is_string() ||
        (body->contains("region") && !(*body)["region"].is_string()) ||
        (body->contains("contexts") && !(*body)["contexts"].is_object()) ||
        (body->contains("properties") && !(*body)["properties"].is_object())) {
      return Error(400, "invalid_ticket", "ticket fields are required");
    }
    const auto procedure_index = SessionUint32((*body)["procedure_index"]);
    const auto party_size = NonNegativeInteger((*body)["party_size"]);
    const std::string mode = body->value("mode", "");
    const std::string episode = body->value("episode", "");
    const std::string region = body->value("region", "");
    const json contexts = body->value("contexts", json::object());
    const json properties = body->value("properties", json::object());
    if (!procedure_index || !party_size || *party_size < 1 ||
        *party_size > static_cast<std::int64_t>(kMaximumSessionMembers) ||
        !body->contains("ranked") || !(*body)["ranked"].is_boolean() ||
        !ValidIdentifier(mode, 64) || !ValidEpisodeOrAll(episode) ||
        (!region.empty() && !ValidIdentifier(region, 32)) ||
        !ValidFilterObject(contexts, false) || !ValidFilterObject(properties, true)) {
      return Error(400, "invalid_ticket", "ticket validation failed");
    }
    MatchmakingTicket ticket{.id = RandomToken("ticket_", 16),
                             .owner_xuid = identity.xuid,
                             .procedure_index = *procedure_index,
                             .mode = mode,
                             .episode = episode,
                             .region = region,
                             .ranked = (*body)["ranked"].get<bool>(),
                             .party_size = static_cast<std::size_t>(*party_size),
                             .contexts = contexts,
                             .properties = properties,
                             .state = "searching",
                             .created_at = UtcIsoAfter(),
                             .updated_at = UtcIsoAfter(),
                             .expires = Clock::now() + kMatchmakingTicketLease};
    if (ticket.id.empty()) return Error(500, "entropy_failure", "random generation failed");
    std::lock_guard lock(mutex_);
    ResolveMatchmakingTicket(ticket);
    const std::string id = ticket.id;
    tickets_[id] = std::move(ticket);
    return {.status = 201, .body = TicketJson(tickets_.at(id))};
  }

  Response MatchmakingTicketOperation(const Request& request, const Identity& identity) {
    const std::string id = SegmentAfter(request.path, "/api/v2/matchmaking/tickets/");
    if (!ValidIdentifier(id)) return Error(400, "invalid_ticket_id", "ticket id is malformed");
    std::lock_guard lock(mutex_);
    auto found = tickets_.find(id);
    if (found == tickets_.end()) return Error(404, "ticket_not_found", "ticket does not exist");
    if (found->second.owner_xuid != identity.xuid) {
      return Error(403, "not_ticket_owner", "ticket belongs to another identity");
    }
    if (request.method == "GET") {
      found->second.expires = Clock::now() + kMatchmakingTicketLease;
      ResolveMatchmakingTicket(found->second);
      return {.body = TicketJson(found->second)};
    }
    if (request.method == "DELETE") {
      tickets_.erase(found);
      return {.body = {{"id", id}, {"deleted", true}}};
    }
    return Error(404, "not_found", "ticket operation does not exist");
  }

  Response RelayRoutes(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body) return Error(400, "invalid_json", "invalid JSON body");
    const std::string session_id = body->value("session_id", "");
    const auto port = body->value("local_port", 0);
    if (!IsHex(session_id, 16) || port <= 0 || port > 65535) return Error(400, "invalid_route", "relay route is malformed");
    std::lock_guard lock(mutex_);
    const auto session = sessions_.find(session_id);
    if (session == sessions_.end() || !SessionLiveLocked(session_id, Clock::now())) {
      return Error(404, "session_not_found", "session does not exist");
    }
    std::string address;
    for (const auto& member : session->second["members"]) {
      if (member.value("xuid", "") == identity.xuid) address = member.value("virtual_ipv4", "");
    }
    if (address.empty()) return Error(403, "not_member", "identity is not a session member");
    const std::string key = identity.xuid + ":" + std::to_string(port);
    const auto existing = relay_routes_.find(key);
    if (request.method == "DELETE") {
      if (existing != relay_routes_.end() && existing->second.session_id != session_id) {
        return Error(409, "route_rebound", "a stale unregister cannot delete a newer route");
      }
      relay_routes_.erase(key);
      relay_queues_.erase(key);
      return {.status = 204};
    }
    if (existing != relay_routes_.end() && existing->second.session_id != session_id) {
      relay_queues_.erase(key);
    }
    relay_routes_[key] = {.xuid = identity.xuid, .session_id = session_id,
                          .port = static_cast<std::uint16_t>(port), .virtual_ipv4 = address,
                          .expires = Clock::now() + kRelayRouteLease};
    return {.body = {{"virtual_ipv4", address}, {"local_port", port}}};
  }

  Response QosListeners(const Request& request, const Identity& identity) {
    if (request.method != "PUT" && request.method != "DELETE") {
      return Error(404, "not_found", "QoS listener operation does not exist");
    }
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    const std::string session_id = body->value("session_id", "");
    const std::string exchange_key = body->value("exchange_key", "");
    if (!IsHex(session_id, 16) || !IsHex(exchange_key, 32)) {
      return Error(400, "invalid_qos_listener", "QoS session identity is malformed");
    }
    std::lock_guard lock(mutex_);
    const auto session = sessions_.find(session_id);
    if (session == sessions_.end() || !SessionLiveLocked(session_id, Clock::now())) {
      return Error(404, "session_not_found", "session does not exist");
    }
    if (session->second.value("host_xuid", "") != identity.xuid) {
      return Error(403, "not_host", "only the session host can manage its QoS listener");
    }
    if (session->second.value("exchange_key", "") != exchange_key) {
      return Error(403, "invalid_exchange_key", "QoS exchange key does not match session");
    }
    if (request.method == "DELETE") {
      qos_listeners_.erase(session_id);
      return {.status = 204};
    }

    const bool has_enabled = body->contains("enabled");
    const bool has_title_data = body->contains("title_data");
    const bool has_bits_per_second = body->contains("bits_per_second");
    if ((!has_enabled && !has_title_data && !has_bits_per_second) ||
        (has_enabled && !body->at("enabled").is_boolean())) {
      return Error(400, "invalid_qos_listener", "QoS listener update is malformed");
    }
    std::optional<std::string> title_data;
    if (has_title_data) {
      if (!body->at("title_data").is_string()) {
        return Error(400, "invalid_qos_title_data", "QoS title data must be exactly 12 bytes");
      }
      title_data = body->at("title_data").get<std::string>();
      const auto decoded = DecodeBase64Url(*title_data);
      if (title_data->size() != kQosTitleDataEncodedBytes || !decoded ||
          decoded->size() != kQosTitleDataBytes) {
        return Error(400, "invalid_qos_title_data", "QoS title data must be exactly 12 bytes");
      }
    }
    std::optional<std::uint32_t> bits_per_second;
    if (has_bits_per_second) {
      const auto bits = NonNegativeInteger(body->at("bits_per_second"));
      if (!bits || static_cast<std::uint64_t>(*bits) >
                       std::numeric_limits<std::uint32_t>::max()) {
        return Error(400, "invalid_qos_listener", "QoS bandwidth is malformed");
      }
      bits_per_second = static_cast<std::uint32_t>(*bits);
    }

    auto& listener = qos_listeners_[session_id];
    const bool changed = !listener.generation || listener.host_xuid != identity.xuid ||
        listener.exchange_key != exchange_key ||
        (has_enabled && listener.enabled != body->at("enabled").get<bool>()) ||
        (title_data && listener.title_data != title_data) ||
        (bits_per_second && listener.bits_per_second != *bits_per_second);
    if (changed) {
      if (next_qos_listener_generation_ == std::numeric_limits<std::uint64_t>::max()) {
        return Error(409, "qos_generation_exhausted", "QoS listener generation exhausted");
      }
      listener.generation = ++next_qos_listener_generation_;
    }
    listener.host_xuid = identity.xuid;
    listener.exchange_key = exchange_key;
    if (has_enabled) listener.enabled = body->at("enabled").get<bool>();
    if (title_data) listener.title_data = *title_data;
    if (bits_per_second) listener.bits_per_second = *bits_per_second;
    return {.status = 204};
  }

#include "qos_probe_service.inc"

  bool RelayRouteLiveLocked(const RelayRoute& route, Clock::time_point now) const {
    const auto session = sessions_.find(route.session_id);
    if (route.expires <= now || session == sessions_.end() ||
        !SessionLiveLocked(route.session_id, now)) return false;
    return std::ranges::any_of(session->second.at("members"), [&](const json& member) {
      return member.value("xuid", "") == route.xuid &&
             member.value("virtual_ipv4", "") == route.virtual_ipv4;
    });
  }

  Response RelayDatagrams(const Request& request, const Identity& identity) {
    std::unique_lock lock(mutex_);
    if (request.method == "GET") {
      const int port = QueryInt(request, "local_port", 0);
      const std::string key = identity.xuid + ":" + std::to_string(port);
      auto route = relay_routes_.find(key);
      if (route == relay_routes_.end() || !RelayRouteLiveLocked(route->second, Clock::now())) {
        if (route != relay_routes_.end()) {
          relay_routes_.erase(route);
          relay_queues_.erase(key);
          relay_condition_.notify_all();
        }
        return Error(404, "route_not_found", "relay route does not exist");
      }
      const auto requested_session = request.query.find("session_id");
      if (requested_session != request.query.end() &&
          requested_session->second != route->second.session_id) {
        return Error(409, "route_rebound", "poll belongs to an older session route");
      }
      const std::string polled_session = route->second.session_id;
      route->second.expires = Clock::now() + kRelayRouteLease;
      const int wait_ms = std::clamp(QueryInt(request, "wait_ms", 0), 0, 1000);
      if (relay_queues_[key].empty() && wait_ms != 0) {
        relay_condition_.wait_for(lock, std::chrono::milliseconds(wait_ms), [&] {
          return !relay_routes_.contains(key) || !relay_queues_[key].empty();
        });
      }
      route = relay_routes_.find(key);
      if (route == relay_routes_.end() || !RelayRouteLiveLocked(route->second, Clock::now())) {
        return Error(404, "route_not_found", "relay route does not exist");
      }
      if (route->second.session_id != polled_session) {
        return Error(409, "route_rebound", "poll route changed before delivery");
      }
      json output = json::array();
      std::size_t bytes = 0;
      auto& queue = relay_queues_[key];
      while (!queue.empty() && output.size() < 64 && bytes + queue.front().payload.size() <= kMaximumRelayBatchBytes) {
        bytes += queue.front().payload.size();
        output.push_back({{"source_ipv4", queue.front().source_ipv4},
                          {"source_port", queue.front().source_port}, {"payload", queue.front().payload}});
        queue.pop_front();
      }
      return {.body = {{"datagrams", std::move(output)}}};
    }
    const auto body = ParseBody(request);
    if (!body || !body->contains("datagrams") || !(*body)["datagrams"].is_array() ||
        (*body)["datagrams"].size() > 64) return Error(400, "invalid_datagrams", "datagram batch is malformed");
    std::size_t batch_bytes = 0;
    for (const auto& wire : (*body)["datagrams"]) {
      const int source_port = wire.value("source_port", 0);
      const std::string source_key = identity.xuid + ":" + std::to_string(source_port);
      auto source = relay_routes_.find(source_key);
      if (source == relay_routes_.end()) continue;
      if (!RelayRouteLiveLocked(source->second, Clock::now())) {
        relay_routes_.erase(source);
        relay_queues_.erase(source_key);
        relay_condition_.notify_all();
        continue;
      }
      if (wire.contains("session_id") &&
          (!wire.at("session_id").is_string() ||
           wire.at("session_id").get<std::string>() != source->second.session_id)) continue;
      source->second.expires = Clock::now() + kRelayRouteLease;
      const std::string destination = wire.value("destination_ipv4", "");
      const int destination_port = wire.value("destination_port", 0);
      const std::string payload = wire.value("payload", "");
      const auto decoded = DecodeBase64Url(payload);
      if (!decoded || decoded->size() > kMaximumRelayDatagramBytes ||
          decoded->size() > kMaximumRelayBatchBytes - batch_bytes) continue;
      batch_bytes += decoded->size();
      for (const auto& [key, route] : relay_routes_) {
        if (RelayRouteLiveLocked(route, Clock::now()) &&
            route.session_id == source->second.session_id && route.virtual_ipv4 == destination && route.port == destination_port) {
          auto& queue = relay_queues_[key];
          if (queue.size() == 256) queue.pop_front();
          queue.push_back({.source_ipv4 = source->second.virtual_ipv4,
                           .source_port = source->second.port, .payload = payload});
        }
      }
    }
    relay_condition_.notify_all();
    return {.status = 204};
  }

  static int QueryInt(const Request& request, const std::string& name, int fallback) {
    const auto found = request.query.find(name);
    if (found == request.query.end()) return fallback;
    int value = fallback;
    const auto parsed = std::from_chars(found->second.data(), found->second.data() + found->second.size(), value);
    return parsed.ec == std::errc{} ? value : fallback;
  }

  Response FriendsCheck(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->contains("xuids") || !(*body)["xuids"].is_array() ||
        (*body)["xuids"].empty() ||
        (*body)["xuids"].size() > kMaximumFriendsCheckItems) {
      return Error(400, "invalid_xuids", "xuid list is malformed");
    }
    json results = json::array();
    std::unordered_set<std::string> unique_xuids;
    std::lock_guard lock(mutex_);
    for (const auto& xuid_value : (*body)["xuids"]) {
      if (!xuid_value.is_string() ||
          !IsHex(Lower(xuid_value.get<std::string>()), 16)) {
        return Error(400, "invalid_xuids", "xuid list is malformed");
      }
      const std::string xuid = Lower(xuid_value.get<std::string>());
      if (xuid == "0x0000000000000000" || !unique_xuids.insert(xuid).second) {
        return Error(400, "invalid_xuids", "xuid list is malformed");
      }
      const auto owner = relationships_.find(identity.xuid);
      const std::string state = owner != relationships_.end() && owner->second.contains(xuid)
                                    ? owner->second.at(xuid)
                                    : "removed";
      results.push_back({{"xuid", xuid}, {"is_friend", state == "accepted"},
                         {"is_blocked", RelationshipStateBlocks(state)}});
    }
    return {.body = {{"results", std::move(results)}}};
  }

  Response FriendsList(const Request& request, const Identity& identity) {
    std::size_t limit = kMaximumFriendsPageItems;
    if (const auto found = request.query.find("limit"); found != request.query.end()) {
      std::size_t parsed_limit = 0;
      const auto parsed = std::from_chars(found->second.data(),
                                          found->second.data() + found->second.size(),
                                          parsed_limit);
      if (parsed.ec != std::errc{} || parsed.ptr != found->second.data() + found->second.size() ||
          parsed_limit == 0 || parsed_limit > kMaximumFriendsPageItems) {
        return Error(400, "invalid_pagination", "friend page limit is invalid");
      }
      limit = parsed_limit;
    }
    std::string cursor;
    if (const auto found = request.query.find("cursor"); found != request.query.end()) {
      cursor = found->second;
      if (!cursor.empty() && (!IsHex(cursor, 16) || Lower(cursor) != cursor)) {
        return Error(400, "invalid_pagination", "friend page cursor is invalid");
      }
    }
    std::string state_filter;
    if (const auto found = request.query.find("state");
        found != request.query.end()) {
      state_filter = found->second;
      if (state_filter != "incoming" && state_filter != "outgoing" &&
          state_filter != "accepted" && state_filter != "removed" &&
          state_filter != "blocked" && state_filter != "blocked_by") {
        return Error(400, "invalid_relationship_state",
                     "friend relationship state is invalid");
      }
    }

    json items = json::array();
    std::lock_guard lock(mutex_);
    const auto owner = relationships_.find(identity.xuid);
    if (owner != relationships_.end()) {
      std::vector<std::pair<std::string, std::string>> rows;
      for (const auto& relationship : owner->second) {
        if (!state_filter.empty() && relationship.second != state_filter) continue;
        rows.push_back(relationship);
      }
      std::ranges::sort(rows, {}, &std::pair<std::string, std::string>::first);
      std::size_t index = 0;
      if (!cursor.empty()) {
        index = static_cast<std::size_t>(std::upper_bound(
                    rows.begin(), rows.end(), cursor,
                    [](const std::string& value, const auto& row) { return value < row.first; }) -
                rows.begin());
      }
      for (; index < rows.size() && items.size() < limit; ++index) {
        const auto& [xuid, state] = rows[index];
        std::string presence = "offline";
        std::string presence_session_id;
        std::string presence_episode;
        const auto live = presence_.find(xuid);
        if (live != presence_.end() && Clock::now() < live->second.expires) {
          bool valid_presence = true;
          if (!live->second.session_id.empty()) {
            const auto session = sessions_.find(live->second.session_id);
            valid_presence =
                session != sessions_.end() &&
                SessionLiveLocked(live->second.session_id, Clock::now()) &&
                SessionContainsXuid(session->second, xuid);
          }
          if (valid_presence) {
            presence = live->second.state == "in_game" ? "playing" : "online";
            presence_session_id = live->second.session_id;
            presence_episode = live->second.episode;
          } else {
            presence_.erase(live);
          }
        }
        items.push_back({{"xuid", xuid}, {"account_id", xuid},
                         {"player_name", player_names_.contains(xuid) ? player_names_.at(xuid) : "Player"},
                         {"state", state}, {"presence", presence},
                         {"session_id", presence_session_id},
                         {"episode", presence_episode},
                         {"is_blocked", RelationshipStateBlocks(state)}});
      }
      const std::string next_cursor =
          index < rows.size() && !items.empty()
              ? items.back().at("xuid").get<std::string>()
              : std::string{};
      return {.body = {{"items", std::move(items)}, {"next_cursor", next_cursor}}};
    }
    return {.body = {{"items", std::move(items)}, {"next_cursor", ""}}};
  }

  Response FriendRelationship(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("target_xuid") ||
        !body->at("target_xuid").is_string() || !body->contains("action") ||
        !body->at("action").is_string()) {
      return Error(400, "invalid_json", "invalid JSON body");
    }
    const std::string target = body->at("target_xuid").get<std::string>();
    const std::string action = body->at("action").get<std::string>();
    if (!IsHex(target, 16) || target == identity.xuid) {
      return Error(400, "invalid_relationship", "friend target is invalid");
    }
    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    const auto relationship_state = [&staged](const std::string& owner,
                                              const std::string& other) -> std::string {
      const auto owner_it = staged.relationships.find(owner);
      if (owner_it == staged.relationships.end()) return "removed";
      const auto other_it = owner_it->second.find(other);
      return other_it == owner_it->second.end() ? "removed" : other_it->second;
    };
    if (action == "request") {
      const std::string current = relationship_state(identity.xuid, target);
      const std::string reverse = relationship_state(target, identity.xuid);
      if (RelationshipStateBlocks(current) || RelationshipStateBlocks(reverse)) {
        return Error(409, "relationship_blocked", "friend request is blocked");
      }
      if (current != "accepted" || reverse != "accepted") {
        staged.relationships[identity.xuid][target] = "outgoing";
        staged.relationships[target][identity.xuid] = "incoming";
      }
    } else if (action == "accept") {
      const std::string current = relationship_state(identity.xuid, target);
      const std::string reverse = relationship_state(target, identity.xuid);
      if (current == "accepted" && reverse == "accepted") {
        // Retried accepts are idempotent.
      } else if (current != "incoming" || reverse != "outgoing") {
        return Error(409, "request_not_found", "incoming friend request does not exist");
      } else {
        staged.relationships[identity.xuid][target] = "accepted";
        staged.relationships[target][identity.xuid] = "accepted";
      }
    } else if (action == "remove") {
      const std::string current = relationship_state(identity.xuid, target);
      const std::string reverse = relationship_state(target, identity.xuid);
      if (!RelationshipStateBlocks(current) && !RelationshipStateBlocks(reverse)) {
        const auto owner_it = staged.relationships.find(identity.xuid);
        if (owner_it != staged.relationships.end()) owner_it->second.erase(target);
        const auto target_it = staged.relationships.find(target);
        if (target_it != staged.relationships.end()) target_it->second.erase(identity.xuid);
      }
    } else if (action == "block") {
      const std::string current = relationship_state(identity.xuid, target);
      const std::string reverse = relationship_state(target, identity.xuid);
      if (current == "blocked_by" || current == "blocked_mutual" ||
          reverse == "blocked") {
        staged.relationships[identity.xuid][target] = "blocked";
        staged.relationships[target][identity.xuid] = "blocked";
      } else {
        staged.relationships[identity.xuid][target] = "blocked";
        staged.relationships[target][identity.xuid] = "blocked_by";
      }
    } else if (action == "unblock") {
      const std::string current = relationship_state(identity.xuid, target);
      const std::string reverse = relationship_state(target, identity.xuid);
      if (current == "blocked_mutual" ||
          (current == "blocked" && reverse == "blocked")) {
        staged.relationships[identity.xuid][target] = "blocked_by";
        staged.relationships[target][identity.xuid] = "blocked";
      } else if (current == "blocked") {
        const auto owner_it = staged.relationships.find(identity.xuid);
        if (owner_it != staged.relationships.end()) owner_it->second.erase(target);
        if (relationship_state(target, identity.xuid) == "blocked_by") {
          const auto target_it = staged.relationships.find(target);
          if (target_it != staged.relationships.end()) target_it->second.erase(identity.xuid);
        }
      }
    } else {
      return Error(400, "invalid_relationship", "friend relationship action is invalid");
    }
    const std::string state = relationship_state(identity.xuid, target);
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.body = {{"xuid", target}, {"state", state}}};
  }

  Response CreateInvites(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("session_id") ||
        !body->at("session_id").is_string() || !body->contains("recipient_xuids") ||
        !body->at("recipient_xuids").is_array() || body->at("recipient_xuids").empty() ||
        body->at("recipient_xuids").size() > kMaximumInviteRecipients ||
        !body->contains("custom_data") || !body->at("custom_data").is_string()) {
      return Error(400, "invalid_invite", "invite is malformed");
    }
    const std::string session_id = body->value("session_id", "");
    const std::string custom_data = body->at("custom_data").get<std::string>();
    const auto custom = DecodeBase64Url(custom_data);
    if (!IsHex(session_id, 16) || !custom || custom->size() > kMaximumInviteCustomBytes) {
      return Error(400, "invalid_invite", "invite session or custom data is malformed");
    }
    if (const auto key = request.headers.find("idempotency-key");
        key != request.headers.end() &&
        !ValidIdentifier(key->second, kMaximumIdempotencyKeyBytes)) {
      return Error(400, "invalid_idempotency_key", "Idempotency-Key is invalid");
    }
    std::vector<std::pair<std::string, std::string>> generated;
    generated.reserve(body->at("recipient_xuids").size());
    std::unordered_set<std::string> unique_recipients;
    for (const auto& recipient_value : body->at("recipient_xuids")) {
      if (!recipient_value.is_string() ||
          !IsHex(recipient_value.get<std::string>(), 16) ||
          recipient_value.get<std::string>() == "0x0000000000000000" ||
          recipient_value.get<std::string>() == identity.xuid ||
          !unique_recipients.insert(recipient_value.get<std::string>()).second) {
        return Error(400, "invalid_invite", "invite recipient is malformed");
      }
      const std::string id = RandomToken("invite_", 12);
      if (id.empty()) return Error(500, "entropy_failure", "random generation failed");
      generated.emplace_back(id, recipient_value.get<std::string>());
    }
    std::lock_guard lock(mutex_);
    const auto live_session = sessions_.find(session_id);
    if (live_session == sessions_.end()) {
      return Error(404, "session_not_found", "invite session does not exist");
    }
    if (!SessionAcceptsJoins(live_session->second)) {
      return Error(409, "session_closed", "session is not accepting invitations");
    }
    if (!SessionContainsXuid(live_session->second, identity.xuid)) {
      return Error(403, "not_member", "invite sender is not a session member");
    }
    for (const auto& [id, recipient] : generated) {
      (void)id;
      if (!player_names_.contains(recipient)) {
        return Error(404, "recipient_not_found", "invite recipient is unknown");
      }
      if (RelationshipBlocksLocked(identity.xuid, recipient)) {
        return Error(409, "relationship_blocked", "invite recipient is blocked");
      }
    }
    DurableData staged = CaptureDurableLocked();
    const std::int64_t now = UnixSecondsAfter();
    for (auto invite = staged.invites.begin(); invite != staged.invites.end();) {
      if (invite->second.value("expires_at_unix", std::int64_t{0}) <= now ||
          !sessions_.contains(invite->second.value("session_id", ""))) {
        invite = staged.invites.erase(invite);
      } else {
        ++invite;
      }
    }
    const std::string created_at = UtcIsoAfter();
    std::chrono::seconds requested_lifetime = kInviteLifetime;
    if (body->contains("expires_in_seconds")) {
      const auto seconds = NonNegativeInteger(body->at("expires_in_seconds"));
      if (!seconds || *seconds < 1) {
        return Error(400, "invalid_invite", "invite expiry is malformed");
      }
      requested_lifetime = std::chrono::seconds(
          std::min<std::int64_t>(*seconds, kInviteLifetime.count()));
    }
    const std::int64_t expires_at = UnixSecondsAfter(requested_lifetime);
    for (const auto& [id, recipient] : generated) {
      const bool duplicate = std::any_of(
          staged.invites.begin(), staged.invites.end(), [&](const auto& entry) {
            const json& invite = entry.second;
            return invite.value("sender_xuid", "") == identity.xuid &&
                   invite.value("recipient_xuid", "") == recipient &&
                   invite.value("session_id", "") == session_id &&
                   invite.value("custom_data", "") == custom_data &&
                   invite.value("state", "") == "pending" &&
                   invite.value("expires_at_unix", std::int64_t{0}) > now;
          });
      if (duplicate) continue;
      staged.invites[id] = {{"id", id}, {"sender_xuid", identity.xuid},
                             {"recipient_xuid", recipient}, {"session_id", session_id},
                             {"custom_data", custom_data},
                             {"revision", 1}, {"state", "pending"}, {"acknowledged", false},
                             {"created_at", created_at}, {"expires_at_unix", expires_at}};
    }
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.status = 201, .body = {{"status", "created"}}};
  }

  Response ListInvites(const Request& request, const Identity& identity) {
    std::size_t limit = kMaximumFriendsPageItems;
    if (const auto found = request.query.find("limit"); found != request.query.end()) {
      std::size_t parsed_limit = 0;
      const auto parsed = std::from_chars(found->second.data(),
                                          found->second.data() + found->second.size(),
                                          parsed_limit);
      if (parsed.ec != std::errc{} ||
          parsed.ptr != found->second.data() + found->second.size() || parsed_limit == 0 ||
          parsed_limit > kMaximumFriendsPageItems) {
        return Error(400, "invalid_pagination", "invite page limit is invalid");
      }
      limit = parsed_limit;
    }
    if (const auto found = request.query.find("state");
        found != request.query.end() && found->second != "pending") {
      return Error(400, "invalid_invite_state", "invite state filter is invalid");
    }
    json items = json::array();
    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    bool cleaned = false;
    const std::int64_t now = UnixSecondsAfter();
    for (auto invite = staged.invites.begin(); invite != staged.invites.end();) {
      if (invite->second.value("expires_at_unix", std::int64_t{0}) <= now ||
          !sessions_.contains(invite->second.value("session_id", ""))) {
        invite = staged.invites.erase(invite);
        cleaned = true;
      } else {
        ++invite;
      }
    }
    if (cleaned && !SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    std::vector<std::pair<std::string, json>> pending;
    for (const auto& [id, invite] : invites_) {
      if (invite.value("recipient_xuid", "") == identity.xuid &&
          invite.value("state", "") == "pending" &&
          !invite.value("acknowledged", false) &&
          invite.value("expires_at_unix", std::int64_t{0}) > now) {
        pending.emplace_back(id, invite);
      }
    }
    std::ranges::sort(pending, {}, &std::pair<std::string, json>::first);
    for (const auto& [id, invite] : pending) {
      (void)id;
      if (items.size() >= limit) break;
      items.push_back(invite);
    }
    return {.body = {{"items", std::move(items)}}};
  }

  Response AcceptInvite(const Request& request, const Identity& identity) {
    std::string id = SegmentAfter(request.path, "/api/v2/invites/");
    id.resize(id.size() - std::string("/accept").size());
    const auto body = ParseBody(request);
    std::optional<std::int64_t> expected;
    std::optional<std::string> request_digest;
    std::optional<std::string> idempotency_key;
    std::optional<std::string> receipt_key;
    if (const auto found = request.headers.find("idempotency-key");
        found != request.headers.end()) {
      if (!ValidIdentifier(found->second, kMaximumIdempotencyKeyBytes)) {
        return Error(400, "invalid_idempotency_key", "Idempotency-Key is invalid");
      }
      idempotency_key = found->second;
      if (!body || !body->is_object() || !body->contains("expected_revision")) {
        return Error(400, "expected_revision_required", "expected invite revision is required");
      }
      expected = NonNegativeInteger(body->at("expected_revision"));
      if (!expected) return Error(400, "invalid_revision", "expected invite revision is invalid");
      request_digest = Sha256Hex(body->dump());
      if (!request_digest) {
        return Error(500, "digest_failure", "invite accept request could not be hashed");
      }
      receipt_key = InviteAcceptReceiptKey(identity.xuid, id, *idempotency_key);
      if (!receipt_key) {
        return Error(500, "digest_failure", "invite accept key could not be hashed");
      }
    }

    std::lock_guard lock(mutex_);
    if (receipt_key) {
      const auto receipt = invite_accept_receipts_.find(*receipt_key);
      if (receipt != invite_accept_receipts_.end()) {
        if (receipt->second.request_digest != *request_digest) {
          return Error(409, "idempotency_conflict",
                       "Idempotency-Key was already used for a different request");
        }
        return {.body = receipt->second.response};
      }
    }
    const auto found = invites_.find(id);
    if (found == invites_.end() || found->second.value("recipient_xuid", "") != identity.xuid) {
      return Error(404, "invite_not_found", "invite does not exist");
    }
    if (found->second.value("state", "") != "pending") {
      return Error(409, "invite_not_pending", "invite is no longer pending");
    }
    if (found->second.value("expires_at_unix", std::int64_t{0}) <= UnixSecondsAfter()) {
      DurableData staged = CaptureDurableLocked();
      staged.invites.erase(id);
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      return Error(409, "invite_expired", "invite has expired");
    }
    const auto session = sessions_.find(found->second.value("session_id", ""));
    if (session == sessions_.end()) {
      DurableData staged = CaptureDurableLocked();
      staged.invites.erase(id);
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      return Error(404, "session_not_found", "invited session does not exist");
    }
    if (!body || !body->is_object() || !body->contains("expected_revision")) {
      return Error(400, "expected_revision_required", "expected invite revision is required");
    }
    if (!expected) expected = NonNegativeInteger(body->at("expected_revision"));
    if (!expected) return Error(400, "invalid_revision", "expected invite revision is invalid");
    const std::int64_t revision = found->second.value("revision", std::int64_t{1});
    if (*expected != revision) return Error(412, "revision_mismatch", "invite revision changed");
    std::int64_t next_revision = 0;
    if (!CheckedAddInt64(revision, 1, next_revision)) {
      return Error(409, "invite_revision_overflow", "invite revision cannot advance");
    }
    DurableData staged = CaptureDurableLocked();
    staged.invites.at(id)["acknowledged"] = true;
    staged.invites.at(id)["revision"] = next_revision;
    const json accepted_invite = staged.invites.at(id);
    const json invited_session = session->second;
    const json response = {{"invite", accepted_invite}, {"session", invited_session}};
    if (receipt_key) {
      staged.invite_accept_receipts[*receipt_key] =
          InviteAcceptReceipt{identity.xuid, id, *idempotency_key, *request_digest, response};
    }
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.body = response};
  }

  Response AllocateStatSequence(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !HasExactKeys(*body, {"session_id"}) ||
        !body->at("session_id").is_string()) {
      return Error(400, "invalid_stats_sequence", "stat sequence request is malformed");
    }
    const std::string session_id = Lower(body->at("session_id").get<std::string>());
    if (!IsHex(session_id, 16)) {
      return Error(400, "invalid_stats_sequence", "stat sequence session is malformed");
    }

    std::lock_guard lock(mutex_);
    const auto session = sessions_.find(session_id);
    if (session == sessions_.end() || !SessionContainsXuid(session->second, identity.xuid)) {
      return Error(403, "invalid_stats_session", "stat writer is not a session member");
    }
    const std::string owner_key = StatSequenceOwnerKey(identity.xuid, session_id);
    const auto found = stat_next_sequences_.find(owner_key);
    const std::uint64_t sequence = found == stat_next_sequences_.end() ? 1 : found->second;
    std::uint64_t next_sequence = 0;
    if (!CheckedAddUint64(sequence, 1, next_sequence)) {
      return Error(409, "stats_sequence_exhausted", "stat sequence space is exhausted");
    }
    DurableData staged = CaptureDurableLocked();
    staged.stat_next_sequences[owner_key] = next_sequence;
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.status = 201, .body = {{"sequence", std::to_string(sequence)}}};
  }

  Response WriteStats(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("session_id") ||
        !body->at("session_id").is_string() || !body->contains("sequence") ||
        !body->at("sequence").is_string() || !body->contains("views") ||
        !body->at("views").is_array() || body->at("views").empty() ||
        body->at("views").size() > kMaximumStatViews) {
      return Error(400, "invalid_stats", "stat write is malformed");
    }
    const std::string session_id = Lower(body->at("session_id").get<std::string>());
    if (!IsHex(session_id, 16)) {
      return Error(400, "invalid_stats", "stat session id is malformed");
    }
    if (body->contains("mode") &&
        (!body->at("mode").is_string() ||
         !ValidIdentifier(body->at("mode").get<std::string>(), 64))) {
      return Error(400, "invalid_stats", "stat mode metadata is malformed");
    }
    const std::string sequence = body->at("sequence").get<std::string>();
    const auto parsed_sequence = ParseDecimalUint64(sequence);
    if (!parsed_sequence || !*parsed_sequence) {
      return Error(400, "invalid_stats", "stat sequence is malformed");
    }
    const auto request_digest = Sha256Hex(body->dump());
    if (!request_digest) return Error(500, "digest_failure", "stat request could not be hashed");

    struct PendingStatRow {
      std::string view_id;
      json columns;
    };
    std::vector<PendingStatRow> pending;
    bool platform_report = false;
    std::unordered_set<std::string> pending_views;
    std::optional<std::string> target_xuid;
    for (const auto& view : body->at("views")) {
      if (!view.is_object() || !view.contains("view_id") || !view.at("view_id").is_string() ||
          !view.contains("rows") || !view.at("rows").is_array() ||
          view.at("rows").size() != 1) {
        return Error(400, "invalid_stats", "stat view is malformed");
      }
      const std::string view_id = Lower(view.at("view_id").get<std::string>());
      const auto parsed_view_id = ParseHex32(view_id);
      if (!parsed_view_id) return Error(400, "invalid_stats", "stat view id is malformed");
      platform_report = *parsed_view_id == UINT32_C(0xFFFF0000);
      if (platform_report && body->at("views").size() != 1) {
        return Error(400, "invalid_stats", "platform reports cannot mix with title views");
      }
      for (const auto& row : view.at("rows")) {
        if (!row.is_object() || !row.contains("xuid") || !row.at("xuid").is_string() ||
            !row.contains("columns") || !row.at("columns").is_object() ||
            row.at("columns").empty() ||
            row.at("columns").size() > kMaximumStatColumnsPerRow ||
            !pending_views.insert(view_id).second) {
          return Error(400, "invalid_stats", "stat row is malformed");
        }
        const std::string row_xuid = row.at("xuid").get<std::string>();
        if (!IsHex(row_xuid, 16) || row_xuid == "0x0000000000000000" ||
            (target_xuid && Lower(*target_xuid) != Lower(row_xuid))) {
          return Error(400, "invalid_stats", "stat row target is malformed");
        }
        target_xuid = row_xuid;
        json normalized_columns = json::object();
        for (const auto& [stat_id_value, stat_value] : row.at("columns").items()) {
          const std::string stat_id = Lower(stat_id_value);
          const auto parsed_stat_id = ParseHex32(stat_id);
          const auto* descriptor = parsed_stat_id
                                       ? libserver::FindGta4StatField(*parsed_view_id,
                                                                     *parsed_stat_id)
                                       : nullptr;
          const libserver::Gta4StatFieldDescriptor platform_descriptor{
              UINT32_C(0xFFFF0000), parsed_stat_id.value_or(0),
              libserver::StatWireType::kInt32, libserver::StatAggregation::kLast};
          const bool known_platform_field = platform_report && parsed_stat_id &&
              (*parsed_stat_id == UINT32_C(0x1000800A) ||
               *parsed_stat_id == UINT32_C(0x1000800B));
          if (known_platform_field) descriptor = &platform_descriptor;
          if (!descriptor || !StatValueMatchesDescriptor(stat_value, *descriptor)) {
            return Error(400, "invalid_stats", "stat column is malformed");
          }
          if (normalized_columns.contains(stat_id)) {
            return Error(400, "invalid_stats", "duplicate numeric property id");
          }
          normalized_columns[stat_id] = stat_value;
        }
        if (platform_report && normalized_columns.size() != 2) {
          return Error(400, "invalid_stats", "platform report requires both generated properties");
        }
        pending.push_back({.view_id = view_id, .columns = std::move(normalized_columns)});
      }
    }

    std::lock_guard lock(mutex_);
    const std::string owner_key = StatSequenceOwnerKey(identity.xuid, session_id);
    const std::string receipt_key =
        StatWriteReceiptKey(identity.xuid, session_id, *parsed_sequence);
    const auto existing_receipt = stat_write_receipts_.find(receipt_key);
    if (existing_receipt != stat_write_receipts_.end()) {
      if (existing_receipt->second.request_digest != *request_digest) {
        return Error(409, "stats_sequence_conflict",
                     "stat sequence was already committed with another request");
      }
      return {.status = existing_receipt->second.status};
    }
    const auto session = sessions_.find(session_id);
    if (session == sessions_.end() || !SessionLiveLocked(session_id, Clock::now())) {
      return Error(403, "invalid_stats_session", "stat session does not exist");
    }
    if (!SessionContainsXuid(session->second, identity.xuid)) {
      return Error(403, "invalid_stats_session", "stat writer is not a session member");
    }
    const auto target_member =
        !target_xuid
            ? session->second.at("members").end()
            : std::find_if(session->second.at("members").begin(),
                           session->second.at("members").end(),
                           [&](const json& member) {
                             return Lower(member.value("xuid", "")) ==
                                    Lower(*target_xuid);
                           });
    if (!target_xuid || target_member == session->second.at("members").end()) {
      return Error(403, "invalid_stats_target", "stat target is not a session member");
    }
    *target_xuid = target_member->value("xuid", "");
    if (Lower(*target_xuid) != Lower(identity.xuid) &&
        Lower(session->second.value("host_xuid", "")) != Lower(identity.xuid)) {
      return Error(403, "invalid_stats_writer",
                   "only the session host may write another member's stats");
    }
    if (!SessionMetadataMatches(*body, session->second)) {
      return Error(409, "stats_policy_mismatch",
                   "stat session policy metadata does not match the live session");
    }
    const auto allocated = stat_next_sequences_.find(owner_key);
    if (allocated == stat_next_sequences_.end() || *parsed_sequence >= allocated->second) {
      return Error(409, "stats_sequence_not_allocated",
                   "stat sequence was not allocated by this session writer");
    }

    if (!platform_report && session->second.value("ranked", false)) {
      const auto arb = arbitration_snapshots_.find(session_id);
      const auto lifecycle = session->second.value("lifecycle_state", std::uint32_t{0});
      if (arb == arbitration_snapshots_.end() || arb->second.failed ||
          arb->second.registered_machine_ids != arb->second.expected_machine_ids ||
          !arb->second.authorized_xuid_machines.contains(identity.xuid) ||
          !arb->second.authorized_xuid_machines.contains(*target_xuid) ||
          !arb->second.registered_xuids.contains(identity.xuid) ||
          !arb->second.registered_xuids.contains(*target_xuid) ||
          !ranked_started_.contains(session_id) || (lifecycle != 2 && lifecycle != 3)) {
        return Error(409, "ranked_match_not_reportable",
                     "ranked title results require a registered match that has started");
      }
    }
    DurableData staged = CaptureDurableLocked();
    if (platform_report) {
      // Generated .77 sub_829F1D88 submits these two Int32s via .80
      // sub_82A373B8. They are a session-scoped platform report, not XLAST
      // title leaderboard columns. Preserve exact typed values and receipt;
      // do not invent a rating formula or change cash/rank.
      json replacement = session->second;
      replacement["platform_stat_reports"][*target_xuid] = pending.front().columns;
      staged.stat_write_receipts[receipt_key] = StatWriteReceipt{*request_digest, 204};
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      session->second = std::move(replacement);
      return {.status = 204};
    }
    // XSessionWriteStats is also issued by title code in non-ranked sessions,
    // but Player Match/Free Mode results must not enter the persistent ranked
    // store. The live session flags are authoritative; client-supplied labels
    // are intentionally not trusted. Preserve the sequence receipt so retries
    // remain idempotent without mutating cash, rank, or leaderboards.
    if (!session->second.value("ranked", false)) {
      staged.stat_write_receipts[receipt_key] = StatWriteReceipt{*request_digest, 204};
      if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
      return {.status = 204};
    }
    auto& player_stats = staged.stats[*target_xuid];
    for (const auto& row : pending) {
      json merged = json::object();
      const auto current_view = player_stats.find(row.view_id);
      if (current_view != player_stats.end() && current_view->second.is_object()) {
        merged = current_view->second;
      }
      const auto parsed_view_id = ParseHex32(row.view_id);
      if (!parsed_view_id) return Error(409, "stored_stats_invalid", "stored stat view is invalid");
      for (const auto& [stat_id, incoming] : row.columns.items()) {
        const auto parsed_stat_id = ParseHex32(stat_id);
        const auto* descriptor = parsed_stat_id
                                     ? libserver::FindGta4StatField(*parsed_view_id,
                                                                   *parsed_stat_id)
                                     : nullptr;
        if (!descriptor) return Error(409, "stored_stats_invalid", "stored stat schema is invalid");
        const json* current = merged.contains(stat_id) ? &merged.at(stat_id) : nullptr;
        if (current && !StatValueMatchesDescriptor(*current, *descriptor)) {
          return Error(409, "stored_stats_invalid", "stored stat column is invalid");
        }
        auto value = MergeStatValue(*descriptor, current, incoming);
        if (!value) return Error(400, "stats_overflow", "stat aggregation overflowed");
        merged[stat_id] = std::move(*value);
      }
      player_stats[row.view_id] = std::move(merged);
    }

    const auto money_view = player_stats.find("0x0000006d");
    if (money_view != player_stats.end() && money_view->second.contains("0x2000000d")) {
      std::int64_t cash = 0;
      if (!JsonInt64(money_view->second.at("0x2000000d").at("value"), cash)) {
        return Error(409, "stored_stats_invalid", "stored ranked cash is invalid");
      }
      const std::uint32_t rank = RankForCash(cash);
      money_view->second["0x10008001"] = {{"type", "i32"}, {"value", rank}};
      staged.progression[*target_xuid] =
          ProgressionRecord{cash, rank, UtcIsoAfter()};
    }
    if (*target_xuid == identity.xuid) {
      staged.player_names[*target_xuid] = identity.player_name;
    }
    staged.stat_write_receipts[receipt_key] = StatWriteReceipt{*request_digest, 204};
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.status = 204};
  }

  Response ReadStats(const Request& request) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("view_id") ||
        !body->at("view_id").is_string() || !body->contains("stat_ids") ||
        !body->at("stat_ids").is_array() || !body->contains("xuids") ||
        !body->at("xuids").is_array() || body->at("xuids").empty() ||
        body->at("xuids").size() > kMaximumStatReadXuids ||
        body->at("stat_ids").size() > kMaximumStatColumnsPerRow) {
      return Error(400, "invalid_stats", "stat read is malformed");
    }
    const std::string view_id = Lower(body->at("view_id").get<std::string>());
    const auto parsed_view_id = ParseHex32(view_id);
    if (!parsed_view_id ||
        !libserver::FindGta4StatAttributeById(*parsed_view_id, UINT16_C(0xFFFE))) {
      return Error(400, "invalid_stats", "stat read view is malformed");
    }
    const json requested = body->value("stat_ids", json::array());
    struct RequestedStat {
      std::string attribute_key;
      std::string property_key;
      std::uint32_t property_id = 0;
      std::uint16_t attribute_id = 0;
    };
    std::vector<RequestedStat> requested_ids;
    std::unordered_set<std::uint16_t> unique_ids;
    for (const auto& value : requested) {
      if (!value.is_string()) return Error(400, "invalid_stats", "stat read column is malformed");
      const std::string id = Lower(value.get<std::string>());
      const auto parsed_id = ParseHex32(id);
      if (!parsed_id || *parsed_id > std::numeric_limits<std::uint16_t>::max()) {
        return Error(400, "invalid_stats", "stat read column is malformed");
      }
      const std::uint16_t attribute_id = static_cast<std::uint16_t>(*parsed_id);
      const auto* mapping =
          libserver::FindGta4StatAttributeById(*parsed_view_id, attribute_id);
      if (!mapping || !unique_ids.insert(attribute_id).second) {
        return Error(400, "invalid_stats", "stat read column is malformed");
      }
      requested_ids.push_back(
          {id, Hex32Lower(mapping->property_id), mapping->property_id, attribute_id});
    }
    std::vector<std::string> requested_xuids;
    requested_xuids.reserve(body->at("xuids").size());
    std::unordered_set<std::string> unique_xuids;
    for (const auto& xuid_value : body->at("xuids")) {
      if (!xuid_value.is_string()) {
        return Error(400, "invalid_stats", "stat read xuid is malformed");
      }
      const std::string xuid = Lower(xuid_value.get<std::string>());
      if (!IsHex(xuid, 16) || xuid == "0x0000000000000000" ||
          !unique_xuids.insert(xuid).second) {
        return Error(400, "invalid_stats", "stat read xuid is malformed");
      }
      requested_xuids.push_back(xuid);
    }
    json rows = json::array();
    std::lock_guard lock(mutex_);
    const auto ranked_candidates = BuildRankedStatCandidates(stats_, *parsed_view_id);
    if (!ranked_candidates) {
      return Error(500, "invalid_stats_state", "leaderboard rating state is invalid");
    }
    std::unordered_map<std::string, std::pair<std::uint32_t, std::int64_t>> rank_by_xuid;
    for (std::size_t index = 0; index < ranked_candidates->size(); ++index) {
      rank_by_xuid.emplace(ranked_candidates->at(index).xuid,
                           std::pair{static_cast<std::uint32_t>(index) + 1,
                                     ranked_candidates->at(index).rating});
    }
    for (const std::string& xuid : requested_xuids) {
      json columns = json::array();
      const auto player = stats_.find(xuid);
      const json* stored_view =
          player != stats_.end() && player->second.contains(view_id)
              ? &player->second.at(view_id)
              : nullptr;
      for (const RequestedStat& stat : requested_ids) {
        // Retail GTA reads 0xFFFE from GtaStatsRow::rating and does not
        // advance the columns pointer for that intrinsic attribute.
        if (stat.attribute_id == UINT16_C(0xFFFE)) continue;
        json value = {{"type", "unset"}};
        if (stored_view) {
          if (stored_view->contains(stat.property_key)) {
            value = stored_view->at(stat.property_key);
          } else if (stat.property_id == UINT32_C(0x10008001) &&
                     stored_view->contains("0x2000000d")) {
            std::int64_t cash = 0;
            if (JsonInt64(stored_view->at("0x2000000d").at("value"), cash)) {
              value = {{"type", "i32"}, {"value", RankForCash(cash)}};
            }
          }
        }
        // GTA advances one 24-byte column for every requested non-rating
        // attribute, including XUSER_DATA_TYPE_UNSET values.
        columns.push_back({{"stat_id", stat.attribute_key}, {"value", std::move(value)}});
      }
      const auto ranked = rank_by_xuid.find(xuid);
      rows.push_back({{"xuid", xuid},
                      {"rank", ranked == rank_by_xuid.end() ? 0 : ranked->second.first},
                      {"rating", ranked == rank_by_xuid.end() ? 0 : ranked->second.second},
                      {"player_name", player_names_.contains(xuid) ? player_names_.at(xuid) : "Player"},
                      {"columns", std::move(columns)}});
    }
    return {.body = {{"rows", std::move(rows)}}};
  }

  Response ResetStats(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("view_id") ||
        !body->at("view_id").is_string()) {
      return Error(400, "invalid_json", "invalid JSON body");
    }
    const std::string view_id = Lower(body->at("view_id").get<std::string>());
    if (!IsHex(view_id, 8)) return Error(400, "invalid_stats", "stat view id is malformed");
    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    const auto player = staged.stats.find(identity.xuid);
    if (player != staged.stats.end()) player->second.erase(view_id);
    if (view_id == "0x0000006d") staged.progression.erase(identity.xuid);
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.status = 204};
  }

  Response Skill(const Request& request) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    return Error(501, "skill_contract_unavailable",
                 "retail skill parameters are not established; no rating was mutated");
  }

  Response RankedResult(const Request& request, const Identity& identity) {
    (void)identity;
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("result_id") ||
        !body->at("result_id").is_string()) {
      return Error(400, "invalid_result", "ranked result is malformed");
    }
    return Error(501, "nonretail_progression_disabled",
                 "custom progression results are disabled; use retail stat writes");
#if 0
    const std::string result_id = body->at("result_id").get<std::string>();
    const std::string canonical_request = body->dump();
    {
      std::lock_guard lock(mutex_);
      if (ranked_results_.contains(result_id)) {
        const auto stored_request = ranked_result_requests_.find(result_id);
        if (stored_request == ranked_result_requests_.end() ||
            stored_request->second != canonical_request) {
          return Error(409, "result_id_conflict", "result id was already used by another request");
        }
        return {.status = 201, .body = ranked_results_.at(result_id)};
      }
    }
    if (!body->contains("rows") ||
        !body->at("rows").is_array() || body->at("rows").empty() ||
        body->at("rows").size() > kMaximumSessionMembers ||
        !body->contains("session_id") || !body->at("session_id").is_string() ||
        !body->contains("mode") || !body->at("mode").is_string() ||
        !body->contains("expected_revision") || !body->contains("expected_host_epoch")) {
      return Error(400, "invalid_result", "ranked result is malformed");
    }
    const std::string session_id = Lower(body->at("session_id").get<std::string>());
    const std::string mode = body->at("mode").get<std::string>();
    if (result_id.empty() || !IsHex(session_id, 16) || !ValidIdentifier(mode, 64)) {
      return Error(400, "invalid_result", "ranked result identity is missing");
    }
    std::int64_t expected_revision = 0;
    std::int64_t expected_host_epoch = 0;
    if (!JsonInt64(body->at("expected_revision"), expected_revision) ||
        !JsonInt64(body->at("expected_host_epoch"), expected_host_epoch)) {
      return Error(400, "invalid_result", "ranked result revision is malformed");
    }

    struct RankedInputRow {
      std::string xuid;
      std::int64_t cash_delta = 0;
      std::int64_t score = 0;
      std::uint32_t kills = 0;
      std::uint32_t deaths = 0;
      bool won = false;
    };
    std::vector<RankedInputRow> rows;
    rows.reserve(body->at("rows").size());
    for (const auto& row : body->at("rows")) {
      if (!row.is_object() || !row.contains("xuid") || !row.at("xuid").is_string() ||
          !row.contains("cash_delta") || !row.contains("score") || !row.contains("kills") ||
          !row.contains("deaths") || !row.contains("won") || !row.at("won").is_boolean()) {
        return Error(400, "invalid_result_row", "ranked result row is malformed");
      }
      RankedInputRow parsed{.xuid = row.at("xuid").get<std::string>(),
                            .won = row.at("won").get<bool>()};
      if (!IsHex(parsed.xuid, 16) || !JsonInt64(row.at("cash_delta"), parsed.cash_delta) ||
          parsed.cash_delta < 0 || !JsonInt64(row.at("score"), parsed.score) ||
          !JsonUint32(row.at("kills"), parsed.kills) ||
          !JsonUint32(row.at("deaths"), parsed.deaths)) {
        return Error(400, "invalid_result_row", "ranked result row is invalid");
      }
      rows.push_back(std::move(parsed));
    }

    std::lock_guard lock(mutex_);
    if (ranked_results_.contains(result_id)) {
      const auto stored_request = ranked_result_requests_.find(result_id);
      if (stored_request == ranked_result_requests_.end() ||
          stored_request->second != canonical_request) {
        return Error(409, "result_id_conflict", "result id was already used by another request");
      }
      return {.status = 201, .body = ranked_results_.at(result_id)};
    }
    const auto session_it = sessions_.find(session_id);
    if (session_it == sessions_.end()) return Error(404, "session_not_found", "ranked session does not exist");
    const json& session = session_it->second;
    if (session.value("host_xuid", "") != identity.xuid || !session.value("ranked", false) ||
        expected_revision != session.value("revision", std::int64_t{0}) ||
        expected_host_epoch != session.value("host_epoch", std::int64_t{1}) ||
        !SessionMetadataMatches(*body, session)) {
      return Error(403, "invalid_ranked_host", "ranked result host or revision is invalid");
    }
    std::unordered_set<std::string> members;
    for (const auto& member : session["members"]) members.insert(member.value("xuid", ""));
    if (!members.contains(identity.xuid)) {
      return Error(403, "invalid_ranked_host", "ranked result host is not a session member");
    }
    std::unordered_set<std::string> seen;
    json updated = json::array();
    std::unordered_map<std::string, ProgressionRecord> staged_progression;
    std::unordered_map<std::string, json> staged_aggregates;
    const std::string updated_at = UtcIsoAfter();
    for (const auto& row : rows) {
      if (!members.contains(row.xuid) || !seen.insert(row.xuid).second) {
        return Error(400, "invalid_result_row", "ranked result row is invalid");
      }
      ProgressionRecord record;
      const auto current_progression = progression_.find(row.xuid);
      if (current_progression != progression_.end()) record = current_progression->second;
      const auto current_stats = stats_.find(row.xuid);
      if (current_stats != stats_.end() && current_stats->second.contains("0x0000006d") &&
          current_stats->second.at("0x0000006d").contains("0x2000000d")) {
        if (!JsonInt64(current_stats->second.at("0x0000006d")
                           .at("0x2000000d")
                           .at("value"),
                       record.cash)) {
          return Error(409, "invalid_ranked_cash", "stored ranked cash is malformed");
        }
      }
      std::int64_t new_cash = 0;
      if (!CheckedAddInt64(record.cash, row.cash_delta, new_cash)) {
        return Error(400, "cash_overflow", "ranked cash total overflowed");
      }
      record.cash = new_cash;
      record.rank = RankForCash(record.cash);
      record.updated_at = updated_at;

      std::int64_t games = 0;
      std::int64_t wins = 0;
      std::int64_t score = 0;
      std::uint64_t kills = 0;
      std::uint64_t deaths = 0;
      const auto player_modes = mode_stats_.find(row.xuid);
      if (player_modes != mode_stats_.end()) {
        const auto current_mode = player_modes->second.find(mode);
        if (current_mode != player_modes->second.end()) {
          const json& aggregate = current_mode->second;
          if (!aggregate.is_object() ||
              (aggregate.contains("games") && !JsonInt64(aggregate.at("games"), games)) ||
              (aggregate.contains("wins") && !JsonInt64(aggregate.at("wins"), wins)) ||
              (aggregate.contains("score") && !JsonInt64(aggregate.at("score"), score)) ||
              (aggregate.contains("kills") && !JsonUint64(aggregate.at("kills"), kills)) ||
              (aggregate.contains("deaths") && !JsonUint64(aggregate.at("deaths"), deaths))) {
            return Error(409, "invalid_mode_stats", "stored mode statistics are malformed");
          }
        }
      }

      std::int64_t next_games = 0;
      std::int64_t next_wins = 0;
      std::int64_t next_score = 0;
      std::uint64_t next_kills = 0;
      std::uint64_t next_deaths = 0;
      if (!CheckedAddInt64(games, 1, next_games) ||
          !CheckedAddInt64(wins, row.won ? 1 : 0, next_wins) ||
          !CheckedAddInt64(score, row.score, next_score) ||
          !CheckedAddUint64(kills, row.kills, next_kills) ||
          !CheckedAddUint64(deaths, row.deaths, next_deaths)) {
        return Error(400, "mode_stats_overflow", "ranked mode statistics overflowed");
      }
      staged_progression.emplace(row.xuid, record);
      staged_aggregates.emplace(row.xuid,
                                json{{"games", next_games}, {"wins", next_wins},
                                     {"score", next_score}, {"kills", next_kills},
                                     {"deaths", next_deaths}});
      updated.push_back(ProgressionJson(row.xuid, record));
    }
    json response = {{"result_id", result_id}, {"progression", std::move(updated)}};
    DurableData durable_candidate = CaptureDurableLocked();
    for (auto& [xuid, record] : staged_progression) {
      durable_candidate.stats[xuid]["0x0000006d"]["0x2000000d"] =
          {{"type", "i64"}, {"value", record.cash}};
      durable_candidate.stats[xuid]["0x0000006d"]["0x10008001"] =
          {{"type", "i32"}, {"value", record.rank}};
      durable_candidate.progression[xuid] = std::move(record);
    }
    for (auto& [xuid, aggregate] : staged_aggregates) {
      durable_candidate.mode_stats[xuid][mode] = std::move(aggregate);
    }
    durable_candidate.ranked_results[result_id] = response;
    durable_candidate.ranked_result_requests[result_id] = canonical_request;
    if (!SaveAndCommitLocked(std::move(durable_candidate))) return StorageFailure();
    return {.status = 201, .body = std::move(response)};
#endif
  }

  Response ProgressionGet(const std::string& xuid) {
    if (!IsHex(xuid, 16)) return Error(400, "invalid_xuid", "progression xuid is invalid");
    std::lock_guard lock(mutex_);
    const auto found = progression_.find(xuid);
    const ProgressionRecord empty;
    return {.body = ProgressionJson(xuid, found == progression_.end() ? empty : found->second)};
  }

  Response ProfilePut(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->contains("appearance") || !(*body)["appearance"].is_object()) {
      return Error(400, "invalid_profile", "profile appearance is malformed");
    }
    const std::string episode = body->value("episode", "");
    const json appearance = (*body)["appearance"];
    const std::string gender = appearance.value("gender", "");
    const std::string model = appearance.value("model", "");
    const json clothing = appearance.value("clothing", json::array());
    if (!ValidEpisode(episode) ||
        !HasExactKeys(appearance, {"gender", "model", "clothing"}) ||
        (gender != "male" && gender != "female") || model.empty() ||
        !clothing.is_array() || clothing.size() > kMaximumProfileClothingItems) {
      return Error(400, "invalid_profile", "profile fields are invalid");
    }
    for (const auto& item : clothing) {
      if (!item.is_string() || item.get<std::string>().empty()) {
        return Error(400, "invalid_profile", "clothing item is invalid");
      }
    }
    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    json& profile = staged.profiles[identity.xuid];
    if (profile.is_null()) {
      profile = {{"xuid", identity.xuid}, {"player_name", identity.player_name},
                 {"appearances", json::object()}};
    }
    profile["player_name"] = identity.player_name;
    profile["appearances"][episode] = appearance;
    profile["updated_at"] = UtcIsoAfter();
    const json response = profile;
    staged.player_names[identity.xuid] = identity.player_name;
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.body = response};
  }

  Response ProfileGet(const std::string& xuid) {
    if (!IsHex(xuid, 16)) return Error(400, "invalid_xuid", "profile xuid is invalid");
    std::lock_guard lock(mutex_);
    const auto found = profiles_.find(xuid);
    if (found != profiles_.end()) return {.body = found->second};
    if (!player_names_.contains(xuid)) return Error(404, "profile_not_found", "profile does not exist");
    return {.body = {{"xuid", xuid}, {"player_name", player_names_.at(xuid)},
                     {"appearances", json::object()}, {"updated_at", ""}}};
  }

  Response TitleProfileSetting(const Request& request, const Identity& identity) {
    if (request.method == "GET") {
      std::lock_guard lock(mutex_);
      const auto found = title_profile_settings_.find(identity.xuid);
      if (found == title_profile_settings_.end()) {
        return Error(404, "title_setting_not_found", "title profile setting does not exist");
      }
      return {.body = TitleProfileSettingJson(found->second)};
    }
    if (request.method != "PUT") {
      return Error(404, "not_found", "title profile setting operation does not exist");
    }
    const auto body = ParseBody(request);
    if (!body || !HasExactKeys(*body, {"title_id", "setting_id", "expected_revision", "blob"}) ||
        body->value("title_id", "") != "0x545407f2" ||
        body->value("setting_id", "") != "0x63e83fff" ||
        !body->at("expected_revision").is_string() || !body->at("blob").is_string()) {
      return Error(400, "invalid_title_setting", "title profile setting is malformed");
    }
    const auto expected_revision =
        ParseDecimalUint64(body->at("expected_revision").get<std::string>());
    const std::string blob = body->at("blob").get<std::string>();
    const auto decoded = DecodeBase64Url(blob);
    if (!expected_revision || !decoded || !IsValidGta4TitleProfileBlob(*decoded)) {
      return Error(400, "invalid_title_setting", "title profile setting payload is invalid");
    }
    const auto digest = Sha256Hex(std::string_view(
        reinterpret_cast<const char*>(decoded->data()), decoded->size()));
    if (!digest) return Error(500, "digest_failure", "title profile setting could not be hashed");

    std::lock_guard lock(mutex_);
    const auto found = title_profile_settings_.find(identity.xuid);
    const std::uint64_t current_revision =
        found == title_profile_settings_.end() ? 0 : found->second.revision;
    if (*expected_revision != current_revision) {
      return Error(412, "revision_mismatch", "title profile setting revision changed");
    }
    std::uint64_t next_revision = 0;
    if (!CheckedAddUint64(current_revision, 1, next_revision)) {
      return Error(409, "revision_exhausted", "title profile setting revision is exhausted");
    }
    TitleProfileSettingRecord record{
        next_revision, blob, *digest, UtcIsoAfter()};
    DurableData staged = CaptureDurableLocked();
    staged.title_profile_settings[identity.xuid] = record;
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return {.body = TitleProfileSettingJson(record)};
  }

  Response ProgAchStorage(const Request& request, const Identity& identity) {
    if (!request.query.empty()) {
      return Error(400, "invalid_storage_request", "Prog_ACH does not accept query fields");
    }
    const auto response = [](const ProgAchRecord& record) {
      return Response{.body = {{"title_id", "0x545407F2"},
                               {"facility", 3},
                               {"path", "Prog_ACH"},
                               {"blob", record.blob}}};
    };
    if (request.method == "GET") {
      std::lock_guard lock(mutex_);
      const auto found = prog_ach_records_.find(Lower(identity.xuid));
      if (found == prog_ach_records_.end()) {
        return Error(404, "storage_file_not_found", "Prog_ACH does not exist");
      }
      return response(found->second);
    }

    const auto body = ParseBody(request);
    if (!body || !HasExactKeys(*body, {"blob"}) || !body->at("blob").is_string()) {
      return Error(400, "invalid_storage_blob", "Prog_ACH body is malformed");
    }
    const std::string blob = body->at("blob").get<std::string>();
    const auto decoded = DecodeCanonicalProgAchBlob(blob);
    if (!decoded) {
      return Error(400, "invalid_storage_blob", "Prog_ACH must contain exactly 604 bytes");
    }
    const auto digest = Sha256Hex(std::string_view(
        reinterpret_cast<const char*>(decoded->data()), decoded->size()));
    if (!digest) return Error(500, "digest_failure", "Prog_ACH could not be hashed");
    ProgAchRecord record{blob, *digest, UtcIsoAfter()};
    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    staged.prog_ach_records[Lower(identity.xuid)] = record;
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return response(record);
  }

  static Response AchievementResponse(
      const std::unordered_set<std::uint32_t>& unlocked) {
    std::vector<std::uint32_t> ordered(unlocked.begin(), unlocked.end());
    std::ranges::sort(ordered);
    return {.body = {{"achievement_ids", std::move(ordered)}}};
  }

  Response Achievements(const Request& request, const Identity& identity) {
    if (request.method == "GET") {
      std::lock_guard lock(mutex_);
      const auto found = achievements_.find(identity.xuid);
      return AchievementResponse(found == achievements_.end()
                                     ? std::unordered_set<std::uint32_t>{}
                                     : found->second);
    }
    if (request.method != "POST") {
      return Error(404, "not_found", "achievement operation does not exist");
    }

    const auto body = ParseBody(request);
    if (!body || !HasExactKeys(*body, {"achievement_ids"}) ||
        !body->at("achievement_ids").is_array() ||
        body->at("achievement_ids").size() > kGta4AchievementCount) {
      return Error(400, "invalid_achievements", "achievement payload is malformed");
    }
    std::unordered_set<std::uint32_t> requested;
    for (const auto& wire_id : body->at("achievement_ids")) {
      if (!wire_id.is_number_unsigned()) {
        return Error(400, "invalid_achievement_id", "achievement id is invalid");
      }
      const std::uint64_t raw_id = wire_id.get<std::uint64_t>();
      if (raw_id < kFirstGta4AchievementId || raw_id > kLastGta4AchievementId) {
        return Error(400, "invalid_achievement_id", "achievement id is invalid");
      }
      const std::uint32_t id = static_cast<std::uint32_t>(raw_id);
      if (!requested.insert(id).second) {
        return Error(400, "invalid_achievement_id", "achievement id is invalid");
      }
    }

    std::lock_guard lock(mutex_);
    DurableData staged = CaptureDurableLocked();
    auto& unlocked = staged.achievements[identity.xuid];
    const std::size_t previous_size = unlocked.size();
    unlocked.insert(requested.begin(), requested.end());
    if (unlocked.size() == previous_size) return AchievementResponse(unlocked);
    const auto response = AchievementResponse(unlocked);
    if (!SaveAndCommitLocked(std::move(staged))) return StorageFailure();
    return response;
  }

  Response Entitlements(const Request& request, const Identity& identity) {
    if (request.method != "GET") {
      return Error(404, "not_found", "entitlement operation does not exist");
    }
    std::lock_guard lock(mutex_);
    std::vector<std::string> packages;
    const auto owner = entitlements_.find(Lower(identity.xuid));
    if (owner != entitlements_.end()) {
      packages.assign(owner->second.begin(), owner->second.end());
      std::ranges::sort(packages);
    }
    return {.body = {{"packages", std::move(packages)}}};
  }

  Response PresencePut(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body) return Error(400, "invalid_json", "invalid JSON body");
    const std::string state = body->value("state", "");
    const std::string session_id = body->value("session_id", "");
    const std::string episode = body->value("episode", "");
    if ((state != "online" && state != "in_game" && state != "away") ||
        !ValidEpisode(episode) || (!session_id.empty() && !IsHex(session_id, 16)) ||
        (state == "in_game") != !session_id.empty()) {
      return Error(400, "invalid_presence", "presence fields are invalid");
    }
    PresenceRecord record{.state = state, .session_id = session_id, .episode = episode,
                          .expires = Clock::now() + kPresenceLease,
                          .expires_at = UtcIsoAfter(kPresenceLease)};
    std::lock_guard lock(mutex_);
    if (!session_id.empty()) {
      const auto session = sessions_.find(session_id);
      if (session == sessions_.end() || !SessionLiveLocked(session_id, Clock::now())) {
        return Error(404, "session_not_found", "presence session does not exist");
      }
      if (!SessionContainsXuid(session->second, identity.xuid)) {
        return Error(403, "not_member", "presence identity is not a session member");
      }
    }
    presence_[identity.xuid] = record;
    return {.body = {{"xuid", identity.xuid}, {"state", record.state},
                     {"session_id", record.session_id}, {"episode", record.episode},
                     {"expires_at", record.expires_at}}};
  }

  Response PresenceGet(const std::string& xuid) {
    if (!IsHex(xuid, 16)) return Error(400, "invalid_xuid", "presence xuid is invalid");
    std::lock_guard lock(mutex_);
    auto found = presence_.find(xuid);
    if (found == presence_.end() || Clock::now() >= found->second.expires) {
      return {.body = {{"xuid", xuid}, {"state", "offline"}, {"session_id", ""},
                       {"episode", ""}, {"expires_at", ""}}};
    }
    if (!found->second.session_id.empty()) {
      const auto session = sessions_.find(found->second.session_id);
      if (session == sessions_.end() ||
          !SessionLiveLocked(found->second.session_id, Clock::now()) ||
          !SessionContainsXuid(session->second, xuid)) {
        presence_.erase(found);
        return {.body = {{"xuid", xuid}, {"state", "offline"}, {"session_id", ""},
                         {"episode", ""}, {"expires_at", ""}}};
      }
    }
    return {.body = {{"xuid", xuid}, {"state", found->second.state},
                     {"session_id", found->second.session_id}, {"episode", found->second.episode},
                     {"expires_at", found->second.expires_at}}};
  }

  Response Leaderboard(const Request& request, const Identity& identity) {
    const std::string view_id =
        Lower(SegmentAfter(request.path, "/api/v2/leaderboards/"));
    const auto parsed_view_id = ParseHex32(view_id);
    if (!parsed_view_id ||
        !libserver::FindGta4StatAttributeById(*parsed_view_id, UINT16_C(0xFFFE))) {
      return Error(400, "invalid_leaderboard", "leaderboard view is malformed");
    }

    struct ProjectedStat {
      std::string attribute_key;
      std::string property_key;
      std::uint16_t attribute_id = 0;
    };
    std::vector<ProjectedStat> projected_stats;
    std::unordered_set<std::uint16_t> unique_attributes;
    if (request.query.contains("stat_id") && request.query.contains("stat_ids")) {
      return Error(400, "invalid_leaderboard", "leaderboard attributes are ambiguous");
    }
    std::vector<std::string> attribute_keys;
    if (request.query.contains("stat_id")) {
      attribute_keys.push_back(Lower(request.query.at("stat_id")));
    } else if (request.query.contains("stat_ids")) {
      const std::string encoded = Lower(request.query.at("stat_ids"));
      std::size_t begin = 0;
      while (begin <= encoded.size()) {
        const std::size_t delimiter = encoded.find(',', begin);
        const std::string item = encoded.substr(
            begin, delimiter == std::string::npos ? std::string::npos : delimiter - begin);
        if (item.empty()) {
          return Error(400, "invalid_leaderboard", "leaderboard attribute is malformed");
        }
        attribute_keys.push_back(item);
        if (delimiter == std::string::npos) break;
        begin = delimiter + 1;
      }
    }
    if (attribute_keys.size() > kMaximumStatColumnsPerRow) {
      return Error(400, "invalid_leaderboard", "too many leaderboard attributes");
    }
    for (const std::string& attribute_key : attribute_keys) {
      const auto parsed_attribute = ParseHex32(attribute_key);
      if (!parsed_attribute ||
          *parsed_attribute > std::numeric_limits<std::uint16_t>::max()) {
        return Error(400, "invalid_leaderboard", "leaderboard attribute is malformed");
      }
      const std::uint16_t attribute_id =
          static_cast<std::uint16_t>(*parsed_attribute);
      const auto* mapping =
          libserver::FindGta4StatAttributeById(*parsed_view_id, attribute_id);
      if (!mapping || !unique_attributes.insert(attribute_id).second) {
        return Error(400, "invalid_leaderboard", "leaderboard attribute is malformed");
      }
      projected_stats.push_back(
          {.attribute_key = attribute_key,
           .property_key = Hex32Lower(mapping->property_id),
           .attribute_id = attribute_id});
    }
    const auto parse_query = [&](std::string_view name,
                                 std::uint32_t fallback) -> std::optional<std::uint32_t> {
      const auto found = request.query.find(std::string(name));
      if (found == request.query.end()) return fallback;
      const auto parsed = ParseDecimalUint64(found->second);
      if (!parsed || *parsed > std::numeric_limits<std::uint32_t>::max()) {
        return std::nullopt;
      }
      return static_cast<std::uint32_t>(*parsed);
    };
    const auto offset = parse_query("offset", 0);
    const auto limit = parse_query("limit", 100);
    if (!offset || !limit || *limit == 0 || *limit > 100) {
      return Error(400, "invalid_pagination", "leaderboard pagination is malformed");
    }
    bool friends_only = false;
    if (const auto found = request.query.find("friends_only");
        found != request.query.end()) {
      if (found->second != "true" && found->second != "false") {
        return Error(400, "invalid_leaderboard", "friends_only must be true or false");
      }
      friends_only = found->second == "true";
    }
    std::lock_guard lock(mutex_);
    const auto ranked_candidates = BuildRankedStatCandidates(stats_, *parsed_view_id);
    if (!ranked_candidates) {
      return Error(500, "invalid_stats_state", "leaderboard rating state is invalid");
    }
    json candidates = json::array();
    const auto relationship_owner = relationships_.find(identity.xuid);
    for (const auto& candidate : *ranked_candidates) {
      const std::string& xuid = candidate.xuid;
      const bool is_friend = relationship_owner != relationships_.end() &&
                             relationship_owner->second.contains(xuid) &&
                             relationship_owner->second.at(xuid) == "accepted";
      if (friends_only && xuid != identity.xuid && !is_friend) continue;
      json columns = json::array();
      for (const ProjectedStat& projected : projected_stats) {
        if (projected.attribute_id == UINT16_C(0xFFFE)) continue;
        json value = {{"type", "unset"}};
        if (candidate.view->contains(projected.property_key)) {
          value = candidate.view->at(projected.property_key);
        }
        columns.push_back(
            {{"stat_id", projected.attribute_key}, {"value", std::move(value)}});
      }
      const auto player_name = player_names_.find(xuid);
      candidates.push_back({{"xuid", xuid},
                            {"player_name", player_name == player_names_.end()
                                                ? "Player"
                                                : player_name->second},
                            {"rank", 0},
                            {"rating", candidate.rating},
                            {"columns", std::move(columns)}});
    }
    json rows = json::array();
    const std::size_t first = std::min<std::size_t>(*offset, candidates.size());
    for (std::size_t index = first;
         index < candidates.size() && rows.size() < *limit; ++index) {
      json row = candidates.at(index);
      row["rank"] = index + 1;
      rows.push_back(std::move(row));
    }
    return {.body = {{"total", candidates.size()}, {"rows", std::move(rows)}}};
  }

  bool RealtimeEventDeliverableLocked(const RealtimeEvent& event,
                                      std::string_view recipient_xuid) const {
    const auto session = sessions_.find(event.session_id);
    return session != sessions_.end() &&
           SessionLiveLocked(event.session_id, Clock::now()) &&
           SessionContainsXuid(session->second, event.source_xuid) &&
           SessionContainsXuid(session->second, recipient_xuid) &&
           !RelationshipBlocksLocked(event.source_xuid, recipient_xuid);
  }

  void PruneRealtimeEventsLocked(const std::string& recipient_xuid,
                                 std::uint64_t acknowledged_cursor) {
    const auto found = realtime_events_.find(recipient_xuid);
    if (found == realtime_events_.end()) return;
    auto& queue = found->second;
    std::erase_if(queue, [&](const RealtimeEvent& event) {
      return event.id <= acknowledged_cursor ||
             !RealtimeEventDeliverableLocked(event, recipient_xuid);
    });
    if (queue.empty()) realtime_events_.erase(found);
  }

  Response ChatMessage(const Request& request, const Identity& identity) {
    const auto idempotency = request.headers.find("idempotency-key");
    if (idempotency == request.headers.end()) {
      return Error(400, "idempotency_key_required",
                   "Idempotency-Key is required for chat delivery");
    }
    return RunSessionMutation(
        request, identity, [&](bool /*pending_retry*/) -> Response {
          const auto body = ParseBody(request);
          if (!body ||
              !HasExactKeys(*body, {"session_id", "channel", "target_xuids",
                                    "sequence", "text"}) ||
              !body->at("session_id").is_string() ||
              !body->at("channel").is_string() ||
              !body->at("target_xuids").is_array() ||
              !body->at("text").is_string() ||
              body->at("target_xuids").size() > kMaximumSessionMembers) {
            return Error(400, "invalid_chat_message",
                         "chat message is malformed");
          }

          const std::string session_id =
              Lower(body->at("session_id").get<std::string>());
          const std::string channel = body->at("channel").get<std::string>();
          const std::string text = body->at("text").get<std::string>();
          const auto sequence = NonNegativeInteger(body->at("sequence"));
          if (!IsHex(session_id, 16) ||
              (channel != "all" && channel != "team") ||
              !sequence ||
              static_cast<std::uint64_t>(*sequence) >
                  std::numeric_limits<std::uint32_t>::max() ||
              text.empty() || text.size() > kMaximumTextChatBytes ||
              text.find_first_not_of(" \t") == std::string::npos ||
              text.find_first_of("\r\n") != std::string::npos ||
              text.find('\0') != std::string::npos) {
            return Error(400, "invalid_chat_message",
                         "chat message fields are invalid");
          }

          const auto session = sessions_.find(session_id);
          if (session == sessions_.end() ||
              !SessionLiveLocked(session_id, Clock::now())) {
            return Error(404, "session_not_found", "session does not exist");
          }
          if (!SessionContainsXuid(session->second, identity.xuid)) {
            return Error(403, "not_member",
                         "chat sender is not a session member");
          }

          std::vector<std::string> recipients;
          std::unordered_set<std::string> unique_recipients;
          for (const auto& target : body->at("target_xuids")) {
            if (!target.is_string()) {
              return Error(400, "invalid_chat_target",
                           "chat target is malformed");
            }
            const std::string xuid = Lower(target.get<std::string>());
            if (!IsHex(xuid, 16) || xuid == identity.xuid ||
                !SessionContainsXuid(session->second, xuid) ||
                !unique_recipients.insert(xuid).second) {
              return Error(400, "invalid_chat_target",
                           "chat target is not a unique remote session member");
            }
            recipients.push_back(xuid);
          }
          if ((channel == "all" && !recipients.empty()) ||
              (channel == "team" && recipients.empty())) {
            return Error(400, "invalid_chat_target",
                         "chat channel target policy is invalid");
          }
          if (channel == "all") {
            for (const auto& member : session->second.at("members")) {
              const std::string xuid = member.value("xuid", "");
              if (xuid != identity.xuid && unique_recipients.insert(xuid).second) {
                recipients.push_back(xuid);
              }
            }
          }

          std::uint64_t event_id = 0;
          if (!CheckedAddUint64(next_realtime_event_id_, 1, event_id) ||
              event_id > static_cast<std::uint64_t>(
                             std::numeric_limits<std::int64_t>::max())) {
            return Error(409, "event_cursor_exhausted",
                         "realtime event cursor space is exhausted");
          }
          next_realtime_event_id_ = event_id;
          const std::string player_name =
              player_names_.contains(identity.xuid)
                  ? player_names_.at(identity.xuid)
                  : identity.player_name;
          const json wire = {
              {"id", static_cast<std::int64_t>(event_id)},
              {"type", "chat.message"},
              {"aggregate_type", "session-chat"},
              {"aggregate_id", session_id},
              {"payload",
               {{"source_xuid", identity.xuid},
                {"session_id", session_id},
                {"channel", channel},
                {"player_name", player_name},
                {"sequence", static_cast<std::uint32_t>(*sequence)},
                {"text", text}}}};
          for (const std::string& recipient : recipients) {
            if (RelationshipBlocksLocked(identity.xuid, recipient)) continue;
            auto& queue = realtime_events_[recipient];
            while (queue.size() >= kMaximumQueuedTextChatEvents) {
              queue.pop_front();
            }
            queue.push_back({.id = event_id,
                             .session_id = session_id,
                             .source_xuid = identity.xuid,
                             .wire = wire});
          }
          realtime_event_condition_.notify_all();
          return {.status = 201,
                  .body = {{"id", static_cast<std::int64_t>(event_id)}}};
        });
  }

  Response RealtimeEvents(const Request& request, const Identity& identity) {
    std::uint64_t after = 0;
    if (const auto found = request.query.find("after");
        found != request.query.end()) {
      const auto parsed = ParseDecimalUint64(found->second);
      if (!parsed ||
          *parsed > static_cast<std::uint64_t>(
                        std::numeric_limits<std::int64_t>::max())) {
        return Error(400, "invalid_event_cursor",
                     "realtime event cursor is malformed");
      }
      after = *parsed;
    }
    std::uint64_t wait_ms = 0;
    if (const auto found = request.query.find("wait_ms");
        found != request.query.end()) {
      const auto parsed = ParseDecimalUint64(found->second);
      if (!parsed || *parsed > kMaximumTextChatWaitMilliseconds) {
        return Error(400, "invalid_event_wait",
                     "realtime event wait is malformed");
      }
      wait_ms = *parsed;
    }

    std::unique_lock lock(mutex_);
    if ((after != 0 && after <= realtime_event_cursor_floor_) ||
        after > next_realtime_event_id_) {
      return Error(409, "event_cursor_reset",
                   "realtime event cursor is ahead of this server");
    }
    PruneRealtimeEventsLocked(identity.xuid, after);
    const auto has_event_after = [&] {
      const auto found = realtime_events_.find(identity.xuid);
      return found != realtime_events_.end() &&
             std::any_of(found->second.begin(), found->second.end(),
                         [&](const RealtimeEvent& event) {
                           return event.id > after;
                         });
    };
    if (wait_ms && !has_event_after() && !realtime_events_stopping_) {
      realtime_event_condition_.wait_for(
          lock, std::chrono::milliseconds(wait_ms), [&] {
            PruneRealtimeEventsLocked(identity.xuid, after);
            return realtime_events_stopping_ || has_event_after();
          });
    }
    if (realtime_events_stopping_) {
      return Error(503, "service_stopping", "realtime event service is stopping");
    }
    PruneRealtimeEventsLocked(identity.xuid, after);
    json events = json::array();
    if (const auto found = realtime_events_.find(identity.xuid);
        found != realtime_events_.end()) {
      for (const RealtimeEvent& event : found->second) {
        if (event.id <= after) continue;
        events.push_back(event.wire);
        if (events.size() >= kMaximumTextChatEventsPerPage) break;
      }
    }
    return {.body = {{"events", std::move(events)}}};
  }

  Response VoiceRoutes(const Request& request, const Identity& identity) {
    const auto body = ParseBody(request);
    if (!body || !body->is_object()) return Error(400, "invalid_json", "invalid JSON body");
    std::lock_guard lock(mutex_);
    if (request.method == "DELETE") {
      if (!body->contains("route_token") || !body->at("route_token").is_string()) {
        return Error(400, "invalid_voice_route", "voice route token is malformed");
      }
      const std::string token = body->value("route_token", "");
      const auto route = voice_routes_.find(token);
      if (route == voice_routes_.end() || route->second.expires <= Clock::now()) {
        if (route != voice_routes_.end()) EraseVoiceRouteLocked(token);
        return Error(404, "route_not_found", "voice route does not exist");
      }
      if (route->second.owner_xuid != identity.xuid) {
        return Error(403, "not_route_owner", "voice route belongs to another identity");
      }
      EraseVoiceRouteLocked(token);
      return {.status = 204};
    }
    if (request.method != "POST" || !body->contains("session_id") ||
        !body->at("session_id").is_string() || !body->contains("channel") ||
        !body->at("channel").is_string() || !body->contains("target_xuids") ||
        !body->at("target_xuids").is_array() || !body->contains("mute_xuids") ||
        !body->at("mute_xuids").is_array() ||
        body->at("target_xuids").size() > kMaximumSessionMembers ||
        body->at("mute_xuids").size() > kMaximumSessionMembers) {
      return Error(400, "invalid_voice_route", "voice route is malformed");
    }
    const std::string session_id = body->at("session_id").get<std::string>();
    const std::string channel = body->at("channel").get<std::string>();
    std::string replaced_token;
    if (body->contains("replace_route_token")) {
      if (!body->at("replace_route_token").is_string()) {
        return Error(400, "invalid_voice_route",
                     "replacement voice route token is malformed");
      }
      replaced_token = body->at("replace_route_token").get<std::string>();
      if (!IsCanonicalVoiceRouteToken(replaced_token)) {
        return Error(400, "invalid_voice_route",
                     "replacement voice route token is malformed");
      }
    }
    if (!IsHex(session_id, 16) ||
        (channel != "all" && channel != "team" && channel != "private")) {
      return Error(400, "invalid_voice_route", "voice route channel or session is invalid");
    }
    const auto session = sessions_.find(session_id);
    if (session == sessions_.end() || !SessionLiveLocked(session_id, Clock::now())) {
      return Error(404, "session_not_found", "session does not exist");
    }
    if (!SessionContainsXuid(session->second, identity.xuid)) {
      return Error(403, "not_member", "voice route owner is not a session member");
    }
    std::vector<std::string> targets;
    std::unordered_set<std::string> unique_targets;
    std::unordered_set<std::string> muted;
    for (const auto& wire_xuid : body->at("target_xuids")) {
      if (!wire_xuid.is_string()) return Error(400, "invalid_voice_route", "voice target is malformed");
      const std::string xuid = wire_xuid.get<std::string>();
      if (!IsHex(xuid, 16) || xuid == identity.xuid ||
          !SessionContainsXuid(session->second, xuid) || !unique_targets.insert(xuid).second) {
        return Error(400, "invalid_voice_route", "voice target is not a unique session member");
      }
      targets.push_back(xuid);
    }
    for (const auto& wire_xuid : body->at("mute_xuids")) {
      if (!wire_xuid.is_string()) return Error(400, "invalid_voice_route", "muted member is malformed");
      const std::string xuid = wire_xuid.get<std::string>();
      if (!IsHex(xuid, 16) || xuid == identity.xuid ||
          !SessionContainsXuid(session->second, xuid) ||
          !muted.insert(xuid).second) {
        return Error(400, "invalid_voice_route", "muted identity is not a unique session member");
      }
    }
    // GTA IV's title policy uses an empty target set for proximity/all chat,
    // at most one peer for a private phone call, and the explicit teammate set
    // for team chat.  Enforce the same shape at the trust boundary so a
    // modified client cannot turn the all/private labels into arbitrary
    // multicast routes.
    if ((channel == "all" && !targets.empty()) ||
        (channel == "private" && targets.size() > 1)) {
      return Error(400, "invalid_voice_route",
                   "voice channel target policy is invalid");
    }
    if (!replaced_token.empty()) {
      const auto replaced = voice_routes_.find(replaced_token);
      if (replaced != voice_routes_.end() &&
          replaced->second.owner_xuid != identity.xuid) {
        return Error(403, "not_route_owner",
                     "replacement voice route belongs to another identity");
      }
    }
    const std::string token = RandomToken("voice_", 16);
    if (token.empty()) return Error(500, "entropy_failure", "random generation failed");
    VoiceRoute route{.owner_xuid = identity.xuid, .session_id = session_id,
                     .channel = channel, .targets = std::move(targets), .muted = std::move(muted),
                     .expires = Clock::now() + kVoiceRouteLease};
    std::vector<std::string> replaced_routes;
    for (const auto& [existing_token, existing] : voice_routes_) {
      // A same-session reconfiguration or lease recovery atomically
      // supersedes the prior endpoint. Different session routes may coexist
      // for session-isolated cursors; the active client explicitly supplies
      // replace_route_token when it transitions across sessions.
      if (existing.owner_xuid == identity.xuid &&
          (existing.session_id == session_id ||
           existing_token == replaced_token)) {
        replaced_routes.push_back(existing_token);
      }
    }
    for (const std::string& existing_token : replaced_routes) {
      EraseVoiceRouteLocked(existing_token);
    }
    voice_routes_[token] = std::move(route);
    voice_route_next_cursors_[token] = 0;
    voice_route_delivered_cursors_[token] = 0;
    voice_route_replay_after_cursors_[token] = 0;
    return {.status = 201, .body = {{"route_token", token}}};
  }

  Response VoicePackets(const Request& request, const Identity& identity) {
    std::unique_lock lock(mutex_);
    if (request.method == "GET") {
      const std::string token = request.query.contains("route_token")
                                    ? request.query.at("route_token")
                                    : "";
      std::uint64_t after_cursor = 0;
      std::string after;
      bool explicit_after = false;
      if (const auto found = request.query.find("after"); found != request.query.end()) {
        explicit_after = true;
        after = found->second;
        static constexpr std::string_view kCursorPrefix = "cursor_";
        if (!after.starts_with(kCursorPrefix)) {
          return Error(400, "invalid_voice_cursor", "voice cursor is malformed");
        }
        const auto parsed = ParseDecimalUint64(
            std::string_view(after).substr(kCursorPrefix.size()));
        if (!parsed || !*parsed) {
          return Error(400, "invalid_voice_cursor", "voice cursor is malformed");
        }
        after_cursor = *parsed;
      }
      std::uint64_t wait_ms = 0;
      if (const auto found = request.query.find("wait_ms"); found != request.query.end()) {
        const auto parsed = ParseDecimalUint64(found->second);
        if (!parsed || *parsed > 1000) {
          return Error(400, "invalid_voice_wait", "voice wait is malformed");
        }
        wait_ms = *parsed;
      }
      auto route = voice_routes_.find(token);
      if (route == voice_routes_.end() || route->second.owner_xuid != identity.xuid ||
          route->second.expires <= Clock::now()) {
        if (route != voice_routes_.end() && route->second.expires <= Clock::now()) {
          EraseVoiceRouteLocked(token);
        }
        return Error(404, "route_not_found", "voice route does not exist");
      }
      const auto session = sessions_.find(route->second.session_id);
      if (session == sessions_.end() ||
          !SessionContainsXuid(session->second, identity.xuid)) {
        EraseVoiceRouteLocked(token);
        return Error(403, "not_member", "voice receiver is not a session member");
      }
      if (explicit_after &&
          after_cursor > voice_route_next_cursors_[token]) {
        return Error(409, "invalid_voice_cursor",
                     "voice cursor is ahead of this route");
      }
      route->second.expires = Clock::now() + kVoiceRouteLease;
      PruneVoiceQueueLocked(token, Clock::now());
      if (!explicit_after) {
        const std::uint64_t delivered = voice_route_delivered_cursors_[token];
        const auto queued = voice_queues_.find(token);
        const bool has_newer =
            queued != voice_queues_.end() &&
            std::any_of(queued->second.begin(), queued->second.end(),
                        [&](const VoicePacket& packet) {
                          return packet.cursor > delivered;
                        });
        after_cursor = has_newer ? delivered : voice_route_replay_after_cursors_[token];
      }
      const auto has_packet_after = [&] {
        const auto queue = voice_queues_.find(token);
        return queue != voice_queues_.end() &&
               std::any_of(queue->second.begin(), queue->second.end(),
                           [&](const VoicePacket& packet) {
                             return packet.cursor > after_cursor;
                           });
      };
      if (wait_ms && !has_packet_after()) {
        voice_condition_.wait_for(
            lock, std::chrono::milliseconds(wait_ms), [&] {
              const auto current = voice_routes_.find(token);
              return current == voice_routes_.end() ||
                     current->second.owner_xuid != identity.xuid || has_packet_after();
            });
      }
      route = voice_routes_.find(token);
      if (route == voice_routes_.end() || route->second.owner_xuid != identity.xuid) {
        return Error(404, "route_not_found", "voice route does not exist");
      }
      route->second.expires = Clock::now() + kVoiceRouteLease;
      PruneVoiceQueueLocked(token, Clock::now());
      json packets = json::array();
      std::string next = after;
      std::uint64_t last_emitted_cursor = after_cursor;
      const auto queue = voice_queues_.find(token);
      if (queue != voice_queues_.end()) {
        for (const VoicePacket& packet : queue->second) {
          if (packet.cursor <= after_cursor) continue;
          packets.push_back({{"id", packet.id}, {"source_xuid", packet.source_xuid},
                             {"session_id", packet.session_id},
                             {"sequence", packet.sequence}, {"payload", packet.payload}});
          next = packet.id;
          last_emitted_cursor = packet.cursor;
          if (packets.size() >= kMaximumVoicePacketsPerBatch) break;
        }
      }
      if (!explicit_after && !packets.empty()) {
        voice_route_replay_after_cursors_[token] = after_cursor;
        voice_route_delivered_cursors_[token] = last_emitted_cursor;
      }
      return {.body = {{"packets", std::move(packets)}, {"next_after", next}}};
    }
    if (request.method != "POST") return Error(404, "not_found", "voice packet operation does not exist");
    const auto body = ParseBody(request);
    if (!body || !body->is_object() || !body->contains("packets") ||
        !body->at("packets").is_array() || body->at("packets").empty() ||
        body->at("packets").size() > kMaximumVoicePacketsPerBatch) {
      return Error(400, "invalid_voice_packets", "voice packet batch is malformed");
    }
    struct PendingVoice {
      std::string route_token;
      std::uint32_t sequence;
      std::string payload;
      std::string payload_digest;
      std::size_t encoded_bytes;
      bool duplicate = false;
    };
    std::vector<PendingVoice> pending;
    std::unordered_map<std::string,
                       std::unordered_map<std::uint32_t, std::string>>
        batch_sequences;
    pending.reserve(body->at("packets").size());
    for (const auto& wire : body->at("packets")) {
      if (!wire.is_object() || !wire.contains("route_token") ||
          !wire.at("route_token").is_string() || !wire.contains("sequence") ||
          (!wire.at("sequence").is_number_unsigned() && !wire.at("sequence").is_number_integer()) ||
          !wire.contains("payload") || !wire.at("payload").is_string()) {
        return Error(400, "invalid_voice_packets", "voice packet is malformed");
      }
      const std::string token = wire.value("route_token", "");
      auto route = voice_routes_.find(token);
      if (route == voice_routes_.end() || route->second.owner_xuid != identity.xuid ||
          route->second.expires <= Clock::now()) {
        if (route != voice_routes_.end() && route->second.expires <= Clock::now()) {
          EraseVoiceRouteLocked(token);
        }
        return Error(403, "invalid_voice_route", "voice packet route is unauthorized");
      }
      route->second.expires = Clock::now() + kVoiceRouteLease;
      const auto sequence = NonNegativeInteger(wire.at("sequence"));
      const std::string payload = wire.at("payload").get<std::string>();
      const auto decoded = DecodeBase64Url(payload);
      const auto payload_digest = Sha256Hex(payload);
      if (!sequence || *sequence > std::numeric_limits<std::uint32_t>::max() ||
          payload.size() > kMaximumVoiceEncodedPayloadBytes || !decoded || decoded->empty() ||
          decoded->size() > kMaximumVoicePayloadBytes || !payload_digest) {
        return Error(400, "invalid_voice_packets", "voice payload or sequence is invalid");
      }
      const auto session = sessions_.find(route->second.session_id);
      if (session == sessions_.end() || !SessionContainsXuid(session->second, identity.xuid)) {
        return Error(403, "not_member", "voice sender is not a session member");
      }
      const std::uint32_t wire_sequence = static_cast<std::uint32_t>(*sequence);
      bool duplicate = false;
      if (const auto accepted =
              route->second.accepted_payload_digests.find(wire_sequence);
          accepted != route->second.accepted_payload_digests.end()) {
        if (accepted->second != *payload_digest) {
          return Error(409, "voice_sequence_conflict",
                       "voice sequence was already accepted with another payload");
        }
        duplicate = true;
      }
      auto& current_batch = batch_sequences[token];
      const auto [batch_sequence, inserted] =
          current_batch.try_emplace(wire_sequence, payload);
      if (!inserted) {
        if (batch_sequence->second != payload) {
          return Error(409, "voice_sequence_conflict",
                       "voice batch reuses a sequence with another payload");
        }
        duplicate = true;
      }
      pending.push_back({.route_token = token,
                         .sequence = wire_sequence,
                         .payload = payload,
                         .payload_digest = *payload_digest,
                         .encoded_bytes = payload.size(),
                         .duplicate = duplicate});
    }
    struct Delivery {
      std::string receiver_token;
      std::string session_id;
      std::uint32_t sequence = 0;
      std::string payload;
      std::size_t encoded_bytes = 0;
      std::uint64_t cursor = 0;
    };
    std::vector<Delivery> deliveries;
    std::unordered_map<std::string, std::uint64_t> staged_cursors;
    for (const auto& packet : pending) {
      if (packet.duplicate) continue;
      const auto sender_route = voice_routes_.find(packet.route_token);
      if (sender_route == voice_routes_.end()) {
        return Error(403, "invalid_voice_route", "voice packet route was revoked");
      }
      const VoiceRoute& route = sender_route->second;
      std::vector<std::string> recipients = route.targets;
      if (route.channel == "all" && recipients.empty()) {
        const auto session = sessions_.find(route.session_id);
        if (session != sessions_.end()) {
          for (const auto& member : session->second["members"]) {
            recipients.push_back(member.value("xuid", ""));
          }
        }
      }
      for (const auto& recipient : recipients) {
        if (recipient == identity.xuid) {
          continue;
        }
        if (RelationshipBlocksLocked(identity.xuid, recipient)) {
          continue;
        }
        auto receiver_route = std::find_if(
            voice_routes_.begin(), voice_routes_.end(), [&](const auto& entry) {
              return entry.second.owner_xuid == recipient &&
                     entry.second.session_id == route.session_id &&
                     entry.second.expires > Clock::now();
            });
        if (receiver_route == voice_routes_.end() ||
            receiver_route->second.muted.contains(identity.xuid)) {
          continue;
        }
        const bool receiver_accepts_sender =
            receiver_route->second.channel == "all" ||
            std::ranges::find(receiver_route->second.targets,
                              identity.xuid) !=
                receiver_route->second.targets.end();
        if (!receiver_accepts_sender) {
          continue;
        }
        const auto session = sessions_.find(route.session_id);
        if (session == sessions_.end() ||
            !SessionContainsXuid(session->second, recipient)) {
          continue;
        }
        auto [cursor, inserted] = staged_cursors.try_emplace(
            receiver_route->first, voice_route_next_cursors_[receiver_route->first]);
        (void)inserted;
        std::uint64_t next_cursor = 0;
        if (!CheckedAddUint64(cursor->second, 1, next_cursor)) {
          return Error(409, "voice_cursor_exhausted", "voice cursor space is exhausted");
        }
        cursor->second = next_cursor;
        deliveries.push_back({.receiver_token = receiver_route->first,
                              .session_id = route.session_id,
                              .sequence = packet.sequence,
                              .payload = packet.payload,
                              .encoded_bytes = packet.encoded_bytes,
                              .cursor = next_cursor});
      }
    }
    const auto packet_expiry = Clock::now() + kVoicePacketLifetime;
    for (const Delivery& delivery : deliveries) {
        PruneVoiceQueueLocked(delivery.receiver_token, Clock::now());
        auto& queue = voice_queues_[delivery.receiver_token];
        auto& queued_bytes = voice_queue_encoded_bytes_[delivery.receiver_token];
        while (!queue.empty() &&
               (queue.size() >= kMaximumVoiceQueuedPackets ||
                delivery.encoded_bytes > kMaximumVoiceQueueEncodedBytes - queued_bytes)) {
          queued_bytes -= queue.front().encoded_bytes;
          queue.pop_front();
        }
        queue.push_back({.id = "cursor_" + std::to_string(delivery.cursor),
                         .cursor = delivery.cursor,
                         .source_xuid = identity.xuid,
                         .session_id = delivery.session_id,
                         .sequence = delivery.sequence,
                         .payload = delivery.payload,
                         .encoded_bytes = delivery.encoded_bytes,
                         .expires = packet_expiry});
        queued_bytes += delivery.encoded_bytes;
        voice_route_next_cursors_[delivery.receiver_token] = delivery.cursor;
    }
    for (const PendingVoice& packet : pending) {
      if (packet.duplicate) continue;
      auto route = voice_routes_.find(packet.route_token);
      if (route == voice_routes_.end()) {
        return Error(403, "invalid_voice_route", "voice packet route was revoked");
      }
      route->second.accepted_payload_digests[packet.sequence] =
          packet.payload_digest;
      route->second.accepted_sequence_order.push_back(packet.sequence);
      while (route->second.accepted_sequence_order.size() >
             kMaximumVoiceQueuedPackets) {
        const std::uint32_t expired_sequence =
            route->second.accepted_sequence_order.front();
        route->second.accepted_sequence_order.pop_front();
        route->second.accepted_payload_digests.erase(expired_sequence);
      }
    }
    voice_condition_.notify_all();
    return {.status = 204};
  }

  std::atomic<std::uint64_t> activity_generation_{0};
  std::mutex mutex_;
  std::condition_variable relay_condition_;
  std::condition_variable voice_condition_;
  std::condition_variable realtime_event_condition_;
  std::condition_variable sweeper_condition_;
  std::thread sweeper_;
  bool sweeper_stopping_ = false;
  bool realtime_events_stopping_ = false;
  libserver::PersistentState* persistent_state_ = nullptr;
  std::string public_url_;
  std::string initialization_error_;
  bool initialized_ = false;
  std::unordered_map<std::string, Challenge> challenges_;
  std::unordered_map<std::string, AccessSession> access_tokens_;
  std::unordered_map<std::string, DeviceBinding> devices_;
  std::unordered_map<std::string, RefreshSession> refresh_sessions_;
  std::map<std::string, json> sessions_;
  std::unordered_map<std::string, ArbitrationState> arbitration_snapshots_;
  std::unordered_set<std::string> ranked_started_;
  std::unordered_map<std::string, SessionMutationReceipt> session_mutation_receipts_;
  std::unordered_map<std::string, Clock::time_point> session_leases_;
  std::unordered_set<std::string> pending_durable_session_cleanup_;
  std::unordered_map<std::string, LobbyState> lobbies_;
  std::unordered_map<std::string, MatchmakingTicket> tickets_;
  std::unordered_map<std::string, RelayRoute> relay_routes_;
  std::unordered_map<std::string, QosListener> qos_listeners_;
  std::uint64_t next_qos_listener_generation_ = 0;
  std::unordered_map<std::string, libserver::QosProbeBatch> qos_probe_batches_;
  std::unordered_map<std::string, std::deque<Datagram>> relay_queues_;
  std::unordered_map<std::string, json> invites_;
  std::unordered_map<std::string, std::unordered_map<std::string, json>> stats_;
  std::unordered_map<std::string, std::string> player_names_;
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> relationships_;
  std::unordered_map<std::string, ProgressionRecord> progression_;
  std::unordered_map<std::string, std::unordered_map<std::string, json>> mode_stats_;
  std::unordered_map<std::string, json> ranked_results_;
  std::unordered_map<std::string, std::string> ranked_result_requests_;
  std::unordered_map<std::string, json> profiles_;
  std::unordered_map<std::string, StatWriteReceipt> stat_write_receipts_;
  std::unordered_map<std::string, InviteAcceptReceipt> invite_accept_receipts_;
  std::unordered_map<std::string, std::uint64_t> stat_next_sequences_;
  std::unordered_map<std::string, TitleProfileSettingRecord> title_profile_settings_;
  std::unordered_map<std::string, ProgAchRecord> prog_ach_records_;
  std::unordered_map<std::string, std::unordered_set<std::uint32_t>> achievements_;
  std::unordered_map<std::string, std::unordered_set<std::string>> entitlements_;
  std::unordered_map<std::string, PresenceRecord> presence_;
  std::unordered_map<std::string, VoiceRoute> voice_routes_;
  std::unordered_map<std::string, std::deque<VoicePacket>> voice_queues_;
  std::unordered_map<std::string, std::size_t> voice_queue_encoded_bytes_;
  std::unordered_map<std::string, std::uint64_t> voice_route_next_cursors_;
  std::unordered_map<std::string, std::uint64_t> voice_route_delivered_cursors_;
  std::unordered_map<std::string, std::uint64_t> voice_route_replay_after_cursors_;
  std::uint64_t realtime_event_cursor_floor_ = 0;
  std::uint64_t next_realtime_event_id_ = 0;
  std::unordered_map<std::string, std::deque<RealtimeEvent>> realtime_events_;
};

bool EmptyPollResult(const Request& request, const Response& response) {
  if (request.path == "/api/v2/qos/lookup") return response.status == 202;
  if (response.status != 200 || !response.body.is_object()) return false;
  const std::string_view field = request.path == "/api/v2/events" ? "events" :
      request.path == "/api/v2/relay/datagrams" ? "datagrams" :
      request.path == "/api/v2/voice/packets" ? "packets" :
      request.path == "/api/v3/qos/probes" ? "probes" : "";
  return !field.empty() && response.body.contains(field) &&
         response.body.at(field).is_array() && response.body.at(field).empty();
}

void HandleClient(Connection& connection, Service& service,
                  libserver::http::DeferredResponses* deferred = nullptr) {
  if (!connection.Handshake()) return;
  const auto result = libserver::http::ReadRequest(connection);
  if (result) {
    try {
      Request request = *result.request;
      std::uint64_t wait_ms = 0;
      const bool qos = request.method == "POST" && request.path == "/api/v2/qos/lookup";
      const bool get_poll = request.method == "GET" &&
          (request.path == "/api/v2/events" || request.path == "/api/v2/relay/datagrams" ||
           request.path == "/api/v2/voice/packets" || request.path == "/api/v3/qos/probes");
      if (deferred && qos) wait_ms = 2500;
      if (deferred && get_poll && request.query.contains("wait_ms")) {
        const auto parsed = ParseDecimalUint64(request.query.at("wait_ms"));
        const std::uint64_t maximum = request.path == "/api/v2/events" ? 30000 : 1000;
        if (!parsed || *parsed > maximum) {
          SendResponse(connection, Error(400, "invalid_poll_wait", "poll wait is outside its bound"));
          connection.Shutdown(std::chrono::milliseconds::zero());
          return;
        }
        wait_ms = *parsed;
        request.query["wait_ms"] = "0";
      }
      Response response = service.Dispatch(request);
      // Authenticate and validate BEFORE allocating any waiting connection.
      if (deferred && wait_ms && EmptyPollResult(request, response)) {
        const auto owner = service.WaitingOwner(request);
        if (!owner) {
          response = Error(401, "unauthorized", "poll identity expired");
        } else {
          std::string key = *owner + ':' + request.path;
          for (const std::string field : {"local_port", "route_token"}) {
            if (request.query.contains(field)) key += ':' + request.query.at(field);
          }
          if (qos) key += ':' + Sha256Hex(request.body).value_or("");
          const auto admission = deferred->Submit(connection, *owner, key,
              std::chrono::milliseconds(wait_ms),
              [request, &service, generation = service.ActivityGeneration(),
               next_check = Clock::now() + std::chrono::seconds(1)](bool expired) mutable
                  -> std::optional<std::string> {
                const auto current = service.ActivityGeneration();
                if (!expired && current == generation && Clock::now() < next_check) return {};
                generation = current;
                next_check = Clock::now() + std::chrono::seconds(1);
                try {
                  Response polled = service.Dispatch(request, true);
                  if (!expired && EmptyPollResult(request, polled)) return {};
                  if (expired && polled.status == 202) {
                    polled = Error(408, "probe_timeout", "QoS probe deadline expired");
                  }
                  return EncodeResponse(polled);
                } catch (const json::exception&) {
                  return EncodeResponse(Error(400, "invalid_json", "malformed poll data"));
                }
              });
          if (admission == libserver::http::DeferredResponses::Admission::kAccepted) return;
          response = Error(429, "poll_limit", "waiting subscription limit reached");
        }
      }
      SendResponse(connection, response);
    } catch (const json::exception&) {
      SendResponse(connection,
                   Error(400, "invalid_json", "JSON body contains a value of the wrong type"));
    }
  } else if (result.error_status != 0) {
    SendResponse(connection, Error(result.error_status, result.error_code, result.error_message));
  }
  connection.Shutdown(std::chrono::milliseconds::zero());
}

struct EntitlementGrant {
  std::string xuid;
  std::string package;
};

struct ServerOptions {
  std::string listen_address = "127.0.0.1";
  std::uint16_t port = 8080;
  std::filesystem::path data_directory = "libserver-data";
  std::string public_url;
  std::string tls_certificate;
  std::string tls_private_key;
  std::vector<EntitlementGrant> entitlement_grants;
  TransportMode transport = TransportMode::kPlaintextLoopback;
};

std::optional<EntitlementGrant> ParseEntitlementGrant(std::string_view text) {
  const std::size_t separator = text.find(':');
  if (separator == std::string_view::npos || text.find(':', separator + 1) !=
                                                std::string_view::npos) {
    return std::nullopt;
  }
  std::string xuid = Lower(std::string(text.substr(0, separator)));
  std::string package(text.substr(separator + 1));
  if (!IsHex(xuid, 16) || !IsKnownGta4EpisodePackage(package)) return std::nullopt;
  return EntitlementGrant{std::move(xuid), std::move(package)};
}

int RunServer(const ServerOptions& options) {
  using TlsContext = std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)>;
  TlsContext tls_context(nullptr, &SSL_CTX_free);
  if (options.transport == TransportMode::kDirectTls) {
    tls_context.reset(SSL_CTX_new(TLS_server_method()));
    if (!tls_context ||
        SSL_CTX_set_min_proto_version(tls_context.get(), TLS1_2_VERSION) != 1 ||
        SSL_CTX_use_certificate_chain_file(tls_context.get(),
                                           options.tls_certificate.c_str()) != 1 ||
        SSL_CTX_use_PrivateKey_file(tls_context.get(), options.tls_private_key.c_str(),
                                    SSL_FILETYPE_PEM) != 1 ||
        SSL_CTX_check_private_key(tls_context.get()) != 1) {
      std::cerr << "libserver: TLS certificate chain/private key configuration failed\n";
      ERR_clear_error();
      return 1;
    }
    SSL_CTX_set_options(tls_context.get(), SSL_OP_NO_COMPRESSION | SSL_OP_NO_RENEGOTIATION);
    SSL_CTX_set_mode(tls_context.get(), SSL_MODE_RELEASE_BUFFERS);
  }

  libserver::PersistentState persistent_state({.data_directory = options.data_directory});
  if (!persistent_state.Open()) {
    std::cerr << "libserver: durable store open failed: "
              << libserver::PersistentState::ErrorCodeName(persistent_state.error_code())
              << ": " << persistent_state.error_detail() << '\n';
    return 1;
  }
  Service service(&persistent_state, options.public_url);
  if (!service.Initialize()) {
    std::cerr << "libserver: durable payload validation failed: " << service.InitializationError() << '\n';
    return 1;
  }
  for (const auto& grant : options.entitlement_grants) {
    if (!service.GrantEntitlement(grant.xuid, grant.package)) {
      std::cerr << "libserver: entitlement grant could not be committed for " << grant.xuid
                << ':' << grant.package << '\n';
      return 1;
    }
  }
#if defined(_WIN32)
  WSADATA data{};
  if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 1;
#endif
  Socket server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server == libserver::http::kInvalidSocket) {
#if defined(_WIN32)
    WSACleanup();
#endif
    return 1;
  }
  int reuse = 1;
  setsockopt(server, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = htons(options.port);
  if (inet_pton(AF_INET, options.listen_address.c_str(), &address.sin_addr) != 1 ||
      bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0 ||
      listen(server, libserver::http::kDefaultListenBacklog) != 0) {
    libserver::http::CloseSocket(server);
    std::cerr << "libserver: could not listen on " << options.listen_address << ':'
              << options.port << '\n';
#if defined(_WIN32)
    WSACleanup();
#endif
    return 1;
  }
  libserver::http::DeferredResponses deferred;
  if (!deferred.Start()) {
    libserver::http::CloseSocket(server);
    return 1;
  }
  libserver::http::WorkerPool workers(
      libserver::http::kDefaultWorkerThreads,
      libserver::http::kDefaultQueuedConnections,
      [&](Connection& client) { HandleClient(client, service, &deferred); }, tls_context.get());
  if (!workers.Start()) {
    libserver::http::CloseSocket(server);
    std::cerr << "libserver: could not start HTTP worker pool\n";
#if defined(_WIN32)
    WSACleanup();
#endif
    return 1;
  }
  const char* local_scheme =
      options.transport == TransportMode::kDirectTls ? "https" : "http";
  std::cout << "libserver: ready at " << local_scheme << "://" << options.listen_address << ':'
            << options.port << " (public " << options.public_url
            << "; 64-player /api/v2)" << std::endl;
  while (!g_stopping) {
    const auto readiness =
        libserver::http::WaitReadable(server, libserver::http::kAcceptPollInterval);
    if (readiness == libserver::http::WaitResult::kTimeout ||
        readiness == libserver::http::WaitResult::kInterrupted) {
      continue;
    }
    if (readiness == libserver::http::WaitResult::kError) {
      if (g_stopping) break;
      std::cerr << "libserver: listener readiness check failed\n";
      break;
    }
    sockaddr_in peer{};
#if defined(_WIN32)
    int length = sizeof(peer);
#else
    socklen_t length = sizeof(peer);
#endif
    Socket client = accept(server, reinterpret_cast<sockaddr*>(&peer), &length);
    if (client == libserver::http::kInvalidSocket) {
      if (g_stopping) break;
      continue;
    }
    if (!workers.Enqueue(client)) {
      libserver::http::CloseSocket(client);
    }
  }
  libserver::http::CloseSocket(server);
  workers.Shutdown();
  deferred.Shutdown();
#if defined(_WIN32)
  WSACleanup();
#endif
  return 0;
}

bool VerifySessionOperationTraceSelfTest() {
  const Identity identity{.device_id = "trace-device",
                          .xuid = "0x00000000000000f1",
                          .machine_id = "0x00000000000000f2",
                          .player_name = "Trace Player"};
  const Request arbitration{
      .method = "PATCH",
      .path = "/api/v2/sessions/0xae355a5d509a5c27",
      .body = json({{"expected_revision", 4},
                    {"state", "lobby"},
                    {"lifecycle_state", 1},
                    {"mode", "gta4"},
                    {"ranked", true}})
                  .dump()};
  const Response rejected = Error(412, "revision_mismatch", "session revision changed");
  const std::string expected =
      "libserver-session-op: method=\"PATCH\" "
      "path=\"/api/v2/sessions/0xae355a5d509a5c27\" operation=arbitration "
      "xuid=\"0x00000000000000f1\" session=\"0xae355a5d509a5c27\" "
      "response_session=- status=412 result=error revision=- expected_revision=4 "
      "state=\"lobby\" lifecycle_state=1 lifecycle=\"registration\" mode=\"gta4\" "
      "ranked=true members=- results=- error=\"revision_mismatch\"";
  if (FormatSessionOperationTrace(arbitration, identity, rejected) != expected) return false;

  const Request start{
      .method = "PATCH",
      .path = "/api/v2/sessions/0xae355a5d509a5c27",
      .body = json({{"expected_revision", 5}, {"lifecycle_state", 2}}).dump()};
  const Request remove{
      .method = "DELETE",
      .path = "/api/v2/sessions/0xae355a5d509a5c27",
      .body = json({{"expected_revision", 6}}).dump()};
  const Request join{
      .method = "POST",
      .path = "/api/v2/sessions/0xae355a5d509a5c27/join",
      .body = json({{"expected_revision", 6}}).dump()};
  return SessionTraceOperation(start, *ParseBody(start)) == "start" &&
         SessionTraceOperation(remove, *ParseBody(remove)) == "delete" &&
         SessionTraceOperation(join, *ParseBody(join)) == "join";
}

bool SelfTest() {
  if (!VerifySessionOperationTraceSelfTest()) {
    std::cerr << "libserver self-test: session operation trace formatting failed\n";
    return false;
  }
  for (std::uint32_t rank = 0; rank < kRankCashThresholds.size(); ++rank) {
    if (RankForCash(kRankCashThresholds[rank]) != rank) {
      std::cerr << "libserver self-test: ranked cash threshold failed\n";
      return false;
    }
  }
  const auto parsed_grant =
      ParseEntitlementGrant("0x00000000000000D1:TLAD");
  if (!parsed_grant || parsed_grant->xuid != "0x00000000000000d1" ||
      parsed_grant->package != "TLAD" ||
      ParseEntitlementGrant("0x00000000000000d1:UNKNOWN") ||
      ParseEntitlementGrant("0x00000000000000d1:TLAD:extra")) {
    std::cerr << "libserver self-test: operator entitlement parser failed\n";
    return false;
  }
  DurableData empty;
  json schema_one = SerializeDurable(empty);
  schema_one["payload_schema"] = kLegacyDurablePayloadSchema;
  schema_one.erase("achievements");
  schema_one.erase("entitlements");
  schema_one.erase("stat_write_receipts");
  schema_one.erase("invite_accept_receipts");
  schema_one.erase("prog_ach_records");
  schema_one.erase("stat_next_sequences");
  schema_one.erase("title_profile_settings");
  json schema_two = SerializeDurable(empty);
  schema_two["payload_schema"] = kAchievementDurablePayloadSchema;
  schema_two.erase("entitlements");
  schema_two.erase("stat_write_receipts");
  schema_two.erase("invite_accept_receipts");
  schema_two.erase("prog_ach_records");
  schema_two.erase("stat_next_sequences");
  schema_two.erase("title_profile_settings");
  json schema_three = SerializeDurable(empty);
  schema_three["payload_schema"] = kEntitlementDurablePayloadSchema;
  schema_three.erase("stat_write_receipts");
  schema_three.erase("invite_accept_receipts");
  schema_three.erase("prog_ach_records");
  schema_three.erase("stat_next_sequences");
  schema_three.erase("title_profile_settings");
  schema_three["progression"]["0x00000000000000a1"] =
      {{"cash", 50000}, {"rank", 3}, {"updated_at", "legacy"}};
  schema_three["mode_stats"]["0x00000000000000a1"]["legacy_unknown_mode"] =
      {{"games", 1}, {"wins", 1}, {"score", 7}, {"kills", 2}, {"deaths", 0}};
  json schema_four = SerializeDurable(empty);
  schema_four["payload_schema"] = kProfileDurablePayloadSchema;
  schema_four.erase("invite_accept_receipts");
  schema_four.erase("prog_ach_records");
  DurableData migrated;
  std::string migration_error;
  if (!DeserializeDurable(schema_one, migrated, migration_error) ||
      !migrated.achievements.empty() || !migrated.entitlements.empty() ||
      !DeserializeDurable(schema_two, migrated, migration_error) ||
      !migrated.achievements.empty() || !migrated.entitlements.empty() ||
      !DeserializeDurable(schema_three, migrated, migration_error) ||
      migrated.stats.at("0x00000000000000a1")
              .at("0x0000006d")
              .at("0x2000000d")
              .at("value") != 50000 ||
      !migrated.mode_stats.at("0x00000000000000a1").contains("legacy_unknown_mode") ||
      !DeserializeDurable(schema_four, migrated, migration_error) ||
      !migrated.invite_accept_receipts.empty() || !migrated.prog_ach_records.empty()) {
    std::cerr << "libserver self-test: durable schema migration failed: "
              << migration_error << '\n';
    return false;
  }
  Service service;
  if (!service.Initialize()) return false;
  const auto live = service.Dispatch({.method = "GET", .path = "/health/live"});
  const auto unauthenticated = service.Dispatch({.method = "POST", .path = "/api/v2/sessions/search", .body = "{}"});
  const auto unauthenticated_achievements =
      service.Dispatch({.method = "GET", .path = "/api/v2/achievements"});
  const auto unauthenticated_entitlements =
      service.Dispatch({.method = "GET", .path = "/api/v2/entitlements"});
  const auto unauthenticated_prog_ach = service.Dispatch(
      {.method = "GET", .path = "/api/v3/storage/0x545407F2/3/Prog_ACH"});
  if (live.status != 200 || live.body.value("maximum_session_members", 0) != 64 ||
      unauthenticated.status != 401 || unauthenticated_achievements.status != 401 ||
      unauthenticated_entitlements.status != 401 || unauthenticated_prog_ach.status != 401) {
    std::cerr << "libserver self-test: basic endpoint contract failed\n";
    return false;
  }
  if (!service.VerifyParitySelfTest()) {
    std::cerr << "libserver self-test: parity contract failed\n";
    return false;
  }
  if (!service.VerifyResourceLeaseSelfTest()) {
    std::cerr << "libserver self-test: resource lease contract failed\n";
    return false;
  }
  if (!service.VerifyRelaySelfTest()) {
    std::cerr << "libserver self-test: relay contract failed\n";
    return false;
  }
  if (!service.VerifyQosSelfTest()) {
    std::cerr << "libserver self-test: QoS contract failed\n";
    return false;
  }
  if (!service.VerifySessionMutationIdempotencySelfTest()) {
    std::cerr << "libserver self-test: session mutation idempotency failed\n";
    return false;
  }

  const std::filesystem::path root = std::filesystem::temp_directory_path() /
                                     RandomToken("libserver-service-test-", 8);
  bool passed = true;
  {
    libserver::PersistentState state({.data_directory = root / "restart"});
    Service durable(&state, "http://self-test.invalid");
    passed = state.Open() && durable.Initialize() && durable.VerifyDurableAuthSelfTest();
    if (!passed) std::cerr << "libserver self-test: durable auth state failed\n";
  }
  if (passed) {
    libserver::PersistentState restarted_state({.data_directory = root / "restart"});
    Service restarted(&restarted_state, "http://self-test.invalid");
    passed = restarted_state.Open() && restarted.Initialize() &&
             restarted.VerifyRestartedStateSelfTest();
    if (!passed) std::cerr << "libserver self-test: restarted durable state failed\n";
  }
  if (passed) {
    libserver::PersistentState invalid_state({.data_directory = root / "invalid-schema"});
    passed = invalid_state.Open() && invalid_state.Save(json::object());
    Service invalid_service(&invalid_state, "http://self-test.invalid");
    passed = passed && !invalid_service.Initialize();
  }
  bool inject_save_fault = false;
  if (passed) {
    libserver::PersistentState::Options options{.data_directory = root / "fault"};
    options.fault_injector = [&](libserver::PersistentState::FaultPoint point) {
      return inject_save_fault &&
             point == libserver::PersistentState::FaultPoint::kBeforeRename;
    };
    libserver::PersistentState fault_state(std::move(options));
    Service fault_service(&fault_state, "http://self-test.invalid");
    passed = fault_state.Open() && fault_service.Initialize();
    inject_save_fault = true;
    const Identity identity{.device_id = "fault-device",
                            .xuid = "0x00000000000000f1",
                            .machine_id = "0x00000000000000f2",
                            .player_name = "Fault Player"};
    passed = passed && fault_service.VerifyFaultAtomicitySelfTest(identity);
    if (!passed) std::cerr << "libserver self-test: durable fault atomicity failed\n";
  }
  std::error_code ignored;
  std::filesystem::remove_all(root, ignored);
  return passed;
}

bool Multiplayer64Simulation() {
  Service service;
  return service.Initialize() && service.VerifyMultiplayer64Simulation();
}

void SignalHandler(int) { g_stopping = 1; }

}  // namespace

int main(int argc, char** argv) {
  if (argc == 2 && std::string_view(argv[1]) == "--self-test") {
    const bool passed = SelfTest();
    std::cout << (passed ? "libserver self-test passed\n" : "libserver self-test failed\n");
    return passed ? 0 : 1;
  }
  if (argc == 2 && std::string_view(argv[1]) == "--simulate-mp64") {
    const bool passed = Multiplayer64Simulation();
    std::cout << (passed
                      ? "libserver 64-slot session/transport simulation passed; this is not "
                        "a gameplay validation\n"
                      : "libserver 64-slot session/transport simulation failed\n");
    return passed ? 0 : 1;
  }

  ServerOptions options;
  bool listen_supplied = false;
  bool port_supplied = false;
  bool data_directory_supplied = false;
  bool public_url_supplied = false;
  bool certificate_supplied = false;
  bool private_key_supplied = false;
  bool behind_proxy = false;
  auto cli_error = [](std::string_view message) {
    std::cerr << "libserver: " << message << '\n';
    return 2;
  };

  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument == "--listen") {
      if (listen_supplied || index + 1 >= argc) return cli_error("invalid --listen option");
      listen_supplied = true;
      options.listen_address = argv[++index];
    } else if (argument == "--port") {
      if (port_supplied || index + 1 >= argc) return cli_error("invalid --port option");
      port_supplied = true;
      unsigned value = 0;
      const std::string text = argv[++index];
      const auto parsed = std::from_chars(text.data(), text.data() + text.size(), value);
      if (parsed.ec != std::errc{} || parsed.ptr != text.data() + text.size() || value == 0 ||
          value > std::numeric_limits<std::uint16_t>::max()) {
        return cli_error("port must be an integer from 1 through 65535");
      }
      options.port = static_cast<std::uint16_t>(value);
    } else if (argument == "--data-dir") {
      if (data_directory_supplied || index + 1 >= argc) {
        return cli_error("invalid --data-dir option");
      }
      data_directory_supplied = true;
      options.data_directory = argv[++index];
      if (options.data_directory.empty()) return cli_error("data directory must not be empty");
    } else if (argument == "--public-url") {
      if (public_url_supplied || index + 1 >= argc) {
        return cli_error("invalid --public-url option");
      }
      public_url_supplied = true;
      options.public_url = argv[++index];
    } else if (argument == "--tls-cert") {
      if (certificate_supplied || index + 1 >= argc) {
        return cli_error("invalid --tls-cert option");
      }
      certificate_supplied = true;
      options.tls_certificate = argv[++index];
      if (options.tls_certificate.empty()) return cli_error("TLS certificate path must not be empty");
    } else if (argument == "--tls-key") {
      if (private_key_supplied || index + 1 >= argc) {
        return cli_error("invalid --tls-key option");
      }
      private_key_supplied = true;
      options.tls_private_key = argv[++index];
      if (options.tls_private_key.empty()) return cli_error("TLS private key path must not be empty");
    } else if (argument == "--behind-proxy") {
      if (behind_proxy) return cli_error("--behind-proxy may only be specified once");
      behind_proxy = true;
    } else if (argument == "--grant-entitlement" && index + 1 < argc) {
      const auto grant = ParseEntitlementGrant(argv[++index]);
      if (!grant) return cli_error("invalid --grant-entitlement value");
      options.entitlement_grants.push_back(*grant);
    } else {
      return cli_error("unknown or incomplete option: " + argument);
    }
  }

  bool loopback = false;
  if (!IsValidListenAddress(options.listen_address, loopback)) {
    return cli_error("listen address must be a numeric IPv4 address");
  }
  if (certificate_supplied != private_key_supplied) {
    return cli_error("--tls-cert and --tls-key must be supplied together");
  }
  if (behind_proxy) {
    if (certificate_supplied) {
      return cli_error("--behind-proxy cannot be combined with direct TLS options");
    }
    if (!loopback) return cli_error("--behind-proxy requires a loopback listener");
    if (!public_url_supplied || !IsValidPublicUrl(options.public_url, true, false)) {
      return cli_error("--behind-proxy requires an explicit valid HTTPS public URL");
    }
    options.transport = TransportMode::kBehindProxy;
  } else if (certificate_supplied) {
    if (!public_url_supplied) {
      if (!loopback) {
        return cli_error("non-loopback direct TLS requires an explicit HTTPS public URL");
      }
      options.public_url =
          "https://" + options.listen_address + ':' + std::to_string(options.port);
    } else if (!IsValidPublicUrl(options.public_url, true, false)) {
      return cli_error("direct TLS requires a valid HTTPS public URL");
    }
    options.transport = TransportMode::kDirectTls;
  } else {
    if (!loopback) return cli_error("plaintext listeners are restricted to loopback");
    if (!public_url_supplied) {
      options.public_url =
          "http://" + options.listen_address + ':' + std::to_string(options.port);
    } else if (!IsValidPublicUrl(options.public_url, false, true)) {
      return cli_error("plaintext mode requires a valid HTTP public URL");
    }
  }

  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);
#if defined(SIGPIPE)
  std::signal(SIGPIPE, SIG_IGN);
#endif
  return RunServer(options);
}
