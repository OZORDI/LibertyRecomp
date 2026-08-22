#include "community_multiplayer.h"

#include <array>
#include <atomic>
#include <charconv>
#include <condition_variable>
#include <cstdint>
#include <cstring>
#include <deque>
#include <limits>
#include <mutex>
#include <optional>
#include <ranges>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <rex/logging.h>

#if !defined(LIBERTY_RECOMP_NO_CURL)
#include <curl/curl.h>
#endif

#if !defined(LIBERTY_RECOMP_NO_GNS)
#include <steam/isteamnetworkingmessages.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/steamnetworkingcustomsignaling.h>
#include <steam/steamnetworkingsockets.h>
#endif

#if defined(_WIN32)
#include <WS2tcpip.h>
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

namespace LibertyRecomp::Network {
namespace {

using rex::system::xam::IPeerDatagramTransport;
using rex::system::xam::ISessionDirectory;
using rex::system::xam::IsValidSessionRecord;
using rex::system::xam::LiveBackendServices;
using rex::system::xam::LiveConfig;
using rex::system::xam::LiveIdentity;
using rex::system::xam::PeerDatagram;
using rex::system::xam::RelayPolicy;
using rex::system::xam::SessionContext;
using rex::system::xam::SessionLifecycleState;
using rex::system::xam::SessionMember;
using rex::system::xam::SessionProperty;
using rex::system::xam::SessionRecord;
using json = nlohmann::json;

constexpr uint32_t kProtocolVersion = 1;
constexpr size_t kMaximumHttpResponseSize = 1024 * 1024;
constexpr size_t kMaximumWebSocketMessageSize = 256 * 1024;
constexpr size_t kMaximumSignalingQueueDepth = 128;
constexpr auto kConnectTimeout = std::chrono::seconds(10);
constexpr auto kRequestTimeout = std::chrono::seconds(15);
constexpr auto kHeartbeatInterval = std::chrono::seconds(10);
constexpr auto kWebSocketPollInterval = std::chrono::milliseconds(50);

std::string BytesToHex(std::span<const uint8_t> bytes) {
  static constexpr char kDigits[] = "0123456789abcdef";
  std::string result;
  result.reserve(bytes.size() * 2);
  for (const uint8_t byte : bytes) {
    result.push_back(kDigits[byte >> 4U]);
    result.push_back(kDigits[byte & 0x0FU]);
  }
  return result;
}

std::optional<std::vector<uint8_t>> HexToBytes(std::string_view value) {
  if (value.size() % 2 != 0) {
    return std::nullopt;
  }
  std::vector<uint8_t> result;
  result.reserve(value.size() / 2);
  for (size_t index = 0; index < value.size(); index += 2) {
    uint32_t byte = 0;
    const auto conversion = std::from_chars(value.data() + index,
                                            value.data() + index + 2, byte, 16);
    if (conversion.ec != std::errc{} ||
        conversion.ptr != value.data() + index + 2) {
      return std::nullopt;
    }
    result.push_back(static_cast<uint8_t>(byte));
  }
  return result;
}

std::string Uint64ToString(uint64_t value) {
  return fmt::format("{:016x}", value);
}

std::optional<uint64_t> StringToUint64(const json &value) {
  if (value.is_number_unsigned()) {
    return value.get<uint64_t>();
  }
  if (!value.is_string()) {
    return std::nullopt;
  }
  const std::string text = value.get<std::string>();
  uint64_t result = 0;
  const auto conversion =
      std::from_chars(text.data(), text.data() + text.size(), result, 16);
  return conversion.ec == std::errc{} &&
                 conversion.ptr == text.data() + text.size()
             ? std::optional<uint64_t>(result)
             : std::nullopt;
}

std::string IPv4ToString(uint32_t address) {
  in_addr native_address{.s_addr = address};
  std::array<char, INET_ADDRSTRLEN> buffer{};
  return inet_ntop(AF_INET, &native_address, buffer.data(), buffer.size())
             ? buffer.data()
             : "";
}

std::optional<uint32_t> StringToIPv4(const json &value) {
  if (value.is_number_unsigned()) {
    return value.get<uint32_t>();
  }
  if (!value.is_string()) {
    return std::nullopt;
  }
  in_addr address{};
  const std::string text = value.get<std::string>();
  return inet_pton(AF_INET, text.c_str(), &address) == 1
             ? std::optional(address.s_addr)
             : std::nullopt;
}

std::string RelayPolicyName(RelayPolicy policy) {
  switch (policy) {
  case RelayPolicy::kAuto:
    return "auto";
  case RelayPolicy::kDirectOnly:
    return "direct_only";
  case RelayPolicy::kRelayOnly:
    return "relay_only";
  }
  return "auto";
}

bool IsSecureServiceUrl(std::string_view url) {
  if (url.starts_with("https://")) {
    return true;
  }
  return url.starts_with("http://127.0.0.1") ||
         url.starts_with("http://localhost") || url.starts_with("http://[::1]");
}

std::string WebSocketUrlFor(std::string_view base_url) {
  std::string result(base_url);
  while (!result.empty() && result.back() == '/') {
    result.pop_back();
  }
  if (result.starts_with("https://")) {
    result.replace(0, std::string_view("https").size(), "wss");
  } else if (result.starts_with("http://")) {
    result.replace(0, std::string_view("http").size(), "ws");
  }
  result.append("/v1/rendezvous");
  return result;
}

json ContextToJson(const SessionContext &context) {
  return {{"id", context.id}, {"value", context.value}};
}

json PropertyToJson(const SessionProperty &property) {
  return {{"id", property.id}, {"value_hex", BytesToHex(property.value)}};
}

json MemberToJson(const SessionMember &member) {
  return {{"xuid", Uint64ToString(member.xuid)},
          {"private_slot", member.private_slot},
          {"machine_id", Uint64ToString(member.machine_id)},
          {"virtual_ipv4", IPv4ToString(member.virtual_ipv4)},
          {"online_port", member.online_port},
          {"peer_id", member.peer_id}};
}

json SessionToJson(const SessionRecord &session) {
  json contexts = json::array();
  for (const auto &context : session.contexts) {
    contexts.push_back(ContextToJson(context));
  }
  json properties = json::array();
  for (const auto &property : session.properties) {
    properties.push_back(PropertyToJson(property));
  }
  json members = json::array();
  for (const auto &member : session.members) {
    members.push_back(MemberToJson(member));
  }
  return {{"title_id", session.title_id},
          {"media_id", session.media_id},
          {"title_version", session.title_version},
          {"protocol_version", session.protocol_version},
          {"session_id", Uint64ToString(session.session_id)},
          {"previous_session_id", Uint64ToString(session.previous_session_id)},
          {"exchange_key_hex", BytesToHex(session.exchange_key)},
          {"nonce", Uint64ToString(session.nonce)},
          {"flags", session.flags},
          {"lifecycle_state", static_cast<uint32_t>(session.lifecycle_state)},
          {"max_public_slots", session.max_public_slots},
          {"max_private_slots", session.max_private_slots},
          {"open_public_slots", session.open_public_slots},
          {"open_private_slots", session.open_private_slots},
          {"host_xuid", Uint64ToString(session.host_xuid)},
          {"host_machine_id", Uint64ToString(session.host_machine_id)},
          {"host_virtual_ipv4", IPv4ToString(session.host_ipv4)},
          {"host_port", session.host_port},
          {"host_ethernet_hex", BytesToHex(session.host_ethernet_address)},
          {"host_peer_id", session.host_peer_id},
          {"contexts", std::move(contexts)},
          {"properties", std::move(properties)},
          {"members", std::move(members)}};
}

template <size_t Size>
bool ReadFixedHex(const json &source, std::string_view key,
                  std::array<uint8_t, Size> &output) {
  const auto it = source.find(key);
  if (it == source.end() || !it->is_string()) {
    return false;
  }
  auto bytes = HexToBytes(it->get<std::string>());
  if (!bytes || bytes->size() != output.size()) {
    return false;
  }
  std::copy(bytes->begin(), bytes->end(), output.begin());
  return true;
}

std::optional<SessionRecord> SessionFromJson(const json &source) {
  try {
    const auto session_id = StringToUint64(source.at("session_id"));
    const auto nonce = StringToUint64(source.at("nonce"));
    const auto host_xuid = StringToUint64(source.at("host_xuid"));
    const auto host_machine_id = StringToUint64(source.at("host_machine_id"));
    const auto host_ipv4 = StringToIPv4(source.at("host_virtual_ipv4"));
    if (!session_id || !nonce || !host_xuid || !host_machine_id || !host_ipv4) {
      return std::nullopt;
    }
    SessionRecord result;
    result.title_id = source.at("title_id").get<uint32_t>();
    result.media_id = source.at("media_id").get<uint32_t>();
    result.title_version = source.at("title_version").get<uint32_t>();
    result.protocol_version = source.at("protocol_version").get<uint32_t>();
    result.session_id = *session_id;
    if (const auto previous = source.find("previous_session_id");
        previous != source.end()) {
      result.previous_session_id = StringToUint64(*previous).value_or(0);
    }
    result.nonce = *nonce;
    result.flags = source.at("flags").get<uint32_t>();
    result.lifecycle_state = static_cast<SessionLifecycleState>(
        source.at("lifecycle_state").get<uint32_t>());
    result.max_public_slots = source.at("max_public_slots").get<uint32_t>();
    result.max_private_slots = source.at("max_private_slots").get<uint32_t>();
    result.open_public_slots = source.at("open_public_slots").get<uint32_t>();
    result.open_private_slots = source.at("open_private_slots").get<uint32_t>();
    result.host_xuid = *host_xuid;
    result.host_machine_id = *host_machine_id;
    result.host_ipv4 = *host_ipv4;
    result.host_port = source.at("host_port").get<uint16_t>();
    result.host_peer_id = source.at("host_peer_id").get<std::string>();
    if (!ReadFixedHex(source, "exchange_key_hex", result.exchange_key) ||
        !ReadFixedHex(source, "host_ethernet_hex",
                      result.host_ethernet_address)) {
      return std::nullopt;
    }
    if (const auto contexts = source.find("contexts");
        contexts != source.end()) {
      for (const auto &context : *contexts) {
        result.contexts.push_back(
            {.id = context.at("id").get<uint32_t>(),
             .value = context.at("value").get<uint32_t>()});
      }
    }
    if (const auto properties = source.find("properties");
        properties != source.end()) {
      for (const auto &property : *properties) {
        auto value = HexToBytes(property.at("value_hex").get<std::string>());
        if (!value) {
          return std::nullopt;
        }
        result.properties.push_back({.id = property.at("id").get<uint32_t>(),
                                     .value = std::move(*value)});
      }
    }
    if (const auto members = source.find("members"); members != source.end()) {
      for (const auto &member : *members) {
        const auto xuid = StringToUint64(member.at("xuid"));
        const auto machine_id = StringToUint64(member.at("machine_id"));
        const auto virtual_ipv4 = StringToIPv4(member.at("virtual_ipv4"));
        if (!xuid || !machine_id || !virtual_ipv4) {
          return std::nullopt;
        }
        result.members.push_back(
            {.xuid = *xuid,
             .private_slot = member.at("private_slot").get<bool>(),
             .machine_id = *machine_id,
             .virtual_ipv4 = *virtual_ipv4,
             .online_port = member.at("online_port").get<uint16_t>(),
             .peer_id = member.at("peer_id").get<std::string>()});
      }
    }
    result.last_seen = std::chrono::steady_clock::now();
    return IsValidSessionRecord(result) ? std::optional(std::move(result))
                                        : std::nullopt;
  } catch (const json::exception &) {
    return std::nullopt;
  }
}

class CommunityMultiplayerBackend final
    : public ISessionDirectory,
      public IPeerDatagramTransport,
      public std::enable_shared_from_this<CommunityMultiplayerBackend> {
public:
  CommunityMultiplayerBackend(LiveConfig config, LiveIdentity identity);
  ~CommunityMultiplayerBackend() override;

  bool ready() const override { return ready_.load(std::memory_order_acquire); }
  std::string last_error() const override;

  bool Create(const SessionRecord &session) override;
  bool Heartbeat(const SessionRecord &session) override;
  std::vector<SessionRecord> Search(uint32_t title_id, uint32_t media_id,
                                    uint32_t title_version,
                                    uint32_t protocol_version,
                                    std::span<const SessionContext> contexts,
                                    std::span<const SessionProperty> properties,
                                    uint32_t maximum_results) override;
  std::optional<SessionRecord> Get(uint64_t session_id) override;
  bool Modify(const SessionRecord &session) override;
  bool Join(uint64_t session_id, const SessionMember &member) override;
  bool Leave(uint64_t session_id, uint64_t xuid) override;
  bool Migrate(uint64_t session_id, const SessionRecord &replacement) override;
  bool Delete(uint64_t session_id) override;

  void RegisterRoute(uint32_t virtual_ipv4,
                     const SessionRecord &session) override;
  void UnregisterRoute(uint32_t virtual_ipv4) override;
  bool Send(uint32_t destination_ipv4, uint16_t destination_port,
            uint16_t source_port, std::span<const uint8_t> payload) override;
  bool HasPending(uint16_t local_port) override;
  std::optional<PeerDatagram> Receive(uint16_t local_port,
                                      uint32_t maximum_payload_size) override;

private:
  struct Route {
    std::string peer_id;
    uint32_t virtual_ipv4 = 0;
  };

  bool Initialize();
  bool RegisterClient();
  bool ConfigureIce();
  void ConfigureRoutes(const SessionRecord &session);
  bool Request(std::string_view method, std::string_view path,
               const json *request, json *response, bool authenticated = true);
  bool RequestSession(std::string_view method, std::string_view path,
                      const json *request, SessionRecord *response);
  void WorkerMain();
  bool ConnectWebSocket();
  bool FlushWebSocketMessage();
  bool PollWebSocket();
  void HandleWebSocketMessage(std::string_view message);
  void QueueSignal(std::string peer_id, const void *data, size_t size);
  bool PumpIncoming(uint16_t local_port);
  void RecordError(std::string message);
  void SetError(std::string message);
  void SetReady(bool ready);

#if !defined(LIBERTY_RECOMP_NO_GNS)
  class ConnectionSignaling;
  class ReceiveContext;
  static CommunityMultiplayerBackend *Active();
  static ISteamNetworkingConnectionSignaling *
  CreateConnectionSignaling(ISteamNetworkingSockets *,
                            const SteamNetworkingIdentity &peer, int, int);
  static void
  OnMessagesSessionRequest(SteamNetworkingMessagesSessionRequest_t *request);
#endif

  LiveConfig config_;
  LiveIdentity identity_;
  std::string service_url_;
  std::string websocket_url_;
  std::string auth_token_;
  std::string client_id_;
  std::string transport_identity_;
  uint32_t local_virtual_ipv4_ = 0;
  std::vector<std::string> stun_servers_;
  std::vector<std::string> turn_servers_;
  std::vector<std::string> turn_usernames_;
  std::vector<std::string> turn_passwords_;
  mutable std::mutex service_mutex_;

  std::atomic<bool> running_{false};
  std::atomic<bool> ready_{false};
  mutable std::mutex state_mutex_;
  std::condition_variable state_condition_;
  std::string last_error_;
  std::thread worker_;

  mutable std::mutex session_mutex_;
  std::unordered_map<uint64_t, SessionRecord> hosted_sessions_;

  mutable std::mutex route_mutex_;
  std::unordered_map<uint32_t, Route> routes_by_ipv4_;
  std::unordered_map<std::string, uint32_t> ipv4_by_peer_;

  mutable std::mutex incoming_mutex_;
  std::unordered_map<uint16_t, std::deque<PeerDatagram>> incoming_datagrams_;

  mutable std::mutex signaling_mutex_;
  std::deque<std::string> signaling_queue_;
  std::optional<std::string> websocket_send_message_;
  size_t websocket_send_offset_ = 0;
  std::string websocket_receive_message_;

#if !defined(LIBERTY_RECOMP_NO_CURL)
  CURL *websocket_ = nullptr;
  curl_slist *websocket_headers_ = nullptr;
#endif
#if !defined(LIBERTY_RECOMP_NO_GNS)
  ISteamNetworkingSockets *sockets_ = nullptr;
  ISteamNetworkingMessages *messages_ = nullptr;
  bool gns_initialized_ = false;
  static std::atomic<CommunityMultiplayerBackend *> active_backend_;
#endif
};

#if !defined(LIBERTY_RECOMP_NO_CURL)
size_t WriteResponse(char *data, size_t size, size_t count, void *context) {
  auto *output = static_cast<std::string *>(context);
  const size_t incoming_size = size * count;
  if (incoming_size > kMaximumHttpResponseSize ||
      output->size() > kMaximumHttpResponseSize - incoming_size) {
    return 0;
  }
  output->append(data, incoming_size);
  return incoming_size;
}
#endif

#if !defined(LIBERTY_RECOMP_NO_GNS)
std::atomic<CommunityMultiplayerBackend *>
    CommunityMultiplayerBackend::active_backend_{nullptr};

class CommunityMultiplayerBackend::ConnectionSignaling final
    : public ISteamNetworkingConnectionSignaling {
public:
  ConnectionSignaling(CommunityMultiplayerBackend *owner, std::string peer_id)
      : owner_(owner), peer_id_(std::move(peer_id)) {}

  bool SendSignal(HSteamNetConnection, const SteamNetConnectionInfo_t &,
                  const void *message, int message_size) override {
    if (message_size <= 0) {
      return false;
    }
    owner_->QueueSignal(peer_id_, message, static_cast<size_t>(message_size));
    return owner_->running_.load(std::memory_order_acquire);
  }

  void Release() override { delete this; }

private:
  CommunityMultiplayerBackend *owner_;
  std::string peer_id_;
};

class CommunityMultiplayerBackend::ReceiveContext final
    : public ISteamNetworkingSignalingRecvContext {
public:
  explicit ReceiveContext(CommunityMultiplayerBackend *owner) : owner_(owner) {}

  ISteamNetworkingConnectionSignaling *
  OnConnectRequest(HSteamNetConnection connection,
                   const SteamNetworkingIdentity &peer, int) override {
    const char *peer_id = peer.GetGenericString();
    if (!peer_id || !owner_->sockets_ ||
        owner_->sockets_->AcceptConnection(connection) != k_EResultOK) {
      return nullptr;
    }
    return new ConnectionSignaling(owner_, peer_id);
  }

  void SendRejectionSignal(const SteamNetworkingIdentity &, const void *,
                           int) override {}

private:
  CommunityMultiplayerBackend *owner_;
};

CommunityMultiplayerBackend *CommunityMultiplayerBackend::Active() {
  return active_backend_.load(std::memory_order_acquire);
}

ISteamNetworkingConnectionSignaling *
CommunityMultiplayerBackend::CreateConnectionSignaling(
    ISteamNetworkingSockets *, const SteamNetworkingIdentity &peer, int, int) {
  auto *backend = Active();
  const char *peer_id = peer.GetGenericString();
  return backend && peer_id ? new ConnectionSignaling(backend, peer_id)
                            : nullptr;
}

void CommunityMultiplayerBackend::OnMessagesSessionRequest(
    SteamNetworkingMessagesSessionRequest_t *request) {
  auto *backend = Active();
  if (backend && backend->messages_) {
    backend->messages_->AcceptSessionWithUser(request->m_identityRemote);
  }
}
#endif

CommunityMultiplayerBackend::CommunityMultiplayerBackend(LiveConfig config,
                                                         LiveIdentity identity)
    : config_(std::move(config)), identity_(std::move(identity)) {
  Initialize();
}

CommunityMultiplayerBackend::~CommunityMultiplayerBackend() {
  running_.store(false, std::memory_order_release);
  if (worker_.joinable()) {
    worker_.join();
  }

  std::vector<uint64_t> hosted_ids;
  {
    std::lock_guard lock(session_mutex_);
    hosted_ids.reserve(hosted_sessions_.size());
    for (const auto &[session_id, session] : hosted_sessions_) {
      hosted_ids.push_back(session_id);
    }
  }
  for (const uint64_t session_id : hosted_ids) {
    json ignored;
    Request("DELETE", fmt::format("/v1/sessions/{:016x}", session_id), nullptr,
            &ignored);
  }

#if !defined(LIBERTY_RECOMP_NO_CURL)
  if (websocket_) {
    curl_easy_cleanup(websocket_);
  }
  if (websocket_headers_) {
    curl_slist_free_all(websocket_headers_);
  }
#endif
#if !defined(LIBERTY_RECOMP_NO_GNS)
  CommunityMultiplayerBackend *expected = this;
  active_backend_.compare_exchange_strong(expected, nullptr,
                                          std::memory_order_acq_rel);
  if (gns_initialized_) {
    GameNetworkingSockets_Kill();
  }
#endif
}

std::string CommunityMultiplayerBackend::last_error() const {
  std::lock_guard lock(state_mutex_);
  return last_error_;
}

void CommunityMultiplayerBackend::SetError(std::string message) {
  RecordError(std::move(message));
  ready_.store(false, std::memory_order_release);
  state_condition_.notify_all();
}

void CommunityMultiplayerBackend::RecordError(std::string message) {
  REXSYS_WARN("{}", message);
  {
    std::lock_guard lock(state_mutex_);
    last_error_ = std::move(message);
  }
}

void CommunityMultiplayerBackend::SetReady(bool ready) {
  if (ready) {
    std::lock_guard lock(state_mutex_);
    last_error_.clear();
  }
  ready_.store(ready, std::memory_order_release);
  state_condition_.notify_all();
}

bool CommunityMultiplayerBackend::Initialize() {
#if defined(LIBERTY_RECOMP_NO_CURL) || defined(LIBERTY_RECOMP_NO_GNS)
  SetError("Community multiplayer requires desktop cURL and "
           "GameNetworkingSockets support");
  return false;
#else
  service_url_ = config_.community_url;
  while (!service_url_.empty() && service_url_.back() == '/') {
    service_url_.pop_back();
  }
  if (!IsSecureServiceUrl(service_url_)) {
    SetError("Community service URL must use HTTPS (plain HTTP is allowed only "
             "for loopback)");
    return false;
  }

  static std::once_flag curl_init_once;
  static CURLcode curl_init_result = CURLE_FAILED_INIT;
  std::call_once(curl_init_once, []() {
    curl_init_result = curl_global_init(CURL_GLOBAL_DEFAULT);
  });
  if (curl_init_result != CURLE_OK) {
    SetError("Unable to initialize cURL");
    return false;
  }

  transport_identity_ =
      fmt::format("liberty-{}-{}", Uint64ToString(identity_.xuid),
                  Uint64ToString(identity_.machine_id));
  if (!RegisterClient()) {
    return false;
  }

  SteamNetworkingIdentity network_identity;
  network_identity.Clear();
  if (!network_identity.SetGenericString(transport_identity_.c_str())) {
    SetError("Generated peer identity is too long for GameNetworkingSockets");
    return false;
  }
  SteamNetworkingErrMsg error{};
  if (!GameNetworkingSockets_Init(&network_identity, error)) {
    SetError(
        fmt::format("GameNetworkingSockets initialization failed: {}", error));
    return false;
  }
  gns_initialized_ = true;
  sockets_ = SteamNetworkingSockets();
  messages_ = SteamNetworkingMessages();
  if (!sockets_ || !messages_) {
    SetError("GameNetworkingSockets did not provide the P2P interfaces");
    return false;
  }

  if (!ConfigureIce()) {
    return false;
  }
  auto *utils = SteamNetworkingUtils();
  CommunityMultiplayerBackend *expected = nullptr;
  if (!active_backend_.compare_exchange_strong(expected, this,
                                               std::memory_order_acq_rel)) {
    SetError("Only one Community multiplayer backend may be active");
    return false;
  }
  utils->SetGlobalConfigValuePtr(
      k_ESteamNetworkingConfig_Callback_CreateConnectionSignaling,
      reinterpret_cast<void *>(&CreateConnectionSignaling));
  utils->SetGlobalCallback_MessagesSessionRequest(&OnMessagesSessionRequest);

  running_.store(true, std::memory_order_release);
  worker_ = std::thread([this]() { WorkerMain(); });
  std::unique_lock lock(state_mutex_);
  state_condition_.wait_for(lock, kConnectTimeout, [this]() {
    return ready_.load(std::memory_order_acquire) || !last_error_.empty();
  });
  if (!ready_.load(std::memory_order_acquire) && last_error_.empty()) {
    lock.unlock();
    SetError("Timed out connecting to the community rendezvous service");
  }
  return ready_.load(std::memory_order_acquire);
#endif
}

bool CommunityMultiplayerBackend::ConfigureIce() {
#if defined(LIBERTY_RECOMP_NO_GNS)
  SetError("GameNetworkingSockets support is not available");
  return false;
#else
  auto JoinValues = [](const std::vector<std::string> &values) {
    std::string result;
    for (const auto &value : values) {
      if (!result.empty()) {
        result.push_back(',');
      }
      result.append(value);
    }
    return result;
  };
  std::vector<std::string> stun_servers;
  std::vector<std::string> turn_servers;
  std::vector<std::string> turn_usernames;
  std::vector<std::string> turn_passwords;
  {
    std::lock_guard lock(service_mutex_);
    stun_servers = stun_servers_;
    turn_servers = turn_servers_;
    turn_usernames = turn_usernames_;
    turn_passwords = turn_passwords_;
  }

  auto *utils = SteamNetworkingUtils();
  const std::string stun_list = JoinValues(stun_servers);
  const std::string turn_list = JoinValues(turn_servers);
  const std::string turn_users = JoinValues(turn_usernames);
  const std::string turn_password_list = JoinValues(turn_passwords);
  utils->SetGlobalConfigValueString(
      k_ESteamNetworkingConfig_P2P_STUN_ServerList, stun_list.c_str());
  utils->SetGlobalConfigValueString(
      k_ESteamNetworkingConfig_P2P_TURN_ServerList, turn_list.c_str());
  utils->SetGlobalConfigValueString(k_ESteamNetworkingConfig_P2P_TURN_UserList,
                                    turn_users.c_str());
  utils->SetGlobalConfigValueString(k_ESteamNetworkingConfig_P2P_TURN_PassList,
                                    turn_password_list.c_str());

  int32_t ice_candidates =
      k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Relay |
      k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Private |
      k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Public;
  if (config_.relay_policy == RelayPolicy::kDirectOnly) {
    ice_candidates = k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Private |
                     k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Public;
  } else if (config_.relay_policy == RelayPolicy::kRelayOnly) {
    if (turn_servers.empty() || turn_usernames.size() != turn_servers.size() ||
        turn_passwords.size() != turn_servers.size()) {
      SetError("Relay-only policy requires service-issued TURN servers and "
               "credentials");
      return false;
    }
    ice_candidates = k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Relay;
  }
  utils->SetGlobalConfigValueInt32(
      k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable, ice_candidates);
  return true;
#endif
}

bool CommunityMultiplayerBackend::RegisterClient() {
  json request = {
      {"protocol_version", kProtocolVersion},
      {"install_id", fmt::format("{}{}", Uint64ToString(identity_.xuid),
                                 Uint64ToString(identity_.machine_id))},
      {"xuid", Uint64ToString(identity_.xuid)},
      {"machine_id", Uint64ToString(identity_.machine_id)},
      {"player_name", identity_.player_name},
      {"transport_identity", transport_identity_},
      {"relay_policy", RelayPolicyName(config_.relay_policy)}};
  json response;
  if (!Request("POST", "/v1/clients/register", &request, &response, false)) {
    return false;
  }
  try {
    std::string auth_token = response.at("token").get<std::string>();
    std::string client_id = response.at("client_id").get<std::string>();
    const auto virtual_ipv4 = StringToIPv4(response.at("virtual_ipv4"));
    if (!virtual_ipv4 || auth_token.empty() || client_id.empty()) {
      SetError("Community registration returned an incomplete identity");
      return false;
    }
    std::string websocket_url =
        response.value("websocket_url", WebSocketUrlFor(service_url_));
    if (!websocket_url.starts_with("wss://") &&
        !websocket_url.starts_with("ws://127.0.0.1") &&
        !websocket_url.starts_with("ws://localhost") &&
        !websocket_url.starts_with("ws://[::1]")) {
      SetError("Community rendezvous URL must use WSS");
      return false;
    }
    std::vector<std::string> stun_servers;
    std::vector<std::string> turn_servers;
    std::vector<std::string> turn_usernames;
    std::vector<std::string> turn_passwords;
    if (const auto ice = response.find("ice"); ice != response.end()) {
      stun_servers = ice->value("stun_servers", std::vector<std::string>{});
      if (const auto turns = ice->find("turn_servers"); turns != ice->end()) {
        for (const auto &turn : *turns) {
          turn_servers.push_back(turn.at("url").get<std::string>());
          turn_usernames.push_back(turn.at("username").get<std::string>());
          turn_passwords.push_back(turn.at("password").get<std::string>());
        }
      }
    }
    {
      std::lock_guard lock(service_mutex_);
      if (local_virtual_ipv4_ && local_virtual_ipv4_ != *virtual_ipv4) {
        SetError("Community service changed this client's virtual address");
        return false;
      }
      auth_token_ = std::move(auth_token);
      client_id_ = std::move(client_id);
      if (!local_virtual_ipv4_) {
        local_virtual_ipv4_ = *virtual_ipv4;
      }
      websocket_url_ = std::move(websocket_url);
      stun_servers_ = std::move(stun_servers);
      turn_servers_ = std::move(turn_servers);
      turn_usernames_ = std::move(turn_usernames);
      turn_passwords_ = std::move(turn_passwords);
    }
  } catch (const json::exception &error) {
    SetError(fmt::format("Invalid community registration response: {}",
                         error.what()));
    return false;
  }
  return true;
}

bool CommunityMultiplayerBackend::Request(std::string_view method,
                                          std::string_view path,
                                          const json *request, json *response,
                                          bool authenticated) {
#if defined(LIBERTY_RECOMP_NO_CURL)
  SetError("cURL support is not available");
  return false;
#else
  CURL *curl = curl_easy_init();
  if (!curl) {
    SetError("Unable to allocate a cURL request");
    return false;
  }
  const std::string url = service_url_ + std::string(path);
  const std::string method_string(method);
  const std::string body = request ? request->dump() : std::string{};
  std::string response_body;
  curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  std::string authorization;
  std::string auth_token;
  if (authenticated) {
    std::lock_guard lock(service_mutex_);
    auth_token = auth_token_;
  }
  if (!auth_token.empty()) {
    authorization = "Authorization: Bearer " + auth_token;
    headers = curl_slist_append(headers, authorization.c_str());
  }
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method_string.c_str());
#if LIBCURL_VERSION_NUM >= 0x075500
  curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "https,http");
#else
  curl_easy_setopt(curl, CURLOPT_PROTOCOLS,
                   static_cast<long>(CURLPROTO_HTTPS | CURLPROTO_HTTP));
#endif
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_USERAGENT,
                   "LibertyRecomp/CommunityCompatibility");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteResponse);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);
  curl_easy_setopt(
      curl, CURLOPT_CONNECTTIMEOUT_MS,
      static_cast<long>(
          std::chrono::duration_cast<std::chrono::milliseconds>(kConnectTimeout)
              .count()));
  curl_easy_setopt(
      curl, CURLOPT_TIMEOUT_MS,
      static_cast<long>(
          std::chrono::duration_cast<std::chrono::milliseconds>(kRequestTimeout)
              .count()));
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
  if (request) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE,
                     static_cast<curl_off_t>(body.size()));
  }
  const CURLcode result = curl_easy_perform(curl);
  long status = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  if (result != CURLE_OK) {
    SetError(fmt::format("Community request {} {} failed: {}", method, path,
                         curl_easy_strerror(result)));
    return false;
  }
  if (status < 200 || status >= 300) {
    auto error = fmt::format("Community request {} {} returned HTTP {}", method,
                             path, status);
    if (status == 401 || status == 403 || status >= 500) {
      SetError(std::move(error));
    } else {
      RecordError(std::move(error));
    }
    return false;
  }
  if (response) {
    if (response_body.empty()) {
      *response = json::object();
    } else {
      try {
        *response = json::parse(response_body);
      } catch (const json::exception &error) {
        SetError(fmt::format("Community response for {} was invalid JSON: {}",
                             path, error.what()));
        return false;
      }
    }
  }
  return true;
#endif
}

bool CommunityMultiplayerBackend::RequestSession(std::string_view method,
                                                 std::string_view path,
                                                 const json *request,
                                                 SessionRecord *response) {
  json document;
  if (!Request(method, path, request, &document)) {
    return false;
  }
  const json &encoded =
      document.contains("session") ? document.at("session") : document;
  auto session = SessionFromJson(encoded);
  if (!session) {
    SetError(fmt::format(
        "Community response for {} contained an invalid session", path));
    return false;
  }
  ConfigureRoutes(*session);
  if (response) {
    *response = std::move(*session);
  }
  return true;
}

bool CommunityMultiplayerBackend::Create(const SessionRecord &session) {
  SessionRecord published = session;
  published.host_ipv4 = local_virtual_ipv4_;
  published.host_peer_id = transport_identity_;
  json request = SessionToJson(published);
  SessionRecord created;
  if (!RequestSession("POST", "/v1/sessions", &request, &created) ||
      created.session_id != session.session_id) {
    return false;
  }
  std::lock_guard lock(session_mutex_);
  hosted_sessions_[created.session_id] = std::move(created);
  return true;
}

bool CommunityMultiplayerBackend::Heartbeat(const SessionRecord &session) {
  json request = {{"session", SessionToJson(session)}};
  SessionRecord updated;
  if (!RequestSession(
          "POST",
          fmt::format("/v1/sessions/{:016x}/heartbeat", session.session_id),
          &request, &updated)) {
    return false;
  }
  std::lock_guard lock(session_mutex_);
  if (hosted_sessions_.contains(session.session_id)) {
    hosted_sessions_[session.session_id] = std::move(updated);
  }
  return true;
}

std::vector<SessionRecord> CommunityMultiplayerBackend::Search(
    uint32_t title_id, uint32_t media_id, uint32_t title_version,
    uint32_t protocol_version, std::span<const SessionContext> contexts,
    std::span<const SessionProperty> properties, uint32_t maximum_results) {
  json encoded_contexts = json::array();
  for (const auto &context : contexts) {
    encoded_contexts.push_back(ContextToJson(context));
  }
  json encoded_properties = json::array();
  for (const auto &property : properties) {
    encoded_properties.push_back(PropertyToJson(property));
  }
  json request = {{"title_id", title_id},
                  {"media_id", media_id},
                  {"title_version", title_version},
                  {"protocol_version", protocol_version},
                  {"maximum_results", maximum_results},
                  {"contexts", std::move(encoded_contexts)},
                  {"properties", std::move(encoded_properties)}};
  json response;
  if (!Request("POST", "/v1/sessions/search", &request, &response)) {
    return {};
  }
  std::vector<SessionRecord> result;
  try {
    const auto &sessions =
        response.contains("sessions") ? response.at("sessions") : response;
    if (!sessions.is_array()) {
      SetError("Community search response was not an array");
      return {};
    }
    for (const auto &encoded : sessions) {
      if (result.size() >= maximum_results) {
        break;
      }
      auto session = SessionFromJson(encoded);
      if (!session || session->title_id != title_id ||
          session->media_id != media_id ||
          session->title_version != title_version ||
          session->protocol_version != protocol_version) {
        continue;
      }
      ConfigureRoutes(*session);
      result.push_back(std::move(*session));
    }
  } catch (const json::exception &error) {
    SetError(
        fmt::format("Invalid community search response: {}", error.what()));
    return {};
  }
  return result;
}

std::optional<SessionRecord>
CommunityMultiplayerBackend::Get(uint64_t session_id) {
  SessionRecord result;
  return RequestSession("GET", fmt::format("/v1/sessions/{:016x}", session_id),
                        nullptr, &result)
             ? std::optional(std::move(result))
             : std::nullopt;
}

bool CommunityMultiplayerBackend::Modify(const SessionRecord &session) {
  json request = SessionToJson(session);
  SessionRecord updated;
  if (!RequestSession("PATCH",
                      fmt::format("/v1/sessions/{:016x}", session.session_id),
                      &request, &updated)) {
    return false;
  }
  std::lock_guard lock(session_mutex_);
  if (hosted_sessions_.contains(session.session_id)) {
    hosted_sessions_[session.session_id] = std::move(updated);
  }
  return true;
}

bool CommunityMultiplayerBackend::Join(uint64_t session_id,
                                       const SessionMember &member) {
  SessionMember local_member = member;
  local_member.machine_id = identity_.machine_id;
  local_member.virtual_ipv4 = local_virtual_ipv4_;
  local_member.peer_id = transport_identity_;
  json request = MemberToJson(local_member);
  SessionRecord updated;
  return RequestSession("POST",
                        fmt::format("/v1/sessions/{:016x}/join", session_id),
                        &request, &updated);
}

bool CommunityMultiplayerBackend::Leave(uint64_t session_id, uint64_t xuid) {
  json request = {{"xuid", Uint64ToString(xuid)}};
  SessionRecord updated;
  return RequestSession("POST",
                        fmt::format("/v1/sessions/{:016x}/leave", session_id),
                        &request, &updated);
}

bool CommunityMultiplayerBackend::Migrate(uint64_t session_id,
                                          const SessionRecord &replacement) {
  SessionRecord published = replacement;
  if (replacement.host_xuid == identity_.xuid) {
    published.host_ipv4 = local_virtual_ipv4_;
    published.host_peer_id = transport_identity_;
  }
  json request = SessionToJson(published);
  SessionRecord updated;
  if (!RequestSession("POST",
                      fmt::format("/v1/sessions/{:016x}/migrate", session_id),
                      &request, &updated)) {
    return false;
  }
  std::lock_guard lock(session_mutex_);
  hosted_sessions_.erase(session_id);
  if (updated.host_xuid == identity_.xuid) {
    hosted_sessions_[updated.session_id] = std::move(updated);
  }
  return true;
}

bool CommunityMultiplayerBackend::Delete(uint64_t session_id) {
  json response;
  if (!Request("DELETE", fmt::format("/v1/sessions/{:016x}", session_id),
               nullptr, &response)) {
    return false;
  }
  std::lock_guard lock(session_mutex_);
  hosted_sessions_.erase(session_id);
  return true;
}

void CommunityMultiplayerBackend::ConfigureRoutes(
    const SessionRecord &session) {
  RegisterRoute(session.host_ipv4, session);
  std::lock_guard lock(route_mutex_);
  for (const auto &member : session.members) {
    if (!member.virtual_ipv4 || member.peer_id.empty() ||
        member.peer_id == transport_identity_) {
      continue;
    }
    routes_by_ipv4_[member.virtual_ipv4] = {
        .peer_id = member.peer_id, .virtual_ipv4 = member.virtual_ipv4};
    ipv4_by_peer_[member.peer_id] = member.virtual_ipv4;
  }
}

void CommunityMultiplayerBackend::RegisterRoute(uint32_t virtual_ipv4,
                                                const SessionRecord &session) {
  if (!virtual_ipv4 || session.host_peer_id.empty() ||
      session.host_peer_id == transport_identity_) {
    return;
  }
  std::lock_guard lock(route_mutex_);
  routes_by_ipv4_[virtual_ipv4] = {.peer_id = session.host_peer_id,
                                   .virtual_ipv4 = virtual_ipv4};
  ipv4_by_peer_[session.host_peer_id] = virtual_ipv4;
}

void CommunityMultiplayerBackend::UnregisterRoute(uint32_t virtual_ipv4) {
  std::lock_guard lock(route_mutex_);
  auto route = routes_by_ipv4_.find(virtual_ipv4);
  if (route == routes_by_ipv4_.end()) {
    return;
  }
#if !defined(LIBERTY_RECOMP_NO_GNS)
  if (messages_) {
    SteamNetworkingIdentity peer;
    peer.Clear();
    if (peer.SetGenericString(route->second.peer_id.c_str())) {
      messages_->CloseSessionWithUser(peer);
    }
  }
#endif
  ipv4_by_peer_.erase(route->second.peer_id);
  routes_by_ipv4_.erase(route);
}

bool CommunityMultiplayerBackend::Send(uint32_t destination_ipv4,
                                       uint16_t destination_port,
                                       uint16_t source_port,
                                       std::span<const uint8_t> payload) {
#if defined(LIBERTY_RECOMP_NO_GNS)
  return false;
#else
  if (!ready() || !messages_ ||
      payload.size() >
          std::numeric_limits<uint32_t>::max() - sizeof(uint16_t)) {
    return false;
  }
  std::string peer_id;
  {
    std::lock_guard lock(route_mutex_);
    const auto route = routes_by_ipv4_.find(destination_ipv4);
    if (route == routes_by_ipv4_.end()) {
      return false;
    }
    peer_id = route->second.peer_id;
  }
  SteamNetworkingIdentity peer;
  peer.Clear();
  if (!peer.SetGenericString(peer_id.c_str())) {
    return false;
  }
  std::vector<uint8_t> framed(sizeof(uint16_t) + payload.size());
  const uint16_t encoded_source_port = htons(source_port);
  std::memcpy(framed.data(), &encoded_source_port, sizeof(encoded_source_port));
  std::memcpy(framed.data() + sizeof(encoded_source_port), payload.data(),
              payload.size());
  const EResult result = messages_->SendMessageToUser(
      peer, framed.data(), static_cast<uint32_t>(framed.size()),
      k_nSteamNetworkingSend_Unreliable |
          k_nSteamNetworkingSend_AutoRestartBrokenSession,
      destination_port);
  return result == k_EResultOK;
#endif
}

std::optional<PeerDatagram>
CommunityMultiplayerBackend::Receive(uint16_t local_port,
                                     uint32_t maximum_payload_size) {
#if defined(LIBERTY_RECOMP_NO_GNS)
  return std::nullopt;
#else
  if (!PumpIncoming(local_port)) {
    return std::nullopt;
  }
  std::lock_guard lock(incoming_mutex_);
  auto queue = incoming_datagrams_.find(local_port);
  if (queue == incoming_datagrams_.end() || queue->second.empty()) {
    return std::nullopt;
  }
  PeerDatagram result = std::move(queue->second.front());
  queue->second.pop_front();
  if (queue->second.empty()) {
    incoming_datagrams_.erase(queue);
  }
  if (result.payload.size() > maximum_payload_size) {
    result.payload.resize(maximum_payload_size);
  }
  return result;
#endif
}

bool CommunityMultiplayerBackend::HasPending(uint16_t local_port) {
#if defined(LIBERTY_RECOMP_NO_GNS)
  return false;
#else
  return PumpIncoming(local_port);
#endif
}

bool CommunityMultiplayerBackend::PumpIncoming(uint16_t local_port) {
#if defined(LIBERTY_RECOMP_NO_GNS)
  return false;
#else
  if (!ready() || !messages_) {
    return false;
  }
  std::lock_guard incoming_lock(incoming_mutex_);
  if (const auto queue = incoming_datagrams_.find(local_port);
      queue != incoming_datagrams_.end() && !queue->second.empty()) {
    return true;
  }
  SteamNetworkingMessage_t *message = nullptr;
  if (messages_->ReceiveMessagesOnChannel(local_port, &message, 1) != 1 ||
      !message) {
    return false;
  }
  struct MessageReleaser {
    void operator()(SteamNetworkingMessage_t *value) const { value->Release(); }
  };
  std::unique_ptr<SteamNetworkingMessage_t, MessageReleaser> owned(message);
  if (message->m_cbSize < static_cast<int>(sizeof(uint16_t))) {
    return false;
  }
  const size_t payload_size =
      static_cast<size_t>(message->m_cbSize) - sizeof(uint16_t);
  const char *peer_id = message->m_identityPeer.GetGenericString();
  if (!peer_id) {
    return false;
  }
  uint32_t source_ipv4 = 0;
  {
    std::lock_guard lock(route_mutex_);
    const auto route = ipv4_by_peer_.find(peer_id);
    if (route == ipv4_by_peer_.end()) {
      return false;
    }
    source_ipv4 = route->second;
  }
  uint16_t encoded_source_port = 0;
  std::memcpy(&encoded_source_port, message->m_pData,
              sizeof(encoded_source_port));
  PeerDatagram result{.source_ipv4 = source_ipv4,
                      .source_port = ntohs(encoded_source_port)};
  const auto *payload =
      static_cast<const uint8_t *>(message->m_pData) + sizeof(uint16_t);
  result.payload.assign(payload, payload + payload_size);
  incoming_datagrams_[local_port].push_back(std::move(result));
  return true;
#endif
}

void CommunityMultiplayerBackend::QueueSignal(std::string peer_id,
                                              const void *data, size_t size) {
  json signal = {
      {"type", "signal"},
      {"to", std::move(peer_id)},
      {"payload_hex",
       BytesToHex(std::span(static_cast<const uint8_t *>(data), size))}};
  std::lock_guard lock(signaling_mutex_);
  if (signaling_queue_.size() >= kMaximumSignalingQueueDepth) {
    signaling_queue_.pop_front();
  }
  signaling_queue_.push_back(signal.dump());
}

bool CommunityMultiplayerBackend::ConnectWebSocket() {
#if defined(LIBERTY_RECOMP_NO_CURL) || LIBCURL_VERSION_NUM < 0x075600
  SetError("This libcurl build does not support WebSocket connections");
  return false;
#else
  if (websocket_) {
    curl_easy_cleanup(websocket_);
    websocket_ = nullptr;
  }
  if (websocket_headers_) {
    curl_slist_free_all(websocket_headers_);
    websocket_headers_ = nullptr;
  }
  websocket_ = curl_easy_init();
  if (!websocket_) {
    SetError("Unable to allocate the rendezvous WebSocket");
    return false;
  }
  std::string auth_token;
  std::string websocket_url;
  {
    std::lock_guard lock(service_mutex_);
    auth_token = auth_token_;
    websocket_url = websocket_url_;
  }
  const std::string authorization = "Authorization: Bearer " + auth_token;
  websocket_headers_ =
      curl_slist_append(websocket_headers_, authorization.c_str());
  curl_easy_setopt(websocket_, CURLOPT_URL, websocket_url.c_str());
#if LIBCURL_VERSION_NUM >= 0x075500
  curl_easy_setopt(websocket_, CURLOPT_PROTOCOLS_STR, "wss,ws");
#else
  curl_easy_setopt(websocket_, CURLOPT_PROTOCOLS,
                   static_cast<long>(CURLPROTO_HTTPS | CURLPROTO_HTTP));
#endif
  curl_easy_setopt(websocket_, CURLOPT_HTTPHEADER, websocket_headers_);
  curl_easy_setopt(websocket_, CURLOPT_CONNECT_ONLY, 2L);
  curl_easy_setopt(
      websocket_, CURLOPT_CONNECTTIMEOUT_MS,
      static_cast<long>(
          std::chrono::duration_cast<std::chrono::milliseconds>(kConnectTimeout)
              .count()));
  curl_easy_setopt(websocket_, CURLOPT_NOSIGNAL, 1L);
  const CURLcode result = curl_easy_perform(websocket_);
  if (result != CURLE_OK) {
    SetError(fmt::format("Community rendezvous connection failed: {}",
                         curl_easy_strerror(result)));
    return false;
  }
  websocket_receive_message_.clear();
  websocket_send_message_.reset();
  websocket_send_offset_ = 0;
  return true;
#endif
}

bool CommunityMultiplayerBackend::FlushWebSocketMessage() {
#if defined(LIBERTY_RECOMP_NO_CURL) || LIBCURL_VERSION_NUM < 0x075600
  return false;
#else
  if (!websocket_send_message_) {
    std::lock_guard lock(signaling_mutex_);
    if (signaling_queue_.empty()) {
      return true;
    }
    websocket_send_message_ = std::move(signaling_queue_.front());
    signaling_queue_.pop_front();
    websocket_send_offset_ = 0;
  }
  const std::string &message = *websocket_send_message_;
  size_t sent = 0;
  const unsigned int flags =
      websocket_send_offset_ == 0 ? CURLWS_TEXT : CURLWS_CONT;
  const CURLcode result =
      curl_ws_send(websocket_, message.data() + websocket_send_offset_,
                   message.size() - websocket_send_offset_, &sent,
                   websocket_send_offset_ == 0 ? message.size() : 0, flags);
  if (result == CURLE_AGAIN) {
    return true;
  }
  if (result != CURLE_OK || sent == 0) {
    SetError(fmt::format("Community rendezvous send failed: {}",
                         curl_easy_strerror(result)));
    return false;
  }
  websocket_send_offset_ += sent;
  if (websocket_send_offset_ == message.size()) {
    websocket_send_message_.reset();
    websocket_send_offset_ = 0;
  }
  return true;
#endif
}

bool CommunityMultiplayerBackend::PollWebSocket() {
#if !defined(LIBERTY_RECOMP_NO_CURL) && LIBCURL_VERSION_NUM >= 0x075600
  std::array<char, 65536> buffer{};
  size_t received = 0;
  const curl_ws_frame *metadata = nullptr;
  const CURLcode result = curl_ws_recv(websocket_, buffer.data(), buffer.size(),
                                       &received, &metadata);
  if (result == CURLE_AGAIN) {
    return true;
  }
  if (result != CURLE_OK || !metadata) {
    SetError(fmt::format("Community rendezvous receive failed: {}",
                         curl_easy_strerror(result)));
    return false;
  }
  if ((metadata->flags & CURLWS_CLOSE) != 0) {
    SetError("Community rendezvous service closed the connection");
    return false;
  }
  if ((metadata->flags & (CURLWS_TEXT | CURLWS_CONT)) == 0) {
    return true;
  }
  if (received > kMaximumWebSocketMessageSize ||
      websocket_receive_message_.size() >
          kMaximumWebSocketMessageSize - received) {
    SetError("Community rendezvous message exceeded the configured limit");
    return false;
  }
  websocket_receive_message_.append(buffer.data(), received);
  if (metadata->bytesleft == 0) {
    HandleWebSocketMessage(websocket_receive_message_);
    websocket_receive_message_.clear();
  }
  return true;
#else
  return false;
#endif
}

void CommunityMultiplayerBackend::HandleWebSocketMessage(
    std::string_view message) {
  try {
    const json document = json::parse(message);
    const std::string type = document.at("type").get<std::string>();
    if (type == "signal") {
#if !defined(LIBERTY_RECOMP_NO_GNS)
      auto payload = HexToBytes(document.at("payload_hex").get<std::string>());
      if (!payload || payload->empty() ||
          payload->size() > std::numeric_limits<int>::max()) {
        return;
      }
      ReceiveContext context(this);
      sockets_->ReceivedP2PCustomSignal(
          payload->data(), static_cast<int>(payload->size()), &context);
#endif
    } else if (type == "session" || type == "session_updated" ||
               type == "member_joined" || type == "member_left" ||
               type == "host_migrated") {
      const json &encoded =
          document.contains("session") ? document.at("session") : document;
      if (auto session = SessionFromJson(encoded)) {
        ConfigureRoutes(*session);
        std::lock_guard lock(session_mutex_);
        if (hosted_sessions_.contains(session->session_id)) {
          hosted_sessions_[session->session_id] = std::move(*session);
        }
      }
    } else if (type == "session_deleted") {
      const auto session_id = StringToUint64(document.at("session_id"));
      if (session_id) {
        std::lock_guard lock(session_mutex_);
        hosted_sessions_.erase(*session_id);
      }
    }
  } catch (const json::exception &) {
    REXSYS_WARN("Ignoring malformed community rendezvous message");
  }
}

void CommunityMultiplayerBackend::WorkerMain() {
  bool connected_once = false;
  while (running_.load(std::memory_order_acquire)) {
    if (connected_once && (!RegisterClient() || !ConfigureIce())) {
      std::this_thread::sleep_for(kHeartbeatInterval);
      continue;
    }
    if (!ConnectWebSocket()) {
      if (!connected_once) {
        running_.store(false, std::memory_order_release);
        return;
      }
      std::this_thread::sleep_for(kHeartbeatInterval);
      continue;
    }
    connected_once = true;
    SetReady(true);
    auto next_heartbeat = std::chrono::steady_clock::now() + kHeartbeatInterval;
    while (running_.load(std::memory_order_acquire) && ready()) {
      if (!FlushWebSocketMessage() || !PollWebSocket()) {
        break;
      }
#if !defined(LIBERTY_RECOMP_NO_GNS)
      sockets_->RunCallbacks();
#endif
      const auto now = std::chrono::steady_clock::now();
      if (now >= next_heartbeat) {
        std::vector<SessionRecord> hosted;
        {
          std::lock_guard lock(session_mutex_);
          hosted.reserve(hosted_sessions_.size());
          for (const auto &[session_id, session] : hosted_sessions_) {
            hosted.push_back(session);
          }
        }
        for (const auto &session : hosted) {
          Heartbeat(session);
        }
        next_heartbeat = now + kHeartbeatInterval;
      }
      std::this_thread::sleep_for(kWebSocketPollInterval);
    }
    SetReady(false);
    if (running_.load(std::memory_order_acquire)) {
      std::this_thread::sleep_for(kHeartbeatInterval);
    }
  }
  SetReady(false);
}

} // namespace

LiveBackendServices
CreateCommunityMultiplayerBackend(const LiveConfig &config,
                                  const LiveIdentity &identity) {
  auto backend =
      std::make_shared<CommunityMultiplayerBackend>(config, identity);
  return {.session_directory = backend, .peer_transport = std::move(backend)};
}

} // namespace LibertyRecomp::Network
