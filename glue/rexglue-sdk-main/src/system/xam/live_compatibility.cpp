/**
 ******************************************************************************
 * @file        live_compatibility.cpp
 * @brief       Community/LAN Xbox Live compatibility runtime.
 ******************************************************************************
 */

#include <rex/system/xam/live_compatibility.h>

#include <algorithm>
#include <cstdlib>
#include <array>
#include <cmath>
#include <cstring>
#include <deque>
#include <fstream>
#include <limits>
#include <random>
#include <ranges>
#include <system_error>
#include <unordered_set>

#include <rex/logging.h>
#include <rex/net/socket.h>
#include <rex/platform.h>
#include <rex/system/xam/social_cache.h>
#include "live_compatibility_internal.h"

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
constexpr uint8_t kLanProtocolVersion = 5;
constexpr uint8_t kOperationQuery = 1;
constexpr uint8_t kOperationAdvertise = 2;
constexpr uint8_t kOperationDelete = 3;
constexpr uint8_t kOperationJoin = 4;
constexpr uint8_t kOperationLeave = 5;
constexpr uint8_t kOperationMigrate = 6;
constexpr size_t kMaximumContexts = 64;
constexpr size_t kMaximumProperties = 64;
constexpr size_t kMaximumPropertySize = 512;
constexpr std::chrono::milliseconds kSearchWindow{350};
constexpr std::chrono::seconds kMutationWindow{2};
constexpr std::chrono::seconds kAdvertisementInterval{1};
constexpr std::chrono::seconds kRecordLifetime{5};
constexpr size_t kMaximumPendingReassemblies = 128;
constexpr const char* kMulticastAddress = "239.255.42.99";
constexpr uint32_t kIdentityVersion = 1;
constexpr std::array<uint8_t, 8> kIdentityMagic = {'L', 'B', 'R', 'L', 'I', 'V', 'E', 0};

bool HasMatchingStatPayload(const StatColumn& column) {
  switch (column.type) {
    case StatValueType::kUnset:
      // Unset is a read-side placeholder, never a writable stat payload.
      return false;
    case StatValueType::kInt32:
      return std::holds_alternative<int32_t>(column.value);
    case StatValueType::kInt64:
      return std::holds_alternative<int64_t>(column.value);
    case StatValueType::kDouble:
      return std::holds_alternative<double>(column.value) &&
             std::isfinite(std::get<double>(column.value));
    case StatValueType::kUnicode:
      return std::holds_alternative<std::u16string>(column.value) &&
             std::get<std::u16string>(column.value).size() <= 256;
    case StatValueType::kBinary:
      return std::holds_alternative<std::vector<uint8_t>>(column.value) &&
             std::get<std::vector<uint8_t>>(column.value).size() <= 512;
  }
  return false;
}

uint64_t InitialLanMessageId() {
  std::random_device random;
  uint64_t value = (static_cast<uint64_t>(random()) << 32) | random();
  return value ? value : 1;
}

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

class InMemoryOnlineServices final : public ISocialService,
                                     public IStatsService,
                                     public IVoicePacketTransport {
 public:
  InMemoryOnlineServices(uint64_t local_xuid,
                         std::shared_ptr<ISessionDirectory> session_directory)
      : local_xuid_(local_xuid), session_directory_(std::move(session_directory)) {}

  bool ready() const override { return true; }
  std::string last_error() const override { return {}; }

  std::vector<bool> AreFriends(std::span<const uint64_t> xuids) override {
    if (xuids.empty() || xuids.size() > 100 ||
        std::ranges::any_of(xuids, [](uint64_t xuid) { return xuid == 0; })) {
      return {};
    }
    std::lock_guard lock(mutex_);
    std::vector<bool> result;
    result.reserve(xuids.size());
    for (uint64_t xuid : xuids) {
      result.push_back(std::ranges::any_of(
          friends_, [xuid](const FriendRecord& record) { return record.xuid == xuid; }));
    }
    return result;
  }

  FriendEnumerationResult EnumerateFriends(uint32_t maximum_results) override {
    if (!maximum_results || maximum_results > 100) {
      return {.status = SocialServiceStatus::kInvalidRequest};
    }
    std::lock_guard lock(mutex_);
    const size_t count = std::min<size_t>(friends_.size(), maximum_results);
    return {.status = SocialServiceStatus::kSuccess,
            .friends = {friends_.begin(), friends_.begin() + count}};
  }

  bool SendInvitations(uint64_t session_id, std::span<const uint64_t> recipients,
                       std::span<const uint8_t> custom_data) override {
    std::unordered_set<uint64_t> unique_recipients;
    if (!session_id || recipients.empty() || recipients.size() > 32 || custom_data.size() > 512 ||
        std::ranges::any_of(recipients, [&unique_recipients](uint64_t xuid) {
          return !xuid || !unique_recipients.insert(xuid).second;
        })) {
      return false;
    }
    auto directory = session_directory_.lock();
    auto session = directory ? directory->Get(session_id) : std::nullopt;
    if (!session) return false;
    std::function<void()> notification;
    std::unique_lock lock(mutex_);
    for (uint64_t recipient : recipients) {
      if (recipient == local_xuid_ && !accepted_invitation_) {
        InvitationRecord invitation{
            .id = "lan-loopback",
            .sender_xuid = local_xuid_,
            .recipient_xuid = local_xuid_,
            .session_id = session_id,
            .custom_data = {custom_data.begin(), custom_data.end()},
            .session = *session,
            .revision = 1,
        };
        pending_invitation_ = invitation;
        accepted_invitation_ = std::move(invitation);
        notification = invite_notification_handler_;
      }
    }
    lock.unlock();
    if (notification) notification();
    return true;
  }

  std::optional<InvitationRecord> AcceptInvitation(uint64_t session_id,
                                                    uint64_t sender_xuid) override {
    std::lock_guard lock(mutex_);
    if (!pending_invitation_ || pending_invitation_->sender_xuid != sender_xuid ||
        (pending_invitation_->session_id != session_id &&
         (!pending_invitation_->session ||
          pending_invitation_->session->previous_session_id != session_id))) {
      return std::nullopt;
    }
    auto invitation = std::move(pending_invitation_);
    pending_invitation_.reset();
    return invitation;
  }

  std::optional<InvitationRecord> AcceptedInvitation() override {
    std::lock_guard lock(mutex_);
    auto invitation = std::move(accepted_invitation_);
    accepted_invitation_.reset();
    return invitation;
  }

  bool HasAcceptedInvitation() const override {
    std::lock_guard lock(mutex_);
    return accepted_invitation_.has_value();
  }

  CachedMuteState QueryMute(uint64_t xuid) const override {
    return xuid ? CachedMuteState::kNotMuted : CachedMuteState::kMissing;
  }

  void SetInviteNotificationHandler(std::function<void()> handler) override {
    bool notify = false;
    {
      std::lock_guard lock(mutex_);
      invite_notification_handler_ = shutdown_ ? std::function<void()>{}
                                               : std::move(handler);
      notify = accepted_invitation_.has_value() &&
               static_cast<bool>(invite_notification_handler_);
      handler = invite_notification_handler_;
    }
    if (notify) handler();
  }

  void Shutdown() override {
    std::lock_guard lock(mutex_);
    shutdown_ = true;
    invite_notification_handler_ = {};
  }

  bool ResetView(uint32_t view_id) override {
    if (!view_id) return false;
    std::lock_guard lock(mutex_);
    stats_.erase(view_id);
    return true;
  }

  bool ModifySkill(uint64_t session_id, std::span<const uint64_t> xuids) override {
    return session_id != 0 && !xuids.empty() && xuids.size() <= kMaximumSessionMembers &&
           std::ranges::none_of(xuids, [](uint64_t xuid) { return xuid == 0; });
  }

  bool Write(uint64_t session_id, uint64_t xuid, std::span<const StatView> views) override {
    if (!session_id || !xuid || views.empty() || views.size() > 16) return false;
    std::lock_guard lock(mutex_);
    std::vector<uint32_t> view_ids;
    view_ids.reserve(views.size());
    for (const auto& view : views) {
      if (!view.id || view.rows.size() != 1 || view.rows.front().columns.empty() ||
          view.rows.front().columns.size() > 64 ||
          std::ranges::find(view_ids, view.id) != view_ids.end()) {
        return false;
      }
      view_ids.push_back(view.id);
      std::vector<uint32_t> column_ids;
      column_ids.reserve(view.rows.front().columns.size());
      for (const auto& column : view.rows.front().columns) {
        if (!column.id || !HasMatchingStatPayload(column) ||
            std::ranges::find(column_ids, column.id) != column_ids.end()) {
          return false;
        }
        column_ids.push_back(column.id);
      }
    }
    for (const auto& view : views) {
      StatRow row = view.rows.front();
      row.xuid = xuid;
      stats_[view.id][xuid] = std::move(row);
    }
    return true;
  }

  bool Flush(uint64_t session_id) override { return session_id != 0; }

  std::vector<StatView> Read(std::span<const uint64_t> xuids,
                             std::span<const uint32_t> view_ids,
                             std::span<const uint32_t> attribute_ids) override {
    std::unordered_set<uint32_t> unique_views;
    std::unordered_set<uint32_t> unique_attributes;
    if (xuids.empty() || xuids.size() > 100 || view_ids.empty() || view_ids.size() > 16 ||
        attribute_ids.size() > 64 ||
        std::ranges::any_of(xuids, [](uint64_t xuid) { return xuid == 0; }) ||
        std::ranges::any_of(view_ids, [&unique_views](uint32_t id) {
          return !id || !unique_views.insert(id).second;
        }) ||
        std::ranges::any_of(attribute_ids, [&unique_attributes](uint32_t id) {
          return !id || id > std::numeric_limits<uint16_t>::max() ||
                 !unique_attributes.insert(id).second;
        })) {
      return {};
    }
    std::lock_guard lock(mutex_);
    std::vector<StatView> result;
    for (uint32_t view_id : view_ids) {
      StatView view{.id = view_id};
      const auto stored_view = stats_.find(view_id);
      for (uint64_t xuid : xuids) {
        if (stored_view == stats_.end()) {
          view.rows.push_back({.xuid = xuid, .player_name = "Player"});
          continue;
        }
        const auto stored_row = stored_view->second.find(xuid);
        if (stored_row == stored_view->second.end()) {
          view.rows.push_back({.xuid = xuid, .player_name = "Player"});
          continue;
        }
        StatRow row = stored_row->second;
        row.columns.clear();
        for (uint32_t attribute_id : attribute_ids) {
          const auto column = std::ranges::find_if(
              stored_row->second.columns, [attribute_id](const StatColumn& item) {
                return item.id_kind == StatColumnIdKind::kAttribute &&
                       item.id == attribute_id;
              });
          if (column != stored_row->second.columns.end()) row.columns.push_back(*column);
        }
        row.rank = 1;
        for (const auto& [candidate_xuid, candidate] : stored_view->second) {
          if (candidate.rating > stored_row->second.rating ||
              (candidate.rating == stored_row->second.rating && candidate_xuid < xuid)) {
            ++row.rank;
          }
        }
        view.rows.push_back(std::move(row));
      }
      result.push_back(std::move(view));
    }
    return result;
  }

  LeaderboardResult Leaderboard(uint32_t view_id,
                                std::span<const uint32_t> attribute_ids,
                                uint32_t offset, uint32_t maximum_results,
                                bool friends_only) override {
    std::lock_guard lock(mutex_);
    LeaderboardResult result{.status = StatsServiceStatus::kSuccess};
    std::unordered_set<uint32_t> unique_attributes;
    if (!view_id || attribute_ids.size() > 64 || !maximum_results || maximum_results > 100 ||
        std::ranges::any_of(attribute_ids, [&unique_attributes](uint32_t id) {
          return !id || id > std::numeric_limits<uint16_t>::max() ||
                 !unique_attributes.insert(id).second;
        })) {
      result.status = StatsServiceStatus::kInvalidRequest;
      return result;
    }
    const auto stored_view = stats_.find(view_id);
    if (stored_view == stats_.end()) return result;
    for (const auto& [xuid, stored] : stored_view->second) {
      if (friends_only && xuid != local_xuid_ &&
          !std::ranges::any_of(friends_, [xuid](const FriendRecord& record) {
            return record.xuid == xuid;
          })) {
        continue;
      }
      StatRow row = stored;
      row.columns.clear();
      for (uint32_t attribute_id : attribute_ids) {
        const auto column = std::ranges::find_if(
            stored.columns, [attribute_id](const StatColumn& item) {
              return item.id_kind == StatColumnIdKind::kAttribute &&
                     item.id == attribute_id;
            });
        if (column == stored.columns.end()) continue;
        if (!HasMatchingStatPayload(*column)) {
          result.status = StatsServiceStatus::kInvalidResponse;
          result.page = {};
          return result;
        }
        row.columns.push_back(*column);
      }
      result.page.rows.push_back(std::move(row));
    }
    std::ranges::sort(result.page.rows, [](const StatRow& left, const StatRow& right) {
      if (left.rating != right.rating) return left.rating > right.rating;
      return left.xuid < right.xuid;
    });
    result.page.total = static_cast<uint32_t>(result.page.rows.size());
    for (size_t index = 0; index < result.page.rows.size(); ++index) {
      result.page.rows[index].rank = static_cast<uint32_t>(index) + 1;
    }
    if (offset >= result.page.rows.size()) {
      result.page.rows.clear();
      return result;
    }
    result.page.rows.erase(result.page.rows.begin(), result.page.rows.begin() + offset);
    if (result.page.rows.size() > maximum_results) result.page.rows.resize(maximum_results);
    return result;
  }

  bool Configure(const VoiceRoute& route) override {
    std::unordered_set<uint64_t> unique_targets;
    std::unordered_set<uint64_t> unique_muted;
    if (!route.session_id || route.target_xuids.size() > kMaximumSessionMembers ||
        route.muted_xuids.size() > kMaximumSessionMembers ||
        std::ranges::any_of(route.target_xuids, [&unique_targets](uint64_t xuid) {
          return !xuid || !unique_targets.insert(xuid).second;
        }) ||
        std::ranges::any_of(route.muted_xuids, [&unique_muted](uint64_t xuid) {
          return !xuid || !unique_muted.insert(xuid).second;
        })) {
      return false;
    }
    std::lock_guard lock(mutex_);
    voice_route_ = route;
    return true;
  }

  bool Send(uint32_t sequence, std::span<const uint8_t> payload) override {
    if (payload.empty() || payload.size() > 4096) return false;
    std::lock_guard lock(mutex_);
    if (!voice_route_) return false;
    const bool muted = std::ranges::find(voice_route_->muted_xuids, local_xuid_) !=
                       voice_route_->muted_xuids.end();
    const bool targeted = voice_route_->channel == VoiceChannel::kAll ||
                          std::ranges::find(voice_route_->target_xuids, local_xuid_) !=
                              voice_route_->target_xuids.end();
    if (!muted && targeted) {
      voice_packets_.push_back({.source_xuid = local_xuid_,
                                .session_id = voice_route_->session_id,
                                .sequence = sequence,
                                .payload = {payload.begin(), payload.end()}});
    }
    return true;
  }

  std::optional<VoicePacket> Receive(uint32_t maximum_payload_size) override {
    std::lock_guard lock(mutex_);
    while (!voice_packets_.empty()) {
      VoicePacket packet = std::move(voice_packets_.front());
      voice_packets_.pop_front();
      if (packet.payload.size() <= maximum_payload_size) return packet;
    }
    return std::nullopt;
  }

  void Close() override {
    std::lock_guard lock(mutex_);
    voice_route_.reset();
    voice_packets_.clear();
  }

 private:
  uint64_t local_xuid_;
  std::weak_ptr<ISessionDirectory> session_directory_;
  mutable std::mutex mutex_;
  std::vector<FriendRecord> friends_;
  std::optional<InvitationRecord> pending_invitation_;
  std::optional<InvitationRecord> accepted_invitation_;
  std::function<void()> invite_notification_handler_;
  bool shutdown_ = false;
  std::unordered_map<uint32_t, std::unordered_map<uint64_t, StatRow>> stats_;
  std::optional<VoiceRoute> voice_route_;
  std::deque<VoicePacket> voice_packets_;
};

class InMemoryQosService final : public IQosService {
 public:
  explicit InMemoryQosService(std::shared_ptr<ISessionDirectory> directory)
      : directory_(std::move(directory)) {}

  bool ready() const override { return !directory_.expired(); }
  std::string last_error() const override {
    return ready() ? std::string{} : std::string{"session directory is unavailable"};
  }

  bool UpdateListener(uint64_t session_id,
                      std::span<const uint8_t, 16> exchange_key,
                      const QosListenerUpdate& update) override {
    if (!update.enabled && !update.title_data && !update.bits_per_second) return false;
    auto directory = directory_.lock();
    auto session = directory ? directory->Get(session_id) : std::nullopt;
    if (!session || !std::ranges::equal(session->exchange_key, exchange_key)) return false;
    if (update.enabled) session->qos_listener_enabled = *update.enabled;
    if (update.title_data) session->qos_title_data = *update.title_data;
    return directory->Modify(*session);
  }

  bool Close(uint64_t session_id, std::span<const uint8_t, 16> exchange_key) override {
    auto directory = directory_.lock();
    auto session = directory ? directory->Get(session_id) : std::nullopt;
    if (!session || !std::ranges::equal(session->exchange_key, exchange_key)) return false;
    session->qos_listener_enabled = false;
    session->qos_title_data.reset();
    return directory->Modify(*session);
  }

  std::vector<QosResult> Lookup(std::span<const QosTarget> targets) override {
    std::vector<QosResult> results(targets.size());
    if (targets.empty() || targets.size() > kMaximumQosTargets) return {};
    auto directory = directory_.lock();
    if (!directory) return {};
    for (size_t index = 0; index < targets.size(); ++index) {
      const auto session = directory->Get(targets[index].session_id);
      if (!session || !session->qos_listener_enabled ||
          session->exchange_key != targets[index].exchange_key || !session->host_ipv4) {
        continue;
      }
      results[index].reachable = true;
      results[index].title_data = session->qos_title_data;
      results[index].probes_xmit = 1;
      results[index].probes_recv = 1;
    }
    return results;
  }

 private:
  std::weak_ptr<ISessionDirectory> directory_;
};

}  // namespace

LiveBackendServices detail::CreateInMemoryOnlineServices(
    uint64_t local_xuid, std::shared_ptr<ISessionDirectory> session_directory) {
  auto services = std::make_shared<InMemoryOnlineServices>(local_xuid, session_directory);
  auto qos = std::make_shared<InMemoryQosService>(std::move(session_directory));
  return {
      .social_service = services,
      .stats_service = services,
      .voice_transport = services,
      .qos_service = std::move(qos),
  };
}

namespace {

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
  if (!session.roster_complete) {
    // A discovery record carries authoritative open-slot counts but no member
    // identities. It is valid only as an entirely redacted roster; partial
    // identity lists would make occupancy ambiguous.
    return session.members.empty() &&
           session.declared_public_members <= session.max_public_slots &&
           session.declared_private_members <= session.max_private_slots &&
           session.declared_public_members + session.open_public_slots ==
               session.max_public_slots &&
           session.declared_private_members + session.open_private_slots ==
               session.max_private_slots;
  }
  if (session.declared_public_members || session.declared_private_members) {
    return false;
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
    // Session creation publishes a provisional host before GTA supplies the
    // per-user private-slot bit through JoinLocal. Reconcile that later title
    // classification without discarding the directory-assigned peer slot.
    if (existing->private_slot != member.private_slot) {
      if (member.private_slot && session.open_private_slots) {
        ++session.open_public_slots;
        --session.open_private_slots;
        existing->private_slot = true;
      } else if (!member.private_slot && session.open_public_slots) {
        ++session.open_private_slots;
        --session.open_public_slots;
        existing->private_slot = false;
      }
    }
    if (member.machine_id) existing->machine_id = member.machine_id;
    if (member.virtual_ipv4) existing->virtual_ipv4 = member.virtual_ipv4;
    if (member.online_port) existing->online_port = member.online_port;
    if (!member.peer_id.empty()) existing->peer_id = std::move(member.peer_id);
    if (member.multiplayer_peer_id < kMaximumSessionMembers) {
      existing->multiplayer_peer_id = member.multiplayer_peer_id;
    }
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
  explicit PacketWriter(size_t maximum_size) : maximum_size_(maximum_size) {}

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
    if (data_.size() > maximum_size_ || bytes.size() > maximum_size_ - data_.size()) {
      valid_ = false;
      return false;
    }
    data_.insert(data_.end(), bytes.begin(), bytes.end());
    return true;
  }

  bool valid() const { return valid_; }
  const std::vector<uint8_t>& data() const { return data_; }

 private:
  size_t maximum_size_;
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
  size_t remaining() const { return offset_ <= data_.size() ? data_.size() - offset_ : 0; }
  bool at_end() const { return valid_ && offset_ == data_.size(); }

 private:
  std::span<const uint8_t> data_;
  size_t offset_ = 0;
  bool valid_ = true;
};

std::vector<uint8_t> SerializeSessionRecord(const SessionRecord& session) {
  PacketWriter writer(detail::kLanMaximumRecordSize);
  if (!session.roster_complete ||
      session.contexts.size() > kMaximumContexts ||
      session.properties.size() > kMaximumProperties ||
      session.members.size() > kMaximumSessionMembers) {
    return {};
  }

  writer.PutU32(session.title_id);
  writer.PutU32(session.media_id);
  writer.PutU32(session.title_version);
  writer.PutU32(session.protocol_version);
  writer.PutU64(session.session_id);
  writer.PutU64(session.previous_session_id);
  writer.PutBytes(session.exchange_key);
  writer.PutU64(session.nonce);
  writer.PutU32(session.flags);
  writer.PutU32(static_cast<uint32_t>(session.lifecycle_state));
  writer.PutU32(session.max_public_slots);
  writer.PutU32(session.max_private_slots);
  writer.PutU32(session.open_public_slots);
  writer.PutU32(session.open_private_slots);
  writer.PutU64(session.host_xuid);
  writer.PutU64(session.host_machine_id);
  writer.PutBytes(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(&session.host_ipv4),
                                           sizeof(session.host_ipv4)));
  writer.PutU16(session.host_port);
  writer.PutBytes(session.host_ethernet_address);
  const uint8_t qos_listener_state =
      (session.qos_title_data ? 1 : 0) | (session.qos_listener_enabled ? 2 : 0);
  writer.PutU8(qos_listener_state);
  if (session.qos_title_data) writer.PutBytes(*session.qos_title_data);
  writer.PutU16(static_cast<uint16_t>(session.contexts.size()));
  writer.PutU16(static_cast<uint16_t>(session.properties.size()));
  writer.PutU16(static_cast<uint16_t>(session.members.size()));

  for (const auto& context : session.contexts) {
    writer.PutU32(context.id);
    writer.PutU32(context.value);
  }
  for (const auto& property : session.properties) {
    if (property.value.size() > kMaximumPropertySize) {
      return {};
    }
    writer.PutU32(property.id);
    writer.PutU16(static_cast<uint16_t>(property.value.size()));
    writer.PutBytes(property.value);
  }
  for (const auto& member : session.members) {
    writer.PutU64(member.xuid);
    writer.PutU8(member.private_slot ? 1 : 0);
  }
  return writer.valid() ? writer.data() : std::vector<uint8_t>{};
}

std::optional<SessionRecord> DeserializeSessionRecord(uint8_t operation,
                                                      std::span<const uint8_t> bytes) {
  PacketReader reader(bytes);
  SessionRecord session;
  if (operation == kOperationQuery) {
    return bytes.empty() ? std::optional(session) : std::nullopt;
  }

  uint16_t context_count = 0;
  uint16_t property_count = 0;
  uint16_t member_count = 0;
  uint32_t lifecycle_state = 0;
  uint8_t qos_listener_state = 0;
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
      !reader.GetU8(qos_listener_state) || qos_listener_state > 3 ||
      ((qos_listener_state & 1) &&
       !reader.GetBytes(session.qos_title_data.emplace())) ||
      !reader.GetU16(context_count) || !reader.GetU16(property_count) ||
      !reader.GetU16(member_count) || context_count > kMaximumContexts ||
      property_count > kMaximumProperties || member_count > kMaximumSessionMembers) {
    return std::nullopt;
  }
  if (lifecycle_state > static_cast<uint32_t>(SessionLifecycleState::kDeleted)) {
    return std::nullopt;
  }
  session.lifecycle_state = static_cast<SessionLifecycleState>(lifecycle_state);
  session.qos_listener_enabled = (qos_listener_state & 2) != 0;

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
  return reader.at_end() ? std::optional(std::move(session)) : std::nullopt;
}

std::vector<SessionRecord> SearchRecords(
    const std::unordered_map<uint64_t, SessionRecord>& records, uint32_t title_id,
    uint32_t media_id, uint32_t title_version, uint32_t protocol_version,
    uint32_t /*procedure_index*/,
    std::span<const SessionContext> contexts, std::span<const SessionProperty> properties,
    uint32_t maximum_results,
    const std::unordered_map<uint64_t, SessionRecord>* exclusions = nullptr) {
  // XSessionSearchEx's procedure selects a title-side query schema. XSessionCreate
  // has no corresponding field, so advertised records are matched by the schema's
  // exact context/property inputs rather than a stored procedure value.
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

namespace detail {

namespace {

bool CompletePendingInvitation(const InvitationRecord& invitation) {
  return !invitation.id.empty() && invitation.sender_xuid && invitation.recipient_xuid &&
         invitation.session_id && invitation.revision > 0 && invitation.session &&
         invitation.session->session_id == invitation.session_id;
}

bool ContainsSessionAlias(const InvitationAcceptAttempt& attempt, uint64_t session_id) {
  return std::ranges::find(attempt.session_aliases, session_id) !=
         attempt.session_aliases.end();
}

void RememberSessionAlias(InvitationAcceptAttempt& attempt, uint64_t session_id) {
  if (session_id && !ContainsSessionAlias(attempt, session_id)) {
    attempt.session_aliases.push_back(session_id);
  }
}

}  // namespace

bool SocialCacheState::PendingDescriptorCurrent(const InvitationRecord& invitation) const {
  if (invitation.id.empty() || !invitation.sender_xuid || !invitation.recipient_xuid ||
      !invitation.session_id || invitation.revision <= 0) {
    return false;
  }
  std::lock_guard lock(mutex_);
  const auto found = pending_invitations_.find(invitation.id);
  if (found == pending_invitations_.end()) return false;
  const InvitationRecord& cached = found->second.attempt.invitation;
  return cached.sender_xuid == invitation.sender_xuid &&
         cached.recipient_xuid == invitation.recipient_xuid &&
         cached.session_id == invitation.session_id && cached.revision == invitation.revision &&
         cached.custom_data == invitation.custom_data && cached.session &&
         cached.session->session_id == cached.session_id;
}

bool SocialCacheState::ObservePending(InvitationRecord invitation,
                                      std::string candidate_idempotency) {
  if (!CompletePendingInvitation(invitation) || candidate_idempotency.empty()) return false;
  std::lock_guard lock(mutex_);
  auto found = pending_invitations_.find(invitation.id);
  if (found == pending_invitations_.end()) {
    const bool already_presented = delivered_ids_.contains(invitation.id);
    PendingInvitation pending{
        .attempt = InvitationAcceptAttempt{
            .invitation = std::move(invitation),
            .idempotency = std::move(candidate_idempotency),
        },
        .presented = already_presented,
    };
    RememberSessionAlias(pending.attempt, pending.attempt.invitation.session_id);
    pending_invitations_.emplace(pending.attempt.invitation.id, std::move(pending));
    return true;
  }

  PendingInvitation& pending = found->second;
  const InvitationRecord& previous = pending.attempt.invitation;
  if (previous.sender_xuid != invitation.sender_xuid ||
      previous.recipient_xuid != invitation.recipient_xuid ||
      previous.custom_data != invitation.custom_data) {
    return false;
  }
  RememberSessionAlias(pending.attempt, previous.session_id);
  RememberSessionAlias(pending.attempt, invitation.session_id);
  if (previous.revision != invitation.revision) {
    pending.attempt.idempotency = std::move(candidate_idempotency);
  }
  pending.attempt.invitation = std::move(invitation);
  if (accepted_invitation_ && accepted_invitation_->id == pending.attempt.invitation.id) {
    accepted_invitation_ = pending.attempt.invitation;
  }
  return true;
}

void SocialCacheState::RetainPending(std::span<const std::string> invitation_ids) {
  const std::unordered_set<std::string> retained(invitation_ids.begin(), invitation_ids.end());
  std::lock_guard lock(mutex_);
  std::erase_if(pending_invitations_, [&](const auto& entry) {
    return !entry.second.accepting && !retained.contains(entry.first);
  });
}

bool SocialCacheState::PresentNextPending() {
  std::function<void()> notification;
  {
    std::lock_guard lock(mutex_);
    if (accepted_invitation_) return false;
    const auto pending = std::ranges::find_if(pending_invitations_, [](const auto& entry) {
      return !entry.second.presented && !entry.second.accepting;
    });
    if (pending == pending_invitations_.end()) return false;
    pending->second.presented = true;
    RememberDeliveredLocked(pending->first);
    accepted_invitation_ = pending->second.attempt.invitation;
    notification = invite_notification_handler_;
  }
  if (notification) notification();
  return true;
}

std::optional<InvitationAcceptAttempt> SocialCacheState::PrepareExplicitAcceptance(
    uint64_t session_id, uint64_t sender_xuid) {
  if (!session_id || !sender_xuid) return std::nullopt;
  std::lock_guard lock(mutex_);
  const auto pending = std::ranges::find_if(pending_invitations_, [&](const auto& entry) {
    return !entry.second.accepting &&
           entry.second.attempt.invitation.sender_xuid == sender_xuid &&
           ContainsSessionAlias(entry.second.attempt, session_id);
  });
  if (pending == pending_invitations_.end()) return std::nullopt;
  pending->second.accepting = true;
  return pending->second.attempt;
}

bool SocialCacheState::PublishExplicitAcceptance(
    const std::string& invitation_id, const InvitationRecord& accepted_invitation) {
  if (!CompletePendingInvitation(accepted_invitation) ||
      accepted_invitation.id != invitation_id) {
    return false;
  }
  std::lock_guard lock(mutex_);
  const auto found = pending_invitations_.find(invitation_id);
  if (found == pending_invitations_.end() || !found->second.accepting) return false;
  const InvitationAcceptAttempt& attempt = found->second.attempt;
  if (accepted_invitation.sender_xuid != attempt.invitation.sender_xuid ||
      accepted_invitation.recipient_xuid != attempt.invitation.recipient_xuid ||
      (!ContainsSessionAlias(attempt, accepted_invitation.session_id) &&
       !ContainsSessionAlias(attempt, accepted_invitation.session->previous_session_id))) {
    return false;
  }
  pending_invitations_.erase(found);
  return true;
}

void SocialCacheState::AbandonExplicitAcceptance(const std::string& invitation_id) {
  std::lock_guard lock(mutex_);
  const auto found = pending_invitations_.find(invitation_id);
  if (found != pending_invitations_.end()) found->second.accepting = false;
}

std::optional<InvitationRecord> SocialCacheState::ConsumeAcceptedInvitation() {
  std::lock_guard lock(mutex_);
  auto invitation = std::move(accepted_invitation_);
  accepted_invitation_.reset();
  return invitation;
}

bool SocialCacheState::HasAcceptedInvitation() const {
  std::lock_guard lock(mutex_);
  return accepted_invitation_.has_value();
}

void SocialCacheState::ReplaceRelationships(std::span<const FriendRecord> relationships) {
  std::unordered_map<uint64_t, bool> replacement;
  replacement.reserve(relationships.size());
  for (const auto& relationship : relationships) {
    if (relationship.xuid) replacement[relationship.xuid] = relationship.blocked;
  }
  std::lock_guard lock(mutex_);
  muted_relationships_ = std::move(replacement);
  relationship_cache_ready_ = true;
}

CachedMuteState SocialCacheState::QueryMute(uint64_t xuid) const {
  std::lock_guard lock(mutex_);
  if (!xuid || !relationship_cache_ready_) return CachedMuteState::kMissing;
  const auto found = muted_relationships_.find(xuid);
  return found != muted_relationships_.end() && found->second
             ? CachedMuteState::kMuted
             : CachedMuteState::kNotMuted;
}

void SocialCacheState::SetInviteNotificationHandler(std::function<void()> handler) {
  bool notify = false;
  {
    std::lock_guard lock(mutex_);
    invite_notification_handler_ = std::move(handler);
    notify = accepted_invitation_.has_value() &&
             static_cast<bool>(invite_notification_handler_);
    handler = invite_notification_handler_;
  }
  if (notify) handler();
}

size_t SocialCacheState::delivered_invitation_count() const {
  std::lock_guard lock(mutex_);
  return delivered_ids_.size();
}

size_t SocialCacheState::pending_invitation_count() const {
  std::lock_guard lock(mutex_);
  return pending_invitations_.size();
}

void SocialCacheState::RememberDeliveredLocked(const std::string& invitation_id) {
  if (!delivered_ids_.insert(invitation_id).second) return;
  delivered_order_.push_back(invitation_id);
  while (delivered_order_.size() > kCommunityDeliveredInviteHistoryLimit) {
    delivered_ids_.erase(delivered_order_.front());
    delivered_order_.pop_front();
  }
}

std::vector<uint8_t> SerializeLanSessionRecord(const SessionRecord& session) {
  return SerializeSessionRecord(session);
}

std::optional<SessionRecord> DeserializeLanSessionRecord(uint8_t operation,
                                                         std::span<const uint8_t> bytes) {
  if (operation < kOperationQuery || operation > kOperationMigrate) {
    return std::nullopt;
  }
  return DeserializeSessionRecord(operation, bytes);
}

std::vector<std::vector<uint8_t>> FrameLanPayload(uint8_t operation,
                                                  std::span<const uint8_t> payload,
                                                  uint64_t message_id) {
  if (!message_id || operation < kOperationQuery || operation > kOperationMigrate ||
      payload.size() > kLanMaximumRecordSize ||
      (operation == kOperationQuery && !payload.empty())) {
    return {};
  }

  const size_t fragment_count_size =
      payload.empty() ? 1 : (payload.size() + kLanMaximumFragmentPayload - 1) /
                                  kLanMaximumFragmentPayload;
  if (fragment_count_size > kLanMaximumFragmentCount) {
    return {};
  }
  const auto fragment_count = static_cast<uint16_t>(fragment_count_size);
  std::vector<std::vector<uint8_t>> frames;
  frames.reserve(fragment_count);
  for (uint16_t fragment_index = 0; fragment_index < fragment_count; ++fragment_index) {
    const size_t payload_offset =
        static_cast<size_t>(fragment_index) * kLanMaximumFragmentPayload;
    const size_t fragment_size =
        std::min(kLanMaximumFragmentPayload, payload.size() - payload_offset);
    PacketWriter writer(kLanMaximumDatagramSize);
    writer.PutBytes(kLanMagic);
    writer.PutU8(kLanProtocolVersion);
    writer.PutU8(operation);
    writer.PutU64(message_id);
    writer.PutU16(fragment_index);
    writer.PutU16(fragment_count);
    writer.PutU32(static_cast<uint32_t>(payload.size()));
    writer.PutBytes(payload.subspan(payload_offset, fragment_size));
    if (!writer.valid()) {
      return {};
    }
    frames.push_back(writer.data());
  }
  return frames;
}

struct LanPacketReassembler::Impl {
  struct Key {
    uint32_t source_ipv4 = 0;
    uint16_t source_port = 0;
    uint64_t message_id = 0;

    bool operator==(const Key&) const = default;
  };

  struct KeyHash {
    size_t operator()(const Key& key) const {
      size_t value = std::hash<uint64_t>{}(key.message_id);
      value ^= std::hash<uint32_t>{}(key.source_ipv4) + 0x9E3779B9U + (value << 6) +
               (value >> 2);
      value ^= std::hash<uint16_t>{}(key.source_port) + 0x9E3779B9U + (value << 6) +
               (value >> 2);
      return value;
    }
  };

  struct Pending {
    uint8_t operation = 0;
    uint16_t fragment_count = 0;
    uint32_t total_size = 0;
    uint16_t received_count = 0;
    std::vector<uint8_t> bytes;
    std::vector<uint8_t> received;
    std::chrono::steady_clock::time_point expires_at;
  };

  std::unordered_map<Key, Pending, KeyHash> pending;
  std::unordered_map<Key, std::chrono::steady_clock::time_point, KeyHash> completed;
};

LanPacketReassembler::LanPacketReassembler() : impl_(std::make_unique<Impl>()) {}
LanPacketReassembler::~LanPacketReassembler() = default;
LanPacketReassembler::LanPacketReassembler(LanPacketReassembler&&) noexcept = default;
LanPacketReassembler& LanPacketReassembler::operator=(LanPacketReassembler&&) noexcept = default;

void LanPacketReassembler::Expire(std::chrono::steady_clock::time_point now) {
  std::erase_if(impl_->pending,
                [now](const auto& entry) { return entry.second.expires_at <= now; });
  std::erase_if(impl_->completed, [now](const auto& entry) { return entry.second <= now; });
}

std::optional<DecodedLanPayload> LanPacketReassembler::Consume(
    uint32_t source_ipv4, uint16_t source_port, std::span<const uint8_t> datagram,
    std::chrono::steady_clock::time_point now) {
  Expire(now);
  if (datagram.size() < kLanFrameHeaderSize || datagram.size() > kLanMaximumDatagramSize) {
    return std::nullopt;
  }

  PacketReader reader(datagram);
  std::array<uint8_t, 8> magic{};
  uint8_t version = 0;
  uint8_t operation = 0;
  uint64_t message_id = 0;
  uint16_t fragment_index = 0;
  uint16_t fragment_count = 0;
  uint32_t total_size = 0;
  if (!reader.GetBytes(magic) || magic != kLanMagic || !reader.GetU8(version) ||
      version != kLanProtocolVersion || !reader.GetU8(operation) ||
      operation < kOperationQuery || operation > kOperationMigrate ||
      !reader.GetU64(message_id) || !message_id || !reader.GetU16(fragment_index) ||
      !reader.GetU16(fragment_count) || !reader.GetU32(total_size) ||
      fragment_count == 0 || fragment_count > kLanMaximumFragmentCount ||
      fragment_index >= fragment_count || total_size > kLanMaximumRecordSize ||
      (operation == kOperationQuery && total_size != 0)) {
    return std::nullopt;
  }

  const size_t canonical_fragment_count =
      total_size == 0 ? 1 : (static_cast<size_t>(total_size) + kLanMaximumFragmentPayload - 1) /
                                  kLanMaximumFragmentPayload;
  if (fragment_count != canonical_fragment_count) {
    return std::nullopt;
  }
  const size_t payload_offset =
      static_cast<size_t>(fragment_index) * kLanMaximumFragmentPayload;
  const size_t expected_size =
      std::min(kLanMaximumFragmentPayload, static_cast<size_t>(total_size) - payload_offset);
  if (reader.remaining() != expected_size) {
    return std::nullopt;
  }
  const auto fragment = datagram.subspan(kLanFrameHeaderSize, expected_size);
  const Impl::Key key{.source_ipv4 = source_ipv4,
                      .source_port = source_port,
                      .message_id = message_id};
  if (impl_->completed.contains(key)) {
    return std::nullopt;
  }

  auto pending = impl_->pending.find(key);
  if (pending == impl_->pending.end()) {
    if (impl_->pending.size() >= kMaximumPendingReassemblies) {
      auto oldest = std::ranges::min_element(
          impl_->pending, {}, [](const auto& entry) { return entry.second.expires_at; });
      if (oldest != impl_->pending.end()) {
        impl_->pending.erase(oldest);
      }
    }
    Impl::Pending created;
    created.operation = operation;
    created.fragment_count = fragment_count;
    created.total_size = total_size;
    created.bytes.resize(total_size);
    created.received.resize(fragment_count);
    created.expires_at = now + kLanReassemblyLifetime;
    pending = impl_->pending.emplace(key, std::move(created)).first;
  } else if (pending->second.operation != operation ||
             pending->second.fragment_count != fragment_count ||
             pending->second.total_size != total_size) {
    impl_->pending.erase(pending);
    return std::nullopt;
  }

  auto& assembly = pending->second;
  if (assembly.received[fragment_index]) {
    if (!std::equal(fragment.begin(), fragment.end(),
                    assembly.bytes.begin() + payload_offset)) {
      impl_->pending.erase(pending);
    }
    return std::nullopt;
  }
  std::copy(fragment.begin(), fragment.end(), assembly.bytes.begin() + payload_offset);
  assembly.received[fragment_index] = 1;
  ++assembly.received_count;
  assembly.expires_at = now + kLanReassemblyLifetime;
  if (assembly.received_count != assembly.fragment_count) {
    return std::nullopt;
  }

  DecodedLanPayload complete{.operation = assembly.operation,
                             .bytes = std::move(assembly.bytes)};
  impl_->pending.erase(pending);
  if (impl_->completed.size() >= kMaximumPendingReassemblies) {
    auto oldest = std::ranges::min_element(
        impl_->completed, {}, [](const auto& entry) { return entry.second; });
    if (oldest != impl_->completed.end()) {
      impl_->completed.erase(oldest);
    }
  }
  impl_->completed[key] = now + kLanReassemblyLifetime;
  return complete;
}

size_t LanPacketReassembler::pending_count() const { return impl_->pending.size(); }
size_t LanPacketReassembler::completed_count() const { return impl_->completed.size(); }

}  // namespace detail

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
    uint32_t procedure_index, std::span<const SessionContext> contexts,
    std::span<const SessionProperty> properties, uint32_t maximum_results) {
  std::lock_guard lock(mutex_);
  return SearchRecords(sessions_, title_id, media_id, title_version, protocol_version,
                       procedure_index, contexts, properties, maximum_results);
}

std::optional<SessionRecord> InMemorySessionDirectory::Get(uint64_t session_id) {
  std::lock_guard lock(mutex_);
  auto it = sessions_.find(session_id);
  return it == sessions_.end() ? std::nullopt : std::optional(it->second);
}

std::optional<SessionRecord> InMemorySessionDirectory::ResolveMigration(
    uint64_t previous_session_id) {
  std::lock_guard lock(mutex_);
  const auto replacement = std::ranges::find_if(
      sessions_, [&](const auto& entry) {
        return entry.second.previous_session_id == previous_session_id &&
               entry.second.session_id != previous_session_id;
      });
  return replacement == sessions_.end()
             ? std::nullopt
             : std::optional<SessionRecord>(replacement->second);
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

std::optional<SessionRecord> InMemorySessionDirectory::Migrate(
    uint64_t session_id, const SessionRecord& replacement) {
  std::lock_guard lock(mutex_);
  auto it = sessions_.find(session_id);
  if (it == sessions_.end() || !ValidateRecord(replacement) ||
      (replacement.session_id != session_id && sessions_.contains(replacement.session_id))) {
    return std::nullopt;
  }
  SessionRecord migrated = replacement;
  migrated.previous_session_id = session_id;
  migrated.last_seen = std::chrono::steady_clock::now();
  sessions_.erase(it);
  sessions_[migrated.session_id] = migrated;
  return migrated;
}

bool InMemorySessionDirectory::Delete(uint64_t session_id) {
  std::lock_guard lock(mutex_);
  return sessions_.erase(session_id) != 0;
}

LanSessionDirectory::LanSessionDirectory(uint16_t discovery_port)
    : discovery_port_(discovery_port),
      reassembler_(std::make_unique<detail::LanPacketReassembler>()),
      next_message_id_(InitialLanMessageId()) {
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
      detail::SerializeLanSessionRecord(session).empty()) {
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
                                                       uint32_t procedure_index,
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
  return SearchRecords(discovered_sessions_, title_id, media_id, title_version,
                       protocol_version, procedure_index, contexts, properties,
                       maximum_results, &hosted_sessions_);
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

std::optional<SessionRecord> LanSessionDirectory::ResolveMigration(
    uint64_t previous_session_id) {
  std::lock_guard lock(mutex_);
  const auto find_replacement = [&](const auto& sessions)
      -> std::optional<SessionRecord> {
    const auto replacement = std::ranges::find_if(
        sessions, [&](const auto& entry) {
          return entry.second.previous_session_id == previous_session_id &&
                 entry.second.session_id != previous_session_id;
        });
    return replacement == sessions.end()
               ? std::nullopt
               : std::optional<SessionRecord>(replacement->second);
  };
  if (auto hosted = find_replacement(hosted_sessions_)) return hosted;
  return find_replacement(discovered_sessions_);
}

bool LanSessionDirectory::Modify(const SessionRecord& session) {
  if (!ready() || !ValidateRecord(session) ||
      detail::SerializeLanSessionRecord(session).empty()) {
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

std::optional<SessionRecord> LanSessionDirectory::Migrate(
    uint64_t session_id, const SessionRecord& replacement) {
  SessionRecord migrated = replacement;
  migrated.previous_session_id = session_id;
  migrated.last_seen = std::chrono::steady_clock::now();
  if (!ready() || !ValidateRecord(migrated) ||
      detail::SerializeLanSessionRecord(migrated).empty()) {
    return std::nullopt;
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
      return std::nullopt;
    }
    hosted_sessions_.erase(session_id);
    discovered_sessions_.erase(session_id);
    deletion_tombstones_[session_id] = std::chrono::steady_clock::now();
    deletion_tombstones_.erase(migrated.session_id);
    hosted_sessions_[migrated.session_id] = migrated;
  }
  if (SendRecord(migrated, kOperationMigrate)) {
    return migrated;
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
  return std::nullopt;
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
  return SendPayload(kOperationQuery, {});
}

bool LanSessionDirectory::SendPayload(uint8_t operation, std::span<const uint8_t> payload) {
  if (socket_ == rex::net::kInvalidSocket) {
    return false;
  }
  uint64_t message_id = next_message_id_.fetch_add(1, std::memory_order_relaxed);
  if (!message_id) {
    message_id = next_message_id_.fetch_add(1, std::memory_order_relaxed);
  }
  const auto frames = detail::FrameLanPayload(operation, payload, message_id);
  if (frames.empty()) {
    return false;
  }
  sockaddr_in destination{};
  destination.sin_family = AF_INET;
  destination.sin_port = htons(discovery_port_);
  inet_pton(AF_INET, kMulticastAddress, &destination.sin_addr);
  for (const auto& frame : frames) {
    if (sendto(static_cast<int>(socket_), reinterpret_cast<const char*>(frame.data()),
               static_cast<int>(frame.size()), 0, reinterpret_cast<sockaddr*>(&destination),
               sizeof(destination)) < 0) {
      SetError("Unable to send LAN session frame");
      return false;
    }
  }
  return true;
}

bool LanSessionDirectory::SendRecord(const SessionRecord& session, uint8_t operation) {
  const auto payload = detail::SerializeLanSessionRecord(session);
  if (payload.empty()) {
    return false;
  }
  return SendPayload(operation, payload);
}

void LanSessionDirectory::ReceivePacket() {
  std::array<uint8_t, detail::kLanMaximumDatagramSize> bytes{};
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

  auto payload = reassembler_->Consume(
      source.sin_addr.s_addr, source.sin_port,
      std::span(bytes.data(), static_cast<size_t>(received)));
  if (!payload) {
    return;
  }
  const uint8_t operation = payload->operation;
  auto decoded = detail::DeserializeLanSessionRecord(operation, payload->bytes);
  if (!decoded) {
    return;
  }
  auto session = std::move(*decoded);
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
  reassembler_->Expire();
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
      {
        auto services = detail::CreateInMemoryOnlineServices(identity_.xuid, session_directory_);
        social_service_ = std::move(services.social_service);
        stats_service_ = std::move(services.stats_service);
        voice_transport_ = std::move(services.voice_transport);
        qos_service_ = std::move(services.qos_service);
      }
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
      social_service_ = std::move(services.social_service);
      stats_service_ = std::move(services.stats_service);
      achievement_service_ = std::move(services.achievement_service);
      entitlement_service_ = std::move(services.entitlement_service);
      title_profile_service_ = std::move(services.title_profile_service);
      gta4_achievement_storage_service_ =
          std::move(services.gta4_achievement_storage_service);
      voice_transport_ = std::move(services.voice_transport);
      text_chat_transport_ = std::move(services.text_chat_transport);
      qos_service_ = std::move(services.qos_service);
      break;
    }
  }

  if (config_.backend == LiveBackend::kCommunity) {
    try {
      heartbeat_worker_ = std::thread(&LiveCompatibilityRuntime::HeartbeatWorkerMain, this);
      community_recovery_enabled_ = true;
    } catch (...) {
      REXSYS_ERROR("Unable to start the community session heartbeat worker");
      state_.store(LiveState::kError, std::memory_order_release);
      return;
    }
  }

  if (ServicesReady()) {
    state_.store(LiveState::kAvailable, std::memory_order_release);
    REXSYS_INFO("LibertyRecomp multiplayer services available as {} ({:016X})",
                identity_.player_name, identity_.xuid);
  } else {
    const std::string error =
        session_directory_ && !session_directory_->ready() ? session_directory_->last_error()
        : peer_transport_ && !peer_transport_->ready()     ? peer_transport_->last_error()
        : social_service_ && !social_service_->ready()     ? social_service_->last_error()
        : stats_service_ && !stats_service_->ready()       ? stats_service_->last_error()
        : achievement_service_ && !achievement_service_->ready()
            ? achievement_service_->last_error()
        : entitlement_service_ && !entitlement_service_->ready()
            ? entitlement_service_->last_error()
        : title_profile_service_ && !title_profile_service_->ready()
            ? title_profile_service_->last_error()
        : gta4_achievement_storage_service_ &&
                  !gta4_achievement_storage_service_->ready()
            ? gta4_achievement_storage_service_->last_error()
        : voice_transport_ && !voice_transport_->ready()   ? voice_transport_->last_error()
        : text_chat_transport_ && !text_chat_transport_->ready()
            ? text_chat_transport_->last_error()
        : qos_service_ && !qos_service_->ready()           ? qos_service_->last_error()
                                                           : "missing backend service";
    REXSYS_ERROR("Unable to initialize multiplayer backend: {}", error);
    state_.store(LiveState::kError, std::memory_order_release);
    if (signed_in()) {
      REXSYS_INFO(
          "LIVE profile identity remains signed in as {} ({:016X}); backend service "
          "operations remain unavailable",
          identity_.player_name, identity_.xuid);
    }
  }
}

LiveCompatibilityRuntime::~LiveCompatibilityRuntime() { Shutdown(); }

bool LiveCompatibilityRuntime::ServicesReady() const {
  const bool transport_ready =
      config_.backend != LiveBackend::kCommunity || (peer_transport_ && peer_transport_->ready());
  const bool imported_services_ready =
      social_service_ && social_service_->ready() && stats_service_ && stats_service_->ready() &&
      voice_transport_ && voice_transport_->ready() && qos_service_ && qos_service_->ready() &&
      (config_.backend != LiveBackend::kCommunity ||
       (achievement_service_ && achievement_service_->ready() && entitlement_service_ &&
        entitlement_service_->ready() && title_profile_service_ &&
        title_profile_service_->ready() && gta4_achievement_storage_service_ &&
        gta4_achievement_storage_service_->ready() && text_chat_transport_ &&
        text_chat_transport_->ready()));
  return session_directory_ && session_directory_->ready() && transport_ready &&
         imported_services_ready;
}

bool LiveCompatibilityRuntime::voice_capture_available() const noexcept {
  return detail::HostVoiceCaptureAvailable(config_);
}

namespace {

void CanonicalizeVoiceRoute(VoiceRoute& route,
                            const std::optional<SessionRecord>& session,
                            uint64_t local_xuid) {
  const auto is_remote_member = [&](uint64_t xuid) {
    return xuid && xuid != local_xuid &&
           (!session || std::ranges::find(session->members, xuid,
                                          &SessionMember::xuid) !=
                            session->members.end());
  };
  std::erase_if(route.target_xuids,
                [&](uint64_t xuid) { return !is_remote_member(xuid); });
  std::erase_if(route.muted_xuids,
                [&](uint64_t xuid) { return !is_remote_member(xuid); });
  std::ranges::sort(route.target_xuids);
  route.target_xuids.erase(
      std::unique(route.target_xuids.begin(), route.target_xuids.end()),
      route.target_xuids.end());
  std::ranges::sort(route.muted_xuids);
  route.muted_xuids.erase(
      std::unique(route.muted_xuids.begin(), route.muted_xuids.end()),
      route.muted_xuids.end());
  if (route.channel == VoiceChannel::kAll) route.target_xuids.clear();
}

}  // namespace

bool LiveCompatibilityRuntime::ConfigureVoiceRoute(VoiceRoute route) {
  auto* transport = voice_transport();
  if (!transport || !transport->ready() || !route.session_id) return false;
  const std::optional<SessionRecord> session = FindSessionRoute(route.session_id);
  CanonicalizeVoiceRoute(route, session, identity_.xuid);

  std::lock_guard lock(voice_route_mutex_);
  if (configured_voice_route_ == route) return true;
  if (!transport->Configure(route)) return false;
  configured_voice_route_ = std::move(route);
  return true;
}

bool LiveCompatibilityRuntime::ConfigureVoiceSession(
    uint64_t session_id, VoiceSessionTransition transition) {
  if (!session_id) return false;
  VoiceRoute route{.session_id = session_id};
  {
    std::lock_guard lock(voice_route_mutex_);
    const bool preserve =
        configured_voice_route_ &&
        (transition == VoiceSessionTransition::kMigration ||
         (transition == VoiceSessionTransition::kMembershipChanged &&
          configured_voice_route_->session_id == session_id));
    if (preserve) {
      route = *configured_voice_route_;
      route.session_id = session_id;
    }
  }
  return ConfigureVoiceRoute(std::move(route));
}

void LiveCompatibilityRuntime::CloseVoiceSession(uint64_t session_id) {
  std::lock_guard lock(voice_route_mutex_);
  if (!configured_voice_route_ || configured_voice_route_->session_id != session_id) return;
  if (voice_transport_) voice_transport_->Close();
  configured_voice_route_.reset();
}

std::optional<VoiceRoute> LiveCompatibilityRuntime::configured_voice_route() const {
  std::lock_guard lock(voice_route_mutex_);
  return configured_voice_route_;
}

void LiveCompatibilityRuntime::SetInviteNotificationHandler(
    std::function<void()> handler) {
  if (social_service_) {
    social_service_->SetInviteNotificationHandler(std::move(handler));
  }
}

void LiveCompatibilityRuntime::Shutdown() {
  std::call_once(shutdown_once_, [this] {
    {
      std::lock_guard lock(heartbeat_mutex_);
      heartbeat_stopping_ = true;
    }
    heartbeat_condition_.notify_all();
    if (heartbeat_worker_.joinable()) heartbeat_worker_.join();
    if (social_service_) {
      social_service_->SetInviteNotificationHandler({});
      social_service_->Shutdown();
    }
  });
}

void LiveCompatibilityRuntime::HeartbeatWorkerMain() {
  std::unique_lock lock(heartbeat_mutex_);
  while (!heartbeat_stopping_) {
    if (heartbeat_condition_.wait_for(lock, detail::kCommunityHeartbeatInterval,
                                      [&] { return heartbeat_stopping_; })) {
      break;
    }
    lock.unlock();
    const uint64_t session_id = active_session_id();
    if (session_id && session_directory_) {
      const auto session = session_directory_->Get(session_id);
      if (session && detail::ShouldHeartbeatActiveSession(
                         config_.backend, session_id, identity_.xuid, session->host_xuid)) {
        (void)session_directory_->Heartbeat(*session);
      }
    }
    lock.lock();
  }
}

LiveState LiveCompatibilityRuntime::state() const {
  const LiveState current = state_.load(std::memory_order_acquire);
  if (config_.backend == LiveBackend::kCommunity && !community_recovery_enabled_) {
    return current;
  }
  return detail::ResolveLiveServiceState(config_.backend, current, ServicesReady());
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
  if (std::getenv("REX_GTA4_SESSION_ATTRIBUTE_TRACE")) {
    REXSYS_INFO("gta4-session-attributes point=context-set session={:016X} id={:08X} value={}",
                active_session_id(), id, value);
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
  if (std::getenv("REX_GTA4_SESSION_ATTRIBUTE_TRACE")) {
    uint32_t scalar = 0;
    if (value.size() == sizeof(scalar)) {
      for (uint8_t byte : value) scalar = (scalar << 8) | byte;
    }
    REXSYS_INFO("gta4-session-attributes point=property-set session={:016X} id={:08X} bytes={} scalar={}",
                active_session_id(), id, value.size(), scalar);
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
  // These are cached profile capabilities. Backend readiness is enforced by
  // the concrete XSession/social/transport calls, and must not masquerade as
  // an account privilege revocation during a transient service outage.
  if (!signed_in()) {
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

std::optional<SessionRecord> LiveCompatibilityRuntime::FindSessionRoute(
    uint64_t session_id) const {
  std::lock_guard lock(route_mutex_);
  const auto route = std::ranges::find_if(
      routes_, [&](const auto& entry) { return entry.second.session_id == session_id; });
  return route == routes_.end() ? std::nullopt
                               : std::optional<SessionRecord>(route->second);
}

detail::TitleOnlineEndpoint detail::SelectTitleOnlineEndpoint(
    LiveBackend backend, uint32_t local_ipv4, uint16_t local_port,
    const std::optional<SessionMember>& local_session_member) {
  if (backend == LiveBackend::kCommunity && local_session_member &&
      local_session_member->virtual_ipv4 && local_session_member->online_port) {
    return {.ipv4 = local_session_member->virtual_ipv4,
            .port = local_session_member->online_port};
  }
  return {.ipv4 = local_ipv4, .port = local_port};
}

std::optional<SessionMember> LiveCompatibilityRuntime::FindLocalSessionMember(
    uint64_t session_id) const {
  if (!session_id || !identity_.xuid) return std::nullopt;
  std::lock_guard lock(route_mutex_);
  for (const auto& [ipv4, session] : routes_) {
    (void)ipv4;
    if (session.session_id != session_id) continue;
    const auto local = std::ranges::find(
        session.members, identity_.xuid, &SessionMember::xuid);
    if (local != session.members.end() && local->virtual_ipv4 &&
        local->online_port) {
      return *local;
    }
  }
  return std::nullopt;
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

std::optional<std::array<uint8_t, 16>> LiveCompatibilityRuntime::RegisteredKey(
    uint64_t session_id) const {
  std::lock_guard lock(route_mutex_);
  const auto found = registered_keys_.find(session_id);
  return found == registered_keys_.end() ? std::nullopt
                                         : std::optional(found->second);
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
