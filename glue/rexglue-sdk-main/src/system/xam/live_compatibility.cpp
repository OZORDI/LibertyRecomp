/**
 ******************************************************************************
 * @file        live_compatibility.cpp
 * @brief       Community/LAN Xbox Live compatibility runtime.
 ******************************************************************************
 */

#include <rex/system/xam/live_compatibility.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <limits>
#include <random>
#include <ranges>
#include <system_error>

#include <rex/logging.h>
#include <rex/net/socket.h>
#include <rex/platform.h>

#if REX_PLATFORM_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace rex::system::xam {
namespace {

constexpr std::array<uint8_t, 8> kLanMagic = {'L', 'B', 'R', 'X', 'N', 'E', 'T', 0};
constexpr uint8_t kLanProtocolVersion = 3;
constexpr uint8_t kOperationQuery = 1;
constexpr uint8_t kOperationAdvertise = 2;
constexpr uint8_t kOperationDelete = 3;
constexpr uint8_t kOperationJoin = 4;
constexpr uint8_t kOperationLeave = 5;
constexpr uint8_t kOperationMigrate = 6;
constexpr size_t kMaximumDatagramSize = 1400;
constexpr size_t kMaximumContexts = 64;
constexpr size_t kMaximumProperties = 64;
constexpr size_t kMaximumPropertySize = 512;
constexpr std::chrono::milliseconds kSearchWindow{350};
constexpr std::chrono::seconds kMutationWindow{2};
constexpr std::chrono::seconds kAdvertisementInterval{1};
constexpr std::chrono::seconds kRecordLifetime{5};
constexpr const char* kMulticastAddress = "239.255.42.99";
constexpr uint32_t kIdentityVersion = 1;
constexpr std::array<uint8_t, 8> kIdentityMagic = {'L', 'B', 'R', 'L', 'I', 'V', 'E', 0};

struct IdentityDisk {
  std::array<uint8_t, 8> magic{};
  uint32_t version = 0;
  uint64_t xuid = 0;
  uint64_t machine_id = 0;
  std::array<uint8_t, 6> ethernet_address{};
  std::array<uint8_t, 32> install_secret{};
  std::array<char, 16> player_name{};
};
static_assert(std::is_trivially_copyable_v<IdentityDisk>);

void RestrictIdentityPermissions(const std::filesystem::path& path) {
#if !REX_PLATFORM_WIN32
  std::error_code error;
  std::filesystem::permissions(
      path, std::filesystem::perms::owner_read | std::filesystem::perms::owner_write,
      std::filesystem::perm_options::replace, error);
  if (error) {
    REXSYS_WARN("Unable to restrict multiplayer identity permissions for {}: {}", path.string(),
                error.message());
  }
#endif
}

bool ContextsMatch(const SessionRecord& session, std::span<const SessionContext> required) {
  return std::ranges::all_of(required, [&session](const SessionContext& filter) {
    return std::ranges::any_of(
        session.contexts, [&filter](const SessionContext& context) { return context == filter; });
  });
}

bool PropertiesMatch(const SessionRecord& session, std::span<const SessionProperty> required) {
  return std::ranges::all_of(required, [&session](const SessionProperty& filter) {
    return std::ranges::any_of(session.properties, [&filter](const SessionProperty& property) {
      return property == filter;
    });
  });
}

bool ValidateRecord(const SessionRecord& session) {
  if (!session.session_id || !session.protocol_version ||
      session.contexts.size() > kMaximumContexts ||
      session.properties.size() > kMaximumProperties ||
      session.members.size() > kMaximumSessionMembers ||
      session.max_public_slots > kMaximumSessionMembers ||
      session.max_private_slots > kMaximumSessionMembers ||
      session.max_private_slots > kMaximumSessionMembers - session.max_public_slots ||
      session.open_public_slots > session.max_public_slots ||
      session.open_private_slots > session.max_private_slots) {
    return false;
  }
  if (std::ranges::any_of(session.properties, [](const SessionProperty& property) {
        return property.value.size() > kMaximumPropertySize;
      })) {
    return false;
  }
  for (size_t left = 0; left < session.members.size(); ++left) {
    if (!session.members[left].xuid) {
      return false;
    }
    for (size_t right = left + 1; right < session.members.size(); ++right) {
      if (session.members[left].xuid == session.members[right].xuid) {
        return false;
      }
    }
  }
  const auto private_members = static_cast<uint32_t>(
      std::ranges::count(session.members, true, &SessionMember::private_slot));
  const auto public_members = static_cast<uint32_t>(session.members.size()) - private_members;
  if (private_members + session.open_private_slots != session.max_private_slots ||
      public_members + session.open_public_slots != session.max_public_slots) {
    return false;
  }
  return true;
}

bool AddMember(SessionRecord& session, SessionMember member) {
  auto existing = std::ranges::find(session.members, member.xuid, &SessionMember::xuid);
  if (existing != session.members.end()) {
    return true;
  }

  if (member.private_slot && session.open_private_slots) {
    --session.open_private_slots;
  } else if (session.open_public_slots) {
    member.private_slot = false;
    --session.open_public_slots;
  } else if (session.open_private_slots) {
    member.private_slot = true;
    --session.open_private_slots;
  } else {
    return false;
  }

  session.members.push_back(member);
  return true;
}

bool RemoveMember(SessionRecord& session, uint64_t xuid) {
  auto existing = std::ranges::find(session.members, xuid, &SessionMember::xuid);
  if (existing == session.members.end()) {
    return true;
  }

  if (existing->private_slot) {
    session.open_private_slots =
        std::min(session.max_private_slots, session.open_private_slots + 1);
  } else {
    session.open_public_slots = std::min(session.max_public_slots, session.open_public_slots + 1);
  }
  session.members.erase(existing);
  return true;
}

class PacketWriter {
 public:
  bool PutU8(uint8_t value) { return PutBytes(std::span<const uint8_t>(&value, 1)); }

  bool PutU16(uint16_t value) {
    std::array<uint8_t, 2> bytes = {static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value)};
    return PutBytes(bytes);
  }

  bool PutU32(uint32_t value) {
    std::array<uint8_t, 4> bytes = {static_cast<uint8_t>(value >> 24),
                                    static_cast<uint8_t>(value >> 16),
                                    static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value)};
    return PutBytes(bytes);
  }

  bool PutU64(uint64_t value) {
    std::array<uint8_t, 8> bytes = {
        static_cast<uint8_t>(value >> 56), static_cast<uint8_t>(value >> 48),
        static_cast<uint8_t>(value >> 40), static_cast<uint8_t>(value >> 32),
        static_cast<uint8_t>(value >> 24), static_cast<uint8_t>(value >> 16),
        static_cast<uint8_t>(value >> 8),  static_cast<uint8_t>(value)};
    return PutBytes(bytes);
  }

  bool PutBytes(std::span<const uint8_t> bytes) {
    if (bytes.size() > kMaximumDatagramSize - data_.size()) {
      valid_ = false;
      return false;
    }
    data_.insert(data_.end(), bytes.begin(), bytes.end());
    return true;
  }

  bool valid() const { return valid_; }
  const std::vector<uint8_t>& data() const { return data_; }

 private:
  bool valid_ = true;
  std::vector<uint8_t> data_;
};

class PacketReader {
 public:
  explicit PacketReader(std::span<const uint8_t> data) : data_(data) {}

  bool GetU8(uint8_t& value) { return GetBytes(std::span<uint8_t>(&value, 1)); }

  bool GetU16(uint16_t& value) {
    std::array<uint8_t, 2> bytes{};
    if (!GetBytes(bytes)) {
      return false;
    }
    value = static_cast<uint16_t>((static_cast<uint16_t>(bytes[0]) << 8) | bytes[1]);
    return true;
  }

  bool GetU32(uint32_t& value) {
    std::array<uint8_t, 4> bytes{};
    if (!GetBytes(bytes)) {
      return false;
    }
    value = (static_cast<uint32_t>(bytes[0]) << 24) | (static_cast<uint32_t>(bytes[1]) << 16) |
            (static_cast<uint32_t>(bytes[2]) << 8) | static_cast<uint32_t>(bytes[3]);
    return true;
  }

  bool GetU64(uint64_t& value) {
    std::array<uint8_t, 8> bytes{};
    if (!GetBytes(bytes)) {
      return false;
    }
    value = (static_cast<uint64_t>(bytes[0]) << 56) | (static_cast<uint64_t>(bytes[1]) << 48) |
            (static_cast<uint64_t>(bytes[2]) << 40) | (static_cast<uint64_t>(bytes[3]) << 32) |
            (static_cast<uint64_t>(bytes[4]) << 24) | (static_cast<uint64_t>(bytes[5]) << 16) |
            (static_cast<uint64_t>(bytes[6]) << 8) | static_cast<uint64_t>(bytes[7]);
    return true;
  }

  bool GetBytes(std::span<uint8_t> output) {
    if (offset_ > data_.size() || output.size() > data_.size() - offset_) {
      valid_ = false;
      return false;
    }
    std::memcpy(output.data(), data_.data() + offset_, output.size());
    offset_ += output.size();
    return true;
  }

  bool valid() const { return valid_; }

 private:
  std::span<const uint8_t> data_;
  size_t offset_ = 0;
  bool valid_ = true;
};

std::vector<uint8_t> SerializePacket(const SessionRecord* session, uint8_t operation) {
  PacketWriter writer;
  writer.PutBytes(kLanMagic);
  writer.PutU8(kLanProtocolVersion);
  writer.PutU8(operation);
  writer.PutU16(0);

  if (!session) {
    return writer.data();
  }

  if (session->contexts.size() > kMaximumContexts ||
      session->properties.size() > kMaximumProperties ||
      session->members.size() > kMaximumSessionMembers) {
    return {};
  }

  writer.PutU32(session->title_id);
  writer.PutU32(session->media_id);
  writer.PutU32(session->title_version);
  writer.PutU32(session->protocol_version);
  writer.PutU64(session->session_id);
  writer.PutU64(session->previous_session_id);
  writer.PutBytes(session->exchange_key);
  writer.PutU64(session->nonce);
  writer.PutU32(session->flags);
  writer.PutU32(static_cast<uint32_t>(session->lifecycle_state));
  writer.PutU32(session->max_public_slots);
  writer.PutU32(session->max_private_slots);
  writer.PutU32(session->open_public_slots);
  writer.PutU32(session->open_private_slots);
  writer.PutU64(session->host_xuid);
  writer.PutU64(session->host_machine_id);
  writer.PutBytes(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(&session->host_ipv4),
                                           sizeof(session->host_ipv4)));
  writer.PutU16(session->host_port);
  writer.PutBytes(session->host_ethernet_address);
  writer.PutU16(static_cast<uint16_t>(session->contexts.size()));
  writer.PutU16(static_cast<uint16_t>(session->properties.size()));
  writer.PutU16(static_cast<uint16_t>(session->members.size()));

  for (const auto& context : session->contexts) {
    writer.PutU32(context.id);
    writer.PutU32(context.value);
  }
  for (const auto& property : session->properties) {
    if (property.value.size() > kMaximumPropertySize) {
      return {};
    }
    writer.PutU32(property.id);
    writer.PutU16(static_cast<uint16_t>(property.value.size()));
    writer.PutBytes(property.value);
  }
  for (const auto& member : session->members) {
    writer.PutU64(member.xuid);
    writer.PutU8(member.private_slot ? 1 : 0);
  }
  return writer.valid() ? writer.data() : std::vector<uint8_t>{};
}

std::optional<std::pair<uint8_t, SessionRecord>> DeserializePacket(std::span<const uint8_t> bytes) {
  PacketReader reader(bytes);
  std::array<uint8_t, 8> magic{};
  uint8_t version = 0;
  uint8_t operation = 0;
  uint16_t reserved = 0;
  if (!reader.GetBytes(magic) || magic != kLanMagic || !reader.GetU8(version) ||
      version != kLanProtocolVersion || !reader.GetU8(operation) || !reader.GetU16(reserved)) {
    return std::nullopt;
  }

  SessionRecord session;
  if (operation == kOperationQuery) {
    return std::pair(operation, session);
  }

  uint16_t context_count = 0;
  uint16_t property_count = 0;
  uint16_t member_count = 0;
  uint32_t lifecycle_state = 0;
  if (!reader.GetU32(session.title_id) || !reader.GetU32(session.media_id) ||
      !reader.GetU32(session.title_version) || !reader.GetU32(session.protocol_version) ||
      !reader.GetU64(session.session_id) || !reader.GetU64(session.previous_session_id) ||
      !reader.GetBytes(session.exchange_key) || !reader.GetU64(session.nonce) ||
      !reader.GetU32(session.flags) || !reader.GetU32(lifecycle_state) ||
      !reader.GetU32(session.max_public_slots) || !reader.GetU32(session.max_private_slots) ||
      !reader.GetU32(session.open_public_slots) || !reader.GetU32(session.open_private_slots) ||
      !reader.GetU64(session.host_xuid) || !reader.GetU64(session.host_machine_id) ||
      !reader.GetBytes(std::span<uint8_t>(reinterpret_cast<uint8_t*>(&session.host_ipv4),
                                          sizeof(session.host_ipv4))) ||
      !reader.GetU16(session.host_port) || !reader.GetBytes(session.host_ethernet_address) ||
      !reader.GetU16(context_count) || !reader.GetU16(property_count) ||
      !reader.GetU16(member_count) || context_count > kMaximumContexts ||
      property_count > kMaximumProperties || member_count > kMaximumSessionMembers) {
    return std::nullopt;
  }
  if (lifecycle_state > static_cast<uint32_t>(SessionLifecycleState::kDeleted)) {
    return std::nullopt;
  }
  session.lifecycle_state = static_cast<SessionLifecycleState>(lifecycle_state);

  session.contexts.reserve(context_count);
  for (uint16_t index = 0; index < context_count; ++index) {
    SessionContext context;
    if (!reader.GetU32(context.id) || !reader.GetU32(context.value)) {
      return std::nullopt;
    }
    session.contexts.push_back(context);
  }

  session.properties.reserve(property_count);
  for (uint16_t index = 0; index < property_count; ++index) {
    SessionProperty property;
    uint16_t value_size = 0;
    if (!reader.GetU32(property.id) || !reader.GetU16(value_size) ||
        value_size > kMaximumPropertySize) {
      return std::nullopt;
    }
    property.value.resize(value_size);
    if (!reader.GetBytes(property.value)) {
      return std::nullopt;
    }
    session.properties.push_back(std::move(property));
  }

  session.members.reserve(member_count);
  for (uint16_t index = 0; index < member_count; ++index) {
    SessionMember member;
    uint8_t private_slot = 0;
    if (!reader.GetU64(member.xuid) || !reader.GetU8(private_slot)) {
      return std::nullopt;
    }
    member.private_slot = private_slot != 0;
    session.members.push_back(member);
  }

  session.last_seen = std::chrono::steady_clock::now();
  if ((operation == kOperationAdvertise || operation == kOperationMigrate) &&
      !ValidateRecord(session)) {
    return std::nullopt;
  }
  return reader.valid() ? std::optional(std::pair(operation, std::move(session))) : std::nullopt;
}

std::vector<SessionRecord> SearchRecords(
    const std::unordered_map<uint64_t, SessionRecord>& records, uint32_t title_id,
    uint32_t media_id, uint32_t title_version, uint32_t protocol_version,
    std::span<const SessionContext> contexts, std::span<const SessionProperty> properties,
    uint32_t maximum_results,
    const std::unordered_map<uint64_t, SessionRecord>* exclusions = nullptr) {
  std::vector<SessionRecord> result;
  for (const auto& [session_id, session] : records) {
    if (result.size() >= maximum_results) {
      break;
    }
    if (exclusions && exclusions->contains(session_id)) {
      continue;
    }
    if (session.title_id != title_id || session.protocol_version != protocol_version ||
        (media_id && session.media_id && session.media_id != media_id) ||
        (title_version && session.title_version && session.title_version != title_version) ||
        !ContextsMatch(session, contexts) || !PropertiesMatch(session, properties)) {
      continue;
    }
    result.push_back(session);
  }
  return result;
}

}  // namespace

bool IsValidSessionRecord(const SessionRecord& session) {
  return ValidateRecord(session);
}

bool InMemorySessionDirectory::Create(const SessionRecord& session) {
  std::lock_guard lock(mutex_);
  if (!ValidateRecord(session) || sessions_.contains(session.session_id)) {
    return false;
  }
  sessions_.emplace(session.session_id, session);
  return true;
}

bool InMemorySessionDirectory::Heartbeat(const SessionRecord& session) {
  return Modify(session);
}

std::vector<SessionRecord> InMemorySessionDirectory::Search(
    uint32_t title_id, uint32_t media_id, uint32_t title_version, uint32_t protocol_version,
    std::span<const SessionContext> contexts, std::span<const SessionProperty> properties,
    uint32_t maximum_results) {
  std::lock_guard lock(mutex_);
  return SearchRecords(sessions_, title_id, media_id, title_version, protocol_version, contexts,
                       properties, maximum_results);
}

std::optional<SessionRecord> InMemorySessionDirectory::Get(uint64_t session_id) {
  std::lock_guard lock(mutex_);
  auto it = sessions_.find(session_id);
  return it == sessions_.end() ? std::nullopt : std::optional(it->second);
}

bool InMemorySessionDirectory::Modify(const SessionRecord& session) {
  std::lock_guard lock(mutex_);
  if (!ValidateRecord(session)) {
    return false;
  }
  auto it = sessions_.find(session.session_id);
  if (it == sessions_.end()) {
    return false;
  }
  it->second = session;
  it->second.last_seen = std::chrono::steady_clock::now();
  return true;
}

bool InMemorySessionDirectory::Join(uint64_t session_id, const SessionMember& member) {
  std::lock_guard lock(mutex_);
  if (!member.xuid) {
    return false;
  }
  auto it = sessions_.find(session_id);
  return it != sessions_.end() && AddMember(it->second, member);
}

bool InMemorySessionDirectory::Leave(uint64_t session_id, uint64_t xuid) {
  std::lock_guard lock(mutex_);
  if (!xuid) {
    return false;
  }
  auto it = sessions_.find(session_id);
  return it != sessions_.end() && RemoveMember(it->second, xuid);
}

bool InMemorySessionDirectory::Migrate(uint64_t session_id, const SessionRecord& replacement) {
  std::lock_guard lock(mutex_);
  auto it = sessions_.find(session_id);
  if (it == sessions_.end() || !ValidateRecord(replacement) ||
      (replacement.session_id != session_id && sessions_.contains(replacement.session_id))) {
    return false;
  }
  SessionRecord migrated = replacement;
  migrated.previous_session_id = session_id;
  migrated.last_seen = std::chrono::steady_clock::now();
  sessions_.erase(it);
  sessions_[migrated.session_id] = std::move(migrated);
  return true;
}

bool InMemorySessionDirectory::Delete(uint64_t session_id) {
  std::lock_guard lock(mutex_);
  return sessions_.erase(session_id) != 0;
}

LanSessionDirectory::LanSessionDirectory(uint16_t discovery_port)
    : discovery_port_(discovery_port) {
  if (OpenSocket()) {
    worker_ = std::thread(&LanSessionDirectory::WorkerMain, this);
  }
}

LanSessionDirectory::~LanSessionDirectory() {
  std::vector<SessionRecord> hosted;
  {
    std::lock_guard lock(mutex_);
    hosted.reserve(hosted_sessions_.size());
    for (const auto& [session_id, record] : hosted_sessions_) {
      SessionRecord deleted;
      deleted.session_id = session_id;
      hosted.push_back(std::move(deleted));
    }
    hosted_sessions_.clear();
  }
  for (const auto& session : hosted) {
    SendRecord(session, kOperationDelete);
  }
  running_.store(false, std::memory_order_release);
  if (socket_ != rex::net::kInvalidSocket) {
#if REX_PLATFORM_WIN32
    shutdown(static_cast<SOCKET>(socket_), SD_BOTH);
#else
    shutdown(static_cast<int>(socket_), SHUT_RDWR);
#endif
  }
  if (worker_.joinable()) {
    worker_.join();
  }
  CloseSocket();
}

std::string LanSessionDirectory::last_error() const {
  std::lock_guard lock(mutex_);
  return last_error_;
}

bool LanSessionDirectory::Create(const SessionRecord& session) {
  if (!ready() || !ValidateRecord(session) ||
      SerializePacket(&session, kOperationAdvertise).empty()) {
    return false;
  }
  {
    std::lock_guard lock(mutex_);
    if (hosted_sessions_.contains(session.session_id)) {
      return false;
    }
    deletion_tombstones_.erase(session.session_id);
    hosted_sessions_[session.session_id] = session;
  }
  if (SendRecord(session, kOperationAdvertise)) {
    return true;
  }
  std::lock_guard lock(mutex_);
  hosted_sessions_.erase(session.session_id);
  return false;
}

bool LanSessionDirectory::Heartbeat(const SessionRecord& session) {
  return Modify(session);
}

std::vector<SessionRecord> LanSessionDirectory::Search(uint32_t title_id, uint32_t media_id,
                                                       uint32_t title_version,
                                                       uint32_t protocol_version,
                                                       std::span<const SessionContext> contexts,
                                                       std::span<const SessionProperty> properties,
                                                       uint32_t maximum_results) {
  if (!ready() || !maximum_results) {
    return {};
  }

  if (!SendQuery()) {
    return {};
  }
  std::unique_lock lock(mutex_);
  search_condition_.wait_for(lock, kSearchWindow);
  return SearchRecords(discovered_sessions_, title_id, media_id, title_version, protocol_version,
                       contexts, properties, maximum_results, &hosted_sessions_);
}

std::optional<SessionRecord> LanSessionDirectory::Get(uint64_t session_id) {
  std::lock_guard lock(mutex_);
  if (auto hosted = hosted_sessions_.find(session_id); hosted != hosted_sessions_.end()) {
    return hosted->second;
  }
  if (auto discovered = discovered_sessions_.find(session_id);
      discovered != discovered_sessions_.end()) {
    return discovered->second;
  }
  return std::nullopt;
}

bool LanSessionDirectory::Modify(const SessionRecord& session) {
  if (!ready() || !ValidateRecord(session) ||
      SerializePacket(&session, kOperationAdvertise).empty()) {
    return false;
  }
  SessionRecord previous;
  {
    std::lock_guard lock(mutex_);
    auto it = hosted_sessions_.find(session.session_id);
    if (it == hosted_sessions_.end()) {
      return false;
    }
    previous = it->second;
    it->second = session;
    it->second.last_seen = std::chrono::steady_clock::now();
  }
  if (SendRecord(session, kOperationAdvertise)) {
    return true;
  }
  std::lock_guard lock(mutex_);
  if (auto it = hosted_sessions_.find(session.session_id); it != hosted_sessions_.end()) {
    it->second = std::move(previous);
  }
  return false;
}

bool LanSessionDirectory::Join(uint64_t session_id, const SessionMember& member) {
  if (!ready() || !member.xuid) {
    return false;
  }
  SessionRecord operation;
  operation.session_id = session_id;
  operation.members.push_back(member);
  {
    std::lock_guard lock(mutex_);
    auto discovered = discovered_sessions_.find(session_id);
    if (discovered == discovered_sessions_.end()) {
      return false;
    }
    SessionRecord candidate = discovered->second;
    if (!AddMember(candidate, member)) {
      return false;
    }
  }
  if (!SendRecord(operation, kOperationJoin)) {
    return false;
  }

  std::unique_lock lock(mutex_);
  return search_condition_.wait_for(lock, kMutationWindow, [this, session_id, &member] {
    const auto discovered = discovered_sessions_.find(session_id);
    return discovered != discovered_sessions_.end() &&
           std::ranges::find(discovered->second.members, member.xuid, &SessionMember::xuid) !=
               discovered->second.members.end();
  });
}

bool LanSessionDirectory::Leave(uint64_t session_id, uint64_t xuid) {
  if (!ready() || !xuid) {
    return false;
  }
  SessionRecord operation;
  operation.session_id = session_id;
  operation.members.push_back(SessionMember{.xuid = xuid});
  {
    std::lock_guard lock(mutex_);
    const auto discovered = discovered_sessions_.find(session_id);
    if (discovered == discovered_sessions_.end()) {
      return false;
    }
    if (std::ranges::find(discovered->second.members, xuid, &SessionMember::xuid) ==
        discovered->second.members.end()) {
      return true;
    }
  }
  if (!SendRecord(operation, kOperationLeave)) {
    return false;
  }

  std::unique_lock lock(mutex_);
  return search_condition_.wait_for(lock, kMutationWindow, [this, session_id, xuid] {
    const auto discovered = discovered_sessions_.find(session_id);
    return discovered == discovered_sessions_.end() ||
           std::ranges::find(discovered->second.members, xuid, &SessionMember::xuid) ==
               discovered->second.members.end();
  });
}

bool LanSessionDirectory::Migrate(uint64_t session_id, const SessionRecord& replacement) {
  SessionRecord migrated = replacement;
  migrated.previous_session_id = session_id;
  migrated.last_seen = std::chrono::steady_clock::now();
  if (!ready() || !ValidateRecord(migrated) ||
      SerializePacket(&migrated, kOperationMigrate).empty()) {
    return false;
  }
  std::optional<SessionRecord> previous_hosted;
  std::optional<SessionRecord> previous_discovered;
  {
    std::lock_guard lock(mutex_);
    if (auto hosted = hosted_sessions_.find(session_id); hosted != hosted_sessions_.end()) {
      previous_hosted = hosted->second;
    }
    if (auto discovered = discovered_sessions_.find(session_id);
        discovered != discovered_sessions_.end()) {
      previous_discovered = discovered->second;
    }
    const bool had_hosted = previous_hosted.has_value();
    const bool had_discovered = previous_discovered.has_value();
    if ((!had_hosted && !had_discovered) ||
        (replacement.session_id != session_id &&
         (hosted_sessions_.contains(replacement.session_id) ||
          discovered_sessions_.contains(replacement.session_id)))) {
      return false;
    }
    hosted_sessions_.erase(session_id);
    discovered_sessions_.erase(session_id);
    deletion_tombstones_[session_id] = std::chrono::steady_clock::now();
    deletion_tombstones_.erase(migrated.session_id);
    hosted_sessions_[migrated.session_id] = migrated;
  }
  if (SendRecord(migrated, kOperationMigrate)) {
    return true;
  }

  std::lock_guard lock(mutex_);
  hosted_sessions_.erase(migrated.session_id);
  deletion_tombstones_.erase(session_id);
  if (previous_hosted) {
    hosted_sessions_[session_id] = std::move(*previous_hosted);
  }
  if (previous_discovered) {
    discovered_sessions_[session_id] = std::move(*previous_discovered);
  }
  return false;
}

bool LanSessionDirectory::Delete(uint64_t session_id) {
  SessionRecord deleted;
  deleted.session_id = session_id;
  std::optional<SessionRecord> previous;
  std::optional<SessionRecord> previous_discovered;
  {
    std::lock_guard lock(mutex_);
    auto hosted = hosted_sessions_.find(session_id);
    if (hosted == hosted_sessions_.end()) {
      return false;
    }
    previous = std::move(hosted->second);
    hosted_sessions_.erase(hosted);
    if (auto discovered = discovered_sessions_.find(session_id);
        discovered != discovered_sessions_.end()) {
      previous_discovered = std::move(discovered->second);
      discovered_sessions_.erase(discovered);
    }
    deletion_tombstones_[session_id] = std::chrono::steady_clock::now();
  }
  if (SendRecord(deleted, kOperationDelete)) {
    return true;
  }
  std::lock_guard lock(mutex_);
  deletion_tombstones_.erase(session_id);
  hosted_sessions_[session_id] = std::move(*previous);
  if (previous_discovered) {
    discovered_sessions_[session_id] = std::move(*previous_discovered);
  }
  return false;
}

void LanSessionDirectory::SetError(std::string message) {
  {
    std::lock_guard lock(mutex_);
    last_error_ = std::move(message);
  }
  ready_.store(false, std::memory_order_release);
}

bool LanSessionDirectory::OpenSocket() {
#if REX_PLATFORM_WIN32
  WSADATA winsock_data{};
  if (WSAStartup(MAKEWORD(2, 2), &winsock_data) != 0) {
    SetError("WSAStartup failed");
    return false;
  }
#endif

  socket_ = static_cast<intptr_t>(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
  if (socket_ == rex::net::kInvalidSocket) {
    SetError("Unable to create LAN discovery socket");
    return false;
  }

  int enabled = 1;
  setsockopt(static_cast<int>(socket_), SOL_SOCKET, SO_REUSEADDR,
             reinterpret_cast<const char*>(&enabled), sizeof(enabled));
#if defined(SO_REUSEPORT) && !REX_PLATFORM_WIN32
  setsockopt(static_cast<int>(socket_), SOL_SOCKET, SO_REUSEPORT,
             reinterpret_cast<const char*>(&enabled), sizeof(enabled));
#endif

  sockaddr_in bind_address{};
  bind_address.sin_family = AF_INET;
  bind_address.sin_port = htons(discovery_port_);
  bind_address.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(static_cast<int>(socket_), reinterpret_cast<sockaddr*>(&bind_address),
           sizeof(bind_address)) != 0) {
    SetError("Unable to bind LAN discovery socket");
    return false;
  }

  ip_mreq membership{};
  if (inet_pton(AF_INET, kMulticastAddress, &membership.imr_multiaddr) != 1) {
    SetError("Invalid LAN multicast address");
    return false;
  }
  membership.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(static_cast<int>(socket_), IPPROTO_IP, IP_ADD_MEMBERSHIP,
                 reinterpret_cast<const char*>(&membership), sizeof(membership)) != 0) {
    SetError("Unable to join LAN multicast group");
    return false;
  }

  unsigned char loopback = 1;
  setsockopt(static_cast<int>(socket_), IPPROTO_IP, IP_MULTICAST_LOOP,
             reinterpret_cast<const char*>(&loopback), sizeof(loopback));
  ready_.store(true, std::memory_order_release);
  return true;
}

void LanSessionDirectory::CloseSocket() {
  if (socket_ != rex::net::kInvalidSocket) {
    rex::net::socket_close(socket_);
    socket_ = rex::net::kInvalidSocket;
  }
#if REX_PLATFORM_WIN32
  WSACleanup();
#endif
}

bool LanSessionDirectory::SendQuery() {
  const auto packet = SerializePacket(nullptr, kOperationQuery);
  if (packet.empty() || socket_ == rex::net::kInvalidSocket) {
    return false;
  }
  sockaddr_in destination{};
  destination.sin_family = AF_INET;
  destination.sin_port = htons(discovery_port_);
  inet_pton(AF_INET, kMulticastAddress, &destination.sin_addr);
  if (sendto(static_cast<int>(socket_), reinterpret_cast<const char*>(packet.data()),
             static_cast<int>(packet.size()), 0, reinterpret_cast<sockaddr*>(&destination),
             sizeof(destination)) < 0) {
    SetError("Unable to send LAN discovery query");
    return false;
  }
  return true;
}

bool LanSessionDirectory::SendRecord(const SessionRecord& session, uint8_t operation) {
  const auto packet = SerializePacket(&session, operation);
  if (packet.empty() || socket_ == rex::net::kInvalidSocket) {
    return false;
  }
  sockaddr_in destination{};
  destination.sin_family = AF_INET;
  destination.sin_port = htons(discovery_port_);
  inet_pton(AF_INET, kMulticastAddress, &destination.sin_addr);
  if (sendto(static_cast<int>(socket_), reinterpret_cast<const char*>(packet.data()),
             static_cast<int>(packet.size()), 0, reinterpret_cast<sockaddr*>(&destination),
             sizeof(destination)) < 0) {
    SetError("Unable to send LAN session packet");
    return false;
  }
  return true;
}

void LanSessionDirectory::ReceivePacket() {
  std::array<uint8_t, kMaximumDatagramSize> bytes{};
  sockaddr_in source{};
#if REX_PLATFORM_WIN32
  int source_size = sizeof(source);
#else
  socklen_t source_size = sizeof(source);
#endif
  const int received = recvfrom(static_cast<int>(socket_), reinterpret_cast<char*>(bytes.data()),
                                static_cast<int>(bytes.size()), 0,
                                reinterpret_cast<sockaddr*>(&source), &source_size);
  if (received <= 0) {
    return;
  }

  auto packet = DeserializePacket(std::span(bytes.data(), static_cast<size_t>(received)));
  if (!packet) {
    return;
  }
  auto& [operation, session] = *packet;
  if (operation == kOperationQuery) {
    std::vector<SessionRecord> hosted;
    {
      std::lock_guard lock(mutex_);
      hosted.reserve(hosted_sessions_.size());
      for (const auto& [session_id, record] : hosted_sessions_) {
        hosted.push_back(record);
      }
    }
    for (const auto& record : hosted) {
      SendRecord(record, kOperationAdvertise);
    }
    return;
  }

  session.host_ipv4 = source.sin_addr.s_addr;
  std::optional<SessionRecord> authoritative_update;
  {
    std::lock_guard lock(mutex_);
    if (operation == kOperationDelete) {
      discovered_sessions_.erase(session.session_id);
      deletion_tombstones_[session.session_id] = std::chrono::steady_clock::now();
    } else if (operation == kOperationJoin || operation == kOperationLeave) {
      auto hosted = hosted_sessions_.find(session.session_id);
      if (hosted != hosted_sessions_.end() && !session.members.empty()) {
        const bool accepted = operation == kOperationJoin
                                  ? AddMember(hosted->second, session.members.front())
                                  : RemoveMember(hosted->second, session.members.front().xuid);
        if (accepted) {
          hosted->second.last_seen = std::chrono::steady_clock::now();
          authoritative_update = hosted->second;
        }
      }
    } else if (operation == kOperationMigrate) {
      if (session.previous_session_id) {
        hosted_sessions_.erase(session.previous_session_id);
        discovered_sessions_.erase(session.previous_session_id);
        deletion_tombstones_[session.previous_session_id] = std::chrono::steady_clock::now();
      }
      deletion_tombstones_.erase(session.session_id);
      if (!hosted_sessions_.contains(session.session_id)) {
        discovered_sessions_[session.session_id] = session;
      }
    } else if (operation == kOperationAdvertise && !hosted_sessions_.contains(session.session_id) &&
               !deletion_tombstones_.contains(session.session_id)) {
      discovered_sessions_[session.session_id] = session;
    }
    search_condition_.notify_all();
  }
  if (authoritative_update) {
    SendRecord(*authoritative_update, kOperationAdvertise);
  }
}

void LanSessionDirectory::ExpireRecords() {
  const auto cutoff = std::chrono::steady_clock::now() - kRecordLifetime;
  std::lock_guard lock(mutex_);
  std::erase_if(discovered_sessions_,
                [cutoff](const auto& entry) { return entry.second.last_seen < cutoff; });
  std::erase_if(deletion_tombstones_,
                [cutoff](const auto& entry) { return entry.second < cutoff; });
}

void LanSessionDirectory::WorkerMain() {
  auto next_advertisement = std::chrono::steady_clock::now();
  while (running_.load(std::memory_order_acquire)) {
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(static_cast<int>(socket_), &read_set);
    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 250000;
    const int select_result =
        select(static_cast<int>(socket_) + 1, &read_set, nullptr, nullptr, &timeout);
    if (select_result < 0) {
      if (running_.load(std::memory_order_acquire)) {
        SetError("LAN discovery socket failed");
      }
      break;
    }
    if (select_result > 0 && FD_ISSET(static_cast<int>(socket_), &read_set)) {
      ReceivePacket();
    }

    const auto now = std::chrono::steady_clock::now();
    if (now >= next_advertisement) {
      std::vector<SessionRecord> hosted;
      {
        std::lock_guard lock(mutex_);
        hosted.reserve(hosted_sessions_.size());
        for (const auto& [session_id, record] : hosted_sessions_) {
          hosted.push_back(record);
        }
      }
      for (const auto& record : hosted) {
        SendRecord(record, kOperationAdvertise);
      }
      ExpireRecords();
      next_advertisement = now + kAdvertisementInterval;
    }
  }
}

LiveCompatibilityRuntime::LiveCompatibilityRuntime(const LiveConfig& config,
                                                   const std::filesystem::path& user_data_root)
    : config_(config), local_ipv4_(DiscoverLocalIpv4()) {
  state_.store(LiveState::kConnecting, std::memory_order_release);
  if (!LoadOrCreateIdentity(user_data_root)) {
    state_.store(LiveState::kError, std::memory_order_release);
    return;
  }

  switch (config_.backend) {
    case LiveBackend::kOffline:
      state_.store(LiveState::kOffline, std::memory_order_release);
      return;
    case LiveBackend::kLan:
      session_directory_ = std::make_shared<LanSessionDirectory>(config_.lan_discovery_port);
      break;
    case LiveBackend::kCommunity: {
      if (!config_.community_backend_factory) {
        REXSYS_ERROR(
            "Community multiplayer requires an injected service backend; refusing to "
            "pretend Xbox Live is available for {}",
            config_.community_url);
        state_.store(LiveState::kError, std::memory_order_release);
        return;
      }
      auto services = config_.community_backend_factory(config_, identity_);
      session_directory_ = std::move(services.session_directory);
      peer_transport_ = std::move(services.peer_transport);
      break;
    }
  }

  const bool transport_ready =
      config_.backend != LiveBackend::kCommunity || (peer_transport_ && peer_transport_->ready());
  if (session_directory_ && session_directory_->ready() && transport_ready) {
    state_.store(LiveState::kAvailable, std::memory_order_release);
    REXSYS_INFO("LibertyRecomp multiplayer services available as {} ({:016X})",
                identity_.player_name, identity_.xuid);
  } else {
    const std::string error =
        session_directory_ && !session_directory_->ready() ? session_directory_->last_error()
        : peer_transport_ && !peer_transport_->ready()     ? peer_transport_->last_error()
                                                           : "missing backend service";
    REXSYS_ERROR("Unable to initialize multiplayer backend: {}", error);
    state_.store(LiveState::kError, std::memory_order_release);
  }
}

LiveCompatibilityRuntime::~LiveCompatibilityRuntime() = default;

LiveState LiveCompatibilityRuntime::state() const {
  const LiveState current = state_.load(std::memory_order_acquire);
  if (current == LiveState::kAvailable && (!session_directory_ || !session_directory_->ready())) {
    return LiveState::kError;
  }
  if (current == LiveState::kAvailable && config_.backend == LiveBackend::kCommunity &&
      (!peer_transport_ || !peer_transport_->ready())) {
    return LiveState::kError;
  }
  return current;
}

void LiveCompatibilityRuntime::ObserveBoundPort(uint16_t port) {
  if (port) {
    online_port_.store(port, std::memory_order_release);
  }
}

bool LiveCompatibilityRuntime::SetUserContext(uint32_t id, uint32_t value) {
  std::lock_guard lock(user_data_mutex_);
  if (!user_contexts_.contains(id) && user_contexts_.size() >= kMaximumContexts) {
    return false;
  }
  user_contexts_[id] = value;
  return true;
}

std::optional<uint32_t> LiveCompatibilityRuntime::GetUserContext(uint32_t id) const {
  std::lock_guard lock(user_data_mutex_);
  const auto context = user_contexts_.find(id);
  return context == user_contexts_.end() ? std::nullopt : std::optional<uint32_t>(context->second);
}

std::vector<SessionContext> LiveCompatibilityRuntime::user_contexts() const {
  std::lock_guard lock(user_data_mutex_);
  std::vector<SessionContext> result;
  result.reserve(user_contexts_.size());
  for (const auto& [id, value] : user_contexts_) {
    result.push_back({.id = id, .value = value});
  }
  return result;
}

bool LiveCompatibilityRuntime::SetUserProperty(uint32_t id, std::span<const uint8_t> value) {
  std::lock_guard lock(user_data_mutex_);
  if (value.size() > kMaximumPropertySize ||
      (!user_properties_.contains(id) && user_properties_.size() >= kMaximumProperties)) {
    return false;
  }
  user_properties_[id] = std::vector<uint8_t>(value.begin(), value.end());
  return true;
}

std::vector<SessionProperty> LiveCompatibilityRuntime::user_properties() const {
  std::lock_guard lock(user_data_mutex_);
  std::vector<SessionProperty> result;
  result.reserve(user_properties_.size());
  for (const auto& [id, value] : user_properties_) {
    result.push_back({.id = id, .value = value});
  }
  return result;
}

uint64_t LiveCompatibilityRuntime::GenerateSessionId() {
  uint64_t value = 0;
  do {
    value = RandomU64();
    value &= 0x00FFFFFFFFFFFFFFULL;
    value |= 0xAE00000000000000ULL;
  } while (!value);
  return value;
}

uint64_t LiveCompatibilityRuntime::GenerateNonce() {
  uint64_t value = 0;
  do {
    value = RandomU64();
  } while (!value);
  return value;
}

void LiveCompatibilityRuntime::GenerateExchangeKey(std::span<uint8_t, 16> key) {
  FillRandom(key);
}

void LiveCompatibilityRuntime::FillRandomBytes(std::span<uint8_t> output) {
  FillRandom(output);
}

bool LiveCompatibilityRuntime::IsPrivilegeAllowed(uint32_t privilege) const {
  if (!available()) {
    return false;
  }
  switch (privilege) {
    case 189:  // Sessions.
    case 251:  // GTA IV requests this legacy privilege.
    case 252:  // Communications.
    case 254:  // Multiplayer sessions.
      return true;
    default:
      return false;
  }
}

void LiveCompatibilityRuntime::RegisterRoute(uint32_t ipv4, const SessionRecord& session) {
  {
    std::lock_guard lock(route_mutex_);
    routes_[ipv4] = session;
  }
  if (peer_transport_) {
    peer_transport_->RegisterRoute(ipv4, session);
  }
}

std::optional<SessionRecord> LiveCompatibilityRuntime::FindRoute(uint32_t ipv4) const {
  std::lock_guard lock(route_mutex_);
  auto it = routes_.find(ipv4);
  return it == routes_.end() ? std::nullopt : std::optional(it->second);
}

void LiveCompatibilityRuntime::UnregisterRoute(uint32_t ipv4) {
  {
    std::lock_guard lock(route_mutex_);
    routes_.erase(ipv4);
  }
  if (peer_transport_) {
    peer_transport_->UnregisterRoute(ipv4);
  }
}

void LiveCompatibilityRuntime::RegisterKey(uint64_t session_id, std::span<const uint8_t, 16> key) {
  std::lock_guard lock(route_mutex_);
  std::copy(key.begin(), key.end(), registered_keys_[session_id].begin());
}

bool LiveCompatibilityRuntime::IsKeyRegistered(uint64_t session_id) const {
  std::lock_guard lock(route_mutex_);
  return registered_keys_.contains(session_id);
}

void LiveCompatibilityRuntime::UnregisterKey(uint64_t session_id) {
  std::lock_guard lock(route_mutex_);
  registered_keys_.erase(session_id);
}

bool LiveCompatibilityRuntime::SendPeerDatagram(uint32_t destination_ipv4,
                                                uint16_t destination_port, uint16_t source_port,
                                                std::span<const uint8_t> payload) {
  return peer_transport_ && FindRoute(destination_ipv4) &&
         peer_transport_->Send(destination_ipv4, destination_port, source_port, payload);
}

bool LiveCompatibilityRuntime::HasPendingPeerDatagram(uint16_t local_port) {
  return peer_transport_ && peer_transport_->HasPending(local_port);
}

std::optional<PeerDatagram> LiveCompatibilityRuntime::ReceivePeerDatagram(
    uint16_t local_port, uint32_t maximum_payload_size) {
  return peer_transport_ ? peer_transport_->Receive(local_port, maximum_payload_size)
                         : std::nullopt;
}

bool LiveCompatibilityRuntime::LoadOrCreateIdentity(const std::filesystem::path& user_data_root) {
  std::error_code error;
  std::filesystem::create_directories(user_data_root, error);
  if (error) {
    REXSYS_ERROR("Unable to create multiplayer identity directory {}: {}", user_data_root.string(),
                 error.message());
    return false;
  }

  const auto identity_path = user_data_root / "liberty_live_identity.bin";
  IdentityDisk disk{};
  {
    std::ifstream input(identity_path, std::ios::binary);
    if (input) {
      input.read(reinterpret_cast<char*>(&disk), sizeof(disk));
      if (input.gcount() == static_cast<std::streamsize>(sizeof(disk)) &&
          disk.magic == kIdentityMagic && disk.version == kIdentityVersion && disk.xuid &&
          disk.machine_id) {
        identity_.xuid = disk.xuid;
        identity_.machine_id = disk.machine_id;
        identity_.ethernet_address = disk.ethernet_address;
        identity_.install_secret = disk.install_secret;
        const auto name_end =
            std::find(disk.player_name.begin(), disk.player_name.end(), static_cast<char>(0));
        identity_.player_name = config_.player_name.empty()
                                    ? std::string(disk.player_name.begin(), name_end)
                                    : config_.player_name;
        RestrictIdentityPermissions(identity_path);
        return true;
      }
    }
  }

  disk.magic = kIdentityMagic;
  disk.version = kIdentityVersion;
  do {
    disk.xuid = RandomU64() & ~0x00C0000000000000ULL;
  } while (!disk.xuid);
  do {
    disk.machine_id = RandomU64();
  } while (!disk.machine_id);
  FillRandom(disk.ethernet_address);
  disk.ethernet_address.front() =
      static_cast<uint8_t>((disk.ethernet_address.front() & 0xFCU) | 0x02U);
  FillRandom(disk.install_secret);
  const std::string player_name = config_.player_name.empty() ? "Player" : config_.player_name;
  std::memcpy(disk.player_name.data(), player_name.data(),
              std::min(player_name.size(), disk.player_name.size() - 1));

  const auto temporary_path = identity_path.string() + ".tmp";
  {
    std::ofstream output(temporary_path, std::ios::binary | std::ios::trunc);
    if (!output) {
      return false;
    }
    output.write(reinterpret_cast<const char*>(&disk), sizeof(disk));
    output.flush();
    if (!output) {
      return false;
    }
  }
  std::filesystem::rename(temporary_path, identity_path, error);
  if (error) {
    std::filesystem::remove(identity_path, error);
    error.clear();
    std::filesystem::rename(temporary_path, identity_path, error);
  }
  if (error) {
    REXSYS_ERROR("Unable to persist multiplayer identity {}: {}", identity_path.string(),
                 error.message());
    return false;
  }
  RestrictIdentityPermissions(identity_path);

  identity_.xuid = disk.xuid;
  identity_.machine_id = disk.machine_id;
  identity_.ethernet_address = disk.ethernet_address;
  identity_.install_secret = disk.install_secret;
  identity_.player_name = player_name;
  return true;
}

uint32_t LiveCompatibilityRuntime::DiscoverLocalIpv4() {
  char hostname[256]{};
  if (gethostname(hostname, sizeof(hostname)) != 0) {
    return htonl(INADDR_LOOPBACK);
  }

  addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  addrinfo* result = nullptr;
  if (getaddrinfo(hostname, nullptr, &hints, &result) != 0) {
    return htonl(INADDR_LOOPBACK);
  }

  uint32_t address = htonl(INADDR_LOOPBACK);
  for (auto* current = result; current; current = current->ai_next) {
    const auto* ipv4 = reinterpret_cast<const sockaddr_in*>(current->ai_addr);
    if (ipv4->sin_addr.s_addr != htonl(INADDR_LOOPBACK)) {
      address = ipv4->sin_addr.s_addr;
      break;
    }
  }
  freeaddrinfo(result);
  return address;
}

uint64_t LiveCompatibilityRuntime::RandomU64() {
  std::random_device random;
  std::array<uint8_t, sizeof(uint64_t)> bytes{};
  for (auto& byte : bytes) {
    byte = static_cast<uint8_t>(random());
  }
  uint64_t value = 0;
  std::memcpy(&value, bytes.data(), sizeof(value));
  return value;
}

void LiveCompatibilityRuntime::FillRandom(std::span<uint8_t> output) {
  std::random_device random;
  for (auto& byte : output) {
    byte = static_cast<uint8_t>(random());
  }
}

}  // namespace rex::system::xam
