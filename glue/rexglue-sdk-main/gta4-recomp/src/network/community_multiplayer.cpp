/**
 ******************************************************************************
 * @file        community_multiplayer.cpp
 * @brief       Authenticated community directory and bounded HTTP relay client.
 ******************************************************************************
 */

#include "community_multiplayer.h"
#include "pending_stats_flush.h"
#include "community_qos_policy.h"
#include "community_profile_policy.h"
#include "community_retry_policy.h"
#include "community_stats_policy.h"
#include "community_voice_recovery.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <charconv>
#include <chrono>
#include <cctype>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <deque>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <rex/string/utf8.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/system/xam/arbitration_async.h>
#include <rex/system/xam/social_cache.h>
#include <rex/types.h>

#define UTF_CPP_CPLUSPLUS 201703L
#include <utf8.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

namespace LibertyRecomp::Network {
namespace {

using json = nlohmann::json;
using rex::system::xam::IPeerDatagramTransport;
using rex::system::xam::IAchievementService;
using rex::system::xam::IEntitlementService;
using rex::system::xam::ISocialService;
using rex::system::xam::IStatsService;
using rex::system::xam::ITitleProfileService;
using rex::system::xam::IGta4AchievementStorageService;
using rex::system::xam::IVoicePacketTransport;
using rex::system::xam::ITextChatTransport;
using rex::system::xam::IQosService;
using rex::system::xam::ISessionDirectory;
using rex::system::xam::ArbitrationCancellation;
using rex::system::xam::ArbitrationPollAction;
using rex::system::xam::ClassifyArbitrationPollResponse;
using rex::system::xam::FriendPresence;
using rex::system::xam::FriendRecord;
using rex::system::xam::InvitationRecord;
using rex::system::xam::CachedMuteState;
using rex::system::xam::LeaderboardResult;
using rex::system::xam::LiveBackendServices;
using rex::system::xam::LiveConfig;
using rex::system::xam::LiveIdentity;
using rex::system::xam::PeerDatagram;
using rex::system::xam::QosResult;
using rex::system::xam::QosTarget;
using rex::system::xam::SessionContext;
using rex::system::xam::SessionLifecycleState;
using rex::system::xam::SessionLookupResult;
using rex::system::xam::SessionLookupState;
using rex::system::xam::SessionMember;
using rex::system::xam::SessionProperty;
using rex::system::xam::SessionRecord;
using rex::system::xam::FriendEnumerationResult;
using rex::system::xam::SocialServiceStatus;
using rex::system::xam::StatColumn;
using rex::system::xam::StatColumnIdKind;
using rex::system::xam::StatPayload;
using rex::system::xam::StatRow;
using rex::system::xam::StatValueType;
using rex::system::xam::StatView;
using rex::system::xam::StatsServiceStatus;
using rex::system::xam::TitleProfileFetchResult;
using rex::system::xam::TitleProfileFetchStatus;
using rex::system::xam::TitleProfileRecord;
using rex::system::xam::TitleProfileStoreResult;
using rex::system::xam::TitleProfileStoreStatus;
using rex::system::xam::Gta4AchievementStorageFetchResult;
using rex::system::xam::Gta4AchievementStorageFetchStatus;
using rex::system::xam::Gta4AchievementStorageRecord;
using rex::system::xam::detail::CommunityReconnectDelay;
using rex::system::xam::VoiceChannel;
using rex::system::xam::VoicePacket;
using rex::system::xam::VoiceRoute;
using rex::system::xam::TextChatChannel;
using rex::system::xam::TextChatMessage;

// Derived by tools/derive_community_client_limits.py.
constexpr size_t kMaximumHttpResponseBytes = 2097152;
constexpr size_t kMaximumPropertyBytes = 512;
constexpr size_t kMaximumDatagramBytes = 65507;
constexpr size_t kMaximumPendingDatagramsPerPort = 256;
constexpr uint32_t kMaximumReceiveBatchBytes = 1048576;
constexpr size_t kMaximumRelayBatchDatagrams = 64;
constexpr size_t kMaximumRelayBatchEncodedBytes = 1011712;
constexpr size_t kMaximumOutboundDatagrams = 1024;
constexpr size_t kMaximumOutboundEncodedBytes = 16187392;
constexpr long kConnectTimeoutMilliseconds = 3000;
constexpr long kRequestTimeoutMilliseconds = 8000;
constexpr long kRelayPollTimeoutMilliseconds = 1000;
constexpr int kRelayLongPollMilliseconds = 250;
constexpr auto kRelayBatchDelay = std::chrono::milliseconds(2);
constexpr auto kRouteRefreshInterval = std::chrono::seconds(60);
constexpr auto kRouteRetryInterval = std::chrono::seconds(10);
constexpr auto kPresenceRefreshInterval = std::chrono::seconds(60);
constexpr auto kAccessTokenRefreshSkew = std::chrono::seconds(30);
constexpr auto kArbitrationPollInterval = std::chrono::milliseconds(1000);
constexpr size_t kMaximumFriends = 100;
constexpr size_t kMaximumSocialXuids = 100;
constexpr size_t kMaximumInvitationRecipients = 32;
constexpr size_t kMaximumInvitationDataBytes = 512;
constexpr size_t kMaximumStatViews = 16;
constexpr size_t kMaximumStatColumns = 64;
constexpr size_t kMaximumStatUtf16Units = 256;
constexpr size_t kMaximumStatUtf8Bytes = 768;
// Derived and checked by tools/derive_stats_retry_contract.py.
constexpr size_t kMaximumStatWriteAttempts = 3;
// Derived and checked by tools/derive_title_profile_contract.py.
constexpr uint32_t kGta4TitleId = 0x545407F2;
constexpr size_t kMaximumGta4TitleProfileBytes = 1000;
constexpr size_t kGta4TitleProfileEntryBytes = 8;
constexpr size_t kMaximumVoicePacketBytes = 4096;
constexpr size_t kMaximumVoiceBatchPackets = 32;
constexpr size_t kMaximumPendingVoicePackets = 512;
constexpr size_t kMaximumPendingVoiceEncodedBytes = 2797568;
// Derived and checked by /tmp/derive_gta4_chat_contract.py.
constexpr size_t kMaximumTextChatBytes = 256;
constexpr size_t kMaximumPendingTextMessages = 32;
constexpr size_t kMaximumReceivedTextMessages = 128;
constexpr size_t kMaximumTextEventPage = 256;
constexpr int kTextChatPollWaitMilliseconds = 30000;
constexpr long kTextChatPollTimeoutMilliseconds = 31000;
constexpr uint32_t kFirstGta4AchievementId = 1;
constexpr uint32_t kLastGta4AchievementId = 65;
constexpr std::array<std::string_view, 2> kKnownGta4EpisodePackages = {"TLAD", "TBOGT"};
constexpr size_t kQosChallengeBytes = 16;
constexpr size_t kQosChallengeHexCharacters = 34;
// Derived from retail GET_CURRENT_EPISODE (sub_825D4CC8) by
// tools/audit_gta_invite_accept_contract.py.
constexpr uint32_t kGta4CurrentEpisodeAddress = 0x82B39504;

struct HttpResponse {
  long status = 0;
  std::string body;
  std::string error;
};

struct CurlBodySink {
  std::string* body = nullptr;
  size_t limit = 0;
  bool overflow = false;
};

std::string_view CurrentGta4EpisodeName() {
  const auto* runtime = rex::Runtime::instance();
  const auto* memory = runtime ? runtime->memory() : nullptr;
  if (!memory) return "base";
  const auto* episode =
      memory->TranslateVirtual<const rex::be<uint32_t>*>(kGta4CurrentEpisodeAddress);
  return episode ? rex::system::xam::detail::Gta4EpisodePresenceName(*episode) : "base";
}

class ThreadCurlHandle final {
 public:
  ThreadCurlHandle() : handle_(curl_easy_init()) {}
  ~ThreadCurlHandle() {
    if (handle_) curl_easy_cleanup(handle_);
  }

  ThreadCurlHandle(const ThreadCurlHandle&) = delete;
  ThreadCurlHandle& operator=(const ThreadCurlHandle&) = delete;

  CURL* Reset() const {
    if (handle_) curl_easy_reset(handle_);
    return handle_;
  }

 private:
  CURL* handle_ = nullptr;
};

CURL* ThreadCurl() {
  thread_local ThreadCurlHandle handle;
  return handle.Reset();
}

size_t WriteBody(char* data, size_t size, size_t count, void* opaque) {
  auto* sink = static_cast<CurlBodySink*>(opaque);
  if (!sink || !sink->body ||
      (count != 0 && size > std::numeric_limits<size_t>::max() / count)) {
    return 0;
  }
  const size_t bytes = size * count;
  if (bytes > sink->limit || sink->body->size() > sink->limit - bytes) {
    sink->overflow = true;
    return 0;
  }
  sink->body->append(data, bytes);
  return bytes;
}

std::string TrimmedBaseUrl(std::string value) {
  while (!value.empty() && value.back() == '/') {
    value.pop_back();
  }
  return value;
}

bool IsAllowedBaseUrl(std::string_view value) {
  if (value.starts_with("https://")) {
    return true;
  }
  if (!value.starts_with("http://")) {
    return false;
  }
  std::string_view authority = value.substr(std::string_view("http://").size());
  authority = authority.substr(0, authority.find('/'));
  if (authority.empty() || authority.find('@') != std::string_view::npos ||
      authority.find('?') != std::string_view::npos ||
      authority.find('#') != std::string_view::npos) {
    return false;
  }
  const auto valid_port = [](std::string_view suffix) {
    return suffix.empty() ||
           (suffix.front() == ':' && suffix.size() > 1 &&
            std::ranges::all_of(suffix.substr(1), [](char character) {
              return character >= '0' && character <= '9';
            }));
  };
  for (const std::string_view host : {std::string_view("localhost"),
                                      std::string_view("127.0.0.1"),
                                      std::string_view("[::1]")}) {
    if (authority.starts_with(host) && valid_port(authority.substr(host.size()))) return true;
  }
  return false;
}

std::string Hex64(uint64_t value) {
  std::array<char, 19> text{};
  const int written = std::snprintf(text.data(), text.size(), "0x%016llx",
                                    static_cast<unsigned long long>(value));
  return written == 18 ? std::string(text.data(), static_cast<size_t>(written)) : std::string{};
}

std::string Hex32(uint32_t value) {
  std::array<char, 11> text{};
  const int written = std::snprintf(text.data(), text.size(), "0x%08x", value);
  return written == 10 ? std::string(text.data(), static_cast<size_t>(written)) : std::string{};
}

template <typename Integer>
std::optional<Integer> ParseFixedHex(std::string_view text, size_t digits) {
  if (text.size() != digits + 2 || !text.starts_with("0x")) {
    return std::nullopt;
  }
  Integer value = 0;
  const char* first = text.data() + 2;
  const char* last = text.data() + text.size();
  const auto result = std::from_chars(first, last, value, 16);
  if (result.ec != std::errc{} || result.ptr != last) {
    return std::nullopt;
  }
  return value;
}

std::optional<uint64_t> ParseDecimalUint64(std::string_view text) {
  uint64_t parsed = 0;
  const auto result = std::from_chars(text.data(), text.data() + text.size(), parsed);
  if (text.empty() || result.ec != std::errc{} || result.ptr != text.data() + text.size()) {
    return std::nullopt;
  }
  return parsed;
}

bool IsValidGta4TitleProfileBlob(std::span<const uint8_t> blob) {
  if (blob.size() > kMaximumGta4TitleProfileBytes ||
      blob.size() % kGta4TitleProfileEntryBytes != 0) {
    return false;
  }
  std::unordered_set<uint32_t> keys;
  for (size_t offset = 0; offset < blob.size(); offset += kGta4TitleProfileEntryBytes) {
    const uint32_t key = (static_cast<uint32_t>(blob[offset]) << 24) |
                         (static_cast<uint32_t>(blob[offset + 1]) << 16) |
                         (static_cast<uint32_t>(blob[offset + 2]) << 8) |
                         static_cast<uint32_t>(blob[offset + 3]);
    if (!keys.insert(key).second) return false;
  }
  return true;
}

std::string HexBytes(std::span<const uint8_t> bytes) {
  static constexpr char kHex[] = "0123456789abcdef";
  std::string output("0x");
  output.reserve(2 + bytes.size() * 2);
  for (const uint8_t byte : bytes) {
    output.push_back(kHex[byte >> 4]);
    output.push_back(kHex[byte & 0x0F]);
  }
  return output;
}

bool ParseHexBytes(std::string_view text, std::span<uint8_t> output) {
  if (!text.starts_with("0x") || text.size() != output.size() * 2 + 2) {
    return false;
  }
  text.remove_prefix(2);
  for (size_t index = 0; index < output.size(); ++index) {
    unsigned value = 0;
    const char* first = text.data() + index * 2;
    const auto result = std::from_chars(first, first + 2, value, 16);
    if (result.ec != std::errc{} || result.ptr != first + 2) {
      return false;
    }
    output[index] = static_cast<uint8_t>(value);
  }
  return true;
}

std::optional<std::string> Base64Encode(std::span<const uint8_t> input, bool url_safe) {
  if (input.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
    return std::nullopt;
  }
  const size_t capacity = 4 * ((input.size() + 2) / 3);
  std::string output(capacity, '\0');
  const int written = EVP_EncodeBlock(reinterpret_cast<unsigned char*>(output.data()), input.data(),
                                      static_cast<int>(input.size()));
  if (written < 0 || static_cast<size_t>(written) != output.size()) {
    return std::nullopt;
  }
  if (url_safe) {
    std::replace(output.begin(), output.end(), '+', '-');
    std::replace(output.begin(), output.end(), '/', '_');
    while (!output.empty() && output.back() == '=') {
      output.pop_back();
    }
  }
  return output;
}

std::optional<std::vector<uint8_t>> Base64Decode(std::string text, bool url_safe,
                                                  size_t maximum_size) {
  if (url_safe) {
    std::replace(text.begin(), text.end(), '-', '+');
    std::replace(text.begin(), text.end(), '_', '/');
    while (text.size() % 4 != 0) {
      text.push_back('=');
    }
  }
  if (text.empty()) {
    return std::vector<uint8_t>{};
  }
  if (text.size() % 4 != 0 || text.size() > 4 * ((maximum_size + 2) / 3)) {
    return std::nullopt;
  }
  const size_t first_padding = text.find('=');
  const size_t padding = first_padding == std::string::npos ? 0 : text.size() - first_padding;
  if (padding > 2 ||
      (first_padding != std::string::npos &&
       text.find_first_not_of('=', first_padding) != std::string::npos)) {
    return std::nullopt;
  }
  std::vector<uint8_t> output(3 * (text.size() / 4));
  const int decoded = EVP_DecodeBlock(output.data(),
                                      reinterpret_cast<const unsigned char*>(text.data()),
                                      static_cast<int>(text.size()));
  if (decoded < 0) {
    return std::nullopt;
  }
  if (static_cast<size_t>(decoded) < padding) return std::nullopt;
  const size_t actual = static_cast<size_t>(decoded) - padding;
  if (actual > maximum_size) {
    return std::nullopt;
  }
  output.resize(actual);
  return output;
}

std::optional<std::string> Ipv4ToText(uint32_t network_order_address) {
  std::array<char, INET_ADDRSTRLEN> text{};
  if (!inet_ntop(AF_INET, &network_order_address, text.data(), text.size())) {
    return std::nullopt;
  }
  return std::string(text.data());
}

std::optional<uint32_t> ParseIpv4(std::string_view text) {
  uint32_t address = 0;
  const std::string terminated(text);
  if (inet_pton(AF_INET, terminated.c_str(), &address) != 1) {
    return std::nullopt;
  }
  return address;
}

std::optional<SessionLifecycleState> ParseLifecycle(const json& value) {
  if (value.is_number_unsigned()) {
    const uint32_t raw = value.get<uint32_t>();
    if (raw <= static_cast<uint32_t>(SessionLifecycleState::kDeleted)) {
      return static_cast<SessionLifecycleState>(raw);
    }
    return std::nullopt;
  }
  if (!value.is_string()) {
    return std::nullopt;
  }
  const std::string state = value.get<std::string>();
  if (state == "lobby") return SessionLifecycleState::kLobby;
  if (state == "registration") return SessionLifecycleState::kRegistration;
  if (state == "in_game") return SessionLifecycleState::kInGame;
  if (state == "reporting") return SessionLifecycleState::kReporting;
  if (state == "deleted") return SessionLifecycleState::kDeleted;
  return std::nullopt;
}

std::string_view LifecycleServiceState(SessionLifecycleState state) {
  switch (state) {
    case SessionLifecycleState::kLobby:
    case SessionLifecycleState::kRegistration:
      return "open";
    case SessionLifecycleState::kInGame:
      return "in_game";
    case SessionLifecycleState::kReporting:
    case SessionLifecycleState::kDeleted:
      return "closed";
  }
  return "closed";
}

std::string ErrorMessage(const HttpResponse& response) {
  if (!response.error.empty()) {
    return response.error;
  }
  try {
    const json parsed = json::parse(response.body);
    if (parsed.contains("error") && parsed["error"].is_object()) {
      const auto& error = parsed["error"];
      if (error.contains("message") && error["message"].is_string()) {
        const std::string message = error["message"].get<std::string>();
        if (error.contains("correlation_id") && error["correlation_id"].is_string()) {
          return message + " (" + error["correlation_id"].get<std::string>() + ")";
        }
        return message;
      }
    }
  } catch (...) {
  }
  return "community service returned HTTP " + std::to_string(response.status);
}

std::string ServerErrorCode(const HttpResponse& response) {
  if (!response.error.empty()) return "transport";
  try {
    const json parsed = json::parse(response.body);
    if (parsed.contains("error") && parsed.at("error").is_object()) {
      const std::string code = parsed.at("error").value("code", std::string{});
      if (!code.empty() && code.size() <= 64 &&
          std::ranges::all_of(code, [](unsigned char value) {
            return std::isalnum(value) || value == '_' || value == '-';
          })) {
        return code;
      }
    }
  } catch (...) {
  }
  return "-";
}

std::string BoundedPlayerName(std::string name) {
  if (name.empty()) {
    return "Player";
  }
  if (name.size() > 32) {
    name.resize(32);
    while (!name.empty() && (static_cast<unsigned char>(name.back()) & 0xC0) == 0x80) {
      name.pop_back();
    }
  }
  return name.empty() ? "Player" : name;
}

struct RemoteMetadata {
  int64_t revision = 0;
  int64_t host_epoch = 0;
};

class CommunityState final : public std::enable_shared_from_this<CommunityState> {
 public:
  CommunityState(const LiveConfig& config, const LiveIdentity& identity)
      : base_url_(TrimmedBaseUrl(config.community_url)), identity_(identity) {
    static std::once_flag curl_once;
    std::call_once(curl_once, [] { curl_global_init(CURL_GLOBAL_DEFAULT); });
    if (!IsAllowedBaseUrl(base_url_)) {
      SetError("community URL must use HTTPS (HTTP is allowed only for loopback development)");
      return;
    }
    if (!InitializeDeviceKey()) {
      return;
    }
    if (Enroll(kRequestTimeoutMilliseconds)) {
      ready_.store(true, std::memory_order_release);
      PublishCurrentPresence();
    }
    try {
      presence_worker_ = std::thread([this] { PresenceWorkerMain(); });
    } catch (...) {
      MarkUnavailable("could not start the Community reconnect worker");
    }
  }

  ~CommunityState() {
    {
      std::lock_guard lock(presence_mutex_);
      presence_stopping_ = true;
    }
    presence_condition_.notify_all();
    if (presence_worker_.joinable()) presence_worker_.join();
    if (device_key_) {
      EVP_PKEY_free(device_key_);
    }
  }

  bool ready() const { return ready_.load(std::memory_order_acquire); }

  std::string last_error() const {
    std::lock_guard lock(error_mutex_);
    return last_error_;
  }

  const LiveIdentity& identity() const { return identity_; }

  void SetConnectionRestoredHandler(std::function<void()> handler) {
    std::lock_guard lock(connection_handler_mutex_);
    connection_restored_handler_ = std::move(handler);
  }

  HttpResponse Request(std::string_view method, std::string_view path, const json* body,
                       bool authenticated, std::span<const std::string> additional_headers = {},
                       long timeout_ms = kRequestTimeoutMilliseconds,
                       bool allow_recovery = false) {
    std::string token;
    if (authenticated) {
      bool skipped_unavailable = false;
      if (!EnsureAuthenticated(false, timeout_ms, allow_recovery, &skipped_unavailable)) {
        // A request skipped before another thread recovered must not publish
        // an old unavailable observation over that newer authenticated success.
        if (!skipped_unavailable) MarkUnavailable(last_error());
        return {.error = last_error()};
      }
      std::lock_guard lock(auth_mutex_);
      token = access_token_;
    }

    HttpResponse response = RequestRaw(method, path, body, token, additional_headers, timeout_ms);
    if (authenticated && response.status == 401 &&
        EnsureAuthenticated(true, timeout_ms, allow_recovery)) {
      std::lock_guard lock(auth_mutex_);
      token = access_token_;
      response = RequestRaw(method, path, body, token, additional_headers, timeout_ms);
    }
    if (authenticated &&
        (!response.error.empty() || response.status == 401 || response.status >= 500)) {
      MarkUnavailable(ErrorMessage(response));
    } else if (!response.error.empty() || response.status < 200 || response.status >= 300) {
      SetError(ErrorMessage(response));
    } else if (authenticated && allow_recovery) {
      // A retained idempotent write may retry after a prior request marked the
      // service unavailable. Actual authenticated success, not another poll,
      // restores readiness. Run notification callbacks on the presence worker
      // rather than under a caller's stats-write mutex.
      SetError({});
      {
        std::lock_guard lock(presence_mutex_);
        if (!presence_stopping_ && !ready_.exchange(true, std::memory_order_acq_rel)) {
          recovery_notification_ = true;
        }
      }
      presence_condition_.notify_all();
    }
    return response;
  }

  std::optional<RemoteMetadata> Metadata(uint64_t session_id) const {
    std::lock_guard lock(session_mutex_);
    const auto found = metadata_.find(session_id);
    return found == metadata_.end() ? std::nullopt : std::optional(found->second);
  }

  void Remember(uint64_t session_id, int64_t revision, int64_t host_epoch) {
    std::lock_guard lock(session_mutex_);
    auto [entry, inserted] = metadata_.try_emplace(
        session_id, RemoteMetadata{.revision = revision, .host_epoch = host_epoch});
    if (!inserted) {
      // Responses may complete out of order across the session, relay, and
      // arbitration workers.  The service revisions and host epochs are
      // monotonic, so an older response must never make a later mutation send
      // stale optimistic-concurrency preconditions.
      entry->second.revision = std::max(entry->second.revision, revision);
      entry->second.host_epoch = std::max(entry->second.host_epoch, host_epoch);
    }
  }

  void Forget(uint64_t session_id) {
    std::lock_guard lock(session_mutex_);
    metadata_.erase(session_id);
  }

  void PublishOnlinePresence() {
    {
      std::lock_guard lock(presence_mutex_);
      presence_session_id_ = 0;
    }
    PublishCurrentPresence();
  }

  void PublishSessionPresence(uint64_t session_id) {
    if (!session_id) return;
    {
      std::lock_guard lock(presence_mutex_);
      presence_session_id_ = session_id;
    }
    PublishCurrentPresence();
  }

 private:
  void PublishCurrentPresence() {
    if (!ready()) return;
    std::lock_guard publish_lock(presence_publish_mutex_);
    if (!ready()) return;
    uint64_t session_id = 0;
    {
      std::lock_guard lock(presence_mutex_);
      if (presence_stopping_) return;
      session_id = presence_session_id_;
    }
    const json body = {{"state", session_id ? "in_game" : "online"},
                       {"session_id", session_id ? Hex64(session_id) : std::string{}},
                       {"episode", CurrentGta4EpisodeName()}};
    Request("PUT", "/api/v2/presence", &body, true);
  }

  void PresenceWorkerMain() {
    size_t failed_attempts = 0;
    for (;;) {
      bool recovered_by_request = false;
      {
        std::lock_guard lock(presence_mutex_);
        if (presence_stopping_) return;
        recovered_by_request = recovery_notification_ && ready();
        recovery_notification_ = false;
      }
      if (recovered_by_request) {
        failed_attempts = 0;
        PublishCurrentPresence();
        if (ready()) {
          std::lock_guard handler_lock(connection_handler_mutex_);
          if (connection_restored_handler_) connection_restored_handler_();
        }
        continue;
      }
      if (!ready()) {
        const auto retry_delay = CommunityReconnectDelay(failed_attempts);
        {
          std::unique_lock lock(presence_mutex_);
          if (presence_condition_.wait_for(
                  lock, retry_delay, [this] { return presence_stopping_ || ready(); })) {
            if (presence_stopping_) return;
            failed_attempts = 0;
            continue;
          }
        }

        bool reconnected = false;
        {
          std::lock_guard lock(auth_mutex_);
          if (!ready() && Enroll(kRequestTimeoutMilliseconds)) {
            SetError({});
            ready_.store(true, std::memory_order_release);
            reconnected = true;
          }
        }
        if (reconnected) {
          failed_attempts = 0;
          REXLOG_INFO("Community multiplayer connection recovered as {} ({:016X})",
                      identity_.player_name, identity_.xuid);
          PublishCurrentPresence();
          // Keep the callback serialized with removal so application shutdown
          // can prove no entitlement refresh still owns captured state.
          std::lock_guard handler_lock(connection_handler_mutex_);
          if (connection_restored_handler_) connection_restored_handler_();
        } else if (failed_attempts <
                   rex::system::xam::detail::kCommunityReconnectDelays.size()) {
          ++failed_attempts;
        }
        continue;
      }

      failed_attempts = 0;
      {
        std::unique_lock lock(presence_mutex_);
        if (presence_condition_.wait_for(lock, kPresenceRefreshInterval,
                                         [this] {
                                           return presence_stopping_ || !ready() || recovery_notification_;
                                         })) {
          if (presence_stopping_) return;
          continue;
        }
      }
      PublishCurrentPresence();
    }
  }

  bool InitializeDeviceKey() {
    std::array<uint8_t, 32> seed{};
    static constexpr std::string_view kDomain = "LibertyRecomp community Ed25519 device key v1";
    EVP_MD_CTX* digest = EVP_MD_CTX_new();
    if (!digest || EVP_DigestInit_ex(digest, EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(digest, kDomain.data(), kDomain.size()) != 1 ||
        EVP_DigestUpdate(digest, identity_.install_secret.data(), identity_.install_secret.size()) !=
            1) {
      if (digest) EVP_MD_CTX_free(digest);
      OPENSSL_cleanse(seed.data(), seed.size());
      SetError("could not derive the community device key");
      return false;
    }
    unsigned digest_size = 0;
    if (EVP_DigestFinal_ex(digest, seed.data(), &digest_size) != 1 ||
        digest_size != seed.size()) {
      EVP_MD_CTX_free(digest);
      OPENSSL_cleanse(seed.data(), seed.size());
      SetError("could not finish community device-key derivation");
      return false;
    }
    EVP_MD_CTX_free(digest);
    device_key_ = EVP_PKEY_new_raw_private_key(EVP_PKEY_ED25519, nullptr, seed.data(), seed.size());
    OPENSSL_cleanse(seed.data(), seed.size());
    if (!device_key_) {
      SetError("could not create the community Ed25519 key");
      return false;
    }
    std::array<uint8_t, 32> public_key{};
    size_t public_key_size = public_key.size();
    if (EVP_PKEY_get_raw_public_key(device_key_, public_key.data(), &public_key_size) != 1 ||
        public_key_size != public_key.size()) {
      SetError("could not export the community public key");
      return false;
    }
    const auto encoded = Base64Encode(public_key, true);
    if (!encoded) {
      SetError("could not encode the community public key");
      return false;
    }
    public_key_ = *encoded;

    std::array<uint8_t, 32> device_hash{};
    EVP_MD_CTX* device_digest = EVP_MD_CTX_new();
    static constexpr std::string_view kDeviceDomain = "LibertyRecomp community device id v1";
    if (!device_digest || EVP_DigestInit_ex(device_digest, EVP_sha256(), nullptr) != 1 ||
        EVP_DigestUpdate(device_digest, kDeviceDomain.data(), kDeviceDomain.size()) != 1 ||
        EVP_DigestUpdate(device_digest, public_key.data(), public_key.size()) != 1 ||
        EVP_DigestFinal_ex(device_digest, device_hash.data(), &digest_size) != 1 ||
        digest_size != device_hash.size()) {
      if (device_digest) EVP_MD_CTX_free(device_digest);
      SetError("could not derive the community device identifier");
      return false;
    }
    EVP_MD_CTX_free(device_digest);
    device_id_ = "device_" + HexBytes(device_hash).substr(2);
    return true;
  }

  std::optional<std::string> Sign(std::string_view message) {
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) return std::nullopt;
    size_t signature_size = 0;
    if (EVP_DigestSignInit(context, nullptr, nullptr, nullptr, device_key_) != 1 ||
        EVP_DigestSign(context, nullptr, &signature_size,
                       reinterpret_cast<const uint8_t*>(message.data()), message.size()) != 1) {
      EVP_MD_CTX_free(context);
      return std::nullopt;
    }
    std::vector<uint8_t> signature(signature_size);
    if (EVP_DigestSign(context, signature.data(), &signature_size,
                       reinterpret_cast<const uint8_t*>(message.data()), message.size()) != 1) {
      EVP_MD_CTX_free(context);
      return std::nullopt;
    }
    EVP_MD_CTX_free(context);
    signature.resize(signature_size);
    return Base64Encode(signature, true);
  }

  bool EnsureAuthenticated(bool force_refresh, long timeout_ms, bool allow_recovery = false,
                           bool* skipped_unavailable = nullptr) {
    std::lock_guard lock(auth_mutex_);
    if (skipped_unavailable) *skipped_unavailable = false;
    if (!ready() && !allow_recovery) {
      if (skipped_unavailable) *skipped_unavailable = true;
      return false;
    }
    const auto now = std::chrono::steady_clock::now();
    if (!force_refresh && !access_token_.empty() && now < access_token_deadline_) {
      return true;
    }
    if (!refresh_token_.empty()) {
      const json refresh = {{"refresh_token", refresh_token_}};
      const HttpResponse response = RequestRaw(
          "POST", "/api/v2/devices/refresh", &refresh, {}, {}, timeout_ms);
      if (response.status >= 200 && response.status < 300 && AcceptTokens(response.body)) {
        return true;
      }
      access_token_.clear();
      refresh_token_.clear();
    }
    return Enroll(timeout_ms);
  }

  bool Enroll(long timeout_ms) {
    const json challenge_request = {{"device_id", device_id_},
                                    {"xuid", Hex64(identity_.xuid)},
                                    {"public_key", public_key_}};
    const HttpResponse challenge_response = RequestRaw(
        "POST", "/api/v2/devices/challenge", &challenge_request, {}, {}, timeout_ms);
    if (challenge_response.status != 201) {
      if (ready() && (challenge_response.status == 400 || challenge_response.status == 401 ||
                      challenge_response.status == 403)) {
        MarkUnavailable(ErrorMessage(challenge_response));
      } else {
        SetError(ErrorMessage(challenge_response));
      }
      return false;
    }
    try {
      const json challenge = json::parse(challenge_response.body);
      if (!challenge.contains("challenge_id") || !challenge["challenge_id"].is_string() ||
          !challenge.contains("challenge") || !challenge["challenge"].is_string()) {
        SetError("community challenge response is malformed");
        return false;
      }
      const std::string challenge_text = challenge["challenge"].get<std::string>();
      const auto signature = Sign(challenge_text);
      if (!signature) {
        SetError("could not sign the community challenge");
        return false;
      }
      const json enroll = {{"challenge_id", challenge["challenge_id"]},
                           {"device_id", device_id_},
                           {"xuid", Hex64(identity_.xuid)},
                           {"machine_id", Hex64(identity_.machine_id)},
                           {"player_name", BoundedPlayerName(identity_.player_name)},
                           {"public_key", public_key_},
                           {"signature", *signature}};
      const HttpResponse response = RequestRaw(
          "POST", "/api/v2/devices/enroll", &enroll, {}, {}, timeout_ms);
      if (response.status != 201 || !AcceptTokens(response.body)) {
        if (ready() &&
            (response.status == 400 || response.status == 401 || response.status == 403)) {
          MarkUnavailable(ErrorMessage(response));
        } else {
          SetError(ErrorMessage(response));
        }
        return false;
      }
      return true;
    } catch (const std::exception& exception) {
      SetError(std::string("could not decode community challenge: ") + exception.what());
      return false;
    }
  }

  bool AcceptTokens(std::string_view body) {
    try {
      const json tokens = json::parse(body);
      if (!tokens.contains("access_token") || !tokens["access_token"].is_string() ||
          !tokens.contains("refresh_token") || !tokens["refresh_token"].is_string() ||
          !tokens.contains("expires_in") || !tokens["expires_in"].is_number_integer()) {
        return false;
      }
      const int64_t expires_in = tokens["expires_in"].get<int64_t>();
      if (expires_in <= 0 || expires_in > 604800) {
        return false;
      }
      access_token_ = tokens["access_token"].get<std::string>();
      refresh_token_ = tokens["refresh_token"].get<std::string>();
      if (access_token_.empty() || refresh_token_.empty() || access_token_.size() > 8192 ||
          refresh_token_.size() > 1024) {
        access_token_.clear();
        refresh_token_.clear();
        return false;
      }
      const auto lifetime = std::chrono::seconds(expires_in);
      access_token_deadline_ = std::chrono::steady_clock::now() +
                               (lifetime > kAccessTokenRefreshSkew
                                    ? lifetime - kAccessTokenRefreshSkew
                                    : std::chrono::seconds(1));
      return true;
    } catch (...) {
      return false;
    }
  }

  HttpResponse RequestRaw(std::string_view method, std::string_view path, const json* body,
                          std::string_view bearer,
                          std::span<const std::string> additional_headers,
                          long timeout_ms = kRequestTimeoutMilliseconds) const {
    HttpResponse response;
    CURL* curl = ThreadCurl();
    if (!curl) {
      response.error = "could not initialize the HTTP client";
      return response;
    }
    const std::string url = base_url_ + std::string(path);
    const std::string method_text(method);
    const std::string encoded_body = body ? body->dump() : std::string{};
    CurlBodySink sink{.body = &response.body, .limit = kMaximumHttpResponseBytes};
    curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Accept: application/json");
    if (body) headers = curl_slist_append(headers, "Content-Type: application/json");
    const std::string authorization = bearer.empty() ? std::string{} : "Authorization: Bearer " + std::string(bearer);
    if (!authorization.empty()) headers = curl_slist_append(headers, authorization.c_str());
    for (const auto& header : additional_headers) {
      headers = curl_slist_append(headers, header.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method_text.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, kConnectTimeoutMilliseconds);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout_ms);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "LibertyRecomp-Community/2");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteBody);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sink);
    if (body) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, encoded_body.data());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE,
                       static_cast<curl_off_t>(encoded_body.size()));
    }
    const CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
      response.error = sink.overflow ? "community response exceeded the two MiB limit"
                                     : curl_easy_strerror(result);
    } else {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status);
    }
    curl_slist_free_all(headers);
    return response;
  }

  void SetError(std::string message) const {
    std::lock_guard lock(error_mutex_);
    last_error_ = std::move(message);
  }

  void MarkUnavailable(std::string message) {
    SetError(std::move(message));
    ready_.store(false, std::memory_order_release);
    presence_condition_.notify_all();
  }

  std::string base_url_;
  LiveIdentity identity_;
  std::atomic<bool> ready_{false};
  EVP_PKEY* device_key_ = nullptr;
  std::string public_key_;
  std::string device_id_;

  mutable std::mutex auth_mutex_;
  std::string access_token_;
  std::string refresh_token_;
  std::chrono::steady_clock::time_point access_token_deadline_{};

  mutable std::mutex error_mutex_;
  mutable std::string last_error_;
  mutable std::mutex session_mutex_;
  std::unordered_map<uint64_t, RemoteMetadata> metadata_;
  std::mutex presence_mutex_;
  std::mutex presence_publish_mutex_;
  std::condition_variable presence_condition_;
  bool presence_stopping_ = false;
  bool recovery_notification_ = false;
  uint64_t presence_session_id_ = 0;
  std::mutex connection_handler_mutex_;
  std::function<void()> connection_restored_handler_;
  std::thread presence_worker_;
};

json ContextsToJson(std::span<const SessionContext> contexts) {
  json result = json::object();
  for (const auto& context : contexts) {
    const std::string key = Hex32(context.id);
    if (result.contains(key)) return json();
    result[key] = context.value;
  }
  return result;
}

json PropertiesToJson(std::span<const SessionProperty> properties) {
  json result = json::object();
  for (const auto& property : properties) {
    if (property.value.size() > kMaximumPropertyBytes) return json();
    const auto encoded = Base64Encode(property.value, false);
    if (!encoded) return json();
    const std::string key = Hex32(property.id);
    if (result.contains(key)) return json();
    result[key] = *encoded;
  }
  return result;
}

json MemberToJson(const SessionMember& member) {
  json route = {{"port", member.online_port}};
  // kMaximumSessionMembers is the local "unassigned" sentinel, not a valid
  // wire peer slot. Omit it so the authenticated directory can allocate the
  // first free stable slot while normalizing create and join operations.
  if (member.multiplayer_peer_id < rex::system::xam::kMaximumSessionMembers) {
    route["peer_id"] = member.multiplayer_peer_id;
  }
  if (const auto address = Ipv4ToText(member.virtual_ipv4)) route["address"] = *address;
  if (!member.peer_id.empty()) route["connection_id"] = member.peer_id;
  return {{"xuid", Hex64(member.xuid)},
          {"private", member.private_slot},
          {"machine_id", Hex64(member.machine_id)},
          {"virtual_ipv4", Ipv4ToText(member.virtual_ipv4).value_or("0.0.0.0")},
          {"online_port", member.online_port},
          {"peer_id", member.peer_id},
          {"route", std::move(route)}};
}

json SessionToJson(const SessionRecord& session) {
  json members = json::array();
  for (const auto& member : session.members) members.push_back(MemberToJson(member));
  if (members.empty() && session.host_xuid) {
    const SessionMember host{.xuid = session.host_xuid,
                             .private_slot = session.max_public_slots == 0 &&
                                             session.max_private_slots != 0,
                             .machine_id = session.host_machine_id,
                             .virtual_ipv4 = session.host_ipv4,
                             .online_port = session.host_port,
                             .peer_id = session.host_peer_id};
    members.push_back(MemberToJson(host));
  }
  // These labels keep the generic directory schema populated; the active GTA IV
  // XSession path does not use them for discovery. The title's exact flags,
  // contexts, and properties below remain authoritative.
  return {{"visibility", "public"},
          {"mode", "gta4"},
          {"episode", "all"},
          {"region", "global"},
          {"ranked", false},
          {"public_slots", session.max_public_slots},
          {"private_slots", session.max_private_slots},
          {"build_id", Hex32(session.title_version)},
          {"protocol_version", session.protocol_version},
          {"title_id", Hex32(session.title_id)},
          {"media_id", Hex32(session.media_id)},
          {"title_version", Hex32(session.title_version)},
          {"session_id", Hex64(session.session_id)},
          {"previous_session_id", Hex64(session.previous_session_id)},
          {"exchange_key", HexBytes(session.exchange_key)},
          {"nonce", Hex64(session.nonce)},
          {"flags", session.flags},
          {"lifecycle_state", static_cast<uint32_t>(session.lifecycle_state)},
          {"host_xuid", Hex64(session.host_xuid)},
          {"host_machine_id", Hex64(session.host_machine_id)},
          {"host_ipv4", Ipv4ToText(session.host_ipv4).value_or("0.0.0.0")},
          {"host_port", session.host_port},
          {"host_ethernet_address", HexBytes(session.host_ethernet_address).substr(2)},
          {"host_peer_id", session.host_peer_id},
          {"contexts", ContextsToJson(session.contexts)},
          {"properties", PropertiesToJson(session.properties)},
          {"members", std::move(members)}};
}

std::optional<SessionRecord> SessionFromJson(const json& value, int64_t* revision,
                                              int64_t* host_epoch) {
  try {
    if (!value.is_object()) return std::nullopt;
    const auto title_id = ParseFixedHex<uint32_t>(value.at("title_id").get<std::string>(), 8);
    const auto media_id = ParseFixedHex<uint32_t>(value.at("media_id").get<std::string>(), 8);
    const auto title_version =
        ParseFixedHex<uint32_t>(value.at("title_version").get<std::string>(), 8);
    const auto session_id = ParseFixedHex<uint64_t>(value.at("session_id").get<std::string>(), 16);
    const auto host_xuid = ParseFixedHex<uint64_t>(value.at("host_xuid").get<std::string>(), 16);
    if (!title_id || !media_id || !title_version || !session_id || !host_xuid) {
      return std::nullopt;
    }

    SessionRecord session;
    session.title_id = *title_id;
    session.media_id = *media_id;
    session.title_version = *title_version;
    session.protocol_version = value.at("protocol_version").get<uint32_t>();
    session.session_id = *session_id;
    session.host_xuid = *host_xuid;
    session.max_public_slots = value.at("public_slots").get<uint32_t>();
    session.max_private_slots = value.at("private_slots").get<uint32_t>();
    if (value.contains("roster_complete") &&
        !value.at("roster_complete").is_boolean()) {
      return std::nullopt;
    }
    session.roster_complete = value.value("roster_complete", true);
    if (!session.roster_complete) {
      if (!value.contains("public_member_count") ||
          !value.contains("private_member_count") ||
          !value.at("public_member_count").is_number_unsigned() ||
          !value.at("private_member_count").is_number_unsigned()) {
        return std::nullopt;
      }
      session.declared_public_members =
          value.at("public_member_count").get<uint32_t>();
      session.declared_private_members =
          value.at("private_member_count").get<uint32_t>();
    }
    if (!ParseHexBytes(value.at("exchange_key").get<std::string>(), session.exchange_key)) {
      return std::nullopt;
    }
    if (value.contains("previous_session_id")) {
      const auto parsed =
          ParseFixedHex<uint64_t>(value.at("previous_session_id").get<std::string>(), 16);
      if (!parsed) return std::nullopt;
      session.previous_session_id = *parsed;
    }
    if (value.contains("nonce")) {
      const auto parsed = ParseFixedHex<uint64_t>(value.at("nonce").get<std::string>(), 16);
      if (!parsed) return std::nullopt;
      session.nonce = *parsed;
    }
    if (value.contains("flags")) {
      if (value.at("flags").is_number_unsigned()) {
        session.flags = value.at("flags").get<uint32_t>();
      } else if (value.at("flags").is_string()) {
        const auto parsed = ParseFixedHex<uint32_t>(value.at("flags").get<std::string>(), 8);
        if (!parsed) return std::nullopt;
        session.flags = *parsed;
      } else {
        return std::nullopt;
      }
    }
    if (value.contains("lifecycle_state")) {
      const auto parsed = ParseLifecycle(value.at("lifecycle_state"));
      if (!parsed) return std::nullopt;
      session.lifecycle_state = *parsed;
    }
    if (value.contains("host_machine_id")) {
      const auto parsed = ParseFixedHex<uint64_t>(value.at("host_machine_id").get<std::string>(), 16);
      if (!parsed) return std::nullopt;
      session.host_machine_id = *parsed;
    }
    if (value.contains("host_ipv4")) {
      const auto parsed = ParseIpv4(value.at("host_ipv4").get<std::string>());
      if (!parsed) return std::nullopt;
      session.host_ipv4 = *parsed;
    }
    if (value.contains("host_port")) session.host_port = value.at("host_port").get<uint16_t>();
    if (value.contains("host_ethernet_address")) {
      std::string ethernet = value.at("host_ethernet_address").get<std::string>();
      if (!ethernet.starts_with("0x")) ethernet.insert(0, "0x");
      if (!ParseHexBytes(ethernet, session.host_ethernet_address)) return std::nullopt;
    }
    if (value.contains("host_peer_id")) {
      session.host_peer_id = value.at("host_peer_id").get<std::string>();
      if (session.host_peer_id.size() > 128) return std::nullopt;
    }

    const auto& context_values = value.at("contexts");
    const auto& property_values = value.at("properties");
    const auto& member_values = value.at("members");
    if (!context_values.is_object() || !property_values.is_object() || !member_values.is_array() ||
        context_values.size() > 64 || property_values.size() > 64 || member_values.size() > 64) {
      return std::nullopt;
    }
    for (auto entry = context_values.begin(); entry != context_values.end(); ++entry) {
      const auto id = ParseFixedHex<uint32_t>(entry.key(), 8);
      if (!id || !entry.value().is_number_integer()) return std::nullopt;
      session.contexts.push_back({.id = *id, .value = entry.value().get<uint32_t>()});
    }
    for (auto entry = property_values.begin(); entry != property_values.end(); ++entry) {
      const auto id = ParseFixedHex<uint32_t>(entry.key(), 8);
      if (!id || !entry.value().is_string()) return std::nullopt;
      auto decoded = Base64Decode(entry.value().get<std::string>(), false, kMaximumPropertyBytes);
      if (!decoded) return std::nullopt;
      session.properties.push_back({.id = *id, .value = std::move(*decoded)});
    }
    std::unordered_set<uint64_t> seen_xuids;
    for (const auto& member_value : member_values) {
      if (!member_value.is_object()) return std::nullopt;
      const auto xuid = ParseFixedHex<uint64_t>(member_value.at("xuid").get<std::string>(), 16);
      if (!xuid || !seen_xuids.insert(*xuid).second) return std::nullopt;
      SessionMember member{.xuid = *xuid,
                           .private_slot = member_value.value("private", false)};
      if (member_value.contains("machine_id")) {
        const auto parsed =
            ParseFixedHex<uint64_t>(member_value.at("machine_id").get<std::string>(), 16);
        if (!parsed) return std::nullopt;
        member.machine_id = *parsed;
      }
      if (member_value.contains("virtual_ipv4")) {
        const auto parsed = ParseIpv4(member_value.at("virtual_ipv4").get<std::string>());
        if (!parsed) return std::nullopt;
        member.virtual_ipv4 = *parsed;
      } else if (member_value.contains("route") && member_value["route"].contains("address")) {
        const auto parsed = ParseIpv4(member_value["route"]["address"].get<std::string>());
        if (!parsed) return std::nullopt;
        member.virtual_ipv4 = *parsed;
      }
      member.online_port = member_value.value("online_port", static_cast<uint16_t>(0));
      if (!member.online_port && member_value.contains("route")) {
        member.online_port = member_value["route"].value("port", static_cast<uint16_t>(0));
      }
      member.peer_id = member_value.value("peer_id", std::string{});
      if (member.peer_id.empty() && member_value.contains("route")) {
        member.peer_id = member_value["route"].value("connection_id", std::string{});
      }
      if (member_value.contains("route")) {
        const auto& route = member_value["route"];
        if (!route.is_object() || !route.contains("peer_id") ||
            !route.at("peer_id").is_number_unsigned()) {
          return std::nullopt;
        }
        member.multiplayer_peer_id = route.at("peer_id").get<uint32_t>();
        if (member.multiplayer_peer_id >= rex::system::xam::kMaximumSessionMembers) {
          return std::nullopt;
        }
      }
      if (member.peer_id.size() > 128) return std::nullopt;
      session.members.push_back(std::move(member));
    }
    const auto host = std::find_if(session.members.begin(), session.members.end(),
                                   [&](const SessionMember& member) {
                                     return member.xuid == session.host_xuid;
                                   });
    if (host != session.members.end()) {
      if (!session.host_machine_id) session.host_machine_id = host->machine_id;
      if (!session.host_ipv4) session.host_ipv4 = host->virtual_ipv4;
      if (!session.host_port) session.host_port = host->online_port;
      if (session.host_peer_id.empty()) session.host_peer_id = host->peer_id;
    }
    if (value.contains("open_public_slots") ||
        value.contains("open_private_slots")) {
      if (!value.contains("open_public_slots") ||
          !value.contains("open_private_slots") ||
          !value.at("open_public_slots").is_number_unsigned() ||
          !value.at("open_private_slots").is_number_unsigned()) {
        return std::nullopt;
      }
      session.open_public_slots =
          value.at("open_public_slots").get<uint32_t>();
      session.open_private_slots =
          value.at("open_private_slots").get<uint32_t>();
      if (session.open_public_slots > session.max_public_slots ||
          session.open_private_slots > session.max_private_slots) {
        return std::nullopt;
      }
    } else {
      uint32_t used_public = 0;
      uint32_t used_private = 0;
      for (const auto& member : session.members) {
        if (member.private_slot)
          ++used_private;
        else
          ++used_public;
      }
      session.open_public_slots = used_public <= session.max_public_slots
                                      ? session.max_public_slots - used_public
                                      : 0;
      session.open_private_slots = used_private <= session.max_private_slots
                                       ? session.max_private_slots - used_private
                                       : 0;
    }
    session.last_seen = std::chrono::steady_clock::now();
    if (!rex::system::xam::IsValidSessionRecord(session)) return std::nullopt;
    if (revision) *revision = value.value("revision", static_cast<int64_t>(0));
    if (host_epoch) *host_epoch = value.value("host_epoch", static_cast<int64_t>(0));
    return session;
  } catch (...) {
    return std::nullopt;
  }
}

std::string IdempotencyKey() {
  std::array<uint8_t, 16> random{};
  if (RAND_bytes(random.data(), random.size()) != 1) return {};
  return "idem_" + HexBytes(random).substr(2);
}

bool IsSafeOpaqueToken(std::string_view token) {
  return !token.empty() && token.size() <= 512 &&
         std::ranges::all_of(token, [](char value) {
           return (value >= 'a' && value <= 'z') || (value >= 'A' && value <= 'Z') ||
                  (value >= '0' && value <= '9') || value == '-' || value == '_' || value == '.';
         });
}

std::optional<std::string> Utf16ToBoundedUtf8(std::u16string_view text) {
  try {
    std::string result = rex::string::to_utf8(text);
    if (result.size() > kMaximumStatUtf8Bytes) return std::nullopt;
    return result;
  } catch (...) {
    return std::nullopt;
  }
}

std::optional<std::u16string> Utf8ToBoundedUtf16(std::string_view text) {
  if (text.size() > kMaximumStatUtf8Bytes) return std::nullopt;
  try {
    std::u16string result = rex::string::to_utf16(text);
    if (result.size() > kMaximumStatUtf16Units) return std::nullopt;
    return result;
  } catch (...) {
    return std::nullopt;
  }
}

std::optional<FriendRecord> FriendFromJson(const json& value) {
  try {
    if (!value.is_object() || !value.contains("state") ||
        !value.at("state").is_string()) {
      return std::nullopt;
    }
    const auto xuid = ParseFixedHex<uint64_t>(value.at("xuid").get<std::string>(), 16);
    if (!xuid || !*xuid) return std::nullopt;
    FriendRecord result{.xuid = *xuid,
                        .player_name = BoundedPlayerName(value.at("player_name").get<std::string>())};
    const std::string presence = value.value("presence", "offline");
    result.presence = presence == "online" || presence == "away"
                          ? FriendPresence::kOnline
                      : presence == "playing" || presence == "in_game" ||
                                presence == "in_title" || presence == "playing_title"
                            ? FriendPresence::kPlayingTitle
                            : FriendPresence::kOffline;
    const std::string session_id = value.value("session_id", std::string{});
    if (!session_id.empty()) {
      const auto parsed_session = ParseFixedHex<uint64_t>(session_id, 16);
      if (!parsed_session || !*parsed_session) return std::nullopt;
      result.session_id = *parsed_session;
    }
    if ((result.presence == FriendPresence::kPlayingTitle) !=
        (result.session_id != 0)) {
      return std::nullopt;
    }
    const std::string relationship = value.at("state").get<std::string>();
    if (relationship != "incoming" && relationship != "outgoing" &&
        relationship != "accepted" && relationship != "removed" &&
        relationship != "blocked" && relationship != "blocked_by") {
      return std::nullopt;
    }
    // This page is scoped to the authenticated local actor. Only an outgoing
    // `blocked` link means that actor muted the target; `blocked_by` is the
    // inverse relationship and must not be treated as a local mute.
    result.blocked =
        rex::system::xam::detail::IsLocallyBlockedRelationship(relationship);
    return result;
  } catch (...) {
    return std::nullopt;
  }
}

std::optional<InvitationRecord> InvitationFromJson(const json& value) {
  try {
    if (!value.is_object() || !value.at("id").is_string()) return std::nullopt;
    const auto sender = ParseFixedHex<uint64_t>(value.at("sender_xuid").get<std::string>(), 16);
    const auto recipient =
        ParseFixedHex<uint64_t>(value.at("recipient_xuid").get<std::string>(), 16);
    const auto session =
        ParseFixedHex<uint64_t>(value.at("session_id").get<std::string>(), 16);
    auto custom = Base64Decode(value.value("custom_data", std::string{}), false,
                               kMaximumInvitationDataBytes);
    if (!sender || !recipient || !session || !*sender || !*recipient || !*session || !custom) {
      return std::nullopt;
    }
    InvitationRecord result{.id = value.at("id").get<std::string>(),
                            .sender_xuid = *sender,
                            .recipient_xuid = *recipient,
                            .session_id = *session,
                            .custom_data = std::move(*custom),
                            .revision = value.value("revision", int64_t{})};
    if (!IsSafeOpaqueToken(result.id) || result.revision < 0) return std::nullopt;
    return result;
  } catch (...) {
    return std::nullopt;
  }
}

json StatColumnToJson(const StatColumn& column) {
  json result;
  switch (column.type) {
    case StatValueType::kUnset:
      // The title never writes an unset XUSER_DATA property. Unset values are
      // accepted only from read responses below.
      return json();
    case StatValueType::kInt32:
      if (!std::holds_alternative<int32_t>(column.value)) return json();
      result = {{"type", "i32"}, {"value", std::get<int32_t>(column.value)}};
      break;
    case StatValueType::kInt64:
      if (!std::holds_alternative<int64_t>(column.value)) return json();
      result = {{"type", "i64"}, {"value", std::get<int64_t>(column.value)}};
      break;
    case StatValueType::kDouble:
      if (!std::holds_alternative<double>(column.value) ||
          !std::isfinite(std::get<double>(column.value))) {
        return json();
      }
      result = {{"type", "f64"}, {"value", std::get<double>(column.value)}};
      break;
    case StatValueType::kUnicode:
      if (!std::holds_alternative<std::u16string>(column.value)) return json();
      if (const auto encoded =
              Utf16ToBoundedUtf8(std::get<std::u16string>(column.value))) {
        result = {{"type", "unicode"}, {"value", *encoded}};
      } else {
        return json();
      }
      break;
    case StatValueType::kBinary: {
      if (!std::holds_alternative<std::vector<uint8_t>>(column.value) ||
          std::get<std::vector<uint8_t>>(column.value).size() > 512) {
        return json();
      }
      const auto encoded = Base64Encode(std::get<std::vector<uint8_t>>(column.value), false);
      if (!encoded) return json();
      result = {{"type", "binary"}, {"value", *encoded}};
      break;
    }
  }
  return result;
}

std::optional<StatPayload> StatPayloadFromJson(const json& value) {
  try {
    const std::string type = value.at("type").get<std::string>();
    if (type == "i32") return StatPayload(value.at("value").get<int32_t>());
    if (type == "i64") return StatPayload(value.at("value").get<int64_t>());
    if (type == "f64") return StatPayload(value.at("value").get<double>());
    if (type == "unicode") {
      const std::string text = value.at("value").get<std::string>();
      auto decoded = Utf8ToBoundedUtf16(text);
      if (decoded) return StatPayload(std::move(*decoded));
      return std::nullopt;
    }
    if (type == "binary") {
      auto bytes = Base64Decode(value.at("value").get<std::string>(), false, 512);
      if (bytes) return StatPayload(std::move(*bytes));
    }
  } catch (...) {
  }
  return std::nullopt;
}

std::optional<StatColumn> StatColumnFromJson(uint32_t attribute_id,
                                             const json& value) {
  try {
    if (value.is_object() && value.at("type").get<std::string>() == "unset" &&
        value.size() == 1) {
      return StatColumn{.id = attribute_id,
                        .id_kind = StatColumnIdKind::kAttribute,
                        .type = StatValueType::kUnset,
                        .value = int32_t{}};
    }
  } catch (...) {
    return std::nullopt;
  }
  auto payload = StatPayloadFromJson(value);
  if (!payload ||
      (std::holds_alternative<double>(*payload) &&
       !std::isfinite(std::get<double>(*payload)))) {
    return std::nullopt;
  }
  const StatValueType type =
      std::holds_alternative<int32_t>(*payload) ? StatValueType::kInt32
      : std::holds_alternative<int64_t>(*payload) ? StatValueType::kInt64
      : std::holds_alternative<double>(*payload) ? StatValueType::kDouble
      : std::holds_alternative<std::u16string>(*payload) ? StatValueType::kUnicode
                                                        : StatValueType::kBinary;
  return StatColumn{.id = attribute_id,
                    .id_kind = StatColumnIdKind::kAttribute,
                    .type = type,
                    .value = std::move(*payload)};
}

struct OutboundVoicePacket {
  size_t encoded_payload_bytes = 0;
  json wire;
};

struct OutboundTextChatMessage {
  TextChatChannel channel = TextChatChannel::kAll;
  std::vector<uint64_t> target_xuids;
  uint32_t sequence = 0;
  std::string text;
  std::string idempotency;
};

bool IsRetryableRealtimeRequest(long http_status) {
  return http_status == 0 || http_status == 408 || http_status == 401 ||
         http_status >= 500;
}

class CommunityImportedServices final : public ISocialService,
                                        public IStatsService,
                                        public IAchievementService,
                                        public IEntitlementService,
                                        public ITitleProfileService,
                                        public IGta4AchievementStorageService,
                                        public IVoicePacketTransport,
                                        public ITextChatTransport {
  struct PendingStatWrite {
    uint64_t session_id = 0;
    uint64_t xuid = 0;
    json views;
    json body;
    std::string idempotency;
  };

 public:
  explicit CommunityImportedServices(std::shared_ptr<CommunityState> state)
      : state_(std::move(state)),
        social_worker_([this] { SocialWorkerMain(); }),
        voice_send_worker_([this] { VoiceSendWorkerMain(); }),
        voice_receive_worker_([this] { VoiceReceiveWorkerMain(); }),
        text_send_worker_([this] { TextSendWorkerMain(); }),
        text_receive_worker_([this] { TextReceiveWorkerMain(); }) {}

  ~CommunityImportedServices() override { Shutdown(); }

  bool ready() const override { return state_->ready(); }
  std::string last_error() const override { return state_->last_error(); }

  std::vector<bool> AreFriends(std::span<const uint64_t> xuids) override {
    if (xuids.empty() || xuids.size() > kMaximumSocialXuids) return {};
    json encoded = json::array();
    for (uint64_t xuid : xuids) {
      if (!xuid) return {};
      encoded.push_back(Hex64(xuid));
    }
    const json body = {{"xuids", std::move(encoded)}};
    const HttpResponse response = state_->Request("POST", "/api/v2/friends/check", &body, true);
    if (response.status != 200) return {};
    try {
      const json decoded = json::parse(response.body);
      const auto& results = decoded.at("results");
      if (!results.is_array() || results.size() != xuids.size()) return {};
      std::vector<bool> answer;
      answer.reserve(results.size());
      for (size_t index = 0; index < results.size(); ++index) {
        const auto wire_xuid =
            ParseFixedHex<uint64_t>(results[index].at("xuid").get<std::string>(), 16);
        if (!wire_xuid || *wire_xuid != xuids[index] ||
            !results[index].at("is_friend").is_boolean() ||
            !results[index].at("is_blocked").is_boolean()) {
          return {};
        }
        answer.push_back(results[index].at("is_friend").get<bool>() &&
                         !results[index].at("is_blocked").get<bool>());
      }
      return answer;
    } catch (...) {
      return {};
    }
  }

  FriendEnumerationResult EnumerateFriends(uint32_t maximum_results) override {
    FriendEnumerationResult result;
    if (!maximum_results || maximum_results > kMaximumFriends) {
      result.status = SocialServiceStatus::kInvalidRequest;
      return result;
    }
    const HttpResponse response = state_->Request(
        "GET", "/api/v2/friends?limit=" + std::to_string(maximum_results) +
                   "&state=accepted",
        nullptr, true);
    if (response.status != 200) {
      result.status = response.status == 0 ? SocialServiceStatus::kUnavailable
                                           : SocialServiceStatus::kRejected;
      return result;
    }
    try {
      const json decoded = json::parse(response.body);
      const auto& items = decoded.at("items");
      if (!items.is_array() || items.size() > maximum_results) return result;
      std::unordered_set<uint64_t> unique_xuids;
      result.friends.reserve(items.size());
      for (const auto& item : items) {
        if (!item.is_object() || item.value("state", std::string{}) != "accepted") {
          return {};
        }
        auto record = FriendFromJson(item);
        if (!record || record->blocked || !unique_xuids.insert(record->xuid).second) {
          return {};
        }
        result.friends.push_back(std::move(*record));
      }
      result.status = SocialServiceStatus::kSuccess;
      return result;
    } catch (...) {
      return result;
    }
  }

  bool SendInvitations(uint64_t session_id, std::span<const uint64_t> recipients,
                       std::span<const uint8_t> custom_data) override {
    std::unordered_set<uint64_t> unique_recipients;
    if (!session_id || recipients.empty() || recipients.size() > kMaximumInvitationRecipients ||
        custom_data.size() > kMaximumInvitationDataBytes ||
        std::ranges::any_of(recipients, [&unique_recipients](uint64_t xuid) {
          return !xuid || !unique_recipients.insert(xuid).second;
        })) {
      return false;
    }
    const auto encoded_data = Base64Encode(custom_data, false);
    const std::string idempotency = IdempotencyKey();
    if (!encoded_data || idempotency.empty()) return false;
    json encoded_recipients = json::array();
    for (uint64_t xuid : recipients) {
      encoded_recipients.push_back(Hex64(xuid));
    }
    const json body = {{"session_id", Hex64(session_id)},
                       {"recipient_xuids", std::move(encoded_recipients)},
                       {"custom_data", *encoded_data},
                       {"expires_in_seconds", 300}};
    const std::array<std::string, 1> headers = {"Idempotency-Key: " + idempotency};
    const HttpResponse response =
        state_->Request("POST", "/api/v2/invites", &body, true, headers);
    return response.status == 201 || response.status == 200;
  }

  std::optional<InvitationRecord> AcceptInvitation(uint64_t session_id,
                                                    uint64_t sender_xuid) override {
    auto attempt = social_cache_.PrepareExplicitAcceptance(session_id, sender_xuid);
    if (!attempt) return std::nullopt;

    const json body = {{"expected_revision", attempt->invitation.revision}};
    const std::array<std::string, 1> headers = {
        "Idempotency-Key: " + attempt->idempotency};
    const HttpResponse accepted = state_->Request(
        "POST", "/api/v2/invites/" + attempt->invitation.id + "/accept", &body, true,
        headers, rex::system::xam::detail::kCommunitySocialRequestTimeoutMilliseconds);
    if (accepted.status != 200) {
      social_cache_.AbandonExplicitAcceptance(attempt->invitation.id);
      return std::nullopt;
    }

    try {
      const json accepted_json = json::parse(accepted.body);
      auto accepted_invitation = InvitationFromJson(accepted_json.at("invite"));
      int64_t revision = 0;
      int64_t host_epoch = 0;
      auto session = SessionFromJson(accepted_json.at("session"), &revision, &host_epoch);
      if (!accepted_invitation || accepted_invitation->id != attempt->invitation.id ||
          accepted_invitation->sender_xuid != attempt->invitation.sender_xuid ||
          accepted_invitation->recipient_xuid != attempt->invitation.recipient_xuid ||
          accepted_invitation->recipient_xuid != state_->identity().xuid || !session ||
          session->session_id != accepted_invitation->session_id || revision < 1 ||
          host_epoch < 1) {
        social_cache_.AbandonExplicitAcceptance(attempt->invitation.id);
        return std::nullopt;
      }
      accepted_invitation->session = std::move(*session);
      if (!social_cache_.PublishExplicitAcceptance(attempt->invitation.id,
                                                   *accepted_invitation)) {
        social_cache_.AbandonExplicitAcceptance(attempt->invitation.id);
        return std::nullopt;
      }
      social_condition_.notify_all();
      return accepted_invitation;
    } catch (...) {
      social_cache_.AbandonExplicitAcceptance(attempt->invitation.id);
      return std::nullopt;
    }
  }

  std::optional<InvitationRecord> AcceptedInvitation() override {
    auto invitation = social_cache_.ConsumeAcceptedInvitation();
    social_condition_.notify_all();
    return invitation;
  }

  bool HasAcceptedInvitation() const override {
    return social_cache_.HasAcceptedInvitation();
  }

  CachedMuteState QueryMute(uint64_t xuid) const override {
    return social_cache_.QueryMute(xuid);
  }

  void SetInviteNotificationHandler(std::function<void()> handler) override {
    std::lock_guard lock(social_mutex_);
    if (social_stopping_) handler = {};
    social_cache_.SetInviteNotificationHandler(std::move(handler));
  }

  void Shutdown() override {
    std::call_once(shutdown_once_, [this] {
      {
        std::lock_guard lock(social_mutex_);
        social_stopping_ = true;
        social_cache_.SetInviteNotificationHandler({});
      }
      social_condition_.notify_all();
      if (social_worker_.joinable()) social_worker_.join();

      // Revoke the authenticated server route while CommunityState is still
      // alive.  Clearing the token first also wakes both workers out of their
      // current send/long-poll generation before shutdown joins them.
      Close();
      {
        std::lock_guard lock(voice_mutex_);
        voice_stopping_ = true;
      }
      voice_condition_.notify_all();
      if (voice_send_worker_.joinable()) voice_send_worker_.join();
      if (voice_receive_worker_.joinable()) voice_receive_worker_.join();

      {
        std::lock_guard lock(text_mutex_);
        text_stopping_ = true;
      }
      text_condition_.notify_all();
      if (text_send_worker_.joinable()) text_send_worker_.join();
      if (text_receive_worker_.joinable()) text_receive_worker_.join();
    });
  }

  bool ResetView(uint32_t view_id) override {
    if (!view_id) return false;
    const json body = {{"view_id", Hex32(view_id)}};
    const std::string idempotency = IdempotencyKey();
    if (idempotency.empty()) return false;
    const std::array<std::string, 1> headers = {"Idempotency-Key: " + idempotency};
    const HttpResponse response =
        state_->Request("POST", "/api/v2/stats/reset", &body, true, headers);
    return response.status >= 200 && response.status < 300;
  }

  bool ModifySkill(uint64_t session_id, std::span<const uint64_t> xuids) override {
    if (!session_id || xuids.empty() || xuids.size() > kMaximumSocialXuids) return false;
    json encoded = json::array();
    for (uint64_t xuid : xuids) {
      if (!xuid) return false;
      encoded.push_back(Hex64(xuid));
    }
    const json body = {{"xuids", std::move(encoded)}};
    const HttpResponse response =
        state_->Request("POST", "/api/v2/stats/skill", &body, true);
    if (response.status != 200) return false;
    try {
      const json decoded = json::parse(response.body);
      const auto& ratings = decoded.contains("ratings") ? decoded.at("ratings") : decoded.at("rows");
      if (!ratings.is_array() || ratings.size() != xuids.size()) return false;
      for (size_t index = 0; index < ratings.size(); ++index) {
        const auto xuid =
            ParseFixedHex<uint64_t>(ratings.at(index).at("xuid").get<std::string>(), 16);
        const double rating = ratings.at(index).at("rating").get<double>();
        if (!xuid || *xuid != xuids[index] || !std::isfinite(rating)) return false;
      }
      return true;
    } catch (...) {
      return false;
    }
  }

  bool Write(uint64_t session_id, uint64_t xuid, std::span<const StatView> views) override {
    if (!session_id || !xuid || views.empty() || views.size() > kMaximumStatViews) {
      return false;
    }
    json encoded_views = json::array();
    std::unordered_set<uint32_t> view_ids;
    for (const auto& view : views) {
      if (!view.id || !view_ids.insert(view.id).second || view.rows.size() != 1 ||
          !IsConsistentStatWriteTarget(xuid, view.rows.front().xuid) ||
          view.rows.front().columns.empty() ||
          view.rows.front().columns.size() > kMaximumStatColumns) {
        return false;
      }
      json columns = json::object();
      for (const auto& column : view.rows.front().columns) {
        json encoded = StatColumnToJson(column);
        const std::string id = Hex32(column.id);
        if (!column.id || column.id_kind != StatColumnIdKind::kProperty || encoded.is_null() ||
            columns.contains(id)) {
          return false;
        }
        columns[id] = std::move(encoded);
      }
      encoded_views.push_back({{"view_id", Hex32(view.id)},
                               {"rows", json::array({{{"xuid", Hex64(xuid)},
                                                      {"columns", std::move(columns)}}})}});
    }
    std::lock_guard lock(stats_write_mutex_);
    if (pending_stat_write_) {
      const bool same_request = pending_stat_write_->session_id == session_id &&
                                pending_stat_write_->xuid == xuid &&
                                pending_stat_write_->views == encoded_views;
      bool terminal_failure = false;
      if (SendPendingStatWrite(*pending_stat_write_, terminal_failure)) {
        pending_stat_write_.reset();
        if (same_request) return true;
      } else {
        if (terminal_failure) pending_stat_write_.reset();
        return false;
      }
    }

    const json allocation_body = {{"session_id", Hex64(session_id)}};
    const std::string allocation_idempotency = IdempotencyKey();
    if (allocation_idempotency.empty()) return false;
    const std::array<std::string, 1> allocation_headers = {
        "Idempotency-Key: " + allocation_idempotency};
    const HttpResponse allocation = state_->Request(
        "POST", "/api/v2/stats/sequences", &allocation_body, true, allocation_headers);
    if (allocation.status != 201) {
      REXLOG_WARN(
          "community-stats-op operation=write-sequence views={} properties=32-bit status={} "
          "error={}",
          views.size(), allocation.status, ServerErrorCode(allocation));
      return false;
    }
    std::optional<uint64_t> sequence;
    try {
      const json decoded = json::parse(allocation.body);
      if (!decoded.contains("sequence") || !decoded.at("sequence").is_string()) return false;
      const std::string text = decoded.at("sequence").get<std::string>();
      const auto parsed = ParseDecimalUint64(text);
      if (!parsed || !*parsed) {
        return false;
      }
      sequence = *parsed;
    } catch (...) {
      return false;
    }

    const std::string idempotency = IdempotencyKey();
    if (idempotency.empty()) return false;
    pending_stat_write_ = PendingStatWrite{
        .session_id = session_id,
        .xuid = xuid,
        .views = encoded_views,
        .body = {{"session_id", Hex64(session_id)},
                 {"sequence", std::to_string(*sequence)},
                 {"views", std::move(encoded_views)}},
        .idempotency = idempotency,
    };
    bool terminal_failure = false;
    if (!SendPendingStatWrite(*pending_stat_write_, terminal_failure)) {
      if (terminal_failure) pending_stat_write_.reset();
      return false;
    }
    pending_stat_write_.reset();
    return true;
  }

  bool SendPendingStatWrite(const PendingStatWrite& pending, bool& terminal_failure) {
    terminal_failure = false;
    const std::array<std::string, 1> headers = {
        "Idempotency-Key: " + pending.idempotency};
    for (size_t attempt = 0; attempt < kMaximumStatWriteAttempts; ++attempt) {
      const HttpResponse response =
          state_->Request("POST", "/api/v2/stats/writes", &pending.body, true, headers,
                          kRequestTimeoutMilliseconds, true);
      REXLOG_INFO(
          "community-stats-op operation=write target={:016X} views={} properties=32-bit "
          "status={} error={}",
          pending.xuid, pending.views.size(), response.status,
          ServerErrorCode(response));
      if (response.status >= 200 && response.status < 300) return true;
      if (response.status != 0 && response.status < 500) {
        terminal_failure = true;
        return false;
      }
    }
    return false;
  }

  bool Flush(uint64_t session_id) override {
    std::lock_guard lock(stats_write_mutex_);
    return gta4::network::detail::FlushPendingStats(
        session_id, state_->ready(), pending_stat_write_,
        [this](const PendingStatWrite& pending, bool& terminal) {
          return SendPendingStatWrite(pending, terminal);
        });
  }

  std::vector<StatView> Read(std::span<const uint64_t> xuids,
                             std::span<const uint32_t> view_ids,
                             std::span<const uint32_t> attribute_ids) override {
    std::unordered_set<uint32_t> unique_attributes;
    if (xuids.empty() || xuids.size() > kMaximumSocialXuids || view_ids.size() != 1 ||
        !view_ids.front() || attribute_ids.size() > kMaximumStatColumns ||
        std::ranges::any_of(xuids, [](uint64_t xuid) { return xuid == 0; }) ||
        std::ranges::any_of(attribute_ids, [&unique_attributes](uint32_t id) {
          return !id || id > std::numeric_limits<uint16_t>::max() ||
                 !unique_attributes.insert(id).second;
        })) {
      return {};
    }
    json encoded_xuids = json::array();
    json encoded_columns = json::array();
    for (uint64_t xuid : xuids) encoded_xuids.push_back(Hex64(xuid));
    for (uint32_t attribute : attribute_ids) encoded_columns.push_back(Hex32(attribute));
    const json body = {{"xuids", std::move(encoded_xuids)},
                       {"view_id", Hex32(view_ids.front())},
                       {"stat_ids", std::move(encoded_columns)}};
    const HttpResponse response =
        state_->Request("POST", "/api/v2/stats/read", &body, true);
    REXLOG_INFO(
        "community-stats-op operation=read xuids={} view={} attributes16={} ids={} "
        "status={} error={}",
        xuids.size(), body.at("view_id").get_ref<const std::string&>(),
        attribute_ids.size(), body.at("stat_ids").dump(), response.status,
        ServerErrorCode(response));
    if (response.status != 200) return {};
    try {
      const json decoded = json::parse(response.body);
      const auto& rows = decoded.at("rows");
      if (!rows.is_array() || rows.size() != xuids.size()) return {};
      StatView view{.id = view_ids.front()};
      const std::vector<uint32_t> wire_attribute_ids =
          Gta4WireStatAttributeIds(attribute_ids);
      for (size_t row_index = 0; row_index < rows.size(); ++row_index) {
        const auto& wire = rows.at(row_index);
        const auto xuid = ParseFixedHex<uint64_t>(wire.at("xuid").get<std::string>(), 16);
        if (!xuid || *xuid != xuids[row_index]) return {};
        StatRow row{.xuid = *xuid,
                    .rank = wire.value("rank", uint32_t{}),
                    .rating = wire.value("rating", int64_t{}),
                    .player_name = BoundedPlayerName(wire.value("player_name", "Player"))};
        const auto& columns = wire.at("columns");
        if (!columns.is_array() || columns.size() != wire_attribute_ids.size()) return {};
        size_t requested_position = 0;
        for (const auto& column_wire : columns) {
          if (!column_wire.is_object() || !column_wire.contains("stat_id") ||
              !column_wire.at("stat_id").is_string() ||
              !column_wire.contains("value")) {
            return {};
          }
          const auto id = ParseFixedHex<uint32_t>(
              column_wire.at("stat_id").get<std::string>(), 8);
          auto column = id ? StatColumnFromJson(*id, column_wire.at("value"))
                           : std::nullopt;
          if (!id || *id > std::numeric_limits<uint16_t>::max() ||
              requested_position == wire_attribute_ids.size() ||
              wire_attribute_ids[requested_position] != *id || !column) {
            return {};
          }
          row.columns.push_back(std::move(*column));
          ++requested_position;
        }
        view.rows.push_back(std::move(row));
      }
      return {std::move(view)};
    } catch (...) {
      return {};
    }
  }

  LeaderboardResult Leaderboard(uint32_t view_id,
                                std::span<const uint32_t> attribute_ids,
                                uint32_t offset, uint32_t maximum_results,
                                bool friends_only) override {
    LeaderboardResult result;
    std::unordered_set<uint32_t> unique_attributes;
    if (!view_id || attribute_ids.size() > kMaximumStatColumns || !maximum_results ||
        maximum_results > 100 ||
        std::ranges::any_of(attribute_ids, [&unique_attributes](uint32_t id) {
          return !id || id > std::numeric_limits<uint16_t>::max() ||
                 !unique_attributes.insert(id).second;
        })) {
      result.status = StatsServiceStatus::kInvalidRequest;
      return result;
    }
    std::string path = "/api/v2/leaderboards/" + Hex32(view_id) +
                       "?offset=" + std::to_string(offset) +
                       "&limit=" + std::to_string(maximum_results) +
                       "&friends_only=" + (friends_only ? "true" : "false");
    if (!attribute_ids.empty()) {
      path += "&stat_ids=";
      for (size_t index = 0; index < attribute_ids.size(); ++index) {
        if (index) path.push_back(',');
        path += Hex32(attribute_ids[index]);
      }
    }
    const HttpResponse response = state_->Request("GET", path, nullptr, true);
    if (response.status != 200) {
      REXLOG_WARN(
          "community-stats-op operation=leaderboard-read view={:08X} attributes16={} "
          "status={} error={}",
          view_id, attribute_ids.size(), response.status, ServerErrorCode(response));
      result.status = response.status == 0 ? StatsServiceStatus::kUnavailable
                                           : StatsServiceStatus::kRejected;
      return result;
    }
    try {
      const json decoded = json::parse(response.body);
      result.page.total = decoded.at("total").get<uint32_t>();
      const std::vector<uint32_t> wire_attribute_ids =
          Gta4WireStatAttributeIds(attribute_ids);
      const auto& rows = decoded.at("rows");
      if (!rows.is_array() || rows.size() > maximum_results ||
          result.page.total < rows.size()) {
        result.status = StatsServiceStatus::kInvalidResponse;
        result.page = {};
        return result;
      }
      std::unordered_set<uint64_t> unique_xuids;
      size_t row_index = 0;
      for (const auto& wire : rows) {
        const auto xuid = ParseFixedHex<uint64_t>(wire.at("xuid").get<std::string>(), 16);
        const uint32_t rank = wire.at("rank").get<uint32_t>();
        if (!xuid || !*xuid || !rank || rank > result.page.total ||
            !IsExpectedGlobalLeaderboardRank(offset, row_index, rank) ||
            !unique_xuids.insert(*xuid).second) {
          throw std::runtime_error("invalid leaderboard identity metadata");
        }
        StatRow row{.xuid = *xuid,
                    .rank = rank,
                    .rating = wire.at("rating").get<int64_t>(),
                    .player_name = BoundedPlayerName(wire.at("player_name").get<std::string>())};
        const auto& columns = wire.at("columns");
        if (!columns.is_array() || columns.size() != wire_attribute_ids.size()) {
          throw std::runtime_error("invalid leaderboard columns");
        }
        size_t requested_position = 0;
        for (const auto& column_wire : columns) {
          const auto id = ParseFixedHex<uint32_t>(
              column_wire.at("stat_id").get<std::string>(), 8);
          auto column = id ? StatColumnFromJson(*id, column_wire.at("value"))
                           : std::nullopt;
          if (!id || requested_position == wire_attribute_ids.size() ||
              wire_attribute_ids[requested_position] != *id || !column) {
            throw std::runtime_error("invalid leaderboard column");
          }
          row.columns.push_back(std::move(*column));
          ++requested_position;
        }
        result.page.rows.push_back(std::move(row));
        ++row_index;
      }
    } catch (...) {
      result.status = StatsServiceStatus::kInvalidResponse;
      result.page = {};
      return result;
    }
    result.status = StatsServiceStatus::kSuccess;
    REXLOG_INFO(
        "community-stats-op operation=leaderboard-read view={:08X} attributes16={} "
        "total={} page={} status={} error=-",
        view_id, attribute_ids.size(), result.page.total, result.page.rows.size(), response.status);
    return result;
  }

  std::optional<std::vector<uint32_t>> FetchUnlockedAchievements() override {
    const HttpResponse response =
        state_->Request("GET", "/api/v2/achievements", nullptr, true);
    if (response.status != 200) return std::nullopt;
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.is_object() || decoded.size() != 1 ||
          !decoded.contains("achievement_ids") ||
          !decoded.at("achievement_ids").is_array() ||
          decoded.at("achievement_ids").size() > kLastGta4AchievementId) {
        return std::nullopt;
      }
      std::unordered_set<uint32_t> unique;
      std::vector<uint32_t> result;
      result.reserve(decoded.at("achievement_ids").size());
      for (const auto& wire_id : decoded.at("achievement_ids")) {
        if (!wire_id.is_number_unsigned()) return std::nullopt;
        const uint32_t id = wire_id.get<uint32_t>();
        if (id < kFirstGta4AchievementId || id > kLastGta4AchievementId ||
            !unique.insert(id).second) {
          return std::nullopt;
        }
        result.push_back(id);
      }
      return result;
    } catch (...) {
      return std::nullopt;
    }
  }

  bool MergeUnlockedAchievements(std::span<const uint32_t> achievement_ids) override {
    if (achievement_ids.size() > kLastGta4AchievementId) return false;
    std::unordered_set<uint32_t> unique;
    json encoded = json::array();
    for (const uint32_t id : achievement_ids) {
      if (id < kFirstGta4AchievementId || id > kLastGta4AchievementId ||
          !unique.insert(id).second) {
        return false;
      }
      encoded.push_back(id);
    }
    const json body = {{"achievement_ids", std::move(encoded)}};
    const HttpResponse response =
        state_->Request("POST", "/api/v2/achievements", &body, true);
    if (response.status != 200) return false;
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.is_object() || decoded.size() != 1 ||
          !decoded.contains("achievement_ids") ||
          !decoded.at("achievement_ids").is_array() ||
          decoded.at("achievement_ids").size() > kLastGta4AchievementId) {
        return false;
      }
      std::unordered_set<uint32_t> remote;
      for (const auto& wire_id : decoded.at("achievement_ids")) {
        if (!wire_id.is_number_unsigned()) return false;
        const uint32_t id = wire_id.get<uint32_t>();
        if (id < kFirstGta4AchievementId || id > kLastGta4AchievementId ||
            !remote.insert(id).second) {
          return false;
        }
      }
      return std::ranges::all_of(achievement_ids,
                                 [&](uint32_t id) { return remote.contains(id); });
    } catch (...) {
      return false;
    }
  }

  std::optional<std::vector<std::string>> FetchEpisodePackages() override {
    const HttpResponse response =
        state_->Request("GET", "/api/v2/entitlements", nullptr, true);
    if (response.status != 200) return std::nullopt;
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.is_object() || decoded.size() != 1 || !decoded.contains("packages") ||
          !decoded.at("packages").is_array() ||
          decoded.at("packages").size() > kKnownGta4EpisodePackages.size()) {
        return std::nullopt;
      }
      std::unordered_set<std::string> unique;
      std::vector<std::string> result;
      result.reserve(decoded.at("packages").size());
      for (const auto& wire_package : decoded.at("packages")) {
        if (!wire_package.is_string()) return std::nullopt;
        const std::string package = wire_package.get<std::string>();
        if (std::ranges::find(kKnownGta4EpisodePackages, package) ==
                kKnownGta4EpisodePackages.end() ||
            !unique.insert(package).second) {
          return std::nullopt;
        }
        result.push_back(package);
      }
      std::ranges::sort(result);
      return result;
    } catch (...) {
      return std::nullopt;
    }
  }

  std::optional<TitleProfileRecord> ParseTitleProfileRecord(const HttpResponse& response,
                                                            uint32_t title_id) const {
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.is_object() || decoded.value("title_id", std::string{}) != "0x545407f2" ||
          decoded.value("setting_id", std::string{}) != "0x63e83fff" ||
          !decoded.contains("revision") || !decoded.at("revision").is_string() ||
          !decoded.contains("blob") || !decoded.at("blob").is_string()) {
        return std::nullopt;
      }
      const auto revision =
          ParseDecimalUint64(decoded.at("revision").get<std::string>());
      auto blob = Base64Decode(decoded.at("blob").get<std::string>(), true,
                               kMaximumGta4TitleProfileBytes);
      if (!revision || *revision > static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) ||
          !blob || !IsValidGta4TitleProfileBlob(*blob)) {
        return std::nullopt;
      }
      return TitleProfileRecord{
          .title_id = title_id,
          .xuid = state_->identity().xuid,
          .revision = static_cast<int64_t>(*revision),
          .blob = std::move(*blob),
      };
    } catch (...) {
      return std::nullopt;
    }
  }

  void SetConnectionRestoredHandler(std::function<void()> handler) override {
    state_->SetConnectionRestoredHandler(std::move(handler));
  }

  TitleProfileFetchResult Fetch(uint32_t title_id) override {
    if (title_id != kGta4TitleId) return {};
    const HttpResponse response = state_->Request(
        "GET", "/api/v3/profile/title-settings/0x63e83fff", nullptr, true);
    if (response.status == 404) {
      return {.status = TitleProfileFetchStatus::kNotFound};
    }
    if (response.status != 200) return {};
    auto record = ParseTitleProfileRecord(response, title_id);
    if (!record) return {};
    return {.status = TitleProfileFetchStatus::kFound, .record = std::move(record)};
  }

  TitleProfileStoreResult Store(uint32_t title_id, int64_t expected_revision,
                                std::span<const uint8_t> blob) override {
    if (title_id != kGta4TitleId || expected_revision < 0 ||
        !IsValidGta4TitleProfileBlob(blob)) {
      return {};
    }
    const auto encoded = Base64Encode(blob, true);
    if (!encoded) return {};
    const json body = {
        {"title_id", "0x545407f2"},
        {"setting_id", "0x63e83fff"},
        {"expected_revision", std::to_string(expected_revision)},
        {"blob", *encoded},
    };
    const HttpResponse response = state_->Request(
        "PUT", "/api/v3/profile/title-settings/0x63e83fff", &body, true);
    if (response.status == 412) {
      auto current = Fetch(title_id);
      if (current.status != TitleProfileFetchStatus::kFound || !current.record) return {};
      if (std::ranges::equal(current.record->blob, blob)) {
        return {.status = TitleProfileStoreStatus::kUpdated,
                .revision = current.record->revision};
      }
      return {.status = TitleProfileStoreStatus::kConflict,
              .revision = current.record->revision,
              .conflict_record = std::move(current.record)};
    }
    if (response.status != 200) return {};
    auto record = ParseTitleProfileRecord(response, title_id);
    if (!record || !detail::IsAcknowledgedTitleProfileBlob(blob, record->blob)) {
      return {};
    }
    return {.status = TitleProfileStoreStatus::kUpdated,
            .revision = record->revision};
  }

  std::optional<Gta4AchievementStorageRecord> ParseGta4AchievementStorageRecord(
      const HttpResponse& response) const {
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.is_object() || decoded.size() != 4 ||
          decoded.value("title_id", std::string{}) != "0x545407F2" ||
          decoded.value("facility", 0U) !=
              rex::system::xam::kGta4AchievementStorageFacility ||
          decoded.value("path", std::string{}) !=
              rex::system::xam::kGta4AchievementStoragePath ||
          !decoded.contains("blob") || !decoded.at("blob").is_string()) {
        return std::nullopt;
      }
      const std::string encoded = decoded.at("blob").get<std::string>();
      auto blob = Base64Decode(encoded, true,
                               rex::system::xam::kGta4AchievementStorageBlobBytes);
      if (!blob || blob->size() !=
                       rex::system::xam::kGta4AchievementStorageBlobBytes) {
        return std::nullopt;
      }
      const auto canonical = Base64Encode(*blob, true);
      if (!canonical || *canonical != encoded) return std::nullopt;
      Gta4AchievementStorageRecord record{.xuid = state_->identity().xuid};
      std::ranges::copy(*blob, record.blob.begin());
      return record;
    } catch (...) {
      return std::nullopt;
    }
  }

  Gta4AchievementStorageFetchResult FetchGta4AchievementStorage() override {
    const HttpResponse response = state_->Request(
        "GET", std::string(rex::system::xam::kGta4AchievementStorageEndpoint),
        nullptr, true);
    if (response.status == 404) {
      try {
        const json decoded = json::parse(response.body);
        if (decoded.is_object() && decoded.size() == 1 &&
            decoded.contains("error") && decoded.at("error").is_object() &&
            decoded.at("error").size() == 2 &&
            decoded.at("error").value("code", std::string{}) ==
                "storage_file_not_found" &&
            decoded.at("error").value("message", std::string{}) ==
                "Prog_ACH does not exist") {
          return {.status = Gta4AchievementStorageFetchStatus::kNotFound};
        }
      } catch (...) {
      }
      return {};
    }
    if (response.status != 200) return {};
    auto record = ParseGta4AchievementStorageRecord(response);
    if (!record) return {};
    return {.status = Gta4AchievementStorageFetchStatus::kFound,
            .record = std::move(record)};
  }

  bool StoreGta4AchievementStorage(std::span<const uint8_t> blob) override {
    if (blob.size() != rex::system::xam::kGta4AchievementStorageBlobBytes) {
      return false;
    }
    const auto encoded = Base64Encode(blob, true);
    if (!encoded) return false;
    const json body = {{"blob", *encoded}};
    const HttpResponse response = state_->Request(
        "PUT", std::string(rex::system::xam::kGta4AchievementStorageEndpoint),
        &body, true);
    if (response.status != 200) return false;
    const auto record = ParseGta4AchievementStorageRecord(response);
    return record && record->xuid == state_->identity().xuid &&
           std::ranges::equal(record->blob, blob);
  }

  bool Configure(const VoiceRoute& route) override {
    std::unordered_set<uint64_t> unique_targets;
    std::unordered_set<uint64_t> unique_muted;
    if (!route.session_id || route.target_xuids.size() > rex::system::xam::kMaximumSessionMembers ||
        route.muted_xuids.size() > rex::system::xam::kMaximumSessionMembers ||
        std::ranges::any_of(route.target_xuids, [&unique_targets](uint64_t xuid) {
          return !xuid || !unique_targets.insert(xuid).second;
        }) ||
        std::ranges::any_of(route.muted_xuids, [&unique_muted](uint64_t xuid) {
          return !xuid || !unique_muted.insert(xuid).second;
        })) {
      return false;
    }
    std::lock_guard request_lock(voice_config_request_mutex_);
    std::string replaced_token;
    {
      std::lock_guard lock(voice_mutex_);
      if (voice_stopping_) return false;
      replaced_token = voice_route_token_;
    }
    const auto token = CreateVoiceRoute(route, replaced_token);
    if (!token) return false;

    bool stopped = false;
    {
      std::lock_guard lock(voice_mutex_);
      stopped = voice_stopping_;
      if (!stopped) {
        voice_route_token_ = *token;
        voice_session_id_ = route.session_id;
        configured_voice_route_ = route;
        voice_recovery_gate_.OnConfigured();
        voice_recovery_retry_at_ = {};
        voice_after_.clear();
        received_voice_packets_.clear();
        // A title-requested channel/session change is a routing boundary.  Do
        // not leak already-captured speech to the replacement audience.
        pending_voice_packets_.clear();
        pending_voice_encoded_bytes_ = 0;
      }
    }
    if (stopped) {
      DeleteVoiceRoute(*token, {});
      return false;
    }
    voice_condition_.notify_all();
    // Current libserver replaces this token atomically during route creation.
    // Keep the authenticated DELETE for compatibility with an older server
    // that ignored replace_route_token, and for deterministic stale cleanup.
    DeleteVoiceRoute(replaced_token, *token);
    return true;
  }

  bool Send(uint32_t sequence, std::span<const uint8_t> payload) override {
    if (payload.empty() || payload.size() > kMaximumVoicePacketBytes) return false;
    const auto encoded = Base64Encode(payload, false);
    if (!encoded) return false;
    {
      std::lock_guard lock(voice_mutex_);
      if (!IsSafeOpaqueToken(voice_route_token_) ||
          pending_voice_packets_.size() >= kMaximumPendingVoicePackets ||
          encoded->size() >
              kMaximumPendingVoiceEncodedBytes - pending_voice_encoded_bytes_) {
        return false;
      }
      pending_voice_encoded_bytes_ += encoded->size();
      pending_voice_packets_.push_back(
          {.encoded_payload_bytes = encoded->size(),
           .wire = {{"route_token", voice_route_token_},
                    {"sequence", sequence},
                    {"payload", std::move(*encoded)}}});
    }
    voice_condition_.notify_all();
    return true;
  }

  std::optional<VoicePacket> Receive(uint32_t maximum_payload_size) override {
    std::lock_guard lock(voice_mutex_);
    while (!received_voice_packets_.empty()) {
      VoicePacket packet = std::move(received_voice_packets_.front());
      received_voice_packets_.pop_front();
      if (packet.payload.size() <= maximum_payload_size) return packet;
    }
    return std::nullopt;
  }

  void Close() override {
    std::lock_guard request_lock(voice_config_request_mutex_);
    std::string token;
    {
      std::lock_guard lock(voice_mutex_);
      token = std::move(voice_route_token_);
      voice_session_id_ = 0;
      configured_voice_route_.reset();
      voice_recovery_gate_.OnClosed();
      voice_recovery_retry_at_ = {};
      voice_after_.clear();
      received_voice_packets_.clear();
      pending_voice_packets_.clear();
      pending_voice_encoded_bytes_ = 0;
    }
    voice_condition_.notify_all();
    DeleteVoiceRoute(token, {});
  }

  bool Configure(uint64_t session_id) override {
    if (!session_id) return false;
    {
      std::lock_guard lock(text_mutex_);
      if (text_stopping_) return false;
      if (text_session_id_ == session_id) return true;
      if (text_session_id_) {
        text_last_session_id_ = text_session_id_;
        text_last_after_ = text_after_;
      }
      text_session_id_ = session_id;
      text_after_ = text_last_session_id_ == session_id ? text_last_after_ : 0;
      pending_text_messages_.clear();
      received_text_messages_.clear();
    }
    text_condition_.notify_all();
    return true;
  }

  bool Send(TextChatChannel channel, std::span<const uint64_t> target_xuids,
            uint32_t sequence, std::string_view text) override {
    std::unordered_set<uint64_t> unique_targets;
    if (text.empty() || text.size() > kMaximumTextChatBytes ||
        text.find_first_not_of(" \t") == std::string_view::npos ||
        text.find_first_of("\r\n") != std::string_view::npos ||
        text.find('\0') != std::string_view::npos ||
        !utf8::is_valid(text.begin(), text.end()) ||
        target_xuids.size() > rex::system::xam::kMaximumSessionMembers ||
        std::ranges::any_of(target_xuids, [&unique_targets](uint64_t xuid) {
          return !xuid || !unique_targets.insert(xuid).second;
        }) ||
        (channel == TextChatChannel::kTeam && target_xuids.empty()) ||
        (channel == TextChatChannel::kAll && !target_xuids.empty())) {
      return false;
    }

    OutboundTextChatMessage message;
    message.channel = channel;
    message.target_xuids.assign(target_xuids.begin(), target_xuids.end());
    message.sequence = sequence;
    message.text.assign(text);
    message.idempotency = IdempotencyKey();
    if (message.idempotency.empty()) return false;
    {
      std::lock_guard lock(text_mutex_);
      if (text_stopping_ || !text_session_id_ ||
          pending_text_messages_.size() >= kMaximumPendingTextMessages) {
        return false;
      }
      pending_text_messages_.push_back(std::move(message));
    }
    text_condition_.notify_all();
    return true;
  }

  std::vector<TextChatMessage> ReceiveMessages(uint32_t maximum_messages) override {
    std::vector<TextChatMessage> messages;
    if (!maximum_messages) return messages;
    std::lock_guard lock(text_mutex_);
    const size_t count = std::min<size_t>(maximum_messages,
                                         received_text_messages_.size());
    messages.reserve(count);
    while (messages.size() < count) {
      messages.push_back(std::move(received_text_messages_.front()));
      received_text_messages_.pop_front();
    }
    return messages;
  }

  void CloseTextChat() override {
    {
      std::lock_guard lock(text_mutex_);
      if (text_session_id_) {
        text_last_session_id_ = text_session_id_;
        text_last_after_ = text_after_;
      }
      text_session_id_ = 0;
      pending_text_messages_.clear();
      received_text_messages_.clear();
    }
    text_condition_.notify_all();
  }

 private:
  std::optional<std::string> CreateVoiceRoute(
      const VoiceRoute& route, std::string_view replaced_token) {
    const char* channel = route.channel == VoiceChannel::kPrivate ? "private"
                          : route.channel == VoiceChannel::kTeam  ? "team"
                                                                  : "all";
    json targets = json::array();
    json muted = json::array();
    for (uint64_t xuid : route.target_xuids) targets.push_back(Hex64(xuid));
    for (uint64_t xuid : route.muted_xuids) muted.push_back(Hex64(xuid));
    json body = {{"session_id", Hex64(route.session_id)},
                 {"channel", channel},
                 {"target_xuids", std::move(targets)},
                 {"mute_xuids", std::move(muted)}};
    if (IsSafeOpaqueToken(replaced_token)) {
      body["replace_route_token"] = replaced_token;
    }
    const HttpResponse response =
        state_->Request("POST", "/api/v2/voice/routes", &body, true);
    if (response.status != 200 && response.status != 201) return std::nullopt;
    try {
      const json decoded = json::parse(response.body);
      const std::string token = decoded.at("route_token").get<std::string>();
      return IsSafeOpaqueToken(token) ? std::optional(token) : std::nullopt;
    } catch (...) {
      return std::nullopt;
    }
  }

  void DeleteVoiceRoute(std::string_view token,
                        std::string_view replacement_token) {
    if (!IsSafeOpaqueToken(token) || token == replacement_token) return;
    const json body = {{"route_token", token}};
    (void)state_->Request("DELETE", "/api/v2/voice/routes", &body, true, {},
                          kRelayPollTimeoutMilliseconds);
  }

  bool RecoverVoiceRoute(const std::string& observed_token,
                         uint64_t observed_session_id, long http_status) {
    std::lock_guard request_lock(voice_config_request_mutex_);
    VoiceRoute route;
    {
      std::lock_guard lock(voice_mutex_);
      const auto now = std::chrono::steady_clock::now();
      const bool observed_route_is_current =
          voice_route_token_ == observed_token &&
          voice_session_id_ == observed_session_id;
      const bool retry_window_open =
          voice_recovery_retry_at_ == std::chrono::steady_clock::time_point{} ||
          now >= voice_recovery_retry_at_;
      if (!configured_voice_route_ ||
          !voice_recovery_gate_.Begin(http_status,
                                      observed_route_is_current,
                                      retry_window_open)) {
        return false;
      }
      route = *configured_voice_route_;
    }

    const auto replacement_token = CreateVoiceRoute(route, observed_token);
    bool committed = false;
    {
      std::lock_guard lock(voice_mutex_);
      const bool observed_route_is_current =
          voice_route_token_ == observed_token &&
          voice_session_id_ == observed_session_id;
      const bool configured_route_is_current =
          !voice_stopping_ && configured_voice_route_ &&
          *configured_voice_route_ == route;
      if (replacement_token &&
          voice_recovery_gate_.CanCommit(observed_route_is_current,
                                         configured_route_is_current)) {
        voice_route_token_ = *replacement_token;
        voice_after_.clear();
        for (auto& packet : pending_voice_packets_) {
          packet.wire["route_token"] = *replacement_token;
        }
        voice_recovery_retry_at_ = {};
        committed = true;
      } else if (observed_route_is_current && configured_route_is_current) {
        voice_recovery_retry_at_ =
            std::chrono::steady_clock::now() + kRouteRetryInterval;
      }
      voice_recovery_gate_.Finish();
    }
    voice_condition_.notify_all();

    if (committed) {
      REXLOG_INFO(
          "Community voice route recovered for session {:016X} after HTTP {}",
          observed_session_id, http_status);
      return true;
    }
    if (replacement_token) {
      DeleteVoiceRoute(*replacement_token, {});
    }
    return false;
  }

  void SynchronizePendingInvitations() {
    const size_t page_size = rex::system::xam::detail::kCommunityInvitationPageSize;
    const HttpResponse page = state_->Request(
        "GET", "/api/v2/invites?limit=" + std::to_string(page_size) + "&state=pending",
        nullptr, true, {},
        rex::system::xam::detail::kCommunitySocialRequestTimeoutMilliseconds);
    if (page.status != 200) return;
    try {
      const json decoded = json::parse(page.body);
      const auto& items = decoded.at("items");
      if (!items.is_array() || items.size() > page_size) return;
      std::vector<InvitationRecord> invitations;
      std::vector<std::string> observed_ids;
      invitations.reserve(items.size());
      observed_ids.reserve(items.size());
      for (const auto& item : items) {
        if (item.value("state", std::string{}) != "pending" ||
            item.value("acknowledged", true)) {
          return;
        }
        auto invitation = InvitationFromJson(item);
        if (!invitation || invitation->recipient_xuid != state_->identity().xuid ||
            invitation->revision < 1) {
          return;
        }
        observed_ids.push_back(invitation->id);
        invitations.push_back(std::move(*invitation));
      }

      for (InvitationRecord& invitation : invitations) {
        if (social_cache_.PendingDescriptorCurrent(invitation)) continue;
        const HttpResponse session_response = state_->Request(
            "GET", "/api/v2/sessions/by-xbox-id/" + Hex64(invitation.session_id), nullptr,
            true, {}, rex::system::xam::detail::kCommunitySocialRequestTimeoutMilliseconds);
        if (session_response.status != 200) continue;
        const json session_json = json::parse(session_response.body);
        int64_t revision = 0;
        int64_t host_epoch = 0;
        auto session = SessionFromJson(session_json, &revision, &host_epoch);
        if (!session || revision < 1 || host_epoch < 1 ||
            (session->session_id != invitation.session_id &&
             session->previous_session_id != invitation.session_id)) {
          continue;
        }
        invitation.session_id = session->session_id;
        invitation.session = std::move(*session);
        const std::string idempotency = IdempotencyKey();
        if (!idempotency.empty()) {
          (void)social_cache_.ObservePending(std::move(invitation), idempotency);
        }
      }
      social_cache_.RetainPending(observed_ids);
      (void)social_cache_.PresentNextPending();
    } catch (...) {
      return;
    }
  }

  void RefreshRelationshipCache() {
    std::unordered_set<uint64_t> unique_xuids;
    std::unordered_set<std::string> used_cursors;
    std::vector<FriendRecord> relationships;
    relationships.reserve(
        rex::system::xam::detail::kCommunityRelationshipCacheEntryLimit);
    std::optional<uint64_t> previous_xuid;
    std::string cursor;

    for (size_t page_index = 0;
         page_index < rex::system::xam::detail::kCommunityRelationshipMaximumPages;
         ++page_index) {
      {
        std::lock_guard lock(social_mutex_);
        if (social_stopping_) return;
      }
      std::string path =
          "/api/v2/friends?limit=" +
          std::to_string(rex::system::xam::detail::kCommunityRelationshipPageSize);
      if (!cursor.empty()) path += "&cursor=" + cursor;
      const HttpResponse response = state_->Request(
          "GET", path, nullptr, true, {},
          rex::system::xam::detail::kCommunitySocialRequestTimeoutMilliseconds);
      if (response.status != 200) return;

      try {
        const json decoded = json::parse(response.body);
        const auto& items = decoded.at("items");
        if (!decoded.is_object() || !items.is_array() ||
            !decoded.contains("next_cursor") ||
            !decoded.at("next_cursor").is_string() ||
            items.size() >
                rex::system::xam::detail::kCommunityRelationshipPageSize) {
          return;
        }
        for (const auto& item : items) {
          auto relationship = FriendFromJson(item);
          if (!relationship ||
              relationships.size() >=
                  rex::system::xam::detail::kCommunityRelationshipCacheEntryLimit ||
              !unique_xuids.insert(relationship->xuid).second ||
              (previous_xuid && relationship->xuid <= *previous_xuid)) {
            return;
          }
          previous_xuid = relationship->xuid;
          relationships.push_back(std::move(*relationship));
        }

        const std::string next_cursor =
            decoded.value("next_cursor", std::string{});
        if (next_cursor.empty()) {
          std::lock_guard lock(social_mutex_);
          if (!social_stopping_) {
            social_cache_.ReplaceRelationships(relationships);
          }
          return;
        }
        const auto parsed_cursor = ParseFixedHex<uint64_t>(next_cursor, 16);
        if (!parsed_cursor || Hex64(*parsed_cursor) != next_cursor ||
            !previous_xuid || *parsed_cursor != *previous_xuid ||
            !used_cursors.insert(next_cursor).second) {
          return;
        }
        cursor = next_cursor;
      } catch (...) {
        return;
      }
    }
  }

  void SocialWorkerMain() {
    auto next_relationship_refresh = std::chrono::steady_clock::time_point{};
    for (;;) {
      {
        std::lock_guard lock(social_mutex_);
        if (social_stopping_) return;
      }

      const auto now = std::chrono::steady_clock::now();
      if (now >= next_relationship_refresh) {
        RefreshRelationshipCache();
        next_relationship_refresh =
            now + rex::system::xam::detail::kCommunityRelationshipRefreshInterval;
      }

      SynchronizePendingInvitations();

      std::unique_lock lock(social_mutex_);
      if (social_condition_.wait_for(
              lock, rex::system::xam::detail::kCommunityInvitePollInterval,
              [this] { return social_stopping_; })) {
        return;
      }
    }
  }

  void WaitForVoicePollDelay(const std::string& token) {
    std::unique_lock lock(voice_mutex_);
    voice_condition_.wait_for(
        lock, std::chrono::milliseconds(kRelayLongPollMilliseconds),
        [&] { return voice_stopping_ || voice_route_token_ != token; });
  }

  void VoiceSendWorkerMain() {
    for (;;) {
      json packets = json::array();
      std::string route_token;
      uint64_t session_id = 0;
      {
        std::unique_lock lock(voice_mutex_);
        voice_condition_.wait(lock,
                              [&] { return voice_stopping_ || !pending_voice_packets_.empty(); });
        if (voice_stopping_) return;

        const auto deadline = std::chrono::steady_clock::now() + kRelayBatchDelay;
        voice_condition_.wait_until(lock, deadline, [&] { return voice_stopping_; });
        if (voice_stopping_) return;

        for (const auto& pending : pending_voice_packets_) {
          if (packets.size() >= kMaximumVoiceBatchPackets) break;
          packets.push_back(pending.wire);
        }
        route_token = voice_route_token_;
        session_id = voice_session_id_;
      }
      if (packets.empty()) continue;
      const size_t packet_count = packets.size();
      const json batch = {{"packets", std::move(packets)}};
      const HttpResponse response =
          state_->Request("POST", "/api/v2/voice/packets", &batch, true);
      if (response.status < 200 || response.status >= 300) {
        // Send() promises ownership only after the packet is durably accepted
        // into this bounded queue. Keep it until libserver acknowledges the
        // batch; server-side sequence de-duplication makes response-loss
        // retries safe.
        if (!RecoverVoiceRoute(route_token, session_id, response.status)) {
          WaitForVoicePollDelay(route_token);
        }
        continue;
      }
      std::lock_guard lock(voice_mutex_);
      if (voice_route_token_ != route_token) continue;
      for (size_t index = 0;
           index < packet_count && !pending_voice_packets_.empty(); ++index) {
        pending_voice_encoded_bytes_ -=
            pending_voice_packets_.front().encoded_payload_bytes;
        pending_voice_packets_.pop_front();
      }
    }
  }

  void VoiceReceiveWorkerMain() {
    for (;;) {
      std::string token;
      std::string after;
      uint64_t session_id = 0;
      {
        std::unique_lock lock(voice_mutex_);
        voice_condition_.wait(lock, [&] {
          return voice_stopping_ || IsSafeOpaqueToken(voice_route_token_);
        });
        if (voice_stopping_) return;
        token = voice_route_token_;
        after = voice_after_;
        session_id = voice_session_id_;
      }

      if (!session_id ||
          (!after.empty() &&
           !gta4::network::detail::IsCanonicalVoiceCursor(after))) {
        WaitForVoicePollDelay(token);
        continue;
      }
      std::string path = "/api/v2/voice/packets?route_token=" + token +
                         "&wait_ms=" + std::to_string(kRelayLongPollMilliseconds);
      if (!after.empty()) path += "&after=" + after;
      const HttpResponse response = state_->Request(
          "GET", path, nullptr, true, {}, kRelayPollTimeoutMilliseconds);
      if (response.status == 409) {
        std::lock_guard lock(voice_mutex_);
        if (voice_route_token_ == token && voice_session_id_ == session_id &&
            voice_after_ == after) {
          voice_after_.clear();
        }
        continue;
      }
      if (response.status != 200) {
        if (!RecoverVoiceRoute(token, session_id, response.status)) {
          WaitForVoicePollDelay(token);
        }
        continue;
      }

      try {
        const json decoded = json::parse(response.body);
        const auto& packets = decoded.at("packets");
        if (!packets.is_array() || packets.size() > kMaximumVoiceBatchPackets) {
          WaitForVoicePollDelay(token);
          continue;
        }
        std::deque<VoicePacket> received;
        for (const auto& wire : packets) {
          const auto source =
              ParseFixedHex<uint64_t>(wire.at("source_xuid").get<std::string>(), 16);
          const auto session =
              ParseFixedHex<uint64_t>(wire.at("session_id").get<std::string>(), 16);
          auto payload = Base64Decode(wire.at("payload").get<std::string>(), false,
                                      kMaximumVoicePacketBytes);
          if (!source || !*source || *source == state_->identity().xuid || !session ||
              *session != session_id || !payload || payload->empty()) {
            received.clear();
            break;
          }
          received.push_back({.source_xuid = *source,
                              .session_id = *session,
                              .sequence = wire.at("sequence").get<uint32_t>(),
                              .payload = std::move(*payload)});
        }
        const std::string next = decoded.value("next_after", std::string{});
        if (!next.empty() &&
            !gta4::network::detail::IsCanonicalVoiceCursor(next)) {
          WaitForVoicePollDelay(token);
          continue;
        }

        const bool had_packets = !received.empty();
        {
          std::lock_guard lock(voice_mutex_);
          if (voice_route_token_ != token || voice_session_id_ != session_id) continue;
          voice_after_ = next;
          while (!received.empty()) {
            if (received_voice_packets_.size() >= kMaximumPendingVoicePackets) {
              received_voice_packets_.pop_front();
            }
            received_voice_packets_.push_back(std::move(received.front()));
            received.pop_front();
          }
        }
        if (!had_packets) {
          WaitForVoicePollDelay(token);
        }
      } catch (...) {
        WaitForVoicePollDelay(token);
      }
    }
  }

  void TextSendWorkerMain() {
    for (;;) {
      OutboundTextChatMessage message;
      uint64_t session_id = 0;
      {
        std::unique_lock lock(text_mutex_);
        text_condition_.wait(lock, [&] {
          return text_stopping_ ||
                 (text_session_id_ && !pending_text_messages_.empty());
        });
        if (text_stopping_) return;
        session_id = text_session_id_;
        // Keep the accepted item in the bounded queue until the server either
        // acknowledges it or rejects it permanently.  Removing it while the
        // request is in flight would let concurrent Send() calls refill the
        // queue and force a response-loss retry to drop an item that Send()
        // already accepted.
        message = pending_text_messages_.front();
      }

      json targets = json::array();
      for (const uint64_t xuid : message.target_xuids) {
        targets.push_back(Hex64(xuid));
      }
      const json body = {
          {"session_id", Hex64(session_id)},
          {"channel", message.channel == TextChatChannel::kTeam ? "team" : "all"},
          {"target_xuids", std::move(targets)},
          {"sequence", message.sequence},
          {"text", message.text},
      };
      const std::array<std::string, 1> headers = {
          "Idempotency-Key: " + message.idempotency};
      const HttpResponse response = state_->Request(
          "POST", "/api/v2/chat/messages", &body, true, headers);
      if (response.status != 200 && response.status != 201) {
        if (IsRetryableRealtimeRequest(response.status)) {
          WaitForTextPollDelay(session_id);
          continue;
        }
        REXSYS_WARN("GTA IV text chat send failed: HTTP {}", response.status);
      }
      std::lock_guard lock(text_mutex_);
      if (text_session_id_ == session_id && !pending_text_messages_.empty() &&
          pending_text_messages_.front().idempotency == message.idempotency) {
        pending_text_messages_.pop_front();
      }
    }
  }

  void WaitForTextPollDelay(uint64_t session_id) {
    std::unique_lock lock(text_mutex_);
    text_condition_.wait_for(lock, std::chrono::milliseconds(kRelayLongPollMilliseconds),
                             [&] {
                               return text_stopping_ ||
                                      text_session_id_ != session_id;
                             });
  }

  void TextReceiveWorkerMain() {
    for (;;) {
      uint64_t session_id = 0;
      int64_t after = 0;
      {
        std::unique_lock lock(text_mutex_);
        text_condition_.wait(lock,
                             [&] { return text_stopping_ || text_session_id_ != 0; });
        if (text_stopping_) return;
        session_id = text_session_id_;
        after = text_after_;
      }

      const std::string path =
          "/api/v2/events?after=" + std::to_string(after) +
          "&wait_ms=" + std::to_string(kTextChatPollWaitMilliseconds);
      const HttpResponse response = state_->Request(
          "GET", path, nullptr, true, {}, kTextChatPollTimeoutMilliseconds);
      if (response.status == 409) {
        std::lock_guard lock(text_mutex_);
        if (text_session_id_ == session_id && text_after_ == after) {
          // libserver event queues are intentionally realtime and ephemeral.
          // A future cursor after a server restart is explicitly rejected so
          // the client can reopen the current stream from its new origin.
          text_after_ = 0;
          text_last_session_id_ = session_id;
          text_last_after_ = 0;
        }
        continue;
      }
      if (response.status != 200) {
        WaitForTextPollDelay(session_id);
        continue;
      }

      try {
        const json decoded = json::parse(response.body);
        const auto& events = decoded.at("events");
        if (!events.is_array() || events.size() > kMaximumTextEventPage) {
          WaitForTextPollDelay(session_id);
          continue;
        }

        int64_t next_after = after;
        std::deque<TextChatMessage> received;
        for (const auto& event : events) {
          const int64_t event_id = event.at("id").get<int64_t>();
          if (event_id <= next_after) {
            throw std::runtime_error("non-monotonic event cursor");
          }
          next_after = event_id;
          if (event.value("type", std::string{}) != "chat.message" ||
              event.value("aggregate_type", std::string{}) != "session-chat") {
            continue;
          }
          const auto aggregate_session = ParseFixedHex<uint64_t>(
              event.at("aggregate_id").get<std::string>(), 16);
          const auto& payload = event.at("payload");
          const auto source = ParseFixedHex<uint64_t>(
              payload.at("source_xuid").get<std::string>(), 16);
          const auto wire_session = ParseFixedHex<uint64_t>(
              payload.at("session_id").get<std::string>(), 16);
          const std::string channel = payload.at("channel").get<std::string>();
          const std::string player_name = BoundedPlayerName(
              payload.at("player_name").get<std::string>());
          const std::string text = payload.at("text").get<std::string>();
          if (!aggregate_session || *aggregate_session != session_id || !source ||
              !*source || *source == state_->identity().xuid || !wire_session ||
              *wire_session != session_id ||
              (channel != "all" && channel != "team") || text.empty() ||
              text.size() > kMaximumTextChatBytes ||
              !utf8::is_valid(text.begin(), text.end()) ||
              social_cache_.QueryMute(*source) == CachedMuteState::kMuted) {
            continue;
          }
          received.push_back({
              .source_xuid = *source,
              .session_id = session_id,
              .sequence = payload.at("sequence").get<uint32_t>(),
              .channel = channel == "team" ? TextChatChannel::kTeam
                                            : TextChatChannel::kAll,
              .player_name = player_name,
              .text = text,
          });
        }

        const bool had_events = !events.empty();
        {
          std::lock_guard lock(text_mutex_);
          if (text_session_id_ != session_id) continue;
          text_after_ = next_after;
          text_last_session_id_ = session_id;
          text_last_after_ = next_after;
          while (!received.empty()) {
            if (received_text_messages_.size() >=
                kMaximumReceivedTextMessages) {
              received_text_messages_.pop_front();
            }
            received_text_messages_.push_back(std::move(received.front()));
            received.pop_front();
          }
        }
        if (!had_events) WaitForTextPollDelay(session_id);
      } catch (...) {
        WaitForTextPollDelay(session_id);
      }
    }
  }

  std::shared_ptr<CommunityState> state_;
  std::mutex stats_write_mutex_;
  std::optional<PendingStatWrite> pending_stat_write_;
  rex::system::xam::detail::SocialCacheState social_cache_;
  std::mutex social_mutex_;
  std::condition_variable social_condition_;
  bool social_stopping_ = false;
  std::thread social_worker_;
  std::mutex voice_mutex_;
  std::mutex voice_config_request_mutex_;
  std::condition_variable voice_condition_;
  bool voice_stopping_ = false;
  std::string voice_route_token_;
  uint64_t voice_session_id_ = 0;
  std::optional<VoiceRoute> configured_voice_route_;
  gta4::network::detail::VoiceRouteRecoveryGate voice_recovery_gate_;
  std::chrono::steady_clock::time_point voice_recovery_retry_at_{};
  std::string voice_after_;
  std::deque<VoicePacket> received_voice_packets_;
  std::deque<OutboundVoicePacket> pending_voice_packets_;
  size_t pending_voice_encoded_bytes_ = 0;
  std::thread voice_send_worker_;
  std::thread voice_receive_worker_;
  std::mutex text_mutex_;
  std::condition_variable text_condition_;
  bool text_stopping_ = false;
  uint64_t text_session_id_ = 0;
  int64_t text_after_ = 0;
  uint64_t text_last_session_id_ = 0;
  int64_t text_last_after_ = 0;
  std::deque<OutboundTextChatMessage> pending_text_messages_;
  std::deque<TextChatMessage> received_text_messages_;
  std::thread text_send_worker_;
  std::thread text_receive_worker_;
  std::once_flag shutdown_once_;
};

class SessionObserver {
 public:
  virtual ~SessionObserver() = default;
  virtual void ObserveSession(const SessionRecord& session) = 0;
  virtual void ForgetSession(uint64_t session_id) = 0;
};

class CommunityQosService final : public IQosService {
 public:
  explicit CommunityQosService(std::shared_ptr<CommunityState> state)
      : state_(std::move(state)), probe_worker_([this] { ProbeWorkerMain(); }) {}

  ~CommunityQosService() override {
    {
      std::lock_guard lock(probe_mutex_);
      probe_stopping_ = true;
    }
    probe_condition_.notify_all();
    if (probe_worker_.joinable()) probe_worker_.join();
  }

  bool ready() const override { return state_->ready(); }
  std::string last_error() const override { return state_->last_error(); }

  bool UpdateListener(uint64_t session_id,
                      std::span<const uint8_t, 16> exchange_key,
                      const rex::system::xam::QosListenerUpdate& update) override {
    if (!session_id || (!update.enabled && !update.title_data && !update.bits_per_second)) {
      return false;
    }
    std::lock_guard update_lock(listener_update_mutex_);
    json body = {{"session_id", Hex64(session_id)},
                 {"exchange_key", HexBytes(exchange_key)}};
    if (update.enabled) body["enabled"] = *update.enabled;
    if (update.bits_per_second) body["bits_per_second"] = *update.bits_per_second;
    if (update.title_data) {
      const auto encoded = Base64Encode(*update.title_data, false);
      if (!encoded) return false;
      body["title_data"] = *encoded;
    }
    const HttpResponse response = state_->Request("PUT", "/api/v2/qos/listeners", &body, true);
    if (response.status != 204) return false;
    {
      std::lock_guard lock(probe_mutex_);
      auto& listener = local_listeners_[session_id];
      listener.exchange_key = HexBytes(exchange_key);
      if (update.enabled) listener.enabled = *update.enabled;
    }
    probe_condition_.notify_all();
    return true;
  }

  bool Close(uint64_t session_id, std::span<const uint8_t, 16> exchange_key) override {
    if (!session_id) return false;
    std::lock_guard update_lock(listener_update_mutex_);
    const json body = {{"session_id", Hex64(session_id)},
                       {"exchange_key", HexBytes(exchange_key)}};
    {
      std::lock_guard lock(probe_mutex_);
      const auto listener = local_listeners_.find(session_id);
      if (listener != local_listeners_.end() &&
          listener->second.exchange_key == HexBytes(exchange_key)) {
        local_listeners_.erase(listener);
      }
    }
    probe_condition_.notify_all();
    const HttpResponse response =
        state_->Request("DELETE", "/api/v2/qos/listeners", &body, true);
    return response.status == 204 || response.status == 404;
  }

  std::vector<QosResult> Lookup(std::span<const QosTarget> targets) override {
    if (targets.empty() || targets.size() > rex::system::xam::kMaximumQosTargets) return {};
    json wire_targets = json::array();
    for (const QosTarget& target : targets) {
      std::array<uint8_t, kQosChallengeBytes> random{};
      if (!target.session_id || RAND_bytes(random.data(), random.size()) != 1) return {};
      wire_targets.push_back({{"session_id", Hex64(target.session_id)},
                              {"exchange_key", HexBytes(target.exchange_key)},
                              {"challenge", HexBytes(random)}});
    }
    const json body = {{"targets", std::move(wire_targets)}};
    const auto started = std::chrono::steady_clock::now();
    const HttpResponse response = state_->Request("POST", "/api/v2/qos/lookup", &body, true);
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - started);
    std::vector<QosResult> results(targets.size());
    if (response.status != 200) return results;
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.contains("results") || !decoded.at("results").is_array() ||
          decoded.at("results").size() != targets.size() ||
          decoded.value("measurement", std::string{}) != "relay-host-ack-v1" ||
          !decoded.contains("server_elapsed_microseconds") ||
          !decoded.at("server_elapsed_microseconds").is_number_unsigned()) {
        return {};
      }
      const uint64_t total_us = static_cast<uint64_t>(std::max<int64_t>(elapsed.count(), 0));
      const uint64_t service_us = decoded.at("server_elapsed_microseconds").get<uint64_t>();
      for (size_t index = 0; index < targets.size(); ++index) {
        const json& result = decoded.at("results").at(index);
        if (!result.is_object() || !result.value("reachable", false) ||
            result.value("measurement", std::string{}) != "relay-host-ack-v1" ||
            result.value("probes_recv", 0) != 1 ||
            !result.contains("relay_host_microseconds") ||
            !result.at("relay_host_microseconds").is_number_unsigned()) continue;
        const uint64_t host_us = result.at("relay_host_microseconds").get<uint64_t>();
        const auto rtt = gta4::network::detail::RelayHostRttMilliseconds(
            total_us, service_us, host_us);
        if (!rtt || result.value("probes_xmit", 0) != 1) continue;
        const std::string challenge = result.value("challenge", std::string{});
        if (challenge.size() != kQosChallengeHexCharacters ||
            challenge != body.at("targets").at(index).at("challenge").get<std::string>()) {
          continue;
        }
        std::optional<std::array<uint8_t, rex::system::xam::kQosTitleDataSize>> decoded_title;
        if (result.contains("title_data")) {
          const auto title = Base64Decode(result.value("title_data", std::string{}), false,
                                          rex::system::xam::kQosTitleDataSize);
          if (!title || title->size() != rex::system::xam::kQosTitleDataSize) continue;
          decoded_title.emplace();
          std::ranges::copy(*title, decoded_title->begin());
        }
        results[index].reachable = true;
        results[index].title_data = decoded_title;
        results[index].probes_xmit = 1;
        results[index].probes_recv = 1;
        results[index].rtt_min_milliseconds = *rtt;
        results[index].rtt_median_milliseconds = *rtt;
      }
    } catch (...) {
      return std::vector<QosResult>(targets.size());
    }
    return results;
  }

 private:
  struct LocalListener { std::string exchange_key; bool enabled = false; };
  void ProbeWorkerMain() {
    for (;;) {
      {
        std::unique_lock lock(probe_mutex_);
        probe_condition_.wait(lock, [&] {
          return probe_stopping_ || std::ranges::any_of(local_listeners_,
              [](const auto& entry) { return entry.second.enabled; });
        });
        if (probe_stopping_) return;
      }
      const auto response = state_->Request("GET", "/api/v3/qos/probes?wait_ms=250",
                                            nullptr, true, {}, kRelayPollTimeoutMilliseconds);
      if (response.status == 200) {
        try {
          const json body = json::parse(response.body);
          const auto& probes = body.at("probes");
          if (!probes.is_array() || probes.size() > rex::system::xam::kMaximumQosTargets) {
            throw std::runtime_error("invalid QoS probe page");
          }
          json acknowledgements = json::array();
          for (const auto& probe : probes) {
            const auto session_id = ParseFixedHex<uint64_t>(probe.at("session_id").get<std::string>(), 16);
            const auto token = probe.at("probe_id").get<std::string>();
            const auto challenge = probe.at("challenge").get<std::string>();
            bool authorized = false;
            {
              std::lock_guard lock(probe_mutex_);
              if (probe_stopping_) return;
              const auto listener = session_id ? local_listeners_.find(*session_id) : local_listeners_.end();
              authorized = listener != local_listeners_.end() && listener->second.enabled &&
                  listener->second.exchange_key == probe.at("exchange_key").get<std::string>();
            }
            if (!authorized || !IsSafeOpaqueToken(token) ||
                challenge.size() != kQosChallengeHexCharacters) continue;
            acknowledgements.push_back({{"probe_id", token}, {"challenge", challenge}});
          }
          if (!acknowledgements.empty()) {
            {
              std::lock_guard lock(probe_mutex_);
              if (probe_stopping_) return;
            }
            const json ack = {{"acknowledgements", std::move(acknowledgements)}};
            (void)state_->Request("POST", "/api/v3/qos/ack", &ack, true, {},
                                  kRelayPollTimeoutMilliseconds);
          }
        } catch (...) {
          // Invalid or stale probes are never acknowledged.
        }
      }
      std::unique_lock lock(probe_mutex_);
      if (response.status != 200) {
        probe_condition_.wait_for(lock, std::chrono::milliseconds(kRelayLongPollMilliseconds),
                                  [&] { return probe_stopping_; });
      }
      if (probe_stopping_) return;
    }
  }
  std::shared_ptr<CommunityState> state_;
  std::mutex probe_mutex_;
  std::mutex listener_update_mutex_;
  std::condition_variable probe_condition_;
  bool probe_stopping_ = false;
  std::unordered_map<uint64_t, LocalListener> local_listeners_;
  std::thread probe_worker_;
};

class CommunitySessionDirectory final : public ISessionDirectory {
 public:
  CommunitySessionDirectory(std::shared_ptr<CommunityState> state,
                            std::weak_ptr<SessionObserver> observer)
      : state_(std::move(state)), observer_(std::move(observer)) {}

  bool ready() const override { return state_->ready(); }
  std::string last_error() const override { return state_->last_error(); }

  bool Create(const SessionRecord& session) override {
    if (!rex::system::xam::IsValidSessionRecord(session)) return false;
    const std::string idempotency = IdempotencyKey();
    if (idempotency.empty()) return false;
    const std::array<std::string, 1> headers = {"Idempotency-Key: " + idempotency};
    const json body = SessionToJson(session);
    if (body["contexts"].is_null() || body["properties"].is_null()) return false;
    const HttpResponse response = state_->Request("POST", "/api/v2/sessions", &body, true, headers);
    auto created = DecodeAndRemember(response);
    if (!created) return false;
    state_->PublishSessionPresence(created->session_id);
    return true;
  }

  bool Heartbeat(const SessionRecord& session) override {
    const auto metadata = EnsureMetadata(session.session_id);
    if (!metadata) return false;
    const json body = {{"expected_revision", metadata->revision}};
    auto heartbeat = Action(session.session_id, "heartbeat", body);
    if (!heartbeat) return false;
    state_->PublishSessionPresence(heartbeat->session_id);
    return true;
  }

  std::vector<SessionRecord> Search(uint32_t title_id, uint32_t media_id, uint32_t title_version,
                                    uint32_t protocol_version, uint32_t procedure_index,
                                    std::span<const SessionContext> contexts,
                                    std::span<const SessionProperty> properties,
                                    uint32_t maximum_results) override {
    if (!maximum_results || maximum_results > 100 || properties.size() > 64 ||
        contexts.size() > 64) {
      return {};
    }
    const json encoded_properties = PropertiesToJson(properties);
    const json encoded_contexts = ContextsToJson(contexts);
    if (encoded_contexts.is_null() || encoded_properties.is_null()) return {};
    const json body = {{"title_id", Hex32(title_id)},
                       {"media_id", Hex32(media_id)},
                       {"title_version", Hex32(title_version)},
                       {"protocol_version", protocol_version},
                       {"procedure_index", procedure_index},
                       {"contexts", encoded_contexts},
                       {"properties", encoded_properties},
                       {"maximum_results", maximum_results}};
    const HttpResponse response =
        state_->Request("POST", "/api/v2/sessions/search", &body, true);
    std::vector<SessionRecord> result;
    if (response.status != 200) return result;
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.contains("procedure_index") ||
          !decoded.at("procedure_index").is_number_unsigned() ||
          decoded.at("procedure_index").get<uint32_t>() != procedure_index ||
          !decoded.contains("sessions") || !decoded["sessions"].is_array() ||
          decoded["sessions"].size() > maximum_results) {
        return {};
      }
      result.reserve(decoded["sessions"].size());
      for (const auto& wire_session : decoded["sessions"]) {
        int64_t revision = 0;
        int64_t host_epoch = 0;
        auto session = SessionFromJson(wire_session, &revision, &host_epoch);
        if (!session) continue;
        state_->Remember(session->session_id, revision, host_epoch);
        if (const auto observer = observer_.lock()) observer->ObserveSession(*session);
        result.push_back(std::move(*session));
      }
    } catch (...) {
      return {};
    }
    return result;
  }

  std::optional<SessionRecord> Get(uint64_t session_id) override {
    return Lookup(session_id).session;
  }

  SessionLookupResult Lookup(uint64_t session_id) override {
    const HttpResponse response = state_->Request(
        "GET", "/api/v2/sessions/by-xbox-id/" + Hex64(session_id), nullptr, true);
    if (response.status == 404) {
      return {.state = SessionLookupState::kAbsent,
              .session = std::nullopt};
    }
    if (response.status != 200) {
      return {.state = SessionLookupState::kUnavailable,
              .session = std::nullopt};
    }
    auto session = DecodeAndRemember(response);
    if (!session) {
      return {.state = SessionLookupState::kUnavailable,
              .session = std::nullopt};
    }
    return {.state = SessionLookupState::kFound,
            .session = std::move(session)};
  }

  bool Modify(const SessionRecord& session) override {
    if (!rex::system::xam::IsValidSessionRecord(session)) return false;
    const auto metadata = EnsureMetadata(session.session_id);
    if (!metadata) return false;
    const json full = SessionToJson(session);
    if (full["contexts"].is_null() || full["properties"].is_null()) return false;
    json body = {{"expected_revision", metadata->revision},
                       {"state", LifecycleServiceState(session.lifecycle_state)},
                       {"public_slots", full["public_slots"]},
                       {"private_slots", full["private_slots"]},
                       {"contexts", full["contexts"]},
                       {"properties", full["properties"]},
                       {"previous_session_id", full["previous_session_id"]},
                       {"nonce", full["nonce"]},
                       {"flags", full["flags"]},
                       {"lifecycle_state", full["lifecycle_state"]},
                       {"host_machine_id", full["host_machine_id"]},
                       {"host_ipv4", full["host_ipv4"]},
                       {"host_port", full["host_port"]},
                       {"host_ethernet_address", full["host_ethernet_address"]},
                       {"host_peer_id", full["host_peer_id"]}};
    std::string idempotency = IdempotencyKey();
    if (idempotency.empty()) return false;
    auto expected_revision = metadata->revision;
    auto request = [&] {
      body["expected_revision"] = expected_revision;
      const std::array<std::string, 2> headers = {
          "Idempotency-Key: " + idempotency,
          "If-Match: " + std::to_string(expected_revision)};
      const HttpResponse result = state_->Request(
          "PATCH", "/api/v2/sessions/" + Hex64(session.session_id), &body, true, headers);
      REXLOG_INFO(
          "community-session-op operation=modify role=host session={:016X} revision={} "
          "status={} error={}",
          session.session_id, expected_revision, result.status, ServerErrorCode(result));
      return result;
    };
    HttpResponse response = request();
    if (response.status == 412) {
      if (!Get(session.session_id)) return false;
      const auto refreshed = state_->Metadata(session.session_id);
      if (!refreshed) return false;
      expected_revision = refreshed->revision;
      if (RequiresFreshIdempotencyKey(response.status)) {
        idempotency = IdempotencyKey();
        if (idempotency.empty()) return false;
      }
      response = request();
    }
    return DecodeAndRemember(response).has_value();
  }

  bool Join(uint64_t session_id, const SessionMember& member) override {
    if (member.xuid != state_->identity().xuid) return false;
    const auto metadata = EnsureMetadata(session_id);
    if (!metadata) return false;
    const json body = {{"expected_revision", metadata->revision},
                       {"private", member.private_slot},
                       {"route", MemberToJson(member)["route"]},
                       {"member", MemberToJson(member)}};
    auto joined = Action(session_id, "join", body);
    if (!joined) return false;
    state_->PublishSessionPresence(joined->session_id);
    return true;
  }

  bool Leave(uint64_t session_id, uint64_t xuid) override {
    const auto metadata = EnsureMetadata(session_id);
    if (!metadata) return false;
    if (xuid == state_->identity().xuid) {
      const json body = {{"expected_revision", metadata->revision}};
      auto left = Action(session_id, "leave", body);
      if (!left) return false;
      state_->PublishOnlinePresence();
      return true;
    }

    // The host's XSessionLeaveRemote is an authoritative roster mutation, not
    // a social kick vote. Nonhost remote roster replication is handled locally
    // by XSession and never reaches this authenticated directory operation.
    const auto current = Get(session_id);
    if (!current || current->host_xuid != state_->identity().xuid ||
        std::ranges::find(current->members, xuid, &SessionMember::xuid) ==
            current->members.end()) {
      return false;
    }
    const auto refreshed_metadata = state_->Metadata(session_id);
    if (!refreshed_metadata) return false;
    const json body = {{"expected_revision", refreshed_metadata->revision},
                       {"xuid", Hex64(xuid)}};
    auto updated = Action(session_id, "leave", body);
    return updated &&
           std::ranges::find(updated->members, xuid, &SessionMember::xuid) ==
               updated->members.end();
  }

  std::optional<SessionRecord> Migrate(
      uint64_t session_id, const SessionRecord& replacement) override {
    const auto metadata = EnsureMetadata(session_id);
    if (!metadata || !replacement.host_xuid || replacement.session_id == session_id ||
        !rex::system::xam::IsValidSessionRecord(replacement)) {
      return std::nullopt;
    }
    const json encoded_replacement = SessionToJson(replacement);
    if (encoded_replacement["contexts"].is_null() ||
        encoded_replacement["properties"].is_null()) {
      return std::nullopt;
    }
    const json body = {{"expected_revision", metadata->revision},
                       {"expected_host_epoch", metadata->host_epoch},
                       {"replacement", encoded_replacement}};
    // A new roster revision can change migration eligibility and replacement
    // membership, so do not replay a stale title-elected replacement blindly.
    auto migrated = Action(session_id, "migration", body, false);
    if (!migrated) return std::nullopt;
    if (migrated->session_id != replacement.session_id ||
        migrated->previous_session_id != session_id) {
      return std::nullopt;
    }
    state_->Forget(session_id);
    if (const auto observer = observer_.lock()) observer->ForgetSession(session_id);
    state_->PublishSessionPresence(migrated->session_id);
    return migrated;
  }

  std::optional<SessionRecord> RegisterArbitration(
      uint64_t session_id, uint64_t nonce, uint32_t duration_seconds,
      uint32_t flags,
      const std::shared_ptr<ArbitrationCancellation>& cancellation) override {
    if (!session_id || !nonce || !duration_seconds ||
        (cancellation && cancellation->IsCancelled())) {
      return std::nullopt;
    }
    const auto metadata = EnsureMetadata(session_id);
    if (!metadata || (cancellation && cancellation->IsCancelled())) {
      return std::nullopt;
    }
    json body = {{"expected_revision", metadata->revision},
                 {"nonce", Hex64(nonce)},
                 {"registration_duration_seconds", duration_seconds},
                 {"flags", flags}};
    std::string idempotency = IdempotencyKey();
    if (idempotency.empty()) return std::nullopt;
    const std::string path =
        "/api/v2/sessions/" + Hex64(session_id) + "/arbitration";
    const auto deadline = std::chrono::steady_clock::now() +
                          std::chrono::seconds(duration_seconds);
    while (std::chrono::steady_clock::now() < deadline) {
      const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(
          deadline - std::chrono::steady_clock::now());
      if (remaining.count() <= 0) break;
      const long request_timeout = static_cast<long>(std::min<int64_t>(
          kRequestTimeoutMilliseconds, std::max<int64_t>(remaining.count(), 1)));
      if (cancellation && cancellation->IsCancelled()) return std::nullopt;
      const std::array<std::string, 1> headers = {
          "Idempotency-Key: " + idempotency};
      const HttpResponse response =
          state_->Request("POST", path, &body, true, headers, request_timeout);
      REXLOG_INFO(
          "community-session-op operation=arbitration target=local "
          "session={:016X} revision={} status={} error={}",
          session_id, body.value("expected_revision", int64_t{}),
          response.status, ServerErrorCode(response));
      const auto now = std::chrono::steady_clock::now();
      const ArbitrationPollAction action = ClassifyArbitrationPollResponse(
          response.status, cancellation && cancellation->IsCancelled(),
          now >= deadline);
      if (action == ArbitrationPollAction::kStop) return std::nullopt;
      if (action == ArbitrationPollAction::kComplete) {
        return DecodeAndRemember(response);
      }
      if (response.status == 202) {
        const auto frozen = DecodeAndRemember(response);
        const auto refreshed = state_->Metadata(session_id);
        if (!frozen || !refreshed) return std::nullopt;
        body["expected_revision"] = refreshed->revision;
      } else if (action == ArbitrationPollAction::kRefreshRevision) {
        if (cancellation && cancellation->IsCancelled()) return std::nullopt;
        if (!Get(session_id)) return std::nullopt;
        const auto refreshed = state_->Metadata(session_id);
        if (!refreshed || (cancellation && cancellation->IsCancelled())) {
          return std::nullopt;
        }
        body["expected_revision"] = refreshed->revision;
        if (RequiresFreshIdempotencyKey(response.status)) {
          idempotency = IdempotencyKey();
          if (idempotency.empty()) return std::nullopt;
        }
      } else if (action == ArbitrationPollAction::kFail) {
        return std::nullopt;
      }
      const auto next_poll =
          std::chrono::steady_clock::now() + kArbitrationPollInterval;
      const auto wake_at = std::min(next_poll, deadline);
      if (cancellation) {
        if (cancellation->WaitUntil(wake_at)) return std::nullopt;
      } else {
        std::this_thread::sleep_until(wake_at);
      }
    }
    return std::nullopt;
  }

  void PublishLocalSessionPresence(uint64_t session_id) override {
    state_->PublishSessionPresence(session_id);
  }

  bool Delete(uint64_t session_id) override {
    const auto metadata = EnsureMetadata(session_id);
    if (!metadata) return false;
    std::string idempotency = IdempotencyKey();
    if (idempotency.empty()) return false;
    auto expected_revision = metadata->revision;
    json body = {{"expected_revision", expected_revision}};
    auto request = [&] {
      body["expected_revision"] = expected_revision;
      const std::array<std::string, 2> headers = {
          "Idempotency-Key: " + idempotency,
          "If-Match: " + std::to_string(expected_revision)};
      const HttpResponse result = state_->Request(
          "DELETE", "/api/v2/sessions/" + Hex64(session_id), &body, true, headers);
      REXLOG_INFO(
          "community-session-op operation=delete role=host session={:016X} revision={} "
          "status={} error={}",
          session_id, expected_revision, result.status, ServerErrorCode(result));
      return result;
    };
    HttpResponse response = request();
    if (response.status == 412) {
      if (!Get(session_id)) return false;
      const auto refreshed = state_->Metadata(session_id);
      if (!refreshed) return false;
      expected_revision = refreshed->revision;
      if (RequiresFreshIdempotencyKey(response.status)) {
        idempotency = IdempotencyKey();
        if (idempotency.empty()) return false;
      }
      response = request();
    }
    if (response.status < 200 || response.status >= 300) return false;
    state_->Forget(session_id);
    if (const auto observer = observer_.lock()) observer->ForgetSession(session_id);
    state_->PublishOnlinePresence();
    return true;
  }

 private:
  std::optional<RemoteMetadata> EnsureMetadata(uint64_t session_id) {
    auto metadata = state_->Metadata(session_id);
    if (metadata) return metadata;
    if (!Get(session_id)) return std::nullopt;
    return state_->Metadata(session_id);
  }

  std::optional<SessionRecord> DecodeAndRemember(const HttpResponse& response) {
    if (response.status < 200 || response.status >= 300) return std::nullopt;
    try {
      int64_t revision = 0;
      int64_t host_epoch = 0;
      auto session = SessionFromJson(json::parse(response.body), &revision, &host_epoch);
      if (!session || revision < 1 || host_epoch < 1) return std::nullopt;
      state_->Remember(session->session_id, revision, host_epoch);
      if (const auto observer = observer_.lock()) observer->ObserveSession(*session);
      return session;
    } catch (...) {
      return std::nullopt;
    }
  }

  std::optional<SessionRecord> Action(uint64_t session_id, std::string_view action,
                                      json body, bool retry_revision = true) {
    std::string idempotency = IdempotencyKey();
    if (idempotency.empty()) return std::nullopt;
    const std::string path =
        "/api/v2/sessions/" + Hex64(session_id) + "/" + std::string(action);
    std::string_view target_role = "local";
    const json* encoded_target = nullptr;
    if (body.contains("xuid")) {
      encoded_target = &body.at("xuid");
    } else if (body.contains("member") && body.at("member").is_object() &&
               body.at("member").contains("xuid")) {
      encoded_target = &body.at("member").at("xuid");
    }
    if (encoded_target && encoded_target->is_string()) {
      const auto target =
          ParseFixedHex<uint64_t>(encoded_target->get<std::string>(), 16);
      if (target && *target != state_->identity().xuid) target_role = "remote";
    }
    auto request = [&] {
      const std::array<std::string, 1> headers = {
          "Idempotency-Key: " + idempotency};
      const HttpResponse response = state_->Request("POST", path, &body, true, headers);
      REXLOG_INFO(
          "community-session-op operation={} target={} session={:016X} revision={} "
          "status={} error={}",
          action, target_role,
          session_id, body.value("expected_revision", int64_t{}), response.status,
          ServerErrorCode(response));
      return response;
    };

    HttpResponse response = request();
    if (response.status == 412 && retry_revision) {
      auto current = Get(session_id);
      const auto refreshed = state_->Metadata(session_id);
      if (!current || !refreshed) return std::nullopt;

      std::optional<uint64_t> target;
      if (action == "join" && body.contains("member") && body.at("member").is_object() &&
          body.at("member").contains("xuid") && body.at("member").at("xuid").is_string()) {
        const json& requested = body.at("member");
        target = ParseFixedHex<uint64_t>(requested.at("xuid").get<std::string>(), 16);
        if (target) {
          const auto existing =
              std::ranges::find(current->members, *target, &SessionMember::xuid);
          if (existing != current->members.end()) {
            const bool requested_private =
                body.value("private", requested.value("private", existing->private_slot));
            bool transport_current = existing->private_slot == requested_private;
            if (requested.contains("machine_id") &&
                requested.at("machine_id").is_string()) {
              const auto machine_id = ParseFixedHex<uint64_t>(
                  requested.at("machine_id").get<std::string>(), 16);
              transport_current = transport_current && machine_id &&
                                  (!*machine_id || existing->machine_id == *machine_id);
            }
            if (requested.contains("online_port") &&
                requested.at("online_port").is_number_unsigned()) {
              const uint16_t online_port =
                  requested.at("online_port").get<uint16_t>();
              transport_current = transport_current &&
                                  (!online_port || existing->online_port == online_port);
            }
            if (transport_current) return current;
          }
        }
      } else if (action == "leave") {
        target = state_->identity().xuid;
        if (body.contains("xuid") && body.at("xuid").is_string()) {
          target = ParseFixedHex<uint64_t>(body.at("xuid").get<std::string>(), 16);
        }
        if (target && std::ranges::find(current->members, *target, &SessionMember::xuid) ==
                          current->members.end()) {
          return current;
        }
      }

      body["expected_revision"] = refreshed->revision;
      if (RequiresFreshIdempotencyKey(response.status)) {
        idempotency = IdempotencyKey();
        if (idempotency.empty()) return std::nullopt;
      }
      response = request();
    }
    return DecodeAndRemember(response);
  }

  std::shared_ptr<CommunityState> state_;
  std::weak_ptr<SessionObserver> observer_;
};

struct ActiveRelayRoute {
  uint64_t session_id = 0;
  uint16_t port = 0;
  uint32_t virtual_ipv4 = 0;
  std::chrono::steady_clock::time_point refresh_at{};
};

struct OutboundRelayDatagram {
  uint64_t session_id = 0;
  size_t encoded_payload_bytes = 0;
  json wire;
};

class CommunityPeerTransport final : public IPeerDatagramTransport, public SessionObserver {
 public:
  explicit CommunityPeerTransport(std::shared_ptr<CommunityState> state)
      : state_(std::move(state)),
        worker_([this] { WorkerMain(); }),
        send_worker_([this] { SendWorkerMain(); }) {}

  ~CommunityPeerTransport() override {
    {
      std::lock_guard lock(mutex_);
      stopping_ = true;
    }
    condition_.notify_all();
    outbound_condition_.notify_all();
    if (worker_.joinable()) worker_.join();
    if (send_worker_.joinable()) send_worker_.join();
    std::vector<ActiveRelayRoute> routes;
    {
      std::lock_guard lock(mutex_);
      for (const auto& [port, route] : routes_) routes.push_back(route);
      routes_.clear();
    }
    for (const auto& route : routes) Unregister(route);
  }

  bool ready() const override { return state_->ready(); }
  std::string last_error() const override { return state_->last_error(); }

  void ObserveSession(const SessionRecord& session) override {
    const auto local = std::find_if(session.members.begin(), session.members.end(),
                                    [&](const SessionMember& member) {
                                      return member.xuid == state_->identity().xuid;
                                    });
    if (local == session.members.end()) {
      ForgetSession(session.session_id);
      return;
    }
    const bool local_is_host = session.host_xuid == state_->identity().xuid;
    const uint16_t port =
        local->online_port ? local->online_port : (local_is_host ? session.host_port : 0);
    const uint32_t address = local->virtual_ipv4
                                 ? local->virtual_ipv4
                                 : (local_is_host ? session.host_ipv4 : 0);
    {
      std::lock_guard lock(mutex_);
      pending_sessions_[session.session_id] = address;
    }
    if (port) Register(session.session_id, port, address);
  }

  void ForgetSession(uint64_t session_id) override {
    std::vector<ActiveRelayRoute> removed;
    {
      std::lock_guard lock(mutex_);
      pending_sessions_.erase(session_id);
      for (auto iterator = routes_.begin(); iterator != routes_.end();) {
        if (iterator->second.session_id == session_id) {
          removed.push_back(iterator->second);
          pending_.erase(iterator->first);
          iterator = routes_.erase(iterator);
        } else {
          ++iterator;
        }
      }
      for (auto iterator = outbound_.begin(); iterator != outbound_.end();) {
        if (iterator->session_id == session_id) {
          outbound_encoded_bytes_ -= iterator->encoded_payload_bytes;
          iterator = outbound_.erase(iterator);
        } else {
          ++iterator;
        }
      }
    }
    for (const auto& route : removed) Unregister(route);
  }

  void RegisterRoute(uint32_t virtual_ipv4, const SessionRecord& session) override {
    if (!virtual_ipv4 || !session.session_id) return;
    std::lock_guard lock(mutex_);
    remote_sessions_[virtual_ipv4] = session.session_id;
  }

  void UnregisterRoute(uint32_t virtual_ipv4) override {
    std::lock_guard lock(mutex_);
    remote_sessions_.erase(virtual_ipv4);
  }

  bool Send(uint32_t destination_ipv4, uint16_t destination_port, uint16_t source_port,
            std::span<const uint8_t> payload) override {
    if (!destination_port || !source_port || payload.size() > kMaximumDatagramBytes) return false;
    const auto destination = Ipv4ToText(destination_ipv4);
    const auto encoded = Base64Encode(payload, false);
    if (!destination || !encoded) return false;
    std::optional<std::pair<uint64_t, uint32_t>> registration;
    {
      std::lock_guard lock(mutex_);
      if (!routes_.contains(source_port)) {
        const auto remote = remote_sessions_.find(destination_ipv4);
        if (remote == remote_sessions_.end()) return false;
        const auto pending = pending_sessions_.find(remote->second);
        if (pending == pending_sessions_.end()) return false;
        registration = std::pair(pending->first, pending->second);
      }
    }
    if (registration && !Register(registration->first, source_port, registration->second)) {
      return false;
    }
    uint64_t session_id = 0;
    {
      std::lock_guard lock(mutex_);
      const auto route = routes_.find(source_port);
      if (route == routes_.end()) return false;
      session_id = route->second.session_id;
      if (outbound_.size() >= kMaximumOutboundDatagrams ||
          encoded->size() > kMaximumOutboundEncodedBytes - outbound_encoded_bytes_) {
        return false;
      }
      outbound_encoded_bytes_ += encoded->size();
      outbound_.push_back(
          {.session_id = session_id,
           .encoded_payload_bytes = encoded->size(),
           .wire = {{"session_id", Hex64(session_id)}, {"destination_ipv4", *destination},
                    {"destination_port", destination_port},
                    {"source_port", source_port},
                    {"payload", std::move(*encoded)}}});
    }
    outbound_condition_.notify_one();
    return true;
  }

  bool HasPending(uint16_t local_port) override {
    std::lock_guard lock(mutex_);
    const auto found = pending_.find(local_port);
    return found != pending_.end() && !found->second.empty();
  }

  std::optional<PeerDatagram> Receive(uint16_t local_port,
                                      uint32_t maximum_payload_size) override {
    std::lock_guard lock(mutex_);
    auto found = pending_.find(local_port);
    if (found == pending_.end()) return std::nullopt;
    while (!found->second.empty()) {
      PeerDatagram datagram = std::move(found->second.front());
      found->second.pop_front();
      if (datagram.payload.size() <= maximum_payload_size) return datagram;
    }
    return std::nullopt;
  }

 private:
  bool Register(uint64_t session_id, uint16_t port, uint32_t requested_ipv4) {
    const json body = {{"session_id", Hex64(session_id)}, {"local_port", port}};
    const HttpResponse response = state_->Request("POST", "/api/v2/relay/routes", &body, true);
    if (response.status != 200) return false;
    try {
      const json route = json::parse(response.body);
      const auto assigned = ParseIpv4(route.at("virtual_ipv4").get<std::string>());
      if (!assigned) return false;
      if (requested_ipv4 && requested_ipv4 != *assigned) return false;
      {
        std::lock_guard lock(mutex_);
        routes_[port] = {.session_id = session_id,
                         .port = port,
                         .virtual_ipv4 = *assigned,
                         .refresh_at = std::chrono::steady_clock::now() + kRouteRefreshInterval};
      }
      condition_.notify_all();
      return true;
    } catch (...) {
      return false;
    }
  }

  void Unregister(const ActiveRelayRoute& route) {
    const json body = {{"session_id", Hex64(route.session_id)}, {"local_port", route.port}};
    state_->Request("DELETE", "/api/v2/relay/routes", &body, true);
  }

  void WorkerMain() {
    for (;;) {
      std::vector<uint16_t> ports;
      {
        std::unique_lock lock(mutex_);
        condition_.wait(lock, [&] { return stopping_ || !routes_.empty(); });
        if (stopping_) return;
        ports.reserve(routes_.size());
        for (const auto& [port, route] : routes_) ports.push_back(port);
      }
      for (const uint16_t port : ports) {
        std::optional<ActiveRelayRoute> route;
        {
          std::lock_guard lock(mutex_);
          if (stopping_) return;
          const auto found = routes_.find(port);
          if (found == routes_.end()) continue;
          route = found->second;
        }
        if (std::chrono::steady_clock::now() >= route->refresh_at) {
          if (!Register(route->session_id, route->port, route->virtual_ipv4)) {
            std::lock_guard lock(mutex_);
            const auto found = routes_.find(port);
            if (found != routes_.end()) {
              found->second.refresh_at =
                  std::chrono::steady_clock::now() + kRouteRetryInterval;
            }
          }
        }
        Poll(port, route->session_id);
      }
    }
  }

  void SendWorkerMain() {
    for (;;) {
      json datagrams = json::array();
      {
        std::unique_lock lock(mutex_);
        outbound_condition_.wait(lock, [&] { return stopping_ || !outbound_.empty(); });
        if (stopping_) return;

        const auto deadline = std::chrono::steady_clock::now() + kRelayBatchDelay;
        outbound_condition_.wait_until(lock, deadline, [&] { return stopping_; });
        if (stopping_) return;

        size_t encoded_bytes = 0;
        while (!outbound_.empty() && datagrams.size() < kMaximumRelayBatchDatagrams) {
          const auto& next = outbound_.front();
          if (!datagrams.empty() &&
              next.encoded_payload_bytes > kMaximumRelayBatchEncodedBytes - encoded_bytes) {
            break;
          }
          encoded_bytes += next.encoded_payload_bytes;
          outbound_encoded_bytes_ -= next.encoded_payload_bytes;
          datagrams.push_back(std::move(outbound_.front().wire));
          outbound_.pop_front();
        }
      }

      if (datagrams.empty()) continue;
      const json batch = {{"datagrams", std::move(datagrams)}};
      state_->Request("POST", "/api/v2/relay/datagrams", &batch, true);
    }
  }

  void Poll(uint16_t port, uint64_t session_id) {
    const std::string path = "/api/v2/relay/datagrams?session_id=" + Hex64(session_id) +
                             "&local_port=" + std::to_string(port) +
                             "&max_bytes=" + std::to_string(kMaximumReceiveBatchBytes) +
                             "&wait_ms=" + std::to_string(kRelayLongPollMilliseconds);
    const HttpResponse response =
        state_->Request("GET", path, nullptr, true, {}, kRelayPollTimeoutMilliseconds);
    if (response.status != 200) return;
    try {
      const json decoded = json::parse(response.body);
      if (!decoded.contains("datagrams") || !decoded["datagrams"].is_array() ||
          decoded["datagrams"].size() > 64) {
        return;
      }
      std::deque<PeerDatagram> received;
      for (const auto& wire : decoded["datagrams"]) {
        const auto source = ParseIpv4(wire.at("source_ipv4").get<std::string>());
        const uint16_t source_port = wire.at("source_port").get<uint16_t>();
        auto payload =
            Base64Decode(wire.at("payload").get<std::string>(), false, kMaximumDatagramBytes);
        if (!source || !source_port || !payload) continue;
        received.push_back(
            {.source_ipv4 = *source, .source_port = source_port, .payload = std::move(*payload)});
      }
      if (received.empty()) return;
      std::lock_guard lock(mutex_);
      const auto current_route = routes_.find(port);
      if (current_route == routes_.end() || current_route->second.session_id != session_id) return;
      auto& queue = pending_[port];
      while (!received.empty()) {
        if (queue.size() == kMaximumPendingDatagramsPerPort) queue.pop_front();
        queue.push_back(std::move(received.front()));
        received.pop_front();
      }
    } catch (...) {
    }
  }

  std::shared_ptr<CommunityState> state_;
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  std::condition_variable outbound_condition_;
  bool stopping_ = false;
  std::unordered_map<uint16_t, ActiveRelayRoute> routes_;
  std::unordered_map<uint64_t, uint32_t> pending_sessions_;
  std::unordered_map<uint32_t, uint64_t> remote_sessions_;
  std::unordered_map<uint16_t, std::deque<PeerDatagram>> pending_;
  std::deque<OutboundRelayDatagram> outbound_;
  size_t outbound_encoded_bytes_ = 0;
  std::thread worker_;
  std::thread send_worker_;
};

}  // namespace

LiveBackendServices CreateCommunityMultiplayerBackend(const LiveConfig& config,
                                                       const LiveIdentity& identity) {
  auto state = std::make_shared<CommunityState>(config, identity);
  auto transport = std::make_shared<CommunityPeerTransport>(state);
  auto directory = std::make_shared<CommunitySessionDirectory>(state, transport);
  auto imported_services = std::make_shared<CommunityImportedServices>(state);
  auto qos_service = std::make_shared<CommunityQosService>(state);
  return {.session_directory = std::move(directory),
          .peer_transport = std::move(transport),
          .social_service = imported_services,
          .stats_service = imported_services,
          .achievement_service = imported_services,
          .entitlement_service = imported_services,
          .title_profile_service = imported_services,
          .gta4_achievement_storage_service = imported_services,
          .voice_transport = imported_services,
          .text_chat_transport = std::move(imported_services),
          .qos_service = std::move(qos_service)};
}

}  // namespace LibertyRecomp::Network
