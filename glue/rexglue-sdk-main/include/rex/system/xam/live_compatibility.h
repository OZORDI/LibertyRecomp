/**
 ******************************************************************************
 * @file        live_compatibility.h
 * @brief       Title-facing Xbox Live compatibility services.
 *
 * This is a community service abstraction. It never authenticates with or
 * connects to Microsoft's Xbox Live service.
 ******************************************************************************
 */

#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace rex::system::xam {

// GTA IV's extended multiplayer compatibility profile uses one authoritative
// capacity across discovery, XSession, and the community directory. Keeping
// this public prevents the three transports from silently drifting apart.
inline constexpr uint32_t kMaximumSessionMembers = 64;

struct LiveConfig;
struct LiveIdentity;
class ISessionDirectory;
class IPeerDatagramTransport;

struct LiveBackendServices {
  std::shared_ptr<ISessionDirectory> session_directory;
  std::shared_ptr<IPeerDatagramTransport> peer_transport;
};

using LiveBackendFactory =
    std::function<LiveBackendServices(const LiveConfig&, const LiveIdentity&)>;

enum class LiveBackend : uint32_t {
  kOffline,
  kLan,
  kCommunity,
};

enum class LiveState : uint32_t {
  kOffline,
  kConnecting,
  kAvailable,
  kError,
};

enum class RelayPolicy : uint32_t {
  kAuto,
  kDirectOnly,
  kRelayOnly,
};

struct LiveConfig {
  LiveBackend backend = LiveBackend::kOffline;
  RelayPolicy relay_policy = RelayPolicy::kAuto;
  uint32_t session_protocol_version = 1;
  uint16_t lan_discovery_port = 3074;
  std::string community_url;
  std::string player_name = "Player";
  LiveBackendFactory community_backend_factory;
};

struct LiveIdentity {
  uint64_t xuid = 0;
  uint64_t machine_id = 0;
  std::array<uint8_t, 6> ethernet_address{};
  std::array<uint8_t, 32> install_secret{};
  std::string player_name;
};

struct SessionContext {
  uint32_t id = 0;
  uint32_t value = 0;

  bool operator==(const SessionContext&) const = default;
};

struct SessionProperty {
  uint32_t id = 0;
  std::vector<uint8_t> value;

  bool operator==(const SessionProperty&) const = default;
};

struct SessionMember {
  uint64_t xuid = 0;
  bool private_slot = false;
  uint64_t machine_id = 0;
  uint32_t virtual_ipv4 = 0;  // Network byte order, matching in_addr::s_addr.
  uint16_t online_port = 0;
  std::string peer_id;

  bool operator==(const SessionMember&) const = default;
};

enum class SessionLifecycleState : uint32_t {
  kLobby,
  kRegistration,
  kInGame,
  kReporting,
  kDeleted,
};

struct SessionRecord {
  uint32_t title_id = 0;
  uint32_t media_id = 0;
  uint32_t title_version = 0;
  uint32_t protocol_version = 1;
  uint64_t session_id = 0;
  uint64_t previous_session_id = 0;
  std::array<uint8_t, 16> exchange_key{};
  uint64_t nonce = 0;
  uint32_t flags = 0;
  SessionLifecycleState lifecycle_state = SessionLifecycleState::kLobby;
  uint32_t max_public_slots = 0;
  uint32_t max_private_slots = 0;
  uint32_t open_public_slots = 0;
  uint32_t open_private_slots = 0;
  uint64_t host_xuid = 0;
  uint64_t host_machine_id = 0;
  uint32_t host_ipv4 = 0;  // Network byte order, matching in_addr::s_addr.
  uint16_t host_port = 0;  // Host byte order.
  std::array<uint8_t, 6> host_ethernet_address{};
  std::string host_peer_id;
  std::vector<SessionContext> contexts;
  std::vector<SessionProperty> properties;
  std::vector<SessionMember> members;
  std::chrono::steady_clock::time_point last_seen = std::chrono::steady_clock::now();
};

// Validates every transport-visible invariant, including the shared 64-member
// cap and public/private slot accounting. Community backends must apply this
// after decoding untrusted directory responses.
bool IsValidSessionRecord(const SessionRecord& session);

struct PeerDatagram {
  uint32_t source_ipv4 = 0;  // Network byte order, matching in_addr::s_addr.
  uint16_t source_port = 0;  // Host byte order.
  std::vector<uint8_t> payload;
};

class IPeerDatagramTransport {
 public:
  virtual ~IPeerDatagramTransport() = default;

  virtual bool ready() const = 0;
  virtual std::string last_error() const = 0;
  virtual void RegisterRoute(uint32_t virtual_ipv4, const SessionRecord& session) = 0;
  virtual void UnregisterRoute(uint32_t virtual_ipv4) = 0;
  virtual bool Send(uint32_t destination_ipv4, uint16_t destination_port, uint16_t source_port,
                    std::span<const uint8_t> payload) = 0;
  virtual bool HasPending(uint16_t local_port) = 0;
  virtual std::optional<PeerDatagram> Receive(uint16_t local_port,
                                              uint32_t maximum_payload_size) = 0;
};

class ISessionDirectory {
 public:
  virtual ~ISessionDirectory() = default;

  virtual bool ready() const = 0;
  virtual std::string last_error() const = 0;
  virtual bool Create(const SessionRecord& session) = 0;
  virtual bool Heartbeat(const SessionRecord& session) = 0;
  virtual std::vector<SessionRecord> Search(uint32_t title_id, uint32_t media_id,
                                            uint32_t title_version, uint32_t protocol_version,
                                            std::span<const SessionContext> contexts,
                                            std::span<const SessionProperty> properties,
                                            uint32_t maximum_results) = 0;
  virtual std::optional<SessionRecord> Get(uint64_t session_id) = 0;
  virtual bool Modify(const SessionRecord& session) = 0;
  virtual bool Join(uint64_t session_id, const SessionMember& member) = 0;
  virtual bool Leave(uint64_t session_id, uint64_t xuid) = 0;
  virtual bool Migrate(uint64_t session_id, const SessionRecord& replacement) = 0;
  virtual bool Delete(uint64_t session_id) = 0;
};

class InMemorySessionDirectory final : public ISessionDirectory {
 public:
  bool ready() const override { return true; }
  std::string last_error() const override { return {}; }
  bool Create(const SessionRecord& session) override;
  bool Heartbeat(const SessionRecord& session) override;
  std::vector<SessionRecord> Search(uint32_t title_id, uint32_t media_id, uint32_t title_version,
                                    uint32_t protocol_version,
                                    std::span<const SessionContext> contexts,
                                    std::span<const SessionProperty> properties,
                                    uint32_t maximum_results) override;
  std::optional<SessionRecord> Get(uint64_t session_id) override;
  bool Modify(const SessionRecord& session) override;
  bool Join(uint64_t session_id, const SessionMember& member) override;
  bool Leave(uint64_t session_id, uint64_t xuid) override;
  bool Migrate(uint64_t session_id, const SessionRecord& replacement) override;
  bool Delete(uint64_t session_id) override;

 private:
  std::mutex mutex_;
  std::unordered_map<uint64_t, SessionRecord> sessions_;
};

class LanSessionDirectory final : public ISessionDirectory {
 public:
  explicit LanSessionDirectory(uint16_t discovery_port);
  ~LanSessionDirectory() override;

  bool ready() const override { return ready_.load(std::memory_order_acquire); }
  std::string last_error() const override;
  bool Create(const SessionRecord& session) override;
  bool Heartbeat(const SessionRecord& session) override;
  std::vector<SessionRecord> Search(uint32_t title_id, uint32_t media_id, uint32_t title_version,
                                    uint32_t protocol_version,
                                    std::span<const SessionContext> contexts,
                                    std::span<const SessionProperty> properties,
                                    uint32_t maximum_results) override;
  std::optional<SessionRecord> Get(uint64_t session_id) override;
  bool Modify(const SessionRecord& session) override;
  bool Join(uint64_t session_id, const SessionMember& member) override;
  bool Leave(uint64_t session_id, uint64_t xuid) override;
  bool Migrate(uint64_t session_id, const SessionRecord& replacement) override;
  bool Delete(uint64_t session_id) override;

 private:
  void WorkerMain();
  bool OpenSocket();
  void CloseSocket();
  void SetError(std::string message);
  bool SendQuery();
  bool SendRecord(const SessionRecord& session, uint8_t operation);
  void ReceivePacket();
  void ExpireRecords();

  uint16_t discovery_port_;
  std::atomic<bool> running_{true};
  std::atomic<bool> ready_{false};
  std::thread worker_;
  mutable std::mutex mutex_;
  std::condition_variable search_condition_;
  std::unordered_map<uint64_t, SessionRecord> hosted_sessions_;
  std::unordered_map<uint64_t, SessionRecord> discovered_sessions_;
  std::unordered_map<uint64_t, std::chrono::steady_clock::time_point> deletion_tombstones_;
  std::string last_error_;
  intptr_t socket_ = -1;
};

class LiveCompatibilityRuntime {
 public:
  LiveCompatibilityRuntime(const LiveConfig& config, const std::filesystem::path& user_data_root);
  ~LiveCompatibilityRuntime();

  LiveState state() const;
  bool available() const { return state() == LiveState::kAvailable; }
  const LiveConfig& config() const { return config_; }
  const LiveIdentity& identity() const { return identity_; }
  ISessionDirectory* session_directory() const { return session_directory_.get(); }
  IPeerDatagramTransport* peer_transport() const { return peer_transport_.get(); }

  uint32_t local_ipv4() const { return local_ipv4_; }
  uint16_t online_port() const { return online_port_.load(std::memory_order_acquire); }
  void ObserveBoundPort(uint16_t port);

  bool SetUserContext(uint32_t id, uint32_t value);
  std::optional<uint32_t> GetUserContext(uint32_t id) const;
  std::vector<SessionContext> user_contexts() const;
  bool SetUserProperty(uint32_t id, std::span<const uint8_t> value);
  std::vector<SessionProperty> user_properties() const;

  uint64_t GenerateSessionId();
  uint64_t GenerateNonce();
  void GenerateExchangeKey(std::span<uint8_t, 16> key);
  void FillRandomBytes(std::span<uint8_t> output);
  bool IsPrivilegeAllowed(uint32_t privilege) const;

  void RegisterRoute(uint32_t ipv4, const SessionRecord& session);
  std::optional<SessionRecord> FindRoute(uint32_t ipv4) const;
  void UnregisterRoute(uint32_t ipv4);
  void RegisterKey(uint64_t session_id, std::span<const uint8_t, 16> key);
  bool IsKeyRegistered(uint64_t session_id) const;
  void UnregisterKey(uint64_t session_id);
  bool SendPeerDatagram(uint32_t destination_ipv4, uint16_t destination_port, uint16_t source_port,
                        std::span<const uint8_t> payload);
  bool HasPendingPeerDatagram(uint16_t local_port);
  std::optional<PeerDatagram> ReceivePeerDatagram(uint16_t local_port,
                                                  uint32_t maximum_payload_size);

 private:
  bool LoadOrCreateIdentity(const std::filesystem::path& user_data_root);
  static uint32_t DiscoverLocalIpv4();
  static uint64_t RandomU64();
  static void FillRandom(std::span<uint8_t> output);

  LiveConfig config_;
  std::atomic<LiveState> state_{LiveState::kOffline};
  LiveIdentity identity_;
  std::shared_ptr<ISessionDirectory> session_directory_;
  std::shared_ptr<IPeerDatagramTransport> peer_transport_;
  uint32_t local_ipv4_ = 0;
  std::atomic<uint16_t> online_port_{3074};

  mutable std::mutex user_data_mutex_;
  std::unordered_map<uint32_t, uint32_t> user_contexts_;
  std::unordered_map<uint32_t, std::vector<uint8_t>> user_properties_;

  mutable std::mutex route_mutex_;
  std::unordered_map<uint32_t, SessionRecord> routes_;
  std::unordered_map<uint64_t, std::array<uint8_t, 16>> registered_keys_;
};

}  // namespace rex::system::xam
