#include "gta4_multiplayer_64_policy.h"
#include "gta4_player_info_alias_gate.h"
#include "gta4_sony_event_hooks.h"
#include "input/text_chat_team.h"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <limits>
#include <mutex>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/system/xam/live_compatibility.h>
#include <rex/system/xam/multiplayer_validation.h>

#include "gta4_init.h"

namespace {

namespace mp64 = gta4::multiplayer64;
namespace validation = rex::system::xam;
using GuestFunction = void(PPCContext&, uint8_t*);

mp64::PeerManagerRegistry g_peer_managers;
mp64::PlayerInfoRegistry g_player_infos;
mp64::PeerMaskPairRegistry g_dispatch_masks;
mp64::DispatchPeerStateRegistry g_dispatch_peer_states;
mp64::NetworkEndpointRegistry g_network_endpoints;
mp64::NetworkObjectPeerFlagsRegistry g_network_peer_flags;
mp64::PedNetworkPeerStateRegistry g_ped_network_peer_states;
mp64::EventPeerBufferRegistry g_event_peer_buffers;
mp64::EventScopeRegistry g_event_scopes;
mp64::PlayerTickStateRegistry g_player_tick_states;
mp64::ObjectManagerPeerTimingRegistry g_object_peer_timings;
mp64::ObjectManagerPeerBufferRegistry g_object_peer_buffers;
mp64::ObjectOwnerListRegistry g_object_owner_lists;
mp64::ObjectPeerMatrixRegistry g_object_peer_matrix;
mp64::SessionParticipantRegistry g_session_participants;
mp64::ParticipantCommandRegistry g_participant_commands;
mp64::MigrationTaskRegistry g_migration_tasks;
mp64::ReassignmentRegistry g_reassignments;
std::mutex g_alias_override_mutex;
struct AliasOverrideSet {
  std::array<uint32_t, mp64::kLegacyPeerCapacity> peers{};
  std::array<bool, mp64::kLegacyPeerCapacity> active{};
};
std::unordered_map<uint32_t, AliasOverrideSet> g_alias_overrides;
std::recursive_mutex g_player_info_alias_mutex;
std::atomic<bool> g_player_info_alias_active = false;
std::atomic<uint32_t> g_player_info_alias_legacy_zero = 0;
std::mutex g_player_info_shadow_mutex;
uint32_t g_guest_player_info_shadow = 0;
std::recursive_mutex g_proximity_status_mutex;
std::array<std::array<uint8_t, mp64::kProximityStatusRecordSize>,
           mp64::kExtendedPeerCapacity>
    g_proximity_status_sidecar{};
std::mutex g_voice_cursor_mutex;
std::unordered_map<uint32_t, mp64::RoundRobinCursor> g_voice_cursors;
std::mutex g_extended_slot_command_mutex;
std::mutex g_session_participant_mutation_mutex;
std::recursive_mutex g_reassignment_mutex;
std::recursive_mutex g_dispatch_alias_mutex;
std::recursive_mutex g_network_peer_alias_mutex;
std::recursive_mutex g_event_peer_alias_mutex;
std::recursive_mutex g_peer_lookup_alias_mutex;
std::recursive_mutex g_object_peer_alias_mutex;
std::recursive_mutex g_player_tick_alias_mutex;
std::mutex g_dispatch_message_mutex;
std::unordered_map<uint32_t, std::array<uint32_t, mp64::kExtendedPeerCapacity>> g_dispatch_messages;
std::mutex g_extended_traffic_mutex;
std::array<uint64_t, mp64::kExtendedPeerCapacity> g_extended_traffic_bytes{};

class PeerTeamCache {
 public:
  PeerTeamCache() {
    for (auto& team : teams_) team.store(-1, std::memory_order_relaxed);
  }

  void Set(uint8_t peer_id, int32_t team) {
    if (mp64::IsValidPeerId(peer_id)) {
      teams_[peer_id].store(team, std::memory_order_release);
    }
  }

  int32_t Get(uint8_t peer_id) const {
    return mp64::IsValidPeerId(peer_id)
               ? teams_[peer_id].load(std::memory_order_acquire)
               : -1;
  }

  void Clear(uint8_t peer_id) { Set(peer_id, -1); }

 private:
  std::array<std::atomic<int32_t>, mp64::kExtendedPeerCapacity> teams_{};
};

PeerTeamCache g_peer_teams;

std::once_flag g_invalid_add_peer_warning;
std::once_flag g_extended_peer_allocation_warning;
std::once_flag g_player_info_table_warning;
std::once_flag g_ownership_warning;

constexpr uint8_t kTemporaryAliasPeerId = 0;
constexpr uint32_t kVoiceEnumerationReturnAddress = 0x82700668;
constexpr uint32_t kReassignmentNegotiationReturnAddress = 0x82786EC8;
constexpr uint32_t kReassignmentResetNegotiationReturnAddress = 0x827870FC;
constexpr uint32_t kReassignmentConfirmationReturnAddress = 0x82787204;

struct ExtendedAddContext {
  bool active = false;
  bool initial_alias_lookup_consumed = false;
  bool constructed = false;
  bool notification_boundary_reached = false;
  uint32_t guest_manager = 0;
  uint32_t guest_record = 0;
  uint32_t guest_add_packet = 0;
  uint32_t saved_alias_peer = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
};

struct ExtendedRemoveContext {
  bool active = false;
  bool initial_alias_lookup_consumed = false;
  uint32_t guest_manager = 0;
  uint32_t guest_record = 0;
  uint32_t saved_alias_peer = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
};

struct ExtendedSlotCommandContext {
  bool active = false;
  bool command_patched = false;
  uint32_t guest_session = 0;
  uint32_t public_slots = 0;
  uint32_t private_slots = 0;
};

thread_local ExtendedAddContext g_extended_add;
thread_local ExtendedRemoveContext g_extended_remove;
thread_local ExtendedSlotCommandContext g_extended_slot_command;

struct PlayerTransitionLoopContext {
  bool active = false;
  bool reset_loop_seen = false;
  bool force_loop_seen = false;
};

thread_local PlayerTransitionLoopContext g_player_transition_loop;

struct ThresholdPlayerLoopContext {
  bool active = false;
  bool seen = false;
  int32_t excluded_unique_value = -1;
  uint32_t guest_match_list = 0;
};

thread_local ThresholdPlayerLoopContext g_threshold_player_loop;

struct NearestNetworkDecisionContext {
  bool active = false;
  bool captured = false;
  double score = 0.0;
  PPCContext call_ctx{};
};

thread_local NearestNetworkDecisionContext g_nearest_network_decision;

struct PlayerInfoBatchProjectionContext {
  bool active = false;
  std::array<uint8_t, mp64::kLegacyPeerCapacity> actual_ids{};
};

thread_local PlayerInfoBatchProjectionContext g_player_info_batch_projection;

struct PopulationDispatchCaptureContext {
  bool active = false;
  std::vector<PPCContext> calls;
};

thread_local PopulationDispatchCaptureContext g_population_dispatch_capture;

struct LobbyPositionLoopContext {
  bool active = false;
  bool seen = false;
};

thread_local LobbyPositionLoopContext g_lobby_position_loop;
thread_local bool g_proximity_status_driver_active = false;

struct ProximityStatusCaptureContext {
  bool active = false;
  std::array<bool, mp64::kExtendedPeerCapacity> eligible{};
};

thread_local ProximityStatusCaptureContext g_proximity_status_capture;

struct MigrationSnapshotOverride {
  bool active = false;
  uint32_t guest_owner = 0;
  uint32_t guest_records = 0;
  uint32_t count = 0;
};

thread_local MigrationSnapshotOverride g_migration_snapshot_override;

struct ReassignmentCallContext {
  bool active = false;
  uint32_t guest_manager = 0;
  uint8_t actual_owner = mp64::kInvalidPeerId;
  uint8_t owner_alias = mp64::kReassignmentOwnerAliasPeerId;
  uint8_t actual_recipient = mp64::kInvalidPeerId;
  uint8_t recipient_alias = mp64::kReassignmentRecipientAliasPeerId;
  uint8_t actual_object_list_owner = mp64::kInvalidPeerId;
  uint8_t object_list_alias = mp64::kInvalidPeerId;
  uint32_t guest_transport_state = 0;
};

thread_local ReassignmentCallContext g_reassignment_call;

struct DispatchAliasContext {
  bool active = false;
  uint32_t guest_handler = 0;
  uint32_t guest_network_manager = 0;
  uint32_t guest_peer = 0;
  uint32_t guest_message = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
  uint8_t alias_peer_id = kTemporaryAliasPeerId;
};

thread_local DispatchAliasContext g_dispatch_alias;

struct NetworkPeerAliasContext {
  bool active = false;
  uint32_t guest_object = 0;
  uint32_t guest_peer = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
  uint8_t alias_peer_id = kTemporaryAliasPeerId;
};

thread_local NetworkPeerAliasContext g_network_peer_alias;

struct EventPeerAliasContext {
  bool active = false;
  uint32_t guest_event_manager = 0;
  uint32_t guest_peer_manager = 0;
  uint32_t guest_peer = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
  uint8_t alias_peer_id = kTemporaryAliasPeerId;
};

thread_local EventPeerAliasContext g_event_peer_alias;
thread_local bool g_event_manager_tick_active = false;

struct ObjectManagerPeerAliasContext {
  bool active = false;
  uint32_t guest_object_manager = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
  uint8_t alias_peer_id = kTemporaryAliasPeerId;
};

thread_local ObjectManagerPeerAliasContext g_object_peer_alias;

// sub_826EDFE0 has a fixed sixteen-entry candidate scratch array, but consumes
// only the first eligible peer as the recovery source. When no retail peer is
// eligible, expose one extended peer through an otherwise-ineligible retail
// lookup slot. Unlike the general object-manager alias, this context remaps
// only timeout lookups: object flags/endpoints must continue to see the real
// high peer ID and use their canonical sidecars.
struct ObjectRecoveryPeerAliasContext {
  bool active = false;
  uint32_t guest_object_manager = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
  uint8_t alias_peer_id = kTemporaryAliasPeerId;
};

thread_local ObjectRecoveryPeerAliasContext g_object_recovery_peer_alias;

struct PlayerTickAliasContext {
  bool active = false;
  uint32_t guest_manager = 0;
  uint32_t guest_player_info = 0;
  uint8_t actual_peer_id = mp64::kInvalidPeerId;
  uint8_t alias_peer_id = kTemporaryAliasPeerId;
};

thread_local PlayerTickAliasContext g_player_tick_alias;

struct LocalPlayerInfoAliasContext {
  bool active = false;
  uint8_t actual_player_id = mp64::kInvalidPeerId;
  uint8_t alias_player_id = kTemporaryAliasPeerId;
  uint32_t guest_player_info = 0;
};

thread_local LocalPlayerInfoAliasContext g_local_player_info_alias;

struct PrimaryPlayerInfoAliasSession {
  uint8_t actual_player_id = mp64::kInvalidPeerId;
  uint8_t alias_player_id = kTemporaryAliasPeerId;
  uint32_t guest_player_info = 0;
  uint32_t saved_pointer = 0;
  uint32_t saved_generation = 0;
  uint32_t saved_primary = 0;
  uint32_t saved_secondary = 0;
};

mp64::JoinableProjectionGate<PrimaryPlayerInfoAliasSession> g_player_info_alias_gate;

template <typename Callback>
class ScopeExit {
 public:
  explicit ScopeExit(Callback callback) : callback_(std::move(callback)) {}
  ScopeExit(const ScopeExit&) = delete;
  ScopeExit& operator=(const ScopeExit&) = delete;
  ~ScopeExit() { callback_(); }

 private:
  Callback callback_;
};

template <typename Callback>
ScopeExit(Callback) -> ScopeExit<Callback>;

struct PlayerInfoLease : mp64::PlayerInfoEntry {
  decltype(g_player_info_alias_gate)::ReadAdmission admission;
};

struct ProximityWeightContext {
  bool active = false;
  bool extended_injected = false;
};

thread_local ProximityWeightContext g_proximity_weight;

struct PeerUniquenessValue {
  int32_t value = -1;
  uint32_t guest_peer = 0;
  uint32_t guest_player_info = 0;
};

// sub_826FFCB0 assigns a small player-visible value while avoiding collisions
// with connected peers. Its two uniqueness scans read the embedded sixteen
// pointers directly. The hooks below preserve that control flow and expose
// high-peer identity/value matches at the exact accessor calls made by the
// retail scan.
struct PeerUniquenessContext {
  bool active = false;
  bool force_identity_player_info = false;
  size_t identity_calls_until_injection = 0;
  uint64_t target_identity = 0;
  uint32_t matching_identity_player_info = 0;
  std::array<PeerUniquenessValue, mp64::kExtendedPeerCapacity> high_values{};
  size_t high_value_count = 0;
};

thread_local PeerUniquenessContext g_peer_uniqueness;

struct ClonePeerExpansionContext {
  bool active = false;
};

thread_local ClonePeerExpansionContext g_clone_peer_expansion;

struct ObjectOwnerListAliasContext {
  bool active = false;
  uint32_t guest_object_manager = 0;
  uint8_t actual_owner_id = mp64::kInvalidPeerId;
  uint8_t alias_owner_id = kTemporaryAliasPeerId;
};

thread_local ObjectOwnerListAliasContext g_object_owner_list_alias;

struct NetworkPeerMaskSnapshot {
  bool pending = false;
  uint32_t guest_object = 0;
  uint32_t low_first = 0;
  uint32_t low_second = 0;
  uint32_t low_third = 0;
  mp64::PeerMask64 first;
  mp64::PeerMask64 second;
  mp64::PeerMask64 third;
};

thread_local NetworkPeerMaskSnapshot g_network_peer_mask_snapshot;

uint8_t GuestPeerId(uint64_t value) noexcept {
  return static_cast<uint8_t>(value);
}

uint8_t CanonicalNetworkEndpointPeerId(uint32_t guest_object, uint8_t presented_peer_id) {
  if (g_network_peer_alias.active && g_network_peer_alias.guest_object == guest_object &&
      g_network_peer_alias.alias_peer_id == presented_peer_id &&
      mp64::ClassifyPeerId(g_network_peer_alias.actual_peer_id) ==
          mp64::PeerIdClass::kExtended) {
    return g_network_peer_alias.actual_peer_id;
  }
  return presented_peer_id;
}

bool PublishActiveNetworkAliasEndpoint(uint8_t* base, uint32_t guest_object,
                                       uint8_t actual_peer_id, uint32_t guest_endpoint) {
  if (!g_network_peer_alias.active || g_network_peer_alias.guest_object != guest_object ||
      g_network_peer_alias.actual_peer_id != actual_peer_id) {
    return true;
  }
  const auto endpoint_table =
      mp64::CheckedGuestAddress(guest_object, mp64::kLegacyNetworkEndpointTableOffset);
  const auto alias_endpoint = endpoint_table
                                  ? mp64::CheckedGuestArrayAddress(
                                        *endpoint_table, g_network_peer_alias.alias_peer_id,
                                        mp64::kGuestPointerSize)
                                  : std::nullopt;
  if (!alias_endpoint) {
    return false;
  }
  REX_STORE_U32(*alias_endpoint, guest_endpoint);
  return true;
}

PlayerInfoLease PlayerInfoForId(uint8_t* base, uint8_t player_id) {
  auto alias_gate = g_player_info_alias_gate.EnterRead();
  mp64::PlayerInfoEntry entry;
  switch (mp64::ClassifyPeerId(player_id)) {
    case mp64::PeerIdClass::kLegacy: {
      if (player_id == kTemporaryAliasPeerId && alias_gate.session() != nullptr) {
        entry = {alias_gate.session()->saved_pointer, alias_gate.session()->saved_generation};
        break;
      }
      const auto pointer = mp64::LegacyPlayerInfoPointerAddress(player_id);
      const auto generation = mp64::LegacyPlayerInfoGenerationAddress(player_id);
      entry = pointer && generation
                  ? mp64::PlayerInfoEntry{REX_LOAD_U32(*pointer), REX_LOAD_U32(*generation)}
                  : mp64::PlayerInfoEntry{};
      break;
    }
    case mp64::PeerIdClass::kExtended:
      entry = g_player_infos.Get(player_id);
      break;
    case mp64::PeerIdClass::kInvalid:
      break;
  }
  return {entry, std::move(alias_gate)};
}

void SyncPlayerInfoShadow(uint8_t* base) {
  // Join a published primary projection or overlap with other canonical
  // readers. PlayerInfoForId uses the session snapshot for canonical slot zero.
  auto alias_gate = g_player_info_alias_gate.EnterRead();
  std::scoped_lock shadow_lock(g_player_info_shadow_mutex);
  if (g_guest_player_info_shadow == 0) {
    return;
  }
  for (uint8_t player_id = 0; player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    const auto destination = mp64::CheckedGuestArrayAddress(
        g_guest_player_info_shadow, player_id, mp64::kGuestPointerSize);
    if (destination) {
      REX_STORE_U32(*destination, PlayerInfoForId(base, player_id).guest_player_info);
    }
  }
}

template <typename Callback>
void WithPrimaryPlayerInfoAlias(PPCContext& ctx, uint8_t* base, Callback&& callback) {
  static_cast<void>(ctx);
  static_cast<void>(base);
  auto admission = g_player_info_alias_gate.EnterProjection(
      [base] { return REX_LOAD_U32(mp64::kPrimaryPlayerIdAddress); },
      [](uint32_t primary) {
        return primary >= mp64::kLegacyPeerCapacity &&
               primary < mp64::kExtendedPeerCapacity;
      },
      [base](uint32_t primary) {
        const uint8_t actual_player_id = static_cast<uint8_t>(primary);
        const mp64::PlayerInfoEntry entry = g_player_infos.Get(actual_player_id);
        PrimaryPlayerInfoAliasSession session{
            .actual_player_id = actual_player_id,
            .alias_player_id = kTemporaryAliasPeerId,
            .guest_player_info = entry.guest_player_info,
            .saved_pointer = REX_LOAD_U32(mp64::kLegacyPlayerInfoPointerTableAddress),
            .saved_generation =
                REX_LOAD_U32(mp64::kLegacyPlayerInfoGenerationTableAddress),
            .saved_primary = primary,
            .saved_secondary = REX_LOAD_U32(mp64::kSecondaryPlayerIdAddress),
        };
        REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, entry.guest_player_info);
        REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, entry.generation);
        REX_STORE_U32(mp64::kPrimaryPlayerIdAddress, kTemporaryAliasPeerId);
        if (session.saved_secondary == primary) {
          REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, kTemporaryAliasPeerId);
        }
        return session;
      });

  const LocalPlayerInfoAliasContext saved_context = g_local_player_info_alias;
  if (const PrimaryPlayerInfoAliasSession* session = admission.session()) {
    g_local_player_info_alias = {
        .active = true,
        .actual_player_id = session->actual_player_id,
        .alias_player_id = session->alias_player_id,
        .guest_player_info = session->guest_player_info,
    };
  }

  std::exception_ptr callback_failure;
  try {
    callback();
  } catch (...) {
    callback_failure = std::current_exception();
  }
  g_local_player_info_alias = saved_context;

  std::exception_ptr restoration_failure;
  if (admission.is_owner()) {
    try {
      admission.Complete([base](const PrimaryPlayerInfoAliasSession& session) {
        REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, session.saved_pointer);
        REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, session.saved_generation);
        REX_STORE_U32(mp64::kPrimaryPlayerIdAddress, session.saved_primary);
        REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, session.saved_secondary);
      });
    } catch (...) {
      restoration_failure = std::current_exception();
    }
  }
  if (callback_failure != nullptr) {
    std::rethrow_exception(callback_failure);
  }
  if (restoration_failure != nullptr) {
    std::rethrow_exception(restoration_failure);
  }
}

void RunPrimaryPlayerInfoAlias(PPCContext& ctx, uint8_t* base, GuestFunction* function) {
  WithPrimaryPlayerInfoAlias(ctx, base, [&]() { function(ctx, base); });
}

template <typename Callback>
bool WithOnlyExtendedPlayerInfo(PPCContext& ctx, uint8_t* base, uint8_t player_id,
                                Callback&& callback) {
  if (mp64::ClassifyPeerId(player_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);
  const mp64::PlayerInfoEntry entry = g_player_infos.Get(player_id);
  if (entry.guest_player_info == 0) {
    return false;
  }
  std::array<uint32_t, mp64::kLegacyPeerCapacity> saved_pointers{};
  std::array<uint32_t, mp64::kLegacyPeerCapacity> saved_generations{};
  for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
    const auto pointer = mp64::LegacyPlayerInfoPointerAddress(alias_id);
    const auto generation = mp64::LegacyPlayerInfoGenerationAddress(alias_id);
    if (!pointer || !generation) {
      return false;
    }
    saved_pointers[alias_id] = REX_LOAD_U32(*pointer);
    saved_generations[alias_id] = REX_LOAD_U32(*generation);
    REX_STORE_U32(*pointer, 0);
    REX_STORE_U32(*generation, 0);
  }

  const uint32_t saved_primary = REX_LOAD_U32(mp64::kPrimaryPlayerIdAddress);
  const uint32_t saved_secondary = REX_LOAD_U32(mp64::kSecondaryPlayerIdAddress);
  const LocalPlayerInfoAliasContext saved_context = g_local_player_info_alias;
  REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, entry.guest_player_info);
  REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, entry.generation);
  if (saved_primary == player_id) {
    REX_STORE_U32(mp64::kPrimaryPlayerIdAddress, kTemporaryAliasPeerId);
  }
  if (saved_secondary == player_id) {
    REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, kTemporaryAliasPeerId);
  }
  g_local_player_info_alias = {
      .active = true,
      .actual_player_id = player_id,
      .alias_player_id = kTemporaryAliasPeerId,
      .guest_player_info = entry.guest_player_info,
  };

  ScopeExit restore_alias([&] {
    g_local_player_info_alias = saved_context;
    REX_STORE_U32(mp64::kPrimaryPlayerIdAddress, saved_primary);
    REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, saved_secondary);
    for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
      REX_STORE_U32(*mp64::LegacyPlayerInfoPointerAddress(alias_id), saved_pointers[alias_id]);
      REX_STORE_U32(*mp64::LegacyPlayerInfoGenerationAddress(alias_id),
                    saved_generations[alias_id]);
    }
  });

  callback();
  return true;
}

template <typename Callback>
void WithProjectedPlayerInfoBatch(uint8_t first_player_id, Callback&& callback) {
  auto alias_gate = g_player_info_alias_gate.EnterRead();
  const PlayerInfoBatchProjectionContext saved_projection = g_player_info_batch_projection;
  g_player_info_batch_projection.active = true;
  for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
    g_player_info_batch_projection.actual_ids[alias_id] =
        static_cast<uint8_t>(first_player_id + alias_id);
  }
  ScopeExit restore_projection(
      [&] { g_player_info_batch_projection = saved_projection; });
  callback();
}

void WarnInvalidPeerOnce(std::once_flag& flag, std::string_view path, uint8_t peer_id,
                         std::string_view reason) {
  std::call_once(flag, [=]() {
    REXLOG_WARN("gta4-multiplayer64: path={} ignored invalid peer-id={} reason={}", path, peer_id,
                reason);
  });
}

void WarnRuntimeFailureOnce(std::once_flag& flag, std::string_view path, uint32_t value,
                            std::string_view reason) {
  std::call_once(flag, [=]() {
    REXLOG_WARN("gta4-multiplayer64: path={} operation failed value={} reason={}", path, value,
                reason);
  });
}

void PopulateLegacySidecar(uint8_t* base, uint32_t guest_manager) {
  for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
    const auto offset = mp64::LegacyPeerPointerOffset(peer_id);
    const auto address = offset ? mp64::CheckedGuestAddress(guest_manager, *offset) : std::nullopt;
    if (!address) {
      return;
    }
    g_peer_managers.SetPeer(guest_manager, peer_id, REX_LOAD_U32(*address));
  }
}

void SetAliasOverride(uint32_t guest_manager, uint8_t alias_id, uint32_t guest_peer) {
  if (!mp64::IsLegacyPeerId(alias_id)) {
    return;
  }
  std::scoped_lock lock(g_alias_override_mutex);
  AliasOverrideSet& overrides = g_alias_overrides[guest_manager];
  overrides.peers[alias_id] = guest_peer;
  overrides.active[alias_id] = true;
}

void SetAliasOverride(uint32_t guest_manager, uint32_t guest_peer) {
  SetAliasOverride(guest_manager, kTemporaryAliasPeerId, guest_peer);
}

void ClearAliasOverride(uint32_t guest_manager, uint8_t alias_id) {
  if (!mp64::IsLegacyPeerId(alias_id)) {
    return;
  }
  std::scoped_lock lock(g_alias_override_mutex);
  const auto it = g_alias_overrides.find(guest_manager);
  if (it == g_alias_overrides.end()) {
    return;
  }
  it->second.peers[alias_id] = 0;
  it->second.active[alias_id] = false;
  if (std::none_of(it->second.active.begin(), it->second.active.end(),
                   [](bool active) { return active; })) {
    g_alias_overrides.erase(it);
  }
}

void ClearAliasOverride(uint32_t guest_manager) {
  std::scoped_lock lock(g_alias_override_mutex);
  g_alias_overrides.erase(guest_manager);
}

std::optional<uint32_t> GetAliasOverride(uint32_t guest_manager, uint8_t alias_id) {
  if (!mp64::IsLegacyPeerId(alias_id)) {
    return std::nullopt;
  }
  std::scoped_lock lock(g_alias_override_mutex);
  const auto it = g_alias_overrides.find(guest_manager);
  if (it == g_alias_overrides.end() || !it->second.active[alias_id]) {
    return std::nullopt;
  }
  return it->second.peers[alias_id];
}

std::optional<uint32_t> LegacyAliasTableAddress(uint32_t guest_manager) noexcept {
  const auto offset = mp64::LegacyPeerPointerOffset(kTemporaryAliasPeerId);
  return offset ? mp64::CheckedGuestAddress(guest_manager, *offset) : std::nullopt;
}

std::optional<uint8_t> PeerRecordId(uint8_t* base, uint32_t guest_peer) noexcept {
  if (guest_peer == 0) {
    return std::nullopt;
  }
  const auto address = mp64::CheckedGuestAddress(guest_peer, 16);
  if (!address) {
    return std::nullopt;
  }
  return REX_LOAD_U8(*address);
}

uint64_t PeerRecordIdentity(const PPCContext& input_ctx, uint8_t* base, uint32_t guest_peer) {
  if (guest_peer == 0) {
    return 0;
  }
  PPCContext identity_ctx = input_ctx;
  identity_ctx.r3.u64 = guest_peer;
  __imp__sub_827087E8(identity_ctx, base);
  return identity_ctx.r3.u64;
}

uint32_t PeerRecordPlayerInfo(const PPCContext& input_ctx, uint8_t* base, uint32_t guest_peer) {
  if (guest_peer == 0) {
    return 0;
  }
  PPCContext player_info_ctx = input_ctx;
  player_info_ctx.r3.u64 = guest_peer;
  __imp__sub_827087D8(player_info_ctx, base);
  return player_info_ctx.r3.u32;
}

std::optional<int32_t> PeerUniqueValue(const PPCContext& input_ctx, uint8_t* base,
                                       uint32_t guest_peer) {
  const uint32_t guest_player_info = PeerRecordPlayerInfo(input_ctx, base, guest_peer);
  const auto value_address =
      mp64::CheckedGuestAddress(guest_player_info, mp64::kPlayerInfoUniqueValueOffset);
  if (guest_player_info == 0 || !value_address) {
    return std::nullopt;
  }
  return static_cast<int32_t>(REX_LOAD_U32(*value_address));
}

void CopyNetworkEndpointCloneState(uint8_t* base, uint32_t guest_source,
                                   uint32_t guest_destination) {
  if (guest_source == 0 || guest_destination == 0) {
    return;
  }
  for (uint32_t offset : std::array<uint32_t, 3>{0, 1, 2}) {
    const auto source = mp64::CheckedGuestAddress(guest_source, offset);
    const auto destination = mp64::CheckedGuestAddress(guest_destination, offset);
    if (source && destination) {
      REX_STORE_U8(*destination, REX_LOAD_U8(*source));
    }
  }
  const auto source_word = mp64::CheckedGuestAddress(guest_source, mp64::kGuestPointerSize);
  const auto destination_word =
      mp64::CheckedGuestAddress(guest_destination, mp64::kGuestPointerSize);
  if (source_word && destination_word) {
    REX_STORE_U32(*destination_word, REX_LOAD_U32(*source_word));
  }
}

void SyncDispatchLegacyMasks(uint8_t* base, uint32_t guest_dispatch) {
  const auto first = mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchFirstPeerMaskOffset);
  const auto second =
      mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchSecondPeerMaskOffset);
  if (first && second) {
    g_dispatch_masks.ReplaceLegacyLow16(guest_dispatch, REX_LOAD_U16(*first),
                                        REX_LOAD_U16(*second));
  }
}

uint32_t EnsureDispatchMessage(PPCContext& ctx, uint8_t* base, uint32_t guest_network_manager,
                               uint8_t peer_id) {
  if (guest_network_manager == 0 || mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    return 0;
  }
  std::scoped_lock lock(g_dispatch_message_mutex);
  uint32_t& guest_message = g_dispatch_messages[guest_network_manager][peer_id];
  if (guest_message != 0) {
    return guest_message;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return 0;
  }
  guest_message = runtime->memory()->SystemHeapAlloc(mp64::kDispatchMessageStride);
  if (guest_message == 0) {
    return 0;
  }
  const auto guest_queue =
      mp64::CheckedGuestAddress(guest_message, mp64::kDispatchMessageQueueOffset);
  const auto guest_payload =
      mp64::CheckedGuestAddress(guest_message, mp64::kDispatchMessagePayloadOffset);
  if (!guest_queue || !guest_payload) {
    runtime->memory()->SystemHeapFree(guest_message);
    guest_message = 0;
    return 0;
  }
  std::memset(base + guest_message, 0, mp64::kDispatchMessageStride);
  PPCContext initialize_ctx = ctx;
  initialize_ctx.r3.u64 = guest_message;
  initialize_ctx.r4.u64 = 0;
  initialize_ctx.r5.u64 = 0;
  initialize_ctx.r6.u64 = 0;
  initialize_ctx.r7.u64 = 0;
  sub_82707140(initialize_ctx, base);
  initialize_ctx = ctx;
  initialize_ctx.r3.u64 = *guest_queue;
  initialize_ctx.r4.u64 = *guest_payload;
  initialize_ctx.r5.u64 = mp64::kDispatchMessagePayloadCapacity;
  sub_82853400(initialize_ctx, base);
  return guest_message;
}

void ResetDispatchMessage(PPCContext& ctx, uint8_t* base, uint32_t guest_network_manager,
                          uint8_t peer_id) {
  const uint32_t guest_message = EnsureDispatchMessage(ctx, base, guest_network_manager, peer_id);
  const auto guest_queue =
      mp64::CheckedGuestAddress(guest_message, mp64::kDispatchMessageQueueOffset);
  const auto guest_payload =
      mp64::CheckedGuestAddress(guest_message, mp64::kDispatchMessagePayloadOffset);
  if (guest_message == 0 || !guest_queue || !guest_payload) {
    return;
  }
  std::memset(base + guest_message, 0, mp64::kDispatchMessageStride);
  PPCContext initialize_ctx = ctx;
  initialize_ctx.r3.u64 = guest_message;
  initialize_ctx.r4.u64 = 0;
  initialize_ctx.r5.u64 = 0;
  initialize_ctx.r6.u64 = 0;
  initialize_ctx.r7.u64 = 0;
  sub_82707140(initialize_ctx, base);
  initialize_ctx = ctx;
  initialize_ctx.r3.u64 = *guest_queue;
  initialize_ctx.r4.u64 = *guest_payload;
  initialize_ctx.r5.u64 = mp64::kDispatchMessagePayloadCapacity;
  sub_82853400(initialize_ctx, base);
}

void DestroyDispatchMessages(uint32_t guest_network_manager) {
  std::array<uint32_t, mp64::kExtendedPeerCapacity> messages{};
  {
    std::scoped_lock lock(g_dispatch_message_mutex);
    const auto it = g_dispatch_messages.find(guest_network_manager);
    if (it == g_dispatch_messages.end()) {
      return;
    }
    messages = it->second;
    g_dispatch_messages.erase(it);
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return;
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (messages[peer_id] != 0) {
      runtime->memory()->SystemHeapFree(messages[peer_id]);
    }
  }
}

template <typename Callback>
bool WithExtendedDispatchPeer(PPCContext& ctx, uint8_t* base, uint32_t guest_handler,
                              uint32_t guest_peer, uint8_t actual_peer_id, uint32_t guest_message,
                              Callback&& callback) {
  if (guest_handler == 0 || guest_peer == 0 ||
      mp64::ClassifyPeerId(actual_peer_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  std::scoped_lock dispatch_lock(g_dispatch_alias_mutex);
  constexpr uint8_t kAliasPeerId = kTemporaryAliasPeerId;
  constexpr uint32_t kAliasBit = 1;
  const auto element_count_address =
      mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchElementCountOffset);
  const auto state_table_address =
      mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchPeerStatePointerTableOffset);
  const auto pending_table_address =
      mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchPeerMaskPointerTableOffset);
  const auto first_mask_address =
      mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchFirstPeerMaskOffset);
  const auto second_mask_address =
      mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchSecondPeerMaskOffset);
  const auto network_manager_address =
      mp64::CheckedGuestAddress(guest_handler, mp64::kNetworkArrayManagerPointerOffset);
  if (!element_count_address || !state_table_address || !pending_table_address ||
      !first_mask_address || !second_mask_address || !network_manager_address) {
    return false;
  }

  const size_t element_count = REX_LOAD_U32(*element_count_address);
  const uint32_t guest_state_table = REX_LOAD_U32(*state_table_address);
  const uint32_t guest_pending_table = REX_LOAD_U32(*pending_table_address);
  if (element_count != 0 && (guest_state_table == 0 || guest_pending_table == 0)) {
    return false;
  }

  mp64::DispatchPeerState state =
      g_dispatch_peer_states.Get(guest_handler, actual_peer_id, element_count);
  std::vector<std::array<uint8_t, mp64::kDispatchElementStateRecordSize>> saved_states(
      element_count);
  std::vector<uint32_t> saved_pending_words(element_count);
  std::vector<uint32_t> alias_state_addresses(element_count);
  std::vector<uint32_t> alias_pending_addresses(element_count);
  for (size_t element = 0; element < element_count; ++element) {
    const auto state_pointer = mp64::CheckedGuestArrayAddress(
        guest_state_table, element, mp64::kDispatchElementPointerRecordSize);
    const auto pending_pointer = mp64::CheckedGuestArrayAddress(
        guest_pending_table, element, mp64::kDispatchElementPointerRecordSize);
    if (!state_pointer || !pending_pointer) {
      return false;
    }
    const uint32_t guest_peer_states = REX_LOAD_U32(*state_pointer);
    const uint32_t guest_pending_words = REX_LOAD_U32(*pending_pointer);
    const auto alias_state = mp64::CheckedGuestArrayAddress(guest_peer_states, kAliasPeerId,
                                                            mp64::kDispatchElementStateRecordSize);
    if (!alias_state || guest_pending_words == 0) {
      return false;
    }
    alias_state_addresses[element] = *alias_state;
    alias_pending_addresses[element] = guest_pending_words;
    std::memcpy(saved_states[element].data(), base + *alias_state, saved_states[element].size());
    saved_pending_words[element] = REX_LOAD_U32(guest_pending_words);
  }

  SyncDispatchLegacyMasks(base, guest_handler);
  const mp64::PeerMaskPair64 masks = g_dispatch_masks.Get(guest_handler);
  const uint32_t saved_first_mask = REX_LOAD_U32(*first_mask_address);
  const uint32_t saved_second_mask = REX_LOAD_U32(*second_mask_address);
  for (size_t element = 0; element < element_count; ++element) {
    std::memcpy(base + alias_state_addresses[element], state.element_states[element].data(),
                state.element_states[element].size());
    uint32_t pending = saved_pending_words[element] & ~kAliasBit;
    if (state.pending_elements[element] != 0) {
      pending |= kAliasBit;
    }
    REX_STORE_U32(alias_pending_addresses[element], pending);
  }
  REX_STORE_U32(*first_mask_address, (saved_first_mask & ~kAliasBit) |
                                         (masks.first.Contains(actual_peer_id) ? kAliasBit : 0));
  REX_STORE_U32(*second_mask_address, (saved_second_mask & ~kAliasBit) |
                                          (masks.second.Contains(actual_peer_id) ? kAliasBit : 0));

  const DispatchAliasContext saved_context = g_dispatch_alias;
  g_dispatch_alias.active = true;
  g_dispatch_alias.guest_handler = guest_handler;
  g_dispatch_alias.guest_network_manager = REX_LOAD_U32(*network_manager_address);
  g_dispatch_alias.guest_peer = guest_peer;
  g_dispatch_alias.guest_message = guest_message;
  g_dispatch_alias.actual_peer_id = actual_peer_id;
  g_dispatch_alias.alias_peer_id = kAliasPeerId;
  SetAliasOverride(mp64::kGlobalPeerManagerAddress, kAliasPeerId, guest_peer);
  callback(kAliasPeerId);

  for (size_t element = 0; element < element_count; ++element) {
    std::memcpy(state.element_states[element].data(), base + alias_state_addresses[element],
                state.element_states[element].size());
    state.pending_elements[element] =
        (REX_LOAD_U32(alias_pending_addresses[element]) & kAliasBit) != 0;
    std::memcpy(base + alias_state_addresses[element], saved_states[element].data(),
                saved_states[element].size());
    REX_STORE_U32(alias_pending_addresses[element], saved_pending_words[element]);
  }
  g_dispatch_peer_states.Set(guest_handler, actual_peer_id, state);
  g_dispatch_masks.Set(guest_handler, actual_peer_id,
                       (REX_LOAD_U32(*first_mask_address) & kAliasBit) != 0,
                       (REX_LOAD_U32(*second_mask_address) & kAliasBit) != 0);
  if ((REX_LOAD_U32(*first_mask_address) & kAliasBit) == 0) {
    g_dispatch_masks.Reset(guest_handler, actual_peer_id, true, false);
  }
  if ((REX_LOAD_U32(*second_mask_address) & kAliasBit) == 0) {
    g_dispatch_masks.Reset(guest_handler, actual_peer_id, false, true);
  }
  REX_STORE_U32(*first_mask_address, saved_first_mask);
  REX_STORE_U32(*second_mask_address, saved_second_mask);
  ClearAliasOverride(mp64::kGlobalPeerManagerAddress, kAliasPeerId);
  g_dispatch_alias = saved_context;
  return true;
}

template <typename Callback>
bool WithExtendedNetworkPeer(PPCContext& ctx, uint8_t* base, uint32_t guest_object,
                             uint32_t guest_peer, uint8_t actual_peer_id, bool isolate_alias,
                             Callback&& callback) {
  if (guest_object == 0 || guest_peer == 0 ||
      mp64::ClassifyPeerId(actual_peer_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  std::scoped_lock alias_lock(g_network_peer_alias_mutex);
  const auto owner_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectOwnerPeerOffset);
  if (!owner_address) {
    return false;
  }
  const uint8_t alias_id = REX_LOAD_U8(*owner_address) == 0 ? uint8_t{1} : uint8_t{0};
  const auto flags_table =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectPeerFlagsOffset);
  const auto endpoint_table =
      mp64::CheckedGuestAddress(guest_object, mp64::kLegacyNetworkEndpointTableOffset);
  if (!flags_table || !endpoint_table) {
    return false;
  }

  std::array<mp64::NetworkObjectPeerFlags, mp64::kLegacyPeerCapacity> saved_flags{};
  std::array<uint32_t, mp64::kLegacyPeerCapacity> saved_endpoints{};
  std::array<uint32_t, mp64::kLegacyPeerCapacity> flag_addresses{};
  std::array<uint32_t, mp64::kLegacyPeerCapacity> endpoint_addresses{};
  for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
    const auto flags =
        mp64::CheckedGuestArrayAddress(*flags_table, peer_id, mp64::kNetworkObjectPeerFlagsStride);
    const auto endpoint =
        mp64::CheckedGuestArrayAddress(*endpoint_table, peer_id, mp64::kGuestPointerSize);
    if (!flags || !endpoint) {
      return false;
    }
    flag_addresses[peer_id] = *flags;
    endpoint_addresses[peer_id] = *endpoint;
  }
  for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
    std::memcpy(saved_flags[peer_id].data(), base + flag_addresses[peer_id],
                saved_flags[peer_id].size());
    saved_endpoints[peer_id] = REX_LOAD_U32(endpoint_addresses[peer_id]);
    if (isolate_alias) {
      std::memset(base + flag_addresses[peer_id], 0, saved_flags[peer_id].size());
      REX_STORE_U32(endpoint_addresses[peer_id], 0);
    }
  }
  const uint32_t alias_flags = flag_addresses[alias_id];
  const uint32_t alias_endpoint = endpoint_addresses[alias_id];
  const auto guest_vtable_address = mp64::CheckedGuestAddress(guest_object, 0);
  const uint32_t guest_vtable = guest_vtable_address ? REX_LOAD_U32(*guest_vtable_address) : 0;
  const auto blender_factory_address =
      mp64::CheckedGuestAddress(guest_vtable, mp64::kNetworkObjectBlenderFactoryVtableOffset);
  const bool project_ped_state =
      blender_factory_address &&
      REX_LOAD_U32(*blender_factory_address) == mp64::kPedNetworkBlenderFactoryAddress;
  const auto ped_state_table = project_ped_state
                                   ? mp64::CheckedGuestAddress(
                                         guest_object, mp64::kPedNetworkPeerStateOffset)
                                   : std::nullopt;
  const auto alias_ped_state =
      ped_state_table
          ? mp64::CheckedGuestArrayAddress(*ped_state_table, alias_id,
                                           mp64::kPedNetworkPeerStateStride)
          : std::nullopt;
  if (project_ped_state && !alias_ped_state) {
    return false;
  }
  mp64::PedNetworkPeerState saved_ped_state{};
  if (alias_ped_state) {
    std::memcpy(saved_ped_state.data(), base + *alias_ped_state, saved_ped_state.size());
    const mp64::PedNetworkPeerState extended_ped_state =
        g_ped_network_peer_states.Get(guest_object, actual_peer_id);
    std::memcpy(base + *alias_ped_state, extended_ped_state.data(), extended_ped_state.size());
  }
  const mp64::NetworkObjectPeerFlags extended_flags =
      g_network_peer_flags.Get(guest_object, actual_peer_id);
  std::memcpy(base + alias_flags, extended_flags.data(), extended_flags.size());
  REX_STORE_U32(alias_endpoint, g_network_endpoints.Get(guest_object, actual_peer_id));

  const NetworkPeerAliasContext saved_context = g_network_peer_alias;
  g_network_peer_alias.active = true;
  g_network_peer_alias.guest_object = guest_object;
  g_network_peer_alias.guest_peer = guest_peer;
  g_network_peer_alias.actual_peer_id = actual_peer_id;
  g_network_peer_alias.alias_peer_id = alias_id;
  const auto saved_override =
      GetAliasOverride(mp64::kGlobalPeerManagerAddress, alias_id);
  SetAliasOverride(mp64::kGlobalPeerManagerAddress, alias_id, guest_peer);
  callback(alias_id);

  mp64::NetworkObjectPeerFlags captured_flags{};
  std::memcpy(captured_flags.data(), base + alias_flags, captured_flags.size());
  g_network_peer_flags.Set(guest_object, actual_peer_id, captured_flags);
  if (alias_ped_state) {
    mp64::PedNetworkPeerState captured_ped_state{};
    std::memcpy(captured_ped_state.data(), base + *alias_ped_state,
                captured_ped_state.size());
    g_ped_network_peer_states.Set(guest_object, actual_peer_id, captured_ped_state);
    std::memcpy(base + *alias_ped_state, saved_ped_state.data(), saved_ped_state.size());
  }
  // Endpoint ownership is maintained only by the hooked factory/destructor
  // pair. Never import a raw pointer from the projected retail slot: the
  // original function may have replaced it with storage owned by a fixed
  // retail pool, which is not safe to release through SystemHeapFree.
  for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
    std::memcpy(base + flag_addresses[peer_id], saved_flags[peer_id].data(),
                saved_flags[peer_id].size());
    REX_STORE_U32(endpoint_addresses[peer_id], saved_endpoints[peer_id]);
  }
  if (saved_override) {
    SetAliasOverride(mp64::kGlobalPeerManagerAddress, alias_id, *saved_override);
  } else {
    ClearAliasOverride(mp64::kGlobalPeerManagerAddress, alias_id);
  }
  g_network_peer_alias = saved_context;
  return true;
}

template <typename Callback>
bool WithNetworkEndpointVirtualPeer(PPCContext& ctx, uint8_t* base, uint32_t guest_object,
                                    uint8_t actual_peer_id, Callback&& callback) {
  if (g_network_peer_alias.active && g_network_peer_alias.guest_object == guest_object &&
      g_network_peer_alias.actual_peer_id == actual_peer_id) {
    callback(g_network_peer_alias.alias_peer_id);
    return true;
  }
  const uint32_t guest_peer =
      g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, actual_peer_id);
  return guest_peer != 0 &&
         WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, actual_peer_id, false,
                                 std::forward<Callback>(callback));
}

mp64::EventPeerBuffers EnsureEventPeerBuffers(PPCContext& ctx, uint8_t* base,
                                              uint32_t guest_event_manager, uint8_t peer_id) {
  mp64::EventPeerBuffers buffers = g_event_peer_buffers.Get(guest_event_manager, peer_id);
  if (buffers.guest_outbound != 0 && buffers.guest_inbound != 0) {
    return buffers;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return {};
  }
  buffers.guest_outbound = runtime->memory()->SystemHeapAlloc(mp64::kEventPeerOutboundBufferSize);
  buffers.guest_inbound = runtime->memory()->SystemHeapAlloc(mp64::kEventPeerInboundBufferSize);
  if (buffers.guest_outbound == 0 || buffers.guest_inbound == 0) {
    if (buffers.guest_outbound != 0) {
      runtime->memory()->SystemHeapFree(buffers.guest_outbound);
    }
    if (buffers.guest_inbound != 0) {
      runtime->memory()->SystemHeapFree(buffers.guest_inbound);
    }
    return {};
  }
  PPCContext constructor_ctx = ctx;
  constructor_ctx.r3.u64 = buffers.guest_outbound;
  __imp__sub_82794088(constructor_ctx, base);
  constructor_ctx = ctx;
  constructor_ctx.r3.u64 = buffers.guest_inbound;
  __imp__sub_827941F0(constructor_ctx, base);
  if (!g_event_peer_buffers.Set(guest_event_manager, peer_id, buffers)) {
    runtime->memory()->SystemHeapFree(buffers.guest_outbound);
    runtime->memory()->SystemHeapFree(buffers.guest_inbound);
    return {};
  }
  return buffers;
}

void ReleaseEventPeerBuffers(PPCContext& ctx, uint8_t* base, uint32_t guest_event_manager) {
  rex::Runtime* runtime = rex::Runtime::instance();
  const auto buffers = g_event_peer_buffers.RemoveManager(guest_event_manager);
  if (runtime == nullptr) {
    return;
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (buffers[peer_id].guest_outbound != 0) {
      PPCContext reset_ctx = ctx;
      reset_ctx.r3.u64 = buffers[peer_id].guest_outbound;
      __imp__sub_82793FC8(reset_ctx, base);
      runtime->memory()->SystemHeapFree(buffers[peer_id].guest_outbound);
    }
    if (buffers[peer_id].guest_inbound != 0) {
      PPCContext reset_ctx = ctx;
      reset_ctx.r3.u64 = buffers[peer_id].guest_inbound;
      __imp__sub_82794008(reset_ctx, base);
      runtime->memory()->SystemHeapFree(buffers[peer_id].guest_inbound);
    }
  }
}

void ReleaseEventPeerBuffers(PPCContext& ctx, uint8_t* base, uint32_t guest_event_manager,
                             uint8_t peer_id) {
  rex::Runtime* runtime = rex::Runtime::instance();
  const mp64::EventPeerBuffers buffers =
      g_event_peer_buffers.RemovePeer(guest_event_manager, peer_id);
  if (runtime == nullptr) {
    return;
  }
  if (buffers.guest_outbound != 0) {
    PPCContext reset_ctx = ctx;
    reset_ctx.r3.u64 = buffers.guest_outbound;
    __imp__sub_82793FC8(reset_ctx, base);
    runtime->memory()->SystemHeapFree(buffers.guest_outbound);
  }
  if (buffers.guest_inbound != 0) {
    PPCContext reset_ctx = ctx;
    reset_ctx.r3.u64 = buffers.guest_inbound;
    __imp__sub_82794008(reset_ctx, base);
    runtime->memory()->SystemHeapFree(buffers.guest_inbound);
  }
}

template <typename Callback>
bool WithExtendedEventPeer(PPCContext& ctx, uint8_t* base, uint32_t guest_event_manager,
                           uint32_t guest_peer, uint8_t peer_id, Callback&& callback) {
  if (guest_event_manager == 0 || guest_peer == 0 ||
      mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  std::scoped_lock alias_lock(g_event_peer_alias_mutex);
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_event_manager, mp64::kEventPeerManagerPointerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  const mp64::EventPeerBuffers buffers =
      EnsureEventPeerBuffers(ctx, base, guest_event_manager, peer_id);
  if (guest_peer_manager == 0 || buffers.guest_outbound == 0 || buffers.guest_inbound == 0) {
    return false;
  }
  uint8_t alias_id = kTemporaryAliasPeerId;
  for (uint8_t candidate = 0; candidate < mp64::kLegacyPeerCapacity; ++candidate) {
    if (g_peer_managers.GetPeer(guest_peer_manager, candidate) == 0) {
      alias_id = candidate;
      break;
    }
  }
  const auto outbound_table =
      mp64::CheckedGuestAddress(guest_event_manager, mp64::kEventPeerOutboundTableOffset);
  const auto inbound_table =
      mp64::CheckedGuestAddress(guest_event_manager, mp64::kEventPeerInboundTableOffset);
  const auto alias_outbound =
      outbound_table ? mp64::CheckedGuestArrayAddress(*outbound_table, alias_id,
                                                      mp64::kEventPeerOutboundBufferSize)
                     : std::nullopt;
  const auto alias_inbound = inbound_table
                                 ? mp64::CheckedGuestArrayAddress(*inbound_table, alias_id,
                                                                  mp64::kEventPeerInboundBufferSize)
                                 : std::nullopt;
  if (!alias_outbound || !alias_inbound) {
    return false;
  }
  std::array<uint8_t, mp64::kEventPeerOutboundBufferSize> saved_outbound{};
  std::array<uint8_t, mp64::kEventPeerInboundBufferSize> saved_inbound{};
  std::memcpy(saved_outbound.data(), base + *alias_outbound, saved_outbound.size());
  std::memcpy(saved_inbound.data(), base + *alias_inbound, saved_inbound.size());
  std::memcpy(base + *alias_outbound, base + buffers.guest_outbound, saved_outbound.size());
  std::memcpy(base + *alias_inbound, base + buffers.guest_inbound, saved_inbound.size());

  const EventPeerAliasContext saved_context = g_event_peer_alias;
  g_event_peer_alias = {
      .active = true,
      .guest_event_manager = guest_event_manager,
      .guest_peer_manager = guest_peer_manager,
      .guest_peer = guest_peer,
      .actual_peer_id = peer_id,
      .alias_peer_id = alias_id,
  };
  SetAliasOverride(guest_peer_manager, alias_id, guest_peer);
  callback(alias_id);
  std::memcpy(base + buffers.guest_outbound, base + *alias_outbound, saved_outbound.size());
  std::memcpy(base + buffers.guest_inbound, base + *alias_inbound, saved_inbound.size());
  std::memcpy(base + *alias_outbound, saved_outbound.data(), saved_outbound.size());
  std::memcpy(base + *alias_inbound, saved_inbound.data(), saved_inbound.size());
  ClearAliasOverride(guest_peer_manager, alias_id);
  g_event_peer_alias = saved_context;
  return true;
}

void ClearObjectPeerQueue(PPCContext& ctx, uint8_t* base, uint32_t guest_queue) {
  const auto count = mp64::CheckedGuestAddress(guest_queue, mp64::kObjectPeerQueueCountOffset);
  while (count && REX_LOAD_U32(*count) != 0) {
    const uint32_t guest_node = REX_LOAD_U32(guest_queue);
    if (guest_node == 0) {
      break;
    }
    PPCContext clear_ctx = ctx;
    clear_ctx.r3.u64 = guest_queue;
    clear_ctx.r4.u64 = guest_node;
    __imp__sub_826EC350(clear_ctx, base);
  }
}

mp64::ObjectManagerPeerBuffers EnsureObjectPeerBuffers(PPCContext& ctx, uint8_t* base,
                                                       uint32_t guest_object_manager,
                                                       uint8_t peer_id) {
  mp64::ObjectManagerPeerBuffers buffers = g_object_peer_buffers.Get(guest_object_manager, peer_id);
  if (buffers.guest_message != 0 && buffers.guest_sync_ack != 0 && buffers.guest_reliable != 0 &&
      buffers.guest_queue != 0) {
    return buffers;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return {};
  }
  buffers.guest_message = runtime->memory()->SystemHeapAlloc(mp64::kObjectPeerMessageSize);
  buffers.guest_sync_ack = runtime->memory()->SystemHeapAlloc(mp64::kObjectPeerSyncAckSize);
  buffers.guest_reliable = runtime->memory()->SystemHeapAlloc(mp64::kObjectPeerReliableSize);
  buffers.guest_queue = runtime->memory()->SystemHeapAlloc(mp64::kObjectPeerQueueSize);
  if (buffers.guest_message == 0 || buffers.guest_sync_ack == 0 || buffers.guest_reliable == 0 ||
      buffers.guest_queue == 0) {
    if (buffers.guest_message != 0) {
      runtime->memory()->SystemHeapFree(buffers.guest_message);
    }
    if (buffers.guest_sync_ack != 0) {
      runtime->memory()->SystemHeapFree(buffers.guest_sync_ack);
    }
    if (buffers.guest_reliable != 0) {
      runtime->memory()->SystemHeapFree(buffers.guest_reliable);
    }
    if (buffers.guest_queue != 0) {
      runtime->memory()->SystemHeapFree(buffers.guest_queue);
    }
    return {};
  }
  std::memset(base + buffers.guest_message, 0, mp64::kObjectPeerMessageSize);
  PPCContext constructor_ctx = ctx;
  constructor_ctx.r3.u64 = buffers.guest_message + mp64::kObjectPeerMessageQueueOffset;
  __imp__sub_828534E0(constructor_ctx, base);
  constructor_ctx = ctx;
  constructor_ctx.r3.u64 = buffers.guest_message + mp64::kObjectPeerMessageQueueOffset;
  constructor_ctx.r4.u64 = buffers.guest_message + mp64::kObjectPeerMessagePayloadOffset;
  constructor_ctx.r5.u64 = mp64::kObjectPeerMessagePayloadCapacity;
  __imp__sub_82853400(constructor_ctx, base);
  constructor_ctx = ctx;
  constructor_ctx.r3.u64 = buffers.guest_sync_ack;
  __imp__sub_82795618(constructor_ctx, base);
  constructor_ctx = ctx;
  constructor_ctx.r3.u64 = buffers.guest_reliable;
  __imp__sub_82795348(constructor_ctx, base);
  std::memset(base + buffers.guest_queue, 0, mp64::kObjectPeerQueueSize);
  if (!g_object_peer_buffers.Set(guest_object_manager, peer_id, buffers)) {
    runtime->memory()->SystemHeapFree(buffers.guest_message);
    runtime->memory()->SystemHeapFree(buffers.guest_sync_ack);
    runtime->memory()->SystemHeapFree(buffers.guest_reliable);
    runtime->memory()->SystemHeapFree(buffers.guest_queue);
    return {};
  }
  return buffers;
}

void FreeObjectPeerBuffers(PPCContext& ctx, uint8_t* base, mp64::ObjectManagerPeerBuffers buffers) {
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return;
  }
  if (buffers.guest_message != 0) {
    PPCContext reset_ctx = ctx;
    reset_ctx.r3.u64 = buffers.guest_message + mp64::kObjectPeerMessageQueueOffset;
    reset_ctx.r4.u64 = buffers.guest_message + mp64::kObjectPeerMessagePayloadOffset;
    reset_ctx.r5.u64 = mp64::kObjectPeerMessagePayloadCapacity;
    __imp__sub_82853400(reset_ctx, base);
    runtime->memory()->SystemHeapFree(buffers.guest_message);
  }
  if (buffers.guest_sync_ack != 0) {
    PPCContext reset_ctx = ctx;
    reset_ctx.r3.u64 = buffers.guest_sync_ack;
    __imp__sub_82795298(reset_ctx, base);
    runtime->memory()->SystemHeapFree(buffers.guest_sync_ack);
  }
  if (buffers.guest_reliable != 0) {
    PPCContext reset_ctx = ctx;
    reset_ctx.r3.u64 = buffers.guest_reliable;
    __imp__sub_82795438(reset_ctx, base);
    runtime->memory()->SystemHeapFree(buffers.guest_reliable);
  }
  if (buffers.guest_queue != 0) {
    ClearObjectPeerQueue(ctx, base, buffers.guest_queue);
    runtime->memory()->SystemHeapFree(buffers.guest_queue);
  }
}

void ReleaseObjectPeerBuffers(PPCContext& ctx, uint8_t* base, uint32_t guest_object_manager) {
  const auto buffers = g_object_peer_buffers.RemoveManager(guest_object_manager);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    FreeObjectPeerBuffers(ctx, base, buffers[peer_id]);
  }
}

std::optional<uint32_t> LegacyObjectOwnerListAddress(uint32_t guest_object_manager,
                                                     uint8_t owner_id) {
  if (!mp64::IsLegacyPeerId(owner_id)) {
    return std::nullopt;
  }
  const auto table =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectOwnerListTableOffset);
  return table ? mp64::CheckedGuestArrayAddress(*table, owner_id, mp64::kObjectOwnerListHeaderSize)
               : std::nullopt;
}

std::optional<uint32_t> LegacyObjectPeerMatrixAddress(uint32_t guest_object_manager,
                                                      uint16_t object_id, uint8_t peer_id) {
  if (!mp64::IsLegacyPeerId(peer_id) || object_id == 0 ||
      object_id >= mp64::kObjectPeerMatrixObjectCapacity) {
    return std::nullopt;
  }
  const auto table =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerMatrixTableOffset);
  const auto row = table ? mp64::CheckedGuestArrayAddress(*table, object_id,
                                                          mp64::kObjectPeerMatrixLegacyRowSize)
                         : std::nullopt;
  return row ? mp64::CheckedGuestArrayAddress(*row, peer_id, sizeof(uint32_t)) : std::nullopt;
}

mp64::ObjectOwnerList ObjectOwnerListFor(uint8_t* base, uint32_t guest_object_manager,
                                         uint8_t owner_id) {
  if (mp64::IsLegacyPeerId(owner_id)) {
    const auto list = LegacyObjectOwnerListAddress(guest_object_manager, owner_id);
    return list ? mp64::ObjectOwnerList{REX_LOAD_U32(*list),
                                        REX_LOAD_U32(*list + mp64::kGuestPointerSize)}
                : mp64::ObjectOwnerList{};
  }
  return g_object_owner_lists.Get(guest_object_manager, owner_id);
}

uint16_t NetworkObjectId(const PPCContext& ctx, uint8_t* base, uint32_t guest_object) {
  if (guest_object == 0) {
    return 0;
  }
  PPCContext id_ctx = ctx;
  id_ctx.r3.u64 = guest_object;
  sub_82701F50(id_ctx, base);
  return id_ctx.r3.u16;
}

uint8_t NetworkObjectOwner(const PPCContext& ctx, uint8_t* base, uint32_t guest_object) {
  if (guest_object == 0) {
    return mp64::kInvalidPeerId;
  }
  PPCContext owner_ctx = ctx;
  owner_ctx.r3.u64 = guest_object;
  __imp__sub_82701F58(owner_ctx, base);
  return owner_ctx.r3.u8;
}

std::optional<uint8_t> ObjectManagerLocalPeerId(const PPCContext& ctx, uint8_t* base,
                                                uint32_t guest_object_manager) {
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  if (guest_peer_manager == 0) {
    return std::nullopt;
  }
  PPCContext local_ctx = ctx;
  local_ctx.r3.u64 = guest_peer_manager;
  __imp__sub_826FDD68(local_ctx, base);
  return PeerRecordId(base, local_ctx.r3.u32);
}

uint8_t ObjectManagerTimingPeerId(uint32_t guest_object_manager, uint8_t peer_id) noexcept {
  if (g_object_recovery_peer_alias.active &&
      guest_object_manager == g_object_recovery_peer_alias.guest_object_manager &&
      peer_id == g_object_recovery_peer_alias.alias_peer_id) {
    return g_object_recovery_peer_alias.actual_peer_id;
  }
  if (g_object_peer_alias.active &&
      guest_object_manager == g_object_peer_alias.guest_object_manager &&
      peer_id == g_object_peer_alias.alias_peer_id) {
    return g_object_peer_alias.actual_peer_id;
  }
  return peer_id;
}

bool IsObjectRecoveryPeerEligible(const PPCContext& input_ctx, uint8_t* base,
                                  uint32_t guest_object_manager, uint32_t guest_peer,
                                  uint8_t peer_id) {
  if (guest_peer == 0 || !mp64::IsValidPeerId(peer_id)) {
    return false;
  }
  PPCContext predicate_ctx = input_ctx;
  predicate_ctx.r3.u64 = guest_peer;
  sub_82708620(predicate_ctx, base);
  if (predicate_ctx.r3.u8 == 0) {
    return false;
  }
  predicate_ctx = input_ctx;
  predicate_ctx.r3.u64 = guest_peer;
  sub_82708770(predicate_ctx, base);
  if (predicate_ctx.r3.u8 != 0) {
    return false;
  }
  predicate_ctx = input_ctx;
  predicate_ctx.r3.u64 = guest_object_manager;
  predicate_ctx.r4.u64 = peer_id;
  predicate_ctx.r5.u64 = 0;
  sub_826E8308(predicate_ctx, base);
  return predicate_ctx.r3.u8 != 0;
}

bool ReadPlayerTickState(uint8_t* base, uint32_t guest_manager, uint8_t peer_id,
                         mp64::PlayerTickState& state) {
  const auto first_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickFirstByteArrayOffset);
  const auto second_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickSecondByteArrayOffset);
  const auto eight_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickEightByteTableOffset);
  const auto four_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickFourByteTableOffset);
  const auto pointer_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickPointerTableOffset);
  const auto large_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickLargeRecordOffset);
  const auto first = first_table
                         ? mp64::CheckedGuestArrayAddress(*first_table, peer_id, sizeof(uint8_t))
                         : std::nullopt;
  const auto second = second_table
                          ? mp64::CheckedGuestArrayAddress(*second_table, peer_id, sizeof(uint8_t))
                          : std::nullopt;
  const auto eight = eight_table ? mp64::CheckedGuestArrayAddress(*eight_table, peer_id,
                                                                  state.eight_byte_record.size())
                                 : std::nullopt;
  const auto four = four_table ? mp64::CheckedGuestArrayAddress(*four_table, peer_id,
                                                                state.four_byte_record.size())
                               : std::nullopt;
  const auto pointer = pointer_table ? mp64::CheckedGuestArrayAddress(*pointer_table, peer_id,
                                                                      state.pointer_record.size())
                                     : std::nullopt;
  const auto large =
      large_table ? mp64::CheckedGuestArrayAddress(*large_table, peer_id, state.large_record.size())
                  : std::nullopt;
  if (!first || !second || !eight || !four || !pointer || !large) {
    return false;
  }
  state.first_byte = REX_LOAD_U8(*first);
  state.second_byte = REX_LOAD_U8(*second);
  std::memcpy(state.eight_byte_record.data(), base + *eight, state.eight_byte_record.size());
  std::memcpy(state.four_byte_record.data(), base + *four, state.four_byte_record.size());
  std::memcpy(state.pointer_record.data(), base + *pointer, state.pointer_record.size());
  std::memcpy(state.large_record.data(), base + *large, state.large_record.size());
  return true;
}

bool WritePlayerTickState(uint8_t* base, uint32_t guest_manager, uint8_t peer_id,
                          const mp64::PlayerTickState& state) {
  const auto first_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickFirstByteArrayOffset);
  const auto second_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickSecondByteArrayOffset);
  const auto eight_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickEightByteTableOffset);
  const auto four_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickFourByteTableOffset);
  const auto pointer_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickPointerTableOffset);
  const auto large_table =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPlayerTickLargeRecordOffset);
  const auto first = first_table
                         ? mp64::CheckedGuestArrayAddress(*first_table, peer_id, sizeof(uint8_t))
                         : std::nullopt;
  const auto second = second_table
                          ? mp64::CheckedGuestArrayAddress(*second_table, peer_id, sizeof(uint8_t))
                          : std::nullopt;
  const auto eight = eight_table ? mp64::CheckedGuestArrayAddress(*eight_table, peer_id,
                                                                  state.eight_byte_record.size())
                                 : std::nullopt;
  const auto four = four_table ? mp64::CheckedGuestArrayAddress(*four_table, peer_id,
                                                                state.four_byte_record.size())
                               : std::nullopt;
  const auto pointer = pointer_table ? mp64::CheckedGuestArrayAddress(*pointer_table, peer_id,
                                                                      state.pointer_record.size())
                                     : std::nullopt;
  const auto large =
      large_table ? mp64::CheckedGuestArrayAddress(*large_table, peer_id, state.large_record.size())
                  : std::nullopt;
  if (!first || !second || !eight || !four || !pointer || !large) {
    return false;
  }
  REX_STORE_U8(*first, state.first_byte);
  REX_STORE_U8(*second, state.second_byte);
  std::memcpy(base + *eight, state.eight_byte_record.data(), state.eight_byte_record.size());
  std::memcpy(base + *four, state.four_byte_record.data(), state.four_byte_record.size());
  std::memcpy(base + *pointer, state.pointer_record.data(), state.pointer_record.size());
  std::memcpy(base + *large, state.large_record.data(), state.large_record.size());
  return true;
}

template <typename Callback>
bool WithExtendedPlayerTickAlias(uint8_t* base, uint32_t guest_manager, uint8_t peer_id,
                                 Callback&& callback) {
  if (guest_manager == 0 || mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  const auto player_info = PlayerInfoForId(base, peer_id);
  const uint32_t guest_player_info = player_info.guest_player_info;
  if (guest_player_info == 0) {
    return false;
  }
  std::scoped_lock alias_lock(g_player_tick_alias_mutex);
  mp64::PlayerTickState saved_alias;
  if (!ReadPlayerTickState(base, guest_manager, kTemporaryAliasPeerId, saved_alias)) {
    return false;
  }
  const mp64::PlayerTickState high_state = g_player_tick_states.Get(guest_manager, peer_id);
  if (!WritePlayerTickState(base, guest_manager, kTemporaryAliasPeerId, high_state)) {
    return false;
  }
  const PlayerTickAliasContext saved_context = g_player_tick_alias;
  g_player_tick_alias = {
      .active = true,
      .guest_manager = guest_manager,
      .guest_player_info = guest_player_info,
      .actual_peer_id = peer_id,
      .alias_peer_id = kTemporaryAliasPeerId,
  };
  callback(kTemporaryAliasPeerId);
  mp64::PlayerTickState captured;
  if (ReadPlayerTickState(base, guest_manager, kTemporaryAliasPeerId, captured)) {
    g_player_tick_states.Set(guest_manager, peer_id, captured);
  }
  WritePlayerTickState(base, guest_manager, kTemporaryAliasPeerId, saved_alias);
  g_player_tick_alias = saved_context;
  return true;
}

void InjectExtendedProximityWeights(PPCContext& ctx, uint8_t* base) {
  auto player_info_read = g_player_info_alias_gate.EnterRead();
  if (!g_proximity_weight.active || g_proximity_weight.extended_injected) {
    return;
  }
  g_proximity_weight.extended_injected = true;

  PPCContext local_ctx = ctx;
  local_ctx.r3.u64 = 0;
  sub_82238C28(local_ctx, base);
  const uint32_t guest_local_player = local_ctx.r3.u32;
  const auto local_transform_address =
      mp64::CheckedGuestPointerAddress(guest_local_player,
                                       mp64::kProximityWeightLocalTransformOffset);
  const uint32_t guest_local_transform =
      local_transform_address ? REX_LOAD_U32(*local_transform_address) : 0;
  const auto local_position_address =
      mp64::CheckedGuestPointerAddress(guest_local_transform,
                                       mp64::kProximityWeightPositionOffset);
  if (!local_position_address) {
    return;
  }
  std::array<float, 3> local_position{};
  for (size_t component = 0; component < local_position.size(); ++component) {
    const auto component_address =
        mp64::CheckedGuestArrayAddress(*local_position_address, component, sizeof(float));
    if (!component_address) {
      return;
    }
    local_position[component] = std::bit_cast<float>(REX_LOAD_U32(*component_address));
  }
  const std::array<float, 4> thresholds = {
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightFirstThresholdAddress)),
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightSecondThresholdAddress)),
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightFirstThresholdAddress)),
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightFourthThresholdAddress)),
  };
  const std::array<float, 4> slopes = {
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightFirstSlopeAddress)),
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightSecondSlopeAddress)),
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightFirstSlopeAddress)),
      std::bit_cast<float>(REX_LOAD_U32(mp64::kProximityWeightFourthSlopeAddress)),
  };
  std::array<float, 4> sums{};
  std::array<uint32_t, 4> counts{};
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_player_info = g_player_infos.Get(peer_id).guest_player_info;
    const auto peer_address =
        mp64::CheckedGuestPointerAddress(guest_player_info,
                                         mp64::kProximityWeightPeerPointerOffset);
    const uint32_t guest_peer = peer_address ? REX_LOAD_U32(*peer_address) : 0;
    if (guest_peer == 0) {
      continue;
    }
    PPCContext predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    sub_82708620(predicate_ctx, base);
    if (predicate_ctx.r3.u8 == 0) {
      continue;
    }
    predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    sub_82708770(predicate_ctx, base);
    if (predicate_ctx.r3.u8 != 0) {
      continue;
    }
    const auto player_address =
        mp64::CheckedGuestPointerAddress(guest_player_info,
                                         mp64::kPlayerInfoPlayerPointerOffset);
    const uint32_t guest_player = player_address ? REX_LOAD_U32(*player_address) : 0;
    const auto transform_address =
        mp64::CheckedGuestPointerAddress(guest_player,
                                         mp64::kProximityWeightLocalTransformOffset);
    const uint32_t guest_transform = transform_address ? REX_LOAD_U32(*transform_address) : 0;
    const auto position_address =
        mp64::CheckedGuestPointerAddress(guest_transform,
                                         mp64::kProximityWeightPositionOffset);
    if (!position_address) {
      continue;
    }
    std::array<float, 3> delta{};
    bool valid_position = true;
    for (size_t component = 0; component < delta.size(); ++component) {
      const auto component_address =
          mp64::CheckedGuestArrayAddress(*position_address, component, sizeof(float));
      if (!component_address) {
        valid_position = false;
        break;
      }
      delta[component] =
          std::bit_cast<float>(REX_LOAD_U32(*component_address)) - local_position[component];
    }
    if (!valid_position) {
      continue;
    }
    const float distance =
        std::sqrt(delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]);
    for (size_t channel = 0; channel < sums.size(); ++channel) {
      const float weight =
          mp64::LinearProximityWeight(distance, thresholds[channel], slopes[channel]);
      sums[channel] += weight;
      if (weight > 0.0f) {
        ++counts[channel];
      }
    }
  }

  const auto stack_weights =
      mp64::CheckedGuestAddress(ctx.r1.u32, mp64::kProximityWeightStackOffset);
  if (!stack_weights) {
    return;
  }
  for (size_t channel = 0; channel < sums.size(); ++channel) {
    const auto weight_address =
        mp64::CheckedGuestArrayAddress(*stack_weights, channel, sizeof(float));
    if (!weight_address) {
      return;
    }
    const float low_weight = std::bit_cast<float>(REX_LOAD_U32(*weight_address));
    REX_STORE_U32(*weight_address, std::bit_cast<uint32_t>(low_weight + sums[channel]));
  }
  ctx.r25.u64 += counts[0];
  ctx.r24.u64 += counts[1];
  ctx.r23.u64 += counts[2];
  ctx.r22.u64 += counts[3];
}

template <typename Callback>
bool WithExtendedObjectOwnerList(PPCContext& ctx, uint8_t* base, uint32_t guest_object_manager,
                                 uint8_t owner_id, Callback&& callback);

template <typename Callback>
bool WithObjectManagerLocalOwnerList(PPCContext& ctx, uint8_t* base, uint32_t guest_object_manager,
                                     Callback&& callback) {
  const auto local_peer_id = ObjectManagerLocalPeerId(ctx, base, guest_object_manager);
  if (!local_peer_id || !mp64::IsValidPeerId(*local_peer_id)) {
    return false;
  }
  if (mp64::IsLegacyPeerId(*local_peer_id)) {
    callback(*local_peer_id);
    return true;
  }
  return WithExtendedObjectOwnerList(ctx, base, guest_object_manager, *local_peer_id,
                                     std::forward<Callback>(callback));
}

template <typename Callback>
void ForEachObjectInOwnerList(uint8_t* base, mp64::ObjectOwnerList list, Callback&& callback) {
  uint32_t guest_node = list.guest_head;
  uint32_t visited = 0;
  while (guest_node != 0 && visited < mp64::kObjectPeerMatrixObjectCapacity) {
    const auto object_address =
        mp64::CheckedGuestAddress(guest_node, mp64::kObjectOwnerListNodeObjectOffset);
    const auto next_address =
        mp64::CheckedGuestAddress(guest_node, mp64::kObjectOwnerListNodeNextOffset);
    if (!object_address || !next_address) {
      break;
    }
    const uint32_t guest_object = REX_LOAD_U32(*object_address);
    const uint32_t guest_next = REX_LOAD_U32(*next_address);
    if (guest_object != 0) {
      callback(guest_object);
    }
    guest_node = guest_next;
    ++visited;
  }
}

template <typename Callback>
bool WithExtendedObjectOwnerList(PPCContext& ctx, uint8_t* base, uint32_t guest_object_manager,
                                 uint8_t owner_id, Callback&& callback) {
  if (mp64::ClassifyPeerId(owner_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  std::scoped_lock alias_lock(g_object_peer_alias_mutex);
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, owner_id);
  uint8_t alias_id = kTemporaryAliasPeerId;
  for (uint8_t candidate = 0; candidate < mp64::kLegacyPeerCapacity; ++candidate) {
    const auto candidate_list = LegacyObjectOwnerListAddress(guest_object_manager, candidate);
    if (candidate_list && REX_LOAD_U32(*candidate_list) == 0 &&
        g_peer_managers.GetPeer(guest_peer_manager, candidate) == 0) {
      alias_id = candidate;
      break;
    }
  }
  const auto alias_list = LegacyObjectOwnerListAddress(guest_object_manager, alias_id);
  if (!alias_list) {
    return false;
  }
  std::array<uint8_t, mp64::kObjectOwnerListHeaderSize> saved_list{};
  std::memcpy(saved_list.data(), base + *alias_list, saved_list.size());
  const mp64::ObjectOwnerList extended_list =
      g_object_owner_lists.Get(guest_object_manager, owner_id);
  REX_STORE_U32(*alias_list, extended_list.guest_head);
  REX_STORE_U32(*alias_list + mp64::kGuestPointerSize, extended_list.guest_tail);
  const ObjectOwnerListAliasContext saved_context = g_object_owner_list_alias;
  g_object_owner_list_alias = {
      .active = true,
      .guest_object_manager = guest_object_manager,
      .actual_owner_id = owner_id,
      .alias_owner_id = alias_id,
  };
  const auto saved_override = GetAliasOverride(guest_peer_manager, alias_id);
  if (guest_peer != 0) {
    SetAliasOverride(guest_peer_manager, alias_id, guest_peer);
  }
  callback(alias_id);
  g_object_owner_lists.Set(
      guest_object_manager, owner_id,
      {REX_LOAD_U32(*alias_list), REX_LOAD_U32(*alias_list + mp64::kGuestPointerSize)});
  std::memcpy(base + *alias_list, saved_list.data(), saved_list.size());
  if (saved_override) {
    SetAliasOverride(guest_peer_manager, alias_id, *saved_override);
  } else {
    ClearAliasOverride(guest_peer_manager, alias_id);
  }
  g_object_owner_list_alias = saved_context;
  return true;
}

template <typename Callback>
bool WithExtendedObjectMatrixCell(uint8_t* base, uint32_t guest_object_manager, uint16_t object_id,
                                  uint8_t peer_id, uint8_t alias_id, Callback&& callback) {
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  const auto alias_cell = LegacyObjectPeerMatrixAddress(guest_object_manager, object_id, alias_id);
  if (!alias_cell) {
    return false;
  }
  const uint32_t saved_value = REX_LOAD_U32(*alias_cell);
  REX_STORE_U32(*alias_cell, g_object_peer_matrix.Get(guest_object_manager, object_id, peer_id));
  callback();
  g_object_peer_matrix.Set(guest_object_manager, object_id, peer_id, REX_LOAD_U32(*alias_cell));
  REX_STORE_U32(*alias_cell, saved_value);
  return true;
}

template <typename Callback>
bool WithExtendedObjectManagerPeer(PPCContext& ctx, uint8_t* base, uint32_t guest_object_manager,
                                   uint8_t peer_id, Callback&& callback) {
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  std::scoped_lock alias_lock(g_object_peer_alias_mutex);
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
  mp64::ObjectManagerPeerBuffers buffers =
      EnsureObjectPeerBuffers(ctx, base, guest_object_manager, peer_id);
  if (guest_peer == 0 || buffers.guest_message == 0 || buffers.guest_sync_ack == 0 ||
      buffers.guest_reliable == 0 || buffers.guest_queue == 0) {
    return false;
  }
  const uint8_t alias_id = kTemporaryAliasPeerId;
  const auto message_table =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerMessageTableOffset);
  const auto sync_table =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerSyncAckTableOffset);
  const auto reliable_table =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerReliableTableOffset);
  const auto queue_table =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerQueueTableOffset);
  const auto alias_message =
      message_table
          ? mp64::CheckedGuestArrayAddress(*message_table, alias_id, mp64::kObjectPeerMessageSize)
          : std::nullopt;
  const auto alias_sync = sync_table ? mp64::CheckedGuestArrayAddress(*sync_table, alias_id,
                                                                      mp64::kObjectPeerSyncAckSize)
                                     : std::nullopt;
  const auto alias_reliable =
      reliable_table
          ? mp64::CheckedGuestArrayAddress(*reliable_table, alias_id, mp64::kObjectPeerReliableSize)
          : std::nullopt;
  const auto alias_queue = queue_table ? mp64::CheckedGuestArrayAddress(*queue_table, alias_id,
                                                                        mp64::kObjectPeerQueueSize)
                                       : std::nullopt;
  const auto sequence_table =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerSequenceTableOffset);
  const auto alias_sequence = sequence_table
                                  ? mp64::CheckedGuestArrayAddress(*sequence_table, alias_id,
                                                                   mp64::kObjectPeerSequenceStride)
                                  : std::nullopt;
  if (!alias_message || !alias_sync || !alias_reliable || !alias_queue || !alias_sequence) {
    return false;
  }
  std::array<uint8_t, mp64::kObjectPeerMessageSize> saved_message{};
  std::array<uint8_t, mp64::kObjectPeerSyncAckSize> saved_sync{};
  std::array<uint8_t, mp64::kObjectPeerReliableSize> saved_reliable{};
  std::array<uint8_t, mp64::kObjectPeerQueueSize> saved_queue{};
  std::memcpy(saved_message.data(), base + *alias_message, saved_message.size());
  std::memcpy(saved_sync.data(), base + *alias_sync, saved_sync.size());
  std::memcpy(saved_reliable.data(), base + *alias_reliable, saved_reliable.size());
  std::memcpy(saved_queue.data(), base + *alias_queue, saved_queue.size());
  std::memcpy(base + *alias_message, base + buffers.guest_message, saved_message.size());
  std::memcpy(base + *alias_sync, base + buffers.guest_sync_ack, saved_sync.size());
  std::memcpy(base + *alias_reliable, base + buffers.guest_reliable, saved_reliable.size());
  std::memcpy(base + *alias_queue, base + buffers.guest_queue, saved_queue.size());
  const uint16_t saved_sequence = REX_LOAD_U16(*alias_sequence);
  REX_STORE_U16(*alias_sequence, buffers.sequence);
  const ObjectManagerPeerAliasContext saved_context = g_object_peer_alias;
  g_object_peer_alias = {
      .active = true,
      .guest_object_manager = guest_object_manager,
      .actual_peer_id = peer_id,
      .alias_peer_id = alias_id,
  };
  const auto saved_override = GetAliasOverride(guest_peer_manager, alias_id);
  SetAliasOverride(guest_peer_manager, alias_id, guest_peer);
  callback(alias_id);
  std::memcpy(base + buffers.guest_message, base + *alias_message, saved_message.size());
  std::memcpy(base + buffers.guest_sync_ack, base + *alias_sync, saved_sync.size());
  std::memcpy(base + buffers.guest_reliable, base + *alias_reliable, saved_reliable.size());
  std::memcpy(base + buffers.guest_queue, base + *alias_queue, saved_queue.size());
  buffers.sequence = REX_LOAD_U16(*alias_sequence);
  g_object_peer_buffers.Set(guest_object_manager, peer_id, buffers);
  std::memcpy(base + *alias_message, saved_message.data(), saved_message.size());
  std::memcpy(base + *alias_sync, saved_sync.data(), saved_sync.size());
  std::memcpy(base + *alias_reliable, saved_reliable.data(), saved_reliable.size());
  std::memcpy(base + *alias_queue, saved_queue.data(), saved_queue.size());
  REX_STORE_U16(*alias_sequence, saved_sequence);
  if (saved_override) {
    SetAliasOverride(guest_peer_manager, alias_id, *saved_override);
  } else {
    ClearAliasOverride(guest_peer_manager, alias_id);
  }
  g_object_peer_alias = saved_context;
  return true;
}

uint32_t EventPeerRecord(uint8_t* base, uint32_t guest_event_manager, uint8_t peer_id) {
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_event_manager, mp64::kEventPeerManagerPointerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  return g_peer_managers.GetPeer(guest_peer_manager, peer_id);
}

bool CallNetworkObjectVirtual(PPCContext& ctx, uint8_t* base, uint32_t guest_object,
                              uint32_t vtable_offset);

bool IsPeerInEventScope(PPCContext& ctx, uint8_t* base, uint32_t guest_event, uint32_t guest_peer) {
  PPCContext nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_peer;
  sub_82708620(nested_ctx, base);
  if (nested_ctx.r3.u8 == 0) {
    return false;
  }
  nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_peer;
  sub_82708770(nested_ctx, base);
  if (nested_ctx.r3.u8 != 0) {
    return false;
  }
  nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_peer;
  sub_82A58008(nested_ctx, base);
  nested_ctx.r3.u64 = nested_ctx.r3.u32;
  sub_826C4FA0(nested_ctx, base);
  if (nested_ctx.r3.u8 == 0) {
    return false;
  }
  nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_peer;
  sub_82191F00(nested_ctx, base);
  if (nested_ctx.r3.u32 != mp64::kEventEligiblePeerType) {
    return false;
  }
  nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_event;
  nested_ctx.r4.u64 = guest_peer;
  return CallNetworkObjectVirtual(nested_ctx, base, guest_event,
                                  mp64::kEventPeerScopeVtableOffset) &&
         nested_ctx.r3.u8 != 0;
}

bool IsEventQueued(uint8_t* base, uint32_t guest_event_manager, uint32_t guest_event) {
  const auto head = mp64::CheckedGuestAddress(guest_event_manager, mp64::kEventListHeadOffset);
  uint32_t node = head ? REX_LOAD_U32(*head) : 0;
  size_t remaining = mp64::kEventQueueCapacity;
  while (node != 0 && remaining-- != 0) {
    const auto value = mp64::CheckedGuestAddress(node, mp64::kEventNodeValueOffset);
    const auto next = mp64::CheckedGuestAddress(node, mp64::kEventNodeNextOffset);
    if (!value || !next) {
      return false;
    }
    if (REX_LOAD_U32(*value) == guest_event) {
      return true;
    }
    node = REX_LOAD_U32(*next);
  }
  return false;
}

void RemoveEventScopesForManager(uint8_t* base, uint32_t guest_event_manager) {
  const auto head = mp64::CheckedGuestAddress(guest_event_manager, mp64::kEventListHeadOffset);
  uint32_t node = head ? REX_LOAD_U32(*head) : 0;
  size_t remaining = mp64::kEventQueueCapacity;
  while (node != 0 && remaining-- != 0) {
    const auto value = mp64::CheckedGuestAddress(node, mp64::kEventNodeValueOffset);
    const auto next = mp64::CheckedGuestAddress(node, mp64::kEventNodeNextOffset);
    if (!value || !next) {
      return;
    }
    g_event_scopes.Remove(REX_LOAD_U32(*value));
    node = REX_LOAD_U32(*next);
  }
}

uint32_t AllocateNetworkEndpoint(PPCContext& parent_ctx, uint8_t* base,
                                 uint32_t guest_pool_pointer_address,
                                 GuestFunction* constructor) {
  // Keep GTA's fixed endpoint pools at their retail capacities. Extended
  // recipients are sidecar-owned, so allocate their endpoint storage on
  // demand from the runtime system heap while retaining the exact retail
  // element stride and constructor.
  const uint32_t guest_pool = REX_LOAD_U32(guest_pool_pointer_address);
  if (guest_pool == 0) {
    return 0;
  }

  const uint32_t endpoint_size =
      REX_LOAD_U32(guest_pool + mp64::kFixedPoolElementStrideOffset);
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr || endpoint_size == 0) {
    return 0;
  }

  const uint32_t guest_endpoint_storage = runtime->memory()->SystemHeapAlloc(endpoint_size);
  if (guest_endpoint_storage == 0) {
    return 0;
  }

  PPCContext nested_ctx = parent_ctx;
  nested_ctx.r3.u64 = guest_endpoint_storage;
  constructor(nested_ctx, base);
  if (nested_ctx.r3.u32 != guest_endpoint_storage) {
    runtime->memory()->SystemHeapFree(guest_endpoint_storage);
    return 0;
  }
  return guest_endpoint_storage;
}

bool CallNetworkObjectVirtual(PPCContext& ctx, uint8_t* base, uint32_t guest_object,
                              uint32_t vtable_offset) {
  const auto vtable_address = mp64::CheckedGuestAddress(guest_object, 0);
  if (!vtable_address) {
    return false;
  }
  const uint32_t guest_vtable = REX_LOAD_U32(*vtable_address);
  const auto target_address = mp64::CheckedGuestAddress(guest_vtable, vtable_offset);
  if (!target_address) {
    return false;
  }
  const uint32_t target = REX_LOAD_U32(*target_address);
  if (target == 0) {
    return false;
  }
  ctx.ctr.u64 = target;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  return true;
}

void DestroyNetworkEndpoint(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_endpoint) {
  if (guest_endpoint == 0) {
    return;
  }
  PPCContext destroy_ctx = parent_ctx;
  destroy_ctx.r3.u64 = guest_endpoint;
  // Each endpoint's vfunc zero returns the object to its retail fixed pool
  // only when the deleting flag is set. Sidecar endpoints are system-heap
  // owned, so run the non-deleting destructor and release them here.
  destroy_ctx.r4.u64 = 0;
  if (!CallNetworkObjectVirtual(destroy_ctx, base, guest_endpoint, 0)) {
    REXLOG_WARN("gta4-multiplayer64: extended endpoint destructor unavailable endpoint={:08X}",
                guest_endpoint);
  }
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    runtime->memory()->SystemHeapFree(guest_endpoint);
  }
}

void FinishExtendedEndpointCreation(PPCContext& ctx, uint8_t* base, uint32_t guest_object,
                                    uint8_t peer_id, uint32_t guest_endpoint,
                                    uint32_t create_argument) {
  if (!g_network_endpoints.Set(guest_object, peer_id, guest_endpoint)) {
    DestroyNetworkEndpoint(ctx, base, guest_endpoint);
    return;
  }
  if (!PublishActiveNetworkAliasEndpoint(base, guest_object, peer_id, guest_endpoint)) {
    g_network_endpoints.Remove(guest_object, peer_id);
    DestroyNetworkEndpoint(ctx, base, guest_endpoint);
    return;
  }
  const bool initialized = WithNetworkEndpointVirtualPeer(
      ctx, base, guest_object, peer_id, [&](uint8_t presented_peer_id) {
        ctx.r3.u64 = guest_object;
        ctx.r4.u64 = presented_peer_id;
        ctx.r5.u64 = 1;
        CallNetworkObjectVirtual(ctx, base, guest_object,
                                 mp64::kNetworkObjectCreateSyncVtableOffset);
        ctx.r3.u64 = guest_object;
        ctx.r4.u64 = presented_peer_id;
        ctx.r5.u64 = create_argument;
        CallNetworkObjectVirtual(ctx, base, guest_object,
                                 mp64::kNetworkObjectSerializeSyncVtableOffset);
      });
  if (!initialized) {
    g_network_endpoints.Remove(guest_object, peer_id);
    DestroyNetworkEndpoint(ctx, base, guest_endpoint);
  }
}

bool InsertExtendedPeerAtFreeListHead(uint8_t* base, uint32_t guest_manager, uint32_t guest_peer) {
  const auto list = mp64::CheckedGuestAddress(guest_manager, mp64::kFreePeerListOffset);
  const auto head =
      list ? mp64::CheckedGuestAddress(*list, mp64::kFreeListHeadOffset) : std::nullopt;
  const auto tail =
      list ? mp64::CheckedGuestAddress(*list, mp64::kFreeListTailOffset) : std::nullopt;
  const auto count =
      list ? mp64::CheckedGuestAddress(*list, mp64::kFreeListCountOffset) : std::nullopt;
  const auto peer_next = mp64::CheckedGuestAddress(guest_peer, mp64::kPeerFreeListNextOffset);
  const auto peer_previous =
      mp64::CheckedGuestAddress(guest_peer, mp64::kPeerFreeListPreviousOffset);
  if (!head || !tail || !count || !peer_next || !peer_previous) {
    return false;
  }

  const uint32_t old_head = REX_LOAD_U32(*head);
  const uint32_t old_count = REX_LOAD_U32(*count);
  if (old_count == std::numeric_limits<uint32_t>::max()) {
    return false;
  }
  std::optional<uint32_t> old_head_previous;
  if (old_head != 0) {
    old_head_previous = mp64::CheckedGuestAddress(old_head, mp64::kPeerFreeListPreviousOffset);
    if (!old_head_previous) {
      return false;
    }
  }

  REX_STORE_U32(*peer_next, old_head);
  REX_STORE_U32(*peer_previous, 0);
  if (old_head_previous) {
    REX_STORE_U32(*old_head_previous, guest_peer);
  } else {
    REX_STORE_U32(*tail, guest_peer);
  }
  REX_STORE_U32(*head, guest_peer);
  REX_STORE_U32(*count, old_count + 1);
  return true;
}

void RemoveInjectedFreePeer(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_manager,
                            uint32_t guest_peer) {
  const auto list = mp64::CheckedGuestAddress(guest_manager, mp64::kFreePeerListOffset);
  if (!list) {
    return;
  }
  PPCContext nested_ctx = parent_ctx;
  nested_ctx.r3.u64 = *list;
  nested_ctx.r4.u64 = guest_peer;
  __imp__sub_826FEFB0(nested_ctx, base);
}

void ResetAndFreeExtendedPeer(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_peer) {
  if (guest_peer == 0) {
    return;
  }
  PPCContext nested_ctx = parent_ctx;
  nested_ctx.r3.u64 = guest_peer;
  __imp__sub_827085A8(nested_ctx, base);
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    runtime->memory()->SystemHeapFree(guest_peer);
  }
}

std::optional<uint32_t> SessionParticipantRecordAddress(const mp64::SessionParticipantState& state,
                                                        uint32_t index) noexcept {
  if (state.guest_record_table == 0 || index >= state.count ||
      index >= mp64::kExtendedPeerCapacity) {
    return std::nullopt;
  }
  return mp64::CheckedGuestArrayAddress(state.guest_record_table, index,
                                        mp64::kSessionParticipantRecordSize);
}

void StoreSessionParticipantCounts(uint8_t* base, uint32_t guest_session,
                                   const mp64::SessionParticipantState& state) {
  const auto count = mp64::CheckedGuestAddress(guest_session, mp64::kSessionParticipantCountOffset);
  const auto public_count = mp64::CheckedGuestAddress(guest_session, 1548);
  const auto private_count = mp64::CheckedGuestAddress(guest_session, 1552);
  if (count && public_count && private_count) {
    REX_STORE_U32(*count, std::min<uint32_t>(state.count, mp64::kLegacyCommandParticipantCapacity));
    REX_STORE_U32(*public_count, state.public_count);
    REX_STORE_U32(*private_count, state.private_count);
  }
}

void InitializeSessionParticipantRecord(uint8_t* base, uint32_t guest_record) {
  std::memset(base + guest_record, 0, mp64::kSessionParticipantRecordSize);
  const auto private_flag =
      mp64::CheckedGuestAddress(guest_record, mp64::kSessionParticipantPrivateFlagOffset);
  if (private_flag) {
    REX_STORE_U32(*private_flag, std::numeric_limits<uint32_t>::max());
  }
}

void SyncLegacySessionParticipantMirror(uint8_t* base, uint32_t guest_session,
                                        const mp64::SessionParticipantState& state) {
  const uint32_t mirrored_count =
      std::min<uint32_t>(state.count, mp64::kLegacyCommandParticipantCapacity);
  const auto inline_table =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionParticipantTableOffset);
  if (!inline_table) {
    return;
  }
  for (uint32_t index = 0; index < mirrored_count; ++index) {
    const auto source = SessionParticipantRecordAddress(state, index);
    const auto destination =
        mp64::CheckedGuestArrayAddress(*inline_table, index, mp64::kSessionParticipantRecordSize);
    if (!source || !destination) {
      return;
    }
    std::memcpy(base + *destination, base + *source, mp64::kSessionParticipantRecordSize);
  }
  StoreSessionParticipantCounts(base, guest_session, state);
}

int32_t FindSessionParticipant(PPCContext& parent_ctx, uint8_t* base,
                               const mp64::SessionParticipantState& state,
                               uint32_t guest_identity) {
  for (uint32_t index = 0; index < state.count; ++index) {
    const auto record = SessionParticipantRecordAddress(state, index);
    if (!record) {
      return -1;
    }
    PPCContext compare_ctx = parent_ctx;
    compare_ctx.r3.u64 = guest_identity;
    compare_ctx.r4.u64 = *record;
    __imp__sub_829DB628(compare_ctx, base);
    if (compare_ctx.r3.u8 != 0) {
      return static_cast<int32_t>(index);
    }
  }
  return -1;
}

std::optional<uint8_t> ParticipantPeerId(PPCContext& parent_ctx, uint8_t* base,
                                         uint32_t guest_participant) {
  if (guest_participant == 0) return std::nullopt;
  PPCContext identity_ctx = parent_ctx;
  identity_ctx.r3.u64 = guest_participant;
  __imp__sub_829DBAA8(identity_ctx, base);
  if (identity_ctx.r3.u32 == 0) return std::nullopt;
  PPCContext lookup_ctx = parent_ctx;
  lookup_ctx.r3.u64 = mp64::kGlobalPeerManagerAddress;
  lookup_ctx.r4.u64 = identity_ctx.r3.u32;
  sub_826FE908(lookup_ctx, base);
  return PeerRecordId(base, lookup_ctx.r3.u32);
}

bool CopyInlineSessionParticipantsToSidecar(uint8_t* base, uint32_t guest_session, uint32_t count) {
  const mp64::SessionParticipantState state = g_session_participants.Get(guest_session);
  if (state.guest_record_table == 0 || count > mp64::kLegacyCommandParticipantCapacity) {
    return false;
  }
  for (uint32_t index = 0; index < count; ++index) {
    const auto source_base =
        mp64::CheckedGuestAddress(guest_session, mp64::kSessionParticipantTableOffset);
    const auto source = source_base ? mp64::CheckedGuestArrayAddress(
                                          *source_base, index, mp64::kSessionParticipantRecordSize)
                                    : std::nullopt;
    const auto destination = mp64::CheckedGuestArrayAddress(state.guest_record_table, index,
                                                            mp64::kSessionParticipantRecordSize);
    if (!source || !destination) {
      return false;
    }
    std::memcpy(base + *destination, base + *source, mp64::kSessionParticipantRecordSize);
  }
  uint32_t public_count = 0;
  uint32_t private_count = 0;
  for (uint32_t index = 0; index < count; ++index) {
    const auto record = mp64::CheckedGuestArrayAddress(state.guest_record_table, index,
                                                       mp64::kSessionParticipantRecordSize);
    const auto private_flag =
        record ? mp64::CheckedGuestAddress(*record, mp64::kSessionParticipantPrivateFlagOffset)
               : std::nullopt;
    if (!private_flag) {
      return false;
    }
    if (REX_LOAD_U32(*private_flag) != 0) {
      ++private_count;
    } else {
      ++public_count;
    }
  }
  return g_session_participants.SetCounts(guest_session, count, public_count, private_count);
}

uint32_t AllocateSessionCommand(PPCContext& parent_ctx, uint8_t* base, uint32_t size) {
  const uint32_t guest_allocator = REX_LOAD_U32(mp64::kSessionCommandAllocatorAddress);
  if (guest_allocator == 0) {
    return 0;
  }
  PPCContext allocate_ctx = parent_ctx;
  allocate_ctx.r3.u64 = guest_allocator;
  allocate_ctx.r4.u64 = size;
  allocate_ctx.r5.u64 = 0;
  allocate_ctx.r6.u64 = 0;
  if (!CallNetworkObjectVirtual(allocate_ctx, base, guest_allocator, 8)) {
    return 0;
  }
  return allocate_ctx.r3.u32;
}

void DestroySessionCommand(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_command) {
  if (guest_command == 0) {
    return;
  }
  PPCContext destroy_ctx = parent_ctx;
  destroy_ctx.r3.u64 = guest_command;
  destroy_ctx.r4.u64 = REX_LOAD_U32(mp64::kSessionCommandAllocatorAddress);
  __imp__sub_829F58B8(destroy_ctx, base);
}

void ReleaseParticipantCommandSidecar(uint32_t guest_command) {
  const mp64::ParticipantCommandState removed = g_participant_commands.Remove(guest_command);
  if (removed.guest_record_table == 0) {
    return;
  }
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    runtime->memory()->SystemHeapFree(removed.guest_record_table);
  }
}

void ReleaseSessionParticipantSidecar(uint32_t guest_session) {
  const mp64::SessionParticipantState removed = g_session_participants.RemoveSession(guest_session);
  if (removed.guest_record_table == 0) {
    return;
  }
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    runtime->memory()->SystemHeapFree(removed.guest_record_table);
  }
}

void FailSessionCommandCallback(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_callback) {
  if (guest_callback == 0) {
    return;
  }
  PPCContext callback_ctx = parent_ctx;
  callback_ctx.r3.u64 = guest_callback;
  callback_ctx.r4.u64 = 2;
  __imp__sub_8284E5C0(callback_ctx, base);
  const auto result_address = mp64::CheckedGuestAddress(guest_callback, 4);
  if (result_address) {
    REX_STORE_U32(*result_address, std::numeric_limits<uint32_t>::max());
  }
}

void CompleteSnapshotCallback(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_callback,
                              bool success) {
  if (guest_callback == 0) {
    return;
  }
  PPCContext callback_ctx = parent_ctx;
  callback_ctx.r3.u64 = guest_callback;
  callback_ctx.r4.u64 = 1;
  __imp__sub_8284E5C0(callback_ctx, base);
  REX_STORE_U32(guest_callback + 4, 0);
  callback_ctx = parent_ctx;
  callback_ctx.r3.u64 = guest_callback;
  callback_ctx.r4.u64 = success ? 3 : 2;
  callback_ctx.r5.u64 = 1;
  __imp__sub_8284E5E0(callback_ctx, base);
  REX_STORE_U32(guest_callback + 4, success ? 0 : std::numeric_limits<uint32_t>::max());
}

bool RunLeavePlatformBatch(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_session,
                           const std::vector<uint32_t>& public_records,
                           const std::vector<uint32_t>& private_records, size_t public_start,
                           size_t private_start) {
  const size_t public_count = std::min<size_t>(mp64::kLegacyCommandParticipantCapacity,
                                               public_records.size() - public_start);
  const size_t remaining_capacity = mp64::kLegacyCommandParticipantCapacity - public_count;
  const size_t private_count =
      std::min<size_t>(remaining_capacity, private_records.size() - private_start);
  if (public_count == 0 && private_count == 0) {
    return false;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return false;
  }
  const uint32_t guest_proxy =
      runtime->memory()->SystemHeapAlloc(mp64::kLeaveCommandAllocationSize);
  if (guest_proxy == 0) {
    return false;
  }
  std::memset(base + guest_proxy, 0, mp64::kLeaveCommandAllocationSize);
  REX_STORE_U32(guest_proxy + 20, static_cast<uint32_t>(public_count));
  REX_STORE_U32(guest_proxy + 24, static_cast<uint32_t>(private_count));
  const auto first_record = mp64::CheckedGuestAddress(guest_proxy, 32);
  if (!first_record) {
    runtime->memory()->SystemHeapFree(guest_proxy);
    return false;
  }
  size_t output_index = 0;
  for (size_t index = 0; index < public_count; ++index, ++output_index) {
    const auto destination = mp64::CheckedGuestArrayAddress(*first_record, output_index,
                                                            mp64::kSessionParticipantRecordSize);
    if (!destination) {
      runtime->memory()->SystemHeapFree(guest_proxy);
      return false;
    }
    std::memcpy(base + *destination, base + public_records[public_start + index],
                mp64::kSessionParticipantRecordSize);
  }
  const auto private_begin = mp64::CheckedGuestArrayAddress(*first_record, output_index,
                                                            mp64::kSessionParticipantRecordSize);
  if (!private_begin) {
    runtime->memory()->SystemHeapFree(guest_proxy);
    return false;
  }
  REX_STORE_U32(guest_proxy + 1056, *private_begin);
  for (size_t index = 0; index < private_count; ++index, ++output_index) {
    const auto destination = mp64::CheckedGuestArrayAddress(*first_record, output_index,
                                                            mp64::kSessionParticipantRecordSize);
    if (!destination) {
      runtime->memory()->SystemHeapFree(guest_proxy);
      return false;
    }
    std::memcpy(base + *destination, base + private_records[private_start + index],
                mp64::kSessionParticipantRecordSize);
  }
  PPCContext leave_ctx = parent_ctx;
  leave_ctx.r3.u64 = guest_session;
  leave_ctx.r4.u64 = guest_proxy;
  __imp__sub_829F7428(leave_ctx, base);
  const bool success = leave_ctx.r3.u8 != 0;
  runtime->memory()->SystemHeapFree(guest_proxy);
  return success;
}

bool RunJoinPlatformBatch(PPCContext& parent_ctx, uint8_t* base, uint32_t guest_session,
                          const std::vector<uint32_t>& public_records,
                          const std::vector<uint32_t>& private_records, size_t public_start,
                          size_t private_start) {
  const size_t public_count = std::min<size_t>(mp64::kLegacyCommandParticipantCapacity,
                                               public_records.size() - public_start);
  const size_t remaining_capacity = mp64::kLegacyCommandParticipantCapacity - public_count;
  const size_t private_count =
      std::min<size_t>(remaining_capacity, private_records.size() - private_start);
  if (public_count == 0 && private_count == 0) {
    return false;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return false;
  }
  const uint32_t guest_proxy = runtime->memory()->SystemHeapAlloc(mp64::kJoinCommandAllocationSize);
  const uint32_t guest_records =
      runtime->memory()->SystemHeapAlloc(mp64::kExtendedJoinCommandTableSize);
  if (guest_proxy == 0 || guest_records == 0) {
    if (guest_proxy != 0) {
      runtime->memory()->SystemHeapFree(guest_proxy);
    }
    if (guest_records != 0) {
      runtime->memory()->SystemHeapFree(guest_records);
    }
    return false;
  }
  std::memset(base + guest_proxy, 0, mp64::kJoinCommandAllocationSize);
  std::memset(base + guest_records, 0, mp64::kExtendedJoinCommandTableSize);
  REX_STORE_U32(guest_proxy + 20, static_cast<uint32_t>(public_count));
  REX_STORE_U32(guest_proxy + 24, static_cast<uint32_t>(private_count));
  size_t output_index = 0;
  for (size_t index = 0; index < public_count; ++index, ++output_index) {
    const auto destination =
        mp64::CheckedGuestArrayAddress(guest_proxy + 28, index, mp64::kJoinCommandRecordSize);
    if (!destination) {
      runtime->memory()->SystemHeapFree(guest_records);
      runtime->memory()->SystemHeapFree(guest_proxy);
      return false;
    }
    std::memcpy(base + *destination, base + public_records[public_start + index],
                mp64::kJoinCommandRecordSize);
  }
  REX_STORE_U32(guest_proxy + 284, guest_records);
  for (size_t index = 0; index < private_count; ++index, ++output_index) {
    const auto destination =
        mp64::CheckedGuestArrayAddress(guest_records, index, mp64::kJoinCommandRecordSize);
    if (!destination) {
      runtime->memory()->SystemHeapFree(guest_records);
      runtime->memory()->SystemHeapFree(guest_proxy);
      return false;
    }
    std::memcpy(base + *destination, base + private_records[private_start + index],
                mp64::kJoinCommandRecordSize);
  }
  PPCContext join_ctx = parent_ctx;
  join_ctx.r3.u64 = guest_session;
  join_ctx.r4.u64 = guest_proxy;
  __imp__sub_829F7208(join_ctx, base);
  const bool success = join_ctx.r3.u8 != 0;
  runtime->memory()->SystemHeapFree(guest_records);
  runtime->memory()->SystemHeapFree(guest_proxy);
  return success;
}

bool PrepareMigrationTaskAlias(uint8_t* base, uint32_t guest_task,
                               const mp64::MigrationTaskState& state) {
  const auto count_address = mp64::CheckedGuestAddress(guest_task, mp64::kMigrationTaskCountOffset);
  const auto current_address =
      mp64::CheckedGuestAddress(guest_task, mp64::kMigrationTaskCurrentOffset);
  const auto inline_table =
      mp64::CheckedGuestAddress(guest_task, mp64::kMigrationTaskInlineTableOffset);
  if (!count_address || !current_address || !inline_table) {
    return false;
  }
  if (state.current < static_cast<int32_t>(mp64::kLegacyCommandParticipantCapacity - 1)) {
    REX_STORE_U32(*count_address,
                  std::min<uint32_t>(state.count, mp64::kLegacyCommandParticipantCapacity));
    REX_STORE_U32(*current_address, static_cast<uint32_t>(state.current));
    return true;
  }
  if (state.current >= static_cast<int32_t>(state.count)) {
    REX_STORE_U32(*count_address, mp64::kLegacyCommandParticipantCapacity);
    REX_STORE_U32(*current_address, mp64::kLegacyCommandParticipantCapacity);
    return true;
  }
  const uint32_t current = static_cast<uint32_t>(state.current);
  const auto previous_source = mp64::CheckedGuestArrayAddress(state.guest_record_table, current - 1,
                                                              mp64::kMigrationRecordSize);
  const auto current_source =
      mp64::CheckedGuestArrayAddress(state.guest_record_table, current, mp64::kMigrationRecordSize);
  const auto previous_destination = mp64::CheckedGuestArrayAddress(
      *inline_table, mp64::kLegacyCommandParticipantCapacity - 2, mp64::kMigrationRecordSize);
  const auto current_destination = mp64::CheckedGuestArrayAddress(
      *inline_table, mp64::kLegacyCommandParticipantCapacity - 1, mp64::kMigrationRecordSize);
  if (!previous_source || !current_source || !previous_destination || !current_destination) {
    return false;
  }
  std::memcpy(base + *previous_destination, base + *previous_source, mp64::kMigrationRecordSize);
  std::memcpy(base + *current_destination, base + *current_source, mp64::kMigrationRecordSize);
  REX_STORE_U32(*count_address, mp64::kLegacyCommandParticipantCapacity);
  REX_STORE_U32(*current_address, mp64::kLegacyCommandParticipantCapacity - 1);
  return true;
}

void ReleaseMigrationTaskSidecar(uint32_t guest_task) {
  const mp64::MigrationTaskState removed = g_migration_tasks.Remove(guest_task);
  if (removed.guest_record_table == 0) {
    return;
  }
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    runtime->memory()->SystemHeapFree(removed.guest_record_table);
  }
}

std::optional<uint32_t> ReassignmentCommandAddress(uint32_t guest_manager,
                                                   uint8_t owner_id) noexcept {
  return mp64::CheckedGuestArrayAddress(guest_manager, owner_id,
                                        mp64::kReassignmentCommandRecordStride);
}

std::optional<uint32_t> ReassignmentObjectListAddress(uint8_t* base, uint32_t guest_manager,
                                                      uint8_t owner_id) noexcept {
  if (!mp64::IsValidPeerId(owner_id)) {
    return std::nullopt;
  }
  const uint32_t guest_root = REX_LOAD_U32(guest_manager);
  return mp64::CheckedGuestArrayAddress(
      guest_root, static_cast<size_t>(mp64::kReassignmentObjectListBias) + owner_id,
      mp64::kReassignmentObjectListHeaderSize);
}

void StoreReassignmentMasks(uint8_t* base, uint32_t guest_record,
                            const mp64::ReassignmentOwnerState& state) {
  REX_STORE_U16(guest_record + 16, state.involved.legacy_low16());
  REX_STORE_U16(guest_record + 18, state.confirmed.legacy_low16());
  REX_STORE_U16(guest_record + 20, state.sent.legacy_low16());
}

void LoadReassignmentMasks(uint8_t* base, uint32_t guest_record,
                           mp64::ReassignmentOwnerState& state) {
  state.involved.ReplaceLegacyLow16(REX_LOAD_U16(guest_record + 16));
  state.confirmed.ReplaceLegacyLow16(REX_LOAD_U16(guest_record + 18));
  state.sent.ReplaceLegacyLow16(REX_LOAD_U16(guest_record + 20));
}

mp64::ReassignmentOwnerState LoadReassignmentState(uint8_t* base, uint32_t guest_manager,
                                                   uint8_t owner_id) {
  mp64::ReassignmentOwnerState state = g_reassignments.Get(guest_manager, owner_id);
  if (mp64::IsLegacyPeerId(owner_id)) {
    const auto record = ReassignmentCommandAddress(guest_manager, owner_id);
    if (record) {
      LoadReassignmentMasks(base, *record, state);
      state.initialized = true;
      g_reassignments.Set(guest_manager, owner_id, state);
    }
  } else if (!state.initialized) {
    state.initialized = true;
    g_reassignments.Set(guest_manager, owner_id, state);
  }
  return state;
}

uint32_t EnsureReassignmentTransport(PPCContext& ctx, uint8_t* base, uint32_t guest_manager,
                                     uint8_t owner_id, uint8_t recipient_id) {
  if (!mp64::IsValidPeerId(owner_id) || !mp64::IsValidPeerId(recipient_id)) {
    return 0;
  }
  const uint32_t existing = g_reassignments.GetTransport(guest_manager, owner_id, recipient_id);
  if (existing != 0) {
    return existing;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return 0;
  }
  const uint32_t guest_transport =
      runtime->memory()->SystemHeapAlloc(mp64::kReassignmentTransportStateSize);
  if (guest_transport == 0) {
    return 0;
  }
  std::memset(base + guest_transport, 0, mp64::kReassignmentTransportStateSize);
  PPCContext construct_ctx = ctx;
  construct_ctx.r3.u64 = guest_transport;
  __imp__sub_829F0E10(construct_ctx, base);
  if (!g_reassignments.SetTransport(guest_manager, owner_id, recipient_id, guest_transport)) {
    PPCContext destroy_ctx = ctx;
    destroy_ctx.r3.u64 = guest_transport;
    __imp__sub_829F0920(destroy_ctx, base);
    runtime->memory()->SystemHeapFree(guest_transport);
    return 0;
  }
  return guest_transport;
}

template <typename Callback>
bool WithExtendedReassignmentOwner(PPCContext& ctx, uint8_t* base, uint32_t guest_manager,
                                   uint8_t actual_owner, Callback&& callback) {
  if (mp64::ClassifyPeerId(actual_owner) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  std::scoped_lock reassignment_lock(g_reassignment_mutex);
  constexpr uint8_t alias_id = mp64::kReassignmentOwnerAliasPeerId;
  const auto alias_record = ReassignmentCommandAddress(guest_manager, alias_id);
  const auto alias_object_list = ReassignmentObjectListAddress(base, guest_manager, alias_id);
  if (!alias_record || !alias_object_list) {
    return false;
  }

  mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, actual_owner);
  std::array<uint8_t, mp64::kReassignmentCommandRecordSize> saved_record{};
  std::array<uint8_t, mp64::kReassignmentObjectListHeaderSize> saved_object_list{};
  std::memcpy(saved_record.data(), base + *alias_record, saved_record.size());
  std::memcpy(saved_object_list.data(), base + *alias_object_list, saved_object_list.size());
  std::memcpy(base + *alias_record, state.command_record.data(), state.command_record.size());
  std::memcpy(base + *alias_object_list, state.object_list.data(), state.object_list.size());
  StoreReassignmentMasks(base, *alias_record, state);

  const uint32_t guest_peer =
      g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, actual_owner);
  const ReassignmentCallContext saved_context = g_reassignment_call;
  g_reassignment_call.active = true;
  g_reassignment_call.guest_manager = guest_manager;
  g_reassignment_call.actual_owner = actual_owner;
  g_reassignment_call.owner_alias = alias_id;
  SetAliasOverride(mp64::kGlobalPeerManagerAddress, alias_id, guest_peer);
  callback(alias_id);

  const mp64::ReassignmentOwnerState latest = g_reassignments.Get(guest_manager, actual_owner);
  state.guest_transport_states = latest.guest_transport_states;
  state.involved = latest.involved;
  state.confirmed = latest.confirmed;
  state.sent = latest.sent;
  std::memcpy(state.command_record.data(), base + *alias_record, state.command_record.size());
  std::memcpy(state.object_list.data(), base + *alias_object_list, state.object_list.size());
  LoadReassignmentMasks(base, *alias_record, state);
  state.initialized = true;
  g_reassignments.Set(guest_manager, actual_owner, state);
  std::memcpy(base + *alias_record, saved_record.data(), saved_record.size());
  std::memcpy(base + *alias_object_list, saved_object_list.data(), saved_object_list.size());
  ClearAliasOverride(mp64::kGlobalPeerManagerAddress, alias_id);
  g_reassignment_call = saved_context;
  return true;
}

template <typename Callback>
bool WithExtendedReassignmentObjectList(uint8_t* base, uint32_t guest_manager, uint8_t actual_owner,
                                        uint8_t excluded_alias, Callback&& callback) {
  if (mp64::ClassifyPeerId(actual_owner) != mp64::PeerIdClass::kExtended) {
    return false;
  }
  std::scoped_lock reassignment_lock(g_reassignment_mutex);
  const uint8_t alias_id =
      excluded_alias == mp64::kReassignmentOwnerAliasPeerId ? uint8_t{14} : uint8_t{15};
  const auto alias_object_list = ReassignmentObjectListAddress(base, guest_manager, alias_id);
  if (!alias_object_list) {
    return false;
  }
  mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, actual_owner);
  std::array<uint8_t, mp64::kReassignmentObjectListHeaderSize> saved_object_list{};
  std::memcpy(saved_object_list.data(), base + *alias_object_list, saved_object_list.size());
  std::memcpy(base + *alias_object_list, state.object_list.data(), state.object_list.size());
  const uint8_t saved_actual_object_list_owner = g_reassignment_call.actual_object_list_owner;
  const uint8_t saved_object_list_alias = g_reassignment_call.object_list_alias;
  g_reassignment_call.actual_object_list_owner = actual_owner;
  g_reassignment_call.object_list_alias = alias_id;
  callback(alias_id);
  g_reassignment_call.actual_object_list_owner = saved_actual_object_list_owner;
  g_reassignment_call.object_list_alias = saved_object_list_alias;
  state = g_reassignments.Get(guest_manager, actual_owner);
  std::memcpy(state.object_list.data(), base + *alias_object_list, state.object_list.size());
  state.initialized = true;
  g_reassignments.Set(guest_manager, actual_owner, state);
  std::memcpy(base + *alias_object_list, saved_object_list.data(), saved_object_list.size());
  return true;
}

void CaptureProjectedReassignmentBit(uint8_t* base, uint32_t guest_record, uint8_t alias_id,
                                     uint8_t actual_recipient,
                                     mp64::ReassignmentOwnerState& state) {
  const uint16_t alias_bit = static_cast<uint16_t>(uint16_t{1} << alias_id);
  const auto capture = [&](uint32_t offset, mp64::PeerMask64& mask) {
    if ((REX_LOAD_U16(guest_record + offset) & alias_bit) != 0) {
      mask.Set(actual_recipient);
    } else {
      mask.Reset(actual_recipient);
    }
  };
  capture(16, state.involved);
  capture(18, state.confirmed);
  capture(20, state.sent);
}

void ProjectReassignmentBit(uint8_t* base, uint32_t guest_record, uint8_t alias_id,
                            uint8_t actual_recipient, const mp64::ReassignmentOwnerState& state) {
  const uint16_t alias_bit = static_cast<uint16_t>(uint16_t{1} << alias_id);
  const auto project = [&](uint32_t offset, const mp64::PeerMask64& mask) {
    uint16_t value = REX_LOAD_U16(guest_record + offset);
    value = static_cast<uint16_t>(value & ~alias_bit);
    if (mask.Contains(actual_recipient)) {
      value = static_cast<uint16_t>(value | alias_bit);
    }
    REX_STORE_U16(guest_record + offset, value);
  };
  project(16, state.involved);
  project(18, state.confirmed);
  project(20, state.sent);
}

void SendProjectedReassignmentRecipient(PPCContext& ctx, uint8_t* base,
                                        GuestFunction* original_sender, uint32_t guest_manager,
                                        uint8_t actual_owner, uint8_t physical_owner,
                                        uint8_t actual_recipient) {
  if (!mp64::IsValidPeerId(actual_recipient) || actual_recipient == actual_owner ||
      g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, actual_recipient) == 0) {
    return;
  }
  mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, actual_owner);
  if (!state.involved.Contains(actual_recipient)) {
    return;
  }
  const auto guest_record = ReassignmentCommandAddress(guest_manager, physical_owner);
  if (!guest_record) {
    return;
  }
  const uint8_t recipient_alias =
      physical_owner == mp64::kReassignmentRecipientAliasPeerId ? uint8_t{1} : uint8_t{0};
  const uint32_t guest_transport =
      EnsureReassignmentTransport(ctx, base, guest_manager, actual_owner, actual_recipient);
  if (guest_transport == 0) {
    return;
  }
  const uint16_t saved_involved = REX_LOAD_U16(*guest_record + 16);
  const uint16_t saved_confirmed = REX_LOAD_U16(*guest_record + 18);
  const uint16_t saved_sent = REX_LOAD_U16(*guest_record + 20);
  ProjectReassignmentBit(base, *guest_record, recipient_alias, actual_recipient, state);

  const ReassignmentCallContext saved_context = g_reassignment_call;
  g_reassignment_call.active = true;
  g_reassignment_call.guest_manager = guest_manager;
  g_reassignment_call.actual_owner = actual_owner;
  g_reassignment_call.owner_alias = physical_owner;
  g_reassignment_call.actual_recipient = actual_recipient;
  g_reassignment_call.recipient_alias = recipient_alias;
  g_reassignment_call.guest_transport_state = guest_transport;
  SetAliasOverride(mp64::kGlobalPeerManagerAddress, recipient_alias,
                   g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, actual_recipient));
  PPCContext send_ctx = ctx;
  send_ctx.r3.u64 = guest_manager;
  send_ctx.r4.u64 = physical_owner;
  send_ctx.r5.s64 = static_cast<int8_t>(recipient_alias);
  original_sender(send_ctx, base);
  CaptureProjectedReassignmentBit(base, *guest_record, recipient_alias, actual_recipient, state);
  REX_STORE_U16(*guest_record + 16, saved_involved);
  REX_STORE_U16(*guest_record + 18, saved_confirmed);
  REX_STORE_U16(*guest_record + 20, saved_sent);
  g_reassignments.Set(guest_manager, actual_owner, state);
  ClearAliasOverride(mp64::kGlobalPeerManagerAddress, recipient_alias);
  g_reassignment_call = saved_context;
}

void RunReassignmentSender(PPCContext& ctx, uint8_t* base, GuestFunction* original_sender,
                           uint32_t guest_manager, uint8_t actual_owner, uint8_t physical_owner,
                           int8_t requested_recipient) {
  std::scoped_lock reassignment_lock(g_reassignment_mutex);
  const bool all_recipients = requested_recipient == -1;
  const bool legacy_recipient =
      !all_recipients && mp64::IsLegacyPeerId(static_cast<uint8_t>(requested_recipient));
  if (mp64::IsLegacyPeerId(actual_owner) && (all_recipients || legacy_recipient)) {
    PPCContext legacy_ctx = ctx;
    legacy_ctx.r3.u64 = guest_manager;
    legacy_ctx.r4.u64 = physical_owner;
    legacy_ctx.r5.s64 = requested_recipient;
    original_sender(legacy_ctx, base);
    LoadReassignmentState(base, guest_manager, actual_owner);
  }

  for (uint8_t recipient_id = 0; recipient_id < mp64::kExtendedPeerCapacity; ++recipient_id) {
    if (mp64::IsLegacyPeerId(actual_owner) && mp64::IsLegacyPeerId(recipient_id)) {
      continue;
    }
    if (!all_recipients && static_cast<uint8_t>(requested_recipient) != recipient_id) {
      continue;
    }
    SendProjectedReassignmentRecipient(ctx, base, original_sender, guest_manager, actual_owner,
                                       physical_owner, recipient_id);
  }
}

std::vector<std::pair<uint8_t, uint32_t>> CollectExtendedPeers() {
  std::vector<std::pair<uint8_t, uint32_t>> peers;
  g_peer_managers.VisitExtendedPeers(mp64::kGlobalPeerManagerAddress,
                                     [&](uint8_t peer_id, uint32_t guest_peer) {
                                       peers.emplace_back(peer_id, guest_peer);
                                       return true;
                                     });
  return peers;
}

void PopulateExtendedNegotiationMask(PPCContext& ctx, uint8_t* base, uint32_t guest_manager,
                                     uint8_t owner_id) {
  mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, owner_id);
  if (g_reassignment_call.active) {
    const auto physical_record =
        ReassignmentCommandAddress(guest_manager, g_reassignment_call.owner_alias);
    if (physical_record) {
      LoadReassignmentMasks(base, *physical_record, state);
    }
  }
  for (const auto [recipient_id, guest_peer] : CollectExtendedPeers()) {
    PPCContext predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    __imp__sub_82708770(predicate_ctx, base);
    if (predicate_ctx.r3.u8 != 0) {
      continue;
    }
    predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    __imp__sub_82701F88(predicate_ctx, base);
    if (predicate_ctx.r3.u8 != owner_id) {
      state.involved.Set(recipient_id);
      state.confirmed.Set(recipient_id);
    }
  }
  g_reassignments.Set(guest_manager, owner_id, state);
  const uint8_t physical_owner =
      g_reassignment_call.active ? g_reassignment_call.owner_alias : owner_id;
  const auto record = ReassignmentCommandAddress(guest_manager, physical_owner);
  if (record) {
    StoreReassignmentMasks(base, *record, state);
  }
}

void PopulateExtendedConfirmationMask(PPCContext& ctx, uint8_t* base, uint32_t guest_manager,
                                      uint8_t owner_id) {
  mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, owner_id);
  if (g_reassignment_call.active) {
    const auto physical_record =
        ReassignmentCommandAddress(guest_manager, g_reassignment_call.owner_alias);
    if (physical_record) {
      LoadReassignmentMasks(base, *physical_record, state);
    }
  }
  for (const auto [recipient_id, guest_peer] : CollectExtendedPeers()) {
    if (!state.involved.Contains(recipient_id)) {
      continue;
    }
    PPCContext predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    __imp__sub_82708620(predicate_ctx, base);
    if (predicate_ctx.r3.u8 == 0) {
      continue;
    }
    predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    __imp__sub_82708770(predicate_ctx, base);
    if (predicate_ctx.r3.u8 != 0) {
      continue;
    }
    predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    __imp__sub_82701F88(predicate_ctx, base);
    if (predicate_ctx.r3.u8 == owner_id) {
      continue;
    }
    predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    __imp__sub_82A58008(predicate_ctx, base);
    __imp__sub_826C4FA0(predicate_ctx, base);
    if (predicate_ctx.r3.u8 != 0) {
      state.confirmed.Set(recipient_id);
    }
  }
  g_reassignments.Set(guest_manager, owner_id, state);
  const uint8_t physical_owner =
      g_reassignment_call.active ? g_reassignment_call.owner_alias : owner_id;
  const auto record = ReassignmentCommandAddress(guest_manager, physical_owner);
  if (record) {
    StoreReassignmentMasks(base, *record, state);
  }
}

void ResetExtendedReassignmentState(PPCContext& ctx, uint8_t* base, uint32_t guest_manager,
                                    uint8_t owner_id) {
  mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, owner_id);
  state.confirmed.Clear();
  state.sent.Clear();
  for (uint8_t recipient_id = mp64::kLegacyPeerCapacity; recipient_id < mp64::kExtendedPeerCapacity;
       ++recipient_id) {
    if (g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, recipient_id) == 0) {
      state.involved.Reset(recipient_id);
    }
    const uint32_t guest_transport = state.guest_transport_states[recipient_id];
    if (guest_transport != 0) {
      PPCContext reset_ctx = ctx;
      reset_ctx.r3.u64 = guest_transport;
      __imp__sub_829F04F8(reset_ctx, base);
    }
  }
  g_reassignments.Set(guest_manager, owner_id, state);
}

std::optional<uint8_t> ParseReassignmentConfirmationOwner(PPCContext& ctx, uint8_t* base,
                                                          uint32_t guest_manager,
                                                          uint32_t guest_packet) {
  if (guest_manager == 0 || guest_packet == 0) {
    return std::nullopt;
  }
  const auto packet_data = mp64::CheckedGuestAddress(guest_packet, 20);
  const auto packet_size = mp64::CheckedGuestAddress(guest_packet, 24);
  const auto parser = mp64::CheckedGuestAddress(mp64::kReassignmentMessageParserStateAddress, 8);
  const uint32_t guest_network = REX_LOAD_U32(guest_manager);
  const auto allocator = mp64::CheckedGuestAddress(guest_network, 464);
  if (!packet_data || !packet_size || !parser || !allocator) {
    return std::nullopt;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return std::nullopt;
  }
  const uint32_t scratch =
      runtime->memory()->SystemHeapAlloc(mp64::kReassignmentMessageScratchSize);
  if (scratch == 0) {
    return std::nullopt;
  }
  std::memset(base + scratch, 0, mp64::kReassignmentMessageScratchSize);
  PPCContext parse_ctx = ctx;
  parse_ctx.r3.u64 = scratch;
  parse_ctx.r4.u64 = *allocator;
  parse_ctx.r5.u64 = 1002;
  __imp__sub_82795560(parse_ctx, base);
  parse_ctx = ctx;
  parse_ctx.r3.u64 = scratch;
  parse_ctx.r4.u64 = REX_LOAD_U32(*parser);
  parse_ctx.r5.u64 = REX_LOAD_U32(*packet_data);
  parse_ctx.r6.u64 = REX_LOAD_U32(*packet_size);
  parse_ctx.r7.u64 = 0;
  __imp__sub_82783F98(parse_ctx, base);
  const std::optional<uint8_t> owner =
      parse_ctx.r3.u8 != 0 ? std::optional<uint8_t>{REX_LOAD_U8(scratch)} : std::nullopt;
  runtime->memory()->SystemHeapFree(scratch);
  return owner;
}

std::optional<uint8_t> ParseReassignmentStatusOwner(PPCContext& ctx, uint8_t* base,
                                                    uint32_t guest_packet) {
  if (guest_packet == 0) {
    return std::nullopt;
  }
  const auto packet_data = mp64::CheckedGuestAddress(guest_packet, 20);
  const auto packet_size = mp64::CheckedGuestAddress(guest_packet, 24);
  const auto parser = mp64::CheckedGuestAddress(mp64::kReassignmentStatusParserStateAddress, 8);
  if (!packet_data || !packet_size || !parser) {
    return std::nullopt;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return std::nullopt;
  }
  const uint32_t scratch =
      runtime->memory()->SystemHeapAlloc(mp64::kReassignmentMessageScratchSize);
  if (scratch == 0) {
    return std::nullopt;
  }
  std::memset(base + scratch, 0, mp64::kReassignmentMessageScratchSize);
  PPCContext parse_ctx = ctx;
  parse_ctx.r3.u64 = scratch;
  __imp__sub_827955F0(parse_ctx, base);
  parse_ctx = ctx;
  parse_ctx.r3.u64 = scratch;
  parse_ctx.r4.u64 = REX_LOAD_U32(*parser);
  parse_ctx.r5.u64 = REX_LOAD_U32(*packet_data);
  parse_ctx.r6.u64 = REX_LOAD_U32(*packet_size);
  parse_ctx.r7.u64 = 0;
  __imp__sub_827842D8(parse_ctx, base);
  const std::optional<uint8_t> owner =
      parse_ctx.r3.u8 != 0 ? std::optional<uint8_t>{REX_LOAD_U8(scratch)} : std::nullopt;
  runtime->memory()->SystemHeapFree(scratch);
  return owner;
}

void DestroyReassignmentSidecars(PPCContext& ctx, uint8_t* base, uint32_t guest_manager) {
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    if (!g_reassignments.Get(guest_manager, owner_id).initialized) {
      continue;
    }
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      if (record && REX_LOAD_U32(*record + 12) != 0) {
        PPCContext clear_ctx = ctx;
        clear_ctx.r3.u64 = guest_manager;
        clear_ctx.r4.u64 = alias_id;
        sub_82784C48(clear_ctx, base);
      }
    });
  }
  const auto states = g_reassignments.RemoveManager(guest_manager);
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    for (const auto& state : states) {
      for (const uint32_t guest_transport : state.guest_transport_states) {
        if (guest_transport == 0) {
          continue;
        }
        PPCContext destroy_ctx = ctx;
        destroy_ctx.r3.u64 = guest_transport;
        __imp__sub_829F0920(destroy_ctx, base);
        runtime->memory()->SystemHeapFree(guest_transport);
      }
    }
  }
}

void NotifyExtendedReassignmentEntry(PPCContext& ctx, uint8_t* base, uint32_t guest_manager,
                                     uint8_t owner_id, uint32_t guest_entry) {
  if (guest_entry == 0 || (REX_LOAD_U8(guest_entry + 15) & 0x10) != 0) {
    return;
  }
  const uint32_t guest_object = REX_LOAD_U32(guest_entry);
  if (guest_object == 0) {
    return;
  }
  const mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, owner_id);
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return;
  }
  const uint32_t guest_command = runtime->memory()->SystemHeapAlloc(mp64::kGuestPointerSize);
  if (guest_command == 0) {
    return;
  }
  for (const auto [recipient_id, guest_peer] : CollectExtendedPeers()) {
    (void)guest_peer;
    if (!state.involved.Contains(recipient_id)) {
      continue;
    }
    PPCContext endpoint_ctx = ctx;
    endpoint_ctx.r3.u64 = guest_object;
    endpoint_ctx.r4.u64 = recipient_id;
    sub_82702DE8(endpoint_ctx, base);
    if (endpoint_ctx.r3.u32 == 0) {
      endpoint_ctx = ctx;
      endpoint_ctx.r3.u64 = guest_object;
      endpoint_ctx.r4.u64 = recipient_id;
      endpoint_ctx.r5.u64 = 1;
      CallNetworkObjectVirtual(endpoint_ctx, base, guest_object, 68);
    }
    REX_STORE_U32(guest_command, 0);
    PPCContext fill_ctx = ctx;
    fill_ctx.r3.u64 = guest_object;
    fill_ctx.r4.u64 = guest_command;
    if (!CallNetworkObjectVirtual(fill_ctx, base, guest_object,
                                  mp64::kNetworkObjectFillOwnershipCommandVtableOffset)) {
      continue;
    }
    PPCContext send_ctx = ctx;
    send_ctx.r3.u64 = guest_object;
    send_ctx.r4.u64 = recipient_id;
    send_ctx.r5.u64 = REX_LOAD_U32(guest_command);
    send_ctx.r6.u64 = 1;
    sub_82703160(send_ctx, base);
  }
  runtime->memory()->SystemHeapFree(guest_command);
}

}  // namespace

std::optional<std::vector<uint64_t>> gta4::input::FindTeamChatTargets(
    rex::system::xam::LiveCompatibilityRuntime* live) {
  if (!live || !live->available() || !live->active_session_id()) {
    return std::nullopt;
  }
  const auto session = live->FindSessionRoute(live->active_session_id());
  if (!session) return std::nullopt;

  const auto local = std::ranges::find_if(
      session->members, [&](const rex::system::xam::SessionMember& member) {
        return member.xuid == live->identity().xuid;
      });
  if (local == session->members.end() ||
      !mp64::IsValidPeerId(local->multiplayer_peer_id)) {
    return std::nullopt;
  }
  const int32_t local_team =
      g_peer_teams.Get(static_cast<uint8_t>(local->multiplayer_peer_id));
  if (local_team < 0) return std::nullopt;

  std::vector<uint64_t> targets;
  for (const auto& member : session->members) {
    if (member.xuid == live->identity().xuid ||
        !mp64::IsValidPeerId(member.multiplayer_peer_id)) {
      continue;
    }
    if (g_peer_teams.Get(static_cast<uint8_t>(member.multiplayer_peer_id)) ==
        local_team) {
      targets.push_back(member.xuid);
    }
  }
  return targets;
}

void GTA4_RunWithPrimaryPlayerInfoAlias(PPCContext& ctx, uint8_t* base,
                                        void (*function)(PPCContext&, uint8_t*)) {
  RunPrimaryPlayerInfoAlias(ctx, base, function);
}

#define GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(function)                 \
  extern "C" void function(PPCContext& ctx, uint8_t* base) {         \
    RunPrimaryPlayerInfoAlias(ctx, base, __imp__##function);          \
  }

GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82140698)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82142F90)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82149DF0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82151B40)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8215CB10)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82168D58)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8216CF98)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8216D148)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82297F40)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8229F9C0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822A86A0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822F7AC8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822FAC88)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823082C0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82340A78)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82340B88)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82341190)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82341D58)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823435C8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82343A00)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82344368)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82353038)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8238C090)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8238E338)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82391A10)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82391E38)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823922C8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823923B0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82392C00)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82392D90)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823A8F70)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823B0B18)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823C0768)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823CE3A8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823E97D8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823ECD08)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_823EEEA0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82423078)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82463BA8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82467700)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82469FD8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_821B41E8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_821B4258)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_821B42B8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_821B4380)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_821B4768)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_824C85B0)
extern "C" void sub_824DC670(PPCContext& ctx, uint8_t* base) {
  RunPrimaryPlayerInfoAlias(ctx, base, GTA4_SonyObserveDamage);
}
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_824FF330)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82500C08)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82500F10)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82515FE0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8252A508)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82544FD0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82545528)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82575740)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_825757D0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8258E080)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_825B3B08)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_825B5910)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_825B5D60)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_821C1C78)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_821D08A8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_825D1A38)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_825D24C8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_825FE730)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82615278)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8263C3D0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82671780)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82680278)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826C4CA8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826C4E58)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826C5328)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826C7980)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826C97E8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826CA5C0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826CD660)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826CDEB8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826CEE80)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826D0010)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826D0138)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DBCE8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DBFB0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DC168)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DCBF0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DCE68)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DD2C0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DD580)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DE3C8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826DEAB8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826E1F88)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826E9300)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_826FF198)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_827004A0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82777710)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82203CD8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82204C70)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82205C30)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822094F8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82212758)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8221E790)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8222B7D8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8222BB58)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82238BD0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82238C28)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822398C8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822447B8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82244E48)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82245428)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82245620)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246028)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822461F0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822462D0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822463B0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822464D0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822465B0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246690)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246770)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246850)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246930)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246A10)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246AF0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246BD0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246CB0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246D90)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82246E70)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247298)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247378)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247458)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247538)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_822476C8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247858)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247938)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247A48)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_82247BE0)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225CF80)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225CFB8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225CFF8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225D050)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225D0B8)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225D108)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225D168)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225EF58)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225F480)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225F738)
GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK(sub_8225FBD0)

#undef GTA4_PRIMARY_PLAYER_INFO_ALIAS_HOOK

// Two transition passes in this large state-machine function enumerate the
// retail player-info table directly. Observe the exact call sites while the
// original runs, then replay only those loop bodies for high player IDs. This
// avoids rerunning any of the function's unrelated world-transition work.
extern "C" void sub_822D7188(PPCContext& ctx, uint8_t* base) {
  const PlayerTransitionLoopContext saved_transition = g_player_transition_loop;
  g_player_transition_loop = {.active = true};
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_822D7188);
  const PPCContext result_ctx = ctx;
  const bool reset_loop_seen = g_player_transition_loop.reset_loop_seen;
  const bool force_loop_seen = g_player_transition_loop.force_loop_seen;
  g_player_transition_loop = saved_transition;

  if (reset_loop_seen) {
    for (uint8_t player_id = mp64::kLegacyPeerCapacity;
         player_id < mp64::kExtendedPeerCapacity; ++player_id) {
      const auto player_info = PlayerInfoForId(base, player_id);
      const uint32_t guest_player_info = player_info.guest_player_info;
      if (guest_player_info == 0) {
        continue;
      }
      PPCContext reset_ctx = result_ctx;
      reset_ctx.r3.u64 = guest_player_info;
      reset_ctx.r4.u64 = 0;
      sub_82238988(reset_ctx, base);
    }
  }

  if (force_loop_seen) {
    for (uint8_t player_id = mp64::kLegacyPeerCapacity;
         player_id < mp64::kExtendedPeerCapacity; ++player_id) {
      const auto player_info = PlayerInfoForId(base, player_id);
      const uint32_t guest_player_info = player_info.guest_player_info;
      const auto player_address = mp64::CheckedGuestAddress(
          guest_player_info, mp64::kPlayerInfoPlayerPointerOffset);
      if (guest_player_info == 0 || !player_address) {
        continue;
      }
      const uint32_t guest_player = REX_LOAD_U32(*player_address);
      const auto nested_state_address =
          mp64::CheckedGuestAddress(guest_player, mp64::kTransitionNestedStateOffset);
      const auto player_flags_address =
          mp64::CheckedGuestAddress(guest_player, mp64::kTransitionPlayerFlagsOffset);
      if (guest_player == 0 || !nested_state_address || !player_flags_address) {
        continue;
      }
      const uint32_t nested_state = REX_LOAD_U32(*nested_state_address);
      const uint32_t guest_state =
          nested_state != 0 ? nested_state + mp64::kTransitionNestedStateBias
                            : mp64::kTransitionFallbackStateAddress;
      const auto byte_flags_address =
          mp64::CheckedGuestAddress(guest_state, mp64::kTransitionByteFlagsOffset);
      if (!byte_flags_address) {
        continue;
      }
      REX_STORE_U8(*byte_flags_address,
                   REX_LOAD_U8(*byte_flags_address) & mp64::kTransitionByteClearMask);
      const uint32_t player_flags =
          REX_LOAD_U32(*player_flags_address) | mp64::kTransitionPlayerForceFlag;
      REX_STORE_U32(*player_flags_address, player_flags);
      if ((player_flags & mp64::kTransitionPlayerActiveMask) == 0) {
        continue;
      }
      PPCContext force_ctx = result_ctx;
      force_ctx.r3.u64 = guest_player;
      force_ctx.r4.u64 = 0;
      sub_825BED00(force_ctx, base);
    }
  }
  ctx = result_ctx;
}

// STARTING_GAME owns a sixteen-pointer stack window. Retail handles that
// bounded low-peer batch; dispatch the identical notification and action to
// every high peer exactly once before the transition completes.
extern "C" void sub_826C7E98(PPCContext& ctx, uint8_t* base) {
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_826C7E98);
  const PPCContext result_ctx = ctx;
  if (REX_LOAD_U8(mp64::kStartGameDirectMessageFlagAddress) != 0) {
    return;
  }
  const uint32_t guest_sink = mp64::kStartGameMessageSinkAddress;
  g_peer_managers.VisitExtendedPeers(
      mp64::kGlobalPeerManagerAddress, [&](uint8_t, uint32_t guest_peer) {
        PPCContext message_ctx = result_ctx;
        message_ctx.r3.u64 = guest_sink;
        message_ctx.r4.u64 = 1;
        message_ctx.r5.u64 = mp64::kStartGameLiteralAddress;
        sub_822BCA90(message_ctx, base);

        PPCContext name_ctx = result_ctx;
        name_ctx.r3.u64 = guest_peer;
        sub_82708A18(name_ctx, base);
        message_ctx = result_ctx;
        message_ctx.r3.u64 = guest_sink;
        message_ctx.r4.u64 = 1;
        message_ctx.r5.u64 = 0;
        message_ctx.r6.u64 = mp64::kStartGameFormatLiteralAddress;
        message_ctx.r7.u64 = name_ctx.r3.u32;
        sub_822BCA90(message_ctx, base);

        PPCContext action_ctx = result_ctx;
        action_ctx.r3.u64 = guest_peer;
        action_ctx.r4.u64 = 5;
        sub_82708640(action_ctx, base);
        return true;
      });
  ctx = result_ctx;
}

extern "C" void sub_825B2D70(PPCContext& ctx, uint8_t* base) {
  const int32_t requested_value = ctx.r3.s32;
  uint32_t count = 0;
  for (uint8_t player_id = 0; player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    const auto player_info = PlayerInfoForId(base, player_id);
    const uint32_t guest_player_info = player_info.guest_player_info;
    if (guest_player_info != 0 &&
        REX_LOAD_U32(guest_player_info + mp64::kPlayerInfoStateOffset) != 6 &&
        static_cast<int32_t>(REX_LOAD_U32(
            guest_player_info + mp64::kPlayerInfoUniqueValueOffset)) == requested_value) {
      ++count;
    }
  }
  ctx.r3.u64 = count;
}

extern "C" void sub_8216D490(PPCContext& ctx, uint8_t* base) {
  const ThresholdPlayerLoopContext saved_threshold = g_threshold_player_loop;
  g_threshold_player_loop = {.active = true};
  __imp__sub_8216D490(ctx, base);
  const PPCContext result_ctx = ctx;
  const ThresholdPlayerLoopContext loop = g_threshold_player_loop;
  g_threshold_player_loop = saved_threshold;
  if (!loop.seen || loop.guest_match_list == 0) {
    return;
  }

  const auto count_address =
      mp64::CheckedGuestAddress(loop.guest_match_list, mp64::kThresholdListCountOffset);
  if (!count_address) {
    return;
  }
  const int32_t record_count = static_cast<int32_t>(REX_LOAD_U32(*count_address));
  if (record_count <= 0) {
    return;
  }

  const auto count_matches = [&](uint8_t first_id, uint8_t end_id) {
    uint32_t matches = 0;
    for (uint8_t player_id = first_id; player_id < end_id; ++player_id) {
      const auto player_info = PlayerInfoForId(base, player_id);
      const uint32_t guest_player_info = player_info.guest_player_info;
      if (guest_player_info == 0 ||
          static_cast<int32_t>(REX_LOAD_U32(
              guest_player_info + mp64::kPlayerInfoUniqueValueOffset)) ==
              loop.excluded_unique_value) {
        continue;
      }
      for (int32_t record_index = 0; record_index < record_count; ++record_index) {
        const auto record_address = mp64::CheckedGuestArrayAddress(
            loop.guest_match_list, static_cast<uint32_t>(record_index),
            mp64::kThresholdListRecordStride);
        if (!record_address) {
          break;
        }
        PPCContext predicate_ctx = result_ctx;
        predicate_ctx.r3.u64 = *record_address;
        sub_829DB4E8(predicate_ctx, base);
        if (predicate_ctx.r3.u8 == 0) {
          continue;
        }
        PPCContext vector_ctx = result_ctx;
        vector_ctx.r3.u64 = guest_player_info;
        sub_829DBAA8(vector_ctx, base);
        predicate_ctx = result_ctx;
        predicate_ctx.r3.u64 = *record_address;
        predicate_ctx.r4.u64 = vector_ctx.r3.u32;
        sub_829DB628(predicate_ctx, base);
        if (predicate_ctx.r3.u8 != 0) {
          ++matches;
        }
      }
    }
    return matches;
  };

  const uint32_t low_matches = count_matches(0, mp64::kLegacyPeerCapacity);
  if (low_matches >= mp64::kThresholdMatchCount) {
    return;
  }
  const uint32_t high_matches =
      count_matches(mp64::kLegacyPeerCapacity, mp64::kExtendedPeerCapacity);
  if (low_matches + high_matches >= mp64::kThresholdMatchCount) {
    PPCContext event_ctx = result_ctx;
    event_ctx.r3.u64 = mp64::kThresholdEventId;
    sub_826CCDB8(event_ctx, base);
  }
  ctx = result_ctx;
}

extern "C" void sub_826DE548(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const auto initial_session_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbySessionOffset);
  const auto first_scan_flag_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbyFirstScanFlagOffset);
  const auto first_scan_timestamp_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbyFirstScanTimestampOffset);
  if (guest_manager != 0 && initial_session_address && first_scan_flag_address &&
      first_scan_timestamp_address && REX_LOAD_U32(*initial_session_address) != 0 &&
      REX_LOAD_U8(*first_scan_flag_address) != 0 &&
      REX_LOAD_U32(mp64::kLobbyClockAddress) > REX_LOAD_U32(*first_scan_timestamp_address)) {
    PPCContext gate_ctx = ctx;
    sub_826C4C98(gate_ctx, base);
    if (gate_ctx.r3.u8 != 0) {
      gate_ctx = ctx;
      sub_826C4E48(gate_ctx, base);
      sub_826F8960(gate_ctx, base);
      if (gate_ctx.r3.u8 != 0) {
        const uint32_t guest_session = REX_LOAD_U32(*initial_session_address);
        for (uint8_t player_id = mp64::kLegacyPeerCapacity;
             player_id < mp64::kExtendedPeerCapacity; ++player_id) {
          const auto player_info = PlayerInfoForId(base, player_id);
          const uint32_t guest_player_info = player_info.guest_player_info;
          if (guest_player_info == 0) {
            continue;
          }
          PPCContext vector_ctx = ctx;
          vector_ctx.r3.u64 = guest_player_info;
          sub_829DBAA8(vector_ctx, base);
          const uint32_t guest_vector = vector_ctx.r3.u32;
          PPCContext filter_ctx = ctx;
          sub_826C4E48(filter_ctx, base);
          filter_ctx.r4.u64 = guest_vector;
          sub_826F8F30(filter_ctx, base);
          if (filter_ctx.r3.u8 == 0) {
            continue;
          }
          vector_ctx = ctx;
          vector_ctx.r3.u64 = guest_player_info;
          sub_829DBAB0(vector_ctx, base);
          PPCContext contains_ctx = ctx;
          contains_ctx.r3.u64 = guest_session;
          contains_ctx.r4.u64 = vector_ctx.r3.u32;
          sub_827923F0(contains_ctx, base);
          if (contains_ctx.r3.u8 == 0 &&
              static_cast<int32_t>(REX_LOAD_U32(mp64::kLobbySelectionIndexAddress)) != -1) {
            PPCContext select_ctx = ctx;
            select_ctx.r3.u64 = guest_session;
            sub_82792778(select_ctx, base);
          }
        }
      }
    }
  }
  const LobbyPositionLoopContext saved_loop = g_lobby_position_loop;
  g_lobby_position_loop = {.active = true};
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_826DE548);
  const PPCContext result_ctx = ctx;
  const bool position_loop_seen = g_lobby_position_loop.seen;
  g_lobby_position_loop = saved_loop;
  if (!position_loop_seen || guest_manager == 0) {
    return;
  }
  const auto session_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbySessionOffset);
  const auto record_count_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbyPositionRecordCountOffset);
  const auto flag_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbyPositionFlagOffset);
  const auto timestamp_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbyPositionTimestampOffset);
  const auto records_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kLobbyPositionRecordsOffset);
  if (!session_address || !record_count_address || !flag_address || !timestamp_address ||
      !records_address || REX_LOAD_U32(*session_address) == 0) {
    return;
  }

  const int32_t record_count = static_cast<int32_t>(REX_LOAD_U32(*record_count_address));
  bool inserted = false;
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    const auto player_info = PlayerInfoForId(base, player_id);
    const uint32_t guest_player_info = player_info.guest_player_info;
    if (guest_player_info == 0) {
      continue;
    }
    PPCContext vector_ctx = result_ctx;
    vector_ctx.r3.u64 = guest_player_info;
    sub_829DBAA8(vector_ctx, base);
    const uint32_t guest_vector = vector_ctx.r3.u32;
    PPCContext filter_ctx = result_ctx;
    sub_826C4E48(filter_ctx, base);
    filter_ctx.r4.u64 = guest_vector;
    sub_826F8F30(filter_ctx, base);
    if (filter_ctx.r3.u8 == 0) {
      continue;
    }

    bool duplicate = false;
    for (int32_t record_index = 0; record_index < record_count; ++record_index) {
      const auto record_address = mp64::CheckedGuestArrayAddress(
          *records_address, static_cast<uint32_t>(record_index),
          mp64::kLobbyPositionRecordStride);
      if (!record_address) {
        break;
      }
      PPCContext active_ctx = result_ctx;
      active_ctx.r3.u64 = *record_address;
      sub_829E4F08(active_ctx, base);
      if (active_ctx.r3.u8 == 0) {
        continue;
      }
      PPCContext match_ctx = result_ctx;
      match_ctx.r3.u64 = *record_address;
      match_ctx.r4.u64 = guest_vector;
      sub_829E4F90(match_ctx, base);
      if (match_ctx.r3.u8 != 0) {
        duplicate = true;
        break;
      }
    }
    if (duplicate) {
      continue;
    }

    REX_STORE_U8(*flag_address, 1);
    REX_STORE_U32(*timestamp_address,
                  REX_LOAD_U32(mp64::kLobbyClockAddress) + mp64::kLobbyPositionRetryTicks);
    for (int32_t record_index = 0; record_index < record_count; ++record_index) {
      const auto record_address = mp64::CheckedGuestArrayAddress(
          *records_address, static_cast<uint32_t>(record_index),
          mp64::kLobbyPositionRecordStride);
      if (!record_address) {
        break;
      }
      PPCContext active_ctx = result_ctx;
      active_ctx.r3.u64 = *record_address;
      sub_829E4F08(active_ctx, base);
      if (active_ctx.r3.u8 != 0) {
        continue;
      }
      std::memcpy(REX_RAW_ADDR(*record_address), REX_RAW_ADDR(guest_vector),
                  mp64::kLobbyPositionVectorSize);
      inserted = true;
      break;
    }
  }
  if (inserted) {
    PPCContext update_ctx = result_ctx;
    update_ctx.r3.u64 = guest_manager;
    sub_826DE278(update_ctx, base);
  }
  ctx = result_ctx;
}

extern "C" void sub_82718AE8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  __imp__sub_82718AE8(ctx, base);
  if (ctx.r3.s32 >= 0) {
    return;
  }
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (!WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                    [&]() { __imp__sub_82718AE8(attempt_ctx, base); }) ||
        attempt_ctx.r3.s32 < 0) {
      continue;
    }
    if (attempt_ctx.r3.u32 == kTemporaryAliasPeerId) {
      attempt_ctx.r3.u64 = player_id;
    }
    ctx = attempt_ctx;
    return;
  }
}

extern "C" void sub_825755A0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  __imp__sub_825755A0(ctx, base);
  if (ctx.r3.s32 >= 0) {
    return;
  }
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (!WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                    [&]() { __imp__sub_825755A0(attempt_ctx, base); }) ||
        attempt_ctx.r3.s32 < 0) {
      continue;
    }
    attempt_ctx.r3.u64 = player_id;
    ctx = attempt_ctx;
    return;
  }
}

extern "C" void sub_821F89C0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  __imp__sub_821F89C0(ctx, base);
  if (ctx.r3.u8 == 0) {
    return;
  }
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                   [&]() { __imp__sub_821F89C0(attempt_ctx, base); }) &&
        attempt_ctx.r3.u8 == 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

extern "C" void sub_8222A030(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  __imp__sub_8222A030(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                   [&]() { __imp__sub_8222A030(attempt_ctx, base); }) &&
        attempt_ctx.r3.u8 != 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

extern "C" void sub_821F8A78(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  __imp__sub_821F8A78(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                   [&]() { __imp__sub_821F8A78(attempt_ctx, base); }) &&
        attempt_ctx.r3.u8 != 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

extern "C" void sub_823A3DD0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  __imp__sub_823A3DD0(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                   [&]() { __imp__sub_823A3DD0(attempt_ctx, base); }) &&
        attempt_ctx.r3.u8 != 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

extern "C" void sub_82502B88(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82502B88);
  if (ctx.r3.u8 == 0) {
    return;
  }
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                   [&]() { __imp__sub_82502B88(attempt_ctx, base); }) &&
        attempt_ctx.r3.u8 == 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

template <GuestFunction* Original>
void RunPlayerInfoFloatMinimum(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  Original(ctx, base);
  double best = ctx.f1.f64;
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    if (WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                                   [&]() { Original(attempt_ctx, base); })) {
      best = std::min(best, attempt_ctx.f1.f64);
    }
  }
  ctx.f1.f64 = best;
}

extern "C" void sub_822705A0(PPCContext& ctx, uint8_t* base) {
  RunPlayerInfoFloatMinimum<__imp__sub_822705A0>(ctx, base);
}

extern "C" void sub_826E5D50(PPCContext& ctx, uint8_t* base) {
  RunPlayerInfoFloatMinimum<__imp__sub_826E5D50>(ctx, base);
}

extern "C" void sub_826E5E18(PPCContext& ctx, uint8_t* base) {
  RunPlayerInfoFloatMinimum<__imp__sub_826E5E18>(ctx, base);
}

extern "C" void sub_821C1460(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  __imp__sub_821C1460(ctx, base);
  const PPCContext result_ctx = ctx;
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    PPCContext attempt_ctx = input_ctx;
    WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                               [&]() { __imp__sub_821C1460(attempt_ctx, base); });
  }
  ctx = result_ctx;
}

uint32_t ReadProximityStatusDword(
    const std::array<uint8_t, mp64::kProximityStatusRecordSize>& record, uint32_t offset) {
  rex::be<uint32_t> value{};
  std::memcpy(&value, record.data() + offset, sizeof(value));
  return value;
}

void WriteProximityStatusDword(
    std::array<uint8_t, mp64::kProximityStatusRecordSize>& record, uint32_t offset,
    uint32_t value) {
  const rex::be<uint32_t> stored_value = value;
  std::memcpy(record.data() + offset, &stored_value, sizeof(stored_value));
}

void ApplyGlobalProximityStatusWeights(uint8_t* base) {
  std::array<uint32_t, 3> category_counts{};
  for (uint8_t player_id = 0; player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    if (!g_proximity_status_capture.eligible[player_id]) {
      continue;
    }
    const uint32_t category = ReadProximityStatusDword(
        g_proximity_status_sidecar[player_id], mp64::kProximityStatusCategoryOffset);
    if (category < category_counts.size()) {
      ++category_counts[category];
    }
  }

  const std::array<float, 3> multipliers = {
      1.0f,
      std::bit_cast<float>(
          REX_LOAD_U32(mp64::kProximityStatusCategoryOneMultiplierAddress)),
      std::bit_cast<float>(
          REX_LOAD_U32(mp64::kProximityStatusCategoryTwoMultiplierAddress)),
  };
  const auto weights = mp64::ComputeGlobalProximityStatusWeights(
      category_counts, REX_LOAD_U32(mp64::kProximityStatusRandomAddress), multipliers);

  for (uint8_t player_id = 0; player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    if (!g_proximity_status_capture.eligible[player_id]) {
      continue;
    }
    auto& record = g_proximity_status_sidecar[player_id];
    const uint32_t category =
        ReadProximityStatusDword(record, mp64::kProximityStatusCategoryOffset);
    if (category < weights.size()) {
      WriteProximityStatusDword(record, mp64::kProximityStatusWeightOffset,
                                weights[category]);
    }
  }
}

void RestoreLegacyProximityStatusTable(uint8_t* base) {
  for (uint8_t player_id = 0; player_id < mp64::kLegacyPeerCapacity; ++player_id) {
    const auto record_address = mp64::CheckedGuestArrayAddress(
        mp64::kProximityStatusTableAddress, player_id, mp64::kProximityStatusRecordSize);
    if (record_address) {
      std::memcpy(REX_RAW_ADDR(*record_address), g_proximity_status_sidecar[player_id].data(),
                  mp64::kProximityStatusRecordSize);
    }
  }
}

extern "C" void sub_826E0A90(PPCContext& ctx, uint8_t* base) {
  if (!g_nearest_network_decision.active) {
    __imp__sub_826E0A90(ctx, base);
    return;
  }
  const double score = ctx.f29.f64;
  if (!g_nearest_network_decision.captured || score < g_nearest_network_decision.score) {
    g_nearest_network_decision.captured = true;
    g_nearest_network_decision.score = score;
    g_nearest_network_decision.call_ctx = ctx;
  }
}

extern "C" void sub_826E6E48(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const auto timestamp_address =
      mp64::CheckedGuestAddress(input_ctx.r3.u32, mp64::kNearestNetworkTimestampOffset);
  const uint32_t initial_timestamp = timestamp_address ? REX_LOAD_U32(*timestamp_address) : 0;
  const NearestNetworkDecisionContext saved_decision = g_nearest_network_decision;
  g_nearest_network_decision = {.active = true};
  __imp__sub_826E6E48(ctx, base);
  const PPCContext result_ctx = ctx;
  const uint32_t final_timestamp = timestamp_address ? REX_LOAD_U32(*timestamp_address) : 0;
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    if (timestamp_address) {
      REX_STORE_U32(*timestamp_address, initial_timestamp);
    }
    PPCContext attempt_ctx = input_ctx;
    WithOnlyExtendedPlayerInfo(attempt_ctx, base, player_id,
                               [&]() { __imp__sub_826E6E48(attempt_ctx, base); });
  }
  if (timestamp_address) {
    REX_STORE_U32(*timestamp_address, final_timestamp);
  }
  const NearestNetworkDecisionContext decision = g_nearest_network_decision;
  g_nearest_network_decision = saved_decision;
  if (decision.captured) {
    PPCContext action_ctx = decision.call_ctx;
    __imp__sub_826E0A90(action_ctx, base);
  }
  ctx = result_ctx;
}

extern "C" void sub_823193D8(PPCContext& ctx, uint8_t* base) {
  if (!g_population_dispatch_capture.active) {
    __imp__sub_823193D8(ctx, base);
    return;
  }
  const auto duplicate = std::find_if(
      g_population_dispatch_capture.calls.begin(), g_population_dispatch_capture.calls.end(),
      [&](const PPCContext& saved) {
        return saved.r3.u32 == ctx.r3.u32 && saved.r4.u32 == ctx.r4.u32 &&
               saved.r6.u32 == ctx.r6.u32 && saved.r7.u32 == ctx.r7.u32 &&
               saved.r8.u32 == ctx.r8.u32;
      });
  if (duplicate == g_population_dispatch_capture.calls.end()) {
    g_population_dispatch_capture.calls.push_back(ctx);
  }
  ctx.r3.u64 = 0;
}

extern "C" void sub_823BC0B8(PPCContext& ctx, uint8_t* base) {
  if (g_player_info_batch_projection.active) {
    __imp__sub_823BC0B8(ctx, base);
    return;
  }
  const PPCContext input_ctx = ctx;
  std::vector<uint64_t> merged_records;
  const auto capture_records = [&]() {
    const uint32_t count =
        std::min(REX_LOAD_U32(mp64::kSampleCountAddress), mp64::kSampleCapacity);
    for (uint32_t record_index = 0; record_index < count; ++record_index) {
      const auto record_address = mp64::CheckedGuestArrayAddress(
          mp64::kSampleTableAddress, record_index, mp64::kSampleRecordSize);
      if (!record_address) {
        break;
      }
      const uint64_t record = REX_LOAD_U64(*record_address);
      if (std::find(merged_records.begin(), merged_records.end(), record) ==
              merged_records.end() &&
          merged_records.size() < mp64::kSampleCapacity) {
        merged_records.push_back(record);
      }
    }
  };

  __imp__sub_823BC0B8(ctx, base);
  const PPCContext result_ctx = ctx;
  capture_records();
  for (uint8_t first_player_id = mp64::kLegacyPeerCapacity;
       first_player_id < mp64::kExtendedPeerCapacity;
       first_player_id = static_cast<uint8_t>(first_player_id + mp64::kLegacyPeerCapacity)) {
    PPCContext batch_ctx = input_ctx;
    WithProjectedPlayerInfoBatch(
        first_player_id, [&]() { __imp__sub_823BC0B8(batch_ctx, base); });
    capture_records();
  }
  for (uint32_t record_index = 0; record_index < merged_records.size(); ++record_index) {
    const auto record_address = mp64::CheckedGuestArrayAddress(
        mp64::kSampleTableAddress, record_index, mp64::kSampleRecordSize);
    if (record_address) {
      REX_STORE_U64(*record_address, merged_records[record_index]);
    }
  }
  REX_STORE_U32(mp64::kSampleCountAddress, static_cast<uint32_t>(merged_records.size()));
  ctx = result_ctx;
}

extern "C" void sub_823F70E8(PPCContext& ctx, uint8_t* base) {
  if (g_population_dispatch_capture.active) {
    __imp__sub_823F70E8(ctx, base);
    return;
  }
  const PPCContext input_ctx = ctx;
  const PopulationDispatchCaptureContext saved_capture = g_population_dispatch_capture;
  g_population_dispatch_capture = {.active = true};
  __imp__sub_823F70E8(ctx, base);
  for (uint8_t first_player_id = mp64::kLegacyPeerCapacity;
       first_player_id < mp64::kExtendedPeerCapacity;
       first_player_id = static_cast<uint8_t>(first_player_id + mp64::kLegacyPeerCapacity)) {
    PPCContext batch_ctx = input_ctx;
    WithProjectedPlayerInfoBatch(
        first_player_id, [&]() { __imp__sub_823F70E8(batch_ctx, base); });
  }
  const std::vector<PPCContext> calls = std::move(g_population_dispatch_capture.calls);
  g_population_dispatch_capture = saved_capture;
  uint32_t dispatched = 0;
  for (PPCContext call_ctx : calls) {
    __imp__sub_823193D8(call_ctx, base);
    dispatched += call_ctx.r3.u32;
  }
  ctx.r3.u64 = dispatched;
}

extern "C" void sub_8278D0C8(PPCContext& ctx, uint8_t* base) {
  if (g_player_info_batch_projection.active || g_proximity_status_driver_active) {
    __imp__sub_8278D0C8(ctx, base);
    return;
  }
  std::scoped_lock status_lock(g_proximity_status_mutex);
  const ProximityStatusCaptureContext saved_capture = g_proximity_status_capture;
  g_proximity_status_capture = {.active = true};
  const PPCContext input_ctx = ctx;
  __imp__sub_8278D0C8(ctx, base);
  const PPCContext result_ctx = ctx;
  for (uint8_t player_id = 0; player_id < mp64::kLegacyPeerCapacity; ++player_id) {
    const auto record_address = mp64::CheckedGuestArrayAddress(
        mp64::kProximityStatusTableAddress, player_id, mp64::kProximityStatusRecordSize);
    if (record_address) {
      std::memcpy(g_proximity_status_sidecar[player_id].data(), REX_RAW_ADDR(*record_address),
                  mp64::kProximityStatusRecordSize);
    }
  }
  for (uint8_t first_player_id = mp64::kLegacyPeerCapacity;
       first_player_id < mp64::kExtendedPeerCapacity;
       first_player_id = static_cast<uint8_t>(first_player_id + mp64::kLegacyPeerCapacity)) {
    for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
      const auto record_address = mp64::CheckedGuestArrayAddress(
          mp64::kProximityStatusTableAddress, alias_id, mp64::kProximityStatusRecordSize);
      if (record_address) {
        std::memcpy(REX_RAW_ADDR(*record_address),
                    g_proximity_status_sidecar[first_player_id + alias_id].data(),
                    mp64::kProximityStatusRecordSize);
      }
    }
    PPCContext batch_ctx = input_ctx;
    WithProjectedPlayerInfoBatch(
        first_player_id, [&]() { __imp__sub_8278D0C8(batch_ctx, base); });
    for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
      const auto record_address = mp64::CheckedGuestArrayAddress(
          mp64::kProximityStatusTableAddress, alias_id, mp64::kProximityStatusRecordSize);
      if (record_address) {
        std::memcpy(g_proximity_status_sidecar[first_player_id + alias_id].data(),
                    REX_RAW_ADDR(*record_address), mp64::kProximityStatusRecordSize);
      }
    }
  }
  ApplyGlobalProximityStatusWeights(base);
  RestoreLegacyProximityStatusTable(base);
  g_proximity_status_capture = saved_capture;
  ctx = result_ctx;
}

extern "C" void sub_8278D608(PPCContext& ctx, uint8_t* base) {
  if (g_player_info_batch_projection.active) {
    __imp__sub_8278D608(ctx, base);
    return;
  }
  std::scoped_lock status_lock(g_proximity_status_mutex);
  const ProximityStatusCaptureContext saved_capture = g_proximity_status_capture;
  g_proximity_status_capture = {.active = true};
  const PPCContext input_ctx = ctx;
  const uint32_t initial_timestamp = REX_LOAD_U32(mp64::kProximityStatusTimestampAddress);
  const bool saved_driver = g_proximity_status_driver_active;
  g_proximity_status_driver_active = true;
  __imp__sub_8278D608(ctx, base);
  g_proximity_status_driver_active = saved_driver;
  const PPCContext result_ctx = ctx;
  const uint32_t final_timestamp = REX_LOAD_U32(mp64::kProximityStatusTimestampAddress);
  std::array<uint8_t, mp64::kProximityStatusTableBytes> low_table{};
  std::memcpy(low_table.data(), REX_RAW_ADDR(mp64::kProximityStatusTableAddress),
              low_table.size());
  for (uint8_t player_id = 0; player_id < mp64::kLegacyPeerCapacity; ++player_id) {
    std::memcpy(g_proximity_status_sidecar[player_id].data(),
                low_table.data() + player_id * mp64::kProximityStatusRecordSize,
                mp64::kProximityStatusRecordSize);
  }

  for (uint8_t first_player_id = mp64::kLegacyPeerCapacity;
       first_player_id < mp64::kExtendedPeerCapacity;
       first_player_id = static_cast<uint8_t>(first_player_id + mp64::kLegacyPeerCapacity)) {
    for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
      const auto record_address = mp64::CheckedGuestArrayAddress(
          mp64::kProximityStatusTableAddress, alias_id, mp64::kProximityStatusRecordSize);
      if (record_address) {
        std::memcpy(REX_RAW_ADDR(*record_address),
                    g_proximity_status_sidecar[first_player_id + alias_id].data(),
                    mp64::kProximityStatusRecordSize);
      }
    }
    REX_STORE_U32(mp64::kProximityStatusTimestampAddress, initial_timestamp);
    PPCContext batch_ctx = input_ctx;
    WithProjectedPlayerInfoBatch(
        first_player_id, [&]() { __imp__sub_8278D608(batch_ctx, base); });
    for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
      const auto record_address = mp64::CheckedGuestArrayAddress(
          mp64::kProximityStatusTableAddress, alias_id, mp64::kProximityStatusRecordSize);
      if (record_address) {
        std::memcpy(g_proximity_status_sidecar[first_player_id + alias_id].data(),
                    REX_RAW_ADDR(*record_address), mp64::kProximityStatusRecordSize);
      }
    }
  }
  ApplyGlobalProximityStatusWeights(base);
  RestoreLegacyProximityStatusTable(base);
  REX_STORE_U32(mp64::kProximityStatusTimestampAddress, final_timestamp);
  g_proximity_status_capture = saved_capture;
  ctx = result_ctx;
}

// Retail peer-manager constructor. Register its guest address after all
// embedded records and pointer slots have been initialized.
extern "C" void sub_82700AA0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82700AA0);
  if (g_peer_managers.RegisterManager(guest_manager)) {
    PopulateLegacySidecar(base, guest_manager);
  }
}

// Retail peer-manager destructor. Keep the sidecar alive during the original
// teardown because removal callbacks re-enter the hooked lookup path.
extern "C" void sub_827013F8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  std::vector<uint8_t> extended_peer_ids;
  g_peer_managers.VisitExtendedPeers(guest_manager, [&](uint8_t peer_id, uint32_t) {
    extended_peer_ids.push_back(peer_id);
    return true;
  });
  for (uint8_t peer_id : extended_peer_ids) {
    PPCContext remove_ctx = ctx;
    remove_ctx.r3.u64 = guest_manager;
    remove_ctx.r4.u64 = peer_id;
    sub_82700E30(remove_ctx, base);
  }
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_827013F8);
  const auto reassignment_states = g_reassignments.RemoveManager(guest_manager);
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    for (const auto& state : reassignment_states) {
      for (const uint32_t guest_transport : state.guest_transport_states) {
        if (guest_transport != 0) {
          PPCContext destroy_ctx = ctx;
          destroy_ctx.r3.u64 = guest_transport;
          __imp__sub_829F0920(destroy_ctx, base);
          runtime->memory()->SystemHeapFree(guest_transport);
        }
      }
    }
  }
  g_peer_managers.UnregisterManager(guest_manager);
  std::scoped_lock cursor_lock(g_voice_cursor_mutex);
  g_voice_cursors.erase(guest_manager);
}

// Peer lookup by signed byte ID. The original implementation may index the
// embedded pointer table only for IDs 0..15. Extended IDs use the sidecar and
// invalid/sentinel IDs can never become guest pointer arithmetic.
bool CanBroadcastConnectionData(PPCContext& ctx, uint8_t* base) {
  PPCContext nested_ctx = ctx;
  __imp__sub_826C4E48(nested_ctx, base);
  __imp__sub_826F8960(nested_ctx, base);
  if (nested_ctx.r3.u8 == 0) {
    return false;
  }
  nested_ctx = ctx;
  __imp__sub_826CD808(nested_ctx, base);
  const uint32_t guest_session = nested_ctx.r3.u32;
  if (guest_session == 0) {
    return false;
  }
  nested_ctx = ctx;
  __imp__sub_826C4778(nested_ctx, base);
  if (nested_ctx.r3.u8 != 0) {
    return false;
  }
  nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_session;
  __imp__sub_829DBA98(nested_ctx, base);
  return nested_ctx.r3.u8 != 0;
}

extern "C" void sub_826DDA10(PPCContext& ctx, uint8_t* base) {
  __imp__sub_826DDA10(ctx, base);
  if (!CanBroadcastConnectionData(ctx, base)) {
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return;
  }
  const uint32_t guest_payload =
      runtime->memory()->SystemHeapAlloc(mp64::kConnectionBroadcastPayloadSize);
  if (guest_payload == 0) {
    return;
  }
  PPCContext value_ctx = ctx;
  value_ctx.r3.u64 = 0;
  __imp__sub_826DBFB0(value_ctx, base);
  REX_STORE_U32(guest_payload, value_ctx.r3.s32 < 0 ? 0 : value_ctx.r3.u32);
  value_ctx = ctx;
  value_ctx.r3.u64 = 0;
  __imp__sub_826DC168(value_ctx, base);
  const auto second_word = mp64::CheckedGuestAddress(guest_payload, mp64::kGuestPointerSize);
  if (!second_word) {
    runtime->memory()->SystemHeapFree(guest_payload);
    return;
  }
  REX_STORE_U32(*second_word, value_ctx.r3.u32);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    PPCContext eligible_ctx = ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708978(eligible_ctx, base);
    if (eligible_ctx.r3.u8 != 0) {
      continue;
    }
    eligible_ctx = ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708620(eligible_ctx, base);
    if (eligible_ctx.r3.u8 == 0) {
      continue;
    }
    eligible_ctx = ctx;
    eligible_ctx.r3.u64 = mp64::kGlobalPeerManagerAddress;
    eligible_ctx.r4.u64 = peer_id;
    __imp__sub_826FF150(eligible_ctx, base);
    if (eligible_ctx.r3.s32 < 0) {
      continue;
    }
    const uint32_t channel = eligible_ctx.r3.u32;
    PPCContext sender_ctx = ctx;
    __imp__sub_826C4E38(sender_ctx, base);
    sender_ctx.r3.u64 = sender_ctx.r3.u32;
    sender_ctx.r4.u64 = channel;
    sender_ctx.r5.u64 = guest_payload;
    sender_ctx.r6.u64 = 1;
    sender_ctx.r7.u64 = 0;
    __imp__sub_826DD0C8(sender_ctx, base);
    ctx.r3.u64 = sender_ctx.r3.u64;
  }
  runtime->memory()->SystemHeapFree(guest_payload);
}

extern "C" void sub_826DDB48(PPCContext& ctx, uint8_t* base) {
  const int32_t first_value = ctx.r3.s32;
  const uint32_t second_value = ctx.r4.u32;
  __imp__sub_826DDB48(ctx, base);
  if (!CanBroadcastConnectionData(ctx, base)) {
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    return;
  }
  const uint32_t guest_payload =
      runtime->memory()->SystemHeapAlloc(mp64::kConnectionBroadcastPayloadSize);
  if (guest_payload == 0) {
    return;
  }
  REX_STORE_U32(guest_payload, first_value < 0 ? 0 : static_cast<uint32_t>(first_value));
  const auto second_word = mp64::CheckedGuestAddress(guest_payload, mp64::kGuestPointerSize);
  if (!second_word) {
    runtime->memory()->SystemHeapFree(guest_payload);
    return;
  }
  REX_STORE_U32(*second_word, second_value);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    PPCContext eligible_ctx = ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708978(eligible_ctx, base);
    if (eligible_ctx.r3.u8 != 0) {
      continue;
    }
    eligible_ctx = ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708620(eligible_ctx, base);
    if (eligible_ctx.r3.u8 == 0) {
      continue;
    }
    eligible_ctx = ctx;
    eligible_ctx.r3.u64 = mp64::kGlobalPeerManagerAddress;
    eligible_ctx.r4.u64 = peer_id;
    __imp__sub_826FF150(eligible_ctx, base);
    if (eligible_ctx.r3.s32 < 0) {
      continue;
    }
    const uint32_t channel = eligible_ctx.r3.u32;
    PPCContext sender_ctx = ctx;
    __imp__sub_826C4E38(sender_ctx, base);
    sender_ctx.r3.u64 = sender_ctx.r3.u32;
    sender_ctx.r4.u64 = channel;
    sender_ctx.r5.u64 = guest_payload;
    sender_ctx.r6.u64 = 1;
    sender_ctx.r7.u64 = 0;
    __imp__sub_826DD150(sender_ctx, base);
    ctx.r3.u64 = sender_ctx.r3.u64;
  }
  runtime->memory()->SystemHeapFree(guest_payload);
}

extern "C" void sub_826DDD90(PPCContext& ctx, uint8_t* base) {
  const uint32_t first_argument = ctx.r3.u32;
  const uint32_t second_argument = ctx.r4.u32;
  const uint32_t guest_message = ctx.r5.u32;
  PPCContext type_ctx = ctx;
  type_ctx.r3.u64 = guest_message;
  if (!CallNetworkObjectVirtual(type_ctx, base, guest_message, 0) || type_ctx.r3.u32 != 4) {
    __imp__sub_826DDD90(ctx, base);
    return;
  }
  const auto payload_pointer =
      mp64::CheckedGuestAddress(guest_message, mp64::kConnectionMessagePayloadPointerOffset);
  const uint32_t guest_payload = payload_pointer ? REX_LOAD_U32(*payload_pointer) : 0;
  const auto address =
      mp64::CheckedGuestAddress(guest_payload, mp64::kConnectionMessageAddressOffset);
  uint32_t matching_peer = 0;
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
    if (guest_peer == 0 || !address) {
      continue;
    }
    PPCContext local_address_ctx = ctx;
    local_address_ctx.r3.u64 = guest_peer;
    __imp__sub_82770AC0(local_address_ctx, base);
    local_address_ctx.r3.u64 = local_address_ctx.r3.u32;
    local_address_ctx.r4.u64 = *address;
    __imp__sub_829E4A70(local_address_ctx, base);
    if (local_address_ctx.r3.u8 != 0) {
      matching_peer = guest_peer;
      break;
    }
  }
  if (matching_peer != 0) {
    SetAliasOverride(mp64::kGlobalPeerManagerAddress, kTemporaryAliasPeerId, matching_peer);
  }
  ctx.r3.u64 = first_argument;
  ctx.r4.u64 = second_argument;
  ctx.r5.u64 = guest_message;
  __imp__sub_826DDD90(ctx, base);
  if (matching_peer != 0) {
    ClearAliasOverride(mp64::kGlobalPeerManagerAddress, kTemporaryAliasPeerId);
  }
}

extern "C" void sub_826DFA28(PPCContext& ctx, uint8_t* base) {
  std::scoped_lock event_lock(g_event_peer_alias_mutex);
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint32_t guest_event = ctx.r4.u32;
  if (guest_event == 0) {
    __imp__sub_826DFA28(ctx, base);
    return;
  }
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_event_manager, mp64::kEventPeerManagerPointerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  mp64::PeerMask64 scope;
  for (uint8_t peer_id = 0; peer_id < mp64::kExtendedPeerCapacity; ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (guest_peer != 0 && IsPeerInEventScope(ctx, base, guest_event, guest_peer)) {
      scope.Set(peer_id);
    }
  }

  bool injected = false;
  uint8_t alias_id = kTemporaryAliasPeerId;
  if (scope.has_nonlegacy16_bits() && scope.legacy_low16() == 0) {
    for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
         ++peer_id) {
      if (!scope.Contains(peer_id)) {
        continue;
      }
      const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
      if (guest_peer != 0) {
        SetAliasOverride(guest_peer_manager, alias_id, guest_peer);
        injected = true;
      }
      break;
    }
  }
  ctx.r3.u64 = guest_event_manager;
  ctx.r4.u64 = guest_event;
  __imp__sub_826DFA28(ctx, base);
  if (injected) {
    ClearAliasOverride(guest_peer_manager, alias_id);
  }
  if (!IsEventQueued(base, guest_event_manager, guest_event)) {
    g_event_scopes.Remove(guest_event);
    return;
  }
  const auto metadata_pointer =
      mp64::CheckedGuestAddress(guest_event, mp64::kEventMetadataPointerOffset);
  const uint32_t guest_metadata = metadata_pointer ? REX_LOAD_U32(*metadata_pointer) : 0;
  const auto mask_address = mp64::CheckedGuestAddress(guest_metadata, mp64::kEventScopeMaskOffset);
  if (mask_address) {
    REX_STORE_U16(*mask_address, scope.legacy_low16());
    g_event_scopes.Set(guest_event, scope);
  }
}

extern "C" void sub_826E3150(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint32_t guest_event = ctx.r4.u32;
  const auto metadata_pointer =
      mp64::CheckedGuestAddress(guest_event, mp64::kEventMetadataPointerOffset);
  const uint32_t guest_metadata = metadata_pointer ? REX_LOAD_U32(*metadata_pointer) : 0;
  const auto mask_address = mp64::CheckedGuestAddress(guest_metadata, mp64::kEventScopeMaskOffset);
  if (!mask_address) {
    __imp__sub_826E3150(ctx, base);
    g_event_scopes.Remove(guest_event);
    return;
  }
  mp64::PeerMask64 scope = g_event_scopes.Get(guest_event);
  const uint16_t stored_low_scope = REX_LOAD_U16(*mask_address);
  // High-peer work is made visible to the retail manager loop with bit 15.
  // Strip that wake marker for every canonical high-peer scope before retail
  // interprets the low mask. If peer 15 is a real recipient it remains set in
  // scope.legacy_low16() and is still delivered exactly once.
  const bool manager_high_scope =
      g_event_manager_tick_active && scope.has_nonlegacy16_bits() &&
      stored_low_scope ==
          static_cast<uint16_t>(scope.legacy_low16() | mp64::kEventScopeHighPeerSentinel);
  if (manager_high_scope) {
    REX_STORE_U16(*mask_address, scope.legacy_low16());
    if (scope.legacy_low16() != 0) {
      __imp__sub_826E3150(ctx, base);
      scope.ReplaceLegacyLow16(REX_LOAD_U16(*mask_address));
    }
  } else {
    __imp__sub_826E3150(ctx, base);
    scope.ReplaceLegacyLow16(REX_LOAD_U16(*mask_address));
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (!scope.Contains(peer_id)) {
      continue;
    }
    const uint32_t guest_peer = EventPeerRecord(base, guest_event_manager, peer_id);
    if (guest_peer == 0) {
      scope.Reset(peer_id);
      continue;
    }
    const uint16_t saved_low_scope = REX_LOAD_U16(*mask_address);
    WithExtendedEventPeer(
        ctx, base, guest_event_manager, guest_peer, peer_id, [&](uint8_t alias_id) {
          const uint16_t alias_scope = static_cast<uint16_t>(uint32_t{1} << alias_id);
          REX_STORE_U16(*mask_address, alias_scope);
          PPCContext send_ctx = ctx;
          send_ctx.r3.u64 = guest_event_manager;
          send_ctx.r4.u64 = guest_event;
          __imp__sub_826E3150(send_ctx, base);
          if ((REX_LOAD_U16(*mask_address) & alias_scope) == 0) {
            validation::PublishMultiplayerValidationStage(
                validation::MultiplayerValidationStage::kFirstExtendedEventSend, peer_id,
                guest_event);
            scope.Reset(peer_id);
          }
        });
    REX_STORE_U16(*mask_address, saved_low_scope);
  }
  if (scope.Count() == 0) {
    g_event_scopes.Remove(guest_event);
  } else {
    g_event_scopes.Set(guest_event, scope);
    if (g_event_manager_tick_active && scope.has_nonlegacy16_bits()) {
      REX_STORE_U16(*mask_address, scope.legacy_low16() | mp64::kEventScopeHighPeerSentinel);
    }
  }
}

extern "C" void sub_826DF530(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  ReleaseEventPeerBuffers(ctx, base, guest_event_manager);
  ctx.r3.u64 = guest_event_manager;
  __imp__sub_826DF530(ctx, base);
}

extern "C" void sub_826DF640(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  const auto peer_id = PeerRecordId(base, guest_peer);
  if (!peer_id || mp64::IsLegacyPeerId(*peer_id)) {
    __imp__sub_826DF640(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(*peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = mp64::kInvalidPeerId;
    return;
  }
  g_event_scopes.ResetPeer(*peer_id);
  ReleaseEventPeerBuffers(ctx, base, guest_event_manager, *peer_id);
  ctx.r3.u64 = *peer_id;
}

extern "C" void sub_826DEE10(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  const auto peer_id = PeerRecordId(base, guest_peer);
  if (!peer_id || mp64::IsLegacyPeerId(*peer_id)) {
    __imp__sub_826DEE10(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(*peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = 0;
    return;
  }
  g_event_scopes.ResetPeer(*peer_id);
  ReleaseEventPeerBuffers(ctx, base, guest_event_manager, *peer_id);
  ctx.r3.u64 = *peer_id;
}

extern "C" void sub_826E2CC8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  RemoveEventScopesForManager(base, guest_event_manager);
  ReleaseEventPeerBuffers(ctx, base, guest_event_manager);
  ctx.r3.u64 = guest_event_manager;
  __imp__sub_826E2CC8(ctx, base);
}

extern "C" void sub_826E2738(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E2738(ctx, base);
    return;
  }
  const uint32_t guest_peer = EventPeerRecord(base, guest_event_manager, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = guest_event_manager;
    return;
  }
  WithExtendedEventPeer(ctx, base, guest_event_manager, guest_peer, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_event_manager;
    ctx.r4.u64 = alias_id;
    __imp__sub_826E2738(ctx, base);
  });
}

extern "C" void sub_826E2918(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E2918(ctx, base);
    return;
  }
  const uint32_t guest_peer = EventPeerRecord(base, guest_event_manager, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = guest_event_manager;
    return;
  }
  WithExtendedEventPeer(ctx, base, guest_event_manager, guest_peer, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_event_manager;
    ctx.r4.u64 = alias_id;
    __imp__sub_826E2918(ctx, base);
  });
}

extern "C" void sub_826E2F50(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E2F50(ctx, base);
    return;
  }
  const uint32_t guest_peer = EventPeerRecord(base, guest_event_manager, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = guest_event_manager;
    return;
  }
  const uint64_t third = ctx.r5.u64;
  const uint64_t fourth = ctx.r6.u64;
  const uint64_t fifth = ctx.r7.u64;
  const uint64_t sixth = ctx.r8.u64;
  const uint64_t seventh = ctx.r9.u64;
  const uint64_t eighth = ctx.r10.u64;
  WithExtendedEventPeer(ctx, base, guest_event_manager, guest_peer, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_event_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = third;
    ctx.r6.u64 = fourth;
    ctx.r7.u64 = fifth;
    ctx.r8.u64 = sixth;
    ctx.r9.u64 = seventh;
    ctx.r10.u64 = eighth;
    __imp__sub_826E2F50(ctx, base);
  });
}

extern "C" void sub_826E3020(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E3020(ctx, base);
    return;
  }
  const uint32_t guest_peer = EventPeerRecord(base, guest_event_manager, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = guest_event_manager;
    return;
  }
  const uint64_t event = ctx.r5.u64;
  WithExtendedEventPeer(ctx, base, guest_event_manager, guest_peer, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_event_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = event;
    __imp__sub_826E3020(ctx, base);
  });
  validation::PublishMultiplayerValidationStage(
      validation::MultiplayerValidationStage::kFirstExtendedEventReceive, peer_id,
      static_cast<uint32_t>(event));
}

extern "C" void sub_826E30A8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E30A8(ctx, base);
    return;
  }
  const uint32_t guest_peer = EventPeerRecord(base, guest_event_manager, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = guest_event_manager;
    return;
  }
  const uint64_t third = ctx.r5.u64;
  const uint64_t fourth = ctx.r6.u64;
  const uint64_t fifth = ctx.r7.u64;
  WithExtendedEventPeer(ctx, base, guest_event_manager, guest_peer, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_event_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = third;
    ctx.r6.u64 = fourth;
    ctx.r7.u64 = fifth;
    __imp__sub_826E30A8(ctx, base);
  });
}

extern "C" void sub_826E3740(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_event_manager = ctx.r3.u32;
  const bool saved_tick = g_event_manager_tick_active;
  g_event_manager_tick_active = true;
  __imp__sub_826E3740(ctx, base);
  g_event_manager_tick_active = saved_tick;
  const uint64_t original_result = ctx.r3.u64;
  for (const auto& [guest_event, scope] : g_event_scopes.Snapshot()) {
    if (!IsEventQueued(base, guest_event_manager, guest_event)) {
      g_event_scopes.Remove(guest_event);
      continue;
    }
    const auto metadata_pointer =
        mp64::CheckedGuestAddress(guest_event, mp64::kEventMetadataPointerOffset);
    const uint32_t guest_metadata = metadata_pointer ? REX_LOAD_U32(*metadata_pointer) : 0;
    const auto mask_address =
        mp64::CheckedGuestAddress(guest_metadata, mp64::kEventScopeMaskOffset);
    if (mask_address) {
      REX_STORE_U16(*mask_address, scope.legacy_low16());
    }
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (EventPeerRecord(base, guest_event_manager, peer_id) == 0) {
      continue;
    }
    PPCContext send_ctx = ctx;
    send_ctx.r3.u64 = guest_event_manager;
    send_ctx.r4.u64 = peer_id;
    sub_826E2738(send_ctx, base);
    send_ctx = ctx;
    send_ctx.r3.u64 = guest_event_manager;
    send_ctx.r4.u64 = peer_id;
    sub_826E2918(send_ctx, base);
  }
  ctx.r3.u64 = original_result;
}

extern "C" void sub_826E7B50(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  __imp__sub_826E7B50(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  std::scoped_lock alias_lock(g_peer_lookup_alias_mutex);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    SetAliasOverride(guest_peer_manager, kTemporaryAliasPeerId, guest_peer);
    PPCContext attempt_ctx = input_ctx;
    __imp__sub_826E7B50(attempt_ctx, base);
    ClearAliasOverride(guest_peer_manager, kTemporaryAliasPeerId);
    if (attempt_ctx.r3.u8 != 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

extern "C" void sub_826E7CC8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_output = ctx.r6.u32;
  __imp__sub_826E7CC8(ctx, base);
  if (guest_output == 0) {
    return;
  }
  uint32_t best_peer = REX_LOAD_U32(guest_output);
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  std::scoped_lock alias_lock(g_peer_lookup_alias_mutex);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t candidate_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (candidate_peer == 0 || candidate_peer == best_peer) {
      continue;
    }
    uint8_t candidate_alias = kTemporaryAliasPeerId;
    uint8_t best_alias = 1;
    const auto current_best_id = PeerRecordId(base, best_peer);
    const bool best_is_extended =
        current_best_id && mp64::ClassifyPeerId(*current_best_id) == mp64::PeerIdClass::kExtended;
    if (!best_is_extended && current_best_id && *current_best_id == candidate_alias) {
      candidate_alias = best_alias;
    }
    if (best_is_extended) {
      SetAliasOverride(guest_peer_manager, best_alias, best_peer);
    }
    SetAliasOverride(guest_peer_manager, candidate_alias, candidate_peer);
    PPCContext attempt_ctx = input_ctx;
    __imp__sub_826E7CC8(attempt_ctx, base);
    const uint32_t selected_peer = REX_LOAD_U32(guest_output);
    ClearAliasOverride(guest_peer_manager, candidate_alias);
    if (best_is_extended) {
      ClearAliasOverride(guest_peer_manager, best_alias);
    }
    if (selected_peer != 0) {
      best_peer = selected_peer;
    }
  }
  REX_STORE_U32(guest_output, best_peer);
  ctx.r3.u64 = best_peer != 0 ? 1 : 0;
}

extern "C" void sub_826E7F48(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_output = ctx.r6.u32;
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr || guest_output == 0) {
    __imp__sub_826E7F48(ctx, base);
    return;
  }
  const uint32_t guest_scratch =
      runtime->memory()->SystemHeapAlloc(mp64::kProximityPeerScratchSize);
  if (guest_scratch == 0) {
    __imp__sub_826E7F48(ctx, base);
    return;
  }
  std::vector<uint32_t> selected;
  selected.reserve(mp64::kExtendedPeerCapacity);
  auto collect = [&](const PPCContext& source_ctx) {
    PPCContext attempt_ctx = source_ctx;
    attempt_ctx.r6.u64 = guest_scratch;
    __imp__sub_826E7F48(attempt_ctx, base);
    const uint32_t count = std::min<uint32_t>(attempt_ctx.r3.u32, mp64::kLegacyPeerCapacity);
    for (uint32_t index = 0; index < count; ++index) {
      const auto peer =
          mp64::CheckedGuestArrayAddress(guest_scratch, index, mp64::kGuestPointerSize);
      const uint32_t guest_peer = peer ? REX_LOAD_U32(*peer) : 0;
      if (guest_peer != 0 &&
          std::find(selected.begin(), selected.end(), guest_peer) == selected.end()) {
        selected.push_back(guest_peer);
      }
    }
  };
  collect(input_ctx);

  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  std::scoped_lock alias_lock(g_peer_lookup_alias_mutex);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t candidate_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (candidate_peer == 0) {
      continue;
    }
    SetAliasOverride(guest_peer_manager, kTemporaryAliasPeerId, candidate_peer);
    collect(input_ctx);
    ClearAliasOverride(guest_peer_manager, kTemporaryAliasPeerId);
  }

  for (size_t index = 0; index < selected.size(); ++index) {
    const auto destination =
        mp64::CheckedGuestArrayAddress(guest_scratch, index, mp64::kGuestPointerSize);
    if (destination) {
      REX_STORE_U32(*destination, selected[index]);
    }
  }
  PPCContext sort_ctx = input_ctx;
  sort_ctx.r3.u64 = guest_scratch;
  sort_ctx.r4.u64 = selected.size();
  sort_ctx.r5.u64 = mp64::kGuestPointerSize;
  sort_ctx.r6.u64 = mp64::kProximityPeerComparatorAddress;
  __imp__sub_82A01A20(sort_ctx, base);
  const size_t output_count = std::min(selected.size(), mp64::kProximityPeerResultCapacity);
  for (size_t index = 0; index < output_count; ++index) {
    const auto source =
        mp64::CheckedGuestArrayAddress(guest_scratch, index, mp64::kGuestPointerSize);
    const auto destination =
        mp64::CheckedGuestArrayAddress(guest_output, index, mp64::kGuestPointerSize);
    if (source && destination) {
      REX_STORE_U32(*destination, REX_LOAD_U32(*source));
    }
  }
  runtime->memory()->SystemHeapFree(guest_scratch);
  ctx.r3.u64 = output_count;
}

extern "C" void sub_826E85B0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  __imp__sub_826E85B0(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  std::scoped_lock alias_lock(g_peer_lookup_alias_mutex);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    SetAliasOverride(guest_peer_manager, kTemporaryAliasPeerId, guest_peer);
    PPCContext attempt_ctx = input_ctx;
    __imp__sub_826E85B0(attempt_ctx, base);
    ClearAliasOverride(guest_peer_manager, kTemporaryAliasPeerId);
    if (attempt_ctx.r3.u8 != 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

extern "C" void sub_826E90C0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  __imp__sub_826E90C0(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  std::scoped_lock alias_lock(g_peer_lookup_alias_mutex);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    SetAliasOverride(guest_peer_manager, kTemporaryAliasPeerId, guest_peer);
    PPCContext attempt_ctx = input_ctx;
    __imp__sub_826E90C0(attempt_ctx, base);
    ClearAliasOverride(guest_peer_manager, kTemporaryAliasPeerId);
    if (attempt_ctx.r3.u8 != 0) {
      ctx = attempt_ctx;
      return;
    }
  }
}

extern "C" void sub_826E81E0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E81E0(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended || guest_object == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint16_t object_id = NetworkObjectId(ctx, base, guest_object);
  const auto threshold_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerMatrixThresholdOffset);
  ctx.r3.u64 = object_id != 0 && threshold_address &&
                       g_object_peer_matrix.Get(guest_object_manager, object_id, peer_id) >=
                           REX_LOAD_U32(*threshold_address)
                   ? 1
                   : 0;
}

extern "C" void sub_826E8EA8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const int16_t requested_object_id = ctx.r4.s16;
  const uint8_t owner_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(owner_id)) {
    __imp__sub_826E8EA8(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(owner_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = 0;
    return;
  }
  uint32_t matching_object = 0;
  ForEachObjectInOwnerList(
      base, g_object_owner_lists.Get(guest_object_manager, owner_id), [&](uint32_t guest_object) {
        if (matching_object == 0 &&
            static_cast<int16_t>(NetworkObjectId(ctx, base, guest_object)) == requested_object_id) {
          matching_object = guest_object;
        }
      });
  ctx.r3.u64 = matching_object;
}

extern "C" void sub_826E8F10(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  __imp__sub_826E8F10(ctx, base);
  const auto joining_peer_id = PeerRecordId(base, guest_peer);
  if (!joining_peer_id || !mp64::IsValidPeerId(*joining_peer_id)) {
    return;
  }
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    if (owner_id == *joining_peer_id) {
      continue;
    }
    ForEachObjectInOwnerList(base, g_object_owner_lists.Get(guest_object_manager, owner_id),
                             [&](uint32_t guest_object) {
                               PPCContext notify_ctx = input_ctx;
                               notify_ctx.r3.u64 = guest_object;
                               notify_ctx.r4.u64 = guest_peer;
                               CallNetworkObjectVirtual(notify_ctx, base, guest_object,
                                                        mp64::kNetworkObjectPeerJoinedVtableOffset);
                             });
  }
}

extern "C" void sub_826E8FF0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  __imp__sub_826E8FF0(ctx, base);
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    PPCContext eligible_ctx = input_ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708620(eligible_ctx, base);
    if (eligible_ctx.r3.u8 == 0) {
      continue;
    }
    eligible_ctx = input_ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708770(eligible_ctx, base);
    if (eligible_ctx.r3.u8 != 0) {
      continue;
    }
    ForEachObjectInOwnerList(base, g_object_owner_lists.Get(guest_object_manager, peer_id),
                             [&](uint32_t guest_object) {
                               PPCContext object_ctx = input_ctx;
                               object_ctx.r3.u64 = guest_object;
                               sub_82702130(object_ctx, base);
                               if (object_ctx.r3.u8 == 0) {
                                 return;
                               }
                               object_ctx = input_ctx;
                               object_ctx.r3.u64 = guest_object;
                               sub_826F5B08(object_ctx, base);
                               if (object_ctx.r3.u8 != 0) {
                                 return;
                               }
                               object_ctx = input_ctx;
                               object_ctx.r3.u64 = guest_object;
                               sub_826F6C18(object_ctx, base);
                             });
  }
}

extern "C" void sub_826E95B8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  PPCContext local_ctx = ctx;
  local_ctx.r3.u64 = mp64::kGlobalPeerManagerAddress;
  __imp__sub_826FDDC0(local_ctx, base);
  const uint8_t local_peer_id = local_ctx.r3.u8;
  if (!mp64::IsValidPeerId(local_peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  uint32_t count = 0;
  if (mp64::IsLegacyPeerId(local_peer_id)) {
    __imp__sub_826E95B8(ctx, base);
    count = ctx.r3.u32;
  }
  const auto threshold_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectPeerMatrixThresholdOffset);
  if (!threshold_address) {
    ctx.r3.u64 = count;
    return;
  }
  const uint32_t threshold = REX_LOAD_U32(*threshold_address);
  ForEachObjectInOwnerList(
      base, ObjectOwnerListFor(base, guest_object_manager, local_peer_id),
      [&](uint32_t guest_object) {
        const uint16_t object_id = NetworkObjectId(input_ctx, base, guest_object);
        if (object_id == 0) {
          return;
        }
        bool legacy_qualified = false;
        for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
          const auto cell = LegacyObjectPeerMatrixAddress(guest_object_manager, object_id, peer_id);
          if (cell && REX_LOAD_U32(*cell) >= threshold) {
            legacy_qualified = true;
            break;
          }
        }
        bool extended_qualified = false;
        for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
             ++peer_id) {
          if (g_object_peer_matrix.Get(guest_object_manager, object_id, peer_id) >= threshold) {
            extended_qualified = true;
            break;
          }
        }
        if ((mp64::IsLegacyPeerId(local_peer_id) && !legacy_qualified && extended_qualified) ||
            (mp64::ClassifyPeerId(local_peer_id) == mp64::PeerIdClass::kExtended &&
             (legacy_qualified || extended_qualified))) {
          ++count;
        }
      });
  ctx.r3.u64 = count;
}

extern "C" void sub_826E9E60(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E9E60(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended ||
      !WithExtendedPlayerTickAlias(base, guest_manager, peer_id, [&](uint8_t alias_id) {
        ctx = input_ctx;
        ctx.r4.u64 = alias_id;
        __imp__sub_826E9E60(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826EAA68(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_manager = ctx.r3.u32;
  if (REX_LOAD_U8(mp64::kPlayerTickDisabledAddress) != 0) {
    return;
  }

  int32_t selected_peer = -1;
  PPCContext predicate_ctx = input_ctx;
  predicate_ctx.r3.u64 = guest_manager;
  sub_826C4CA8(predicate_ctx, base);
  if (predicate_ctx.r3.u8 != 0) {
    const auto cursor_address = mp64::CheckedGuestAddress(guest_manager, 0);
    const auto interval_address = mp64::CheckedGuestAddress(guest_manager, mp64::kGuestPointerSize);
    if (!cursor_address || !interval_address) {
      ctx.r3.u64 = 0;
      return;
    }
    uint32_t interval = REX_LOAD_U32(*interval_address);
    if (interval < mp64::kPlayerTickMinimumInterval) {
      const float delta = std::bit_cast<float>(REX_LOAD_U32(mp64::kPlayerTickDeltaAddress));
      const float scale = std::bit_cast<float>(REX_LOAD_U32(mp64::kPlayerTickScaleAddress));
      const uint32_t decrement = static_cast<uint32_t>(static_cast<int32_t>(delta * scale));
      REX_STORE_U32(*interval_address, interval - decrement);
    } else {
      uint32_t cursor = REX_LOAD_U32(*cursor_address);
      cursor = cursor + 1;
      if (cursor >= mp64::kExtendedPeerCapacity) {
        cursor = 0;
      }
      REX_STORE_U32(*cursor_address, cursor);
      PPCContext info_ctx = input_ctx;
      info_ctx.r3.u64 = cursor;
      sub_8225CF68(info_ctx, base);
      while (info_ctx.r3.u32 == 0) {
        cursor = cursor + 1;
        REX_STORE_U32(*cursor_address, cursor);
        if (cursor >= mp64::kExtendedPeerCapacity) {
          cursor = 0;
          REX_STORE_U32(*cursor_address, cursor);
          break;
        }
        info_ctx = input_ctx;
        info_ctx.r3.u64 = cursor;
        sub_8225CF68(info_ctx, base);
      }
      selected_peer = static_cast<int32_t>(cursor);
      REX_STORE_U32(*interval_address, 0);
    }
  }

  PPCContext global_ctx = input_ctx;
  global_ctx.r3.u64 = mp64::kPlayerTickGlobalActivityAddress;
  sub_8222CAF8(global_ctx, base);
  const bool global_active = global_ctx.r3.u32 != 0;
  for (uint8_t peer_id = 0; peer_id < mp64::kExtendedPeerCapacity; ++peer_id) {
    PPCContext tick_ctx = input_ctx;
    tick_ctx.r3.u64 = guest_manager;
    tick_ctx.r4.u64 = peer_id;
    tick_ctx.r5.u64 = 0;
    tick_ctx.r6.u64 = selected_peer >= 0 && static_cast<uint32_t>(selected_peer) == peer_id ? 1 : 0;
    tick_ctx.r7.u64 = global_active ? 1 : 0;
    sub_826E9E60(tick_ctx, base);
  }

  PPCContext second_pass_ctx = input_ctx;
  second_pass_ctx.r3.u64 = guest_manager;
  sub_826E8450(second_pass_ctx, base);
  if (second_pass_ctx.r3.u8 != 0) {
    for (uint8_t peer_id = 0; peer_id < mp64::kExtendedPeerCapacity; ++peer_id) {
      PPCContext tick_ctx = input_ctx;
      tick_ctx.r3.u64 = guest_manager;
      tick_ctx.r4.u64 = peer_id;
      tick_ctx.r5.u64 = 1;
      tick_ctx.r6.u64 = 0;
      tick_ctx.r7.u64 = 0;
      sub_826E9E60(tick_ctx, base);
      ctx = tick_ctx;
    }
  }
}

extern "C" void sub_826F7050(PPCContext& ctx, uint8_t* base) {
  const ProximityWeightContext saved_context = g_proximity_weight;
  g_proximity_weight = {
      .active = true,
      .extended_injected = false,
  };
  __imp__sub_826F7050(ctx, base);
  g_proximity_weight = saved_context;
}

extern "C" void sub_826E9688(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826E9688(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826E9738(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E9738(ctx, base);
    return;
  }
  if (!WithExtendedObjectOwnerList(ctx, base, guest_object_manager, peer_id, [&](uint8_t alias_id) {
        ctx = input_ctx;
        ctx.r4.u64 = alias_id;
        __imp__sub_826E9738(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826E99A8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826E99A8(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826E9BA8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826E9BA8(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826E9C78(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const int32_t requested_type = ctx.r4.s32;
  __imp__sub_826E9C78(ctx, base);
  uint32_t count = ctx.r3.u32;
  const auto local_peer_id = ObjectManagerLocalPeerId(input_ctx, base, guest_object_manager);
  if (!local_peer_id || !mp64::IsValidPeerId(*local_peer_id)) {
    return;
  }
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    ForEachObjectInOwnerList(base, g_object_owner_lists.Get(guest_object_manager, owner_id),
                             [&](uint32_t guest_object) {
                               PPCContext type_ctx = input_ctx;
                               type_ctx.r3.u64 = guest_object;
                               sub_821778A0(type_ctx, base);
                               if (type_ctx.r3.s32 == requested_type) {
                                 ++count;
                               }
                             });
  }
  ctx.r3.u64 = count;
}

extern "C" void sub_826E9D18(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const auto local_peer_id = ObjectManagerLocalPeerId(ctx, base, guest_object_manager);
  if (!local_peer_id || !mp64::IsValidPeerId(*local_peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager,
                                       [&](uint8_t) {
                                         ctx = input_ctx;
                                         __imp__sub_826E9D18(ctx, base);
                                       }) ||
      ctx.r3.u8 == 0) {
    return;
  }
  bool success = true;
  ForEachObjectInOwnerList(
      base, ObjectOwnerListFor(base, guest_object_manager, *local_peer_id),
      [&](uint32_t guest_object) {
        if (!success) {
          return;
        }
        PPCContext active_ctx = input_ctx;
        active_ctx.r3.u64 = guest_object;
        sub_82702130(active_ctx, base);
        if (active_ctx.r3.u8 == 0) {
          return;
        }
        for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
             ++peer_id) {
          const auto peer_manager_address = mp64::CheckedGuestAddress(
              guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
          const uint32_t guest_peer_manager =
              peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
          const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
          if (guest_peer == 0) {
            continue;
          }
          PPCContext eligible_ctx = input_ctx;
          eligible_ctx.r3.u64 = guest_peer;
          sub_82708620(eligible_ctx, base);
          if (eligible_ctx.r3.u8 == 0) {
            continue;
          }
          eligible_ctx = input_ctx;
          eligible_ctx.r3.u64 = guest_peer;
          sub_82708770(eligible_ctx, base);
          if (eligible_ctx.r3.u8 != 0) {
            continue;
          }
          PPCContext mode_ctx = input_ctx;
          mode_ctx.r3.u64 = guest_object;
          mode_ctx.r4.u64 = 2;
          sub_82701FB0(mode_ctx, base);
          const bool special_mode = mode_ctx.r3.u8 != 0;
          PPCContext flag_ctx = input_ctx;
          flag_ctx.r3.u64 = guest_object;
          flag_ctx.r4.u64 = peer_id;
          sub_82702CD8(flag_ctx, base);
          if (flag_ctx.r3.u8 != 0) {
            if (special_mode) {
              success = false;
              return;
            }
            continue;
          }
          if (special_mode) {
            flag_ctx = input_ctx;
            flag_ctx.r3.u64 = guest_object;
            flag_ctx.r4.u64 = peer_id;
            sub_82702DB8(flag_ctx, base);
            if (flag_ctx.r3.u8 != 0) {
              success = false;
              return;
            }
          } else {
            PPCContext recipient_ctx = input_ctx;
            recipient_ctx.r3.u64 = guest_object;
            recipient_ctx.r4.u64 = peer_id;
            if (CallNetworkObjectVirtual(recipient_ctx, base, guest_object,
                                         mp64::kNetworkObjectRecipientAllowedVtableOffset) &&
                recipient_ctx.r3.u8 != 0) {
              success = false;
              return;
            }
          }
        }
      });
  ctx.r3.u64 = success ? 1 : 0;
}

extern "C" void sub_826EB5D8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826EB5D8(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826EB048(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826EB048(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826EB9F8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826EB9F8(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826EBAA0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826EBAA0(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826EC048(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  if (!WithObjectManagerLocalOwnerList(ctx, base, guest_object_manager, [&](uint8_t) {
        ctx = input_ctx;
        __imp__sub_826EC048(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826ED600(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint64_t object_id = ctx.r4.u64;
  const bool allow_non_authoritative = ctx.r5.u8 != 0;
  __imp__sub_826ED600(ctx, base);
  if (ctx.r3.u32 != 0) {
    return;
  }
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    PPCContext lookup_ctx = input_ctx;
    lookup_ctx.r3.u64 = guest_object_manager;
    lookup_ctx.r4.u64 = object_id;
    lookup_ctx.r5.u64 = owner_id;
    sub_826E8EA8(lookup_ctx, base);
    const uint32_t guest_object = lookup_ctx.r3.u32;
    if (guest_object == 0) {
      continue;
    }
    if (!allow_non_authoritative) {
      PPCContext authoritative_ctx = input_ctx;
      authoritative_ctx.r3.u64 = guest_object;
      authoritative_ctx.r4.u64 = 2;
      sub_82701FB0(authoritative_ctx, base);
      if (authoritative_ctx.r3.u8 == 0) {
        continue;
      }
    }
    ctx.r3.u64 = guest_object;
    return;
  }
}

extern "C" void sub_826EDF48(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  __imp__sub_826EDF48(ctx, base);
  uint32_t missing_count = ctx.r3.u32;
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    PPCContext info_ctx = input_ctx;
    info_ctx.r3.u64 = peer_id;
    sub_8225CF68(info_ctx, base);
    const uint32_t guest_player_info = info_ctx.r3.u32;
    if (guest_player_info == 0 ||
        REX_LOAD_U32(guest_player_info + mp64::kPlayerInfoPlayerPointerOffset) == 0) {
      continue;
    }
    info_ctx.r3.u64 = guest_player_info;
    sub_82238D58(info_ctx, base);
    if (info_ctx.r3.u8 != 0) {
      continue;
    }
    const uint32_t guest_player =
        REX_LOAD_U32(guest_player_info + mp64::kPlayerInfoPlayerPointerOffset);
    const uint32_t guest_network_object =
        guest_player != 0 ? REX_LOAD_U32(guest_player + mp64::kPlayerNetworkObjectOffset) : 0;
    const uint16_t object_id =
        guest_network_object != 0
            ? REX_LOAD_U16(guest_network_object + mp64::kPlayerNetworkObjectIdOffset)
            : 0;
    if (object_id == 0) {
      continue;
    }
    PPCContext find_ctx = input_ctx;
    find_ctx.r3.u64 = guest_object_manager;
    find_ctx.r4.u64 = object_id;
    find_ctx.r5.u64 = 0;
    sub_826ED600(find_ctx, base);
    if (find_ctx.r3.u32 == 0) {
      ++missing_count;
    }
  }
  ctx.r3.u64 = missing_count;
}

extern "C" void sub_826EDFE0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  if (guest_peer_manager == 0) {
    __imp__sub_826EDFE0(ctx, base);
    return;
  }

  // Retail's candidate scratch has room for sixteen pointers and later reads
  // only its first entry. If any low peer is eligible, the unmodified path is
  // already complete and preserves its original ordering.
  for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
    PPCContext lookup_ctx = input_ctx;
    lookup_ctx.r3.u64 = guest_peer_manager;
    lookup_ctx.r4.u64 = peer_id;
    sub_826FE880(lookup_ctx, base);
    if (IsObjectRecoveryPeerEligible(input_ctx, base, guest_object_manager, lookup_ctx.r3.u32,
                                     peer_id)) {
      ctx = input_ctx;
      __imp__sub_826EDFE0(ctx, base);
      return;
    }
  }

  uint8_t actual_peer_id = mp64::kInvalidPeerId;
  uint32_t guest_peer = 0;
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t candidate = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (IsObjectRecoveryPeerEligible(input_ctx, base, guest_object_manager, candidate, peer_id)) {
      actual_peer_id = peer_id;
      guest_peer = candidate;
      break;
    }
  }
  if (guest_peer == 0) {
    ctx = input_ctx;
    __imp__sub_826EDFE0(ctx, base);
    return;
  }

  uint8_t alias_peer_id = kTemporaryAliasPeerId;
  for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
    PPCContext lookup_ctx = input_ctx;
    lookup_ctx.r3.u64 = guest_peer_manager;
    lookup_ctx.r4.u64 = peer_id;
    sub_826FE880(lookup_ctx, base);
    if (lookup_ctx.r3.u32 == 0) {
      alias_peer_id = peer_id;
      break;
    }
  }

  std::scoped_lock alias_lock(g_object_peer_alias_mutex);
  const auto saved_override = GetAliasOverride(guest_peer_manager, alias_peer_id);
  const ObjectRecoveryPeerAliasContext saved_context = g_object_recovery_peer_alias;
  g_object_recovery_peer_alias = {
      .active = true,
      .guest_object_manager = guest_object_manager,
      .actual_peer_id = actual_peer_id,
      .alias_peer_id = alias_peer_id,
  };
  SetAliasOverride(guest_peer_manager, alias_peer_id, guest_peer);
  ctx = input_ctx;
  __imp__sub_826EDFE0(ctx, base);
  if (saved_override) {
    SetAliasOverride(guest_peer_manager, alias_peer_id, *saved_override);
  } else {
    ClearAliasOverride(guest_peer_manager, alias_peer_id);
  }
  g_object_recovery_peer_alias = saved_context;
}

extern "C" void sub_826EE9A8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t owner_id = NetworkObjectOwner(ctx, base, guest_object);
  if (mp64::IsLegacyPeerId(owner_id)) {
    __imp__sub_826EE9A8(ctx, base);
    return;
  }
  if (!WithExtendedObjectOwnerList(ctx, base, guest_object_manager, owner_id, [&](uint8_t) {
        ctx.r3.u64 = guest_object_manager;
        ctx.r4.u64 = guest_object;
        __imp__sub_826EE9A8(ctx, base);
      })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826EEC10(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint64_t third_argument = ctx.r5.u64;
  const uint16_t object_id = NetworkObjectId(ctx, base, guest_object);
  const uint8_t owner_id = NetworkObjectOwner(ctx, base, guest_object);
  if (mp64::IsLegacyPeerId(owner_id)) {
    __imp__sub_826EEC10(ctx, base);
  } else if (!WithExtendedObjectOwnerList(ctx, base, guest_object_manager, owner_id, [&](uint8_t) {
               ctx.r3.u64 = guest_object_manager;
               ctx.r4.u64 = guest_object;
               ctx.r5.u64 = third_argument;
               __imp__sub_826EEC10(ctx, base);
             })) {
    ctx.r3.u64 = 0;
    return;
  }
  if (object_id != 0) {
    g_object_peer_matrix.ClearObject(guest_object_manager, object_id);
  }
}

extern "C" void sub_826EEFD0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  __imp__sub_826EEFD0(ctx, base);
  std::vector<uint32_t> removable;
  removable.reserve(mp64::kObjectRemovalBatchCapacity);
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity;
       owner_id < mp64::kExtendedPeerCapacity &&
       removable.size() < mp64::kObjectRemovalBatchCapacity;
       ++owner_id) {
    ForEachObjectInOwnerList(base, g_object_owner_lists.Get(guest_object_manager, owner_id),
                             [&](uint32_t guest_object) {
                               if (removable.size() >= mp64::kObjectRemovalBatchCapacity) {
                                 return;
                               }
                               PPCContext authoritative_ctx = input_ctx;
                               authoritative_ctx.r3.u64 = guest_object;
                               authoritative_ctx.r4.u64 = 2;
                               sub_82701FB0(authoritative_ctx, base);
                               if (authoritative_ctx.r3.u8 != 0) {
                                 removable.push_back(guest_object);
                               }
                             });
  }
  for (const uint32_t guest_object : removable) {
    PPCContext remove_ctx = input_ctx;
    remove_ctx.r3.u64 = guest_object_manager;
    remove_ctx.r4.u64 = guest_object;
    remove_ctx.r5.u64 = 0;
    sub_826EEC10(remove_ctx, base);
  }
}

extern "C" void sub_826F2358(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F2358(ctx, base);
    return;
  }
  const uint16_t object_id = NetworkObjectId(ctx, base, guest_object);
  bool invoked = false;
  WithExtendedObjectManagerPeer(ctx, base, guest_object_manager, peer_id, [&](uint8_t alias_id) {
    invoked =
        WithExtendedObjectMatrixCell(base, guest_object_manager, object_id, peer_id, alias_id, [&] {
          ctx.r3.u64 = guest_object_manager;
          ctx.r4.u64 = guest_object;
          ctx.r5.u64 = alias_id;
          __imp__sub_826F2358(ctx, base);
        });
  });
  if (!invoked) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826F2870(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F2870(ctx, base);
    return;
  }
  const uint16_t object_id = NetworkObjectId(ctx, base, guest_object);
  bool invoked = false;
  WithExtendedObjectManagerPeer(ctx, base, guest_object_manager, peer_id, [&](uint8_t alias_id) {
    invoked =
        WithExtendedObjectMatrixCell(base, guest_object_manager, object_id, peer_id, alias_id, [&] {
          ctx.r3.u64 = guest_object_manager;
          ctx.r4.u64 = guest_object;
          ctx.r5.u64 = alias_id;
          __imp__sub_826F2870(ctx, base);
        });
  });
  if (!invoked) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826F3668(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint16_t object_id = NetworkObjectId(ctx, base, guest_object);
  ctx = input_ctx;
  __imp__sub_826F3668(ctx, base);
  if (ctx.r3.u8 != 0 && object_id != 0) {
    g_object_peer_matrix.ClearObject(guest_object_manager, object_id);
  }
}

extern "C" void sub_826EFE08(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  if (!peer_manager_address) {
    ctx.r3.u64 = 0;
    return;
  }
  PPCContext peer_id_ctx = input_ctx;
  peer_id_ctx.r3.u64 = REX_LOAD_U32(*peer_manager_address);
  peer_id_ctx.r4.u64 = input_ctx.r4.u64;
  sub_826FF108(peer_id_ctx, base);
  const int32_t resolved_peer_id = peer_id_ctx.r3.s32;
  if (resolved_peer_id >= 0 && resolved_peer_id < static_cast<int32_t>(mp64::kLegacyPeerCapacity)) {
    __imp__sub_826EFE08(ctx, base);
    return;
  }
  if (resolved_peer_id < static_cast<int32_t>(mp64::kLegacyPeerCapacity) ||
      resolved_peer_id >= static_cast<int32_t>(mp64::kExtendedPeerCapacity) ||
      !WithExtendedObjectManagerPeer(
          ctx, base, guest_object_manager, static_cast<uint8_t>(resolved_peer_id),
          [&](uint8_t) {
            ctx = input_ctx;
            __imp__sub_826EFE08(ctx, base);
          })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826F3DD0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const bool remove_expired = ctx.r4.u8 != 0;
  const bool time_changed = (REX_LOAD_U32(mp64::kObjectUpdateFlagsAddress) & 1) != 0 &&
                            REX_LOAD_U32(mp64::kObjectUpdatePreviousTimeAddress) !=
                                REX_LOAD_U32(mp64::kObjectUpdateCurrentTimeAddress);
  __imp__sub_826F3DD0(ctx, base);

  if (remove_expired && time_changed) {
    for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
         ++owner_id) {
      ForEachObjectInOwnerList(
          base, g_object_owner_lists.Get(guest_object_manager, owner_id),
          [&](uint32_t guest_object) {
            const auto payload_address =
                mp64::CheckedGuestAddress(guest_object, mp64::kGuestPointerSize);
            if (!payload_address || REX_LOAD_U32(*payload_address) == 0) {
              return;
            }
            PPCContext expired_ctx = input_ctx;
            expired_ctx.r3.u64 = guest_object;
            if (!CallNetworkObjectVirtual(expired_ctx, base, guest_object,
                                          mp64::kNetworkObjectExpiredVtableOffset) ||
                expired_ctx.r3.u8 == 0) {
              return;
            }
            PPCContext remove_ctx = input_ctx;
            remove_ctx.r3.u64 = guest_object_manager;
            remove_ctx.r4.u64 = guest_object;
            remove_ctx.r5.u64 = 0;
            remove_ctx.r6.u64 = 0;
            sub_826EF7D8(remove_ctx, base);
          });
    }
  }

  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  const uint32_t guest_peer_manager =
      peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(guest_peer_manager, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    PPCContext eligible_ctx = input_ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708620(eligible_ctx, base);
    if (eligible_ctx.r3.u8 == 0) {
      continue;
    }
    eligible_ctx = input_ctx;
    eligible_ctx.r3.u64 = guest_peer;
    sub_82708770(eligible_ctx, base);
    if (eligible_ctx.r3.u8 != 0) {
      continue;
    }
    PPCContext address_ctx = input_ctx;
    address_ctx.r3.u64 = guest_peer;
    sub_82A58008(address_ctx, base);
    const uint32_t guest_address = address_ctx.r3.u32;
    address_ctx.r3.u64 = guest_address;
    sub_826C4FA0(address_ctx, base);
    if (address_ctx.r3.u8 == 0) {
      continue;
    }
    PPCContext update_ctx = input_ctx;
    update_ctx.r3.u64 = guest_object_manager;
    update_ctx.r4.u64 = guest_peer;
    sub_826F3B38(update_ctx, base);

    address_ctx = input_ctx;
    address_ctx.r3.u64 = guest_address;
    sub_8278CEB8(address_ctx, base);
    if (address_ctx.r3.u8 != 0) {
      continue;
    }
    update_ctx = input_ctx;
    update_ctx.r3.u64 = guest_object_manager;
    update_ctx.r4.u64 = peer_id;
    sub_826F0390(update_ctx, base);
    update_ctx = input_ctx;
    update_ctx.r3.u64 = guest_object_manager;
    update_ctx.r4.u64 = peer_id;
    sub_826F0520(update_ctx, base);
    update_ctx = input_ctx;
    update_ctx.r3.u64 = guest_object_manager;
    update_ctx.r4.u64 = guest_address;
    sub_826EFE08(update_ctx, base);
  }
}

extern "C" void sub_826F0B38(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const auto initialized_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerInitializedFlagOffset);
  if (initialized_address && REX_LOAD_U8(*initialized_address) != 0) {
    for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
         ++owner_id) {
      uint32_t attempts = 0;
      while (attempts < mp64::kObjectPeerMatrixObjectCapacity) {
        const mp64::ObjectOwnerList list = g_object_owner_lists.Get(guest_object_manager, owner_id);
        if (list.guest_head == 0) {
          break;
        }
        const auto object_address =
            mp64::CheckedGuestAddress(list.guest_head, mp64::kObjectOwnerListNodeObjectOffset);
        const uint32_t guest_object = object_address ? REX_LOAD_U32(*object_address) : 0;
        if (guest_object == 0) {
          g_object_owner_lists.ClearOwner(guest_object_manager, owner_id);
          break;
        }
        PPCContext remove_ctx = input_ctx;
        remove_ctx.r3.u64 = guest_object_manager;
        remove_ctx.r4.u64 = guest_object;
        remove_ctx.r5.u64 = 1;
        remove_ctx.r6.u64 = 0;
        sub_826EF7D8(remove_ctx, base);
        const mp64::ObjectOwnerList after =
            g_object_owner_lists.Get(guest_object_manager, owner_id);
        if (after.guest_head == list.guest_head) {
          break;
        }
        ++attempts;
      }
    }
  }
  ctx = input_ctx;
  __imp__sub_826F0B38(ctx, base);
}

extern "C" void sub_826E8278(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint8_t requested_peer_id = GuestPeerId(ctx.r4.u64);
  const uint8_t peer_id = ObjectManagerTimingPeerId(guest_object_manager, requested_peer_id);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E8278(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended) {
    g_object_peer_timings.SetLastReceived(guest_object_manager, peer_id,
                                          REX_LOAD_U32(mp64::kNetworkClockAddress));
  }
  ctx.r3.u64 = guest_object_manager;
}

extern "C" void sub_826E82A8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint8_t requested_peer_id = GuestPeerId(ctx.r4.u64);
  const uint8_t peer_id = ObjectManagerTimingPeerId(guest_object_manager, requested_peer_id);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E82A8(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = guest_object_manager;
    return;
  }
  PPCContext channel_ctx = ctx;
  channel_ctx.r3.u64 = guest_object_manager;
  channel_ctx.r4.u64 = ctx.r5.u32;
  __imp__sub_826E8160(channel_ctx, base);
  if (channel_ctx.r3.u32 < mp64::kObjectManagerChannelCount) {
    g_object_peer_timings.SetChannelReceived(guest_object_manager, peer_id, channel_ctx.r3.u32,
                                             REX_LOAD_U32(mp64::kNetworkClockAddress));
  }
  ctx.r3.u64 = guest_object_manager;
}

extern "C" void sub_826E8308(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint8_t requested_peer_id = GuestPeerId(ctx.r4.u64);
  const uint8_t peer_id = ObjectManagerTimingPeerId(guest_object_manager, requested_peer_id);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E8308(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t now = REX_LOAD_U32(mp64::kNetworkClockAddress);
  const uint32_t last = g_object_peer_timings.Get(guest_object_manager, peer_id).last_received;
  const uint32_t elapsed = now > last ? now - last : 0;
  const uint32_t threshold_offset = ctx.r5.u8 != 0 ? mp64::kObjectManagerLongPeerTimeoutOffset
                                                   : mp64::kObjectManagerShortPeerTimeoutOffset;
  const auto threshold_address = mp64::CheckedGuestAddress(guest_object_manager, threshold_offset);
  ctx.r3.u64 = threshold_address && elapsed > REX_LOAD_U32(*threshold_address) ? 1 : 0;
}

extern "C" void sub_826E8380(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint8_t requested_peer_id = GuestPeerId(ctx.r4.u64);
  const uint8_t peer_id = ObjectManagerTimingPeerId(guest_object_manager, requested_peer_id);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826E8380(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = 0;
    return;
  }
  PPCContext channel_ctx = ctx;
  channel_ctx.r3.u64 = guest_object_manager;
  channel_ctx.r4.u64 = ctx.r5.u32;
  __imp__sub_826E8160(channel_ctx, base);
  if (channel_ctx.r3.u32 >= mp64::kObjectManagerChannelCount) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t last =
      g_object_peer_timings.Get(guest_object_manager, peer_id).channel_received[channel_ctx.r3.u32];
  const uint32_t now = REX_LOAD_U32(mp64::kNetworkClockAddress);
  ctx.r3.u64 = now > last && now - last > mp64::kObjectManagerChannelTimeout ? 1 : 0;
}

extern "C" void sub_826F0390(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F0390(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    __imp__sub_826F0390(ctx, base);
  });
}

extern "C" void sub_826F0520(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F0520(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    __imp__sub_826F0520(ctx, base);
  });
}

extern "C" void sub_826F0840(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  const uint64_t third = ctx.r5.u64;
  const uint64_t fourth = ctx.r6.u64;
  const uint64_t fifth = ctx.r7.u64;
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F0840(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = third;
    ctx.r6.u64 = fourth;
    ctx.r7.u64 = fifth;
    __imp__sub_826F0840(ctx, base);
  });
}

// The packet builder resolves a connection key to a peer ID before directly
// indexing its 1,032-byte message and 16-bit sequence tables. During a scoped
// high-peer projection, return the physical alias ID to that generated index;
// all peer identity and accounting hooks continue to expose the actual ID.
extern "C" void sub_826FF108(PPCContext& ctx, uint8_t* base) {
  __imp__sub_826FF108(ctx, base);
  if (g_object_peer_alias.active && ctx.r3.u8 == g_object_peer_alias.actual_peer_id) {
    ctx.r3.u64 = g_object_peer_alias.alias_peer_id;
  }
}

extern "C" void sub_826F1158(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
  if (!peer_manager_address) {
    ctx.r3.u64 = 0;
    return;
  }
  PPCContext peer_id_ctx = input_ctx;
  peer_id_ctx.r3.u64 = REX_LOAD_U32(*peer_manager_address);
  peer_id_ctx.r4.u64 = input_ctx.r4.u64;
  sub_826FF108(peer_id_ctx, base);
  const int32_t resolved_peer_id = peer_id_ctx.r3.s32;
  if (resolved_peer_id >= 0 && resolved_peer_id < static_cast<int32_t>(mp64::kLegacyPeerCapacity)) {
    __imp__sub_826F1158(ctx, base);
    return;
  }
  if (resolved_peer_id < static_cast<int32_t>(mp64::kLegacyPeerCapacity) ||
      resolved_peer_id >= static_cast<int32_t>(mp64::kExtendedPeerCapacity) ||
      !WithExtendedObjectManagerPeer(
          ctx, base, guest_object_manager, static_cast<uint8_t>(resolved_peer_id),
          [&](uint8_t) {
            ctx = input_ctx;
            __imp__sub_826F1158(ctx, base);
          })) {
    ctx.r3.u64 = 0;
  }
}

extern "C" void sub_826F13C0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  const std::array<uint64_t, 5> arguments = {ctx.r5.u64, ctx.r6.u64, ctx.r7.u64, ctx.r8.u64,
                                             ctx.r9.u64};
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F13C0(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = arguments[0];
    ctx.r6.u64 = arguments[1];
    ctx.r7.u64 = arguments[2];
    ctx.r8.u64 = arguments[3];
    ctx.r9.u64 = arguments[4];
    __imp__sub_826F13C0(ctx, base);
  });
}

extern "C" void sub_826F1468(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  const uint64_t third = ctx.r5.u64;
  const uint64_t fourth = ctx.r6.u64;
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F1468(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = third;
    ctx.r6.u64 = fourth;
    __imp__sub_826F1468(ctx, base);
  });
}

extern "C" void sub_826F14E8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  const uint64_t third = ctx.r5.u64;
  const uint64_t fourth = ctx.r6.u64;
  const uint64_t fifth = ctx.r7.u64;
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F14E8(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = third;
    ctx.r6.u64 = fourth;
    ctx.r7.u64 = fifth;
    __imp__sub_826F14E8(ctx, base);
  });
}

extern "C" void sub_826F1570(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  const uint64_t third = ctx.r5.u64;
  const uint64_t fourth = ctx.r6.u64;
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F1570(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = third;
    ctx.r6.u64 = fourth;
    __imp__sub_826F1570(ctx, base);
  });
}

extern "C" void sub_826F15F0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  const uint64_t third = ctx.r5.u64;
  const uint64_t fourth = ctx.r6.u64;
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_826F15F0(ctx, base);
    return;
  }
  WithExtendedObjectManagerPeer(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
    ctx.r3.u64 = guest_manager;
    ctx.r4.u64 = alias_id;
    ctx.r5.u64 = third;
    ctx.r6.u64 = fourth;
    __imp__sub_826F15F0(ctx, base);
  });
}

extern "C" void sub_826F4448(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  ReleaseObjectPeerBuffers(ctx, base, guest_object_manager);
  g_player_tick_states.RemoveManager(guest_object_manager);
  g_object_peer_timings.RemoveManager(guest_object_manager);
  g_object_owner_lists.RemoveManager(guest_object_manager);
  g_object_peer_matrix.RemoveManager(guest_object_manager);
  ctx.r3.u64 = guest_object_manager;
  __imp__sub_826F4448(ctx, base);
}

// The one-time object-manager initializer constructs only its sixteen inline
// buffers and tables. Extended storage is lazy, but must start from a clean
// lifecycle state if the guest address is reused after teardown.
extern "C" void sub_826ED450(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  const auto initialized =
      mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerInitializedFlagOffset);
  if (initialized && REX_LOAD_U8(*initialized) == 0) {
    ReleaseObjectPeerBuffers(ctx, base, guest_object_manager);
    g_player_tick_states.RemoveManager(guest_object_manager);
    g_object_peer_timings.RemoveManager(guest_object_manager);
    g_object_owner_lists.RemoveManager(guest_object_manager);
    g_object_peer_matrix.RemoveManager(guest_object_manager);
  }
  ctx.r3.u64 = guest_object_manager;
  __imp__sub_826ED450(ctx, base);
}

extern "C" void sub_826F2040(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object_manager = ctx.r3.u32;
  ctx.r3.u64 = guest_object_manager;
  __imp__sub_826F2040(ctx, base);
  ReleaseObjectPeerBuffers(ctx, base, guest_object_manager);
  g_player_tick_states.RemoveManager(guest_object_manager);
  g_object_peer_timings.RemoveManager(guest_object_manager);
  g_object_owner_lists.RemoveManager(guest_object_manager);
  g_object_peer_matrix.RemoveManager(guest_object_manager);
}

extern "C" void sub_826EF9F8(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_object_manager = ctx.r3.u32;
  const uint32_t guest_departing_peer = ctx.r4.u32;
  const auto peer_id = PeerRecordId(base, guest_departing_peer);
  if (!peer_id || mp64::IsLegacyPeerId(*peer_id)) {
    __imp__sub_826EF9F8(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended) {
    const auto peer_manager_address =
        mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerPeerManagerOffset);
    const uint32_t guest_peer_manager =
        peer_manager_address ? REX_LOAD_U32(*peer_manager_address) : 0;
    PPCContext local_ctx = input_ctx;
    local_ctx.r3.u64 = guest_peer_manager;
    __imp__sub_826FDDC0(local_ctx, base);
    const uint8_t local_peer_id = local_ctx.r3.u8;
    for (uint8_t owner_id = 0; owner_id < mp64::kExtendedPeerCapacity; ++owner_id) {
      ForEachObjectInOwnerList(base, ObjectOwnerListFor(base, guest_object_manager, owner_id),
                               [&](uint32_t guest_object) {
                                 PPCContext notify_ctx = input_ctx;
                                 notify_ctx.r3.u64 = guest_object;
                                 notify_ctx.r4.u64 = guest_departing_peer;
                                 CallNetworkObjectVirtual(
                                     notify_ctx, base, guest_object,
                                     mp64::kNetworkObjectPeerDepartedVtableOffset);
                                 if (owner_id != local_peer_id) {
                                   return;
                                 }
                                 PPCContext predicate_ctx = input_ctx;
                                 predicate_ctx.r3.u64 = guest_object;
                                 predicate_ctx.r4.u64 = 2;
                                 sub_82701FB0(predicate_ctx, base);
                                 if (predicate_ctx.r3.u8 == 0) {
                                   return;
                                 }
                                 predicate_ctx = input_ctx;
                                 predicate_ctx.r3.u64 = guest_object;
                                 sub_82702D08(predicate_ctx, base);
                                 if (predicate_ctx.r3.u8 != 0) {
                                   return;
                                 }
                                 predicate_ctx = input_ctx;
                                 predicate_ctx.r3.u64 = guest_object;
                                 sub_82702CA0(predicate_ctx, base);
                                 if (predicate_ctx.r3.u8 != 0) {
                                   return;
                                 }
                                 predicate_ctx = input_ctx;
                                 predicate_ctx.r3.u64 = guest_object;
                                 sub_82702D78(predicate_ctx, base);
                                 if (predicate_ctx.r3.u8 != 0) {
                                   return;
                                 }
                                 PPCContext remove_ctx = input_ctx;
                                 remove_ctx.r3.u64 = guest_object_manager;
                                 remove_ctx.r4.u64 = guest_object;
                                 remove_ctx.r5.u64 = 0;
                                 sub_826EEC10(remove_ctx, base);
                               });
    }
    g_player_tick_states.RemovePeer(guest_object_manager, *peer_id);
    g_object_peer_timings.RemovePeer(guest_object_manager, *peer_id);
    g_object_peer_matrix.ClearPeer(guest_object_manager, *peer_id);
    FreeObjectPeerBuffers(ctx, base,
                          g_object_peer_buffers.RemovePeer(guest_object_manager, *peer_id));
    const auto reassignment_manager =
        mp64::CheckedGuestAddress(guest_object_manager, mp64::kObjectManagerReassignmentOffset);
    if (reassignment_manager) {
      PPCContext reassignment_ctx = input_ctx;
      reassignment_ctx.r3.u64 = *reassignment_manager;
      reassignment_ctx.r4.u64 = guest_departing_peer;
      sub_82787340(reassignment_ctx, base);
    }
  }
  ctx.r3.u64 = peer_id.value_or(mp64::kInvalidPeerId);
}

extern "C" void sub_826FE880(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);

  if (g_extended_add.active && guest_manager == g_extended_add.guest_manager) {
    if (peer_id == kTemporaryAliasPeerId) {
      if (!g_extended_add.initial_alias_lookup_consumed) {
        g_extended_add.initial_alias_lookup_consumed = true;
        ctx.r3.u64 = 0;
      } else {
        ctx.r3.u64 = g_extended_add.saved_alias_peer;
      }
      return;
    }
    if (peer_id == g_extended_add.actual_peer_id && g_extended_add.constructed) {
      ctx.r3.u64 = g_extended_add.guest_record;
      return;
    }
  }

  if (g_extended_remove.active && guest_manager == g_extended_remove.guest_manager) {
    if (peer_id == kTemporaryAliasPeerId) {
      if (!g_extended_remove.initial_alias_lookup_consumed) {
        g_extended_remove.initial_alias_lookup_consumed = true;
        ctx.r3.u64 = g_extended_remove.guest_record;
      } else {
        ctx.r3.u64 = g_extended_remove.saved_alias_peer;
      }
      return;
    }
    if (peer_id == g_extended_remove.actual_peer_id) {
      ctx.r3.u64 = g_extended_remove.guest_record;
      return;
    }
  }

  if (const auto alias = GetAliasOverride(guest_manager, peer_id)) {
    ctx.r3.u64 = *alias;
    return;
  }

  switch (mp64::ClassifyPeerId(peer_id)) {
    case mp64::PeerIdClass::kLegacy:
      __imp__sub_826FE880(ctx, base);
      g_peer_managers.SetPeer(guest_manager, peer_id, ctx.r3.u32);
      return;
    case mp64::PeerIdClass::kExtended:
      ctx.r3.u64 = g_peer_managers.GetPeer(guest_manager, peer_id);
      return;
    case mp64::PeerIdClass::kInvalid:
      ctx.r3.u64 = 0;
      return;
  }
}

// Peer lookup by the record's title-owned numeric connection key. Retail
// scans only the embedded sixteen-pointer table. Preserve its ordering, then
// inspect extended records through the same record accessor.
extern "C" void sub_826FE808(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const int32_t connection_key = ctx.r4.s32;
  __imp__sub_826FE808(ctx, base);
  if (ctx.r3.u32 != 0 || connection_key < 0) {
    return;
  }

  uint32_t match = 0;
  g_peer_managers.VisitExtendedPeers(guest_manager, [&](uint8_t, uint32_t guest_peer) {
    PPCContext nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_peer;
    __imp__sub_82A58008(nested_ctx, base);
    if (nested_ctx.r3.s32 != connection_key) {
      return true;
    }
    match = guest_peer;
    return false;
  });
  ctx.r3.u64 = match;
}

// These identity lookups scan the inline pointer table directly after their
// local-peer fast path. Let retail resolve the local/low population first,
// then continue through the canonical sidecar without ever indexing manager
// memory with a high ID.
extern "C" void sub_826FE908(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_identity = ctx.r4.u32;
  __imp__sub_826FE908(ctx, base);
  if (ctx.r3.u32 != 0) {
    return;
  }
  const auto flags_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPeerManagerFlagsOffset);
  if (!flags_address ||
      (REX_LOAD_U8(*flags_address) & mp64::kPeerManagerIdentityLookupEnabledFlag) == 0) {
    return;
  }
  uint32_t match = 0;
  g_peer_managers.VisitExtendedPeers(guest_manager, [&](uint8_t, uint32_t guest_peer) {
    PPCContext player_info_ctx = ctx;
    player_info_ctx.r3.u64 = guest_peer;
    sub_82708A48(player_info_ctx, base);
    if (player_info_ctx.r3.u32 == 0) {
      return true;
    }
    PPCContext compare_ctx = ctx;
    compare_ctx.r3.u64 = player_info_ctx.r3.u32;
    compare_ctx.r4.u64 = guest_identity;
    __imp__sub_829DB628(compare_ctx, base);
    if (compare_ctx.r3.u8 == 0) {
      return true;
    }
    match = guest_peer;
    return false;
  });
  ctx.r3.u64 = match;
}

extern "C" void sub_826FE9C8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_identity = ctx.r4.u32;
  __imp__sub_826FE9C8(ctx, base);
  if (ctx.r3.u32 != 0) {
    return;
  }
  const auto flags_address =
      mp64::CheckedGuestAddress(guest_manager, mp64::kPeerManagerFlagsOffset);
  if (!flags_address ||
      (REX_LOAD_U8(*flags_address) & mp64::kPeerManagerIdentityLookupEnabledFlag) == 0) {
    return;
  }
  uint32_t match = 0;
  g_peer_managers.VisitExtendedPeers(guest_manager, [&](uint8_t, uint32_t guest_peer) {
    PPCContext identity_ctx = ctx;
    identity_ctx.r3.u64 = guest_peer;
    sub_82708A78(identity_ctx, base);
    if (identity_ctx.r3.u32 != guest_identity) {
      return true;
    }
    match = guest_peer;
    return false;
  });
  ctx.r3.u64 = match;
}

// Clone construction contains two direct sixteen-peer loops. Keep the retail
// object creation and command path intact while marking its exact endpoint
// build/type-query seams for the extended work performed below.
extern "C" void sub_826EFF58(PPCContext& ctx, uint8_t* base) {
  const ClonePeerExpansionContext saved_context = g_clone_peer_expansion;
  g_clone_peer_expansion.active = true;
  __imp__sub_826EFF58(ctx, base);
  g_clone_peer_expansion = saved_context;
}

// Include sidecar peers in the title's identity/value uniqueness assignment.
// The retail function owns substantial unrelated command-building logic, so
// keep it intact. A scoped accessor context makes a high match visible only at
// its two direct sixteen-peer scans. If the retail table contains no peer
// other than the target, one saved pointer slot is mirrored for the duration
// of the call so the original loop still reaches an accessor seam.
extern "C" void sub_826FFCB0(PPCContext& ctx, uint8_t* base) {
  const PPCContext input_ctx = ctx;
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t target_peer_id = GuestPeerId(ctx.r4.u64);
  if (guest_manager == 0 || !mp64::IsValidPeerId(target_peer_id) || g_peer_uniqueness.active) {
    __imp__sub_826FFCB0(ctx, base);
    return;
  }

  std::scoped_lock scan_lock(g_peer_lookup_alias_mutex);
  PPCContext lookup_ctx = input_ctx;
  lookup_ctx.r3.u64 = guest_manager;
  lookup_ctx.r4.u64 = target_peer_id;
  sub_826FE880(lookup_ctx, base);
  const uint32_t guest_target_peer = lookup_ctx.r3.u32;
  if (guest_target_peer == 0) {
    __imp__sub_826FFCB0(ctx, base);
    return;
  }

  const uint64_t target_identity = PeerRecordIdentity(input_ctx, base, guest_target_peer);
  bool retail_identity_match = false;
  const auto local_peer =
      mp64::CheckedGuestAddress(guest_manager, mp64::kInlineLocalPeerPointerOffset);
  if (local_peer) {
    retail_identity_match = PeerRecordIdentity(input_ctx, base, *local_peer) == target_identity;
  }

  size_t direct_scan_peer_count = 0;
  std::optional<uint32_t> saved_pointer_address;
  uint32_t saved_pointer = 0;
  for (uint8_t peer_id = 0; peer_id < mp64::kLegacyPeerCapacity; ++peer_id) {
    const auto pointer_offset = mp64::LegacyPeerPointerOffset(peer_id);
    const auto pointer_address =
        pointer_offset ? mp64::CheckedGuestAddress(guest_manager, *pointer_offset) : std::nullopt;
    if (!pointer_address) {
      continue;
    }
    const uint32_t guest_peer = REX_LOAD_U32(*pointer_address);
    if (!saved_pointer_address && (guest_peer == 0 || guest_peer == guest_target_peer)) {
      saved_pointer_address = pointer_address;
      saved_pointer = guest_peer;
    }
    if (guest_peer == 0 || guest_peer == guest_target_peer) {
      continue;
    }
    ++direct_scan_peer_count;
    if (PeerRecordIdentity(input_ctx, base, guest_peer) == target_identity) {
      retail_identity_match = true;
    }
  }

  PeerUniquenessContext call_context;
  call_context.active = true;
  call_context.target_identity = target_identity;
  uint32_t first_high_peer = 0;
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(guest_manager, peer_id);
    if (guest_peer == 0 || guest_peer == guest_target_peer) {
      continue;
    }
    const uint32_t guest_player_info = PeerRecordPlayerInfo(input_ctx, base, guest_peer);
    const auto unique_value = PeerUniqueValue(input_ctx, base, guest_peer);
    if (guest_player_info == 0 || !unique_value) {
      continue;
    }
    if (first_high_peer == 0) {
      first_high_peer = guest_peer;
    }
    if (call_context.high_value_count < call_context.high_values.size()) {
      call_context.high_values[call_context.high_value_count++] = {
          .value = *unique_value,
          .guest_peer = guest_peer,
          .guest_player_info = guest_player_info,
      };
    }
    if (!retail_identity_match && call_context.matching_identity_player_info == 0 &&
        PeerRecordIdentity(input_ctx, base, guest_peer) == target_identity) {
      call_context.matching_identity_player_info = guest_player_info;
    }
  }

  if (direct_scan_peer_count == 0 && first_high_peer != 0 && saved_pointer_address) {
    REX_STORE_U32(*saved_pointer_address, first_high_peer);
    ++direct_scan_peer_count;
  }
  if (call_context.matching_identity_player_info != 0) {
    call_context.identity_calls_until_injection = direct_scan_peer_count;
  }

  const PeerUniquenessContext saved_context = g_peer_uniqueness;
  g_peer_uniqueness = call_context;
  __imp__sub_826FFCB0(ctx, base);
  g_peer_uniqueness = saved_context;
  if (saved_pointer_address) {
    REX_STORE_U32(*saved_pointer_address, saved_pointer);
  }
}

extern "C" void sub_827087E8(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller_return_address = ctx.lr;
  __imp__sub_827087E8(ctx, base);
  if (!g_peer_uniqueness.active ||
      caller_return_address != mp64::kPeerUniquenessIdentityScanReturnAddress ||
      g_peer_uniqueness.matching_identity_player_info == 0 ||
      g_peer_uniqueness.identity_calls_until_injection == 0) {
    return;
  }
  --g_peer_uniqueness.identity_calls_until_injection;
  if (g_peer_uniqueness.identity_calls_until_injection == 0) {
    ctx.r3.u64 = g_peer_uniqueness.target_identity;
    g_peer_uniqueness.force_identity_player_info = true;
  }
}

extern "C" void sub_827087D8(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller_return_address = ctx.lr;
  __imp__sub_827087D8(ctx, base);
  if (!g_peer_uniqueness.active) {
    return;
  }
  if (caller_return_address == mp64::kPeerUniquenessIdentityInfoReturnAddress &&
      g_peer_uniqueness.force_identity_player_info) {
    ctx.r3.u64 = g_peer_uniqueness.matching_identity_player_info;
    g_peer_uniqueness.force_identity_player_info = false;
    return;
  }
  if (caller_return_address != mp64::kPeerUniquenessCandidateInfoReturnAddress) {
    return;
  }
  for (size_t index = 0; index < g_peer_uniqueness.high_value_count; ++index) {
    const PeerUniquenessValue& high_value = g_peer_uniqueness.high_values[index];
    if (high_value.value == ctx.r29.s32) {
      ctx.r3.u64 = high_value.guest_player_info;
      return;
    }
  }
}

// Enumerate embedded and sidecar peers without exceeding the caller-owned
// guest buffer. The retail voice update owns a sixteen-pointer stack window,
// so this seam preserves the local peer and round-robins the remaining fifteen
// entries across the complete 64-peer population. Every connected peer is
// serviced over successive voice ticks without writing beyond that stack
// window; callers with a 64-entry buffer receive the full population at once.
extern "C" void sub_826FEBE8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_output = ctx.r4.u32;
  const uint32_t output_capacity = ctx.r5.u32;
  if (guest_output == 0 || output_capacity == 0) {
    ctx.r3.u64 = 0;
    return;
  }

  __imp__sub_826FEBE8(ctx, base);
  size_t written = std::min<size_t>(ctx.r3.u32, output_capacity);
  const size_t extended_count = g_peer_managers.CountExtendedPeers(guest_manager);

  std::vector<uint32_t> peers;
  peers.reserve(mp64::kExtendedPeerCapacity);
  for (size_t index = 0; index < written; ++index) {
    const auto address =
        mp64::CheckedGuestArrayAddress(guest_output, index, mp64::kGuestPointerSize);
    if (!address) {
      break;
    }
    const uint32_t guest_peer = REX_LOAD_U32(*address);
    if (guest_peer != 0) {
      peers.push_back(guest_peer);
    }
  }
  g_peer_managers.VisitExtendedPeers(guest_manager, [&](uint8_t, uint32_t guest_peer) {
    peers.push_back(guest_peer);
    return true;
  });

  if (ctx.lr == kVoiceEnumerationReturnAddress && extended_count != 0 && !peers.empty()) {
    const auto local_peer_address =
        mp64::CheckedGuestAddress(guest_manager, mp64::kInlineLocalPeerPointerOffset);
    const uint32_t local_peer = local_peer_address.value_or(0);
    const auto local_it = std::find(peers.begin(), peers.end(), local_peer);
    if (local_it != peers.end()) {
      std::iter_swap(peers.begin(), local_it);
    }
    const size_t local_count = local_it == peers.end() ? 0 : 1;
    const size_t rotating_population = peers.size() - local_count;
    size_t rotating_start = 0;
    {
      std::scoped_lock cursor_lock(g_voice_cursor_mutex);
      auto& cursor = g_voice_cursors[guest_manager];
      rotating_start = cursor.start(rotating_population);
      const size_t rotating_width =
          output_capacity > local_count ? output_capacity - local_count : 0;
      cursor.Advance(rotating_population, rotating_width);
    }

    written = 0;
    if (local_count != 0 && written < output_capacity) {
      REX_STORE_U32(guest_output, peers.front());
      ++written;
    }
    while (written < output_capacity && written - local_count < rotating_population) {
      const size_t rotating_offset = written - local_count;
      const size_t source_index =
          local_count + ((rotating_start + rotating_offset) % rotating_population);
      const auto address =
          mp64::CheckedGuestArrayAddress(guest_output, written, mp64::kGuestPointerSize);
      if (!address) {
        break;
      }
      REX_STORE_U32(*address, peers[source_index]);
      ++written;
    }
  } else {
    while (written < output_capacity && written < peers.size()) {
      const auto address =
          mp64::CheckedGuestArrayAddress(guest_output, written, mp64::kGuestPointerSize);
      if (!address) {
        break;
      }
      REX_STORE_U32(*address, peers[written]);
      ++written;
    }
  }
  ctx.r3.u64 = written;
}

// Report the real sidecar-aware count while leaving the embedded free-list
// bookkeeping untouched.
extern "C" void sub_826FEAE0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  __imp__sub_826FEAE0(ctx, base);
  const size_t total = std::min<size_t>(
      static_cast<size_t>(ctx.r3.u32) + g_peer_managers.CountExtendedPeers(guest_manager),
      mp64::kExtendedPeerCapacity);
  ctx.r3.u64 = total;
}

// ADDING_PEER. Extended records live in guest system-heap memory. Temporarily
// place the new record at the head of the retail free list and substitute peer
// ID zero only for the generated direct pointer-table store. Constructor and
// notification hooks restore the actual ID before any later callbacks, and the
// saved legacy slot is restored before returning. Thus the original add logic
// runs in full without ever indexing manager memory with 16..63.
extern "C" void sub_82700108(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_add_record = ctx.r6.u32;
  if (guest_manager == 0 || guest_add_record == 0) {
    ctx.r3.u64 = 0;
    return;
  }

  const uint8_t peer_id = REX_LOAD_U8(guest_add_record);
  const mp64::PeerIdClass peer_class = mp64::ClassifyPeerId(peer_id);
  if (peer_class == mp64::PeerIdClass::kInvalid) {
    WarnInvalidPeerOnce(g_invalid_add_peer_warning, "sub_82700108", peer_id,
                        "peer ID is outside 0..63 or is the 0xFF sentinel");
    ctx.r3.u64 = 0;
    return;
  }
  if (peer_class == mp64::PeerIdClass::kLegacy) {
    __imp__sub_82700108(ctx, base);
    if (ctx.r3.u32 != 0) {
      g_peer_managers.SetPeer(guest_manager, peer_id, ctx.r3.u32);
    }
    return;
  }

  if (!g_peer_managers.HasManager(guest_manager) ||
      g_peer_managers.GetPeer(guest_manager, peer_id) != 0 || g_extended_add.active ||
      g_extended_remove.active) {
    ctx.r3.u64 = 0;
    return;
  }

  rex::Runtime* runtime = rex::Runtime::instance();
  const auto alias_table = LegacyAliasTableAddress(guest_manager);
  if (runtime == nullptr || alias_table == std::nullopt) {
    ctx.r3.u64 = 0;
    return;
  }

  const uint32_t guest_peer = runtime->memory()->SystemHeapAlloc(mp64::kPeerRecordSize);
  if (guest_peer == 0) {
    WarnRuntimeFailureOnce(g_extended_peer_allocation_warning, "sub_82700108", peer_id,
                           "guest system heap allocation failed");
    ctx.r3.u64 = 0;
    return;
  }
  std::memset(REX_RAW_ADDR(guest_peer), 0, mp64::kPeerRecordSize);
  if (!InsertExtendedPeerAtFreeListHead(base, guest_manager, guest_peer)) {
    runtime->memory()->SystemHeapFree(guest_peer);
    ctx.r3.u64 = 0;
    return;
  }

  const uint32_t saved_alias_peer = REX_LOAD_U32(*alias_table);
  SetAliasOverride(guest_manager, saved_alias_peer);
  REX_STORE_U8(guest_add_record, kTemporaryAliasPeerId);
  g_extended_add = {
      .active = true,
      .initial_alias_lookup_consumed = false,
      .constructed = false,
      .notification_boundary_reached = false,
      .guest_manager = guest_manager,
      .guest_record = guest_peer,
      .guest_add_packet = guest_add_record,
      .saved_alias_peer = saved_alias_peer,
      .actual_peer_id = peer_id,
  };
  validation::PublishMultiplayerValidationStage(
      validation::MultiplayerValidationStage::kExtendedPeerAdd, peer_id, guest_peer);

  __imp__sub_82700108(ctx, base);
  const bool constructed = g_extended_add.constructed;
  const bool complete =
      constructed && g_extended_add.notification_boundary_reached && ctx.r3.u32 == guest_peer;
  REX_STORE_U8(guest_add_record, peer_id);
  REX_STORE_U32(*alias_table, saved_alias_peer);
  ClearAliasOverride(guest_manager);
  g_extended_add = {};

  if (!complete) {
    if (!constructed) {
      RemoveInjectedFreePeer(ctx, base, guest_manager, guest_peer);
      ResetAndFreeExtendedPeer(ctx, base, guest_peer);
    } else {
      PPCContext remove_ctx = ctx;
      remove_ctx.r3.u64 = guest_manager;
      remove_ctx.r4.u64 = peer_id;
      sub_82700E30(remove_ctx, base);
    }
    ctx.r3.u64 = 0;
  }
}

// REMOVING_PEER uses the same saved table alias. The original function runs
// every callback and resets the peer record, while the free-list insertion
// hook suppresses returning a heap-backed record to the embedded 16-record
// pool. The allocation is released after the original teardown completes.
extern "C" void sub_82700E30(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  const mp64::PeerIdClass peer_class = mp64::ClassifyPeerId(peer_id);
  if (peer_class == mp64::PeerIdClass::kInvalid) {
    ctx.r3.u64 = 0;
    return;
  }
  if (peer_class == mp64::PeerIdClass::kLegacy) {
    validation::PublishMultiplayerValidationStage(
        validation::MultiplayerValidationStage::kRemoval, peer_id);
    __imp__sub_82700E30(ctx, base);
    g_peer_managers.RemovePeer(guest_manager, peer_id);
    return;
  }

  const uint32_t guest_peer = g_peer_managers.GetPeer(guest_manager, peer_id);
  const auto alias_table = LegacyAliasTableAddress(guest_manager);
  if (guest_peer == 0 || alias_table == std::nullopt || g_extended_add.active ||
      g_extended_remove.active) {
    ctx.r3.u64 = 0;
    return;
  }

  const uint32_t saved_alias_peer = REX_LOAD_U32(*alias_table);
  SetAliasOverride(guest_manager, saved_alias_peer);
  g_extended_remove = {
      .active = true,
      .initial_alias_lookup_consumed = false,
      .guest_manager = guest_manager,
      .guest_record = guest_peer,
      .saved_alias_peer = saved_alias_peer,
      .actual_peer_id = peer_id,
  };
  ctx.r4.u64 = kTemporaryAliasPeerId;
  __imp__sub_82700E30(ctx, base);
  REX_STORE_U32(*alias_table, saved_alias_peer);
  ClearAliasOverride(guest_manager);
  g_extended_remove = {};
  validation::PublishMultiplayerValidationStage(
      validation::MultiplayerValidationStage::kRemoval, peer_id, guest_peer);
  if (PlayerInfoForId(base, peer_id).guest_player_info != 0) {
    PPCContext destroy_player_ctx = ctx;
    destroy_player_ctx.r3.u64 = peer_id;
    sub_82261CA8(destroy_player_ctx, base);
  }
  g_peer_managers.RemovePeer(guest_manager, peer_id);
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    runtime->memory()->SystemHeapFree(guest_peer);
  }
}

// Extended ADDING_PEER calls the original 64-byte record constructor with the
// temporary alias. Restore the real ID immediately after construction and
// publish the record to sidecar lookups before subsequent callbacks run.
extern "C" void sub_827088F0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_peer = ctx.r3.u32;
  __imp__sub_827088F0(ctx, base);
  if (!g_extended_add.active || guest_peer != g_extended_add.guest_record) {
    return;
  }
  const auto peer_id_address = mp64::CheckedGuestAddress(guest_peer, 16);
  if (!peer_id_address) {
    return;
  }
  REX_STORE_U8(*peer_id_address, g_extended_add.actual_peer_id);
  if (g_peer_managers.SetPeer(g_extended_add.guest_manager, g_extended_add.actual_peer_id,
                              guest_peer)) {
    g_extended_add.constructed = true;
    validation::PublishMultiplayerValidationStage(
        validation::MultiplayerValidationStage::kExtendedPeerPublication,
        g_extended_add.actual_peer_id, guest_peer);
  }
}

// Player-info creation. Legacy IDs retain the retail global-table path.
// Extended IDs receive the same 1456-byte guest allocation and field layout,
// while their pointer and generation live in a host sidecar instead of writing
// beyond the adjacent sixteen-entry globals.
extern "C" void sub_8225CD80(PPCContext& ctx, uint8_t* base) {
  static_cast<void>(base);
  auto alias_gate = g_player_info_alias_gate.EnterRead();
  const uint32_t guest_player = ctx.r3.u32;
  for (uint8_t player_id = 0; player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    const auto player_info = PlayerInfoForId(base, player_id);
    const uint32_t guest_player_info = player_info.guest_player_info;
    if (guest_player_info != 0 &&
        REX_LOAD_U32(guest_player_info + mp64::kPlayerInfoPlayerPointerOffset) == guest_player) {
      ctx.r3.u64 = player_id;
      return;
    }
  }
  ctx.r3.s64 = -1;
}

extern "C" void sub_8225CE98(PPCContext& ctx, uint8_t* base) {
  static_cast<void>(base);
  auto alias_gate = g_player_info_alias_gate.EnterRead();
  const uint32_t primary_player = REX_LOAD_U32(mp64::kPrimaryPlayerIdAddress);
  uint32_t count = 0;
  for (uint8_t player_id = 0; player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    const auto player_info = PlayerInfoForId(base, player_id);
    const uint32_t guest_player_info = player_info.guest_player_info;
    if (guest_player_info != 0 && player_id != primary_player &&
        REX_LOAD_U32(guest_player_info + mp64::kPlayerInfoStateOffset) != 6) {
      ++count;
    }
  }
  ctx.r3.u64 = count;
}

extern "C" void sub_82260D20(PPCContext& ctx, uint8_t* base) {
  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82260D20);
  const PPCContext result_ctx = ctx;
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    const uint32_t guest_player_info = g_player_infos.Get(player_id).guest_player_info;
    if (guest_player_info == 0) {
      continue;
    }
    const uint32_t guest_player =
        REX_LOAD_U32(guest_player_info + mp64::kPlayerInfoPlayerPointerOffset);
    if (guest_player == 0 ||
        (REX_LOAD_U8(guest_player + mp64::kPlayerEntityFirstCleanupFlagOffset) != 0 &&
         REX_LOAD_U8(guest_player + mp64::kPlayerEntitySecondCleanupFlagOffset) != 0)) {
      continue;
    }
    PPCContext update_ctx = result_ctx;
    update_ctx.r3.u64 = guest_player_info;
    sub_822398C8(update_ctx, base);
  }
  ctx = result_ctx;
}

extern "C" void sub_82261E48(PPCContext& ctx, uint8_t* base) {
  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82261E48);
  std::vector<uint8_t> extended_players;
  const auto entries = g_player_infos.Snapshot();
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    if (entries[player_id].guest_player_info != 0) {
      extended_players.push_back(player_id);
    }
  }
  for (uint8_t player_id : extended_players) {
    PPCContext destroy_ctx = ctx;
    destroy_ctx.r3.u64 = player_id;
    sub_82261CA8(destroy_ctx, base);
  }
  SyncPlayerInfoShadow(base);
}

extern "C" void sub_82261EA8(PPCContext& ctx, uint8_t* base) {
  const uint8_t source_id = GuestPeerId(ctx.r3.u64);
  const uint8_t destination_id = GuestPeerId(ctx.r4.u64);
  if (!mp64::IsValidPeerId(source_id) || !mp64::IsValidPeerId(destination_id) ||
      source_id == destination_id) {
    return;
  }
  if (mp64::IsLegacyPeerId(source_id) && mp64::IsLegacyPeerId(destination_id)) {
    auto alias_gate = g_player_info_alias_gate.EnterOpaque();
    std::scoped_lock alias_lock(g_player_info_alias_mutex);
    RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82261EA8);
    SyncPlayerInfoShadow(base);
    return;
  }

  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);

  const auto moving = PlayerInfoForId(base, source_id);
  if (moving.guest_player_info == 0) {
    return;
  }
  if (PlayerInfoForId(base, destination_id).guest_player_info != 0) {
    PPCContext destroy_ctx = ctx;
    destroy_ctx.r3.u64 = destination_id;
    sub_82261CA8(destroy_ctx, base);
  }

  if (mp64::IsLegacyPeerId(source_id)) {
    if (const auto source = mp64::LegacyPlayerInfoPointerAddress(source_id)) {
      REX_STORE_U32(*source, 0);
    }
  } else {
    g_player_infos.Remove(source_id);
  }
  if (mp64::IsLegacyPeerId(destination_id)) {
    const auto destination = mp64::LegacyPlayerInfoPointerAddress(destination_id);
    const auto generation = mp64::LegacyPlayerInfoGenerationAddress(destination_id);
    if (destination && generation) {
      REX_STORE_U32(*destination, moving.guest_player_info);
      REX_STORE_U32(*generation, moving.generation);
    }
  } else if (!g_player_infos.Set(destination_id, moving)) {
    return;
  }
  const int32_t moving_team = g_peer_teams.Get(source_id);
  g_peer_teams.Clear(source_id);
  g_peer_teams.Set(destination_id, moving_team);

  REX_STORE_U8(moving.guest_player_info + mp64::kPlayerInfoPlayerIdOffset, destination_id);
  if (REX_LOAD_U32(mp64::kPrimaryPlayerIdAddress) == source_id) {
    REX_STORE_U32(mp64::kPrimaryPlayerIdAddress, destination_id);
  }
  if (REX_LOAD_U32(mp64::kSecondaryPlayerIdAddress) == source_id) {
    REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, destination_id);
  }
  const uint32_t guest_player =
      REX_LOAD_U32(moving.guest_player_info + mp64::kPlayerInfoPlayerPointerOffset);
  if (guest_player != 0) {
    PPCContext notify_ctx = ctx;
    notify_ctx.r3.u64 = guest_player;
    sub_823CF550(notify_ctx, base);
  }
  SyncPlayerInfoShadow(base);
}

extern "C" void sub_822619E8(PPCContext& ctx, uint8_t* base) {
  const uint8_t player_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(player_id)) {
    auto alias_gate = g_player_info_alias_gate.EnterOpaque();
    std::scoped_lock alias_lock(g_player_info_alias_mutex);
    RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_822619E8);
    return;
  }
  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);
  const mp64::PlayerInfoEntry entry = g_player_infos.Get(player_id);
  if (entry.guest_player_info == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t saved_pointer = REX_LOAD_U32(mp64::kLegacyPlayerInfoPointerTableAddress);
  const uint32_t saved_generation =
      REX_LOAD_U32(mp64::kLegacyPlayerInfoGenerationTableAddress);
  const LocalPlayerInfoAliasContext saved_context = g_local_player_info_alias;
  REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, entry.guest_player_info);
  REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, entry.generation);
  g_local_player_info_alias = {
      .active = true,
      .actual_player_id = player_id,
      .alias_player_id = kTemporaryAliasPeerId,
      .guest_player_info = entry.guest_player_info,
  };
  ScopeExit restore_alias([&] {
    REX_STORE_U8(entry.guest_player_info + mp64::kPlayerInfoPlayerIdOffset, player_id);
    g_local_player_info_alias = saved_context;
    REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, saved_pointer);
    REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, saved_generation);
  });
  ctx.r4.u64 = kTemporaryAliasPeerId;
  __imp__sub_822619E8(ctx, base);
}

extern "C" void sub_82263000(PPCContext& ctx, uint8_t* base) {
  const uint8_t player_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(player_id)) {
    auto alias_gate = g_player_info_alias_gate.EnterOpaque();
    std::scoped_lock alias_lock(g_player_info_alias_mutex);
    RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82263000);
    SyncPlayerInfoShadow(base);
    return;
  }
  if (!mp64::IsValidPeerId(player_id) || ctx.r3.u32 == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);
  if (g_player_infos.Get(player_id).guest_player_info != 0) {
    PPCContext destroy_ctx = ctx;
    destroy_ctx.r3.u64 = player_id;
    sub_82261CA8(destroy_ctx, base);
  }

  const bool becomes_primary = REX_LOAD_U8(ctx.r3.u32) == 0 && REX_LOAD_U8(ctx.r3.u32 + 1) == 0;
  const uint32_t saved_pointer = REX_LOAD_U32(mp64::kLegacyPlayerInfoPointerTableAddress);
  const uint32_t saved_generation =
      REX_LOAD_U32(mp64::kLegacyPlayerInfoGenerationTableAddress);
  const uint32_t saved_primary = REX_LOAD_U32(mp64::kPrimaryPlayerIdAddress);
  const uint32_t saved_secondary = REX_LOAD_U32(mp64::kSecondaryPlayerIdAddress);
  const LocalPlayerInfoAliasContext saved_context = g_local_player_info_alias;
  bool alias_restored = false;
  ScopeExit restore_alias_on_failure([&] {
    if (!alias_restored) {
      REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, saved_pointer);
      REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, saved_generation);
      REX_STORE_U32(mp64::kPrimaryPlayerIdAddress, saved_primary);
      REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, saved_secondary);
      g_local_player_info_alias = saved_context;
    }
  });
  REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, 0);
  REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, 0);
  ctx.r4.u64 = kTemporaryAliasPeerId;
  __imp__sub_82263000(ctx, base);
  const uint32_t guest_player_info = REX_LOAD_U32(mp64::kLegacyPlayerInfoPointerTableAddress);
  const uint32_t generation = REX_LOAD_U32(mp64::kLegacyPlayerInfoGenerationTableAddress);
  REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, saved_pointer);
  REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, saved_generation);
  REX_STORE_U32(mp64::kPrimaryPlayerIdAddress,
                becomes_primary ? static_cast<uint32_t>(player_id) : saved_primary);
  REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, saved_secondary);
  g_local_player_info_alias = saved_context;
  alias_restored = true;
  if (guest_player_info != 0) {
    REX_STORE_U8(guest_player_info + mp64::kPlayerInfoPlayerIdOffset, player_id);
    g_player_infos.Set(player_id, {guest_player_info, generation});
    g_peer_teams.Set(
        player_id,
        static_cast<int32_t>(REX_LOAD_U32(guest_player_info + 1384)));
  }
  SyncPlayerInfoShadow(base);
}

extern "C" void sub_822636F0(PPCContext& ctx, uint8_t* base) {
  RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_822636F0);
  auto player_info_read = g_player_info_alias_gate.EnterRead();
  const int32_t retail_result = ctx.r3.s32;
  uint32_t best_generation = std::numeric_limits<uint32_t>::max();
  if (retail_result >= 0 && retail_result < mp64::kLegacyPeerCapacity) {
    best_generation = PlayerInfoForId(base, static_cast<uint8_t>(retail_result)).generation;
  }
  uint8_t best_player = mp64::kInvalidPeerId;
  for (uint8_t player_id = mp64::kLegacyPeerCapacity;
       player_id < mp64::kExtendedPeerCapacity; ++player_id) {
    const mp64::PlayerInfoEntry entry = g_player_infos.Get(player_id);
    if (entry.guest_player_info == 0 ||
        REX_LOAD_U32(entry.guest_player_info + mp64::kPlayerInfoStateOffset) == 6 ||
        entry.generation >= best_generation) {
      continue;
    }
    best_generation = entry.generation;
    best_player = player_id;
  }
  if (best_player != mp64::kInvalidPeerId) {
    ctx.r3.u64 = best_player;
  }
}

extern "C" void sub_826DAD70(PPCContext& ctx, uint8_t* base) {
  if (ctx.lr == mp64::kPlayerInfoNetworkArrayRegisterReturnAddress &&
      ctx.r4.u32 == mp64::kLegacyPlayerInfoPointerTableAddress &&
      ctx.r5.u32 == mp64::kLegacyPeerCapacity) {
    {
      std::scoped_lock shadow_lock(g_player_info_shadow_mutex);
      if (g_guest_player_info_shadow == 0) {
        if (rex::Runtime* runtime = rex::Runtime::instance()) {
          g_guest_player_info_shadow =
              runtime->memory()->SystemHeapAlloc(mp64::kExtendedPlayerInfoShadowTableSize);
        }
      }
    }
    SyncPlayerInfoShadow(base);
    if (g_guest_player_info_shadow != 0) {
      ctx.r4.u64 = g_guest_player_info_shadow;
      ctx.r5.u64 = mp64::kExtendedPeerCapacity;
    }
  }
  __imp__sub_826DAD70(ctx, base);
}

extern "C" void sub_82263200(PPCContext& ctx, uint8_t* base) {
  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);
  uint8_t peer_id = GuestPeerId(ctx.r3.u64);
  const int32_t initial_team = ctx.r6.s32;
  if (g_extended_add.active && g_extended_add.constructed &&
      ctx.r7.u32 == g_extended_add.guest_record) {
    REX_STORE_U8(g_extended_add.guest_add_packet, g_extended_add.actual_peer_id);
    if (const auto alias_table = LegacyAliasTableAddress(g_extended_add.guest_manager)) {
      REX_STORE_U32(*alias_table, g_extended_add.saved_alias_peer);
      ClearAliasOverride(g_extended_add.guest_manager);
    }
    peer_id = g_extended_add.actual_peer_id;
    ctx.r3.u64 = peer_id;
  }

  const mp64::PeerIdClass peer_class = mp64::ClassifyPeerId(peer_id);
  if (peer_class == mp64::PeerIdClass::kLegacy) {
    RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82263200);
    if (ctx.r3.u32 != 0) {
      g_peer_teams.Set(peer_id, initial_team);
      validation::PublishMultiplayerValidationStage(
          validation::MultiplayerValidationStage::kPlayerInfoConstruction, peer_id, ctx.r3.u32);
    }
    SyncPlayerInfoShadow(base);
    return;
  }
  if (peer_class == mp64::PeerIdClass::kInvalid || ctx.r4.u32 == 0) {
    WarnInvalidPeerOnce(g_player_info_table_warning, "sub_82263200", peer_id,
                        "invalid player-info ID or null 96-byte payload");
    ctx.r3.u64 = 0;
    return;
  }

  if (g_player_infos.Get(peer_id).guest_player_info != 0) {
    PPCContext destroy_ctx = ctx;
    destroy_ctx.r3.u64 = peer_id;
    sub_82261CA8(destroy_ctx, base);
  }

  const uint32_t payload = ctx.r4.u32;
  const uint32_t value_1380 = ctx.r5.u32;
  const uint32_t value_1384 = ctx.r6.u32;
  const uint32_t value_1376 = ctx.r7.u32;
  const uint32_t value_1232 = ctx.r8.u32;
  PPCContext nested_ctx = ctx;
  nested_ctx.r3.u64 = mp64::kPlayerInfoSize;
  __imp__sub_821B3510(nested_ctx, base);
  uint32_t guest_player_info = nested_ctx.r3.u32;
  if (guest_player_info != 0) {
    nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_player_info;
    __imp__sub_82239828(nested_ctx, base);
    guest_player_info = nested_ctx.r3.u32;
  }
  if (guest_player_info == 0) {
    ctx.r3.u64 = 0;
    return;
  }

  uint32_t generation = REX_LOAD_U32(mp64::kPlayerInfoGenerationCounterAddress);
  ++generation;
  REX_STORE_U32(mp64::kPlayerInfoGenerationCounterAddress, generation);
  std::memcpy(REX_RAW_ADDR(guest_player_info), REX_RAW_ADDR(payload), mp64::kPlayerInfoPayloadSize);
  REX_STORE_U8(guest_player_info + 1230, peer_id);
  REX_STORE_U32(guest_player_info + 1380, value_1380);
  REX_STORE_U32(guest_player_info + 1384, value_1384);
  REX_STORE_U32(guest_player_info + 1376, value_1376);
  REX_STORE_U32(guest_player_info + 1232, value_1232);

  if (!g_player_infos.Set(peer_id, {guest_player_info, generation})) {
    nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_player_info;
    __imp__sub_821B3560(nested_ctx, base);
    ctx.r3.u64 = 0;
    return;
  }
  g_peer_teams.Set(peer_id, static_cast<int32_t>(value_1384));
  ctx.r3.u64 = guest_player_info;
  validation::PublishMultiplayerValidationStage(
      validation::MultiplayerValidationStage::kPlayerInfoConstruction, peer_id,
      guest_player_info);
  if (g_extended_add.active && peer_id == g_extended_add.actual_peer_id) {
    g_extended_add.notification_boundary_reached = true;
  }
  SyncPlayerInfoShadow(base);
}

// This is the canonical player-info-by-ID accessor. It has no retail bounds
// check, so a remote/sparse peer ID must never reach its base + id * 4 load.
// Extended records resolve their heap-backed player-info through the sidecar.
extern "C" void sub_8225CF68(PPCContext& ctx, uint8_t* base) {
  auto alias_gate = g_player_info_alias_gate.EnterRead();
  const uint8_t peer_id = GuestPeerId(ctx.r3.u64);
  if (g_player_transition_loop.active) {
    if (ctx.lr == mp64::kTransitionResetAccessorReturnAddress) {
      g_player_transition_loop.reset_loop_seen = true;
    } else if (ctx.lr == mp64::kTransitionForceAccessorReturnAddress) {
      g_player_transition_loop.force_loop_seen = true;
    }
  }
  if (g_threshold_player_loop.active && ctx.lr == mp64::kThresholdAccessorReturnAddress) {
    g_threshold_player_loop.seen = true;
    g_threshold_player_loop.excluded_unique_value = ctx.r25.s32;
    g_threshold_player_loop.guest_match_list = ctx.r29.u32;
  }
  if (g_lobby_position_loop.active && ctx.lr == mp64::kLobbyPositionAccessorReturnAddress) {
    g_lobby_position_loop.seen = true;
  }
  if (g_proximity_weight.active && peer_id == mp64::kLastLegacyPeerId) {
    InjectExtendedProximityWeights(ctx, base);
  }
  switch (mp64::ClassifyPeerId(peer_id)) {
    case mp64::PeerIdClass::kLegacy:
      if (g_player_info_batch_projection.active) {
        ctx.r3.u64 = PlayerInfoForId(
                         base, g_player_info_batch_projection.actual_ids[peer_id])
                         .guest_player_info;
        return;
      }
      if (g_local_player_info_alias.active &&
          peer_id == g_local_player_info_alias.alias_player_id) {
        ctx.r3.u64 = g_local_player_info_alias.guest_player_info;
        return;
      }
      if (g_player_tick_alias.active && peer_id == g_player_tick_alias.alias_peer_id) {
        ctx.r3.u64 = g_player_tick_alias.guest_player_info;
        return;
      }
      if (peer_id == kTemporaryAliasPeerId &&
          g_player_info_alias_active.load(std::memory_order_acquire)) {
        ctx.r3.u64 = g_player_info_alias_legacy_zero.load(std::memory_order_relaxed);
        return;
      }
      __imp__sub_8225CF68(ctx, base);
      return;
    case mp64::PeerIdClass::kExtended:
      ctx.r3.u64 = g_player_infos.Get(peer_id).guest_player_info;
      return;
    case mp64::PeerIdClass::kInvalid:
      ctx.r3.u64 = 0;
      return;
  }
}

// Player-info destruction. The retail destructor contains substantial logic
// beyond freeing memory. Run it against a serialized slot-zero alias, preserve
// every slot-zero/global-ID value it may touch, then discard the sidecar entry.
extern "C" void sub_82261CA8(PPCContext& ctx, uint8_t* base) {
  const uint8_t peer_id = GuestPeerId(ctx.r3.u64);
  const mp64::PeerIdClass peer_class = mp64::ClassifyPeerId(peer_id);
  if (peer_class == mp64::PeerIdClass::kLegacy) {
    auto alias_gate = g_player_info_alias_gate.EnterOpaque();
    std::scoped_lock alias_lock(g_player_info_alias_mutex);
    RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82261CA8);
    g_peer_teams.Clear(peer_id);
    SyncPlayerInfoShadow(base);
    return;
  }
  if (peer_class == mp64::PeerIdClass::kInvalid) {
    return;
  }

  auto alias_gate = g_player_info_alias_gate.EnterOpaque();
  std::scoped_lock alias_lock(g_player_info_alias_mutex);
  const mp64::PlayerInfoEntry entry = g_player_infos.Get(peer_id);
  if (entry.guest_player_info == 0) {
    return;
  }
  const uint32_t legacy_player_info = REX_LOAD_U32(mp64::kLegacyPlayerInfoPointerTableAddress);
  const uint32_t legacy_generation = REX_LOAD_U32(mp64::kLegacyPlayerInfoGenerationTableAddress);
  const uint32_t primary_player_id = REX_LOAD_U32(mp64::kPrimaryPlayerIdAddress);
  const uint32_t secondary_player_id = REX_LOAD_U32(mp64::kSecondaryPlayerIdAddress);
  const uint32_t saved_alias_legacy_zero =
      g_player_info_alias_legacy_zero.load(std::memory_order_relaxed);
  const bool saved_alias_active = g_player_info_alias_active.load(std::memory_order_acquire);
  g_player_info_alias_legacy_zero.store(legacy_player_info, std::memory_order_relaxed);
  g_player_info_alias_active.store(true, std::memory_order_release);
  REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, entry.guest_player_info);
  REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, entry.generation);
  {
    ScopeExit restore_alias([&] {
      REX_STORE_U32(mp64::kLegacyPlayerInfoPointerTableAddress, legacy_player_info);
      REX_STORE_U32(mp64::kLegacyPlayerInfoGenerationTableAddress, legacy_generation);
      REX_STORE_U32(mp64::kPrimaryPlayerIdAddress, primary_player_id);
      REX_STORE_U32(mp64::kSecondaryPlayerIdAddress, secondary_player_id);
      g_player_info_alias_legacy_zero.store(saved_alias_legacy_zero, std::memory_order_relaxed);
      g_player_info_alias_active.store(saved_alias_active, std::memory_order_release);
    });
    ctx.r3.u64 = kTemporaryAliasPeerId;
    __imp__sub_82261CA8(ctx, base);
  }
  g_player_infos.Remove(peer_id);
  g_peer_teams.Clear(peer_id);
  SyncPlayerInfoShadow(base);
}

// All six record-to-player-info convenience routines call sub_8225CF68 rather
// than indexing the fixed table themselves. Their downstream helper is only
// valid after the peer's player-info lifecycle has constructed its sidecar,
// so a missing object retains the retail null-result semantics.
extern "C" void sub_82708978(PPCContext& ctx, uint8_t* base) {
  const auto peer_id = PeerRecordId(base, ctx.r3.u32);
  if (!peer_id || !mp64::IsValidPeerId(*peer_id) ||
      (mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended &&
       PlayerInfoForId(base, *peer_id).guest_player_info == 0)) {
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_82708978(ctx, base);
}

extern "C" void sub_827089C8(PPCContext& ctx, uint8_t* base) {
  const auto peer_id = PeerRecordId(base, ctx.r3.u32);
  if (!peer_id || !mp64::IsValidPeerId(*peer_id) ||
      (mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended &&
       PlayerInfoForId(base, *peer_id).guest_player_info == 0)) {
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_827089C8(ctx, base);
}

extern "C" void sub_82708A18(PPCContext& ctx, uint8_t* base) {
  const auto peer_id = PeerRecordId(base, ctx.r3.u32);
  if (!peer_id || !mp64::IsValidPeerId(*peer_id) ||
      (mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended &&
       PlayerInfoForId(base, *peer_id).guest_player_info == 0)) {
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_82708A18(ctx, base);
}

extern "C" void sub_82708A48(PPCContext& ctx, uint8_t* base) {
  const auto peer_id = PeerRecordId(base, ctx.r3.u32);
  if (!peer_id || !mp64::IsValidPeerId(*peer_id) ||
      (mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended &&
       PlayerInfoForId(base, *peer_id).guest_player_info == 0)) {
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_82708A48(ctx, base);
}

extern "C" void sub_82708A78(PPCContext& ctx, uint8_t* base) {
  const auto peer_id = PeerRecordId(base, ctx.r3.u32);
  if (!peer_id || !mp64::IsValidPeerId(*peer_id) ||
      (mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended &&
       PlayerInfoForId(base, *peer_id).guest_player_info == 0)) {
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_82708A78(ctx, base);
}

extern "C" void sub_82708B18(PPCContext& ctx, uint8_t* base) {
  const auto peer_id = PeerRecordId(base, ctx.r3.u32);
  if (!peer_id || !mp64::IsValidPeerId(*peer_id) ||
      (mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended &&
       PlayerInfoForId(base, *peer_id).guest_player_info == 0)) {
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_82708B18(ctx, base);
}

// Extended REMOVING_PEER resets its record through the original function, but
// heap-owned records must not be inserted into the manager's embedded free
// list. Preserve the temporary insertion-result ABI without mutating the list.
extern "C" void sub_826FF070(PPCContext& ctx, uint8_t* base) {
  if (g_extended_remove.active && ctx.r6.u32 == g_extended_remove.guest_record) {
    const auto expected_list =
        mp64::CheckedGuestAddress(g_extended_remove.guest_manager, mp64::kFreePeerListOffset);
    if (expected_list && ctx.r4.u32 == *expected_list) {
      const auto result_peer = mp64::CheckedGuestAddress(ctx.r3.u32, 0);
      const auto result_list = mp64::CheckedGuestAddress(ctx.r3.u32, mp64::kGuestPointerSize);
      if (result_peer && result_list) {
        REX_STORE_U32(*result_peer, ctx.r6.u32);
        REX_STORE_U32(*result_list, ctx.r4.u32);
      }
      return;
    }
  }
  __imp__sub_826FF070(ctx, base);
}

// Construct the per-recipient sync endpoint for every canonical peer slot.
// The original covers 0..15; the hooked class factories route 16..63 into the
// endpoint sidecar and preserve the exact endpoint type for each object class.
extern "C" void sub_82702190(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller_return_address = ctx.lr;
  const uint32_t guest_source_object = ctx.r31.u32;
  const uint32_t guest_object = ctx.r3.u32;
  __imp__sub_82702190(ctx, base);
  const auto owner_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectOwnerPeerOffset);
  if (!owner_address) {
    return;
  }
  uint64_t result = ctx.r3.u64;
  const uint8_t owner_id = REX_LOAD_U8(*owner_address);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (peer_id == owner_id) {
      continue;
    }
    PPCContext endpoint_ctx = ctx;
    endpoint_ctx.r3.u64 = guest_object;
    endpoint_ctx.r4.u64 = peer_id;
    endpoint_ctx.r5.u64 = 1;
    CallNetworkObjectVirtual(endpoint_ctx, base, guest_object,
                             mp64::kNetworkObjectCreateEndpointVtableOffset);
    result = endpoint_ctx.r3.u64;
  }
  if (g_clone_peer_expansion.active &&
      caller_return_address == mp64::kCloneEndpointBuildReturnAddress && guest_source_object != 0) {
    for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
         ++peer_id) {
      const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
      if (guest_peer == 0) {
        continue;
      }
      PPCContext predicate_ctx = ctx;
      predicate_ctx.r3.u64 = guest_peer;
      sub_82708620(predicate_ctx, base);
      if (predicate_ctx.r3.u8 == 0) {
        continue;
      }
      predicate_ctx = ctx;
      predicate_ctx.r3.u64 = guest_peer;
      sub_82708770(predicate_ctx, base);
      if (predicate_ctx.r3.u8 != 0) {
        continue;
      }
      PPCContext endpoint_ctx = ctx;
      endpoint_ctx.r3.u64 = guest_object;
      endpoint_ctx.r4.u64 = peer_id;
      sub_82702DE8(endpoint_ctx, base);
      if (endpoint_ctx.r3.u32 == 0) {
        continue;
      }
      endpoint_ctx = ctx;
      endpoint_ctx.r3.u64 = guest_source_object;
      endpoint_ctx.r4.u64 = peer_id;
      sub_82703038(endpoint_ctx, base);
      const uint32_t guest_source_state = endpoint_ctx.r3.u32;
      endpoint_ctx = ctx;
      endpoint_ctx.r3.u64 = guest_object;
      endpoint_ctx.r4.u64 = peer_id;
      sub_82703038(endpoint_ctx, base);
      CopyNetworkEndpointCloneState(base, guest_source_state, endpoint_ctx.r3.u32);
    }
  }
  ctx.r3.u64 = result;
}

// sub_826EFF58 queries the clone type immediately after constructing all
// recipient endpoints. Extend its type-one endpoint flag update to sidecar
// recipients at that exact return address.
extern "C" void sub_821778A0(PPCContext& ctx, uint8_t* base) {
  const uint32_t caller_return_address = ctx.lr;
  const uint32_t guest_object = ctx.r30.u32;
  __imp__sub_821778A0(ctx, base);
  if (!g_clone_peer_expansion.active ||
      caller_return_address != mp64::kCloneTypeQueryReturnAddress || ctx.r3.s32 != 1 ||
      guest_object == 0) {
    return;
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
    if (guest_peer == 0) {
      continue;
    }
    PPCContext predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    sub_82708620(predicate_ctx, base);
    if (predicate_ctx.r3.u8 == 0) {
      continue;
    }
    predicate_ctx = ctx;
    predicate_ctx.r3.u64 = guest_peer;
    sub_82708770(predicate_ctx, base);
    if (predicate_ctx.r3.u8 != 0) {
      continue;
    }
    PPCContext endpoint_ctx = ctx;
    endpoint_ctx.r3.u64 = guest_object;
    endpoint_ctx.r4.u64 = peer_id;
    sub_82702DE8(endpoint_ctx, base);
    if (endpoint_ctx.r3.u32 == 0) {
      continue;
    }
    endpoint_ctx = ctx;
    endpoint_ctx.r3.u64 = guest_object;
    endpoint_ctx.r4.u64 = peer_id;
    endpoint_ctx.r5.u64 = mp64::kCloneEndpointTypeOneFlag;
    endpoint_ctx.r6.u64 = 1;
    sub_82703160(endpoint_ctx, base);
  }
}

extern "C" void sub_82702CA0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  __imp__sub_82702CA0(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (g_network_peer_flags.Get(guest_object, peer_id)[0] != 0) {
      ctx.r3.u64 = 1;
      return;
    }
  }
}

extern "C" void sub_82702CD8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702CD8(ctx, base);
    return;
  }
  ctx.r3.u64 = mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended
                   ? g_network_peer_flags.Get(guest_object, peer_id)[0]
                   : 0;
}

extern "C" void sub_82702CF0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702CF0(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended) {
    g_network_peer_flags.SetFlag(guest_object, peer_id, 0, ctx.r5.u8 != 0);
  }
}

extern "C" void sub_82702D08(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  __imp__sub_82702D08(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (g_network_peer_flags.Get(guest_object, peer_id)[1] != 0) {
      ctx.r3.u64 = 1;
      return;
    }
  }
}

extern "C" void sub_82702D48(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702D48(ctx, base);
    return;
  }
  ctx.r3.u64 = mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended
                   ? g_network_peer_flags.Get(guest_object, peer_id)[1]
                   : 0;
}

extern "C" void sub_82702D60(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702D60(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended) {
    g_network_peer_flags.SetFlag(guest_object, peer_id, 1, ctx.r5.u8 != 0);
  }
}

extern "C" void sub_82702D78(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  __imp__sub_82702D78(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    if (g_network_peer_flags.Get(guest_object, peer_id)[2] != 0) {
      ctx.r3.u64 = 1;
      return;
    }
  }
}

extern "C" void sub_82702DB8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702DB8(ctx, base);
    return;
  }
  ctx.r3.u64 = mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended
                   ? g_network_peer_flags.Get(guest_object, peer_id)[2]
                   : 0;
}

extern "C" void sub_82702DD0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702DD0(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended) {
    g_network_peer_flags.SetFlag(guest_object, peer_id, 2, ctx.r5.u8 != 0);
  }
}

// Capture the complete 64-peer form whenever the title materializes its three
// legacy masks from an object's current per-peer state. The matching setter
// consumes this snapshot, preserving high bits across retail-only call frames.
extern "C" void sub_82703F80(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint32_t guest_first = ctx.r4.u32;
  const uint32_t guest_second = ctx.r5.u32;
  const uint32_t guest_third = ctx.r6.u32;
  __imp__sub_82703F80(ctx, base);
  if (guest_first == 0 || guest_second == 0 || guest_third == 0) {
    g_network_peer_mask_snapshot = {};
    return;
  }
  NetworkPeerMaskSnapshot snapshot;
  snapshot.pending = true;
  snapshot.guest_object = guest_object;
  snapshot.low_first = REX_LOAD_U32(guest_first);
  snapshot.low_second = REX_LOAD_U32(guest_second);
  snapshot.low_third = REX_LOAD_U32(guest_third);
  snapshot.first.ReplaceLegacyLow32(snapshot.low_first);
  snapshot.second.ReplaceLegacyLow32(snapshot.low_second);
  snapshot.third.ReplaceLegacyLow32(snapshot.low_third);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const mp64::NetworkObjectPeerFlags flags = g_network_peer_flags.Get(guest_object, peer_id);
    if (flags[0] != 0) {
      snapshot.first.Set(peer_id);
    } else if (flags[1] != 0) {
      snapshot.second.Set(peer_id);
    } else if (flags[2] != 0) {
      snapshot.third.Set(peer_id);
    }
  }
  g_network_peer_mask_snapshot = snapshot;
}

// The retail clone-state packet reserves bit 16 in its 17-bit recipient word.
// Keep the retail packet byte-for-byte identical while only low peers are
// present. When a high recipient is present, bit 16 marks three following
// 16-bit chunks that carry peers 16..63. Both ends use the same bit-buffer
// primitives, so subsequent fields retain their normal ordering.
extern "C" void sub_82707968(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_stream = ctx.r3.u32;
  const uint32_t low_union =
      (ctx.r4.u32 | ctx.r5.u32 | ctx.r6.u32) & mp64::kNetworkPeerMaskLegacyMask;
  mp64::PeerMask64 complete;
  if (g_network_peer_mask_snapshot.pending) {
    complete.ReplaceAll(g_network_peer_mask_snapshot.first.bits() |
                        g_network_peer_mask_snapshot.second.bits() |
                        g_network_peer_mask_snapshot.third.bits());
  }
  complete.ReplaceLegacyLow16(static_cast<uint16_t>(low_union));

  if (g_network_peer_alias.active) {
    complete.Reset(g_network_peer_alias.actual_peer_id);
  }
  PPCContext local_ctx = ctx;
  local_ctx.r3.u64 = mp64::kGlobalPeerManagerAddress;
  sub_826FDD68(local_ctx, base);
  if (const auto local_peer_id = PeerRecordId(base, local_ctx.r3.u32); local_peer_id) {
    complete.Set(*local_peer_id);
  }
  g_network_peer_mask_snapshot = {};

  const auto guest_bit_buffer = mp64::CheckedGuestAddress(guest_stream, mp64::kGuestPointerSize);
  if (!guest_bit_buffer) {
    ctx.r3.u64 = 0;
    return;
  }
  const mp64::NetworkPeerMaskWireWords wire = mp64::EncodeNetworkPeerMask(complete);
  PPCContext wire_ctx = ctx;
  wire_ctx.r3.u64 = *guest_bit_buffer;
  wire_ctx.r4.u64 = wire.header;
  wire_ctx.r5.u64 = mp64::kNetworkPeerMaskHeaderBits;
  sub_826D9BB0(wire_ctx, base);
  bool success = wire_ctx.r3.u8 != 0;
  if (success && wire.extended) {
    for (const uint16_t chunk : wire.extension) {
      wire_ctx = ctx;
      wire_ctx.r3.u64 = *guest_bit_buffer;
      wire_ctx.r4.u64 = chunk;
      wire_ctx.r5.u64 = mp64::kNetworkPeerMaskExtensionChunkBits;
      sub_826D9BB0(wire_ctx, base);
      success = wire_ctx.r3.u8 != 0;
      if (!success) {
        break;
      }
    }
  }
  ctx.r3.u64 = success ? 1 : 0;
}

extern "C" void sub_82707D78(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_stream = ctx.r3.u32;
  const uint32_t guest_first = ctx.r4.u32;
  const uint32_t guest_second = ctx.r5.u32;
  const uint32_t guest_third = ctx.r6.u32;
  const auto guest_bit_buffer = mp64::CheckedGuestAddress(guest_stream, mp64::kGuestPointerSize);
  if (!guest_bit_buffer || guest_first == 0 || guest_second == 0 || guest_third == 0) {
    if (guest_first != 0) {
      REX_STORE_U32(guest_first, 0);
    }
    if (guest_second != 0) {
      REX_STORE_U32(guest_second, 0);
    }
    if (guest_third != 0) {
      REX_STORE_U32(guest_third, 0);
    }
    ctx.r3.u64 = 0;
    return;
  }

  uint32_t header = 0;
  PPCContext wire_ctx = ctx;
  wire_ctx.r3.u64 = *guest_bit_buffer;
  wire_ctx.r4.u64 = guest_first;
  wire_ctx.r5.u64 = mp64::kNetworkPeerMaskHeaderBits;
  sub_822118C8(wire_ctx, base);
  bool success = wire_ctx.r3.u8 != 0;
  header = REX_LOAD_U32(guest_first);
  std::array<uint16_t, mp64::kNetworkPeerMaskExtensionChunkCount> extension{};
  if (success && (header & mp64::kNetworkPeerMaskExtensionMarker) != 0) {
    for (size_t chunk = 0; chunk < extension.size(); ++chunk) {
      REX_STORE_U32(guest_second, 0);
      wire_ctx = ctx;
      wire_ctx.r3.u64 = *guest_bit_buffer;
      wire_ctx.r4.u64 = guest_second;
      wire_ctx.r5.u64 = mp64::kNetworkPeerMaskExtensionChunkBits;
      sub_822118C8(wire_ctx, base);
      success = wire_ctx.r3.u8 != 0;
      if (!success) {
        break;
      }
      extension[chunk] = static_cast<uint16_t>(REX_LOAD_U32(guest_second));
    }
  }

  const mp64::PeerMask64 complete = mp64::DecodeNetworkPeerMask(header, extension);
  REX_STORE_U32(guest_first, complete.legacy_low16());
  REX_STORE_U32(guest_second, 0);
  REX_STORE_U32(guest_third, 0);
  if (success && ctx.lr == mp64::kNetworkPeerMaskApplyReturnAddress) {
    NetworkPeerMaskSnapshot snapshot;
    snapshot.pending = true;
    snapshot.low_first = complete.legacy_low16();
    snapshot.first = complete;
    g_network_peer_mask_snapshot = snapshot;
  }
  ctx.r3.u64 = success ? 1 : 0;
}

// CNetworkObject embeds three per-peer bytes for only sixteen recipients.
// Refresh the low table with the original and mirror every representable mask
// bit into the extended sidecar. Extended clone packets supply the complete
// high mask through the snapshot set by sub_82707D78; local legacy callers
// continue to drive representable bits through the transition hooks.
extern "C" void sub_82704070(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint32_t first_mask = ctx.r4.u32;
  const uint32_t second_mask = ctx.r5.u32;
  const uint32_t third_mask = ctx.r6.u32;
  const bool use_snapshot = g_network_peer_mask_snapshot.pending &&
                            (g_network_peer_mask_snapshot.guest_object == 0 ||
                             g_network_peer_mask_snapshot.guest_object == guest_object) &&
                            g_network_peer_mask_snapshot.low_first == first_mask &&
                            g_network_peer_mask_snapshot.low_second == second_mask &&
                            g_network_peer_mask_snapshot.low_third == third_mask;
  const NetworkPeerMaskSnapshot snapshot = g_network_peer_mask_snapshot;
  if (use_snapshot || g_network_peer_mask_snapshot.guest_object == guest_object) {
    g_network_peer_mask_snapshot = {};
  }
  __imp__sub_82704070(ctx, base);

  PPCContext local_ctx = ctx;
  local_ctx.r3.u64 = mp64::kGlobalPeerManagerAddress;
  sub_826FDD68(local_ctx, base);
  const uint32_t guest_local_peer = local_ctx.r3.u32;
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    mp64::NetworkObjectPeerFlags flags{};
    const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
    if (guest_peer != 0 && guest_peer != guest_local_peer) {
      PPCContext active_ctx = ctx;
      active_ctx.r3.u64 = guest_peer;
      sub_82708620(active_ctx, base);
      if (active_ctx.r3.u8 != 0) {
        if (use_snapshot) {
          flags[0] = snapshot.first.Contains(peer_id);
          flags[1] = flags[0] == 0 && snapshot.second.Contains(peer_id);
          flags[2] = flags[0] == 0 && flags[1] == 0 && snapshot.third.Contains(peer_id);
        } else if (peer_id < std::numeric_limits<uint32_t>::digits) {
          const uint32_t peer_bit = uint32_t{1} << peer_id;
          flags[0] = (first_mask & peer_bit) != 0;
          flags[1] = flags[0] == 0 && (second_mask & peer_bit) != 0;
          flags[2] = flags[0] == 0 && flags[1] == 0 && (third_mask & peer_bit) != 0;
        }
      }
    }
    g_network_peer_flags.Set(guest_object, peer_id, flags);
  }
}

extern "C" void sub_82703DC0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_result = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703DC0(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0 || mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    if (guest_result != 0) {
      REX_STORE_U32(guest_result, 0);
    }
    ctx.r3.u64 = guest_result;
    return;
  }
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_result;
                            ctx.r4.u64 = guest_object;
                            ctx.r5.u64 = alias_id;
                            __imp__sub_82703DC0(ctx, base);
                          });
}

// The sync serializer indexes both the endpoint table and per-peer flags with
// its recipient argument. Project a high recipient into one safe retail slot;
// the nested mask writer replaces that alias with the canonical peer ID.
extern "C" void sub_827045B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint32_t guest_stream = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  const uint32_t include_state = ctx.r6.u32;
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_827045B8(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0 || mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = 0;
    return;
  }
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_object;
                            ctx.r4.u64 = guest_stream;
                            ctx.r5.u64 = alias_id;
                            ctx.r6.u64 = include_state;
                            __imp__sub_827045B8(ctx, base);
                          });
  validation::PublishMultiplayerValidationStage(
      validation::MultiplayerValidationStage::kFirstExtendedObjectSync, peer_id,
      guest_object);
}

extern "C" void sub_827038B0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  const auto peer_id = PeerRecordId(base, guest_peer);
  if (!peer_id || mp64::IsLegacyPeerId(*peer_id)) {
    __imp__sub_827038B0(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(*peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = mp64::kInvalidPeerId;
    return;
  }
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, *peer_id, false, [&](uint8_t) {
    ctx.r3.u64 = guest_object;
    ctx.r4.u64 = guest_peer;
    __imp__sub_827038B0(ctx, base);
  });
  ctx.r3.u64 = *peer_id;
}

extern "C" void sub_82703BC0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82703BC0);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0 || mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = 0;
    return;
  }
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_object;
                            ctx.r4.u64 = alias_id;
                            RunPrimaryPlayerInfoAlias(ctx, base, __imp__sub_82703BC0);
                          });
}

extern "C" void sub_827041A8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_827041A8(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0 || mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = guest_object;
    return;
  }
  const uint32_t enabled = ctx.r5.u32;
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_object;
                            ctx.r4.u64 = alias_id;
                            ctx.r5.u64 = enabled;
                            __imp__sub_827041A8(ctx, base);
                          });
}

extern "C" void sub_82704390(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  const auto peer_id = PeerRecordId(base, guest_peer);
  if (!peer_id || mp64::IsLegacyPeerId(*peer_id)) {
    __imp__sub_82704390(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(*peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t first_flag = ctx.r5.u32;
  const uint32_t second_flag = ctx.r6.u32;
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, *peer_id, false, [&](uint8_t) {
    ctx.r3.u64 = guest_object;
    ctx.r4.u64 = guest_peer;
    ctx.r5.u64 = first_flag;
    ctx.r6.u64 = second_flag;
    __imp__sub_82704390(ctx, base);
  });
  if (ctx.r3.u8 == 0) {
    return;
  }
  const auto sync_flags_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectSyncFlagsOffset);
  const auto low_flags_table =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectPeerFlagsOffset);
  if (!sync_flags_address || !low_flags_table || (REX_LOAD_U8(*sync_flags_address) & 2) == 0) {
    return;
  }
  for (uint8_t candidate_id = 0; candidate_id < mp64::kExtendedPeerCapacity; ++candidate_id) {
    const uint32_t candidate_peer =
        g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, candidate_id);
    if (candidate_peer == 0) {
      continue;
    }
    PPCContext active_ctx = ctx;
    active_ctx.r3.u64 = candidate_peer;
    sub_82708620(active_ctx, base);
    if (active_ctx.r3.u8 == 0) {
      continue;
    }
    active_ctx = ctx;
    active_ctx.r3.u64 = candidate_peer;
    sub_82708770(active_ctx, base);
    if (active_ctx.r3.u8 != 0) {
      continue;
    }
    bool first_peer_flag = false;
    if (mp64::IsLegacyPeerId(candidate_id)) {
      const auto flags = mp64::CheckedGuestArrayAddress(*low_flags_table, candidate_id,
                                                        mp64::kNetworkObjectPeerFlagsStride);
      first_peer_flag = flags && REX_LOAD_U8(*flags) != 0;
    } else {
      first_peer_flag = g_network_peer_flags.Get(guest_object, candidate_id)[0] != 0;
    }
    if (!first_peer_flag) {
      ctx.r3.u64 = 0;
      return;
    }
  }
}

// Extend CNetworkObject's per-frame endpoint update beyond its hard-coded
// sixteen-recipient loop. Isolating one high peer in a retail slot lets the
// original update endpoint construction, destruction, retransmit mode, and
// timers without replaying any low-peer work.
extern "C" void sub_827047B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  __imp__sub_827047B8(ctx, base);
  const auto owner_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectOwnerPeerOffset);
  if (!owner_address) {
    return;
  }
  const uint8_t owner_id = REX_LOAD_U8(*owner_address);
  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
    if (guest_peer == 0 || peer_id == owner_id ||
        g_network_peer_flags.Get(guest_object, peer_id)[0] == 0) {
      continue;
    }
    PPCContext extended_ctx = ctx;
    WithExtendedNetworkPeer(extended_ctx, base, guest_object, guest_peer, peer_id, true,
                            [&](uint8_t) {
                              extended_ctx.r3.u64 = guest_object;
                              __imp__sub_827047B8(extended_ctx, base);
                            });
  }
  ctx.r3.u64 = 0;
}

extern "C" void sub_82704180(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82704180(ctx, base);
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  const auto mode_address =
      mp64::CheckedGuestAddress(guest_endpoint, mp64::kNetworkEndpointSyncModeOffset);
  if (guest_endpoint == 0 || !mode_address) {
    ctx.r3.u64 = 0;
    return;
  }
  ctx.r3.u64 = guest_endpoint;
  ctx.r4.u64 = REX_LOAD_U8(*mode_address);
  CallNetworkObjectVirtual(ctx, base, guest_endpoint, 16);
}

extern "C" void sub_82703938(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_result = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703938(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0) {
    if (guest_result != 0) {
      REX_STORE_U32(guest_result, 0);
    }
    ctx.r3.u64 = guest_result;
    return;
  }
  const uint32_t guest_changed_mask = ctx.r6.u32;
  const uint32_t guest_requested_mask = ctx.r7.u32;
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_result;
                            ctx.r4.u64 = guest_object;
                            ctx.r5.u64 = alias_id;
                            ctx.r6.u64 = guest_changed_mask;
                            ctx.r7.u64 = guest_requested_mask;
                            __imp__sub_82703938(ctx, base);
                          });
}

extern "C" void sub_82703200(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_result = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703200(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0) {
    if (guest_result != 0) {
      REX_STORE_U32(guest_result, 0);
    }
    ctx.r3.u64 = guest_result;
    return;
  }
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_result;
                            ctx.r4.u64 = guest_object;
                            ctx.r5.u64 = alias_id;
                            __imp__sub_82703200(ctx, base);
                          });
}

extern "C" void sub_82703050(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_result = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703050(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0) {
    if (guest_result != 0) {
      REX_STORE_U32(guest_result, 0);
    }
    ctx.r3.u64 = guest_result;
    return;
  }
  const uint32_t guest_component_mask = ctx.r6.u32;
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_result;
                            ctx.r4.u64 = guest_object;
                            ctx.r5.u64 = alias_id;
                            ctx.r6.u64 = guest_component_mask;
                            __imp__sub_82703050(ctx, base);
                          });
}

extern "C" void sub_82703350(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703350(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t sequence = ctx.r5.u32;
  const uint32_t component_mask = ctx.r6.u32;
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_object;
                            ctx.r4.u64 = alias_id;
                            ctx.r5.u64 = sequence;
                            ctx.r6.u64 = component_mask;
                            __imp__sub_82703350(ctx, base);
                          });
}

extern "C" void sub_82704288(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_result = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r5.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82704288(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0) {
    if (guest_result != 0) {
      REX_STORE_U32(guest_result, 0);
    }
    ctx.r3.u64 = guest_result;
    return;
  }
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_result;
                            ctx.r4.u64 = guest_object;
                            ctx.r5.u64 = alias_id;
                            __imp__sub_82704288(ctx, base);
                          });
}

extern "C" void sub_82703D08(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703D08(ctx, base);
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t third_argument = ctx.r5.u32;
  WithExtendedNetworkPeer(ctx, base, guest_object, guest_peer, peer_id, false,
                          [&](uint8_t alias_id) {
                            ctx.r3.u64 = guest_object;
                            ctx.r4.u64 = alias_id;
                            ctx.r5.u64 = third_argument;
                            __imp__sub_82703D08(ctx, base);
                          });
}

extern "C" void sub_82702C58(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  std::vector<uint32_t> extended_endpoints;
  g_network_endpoints.VisitExtended(guest_object, [&](uint8_t, uint32_t guest_endpoint) {
    extended_endpoints.push_back(guest_endpoint);
    return true;
  });
  for (const uint32_t guest_endpoint : extended_endpoints) {
    DestroyNetworkEndpoint(ctx, base, guest_endpoint);
  }
  __imp__sub_82702C58(ctx, base);
  g_network_endpoints.RemoveObject(guest_object);
  g_network_peer_flags.RemoveObject(guest_object);
  g_ped_network_peer_states.RemoveObject(guest_object);
}

// CNetworkObject owns only sixteen inline per-recipient endpoint pointers at
// object+92. IDs 16..63 use a sidecar; these canonical endpoint accessors and
// forwarding helpers must never evaluate object + (peer+23)*4 for those IDs.
extern "C" void sub_82702DE8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id =
      CanonicalNetworkEndpointPeerId(guest_object, GuestPeerId(ctx.r4.u64));
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = g_network_endpoints.Get(guest_object, peer_id);
    return;
  }
  if (!mp64::IsLegacyPeerId(peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_82702DE8(ctx, base);
}

extern "C" void sub_82702E00(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id =
      CanonicalNetworkEndpointPeerId(guest_object, GuestPeerId(ctx.r4.u64));
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kLegacy) {
    __imp__sub_82702E00(ctx, base);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kInvalid) {
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Remove(guest_object, peer_id);
  if (!PublishActiveNetworkAliasEndpoint(base, guest_object, peer_id, 0)) {
    REXLOG_WARN(
        "gta4-multiplayer64: unable to clear projected endpoint object={:08X} peer={}",
        guest_object, peer_id);
  }
  if (guest_endpoint == 0) {
    return;
  }
  DestroyNetworkEndpoint(ctx, base, guest_endpoint);
}

extern "C" void sub_82702E98(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  __imp__sub_82702E98(ctx, base);
  std::vector<uint8_t> extended_ids;
  g_network_endpoints.VisitExtended(guest_object, [&](uint8_t peer_id, uint32_t) {
    extended_ids.push_back(peer_id);
    return true;
  });
  for (const uint8_t peer_id : extended_ids) {
    PPCContext destroy_ctx = ctx;
    destroy_ctx.r3.u64 = guest_object;
    destroy_ctx.r4.u64 = peer_id;
    sub_82702E00(destroy_ctx, base);
  }
  g_network_endpoints.RemoveObject(guest_object);
  g_network_peer_flags.RemoveObject(guest_object);
  g_ped_network_peer_states.RemoveObject(guest_object);
}

extern "C" void sub_82702EF0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702EF0(ctx, base);
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  if (guest_endpoint == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t argument = ctx.r5.u32;
  ctx.r3.u64 = guest_endpoint;
  ctx.r4.u64 = argument;
  CallNetworkObjectVirtual(ctx, base, guest_endpoint, 20);
}

extern "C" void sub_82702F18(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702F18(ctx, base);
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  if (guest_endpoint == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t first_argument = ctx.r5.u32;
  const uint32_t second_argument = ctx.r6.u32;
  ctx.r3.u64 = guest_endpoint;
  ctx.r4.u64 = first_argument;
  ctx.r5.u64 = second_argument;
  CallNetworkObjectVirtual(ctx, base, guest_endpoint, 24);
}

extern "C" void sub_82702F48(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702F48(ctx, base);
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  if (guest_endpoint == 0 || ctx.r5.u32 == 0) {
    return;
  }
  const auto endpoint_mask = mp64::CheckedGuestAddress(guest_endpoint, 4);
  if (endpoint_mask) {
    REX_STORE_U32(*endpoint_mask, REX_LOAD_U32(*endpoint_mask) | REX_LOAD_U32(ctx.r5.u32));
  }
}

extern "C" void sub_82702F70(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82702F70(ctx, base);
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  const auto endpoint_mask = mp64::CheckedGuestAddress(guest_endpoint, 4);
  if (endpoint_mask) {
    REX_STORE_U32(*endpoint_mask, REX_LOAD_U32(*endpoint_mask) & ~ctx.r5.u32);
  }
}

extern "C" void sub_82702F90(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  __imp__sub_82702F90(ctx, base);
  g_network_endpoints.VisitExtended(guest_object, [&](uint8_t, uint32_t endpoint) {
    PPCContext endpoint_ctx = ctx;
    endpoint_ctx.r3.u64 = endpoint;
    CallNetworkObjectVirtual(endpoint_ctx, base, endpoint, 12);
    return true;
  });
}

extern "C" void sub_82703008(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703008(ctx, base);
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  if (guest_endpoint == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  ctx.r3.u64 = guest_endpoint;
  CallNetworkObjectVirtual(ctx, base, guest_endpoint, 36);
}

extern "C" void sub_82703038(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703038(ctx, base);
    return;
  }
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  ctx.r3.u64 = mp64::CheckedGuestAddress(guest_endpoint, 8).value_or(0);
}

void DispatchExtendedEndpointComponents(PPCContext& ctx, uint8_t* base, uint32_t guest_object,
                                        uint8_t peer_id, uint32_t component_mask, uint32_t value) {
  const uint32_t guest_endpoint = g_network_endpoints.Get(guest_object, peer_id);
  if (guest_endpoint == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  ctx.r3.u64 = guest_object;
  if (!CallNetworkObjectVirtual(ctx, base, guest_object,
                                mp64::kNetworkObjectComponentCountVtableOffset)) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t component_count = ctx.r3.u32;
  for (uint32_t component = 0; component < component_count; ++component) {
    if (component >= std::numeric_limits<uint32_t>::digits) {
      continue;
    }
    if ((component_mask & (uint32_t{1} << component)) == 0) {
      continue;
    }
    ctx.r3.u64 = guest_endpoint;
    ctx.r4.u64 = component;
    ctx.r5.u64 = value;
    CallNetworkObjectVirtual(ctx, base, guest_endpoint, 32);
  }
}

extern "C" void sub_82703160(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82703160(ctx, base);
    return;
  }
  if (!mp64::IsValidPeerId(peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  DispatchExtendedEndpointComponents(ctx, base, guest_object, peer_id, ctx.r5.u32, ctx.r6.u32);
}

extern "C" void sub_827032B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_827032B8(ctx, base);
    return;
  }
  if (!mp64::IsValidPeerId(peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  DispatchExtendedEndpointComponents(ctx, base, guest_object, peer_id, ctx.r5.u32, 0);
}

// The reassignment entry builder asks the object for its current owner and
// immediately uses that byte in the fixed owner-list table. Return the active
// physical slot only inside the serialized extended-owner window.
extern "C" void sub_82701F58(PPCContext& ctx, uint8_t* base) {
  __imp__sub_82701F58(ctx, base);
  if (g_object_owner_list_alias.active && ctx.r3.u8 == g_object_owner_list_alias.actual_owner_id) {
    ctx.r3.u64 = g_object_owner_list_alias.alias_owner_id;
    return;
  }
  if (mp64::IsLegacyPeerId(g_reassignment_call.object_list_alias) &&
      ctx.r3.u8 == g_reassignment_call.actual_object_list_owner) {
    ctx.r3.u64 = g_reassignment_call.object_list_alias;
    return;
  }
  if (g_reassignment_call.active && ctx.r3.u8 == g_reassignment_call.actual_owner) {
    ctx.r3.u64 = g_reassignment_call.owner_alias;
  }
}

extern "C" void sub_826FDDC0(PPCContext& ctx, uint8_t* base) {
  __imp__sub_826FDDC0(ctx, base);
  if (g_object_owner_list_alias.active && ctx.r3.u8 == g_object_owner_list_alias.actual_owner_id) {
    ctx.r3.u64 = g_object_owner_list_alias.alias_owner_id;
  }
}

extern "C" void sub_82701F88(PPCContext& ctx, uint8_t* base) {
  __imp__sub_82701F88(ctx, base);
  const uint8_t actual_id = ctx.r3.u8;
  if (g_proximity_status_capture.active &&
      ctx.lr == mp64::kProximityStatusEligibleIdReturnAddress &&
      mp64::IsValidPeerId(actual_id)) {
    g_proximity_status_capture.eligible[actual_id] = true;
  }
  if (g_player_info_batch_projection.active) {
    for (uint8_t alias_id = 0; alias_id < mp64::kLegacyPeerCapacity; ++alias_id) {
      if (ctx.r3.u8 == g_player_info_batch_projection.actual_ids[alias_id]) {
        ctx.r3.u64 = alias_id;
        return;
      }
    }
  }
  if (g_object_owner_list_alias.active && ctx.r3.u8 == g_object_owner_list_alias.actual_owner_id) {
    ctx.r3.u64 = g_object_owner_list_alias.alias_owner_id;
    return;
  }
  if (g_object_peer_alias.active && ctx.r3.u8 == g_object_peer_alias.actual_peer_id) {
    ctx.r3.u64 = g_object_peer_alias.alias_peer_id;
    return;
  }
  if (g_event_peer_alias.active && ctx.r3.u8 == g_event_peer_alias.actual_peer_id) {
    ctx.r3.u64 = g_event_peer_alias.alias_peer_id;
    return;
  }
  if (g_network_peer_alias.active && ctx.r3.u8 == g_network_peer_alias.actual_peer_id) {
    ctx.r3.u64 = g_network_peer_alias.alias_peer_id;
    return;
  }
  if (g_dispatch_alias.active && ctx.r3.u8 == g_dispatch_alias.actual_peer_id) {
    ctx.r3.u64 = g_dispatch_alias.alias_peer_id;
    return;
  }
  if (g_reassignment_call.active && ctx.r3.u8 == g_reassignment_call.actual_owner) {
    ctx.r3.u64 = g_reassignment_call.owner_alias;
  }
}

// Per-class vtable slot 17 (byte offset 68) endpoint factories. Their guest
// endpoint types have no embedded peer ID. Extended recipients use the exact
// retail constructor over system-heap storage and keep ownership in the
// sidecar; an active projected alias is only a temporary view of that pointer.
extern "C" void sub_82729AC0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id =
      CanonicalNetworkEndpointPeerId(guest_object, GuestPeerId(ctx.r4.u64));
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82729AC0(ctx, base);
    return;
  }
  if (!mp64::IsValidPeerId(peer_id) || g_network_endpoints.Get(guest_object, peer_id) != 0) {
    ctx.r3.u64 = guest_object;
    return;
  }
  const uint32_t create_argument = ctx.r5.u32;
  const uint32_t endpoint =
      AllocateNetworkEndpoint(ctx, base, mp64::kVehicleSyncPoolPointerAddress,
                              __imp__sub_82726AA8);
  FinishExtendedEndpointCreation(ctx, base, guest_object, peer_id, endpoint, create_argument);
}

extern "C" void sub_827788E0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id =
      CanonicalNetworkEndpointPeerId(guest_object, GuestPeerId(ctx.r4.u64));
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_827788E0(ctx, base);
    return;
  }
  if (!mp64::IsValidPeerId(peer_id) || g_network_endpoints.Get(guest_object, peer_id) != 0) {
    ctx.r3.u64 = guest_object;
    return;
  }
  const uint32_t create_argument = ctx.r5.u32;
  const uint32_t endpoint =
      AllocateNetworkEndpoint(ctx, base, mp64::kPlayerSyncPoolPointerAddress,
                              __imp__sub_82778050);
  FinishExtendedEndpointCreation(ctx, base, guest_object, peer_id, endpoint, create_argument);
}

extern "C" void sub_82789678(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id =
      CanonicalNetworkEndpointPeerId(guest_object, GuestPeerId(ctx.r4.u64));
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82789678(ctx, base);
    return;
  }
  if (!mp64::IsValidPeerId(peer_id) || g_network_endpoints.Get(guest_object, peer_id) != 0) {
    ctx.r3.u64 = guest_object;
    return;
  }
  const uint32_t create_argument = ctx.r5.u32;
  const uint32_t endpoint =
      AllocateNetworkEndpoint(ctx, base, mp64::kDummyPedSyncPoolPointerAddress,
                              __imp__sub_82789010);
  FinishExtendedEndpointCreation(ctx, base, guest_object, peer_id, endpoint, create_argument);
}

extern "C" void sub_82711A28(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id =
      CanonicalNetworkEndpointPeerId(guest_object, GuestPeerId(ctx.r4.u64));
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82711A28(ctx, base);
    return;
  }
  if (!mp64::IsValidPeerId(peer_id) || g_network_endpoints.Get(guest_object, peer_id) != 0) {
    ctx.r3.u64 = guest_object;
    return;
  }
  const uint32_t create_argument = ctx.r5.u32;
  if (!WithNetworkEndpointVirtualPeer(
          ctx, base, guest_object, peer_id, [&](uint8_t presented_peer_id) {
            ctx.r3.u64 = guest_object;
            ctx.r4.u64 = presented_peer_id;
            CallNetworkObjectVirtual(ctx, base, guest_object,
                                     mp64::kNetworkObjectPrepareSyncVtableOffset);
          })) {
    ctx.r3.u64 = guest_object;
    return;
  }
  const uint32_t endpoint =
      AllocateNetworkEndpoint(ctx, base, mp64::kPedSyncPoolPointerAddress,
                              __imp__sub_8270C9A8);
  FinishExtendedEndpointCreation(ctx, base, guest_object, peer_id, endpoint, create_argument);
}

extern "C" void sub_8272E098(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t peer_id =
      CanonicalNetworkEndpointPeerId(guest_object, GuestPeerId(ctx.r4.u64));
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_8272E098(ctx, base);
    return;
  }
  if (!mp64::IsValidPeerId(peer_id) || g_network_endpoints.Get(guest_object, peer_id) != 0) {
    ctx.r3.u64 = guest_object;
    return;
  }
  const uint32_t create_argument = ctx.r5.u32;
  const uint32_t endpoint =
      AllocateNetworkEndpoint(ctx, base, mp64::kObjectSyncPoolPointerAddress,
                              __imp__sub_8272C3A0);
  FinishExtendedEndpointCreation(ctx, base, guest_object, peer_id, endpoint, create_argument);
}

// CNetworkObject::CHANGE_OWNER stores owner IDs as bytes and resolves the old
// and new records through the hooked peer manager. After the unchanged retail
// low-ID transition, mirror endpoint creation and ownership-command fan-out to
// every extended recipient held by the endpoint sidecar.
extern "C" void sub_82703590(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_object = ctx.r3.u32;
  const uint8_t new_owner = GuestPeerId(ctx.r4.u64);
  const uint8_t suppress_notifications = GuestPeerId(ctx.r5.u64);
  if (guest_object == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const auto old_owner_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectOwnerPeerOffset);
  if (!old_owner_address) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint8_t old_owner = REX_LOAD_U8(*old_owner_address);
  const auto old_authoritative_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectAuthoritativeOffset);
  const auto endpoint_state_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectEndpointStateOffset);
  const auto ownership_token_address =
      mp64::CheckedGuestAddress(guest_object, mp64::kNetworkObjectOwnershipTokenOffset);
  const uint8_t old_authoritative =
      old_authoritative_address ? REX_LOAD_U8(*old_authoritative_address) : 0;
  const uint32_t old_ownership_token =
      ownership_token_address ? REX_LOAD_U32(*ownership_token_address) : 0;
  const bool old_owner_supported =
      old_owner == mp64::kInvalidPeerId || mp64::IsValidPeerId(old_owner);
  if (!mp64::IsValidPeerId(new_owner) || !old_owner_supported) {
    WarnInvalidPeerOnce(g_ownership_warning, "sub_82703590", new_owner,
                        "owner ID is outside 0..63");
    ctx.r3.u64 = 0;
    return;
  }
  __imp__sub_82703590(ctx, base);
  const uint64_t original_result = ctx.r3.u64;

  std::vector<uint8_t> extended_recipients;
  g_peer_managers.VisitExtendedPeers(mp64::kGlobalPeerManagerAddress,
                                     [&](uint8_t peer_id, uint32_t) {
                                       extended_recipients.push_back(peer_id);
                                       return true;
                                     });
  if (extended_recipients.empty() || old_authoritative == 0 || suppress_notifications != 0 ||
      !old_authoritative_address || REX_LOAD_U8(*old_authoritative_address) != 0 ||
      !endpoint_state_address || REX_LOAD_U32(*endpoint_state_address) == 0 ||
      !ownership_token_address) {
    ctx.r3.u64 = original_result;
    return;
  }
  PPCContext eligibility_ctx = ctx;
  eligibility_ctx.r3.u64 = guest_object;
  if (!CallNetworkObjectVirtual(eligibility_ctx, base, guest_object,
                                mp64::kNetworkObjectOwnershipEligibleVtableOffset) ||
      eligibility_ctx.r3.u8 == 0) {
    ctx.r3.u64 = original_result;
    return;
  }
  PPCContext count_ctx = ctx;
  count_ctx.r3.u64 = guest_object;
  if (!CallNetworkObjectVirtual(count_ctx, base, guest_object,
                                mp64::kNetworkObjectComponentCountVtableOffset)) {
    ctx.r3.u64 = original_result;
    return;
  }
  const uint32_t component_mask = mp64::LowBits32(count_ctx.r3.u32);
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    ctx.r3.u64 = original_result;
    return;
  }
  const uint32_t guest_command_mask = runtime->memory()->SystemHeapAlloc(mp64::kGuestPointerSize);
  if (guest_command_mask == 0) {
    ctx.r3.u64 = original_result;
    return;
  }
  const uint32_t current_ownership_token = REX_LOAD_U32(*ownership_token_address);
  REX_STORE_U32(*ownership_token_address, old_ownership_token);
  for (const uint8_t peer_id : extended_recipients) {
    if (peer_id == new_owner || peer_id == old_owner ||
        g_network_endpoints.Get(guest_object, peer_id) == 0) {
      continue;
    }
    PPCContext clear_ctx = ctx;
    clear_ctx.r3.u64 = guest_object;
    clear_ctx.r4.u64 = peer_id;
    clear_ctx.r5.u64 = component_mask;
    sub_827032B8(clear_ctx, base);

    REX_STORE_U32(guest_command_mask, 0);
    PPCContext fill_ctx = ctx;
    fill_ctx.r3.u64 = guest_object;
    fill_ctx.r4.u64 = guest_command_mask;
    if (!CallNetworkObjectVirtual(fill_ctx, base, guest_object,
                                  mp64::kNetworkObjectFillOwnershipCommandVtableOffset)) {
      continue;
    }
    PPCContext send_ctx = ctx;
    send_ctx.r3.u64 = guest_object;
    send_ctx.r4.u64 = peer_id;
    send_ctx.r5.u64 = REX_LOAD_U32(guest_command_mask);
    send_ctx.r6.u64 = 1;
    sub_82703160(send_ctx, base);
  }
  REX_STORE_U32(*ownership_token_address, current_ownership_token);
  runtime->memory()->SystemHeapFree(guest_command_mask);
  ctx.r3.u64 = original_result;
}

// Packet constructors store the owner ID as a byte. During an extended-owner
// window the physical retail slot must never leak onto the wire.
extern "C" void sub_82795588(PPCContext& ctx, uint8_t* base) {
  if (g_reassignment_call.active && ctx.r4.u8 == g_reassignment_call.owner_alias) {
    ctx.r4.u64 = g_reassignment_call.actual_owner;
  }
  __imp__sub_82795588(ctx, base);
}

extern "C" void sub_827955D0(PPCContext& ctx, uint8_t* base) {
  if (g_reassignment_call.active && ctx.r4.u8 == g_reassignment_call.owner_alias) {
    ctx.r4.u64 = g_reassignment_call.actual_owner;
  }
  __imp__sub_827955D0(ctx, base);
}

extern "C" void sub_82795608(PPCContext& ctx, uint8_t* base) {
  if (g_reassignment_call.active && ctx.r4.u8 == g_reassignment_call.owner_alias) {
    ctx.r4.u64 = g_reassignment_call.actual_owner;
  }
  __imp__sub_82795608(ctx, base);
}

extern "C" void sub_82783F98(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_message = ctx.r3.u32;
  __imp__sub_82783F98(ctx, base);
  if (g_reassignment_call.active && ctx.r3.u8 != 0 &&
      REX_LOAD_U8(guest_message) == g_reassignment_call.actual_owner) {
    REX_STORE_U8(guest_message, g_reassignment_call.owner_alias);
  }
}

extern "C" void sub_827842D8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_message = ctx.r3.u32;
  __imp__sub_827842D8(ctx, base);
  if (g_reassignment_call.active && ctx.r3.u8 != 0 &&
      REX_LOAD_U8(guest_message) == g_reassignment_call.actual_owner) {
    REX_STORE_U8(guest_message, g_reassignment_call.owner_alias);
  }
}

// The retail sender derives a pointer into its 16x16 transport matrix before
// entering these helpers. Replace that pointer with the constructed sidecar
// state for projected recipients.
extern "C" void sub_82785940(PPCContext& ctx, uint8_t* base) {
  if (g_reassignment_call.active && g_reassignment_call.guest_transport_state != 0) {
    ctx.r7.u64 = g_reassignment_call.guest_transport_state;
  }
  __imp__sub_82785940(ctx, base);
}

extern "C" void sub_827859C8(PPCContext& ctx, uint8_t* base) {
  if (g_reassignment_call.active && g_reassignment_call.guest_transport_state != 0) {
    ctx.r7.u64 = g_reassignment_call.guest_transport_state;
  }
  __imp__sub_827859C8(ctx, base);
}

extern "C" void sub_82783960(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const auto stale_states = g_reassignments.RemoveManager(guest_manager);
  if (rex::Runtime* runtime = rex::Runtime::instance()) {
    for (const auto& state : stale_states) {
      for (const uint32_t guest_transport : state.guest_transport_states) {
        if (guest_transport != 0) {
          PPCContext destroy_ctx = ctx;
          destroy_ctx.r3.u64 = guest_transport;
          __imp__sub_829F0920(destroy_ctx, base);
          runtime->memory()->SystemHeapFree(guest_transport);
        }
      }
    }
  }
  ctx.r3.u64 = guest_manager;
  __imp__sub_82783960(ctx, base);
}

// The retail activation constructor initializes its embedded 16-owner command
// records and 16x16 transport matrix. Those remain the serialized low-ID
// window; the complete 64-owner command records and transports are held by the
// sidecar. Capture the newly initialized low masks at this exact lifecycle seam
// so later mixed low/high fan-out starts from one canonical registry state.
extern "C" void sub_827869B0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  __imp__sub_827869B0(ctx, base);
  for (uint8_t owner_id = 0; owner_id < mp64::kLegacyPeerCapacity; ++owner_id) {
    LoadReassignmentState(base, guest_manager, owner_id);
  }
}

extern "C" void sub_827845E0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  DestroyReassignmentSidecars(ctx, base, guest_manager);
  ctx.r3.u64 = guest_manager;
  __imp__sub_827845E0(ctx, base);
}

extern "C" void sub_82783140(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  __imp__sub_82783140(ctx, base);
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      if (!record || REX_LOAD_U32(*record + 12) == 0) {
        return;
      }
      uint32_t node = REX_LOAD_U32(*record + 4);
      while (node != 0) {
        const uint32_t next = REX_LOAD_U32(node + 8);
        const uint32_t entry = REX_LOAD_U32(node + 4);
        const uint32_t guest_object = entry != 0 ? REX_LOAD_U32(entry) : 0;
        if (guest_object != 0) {
          PPCContext notify_ctx = ctx;
          notify_ctx.r3.u64 = guest_object;
          notify_ctx.r4.u64 = guest_peer;
          CallNetworkObjectVirtual(notify_ctx, base, guest_object, 128);
        }
        node = next;
      }
    });
  }
}

extern "C" void sub_82783208(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  __imp__sub_82783208(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    if (!g_reassignments.Get(guest_manager, owner_id).initialized) {
      continue;
    }
    bool active = false;
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      active = record && REX_LOAD_U32(*record + 12) != 0;
    });
    if (active) {
      ctx.r3.u64 = 1;
      return;
    }
  }
  ctx.r3.u64 = 0;
}

extern "C" void sub_82783248(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const int32_t object_key = ctx.r4.s32;
  __imp__sub_82783248(ctx, base);
  uint32_t count = ctx.r3.u32;
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    if (!g_reassignments.Get(guest_manager, owner_id).initialized) {
      continue;
    }
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      if (!record || REX_LOAD_U32(*record + 12) == 0) {
        return;
      }
      uint32_t node = REX_LOAD_U32(*record + 4);
      while (node != 0) {
        const uint32_t next = REX_LOAD_U32(node + 8);
        const uint32_t entry = REX_LOAD_U32(node + 4);
        const uint32_t guest_object = entry != 0 ? REX_LOAD_U32(entry) : 0;
        if (guest_object != 0) {
          PPCContext key_ctx = ctx;
          key_ctx.r3.u64 = guest_object;
          __imp__sub_821778A0(key_ctx, base);
          if (key_ctx.r3.s32 == object_key && (REX_LOAD_U8(entry + 15) & 0x40) == 0) {
            ++count;
          }
        }
        node = next;
      }
    });
  }
  ctx.r3.u64 = count;
}

extern "C" void sub_827832E8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  __imp__sub_827832E8(ctx, base);
  PPCContext id_ctx = ctx;
  id_ctx.r3.u64 = guest_object;
  __imp__sub_82701F50(id_ctx, base);
  const uint32_t object_id = id_ctx.r3.u32;
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    if (!g_reassignments.Get(guest_manager, owner_id).initialized) {
      continue;
    }
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      if (!record || REX_LOAD_U32(*record + 12) == 0) {
        return;
      }
      PPCContext find_ctx = ctx;
      find_ctx.r3.u64 = *record + 4;
      find_ctx.r4.u64 = object_id;
      __imp__sub_82782F58(find_ctx, base);
      if (find_ctx.r3.u32 != 0) {
        REX_STORE_U8(find_ctx.r3.u32 + 15, REX_LOAD_U8(find_ctx.r3.u32 + 15) | 0x10);
      }
    });
  }
}

extern "C" void sub_82782FC8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t object_id = ctx.r4.u32;
  __imp__sub_82782FC8(ctx, base);
  if (ctx.r3.u32 != 0) {
    return;
  }
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    if (!g_reassignments.Get(guest_manager, owner_id).initialized) {
      continue;
    }
    uint32_t match = 0;
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      if (!record || REX_LOAD_U32(*record + 12) == 0) {
        return;
      }
      PPCContext find_ctx = ctx;
      find_ctx.r3.u64 = *record + 4;
      find_ctx.r4.u64 = object_id;
      __imp__sub_82782F58(find_ctx, base);
      if (find_ctx.r3.u32 != 0 && (REX_LOAD_U8(find_ctx.r3.u32 + 15) & 0x40) == 0) {
        match = find_ctx.r3.u32;
      }
    });
    if (match != 0) {
      ctx.r3.u64 = match;
      return;
    }
  }
  ctx.r3.u64 = 0;
}

extern "C" void sub_827830C0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  __imp__sub_827830C0(ctx, base);
  if (ctx.r3.u8 != 0) {
    return;
  }
  bool vacant = false;
  for (const auto [owner_id, guest_peer] : CollectExtendedPeers()) {
    (void)guest_peer;
    const mp64::ReassignmentOwnerState state = g_reassignments.Get(guest_manager, owner_id);
    if (!state.initialized) {
      vacant = true;
      break;
    }
    bool active = false;
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      active = record && REX_LOAD_U32(*record + 12) != 0;
    });
    vacant = !active;
    if (vacant) {
      break;
    }
  }
  ctx.r3.u64 = vacant;
}

extern "C" void sub_827847F0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_object = ctx.r4.u32;
  const uint8_t owner_id = GuestPeerId(ctx.r5.u64);
  if (!mp64::IsValidPeerId(owner_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint8_t logical_owner =
      g_reassignment_call.active && owner_id == g_reassignment_call.owner_alias
          ? g_reassignment_call.actual_owner
          : owner_id;
  PPCContext current_owner_ctx = ctx;
  current_owner_ctx.r3.u64 = guest_object;
  __imp__sub_82701F58(current_owner_ctx, base);
  const uint8_t current_owner = current_owner_ctx.r3.u8;
  const auto invoke_original = [&](uint8_t physical_owner) {
    ctx.r5.u64 = physical_owner;
    if (mp64::ClassifyPeerId(current_owner) != mp64::PeerIdClass::kExtended ||
        current_owner == logical_owner) {
      __imp__sub_827847F0(ctx, base);
      return;
    }
    WithExtendedReassignmentObjectList(base, guest_manager, current_owner, physical_owner,
                                       [&](uint8_t) { __imp__sub_827847F0(ctx, base); });
  };
  if (WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id,
                                    [&](uint8_t alias_id) { invoke_original(alias_id); })) {
    return;
  }
  invoke_original(owner_id);
}

extern "C" void sub_82784960(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_entry = ctx.r4.u32;
  const uint8_t owner_id = GuestPeerId(ctx.r5.u64);
  if (!mp64::IsValidPeerId(owner_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint8_t logical_owner =
      g_reassignment_call.active && owner_id == g_reassignment_call.owner_alias
          ? g_reassignment_call.actual_owner
          : owner_id;
  NotifyExtendedReassignmentEntry(ctx, base, guest_manager, logical_owner, guest_entry);
  const uint8_t new_owner = guest_entry != 0 ? REX_LOAD_U8(guest_entry + 14) : mp64::kInvalidPeerId;
  const auto invoke_original = [&](uint8_t physical_owner) {
    ctx.r5.u64 = physical_owner;
    if (mp64::ClassifyPeerId(new_owner) != mp64::PeerIdClass::kExtended) {
      __imp__sub_82784960(ctx, base);
      return;
    }
    if (new_owner == logical_owner &&
        mp64::ClassifyPeerId(logical_owner) == mp64::PeerIdClass::kExtended) {
      REX_STORE_U8(guest_entry + 14, physical_owner);
      __imp__sub_82784960(ctx, base);
      return;
    }
    WithExtendedReassignmentObjectList(base, guest_manager, new_owner, physical_owner,
                                       [&](uint8_t object_list_alias) {
                                         REX_STORE_U8(guest_entry + 14, object_list_alias);
                                         __imp__sub_82784960(ctx, base);
                                       });
  };
  if (WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id,
                                    [&](uint8_t alias_id) { invoke_original(alias_id); })) {
    return;
  }
  invoke_original(owner_id);
}

extern "C" void sub_82785008(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_message = ctx.r4.u32;
  if (guest_message == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint8_t owner_id = REX_LOAD_U8(guest_message);
  if (!mp64::IsValidPeerId(owner_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  if (WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
        REX_STORE_U8(guest_message, alias_id);
        __imp__sub_82785008(ctx, base);
        REX_STORE_U8(guest_message, owner_id);
      })) {
    return;
  }
  __imp__sub_82785008(ctx, base);
}

extern "C" void sub_82783380(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t guest_message = ctx.r4.u32;
  if (guest_message == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint8_t owner_id = REX_LOAD_U8(guest_message);
  if (!mp64::IsValidPeerId(owner_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  if (WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
        REX_STORE_U8(guest_message, alias_id);
        __imp__sub_82783380(ctx, base);
        REX_STORE_U8(guest_message, owner_id);
      })) {
    return;
  }
  __imp__sub_82783380(ctx, base);
}

extern "C" void sub_82786088(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const auto owner_id = ParseReassignmentConfirmationOwner(ctx, base, guest_manager, ctx.r6.u32);
  if (!owner_id || !mp64::IsValidPeerId(*owner_id)) {
    __imp__sub_82786088(ctx, base);
    return;
  }
  if (WithExtendedReassignmentOwner(ctx, base, guest_manager, *owner_id,
                                    [&](uint8_t) { __imp__sub_82786088(ctx, base); })) {
    return;
  }
  __imp__sub_82786088(ctx, base);
}

extern "C" void sub_82786628(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const auto owner_id = ParseReassignmentStatusOwner(ctx, base, ctx.r6.u32);
  if (!owner_id || !mp64::IsValidPeerId(*owner_id)) {
    __imp__sub_82786628(ctx, base);
    return;
  }
  if (WithExtendedReassignmentOwner(ctx, base, guest_manager, *owner_id,
                                    [&](uint8_t) { __imp__sub_82786628(ctx, base); })) {
    return;
  }
  __imp__sub_82786628(ctx, base);
}

extern "C" void sub_82785AE0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t requested_owner = GuestPeerId(ctx.r4.u64);
  const int8_t requested_recipient = ctx.r5.s8;
  if (!mp64::IsValidPeerId(requested_owner) ||
      (requested_recipient != -1 &&
       !mp64::IsValidPeerId(static_cast<uint8_t>(requested_recipient)))) {
    ctx.r3.u64 = 0;
    return;
  }

  if (g_reassignment_call.active && requested_owner == g_reassignment_call.owner_alias) {
    const uint8_t actual_owner = g_reassignment_call.actual_owner;
    if (ctx.lr == kReassignmentNegotiationReturnAddress) {
      PopulateExtendedNegotiationMask(ctx, base, guest_manager, actual_owner);
    }
    RunReassignmentSender(ctx, base, __imp__sub_82785AE0, guest_manager, actual_owner,
                          requested_owner, requested_recipient);
    return;
  }

  if (mp64::ClassifyPeerId(requested_owner) == mp64::PeerIdClass::kExtended) {
    WithExtendedReassignmentOwner(ctx, base, guest_manager, requested_owner, [&](uint8_t alias_id) {
      if (ctx.lr == kReassignmentNegotiationReturnAddress) {
        PopulateExtendedNegotiationMask(ctx, base, guest_manager, requested_owner);
      }
      RunReassignmentSender(ctx, base, __imp__sub_82785AE0, guest_manager, requested_owner,
                            alias_id, requested_recipient);
    });
    return;
  }

  if (ctx.lr == kReassignmentNegotiationReturnAddress) {
    PopulateExtendedNegotiationMask(ctx, base, guest_manager, requested_owner);
  }
  RunReassignmentSender(ctx, base, __imp__sub_82785AE0, guest_manager, requested_owner,
                        requested_owner, requested_recipient);
}

extern "C" void sub_82785D30(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t requested_owner = GuestPeerId(ctx.r4.u64);
  const int8_t requested_recipient = ctx.r5.s8;
  if (!mp64::IsValidPeerId(requested_owner) ||
      (requested_recipient != -1 &&
       !mp64::IsValidPeerId(static_cast<uint8_t>(requested_recipient)))) {
    ctx.r3.u64 = 0;
    return;
  }

  if (g_reassignment_call.active && requested_owner == g_reassignment_call.owner_alias) {
    const uint8_t actual_owner = g_reassignment_call.actual_owner;
    if (ctx.lr == kReassignmentConfirmationReturnAddress) {
      PopulateExtendedConfirmationMask(ctx, base, guest_manager, actual_owner);
    }
    RunReassignmentSender(ctx, base, __imp__sub_82785D30, guest_manager, actual_owner,
                          requested_owner, requested_recipient);
    return;
  }

  if (mp64::ClassifyPeerId(requested_owner) == mp64::PeerIdClass::kExtended) {
    WithExtendedReassignmentOwner(ctx, base, guest_manager, requested_owner, [&](uint8_t alias_id) {
      if (ctx.lr == kReassignmentConfirmationReturnAddress) {
        PopulateExtendedConfirmationMask(ctx, base, guest_manager, requested_owner);
      }
      RunReassignmentSender(ctx, base, __imp__sub_82785D30, guest_manager, requested_owner,
                            alias_id, requested_recipient);
    });
    return;
  }

  if (ctx.lr == kReassignmentConfirmationReturnAddress) {
    PopulateExtendedConfirmationMask(ctx, base, guest_manager, requested_owner);
  }
  RunReassignmentSender(ctx, base, __imp__sub_82785D30, guest_manager, requested_owner,
                        requested_owner, requested_recipient);
}

// The reassignment state machine places a 24-byte live command record every
// 20 bytes and separately indexes owner lists through (owner+42)*8. Extended
// owners therefore execute through a serialized retail-slot window whose
// record, list header and masks are copied to/from the 64-owner sidecar.
extern "C" void sub_82784C48(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (!mp64::IsValidPeerId(peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint8_t logical_peer_id =
      g_reassignment_call.active && peer_id == g_reassignment_call.owner_alias
          ? g_reassignment_call.actual_owner
          : peer_id;
  if (WithExtendedReassignmentOwner(ctx, base, guest_manager, peer_id, [&](uint8_t alias_id) {
        ctx.r4.u64 = alias_id;
        __imp__sub_82784C48(ctx, base);
      })) {
    mp64::ReassignmentOwnerState state = g_reassignments.Get(guest_manager, peer_id);
    state.involved.Clear();
    state.confirmed.Clear();
    state.sent.Clear();
    g_reassignments.Set(guest_manager, peer_id, state);
    return;
  }
  __imp__sub_82784C48(ctx, base);
  mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, logical_peer_id);
  state.involved.Clear();
  state.confirmed.Clear();
  state.sent.Clear();
  g_reassignments.Set(guest_manager, logical_peer_id, state);
}

extern "C" void sub_82786B18(PPCContext& ctx, uint8_t* base) {
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (!mp64::IsValidPeerId(peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  if (WithExtendedReassignmentOwner(ctx, base, ctx.r3.u32, peer_id, [&](uint8_t alias_id) {
        ctx.r4.u64 = alias_id;
        __imp__sub_82786B18(ctx, base);
      })) {
    return;
  }
  __imp__sub_82786B18(ctx, base);
}

extern "C" void sub_82786ED0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (!mp64::IsValidPeerId(peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint8_t logical_peer_id =
      g_reassignment_call.active && peer_id == g_reassignment_call.owner_alias
          ? g_reassignment_call.actual_owner
          : peer_id;
  ResetExtendedReassignmentState(ctx, base, guest_manager, logical_peer_id);
  if (WithExtendedReassignmentOwner(ctx, base, ctx.r3.u32, peer_id, [&](uint8_t alias_id) {
        ctx.r4.u64 = alias_id;
        __imp__sub_82786ED0(ctx, base);
      })) {
    return;
  }
  __imp__sub_82786ED0(ctx, base);
}

extern "C" void sub_82787108(PPCContext& ctx, uint8_t* base) {
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (!mp64::IsValidPeerId(peer_id)) {
    ctx.r3.u64 = 0;
    return;
  }
  if (WithExtendedReassignmentOwner(ctx, base, ctx.r3.u32, peer_id, [&](uint8_t alias_id) {
        ctx.r4.u64 = alias_id;
        __imp__sub_82787108(ctx, base);
      })) {
    return;
  }
  __imp__sub_82787108(ctx, base);
}

extern "C" void sub_82787210(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  __imp__sub_82787210(ctx, base);
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
      const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
      if (!record || REX_LOAD_U32(*record + 12) == 0 || REX_LOAD_U16(*record + 18) != 0 ||
          REX_LOAD_U16(*record + 20) != 0) {
        return;
      }
      PPCContext advance_ctx = ctx;
      advance_ctx.r3.u64 = guest_manager;
      advance_ctx.r4.u64 = alias_id;
      if (REX_LOAD_U32(*record + 12) == 1) {
        sub_82787108(advance_ctx, base);
      } else {
        sub_82784C48(advance_ctx, base);
      }
    });
  }
}

extern "C" void sub_82787340(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_manager = ctx.r3.u32;
  const uint32_t changed_peer = ctx.r4.u32;
  PPCContext id_ctx = ctx;
  id_ctx.r3.u64 = changed_peer;
  __imp__sub_82701F88(id_ctx, base);
  const uint8_t changed_id = id_ctx.r3.u8;
  if (!mp64::IsValidPeerId(changed_id)) {
    __imp__sub_82787340(ctx, base);
    return;
  }

  if (mp64::ClassifyPeerId(changed_id) == mp64::PeerIdClass::kLegacy) {
    __imp__sub_82787340(ctx, base);
    for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
         ++owner_id) {
      WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id, [&](uint8_t alias_id) {
        const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
        if (!record || REX_LOAD_U32(*record + 12) == 0) {
          return;
        }
        uint32_t node = REX_LOAD_U32(*record + 4);
        while (node != 0) {
          const uint32_t next = REX_LOAD_U32(node + 8);
          const uint32_t entry = REX_LOAD_U32(node + 4);
          const uint32_t guest_object = entry != 0 ? REX_LOAD_U32(entry) : 0;
          if (guest_object != 0) {
            PPCContext notify_ctx = ctx;
            notify_ctx.r3.u64 = guest_object;
            notify_ctx.r4.u64 = changed_peer;
            CallNetworkObjectVirtual(notify_ctx, base, guest_object, 132);
          }
          node = next;
        }
        mp64::ReassignmentOwnerState state = g_reassignments.Get(guest_manager, owner_id);
        state.involved.Reset(changed_id);
        state.confirmed.Reset(changed_id);
        state.sent.Reset(changed_id);
        g_reassignments.Set(guest_manager, owner_id, state);
        StoreReassignmentMasks(base, *record, state);
        PPCContext reset_ctx = ctx;
        reset_ctx.r3.u64 = guest_manager;
        reset_ctx.r4.u64 = alias_id;
        sub_82786ED0(reset_ctx, base);
      });
    }
    return;
  }

  // High departing IDs use the canonical 64-owner state transition directly,
  // preserving every unrelated low bit and every owner record.
  PPCContext network_ready_ctx = ctx;
  __imp__sub_826C4CA8(network_ready_ctx, base);
  if (network_ready_ctx.r3.u8 == 0 || changed_peer == 0) {
    return;
  }

  bool changed_owner_active = false;
  WithExtendedReassignmentOwner(ctx, base, guest_manager, changed_id, [&](uint8_t alias_id) {
    const auto record = ReassignmentCommandAddress(guest_manager, alias_id);
    changed_owner_active = record && REX_LOAD_U32(*record + 12) != 0;
    if (!changed_owner_active) {
      return;
    }
    const auto object_list = ReassignmentObjectListAddress(base, guest_manager, alias_id);
    if (!object_list) {
      return;
    }
    const uint32_t guest_root = REX_LOAD_U32(guest_manager);
    while (REX_LOAD_U32(*object_list) != 0) {
      const uint32_t node = REX_LOAD_U32(*object_list);
      PPCContext remove_ctx = ctx;
      remove_ctx.r3.u64 = guest_root;
      remove_ctx.r4.u64 = REX_LOAD_U32(node + 4);
      remove_ctx.r5.u64 = 1;
      remove_ctx.r6.u64 = 1;
      __imp__sub_826EF7D8(remove_ctx, base);
    }
  });
  if (changed_owner_active) {
    return;
  }

  const auto process_owner = [&](uint8_t owner_id, uint8_t physical_owner) {
    const auto record = ReassignmentCommandAddress(guest_manager, physical_owner);
    if (!record || REX_LOAD_U32(*record + 12) == 0) {
      return;
    }
    uint32_t node = REX_LOAD_U32(*record + 4);
    while (node != 0) {
      const uint32_t next = REX_LOAD_U32(node + 8);
      const uint32_t entry = REX_LOAD_U32(node + 4);
      const uint32_t guest_object = entry != 0 ? REX_LOAD_U32(entry) : 0;
      if (guest_object != 0) {
        PPCContext notify_ctx = ctx;
        notify_ctx.r3.u64 = guest_object;
        notify_ctx.r4.u64 = changed_peer;
        CallNetworkObjectVirtual(notify_ctx, base, guest_object, 132);
      }
      node = next;
    }
    mp64::ReassignmentOwnerState state = LoadReassignmentState(base, guest_manager, owner_id);
    state.involved.Reset(changed_id);
    state.confirmed.Reset(changed_id);
    state.sent.Reset(changed_id);
    g_reassignments.Set(guest_manager, owner_id, state);
    StoreReassignmentMasks(base, *record, state);
    PPCContext reset_ctx = ctx;
    reset_ctx.r3.u64 = guest_manager;
    reset_ctx.r4.u64 = physical_owner;
    sub_82786ED0(reset_ctx, base);
  };

  for (uint8_t owner_id = 0; owner_id < mp64::kLegacyPeerCapacity; ++owner_id) {
    process_owner(owner_id, owner_id);
  }
  for (uint8_t owner_id = mp64::kLegacyPeerCapacity; owner_id < mp64::kExtendedPeerCapacity;
       ++owner_id) {
    if (!g_reassignments.Get(guest_manager, owner_id).initialized) {
      continue;
    }
    WithExtendedReassignmentOwner(ctx, base, guest_manager, owner_id,
                                  [&](uint8_t alias_id) { process_owner(owner_id, alias_id); });
  }

  PPCContext network_role_ctx = ctx;
  __imp__sub_826C4DD8(network_role_ctx, base);
  if (network_role_ctx.r3.u8 == 0) {
    PPCContext cleanup_ctx = ctx;
    cleanup_ctx.r3.u64 = guest_manager;
    cleanup_ctx.r4.u64 = changed_id;
    sub_82786B18(cleanup_ctx, base);
  }
}

// Network-array traffic accounting has a retail 16-entry global table. Keep
// high-peer accounting in host storage so serialization never aliases peer
// zero's counters.
extern "C" void sub_8278CCE8(PPCContext& ctx, uint8_t* base) {
  uint8_t peer_id = GuestPeerId(ctx.r3.u64);
  if (g_event_peer_alias.active && peer_id == g_event_peer_alias.alias_peer_id) {
    peer_id = g_event_peer_alias.actual_peer_id;
  }
  if (g_object_peer_alias.active && peer_id == g_object_peer_alias.alias_peer_id) {
    peer_id = g_object_peer_alias.actual_peer_id;
  }
  if (g_dispatch_alias.active && peer_id == g_dispatch_alias.alias_peer_id) {
    peer_id = g_dispatch_alias.actual_peer_id;
  }
  if (mp64::ClassifyPeerId(peer_id) == mp64::PeerIdClass::kExtended) {
    std::scoped_lock lock(g_extended_traffic_mutex);
    g_extended_traffic_bytes[peer_id] += ctx.r4.u32;
    ctx.r3.u64 = peer_id;
    return;
  }
  __imp__sub_8278CCE8(ctx, base);
}

// The normal outbound-flush helper derives its 1 KiB message buffer by
// indexing an embedded sixteen-record array. During an extended dispatch call
// the exact selected sidecar buffer is already known, so execute the same send,
// accounting, and queue-reset sequence directly against that buffer.
extern "C" void sub_826DAC48(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_network_manager = ctx.r3.u32;
  const uint8_t requested_peer_id = GuestPeerId(ctx.r4.u64);
  if (!g_dispatch_alias.active || requested_peer_id != g_dispatch_alias.alias_peer_id ||
      guest_network_manager != g_dispatch_alias.guest_network_manager) {
    __imp__sub_826DAC48(ctx, base);
    return;
  }

  const uint32_t guest_peer = g_dispatch_alias.guest_peer;
  const uint32_t guest_message = g_dispatch_alias.guest_message;
  const auto queue_address =
      mp64::CheckedGuestAddress(guest_message, mp64::kDispatchMessageQueueOffset);
  const auto payload_address =
      mp64::CheckedGuestAddress(guest_message, mp64::kDispatchMessagePayloadOffset);
  const auto sender_address =
      mp64::CheckedGuestAddress(guest_network_manager, mp64::kNetworkArrayManagerPointerOffset);
  if (guest_peer == 0 || !queue_address || !payload_address || !sender_address) {
    ctx.r3.u64 = 0;
    return;
  }

  PPCContext nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_peer;
  sub_82708620(nested_ctx, base);
  if (nested_ctx.r3.u8 == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  nested_ctx = ctx;
  nested_ctx.r3.u64 = *queue_address;
  sub_82853560(nested_ctx, base);
  if (nested_ctx.r3.s32 <= 0) {
    ctx.r3.u64 = nested_ctx.r3.u64;
    return;
  }

  nested_ctx = ctx;
  nested_ctx.r3.u64 = guest_peer;
  sub_82A58008(nested_ctx, base);
  const uint32_t peer_transport_id = nested_ctx.r3.u32;
  nested_ctx = ctx;
  nested_ctx.r3.u64 = REX_LOAD_U32(*sender_address);
  nested_ctx.r4.u64 = peer_transport_id;
  nested_ctx.r5.u64 = guest_message;
  nested_ctx.r6.u64 = 0;
  nested_ctx.r7.u64 = 0;
  sub_826DA690(nested_ctx, base);

  nested_ctx = ctx;
  nested_ctx.r3.u64 = *queue_address;
  sub_82853560(nested_ctx, base);
  const uint32_t message_bits = nested_ctx.r3.u32;
  nested_ctx = ctx;
  nested_ctx.r3.u64 = g_dispatch_alias.actual_peer_id;
  nested_ctx.r4.u64 = message_bits + mp64::kDispatchMessageWireHeaderBytes;
  sub_8278CCE8(nested_ctx, base);

  ctx.r3.u64 = *queue_address;
  ctx.r4.u64 = *payload_address;
  ctx.r5.u64 = mp64::kDispatchMessagePayloadCapacity;
  sub_82853400(ctx, base);
}

// The retail network-array tick explicitly enumerates peer IDs 0..15. Retain
// that path, then perform the identical host-side eligibility checks for every
// live sidecar peer and feed it through the widened serializer.
extern "C" void sub_826D9E10(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_network_manager = ctx.r3.u32;
  __imp__sub_826D9E10(ctx, base);
  const auto handler_list_address =
      mp64::CheckedGuestAddress(guest_network_manager, mp64::kNetworkArrayHandlerListOffset);
  const auto peer_manager_address =
      mp64::CheckedGuestAddress(guest_network_manager, mp64::kNetworkArrayPeerManagerOffset);
  if (!handler_list_address || !peer_manager_address) {
    return;
  }

  uint32_t guest_node = REX_LOAD_U32(*handler_list_address);
  while (guest_node != 0) {
    const auto value_address =
        mp64::CheckedGuestAddress(guest_node, mp64::kNetworkArrayHandlerNodeValueOffset);
    const auto next_address =
        mp64::CheckedGuestAddress(guest_node, mp64::kNetworkArrayHandlerNodeNextOffset);
    if (!value_address || !next_address) {
      return;
    }
    const uint32_t guest_handler = REX_LOAD_U32(*value_address);
    guest_node = REX_LOAD_U32(*next_address);
    if (guest_handler == 0) {
      continue;
    }

    PPCContext nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_handler;
    if (!CallNetworkObjectVirtual(nested_ctx, base, guest_handler, 12)) {
      continue;
    }
    nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_handler;
    if (!CallNetworkObjectVirtual(nested_ctx, base, guest_handler, 60) || nested_ctx.r3.u8 == 0) {
      continue;
    }
    const auto state_address = mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchStateOffset);
    const auto initialized_address =
        mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchInitializedOffset);
    const auto send_all_address =
        mp64::CheckedGuestAddress(guest_handler, mp64::kDispatchSendAllOffset);
    if (!state_address || !initialized_address || !send_all_address) {
      continue;
    }
    const uint32_t state = REX_LOAD_U32(*state_address);
    nested_ctx = ctx;
    sub_826C4D78(nested_ctx, base);
    const bool send_to_all_peers = (state == 0 && nested_ctx.r3.u8 != 0) || state == 2;
    if (!send_to_all_peers) {
      continue;
    }
    if (REX_LOAD_U8(*initialized_address) == 0) {
      nested_ctx = ctx;
      nested_ctx.r3.u64 = guest_handler;
      sub_82705998(nested_ctx, base);
    }

    SyncDispatchLegacyMasks(base, guest_handler);
    const mp64::PeerMaskPair64 masks = g_dispatch_masks.Get(guest_handler);
    for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
         ++peer_id) {
      if (REX_LOAD_U8(*send_all_address) == 0 && !masks.second.Contains(peer_id) &&
          masks.first.Contains(peer_id)) {
        continue;
      }
      nested_ctx = ctx;
      nested_ctx.r3.u64 = REX_LOAD_U32(*peer_manager_address);
      nested_ctx.r4.u64 = peer_id;
      sub_826FE880(nested_ctx, base);
      const uint32_t guest_peer = nested_ctx.r3.u32;
      if (guest_peer == 0) {
        continue;
      }
      nested_ctx = ctx;
      nested_ctx.r3.u64 = guest_peer;
      sub_82708620(nested_ctx, base);
      if (nested_ctx.r3.u8 == 0) {
        continue;
      }
      nested_ctx = ctx;
      nested_ctx.r3.u64 = guest_peer;
      sub_82708770(nested_ctx, base);
      if (nested_ctx.r3.u8 != 0) {
        continue;
      }
      nested_ctx = ctx;
      nested_ctx.r3.u64 = guest_peer;
      sub_82191F00(nested_ctx, base);
      if (nested_ctx.r3.s32 != 5) {
        continue;
      }
      nested_ctx = ctx;
      nested_ctx.r3.u64 = guest_peer;
      sub_82A58008(nested_ctx, base);
      sub_826C4FA0(nested_ctx, base);
      if (nested_ctx.r3.u8 == 0) {
        continue;
      }
      nested_ctx = ctx;
      nested_ctx.r3.u64 = guest_handler;
      nested_ctx.r4.u64 = guest_peer;
      nested_ctx.r5.u64 = 0;
      sub_82705BD0(nested_ctx, base);
    }
    nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_handler;
    sub_82705300(nested_ctx, base);
  }
}

// Joining/leaving peers may reuse a sparse ID. Reinitialize that peer's
// sidecar message queue at the same lifecycle points used by all handlers.
extern "C" void sub_826DA050(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_network_manager = ctx.r3.u32;
  const auto peer_id = PeerRecordId(base, ctx.r4.u32);
  if (peer_id && mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended) {
    ResetDispatchMessage(ctx, base, guest_network_manager, *peer_id);
  }
  __imp__sub_826DA050(ctx, base);
}

extern "C" void sub_826DA0E0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_network_manager = ctx.r3.u32;
  const auto peer_id = PeerRecordId(base, ctx.r4.u32);
  __imp__sub_826DA0E0(ctx, base);
  if (peer_id && mp64::ClassifyPeerId(*peer_id) == mp64::PeerIdClass::kExtended) {
    ResetDispatchMessage(ctx, base, guest_network_manager, *peer_id);
  }
}

extern "C" void sub_826DAD00(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_network_manager = ctx.r3.u32;
  __imp__sub_826DAD00(ctx, base);
  DestroyDispatchMessages(guest_network_manager);
}

// Reset the extended per-element sequence/acknowledgement state alongside the
// retail sixteen-entry arrays.
extern "C" void sub_82704D40(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  __imp__sub_82704D40(ctx, base);
  g_dispatch_peer_states.Reset(guest_dispatch);
}

// Rebuild CDispatchOrderArrayHandler's "eligible peer" mask for sidecar peers.
// The original continues to own all embedded dispatch state and low 32 bits;
// the same two record predicates are evaluated for IDs 16..63.
extern "C" void sub_82704FE8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  __imp__sub_82704FE8(ctx, base);
  if (guest_dispatch == 0) {
    return;
  }
  SyncDispatchLegacyMasks(base, guest_dispatch);
  g_dispatch_masks.ClearNonLegacy16(guest_dispatch, true, false);
  g_dispatch_masks.Clear(guest_dispatch, false, true);

  const auto state_address = mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchStateOffset);
  if (!state_address || REX_LOAD_U32(*state_address) != 0) {
    return;
  }
  PPCContext nested_ctx = ctx;
  __imp__sub_826C4D78(nested_ctx, base);
  if (nested_ctx.r3.u8 == 0) {
    return;
  }

  for (uint8_t peer_id = mp64::kLegacyPeerCapacity; peer_id < mp64::kExtendedPeerCapacity;
       ++peer_id) {
    nested_ctx = ctx;
    nested_ctx.r3.u64 = mp64::kGlobalPeerManagerAddress;
    nested_ctx.r4.u64 = peer_id;
    sub_826FE880(nested_ctx, base);
    const uint32_t guest_peer = nested_ctx.r3.u32;
    if (guest_peer == 0) {
      continue;
    }
    nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_peer;
    __imp__sub_82708620(nested_ctx, base);
    if (nested_ctx.r3.u8 == 0) {
      continue;
    }
    nested_ctx = ctx;
    nested_ctx.r3.u64 = guest_peer;
    __imp__sub_82708770(nested_ctx, base);
    if (nested_ctx.r3.u8 == 0) {
      g_dispatch_masks.Set(guest_dispatch, peer_id, true, false);
    }
  }
}

// Peer removal clears both dispatch masks. Capture the ID before the original
// record teardown, then mirror its low-word mutation and clear the high bit.
extern "C" void sub_82705190(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  const auto peer_id = PeerRecordId(base, ctx.r4.u32);
  if (!peer_id || mp64::ClassifyPeerId(*peer_id) != mp64::PeerIdClass::kExtended) {
    __imp__sub_82705190(ctx, base);
    SyncDispatchLegacyMasks(base, guest_dispatch);
    if (peer_id && mp64::IsValidPeerId(*peer_id)) {
      g_dispatch_masks.Reset(guest_dispatch, *peer_id, true, true);
      g_dispatch_peer_states.RemovePeer(guest_dispatch, *peer_id);
    }
    return;
  }

  // The retail peer-removal path indexes two per-element allocations whose
  // capacities are exactly sixteen and a one-word bitset whose capacity is
  // exactly 32. Extended peers never live in those allocations. Preserve the
  // authority cleanup that is independent of those tables, and clear the
  // canonical 64-bit masks without entering either unsafe expression.
  const auto mode = mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchStateOffset);
  const auto count = mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchElementCountOffset);
  const auto element_states =
      mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchElementStateTableOffset);
  const auto authorities =
      mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchAuthorityTableOffset);
  if (mode && count && element_states && authorities && REX_LOAD_U32(*mode) == 2) {
    const uint32_t guest_element_states = REX_LOAD_U32(*element_states);
    const uint32_t guest_authorities = REX_LOAD_U32(*authorities);
    for (uint32_t element = 0; element < REX_LOAD_U32(*count); ++element) {
      const auto authority = mp64::CheckedGuestArrayAddress(guest_authorities, element,
                                                            mp64::kDispatchAuthorityRecordSize);
      if (!authority || REX_LOAD_U8(*authority) != *peer_id) {
        continue;
      }
      if (REX_LOAD_U8(*authority + 1) != 0) {
        const auto state = mp64::CheckedGuestArrayAddress(guest_element_states, element,
                                                          mp64::kDispatchElementStateRecordSize);
        if (state) {
          const uint8_t flags = REX_LOAD_U8(*state + 2);
          REX_STORE_U16(*state, 0);
          REX_STORE_U8(*state + 2, flags & 0x7F);
        }
      } else {
        PPCContext clear_ctx = ctx;
        clear_ctx.r3.u64 = guest_dispatch;
        clear_ctx.r4.u64 = element;
        clear_ctx.r5.u64 = 0;
        CallNetworkObjectVirtual(clear_ctx, base, guest_dispatch, 48);
      }
    }
  }
  g_dispatch_masks.Reset(guest_dispatch, *peer_id, true, true);
  g_dispatch_peer_states.RemovePeer(guest_dispatch, *peer_id);
  ctx.r3.u64 = *peer_id;
}

// Acknowledgements directly index the retail sixteen-entry state arrays. A
// high peer is projected through slot zero, then every mutation is captured
// back into its canonical sidecar record.
extern "C" void sub_82705390(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  const uint8_t peer_id = GuestPeerId(ctx.r4.u64);
  if (mp64::IsLegacyPeerId(peer_id)) {
    __imp__sub_82705390(ctx, base);
    SyncDispatchLegacyMasks(base, guest_dispatch);
    return;
  }
  if (mp64::ClassifyPeerId(peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = guest_dispatch;
    return;
  }
  const uint32_t guest_peer = g_peer_managers.GetPeer(mp64::kGlobalPeerManagerAddress, peer_id);
  if (guest_peer == 0) {
    ctx.r3.u64 = guest_dispatch;
    return;
  }
  WithExtendedDispatchPeer(ctx, base, guest_dispatch, guest_peer, peer_id, 0,
                           [&](uint8_t alias_id) {
                             ctx.r3.u64 = guest_dispatch;
                             ctx.r4.u64 = alias_id;
                             __imp__sub_82705390(ctx, base);
                           });
  ctx.r3.u64 = guest_dispatch;
}

// Changing an element's authority clears the retail state/ack slot for every
// peer. Mirror that broadcast reset across all extended peer records.
extern "C" void sub_82705488(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  const size_t element = ctx.r4.u32;
  const uint8_t new_authority = GuestPeerId(ctx.r5.u64);
  bool authority_changed = false;
  const auto count_address =
      mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchElementCountOffset);
  const auto authority_table_address =
      mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchAuthorityTableOffset);
  if (count_address && authority_table_address && element < REX_LOAD_U32(*count_address)) {
    const uint32_t guest_authorities = REX_LOAD_U32(*authority_table_address);
    const auto authority = mp64::CheckedGuestArrayAddress(guest_authorities, element,
                                                          mp64::kDispatchAuthorityRecordSize);
    authority_changed = authority && REX_LOAD_U8(*authority) != new_authority;
  }
  __imp__sub_82705488(ctx, base);
  if (authority_changed) {
    g_dispatch_peer_states.ResetElement(guest_dispatch, element);
  }
}

// The outbound serializer uses four independent fixed-width peer fields: a
// four-byte state record per element, an acknowledgement bit per element, and
// two handler-wide masks. Projecting them as one serialized transaction keeps
// its exact retail retry/sequence behavior for peers 16..63.
extern "C" void sub_82705BD0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  const auto peer_id = PeerRecordId(base, guest_peer);
  if (!peer_id || mp64::IsLegacyPeerId(*peer_id)) {
    __imp__sub_82705BD0(ctx, base);
    SyncDispatchLegacyMasks(base, guest_dispatch);
    return;
  }
  if (mp64::ClassifyPeerId(*peer_id) != mp64::PeerIdClass::kExtended) {
    ctx.r3.u64 = mp64::kInvalidPeerId;
    return;
  }
  const auto send_all_address =
      mp64::CheckedGuestAddress(guest_dispatch, mp64::kDispatchSendAllOffset);
  SyncDispatchLegacyMasks(base, guest_dispatch);
  const mp64::PeerMaskPair64 masks = g_dispatch_masks.Get(guest_dispatch);
  if (send_all_address && REX_LOAD_U8(*send_all_address) == 0 && !masks.second.Contains(*peer_id) &&
      masks.first.Contains(*peer_id)) {
    ctx.r3.u64 = *peer_id;
    return;
  }
  const auto manager_address =
      mp64::CheckedGuestAddress(guest_dispatch, mp64::kNetworkArrayManagerPointerOffset);
  const uint32_t guest_network_manager = manager_address ? REX_LOAD_U32(*manager_address) : 0;
  const uint32_t guest_message = EnsureDispatchMessage(ctx, base, guest_network_manager, *peer_id);
  if (guest_message == 0) {
    ctx.r3.u64 = *peer_id;
    return;
  }
  WithExtendedDispatchPeer(ctx, base, guest_dispatch, guest_peer, *peer_id, guest_message,
                           [&](uint8_t) {
                             ctx.r3.u64 = guest_dispatch;
                             ctx.r4.u64 = guest_peer;
                             ctx.r5.u64 = guest_message;
                             __imp__sub_82705BD0(ctx, base);
                           });
  ctx.r3.u64 = *peer_id;
}

// Dispatch reset/initialization zeroes both embedded masks and invalidates the
// matching sidecar lifetime.
extern "C" void sub_82706CA8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  __imp__sub_82706CA8(ctx, base);
  g_dispatch_masks.Remove(guest_dispatch);
  g_dispatch_peer_states.Reset(guest_dispatch);
}

// CDispatchOrderArrayHandler clears two 32-bit masks using peer->id. Mirror
// low words after the retail path and handle IDs 32..63 exclusively in the
// sidecar, avoiding the PowerPC shift-to-zero behavior.
extern "C" void sub_82704AF8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  const uint32_t guest_peer = ctx.r4.u32;
  const auto peer_id = PeerRecordId(base, guest_peer);
  if (!peer_id || !mp64::IsValidPeerId(*peer_id)) {
    __imp__sub_82704AF8(ctx, base);
    SyncDispatchLegacyMasks(base, guest_dispatch);
    return;
  }
  if (*peer_id < mp64::kLegacyCommandParticipantCapacity) {
    __imp__sub_82704AF8(ctx, base);
    SyncDispatchLegacyMasks(base, guest_dispatch);
  }
  g_dispatch_masks.Reset(guest_dispatch, *peer_id, true, true);
  g_dispatch_peer_states.RemovePeer(guest_dispatch, *peer_id);
}

// The deleting destructor is reachable without the reset virtual on several
// handler subclasses. Retire both sidecars at that unconditional seam.
extern "C" void sub_82718420(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_dispatch = ctx.r3.u32;
  __imp__sub_82718420(ctx, base);
  g_dispatch_masks.Remove(guest_dispatch);
  g_dispatch_peer_states.Reset(guest_dispatch);
}

// rlSession construction relocates the participant table before participant
// 32 would collide with the retail count field at +1544. Low records are
// mirrored into the embedded table for unchanged retail consumers.
extern "C" void sub_829F9810(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  __imp__sub_829F9810(ctx, base);
  rex::Runtime* runtime = rex::Runtime::instance();
  if (guest_session == 0 || runtime == nullptr) {
    return;
  }
  const mp64::SessionParticipantState stale = g_session_participants.RemoveSession(guest_session);
  if (stale.guest_record_table != 0) {
    runtime->memory()->SystemHeapFree(stale.guest_record_table);
  }
  const uint32_t guest_table =
      runtime->memory()->SystemHeapAlloc(mp64::kExtendedSessionParticipantTableSize);
  if (guest_table == 0 || !g_session_participants.Register(guest_session, guest_table)) {
    if (guest_table != 0) {
      runtime->memory()->SystemHeapFree(guest_table);
    }
    return;
  }
  for (uint32_t index = 0; index < mp64::kExtendedPeerCapacity; ++index) {
    const auto record =
        mp64::CheckedGuestArrayAddress(guest_table, index, mp64::kSessionParticipantRecordSize);
    if (!record) {
      break;
    }
    InitializeSessionParticipantRecord(base, *record);
  }
}

// Command objects can be destroyed by queue cancellation and session teardown
// without reaching their normal completion consumers. Release relocated
// CmdJoin/CmdLeave payloads at the common command-deallocation seam.
extern "C" void sub_829F58B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_command = ctx.r3.u32;
  ReleaseParticipantCommandSidecar(guest_command);
  ctx.r3.u64 = guest_command;
  __imp__sub_829F58B8(ctx, base);
}

// rlSession's non-deleting destructor is reached by both direct destruction
// and the deleting vfunc. Keep the relocated participant table alive through
// retail teardown, then release it exactly once.
extern "C" void sub_829F9E58(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  __imp__sub_829F9E58(ctx, base);
  ReleaseSessionParticipantSidecar(guest_session);
}

// Add a participant while preserving the title's lock/state behavior. IDs
// 32..63 use embedded record 31 only as a serialized constructor scratch;
// the old bytes and all overlapping session fields are restored before the
// original routine returns.
extern "C" void sub_829F5628(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_participant = ctx.r4.u32;
  const bool is_private = ctx.r5.u32 != 0;
  const auto participant_peer_id = ParticipantPeerId(ctx, base, guest_participant);
  std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
  mp64::SessionParticipantState state = g_session_participants.Get(guest_session);
  if (state.guest_record_table == 0) {
    __imp__sub_829F5628(ctx, base);
    return;
  }
  if (state.count == mp64::kExtendedPeerCapacity) {
    ctx.r3.u64 = 0;
    return;
  }
  if (state.count < mp64::kLegacyCommandParticipantCapacity) {
    __imp__sub_829F5628(ctx, base);
    const auto count_address =
        mp64::CheckedGuestAddress(guest_session, mp64::kSessionParticipantCountOffset);
    if (count_address && REX_LOAD_U32(*count_address) == state.count + 1 &&
        CopyInlineSessionParticipantsToSidecar(base, guest_session, state.count + 1)) {
      StoreSessionParticipantCounts(base, guest_session, g_session_participants.Get(guest_session));
      if (participant_peer_id) {
        validation::PublishMultiplayerValidationStage(
            validation::MultiplayerValidationStage::kParticipantAdd, *participant_peer_id,
            guest_participant);
      }
    }
    return;
  }

  const auto inline_table =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionParticipantTableOffset);
  const auto scratch_record =
      inline_table ? mp64::CheckedGuestArrayAddress(*inline_table,
                                                    mp64::kLegacyCommandParticipantCapacity - 1,
                                                    mp64::kSessionParticipantRecordSize)
                   : std::nullopt;
  const auto count_address =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionParticipantCountOffset);
  const auto public_address = mp64::CheckedGuestAddress(guest_session, 1548);
  const auto private_address = mp64::CheckedGuestAddress(guest_session, 1552);
  const auto destination = mp64::CheckedGuestArrayAddress(state.guest_record_table, state.count,
                                                          mp64::kSessionParticipantRecordSize);
  if (!scratch_record || !count_address || !public_address || !private_address || !destination) {
    ctx.r3.u64 = 0;
    return;
  }

  std::array<uint8_t, mp64::kSessionParticipantRecordSize> saved_scratch{};
  std::memcpy(saved_scratch.data(), base + *scratch_record, saved_scratch.size());
  const uint32_t saved_public = REX_LOAD_U32(*public_address);
  const uint32_t saved_private = REX_LOAD_U32(*private_address);
  REX_STORE_U32(*count_address, mp64::kLegacyCommandParticipantCapacity - 1);
  ctx.r3.u64 = guest_session;
  ctx.r4.u64 = guest_participant;
  ctx.r5.u64 = is_private;
  __imp__sub_829F5628(ctx, base);
  const bool constructed = REX_LOAD_U32(*count_address) == mp64::kLegacyCommandParticipantCapacity;
  if (constructed) {
    std::memcpy(base + *destination, base + *scratch_record, mp64::kSessionParticipantRecordSize);
  }
  std::memcpy(base + *scratch_record, saved_scratch.data(), saved_scratch.size());
  REX_STORE_U32(*public_address, saved_public);
  REX_STORE_U32(*private_address, saved_private);
  if (constructed && g_session_participants.Add(guest_session, is_private)) {
    if (participant_peer_id) {
      validation::PublishMultiplayerValidationStage(
          validation::MultiplayerValidationStage::kParticipantAdd, *participant_peer_id,
          guest_participant);
    }
    state = g_session_participants.Get(guest_session);
    SyncLegacySessionParticipantMirror(base, guest_session, state);
  } else {
    REX_STORE_U32(*count_address, mp64::kLegacyCommandParticipantCapacity);
  }
}

extern "C" void sub_829F56E8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_identity = ctx.r4.u32;
  const mp64::SessionParticipantState state = g_session_participants.Get(guest_session);
  if (state.guest_record_table == 0) {
    __imp__sub_829F56E8(ctx, base);
    return;
  }
  std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
  ctx.r3.s64 = FindSessionParticipant(ctx, base, state, guest_identity);
}

// Invite duplicate filtering must inspect the relocated half of the roster as
// well as the mirrored first 32 records.
extern "C" void sub_829F6630(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_identities = ctx.r4.u32;
  const int32_t identity_count = ctx.r5.s32;
  const mp64::SessionParticipantState state = g_session_participants.Get(guest_session);
  if (state.guest_record_table == 0 || identity_count <= 0 || identity_count > 31) {
    __imp__sub_829F6630(ctx, base);
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    __imp__sub_829F6630(ctx, base);
    return;
  }
  const uint32_t guest_filtered =
      runtime->memory()->SystemHeapAlloc(mp64::kMaxInviteIdentityBufferSize);
  if (guest_filtered == 0) {
    __imp__sub_829F6630(ctx, base);
    return;
  }
  uint32_t filtered_count = 0;
  {
    std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
    for (int32_t input_index = 0; input_index < identity_count; ++input_index) {
      const auto input = mp64::CheckedGuestArrayAddress(
          guest_identities, static_cast<size_t>(input_index), mp64::kInviteIdentitySize);
      if (!input) {
        continue;
      }
      bool duplicate = false;
      for (uint32_t participant_index = 0; participant_index < state.count; ++participant_index) {
        const auto record = SessionParticipantRecordAddress(state, participant_index);
        const auto identity = record ? mp64::CheckedGuestAddress(*record, 8) : std::nullopt;
        if (!identity) {
          continue;
        }
        PPCContext compare_ctx = ctx;
        compare_ctx.r3.u64 = *input;
        compare_ctx.r4.u64 = *identity;
        __imp__sub_829E4F90(compare_ctx, base);
        if (compare_ctx.r3.u8 != 0) {
          duplicate = true;
          break;
        }
      }
      if (!duplicate) {
        const auto output = mp64::CheckedGuestArrayAddress(guest_filtered, filtered_count++,
                                                           mp64::kInviteIdentitySize);
        if (output) {
          std::memcpy(base + *output, base + *input, mp64::kInviteIdentitySize);
        }
      }
    }
  }
  if (filtered_count == 0) {
    runtime->memory()->SystemHeapFree(guest_filtered);
    ctx.r3.u64 = 1;
    return;
  }
  ctx.r4.u64 = guest_filtered;
  ctx.r5.u64 = filtered_count;
  __imp__sub_829F6630(ctx, base);
  runtime->memory()->SystemHeapFree(guest_filtered);
}

extern "C" void sub_829F7C38(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_identity = ctx.r4.u32;
  std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
  mp64::SessionParticipantState state = g_session_participants.Get(guest_session);
  if (state.guest_record_table == 0) {
    __imp__sub_829F7C38(ctx, base);
    return;
  }
  const int32_t index = FindSessionParticipant(ctx, base, state, guest_identity);
  if (index < 0 || state.count == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const auto record = SessionParticipantRecordAddress(state, static_cast<uint32_t>(index));
  const auto last_record = SessionParticipantRecordAddress(state, state.count - 1);
  const auto private_flag =
      record ? mp64::CheckedGuestAddress(*record, mp64::kSessionParticipantPrivateFlagOffset)
             : std::nullopt;
  if (!record || !last_record || !private_flag) {
    ctx.r3.u64 = 0;
    return;
  }
  const bool was_private = REX_LOAD_U32(*private_flag) != 0;
  if (*record != *last_record) {
    std::memcpy(base + *record, base + *last_record, mp64::kSessionParticipantRecordSize);
  }
  InitializeSessionParticipantRecord(base, *last_record);
  if (!g_session_participants.Remove(guest_session, was_private)) {
    ctx.r3.u64 = 0;
    return;
  }
  state = g_session_participants.Get(guest_session);
  SyncLegacySessionParticipantMirror(base, guest_session, state);
  ctx.r3.u64 = guest_session;
}

extern "C" void sub_829F8C80(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  __imp__sub_829F8C80(ctx, base);
  const mp64::SessionParticipantState state = g_session_participants.Get(guest_session);
  if (state.guest_record_table != 0) {
    g_session_participants.SetCounts(guest_session, 0, 0, 0);
    for (uint32_t index = 0; index < mp64::kExtendedPeerCapacity; ++index) {
      const auto record = mp64::CheckedGuestArrayAddress(state.guest_record_table, index,
                                                         mp64::kSessionParticipantRecordSize);
      if (record) {
        InitializeSessionParticipantRecord(base, *record);
      }
    }
  }
}

extern "C" void sub_829F7428(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_command = ctx.r4.u32;
  const mp64::ParticipantCommandState command_state = g_participant_commands.Get(guest_command);
  if (command_state.guest_record_table == 0) {
    __imp__sub_829F7428(ctx, base);
    return;
  }
  std::vector<uint32_t> public_records;
  std::vector<uint32_t> private_records;
  public_records.reserve(command_state.public_count);
  private_records.reserve(command_state.private_count);
  for (uint32_t index = 0; index < command_state.public_count; ++index) {
    const auto record = mp64::CheckedGuestArrayAddress(command_state.guest_record_table, index,
                                                       mp64::kSessionParticipantRecordSize);
    if (record) {
      public_records.push_back(*record);
    }
  }
  for (uint32_t index = 0; index < command_state.private_count; ++index) {
    const auto record = mp64::CheckedGuestArrayAddress(command_state.guest_record_table,
                                                       command_state.public_count + index,
                                                       mp64::kSessionParticipantRecordSize);
    if (record) {
      private_records.push_back(*record);
    }
  }
  size_t public_start = 0;
  size_t private_start = 0;
  while (public_start < public_records.size() || private_start < private_records.size()) {
    const size_t public_batch = std::min<size_t>(mp64::kLegacyCommandParticipantCapacity,
                                                 public_records.size() - public_start);
    const size_t private_batch =
        std::min<size_t>(mp64::kLegacyCommandParticipantCapacity - public_batch,
                         private_records.size() - private_start);
    if (!RunLeavePlatformBatch(ctx, base, guest_session, public_records, private_records,
                               public_start, private_start)) {
      ctx.r3.u64 = 0;
      return;
    }
    public_start += public_batch;
    private_start += private_batch;
  }
  ctx.r3.u64 = 1;
}

// Consume relocated CmdLeave payloads without entering the retail loops that
// assume both command records and the session participant table stop at 32.
extern "C" void sub_829F9118(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_command = ctx.r4.u32;
  const mp64::ParticipantCommandState command_state = g_participant_commands.Get(guest_command);
  const mp64::SessionParticipantState participant_state = g_session_participants.Get(guest_session);
  if (command_state.guest_record_table == 0 || participant_state.guest_record_table == 0) {
    __imp__sub_829F9118(ctx, base);
    return;
  }

  std::vector<uint32_t> public_records;
  std::vector<uint32_t> private_records;
  public_records.reserve(command_state.public_count);
  private_records.reserve(command_state.private_count);
  {
    std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
    for (uint32_t index = 0; index < command_state.public_count; ++index) {
      const auto record = mp64::CheckedGuestArrayAddress(command_state.guest_record_table, index,
                                                         mp64::kSessionParticipantRecordSize);
      if (record && FindSessionParticipant(ctx, base, participant_state, *record) >= 0) {
        public_records.push_back(*record);
      }
    }
    for (uint32_t index = 0; index < command_state.private_count; ++index) {
      const auto record = mp64::CheckedGuestArrayAddress(command_state.guest_record_table,
                                                         command_state.public_count + index,
                                                         mp64::kSessionParticipantRecordSize);
      if (record && FindSessionParticipant(ctx, base, participant_state, *record) >= 0) {
        private_records.push_back(*record);
      }
    }
  }

  for (const uint32_t record : public_records) {
    PPCContext remove_ctx = ctx;
    remove_ctx.r3.u64 = guest_session;
    remove_ctx.r4.u64 = record;
    sub_829F7C38(remove_ctx, base);
  }
  for (const uint32_t record : private_records) {
    PPCContext remove_ctx = ctx;
    remove_ctx.r3.u64 = guest_session;
    remove_ctx.r4.u64 = record;
    sub_829F7C38(remove_ctx, base);
  }

  size_t public_start = 0;
  size_t private_start = 0;
  while (public_start < public_records.size() || private_start < private_records.size()) {
    const size_t public_batch = std::min<size_t>(mp64::kLegacyCommandParticipantCapacity,
                                                 public_records.size() - public_start);
    const size_t private_batch =
        std::min<size_t>(mp64::kLegacyCommandParticipantCapacity - public_batch,
                         private_records.size() - private_start);
    if (!RunLeavePlatformBatch(ctx, base, guest_session, public_records, private_records,
                               public_start, private_start)) {
      break;
    }
    public_start += public_batch;
    private_start += private_batch;
  }
  ReleaseParticipantCommandSidecar(guest_command);
  ctx.r3.u64 = 1;
}

// SetMaxSlots completion reclassifies public/private records. The original
// mutates only its 32 inline records, so re-run the same ordered conversion on
// the canonical table and then refresh the mirror.
extern "C" void sub_829F9418(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const mp64::SessionParticipantState before = g_session_participants.Get(guest_session);
  __imp__sub_829F9418(ctx, base);
  if (ctx.r3.u8 == 0 || before.guest_record_table == 0 ||
      before.count <= mp64::kLegacyCommandParticipantCapacity) {
    return;
  }
  const auto public_slots =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPublicSlotsOffset);
  const auto private_slots =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPrivateSlotsOffset);
  if (!public_slots || !private_slots) {
    return;
  }

  uint32_t public_count = 0;
  uint32_t private_count = 0;
  uint32_t retained_public = 0;
  uint32_t retained_private = 0;
  const uint32_t requested_public = REX_LOAD_U32(*public_slots);
  const uint32_t requested_private = REX_LOAD_U32(*private_slots);
  for (uint32_t index = 0; index < before.count; ++index) {
    const auto record = SessionParticipantRecordAddress(before, index);
    const auto private_flag =
        record ? mp64::CheckedGuestAddress(*record, mp64::kSessionParticipantPrivateFlagOffset)
               : std::nullopt;
    if (!private_flag) {
      return;
    }
    bool is_private = REX_LOAD_U32(*private_flag) != 0;
    if (!is_private && requested_public < before.public_count) {
      ++retained_public;
      if (retained_public > requested_public) {
        is_private = true;
        REX_STORE_U32(*private_flag, 1);
      }
    } else if (is_private && requested_private < before.private_count) {
      ++retained_private;
      if (retained_private > requested_private) {
        is_private = false;
        REX_STORE_U32(*private_flag, 0);
      }
    }
    if (is_private) {
      ++private_count;
    } else {
      ++public_count;
    }
  }
  if (g_session_participants.SetCounts(guest_session, before.count, public_count, private_count)) {
    SyncLegacySessionParticipantMirror(base, guest_session,
                                       g_session_participants.Get(guest_session));
  }
}

// Session command queue seam. SetMaxSlots has a fixed 36-byte command, so its
// schema already carries 32-bit public/private counts and needs no larger
// allocation. The original routine performs all state/lock/callback work with
// a <=32 proxy; patch the command before the queue sees it.
extern "C" void sub_829F52E8(PPCContext& ctx, uint8_t* base) {
  if (g_extended_slot_command.active && ctx.r3.u32 == g_extended_slot_command.guest_session &&
      ctx.r4.u32 != 0) {
    const uint32_t guest_command = ctx.r4.u32;
    const auto type_address =
        mp64::CheckedGuestAddress(guest_command, mp64::kSessionCommandTypeOffset);
    const auto public_address =
        mp64::CheckedGuestAddress(guest_command, mp64::kSetMaxSlotsCommandPublicOffset);
    const auto private_address =
        mp64::CheckedGuestAddress(guest_command, mp64::kSetMaxSlotsCommandPrivateOffset);
    if (type_address && public_address && private_address &&
        REX_LOAD_U32(*type_address) == mp64::kSetMaxSlotsCommandType) {
      REX_STORE_U32(*public_address, g_extended_slot_command.public_slots);
      REX_STORE_U32(*private_address, g_extended_slot_command.private_slots);
      g_extended_slot_command.command_patched = true;
    }
  }
  __imp__sub_829F52E8(ctx, base);
}

extern "C" void sub_829F6080(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_inputs = ctx.r4.u32;
  const uint32_t input_count = ctx.r5.u32;
  const uint32_t guest_callback = ctx.r6.u32;
  const mp64::SessionParticipantState participant_state = g_session_participants.Get(guest_session);
  if (participant_state.guest_record_table == 0 || input_count == 0 ||
      input_count > mp64::kExtendedPeerCapacity) {
    __imp__sub_829F6080(ctx, base);
    return;
  }
  const auto state_address = mp64::CheckedGuestAddress(guest_session, mp64::kSessionStateOffset);
  const auto pending_address =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPendingCommandOffset);
  if (!state_address || !pending_address || REX_LOAD_U32(*state_address) < 2 ||
      REX_LOAD_U32(*state_address) > 3 || REX_LOAD_U32(*pending_address) != 0) {
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t guest_records =
      runtime->memory()->SystemHeapAlloc(mp64::kExtendedSessionParticipantTableSize);
  if (guest_records == 0) {
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  for (uint32_t index = 0; index < mp64::kExtendedPeerCapacity; ++index) {
    const auto record =
        mp64::CheckedGuestArrayAddress(guest_records, index, mp64::kSessionParticipantRecordSize);
    if (record) {
      InitializeSessionParticipantRecord(base, *record);
    }
  }

  uint32_t public_count = 0;
  uint32_t private_count = 0;
  {
    std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
    for (uint32_t input_index = 0; input_index < input_count; ++input_index) {
      const auto input_address =
          mp64::CheckedGuestArrayAddress(guest_inputs, input_index, mp64::kGuestPointerSize);
      if (!input_address) {
        continue;
      }
      const uint32_t guest_participant = REX_LOAD_U32(*input_address);
      PPCContext identity_ctx = ctx;
      identity_ctx.r3.u64 = guest_participant;
      __imp__sub_829DBAA8(identity_ctx, base);
      const int32_t participant_index =
          FindSessionParticipant(ctx, base, participant_state, identity_ctx.r3.u32);
      if (participant_index < 0) {
        continue;
      }
      PPCContext privacy_ctx = ctx;
      privacy_ctx.r3.u64 = guest_participant;
      __imp__sub_829DB9C0(privacy_ctx, base);
      const bool is_private = privacy_ctx.r3.u8 == 0;
      const uint32_t output_index =
          is_private ? mp64::kExtendedPeerCapacity - 1 - private_count : public_count;
      const auto output_record = mp64::CheckedGuestArrayAddress(
          guest_records, output_index, mp64::kSessionParticipantRecordSize);
      const auto source_record = SessionParticipantRecordAddress(
          participant_state, static_cast<uint32_t>(participant_index));
      if (!output_record || !source_record || output_index < public_count) {
        continue;
      }
      std::memcpy(base + *output_record, base + *source_record, 8);
      PPCContext payload_ctx = ctx;
      payload_ctx.r3.u64 = guest_participant;
      __imp__sub_829DBAB0(payload_ctx, base);
      const auto payload_destination = mp64::CheckedGuestAddress(*output_record, 8);
      if (!payload_destination || payload_ctx.r3.u32 == 0) {
        continue;
      }
      std::memcpy(base + *payload_destination, base + payload_ctx.r3.u32, 16);
      PPCContext value_ctx = ctx;
      value_ctx.r3.u64 = guest_participant;
      __imp__sub_829DB9B8(value_ctx, base);
      const auto value_destination = mp64::CheckedGuestAddress(*output_record, 24);
      if (!value_destination) {
        continue;
      }
      REX_STORE_U32(*value_destination, value_ctx.r3.u32);
      if (is_private) {
        ++private_count;
      } else {
        ++public_count;
      }
    }
  }
  if (private_count != 0) {
    const auto private_source =
        mp64::CheckedGuestArrayAddress(guest_records, mp64::kExtendedPeerCapacity - private_count,
                                       mp64::kSessionParticipantRecordSize);
    const auto private_destination = mp64::CheckedGuestArrayAddress(
        guest_records, public_count, mp64::kSessionParticipantRecordSize);
    if (!private_source || !private_destination) {
      runtime->memory()->SystemHeapFree(guest_records);
      FailSessionCommandCallback(ctx, base, guest_callback);
      ctx.r3.u64 = 0;
      return;
    }
    std::memmove(base + *private_destination, base + *private_source,
                 static_cast<size_t>(private_count) * mp64::kSessionParticipantRecordSize);
  }
  if (public_count == 0 && private_count == 0) {
    runtime->memory()->SystemHeapFree(guest_records);
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }

  const uint32_t guest_command =
      AllocateSessionCommand(ctx, base, mp64::kLeaveCommandAllocationSize);
  if (guest_command == 0) {
    runtime->memory()->SystemHeapFree(guest_records);
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  PPCContext construct_ctx = ctx;
  construct_ctx.r3.u64 = guest_command;
  construct_ctx.r4.u64 = guest_callback;
  __imp__sub_829F5920(construct_ctx, base);
  REX_STORE_U32(guest_command + 20, public_count);
  REX_STORE_U32(guest_command + 24, private_count);
  const auto private_begin = mp64::CheckedGuestArrayAddress(guest_records, public_count,
                                                            mp64::kSessionParticipantRecordSize);
  if (!private_begin ||
      !g_participant_commands.Set(guest_command, {.guest_record_table = guest_records,
                                                  .public_count = public_count,
                                                  .private_count = private_count})) {
    DestroySessionCommand(ctx, base, guest_command);
    runtime->memory()->SystemHeapFree(guest_records);
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  REX_STORE_U32(guest_command + 1056, *private_begin);
  PPCContext queue_ctx = ctx;
  queue_ctx.r3.u64 = guest_session;
  queue_ctx.r4.u64 = guest_command;
  sub_829F52E8(queue_ctx, base);
  ctx.r3.u64 = 1;
}

extern "C" void sub_829F6AA8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t public_slots = ctx.r4.u32;
  const uint32_t private_slots = ctx.r5.u32;
  const mp64::CapacityDecision decision = mp64::ClassifySlotRequest(public_slots, private_slots);
  if (decision.path == mp64::CapacityPath::kLegacy) {
    __imp__sub_829F6AA8(ctx, base);
    return;
  }

  // Preserve the original error callback and result ABI for overflow or >64
  // by feeding it the first value its own validator safely rejects.
  if (decision.path == mp64::CapacityPath::kInvalid || guest_session == 0 ||
      g_extended_slot_command.active) {
    ctx.r4.u64 = mp64::kFirstUnsupportedLegacyParticipantCount;
    ctx.r5.u64 = 0;
    __imp__sub_829F6AA8(ctx, base);
    return;
  }

  const auto proxy = mp64::MakeLegacySlotProxy(public_slots, private_slots);
  const auto participant_count_address =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionParticipantCountOffset);
  const auto public_slots_address =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPublicSlotsOffset);
  const auto private_slots_address =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPrivateSlotsOffset);
  const mp64::SessionParticipantState participant_state = g_session_participants.Get(guest_session);
  const uint32_t actual_participant_count =
      participant_state.guest_record_table != 0 ? participant_state.count
      : participant_count_address               ? REX_LOAD_U32(*participant_count_address)
                                                : 0;
  if (!proxy.valid || !participant_count_address || !public_slots_address ||
      !private_slots_address || decision.total < actual_participant_count) {
    ctx.r4.u64 = mp64::kFirstUnsupportedLegacyParticipantCount;
    ctx.r5.u64 = 0;
    __imp__sub_829F6AA8(ctx, base);
    return;
  }

  std::scoped_lock extended_slot_lock(g_extended_slot_command_mutex);
  const uint32_t saved_participant_count = REX_LOAD_U32(*participant_count_address);
  if (saved_participant_count > mp64::kLegacyCommandParticipantCapacity) {
    REX_STORE_U32(*participant_count_address, mp64::kLegacyCommandParticipantCapacity);
  }
  g_extended_slot_command = {
      .active = true,
      .command_patched = false,
      .guest_session = guest_session,
      .public_slots = public_slots,
      .private_slots = private_slots,
  };
  ctx.r4.u64 = proxy.public_slots;
  ctx.r5.u64 = proxy.private_slots;
  __imp__sub_829F6AA8(ctx, base);
  const bool command_patched = g_extended_slot_command.command_patched;
  g_extended_slot_command = {};
  REX_STORE_U32(*participant_count_address, saved_participant_count);

  if (ctx.r3.u32 != 0 && command_patched) {
    REX_STORE_U32(*public_slots_address, public_slots);
    REX_STORE_U32(*private_slots_address, private_slots);
    return;
  }
  if (ctx.r3.u32 != 0) {
    REXLOG_ERROR(
        "gta4-multiplayer64: SetMaxSlots queued without extended command patch; "
        "session={:08X}",
        guest_session);
  }
}

// The retail SetMaxSlots task stores its desired counts at +36/+40. Expanding
// the verified Free Mode value here keeps the task's persistent desired state
// and the queued session command consistent; other modes retain retail caps.
extern "C" void sub_82802150(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_task = ctx.r3.u32;
  if (guest_task != 0 && REX_LOAD_U8(mp64::kNetworkPreferencesReadyAddress) != 0 &&
      REX_LOAD_U32(mp64::kNetworkPreferencesAddress) == mp64::kFreeRoamGameMode) {
    const auto public_address = mp64::CheckedGuestAddress(
        guest_task, mp64::kSetMaxSlotsTaskPublicOffset);
    const auto private_address = mp64::CheckedGuestAddress(
        guest_task, mp64::kSetMaxSlotsTaskPrivateOffset);
    if (public_address && private_address) {
      const auto request = mp64::ExpandFreeRoamSlotRequest(
          mp64::kFreeRoamGameMode, REX_LOAD_U32(*public_address),
          REX_LOAD_U32(*private_address));
      if (request.expanded) {
        REX_STORE_U32(*public_address, request.public_slots);
        REX_STORE_U32(*private_address, request.private_slots);
        REXLOG_INFO(
            "gta4-multiplayer64: Free Mode session capacity expanded to {} public + {} private",
            request.public_slots, request.private_slots);
      }
    }
  }
  __imp__sub_82802150(ctx, base);
}

extern "C" void sub_829F8710(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_participants = ctx.r4.u32;
  const uint32_t guest_flags = ctx.r5.u32;
  const uint32_t input_count = ctx.r6.u32;
  const uint32_t guest_callback = ctx.r7.u32;
  const mp64::SessionParticipantState participant_state = g_session_participants.Get(guest_session);
  if (participant_state.guest_record_table == 0 || input_count == 0 ||
      input_count > mp64::kExtendedPeerCapacity) {
    __imp__sub_829F8710(ctx, base);
    return;
  }
  const auto state_address = mp64::CheckedGuestAddress(guest_session, mp64::kSessionStateOffset);
  const auto pending_address =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPendingCommandOffset);
  const auto public_slots =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPublicSlotsOffset);
  const auto private_slots =
      mp64::CheckedGuestAddress(guest_session, mp64::kSessionPrivateSlotsOffset);
  if (!state_address || !pending_address || !public_slots || !private_slots ||
      REX_LOAD_U32(*state_address) < 2 || REX_LOAD_U32(*state_address) > 3 ||
      REX_LOAD_U32(*pending_address) != 0) {
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t guest_records =
      runtime->memory()->SystemHeapAlloc(mp64::kExtendedJoinCommandTableSize);
  if (guest_records == 0) {
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  std::memset(base + guest_records, 0, mp64::kExtendedJoinCommandTableSize);
  uint32_t public_count = 0;
  uint32_t private_count = 0;
  uint32_t requested_private_flags = 0;
  {
    std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
    for (uint32_t index = 0; index < input_count; ++index) {
      const auto participant_address =
          mp64::CheckedGuestArrayAddress(guest_participants, index, mp64::kGuestPointerSize);
      const auto flag_address =
          mp64::CheckedGuestArrayAddress(guest_flags, index, mp64::kGuestPointerSize);
      if (!participant_address || !flag_address) {
        continue;
      }
      const uint32_t guest_participant = REX_LOAD_U32(*participant_address);
      const bool requested_private = REX_LOAD_U32(*flag_address) == 1;
      if (requested_private) {
        ++requested_private_flags;
      }
      PPCContext privacy_ctx = ctx;
      privacy_ctx.r3.u64 = guest_participant;
      __imp__sub_829DB9C0(privacy_ctx, base);
      const bool public_group = privacy_ctx.r3.u8 != 0;
      PPCContext identity_ctx = ctx;
      identity_ctx.r3.u64 = guest_participant;
      __imp__sub_829DBAA8(identity_ctx, base);
      if (FindSessionParticipant(ctx, base, participant_state, identity_ctx.r3.u32) >= 0) {
        continue;
      }
      if (public_group) {
        PPCContext validate_ctx = ctx;
        validate_ctx.r3.u64 = guest_participant;
        __imp__sub_822BF360(validate_ctx, base);
        if (validate_ctx.r3.u8 == 0) {
          continue;
        }
        validate_ctx = ctx;
        validate_ctx.r3.u64 = guest_session;
        __imp__sub_829F5050(validate_ctx, base);
        if (validate_ctx.r3.u8 != 0) {
          validate_ctx = ctx;
          validate_ctx.r3.u64 = guest_session;
          __imp__sub_829F5000(validate_ctx, base);
          if (validate_ctx.r3.u32 == 0) {
            PPCContext value_ctx = ctx;
            value_ctx.r3.u64 = guest_participant;
            __imp__sub_829DB9B8(value_ctx, base);
            validate_ctx = ctx;
            validate_ctx.r3.u64 = value_ctx.r3.u32;
            __imp__sub_829DB888(validate_ctx, base);
            if (validate_ctx.r3.u8 == 0) {
              continue;
            }
          }
        }
      }
      const uint32_t output_index =
          public_group ? public_count : mp64::kExtendedPeerCapacity - 1 - private_count;
      if (output_index < public_count) {
        continue;
      }
      const auto output =
          mp64::CheckedGuestArrayAddress(guest_records, output_index, mp64::kJoinCommandRecordSize);
      if (!output) {
        continue;
      }
      REX_STORE_U32(*output, guest_participant);
      REX_STORE_U8(*output + 4, requested_private ? 1 : 0);
      if (public_group) {
        ++public_count;
      } else {
        ++private_count;
      }
    }
  }
  const uint32_t public_slots_value = REX_LOAD_U32(*public_slots);
  const uint32_t private_slots_value = REX_LOAD_U32(*private_slots);
  const bool has_public_capacity =
      participant_state.public_count <= public_slots_value &&
      input_count - requested_private_flags <= public_slots_value - participant_state.public_count;
  const bool has_private_capacity =
      participant_state.private_count <= private_slots_value &&
      requested_private_flags <= private_slots_value - participant_state.private_count;
  if (!has_public_capacity || !has_private_capacity || (public_count == 0 && private_count == 0)) {
    runtime->memory()->SystemHeapFree(guest_records);
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  if (private_count != 0) {
    const auto source = mp64::CheckedGuestArrayAddress(
        guest_records, mp64::kExtendedPeerCapacity - private_count, mp64::kJoinCommandRecordSize);
    const auto destination =
        mp64::CheckedGuestArrayAddress(guest_records, public_count, mp64::kJoinCommandRecordSize);
    if (!source || !destination) {
      runtime->memory()->SystemHeapFree(guest_records);
      FailSessionCommandCallback(ctx, base, guest_callback);
      ctx.r3.u64 = 0;
      return;
    }
    std::memmove(base + *destination, base + *source,
                 static_cast<size_t>(private_count) * mp64::kJoinCommandRecordSize);
  }
  const uint32_t guest_command =
      AllocateSessionCommand(ctx, base, mp64::kJoinCommandAllocationSize);
  if (guest_command == 0) {
    runtime->memory()->SystemHeapFree(guest_records);
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  REX_STORE_U32(guest_command, mp64::kJoinCommandVtableAddress);
  REX_STORE_U32(guest_command + 4, 5);
  REX_STORE_U32(guest_command + 8, guest_callback != 0 ? guest_callback : guest_command + 12);
  REX_STORE_U32(guest_command + 12, 0);
  REX_STORE_U32(guest_command + 16, 0);
  REX_STORE_U32(guest_command + 20, public_count);
  REX_STORE_U32(guest_command + 24, private_count);
  REX_STORE_U32(guest_command + 284, guest_records + public_count * mp64::kJoinCommandRecordSize);
  if (!g_participant_commands.Set(guest_command, {.guest_record_table = guest_records,
                                                  .public_count = public_count,
                                                  .private_count = private_count})) {
    DestroySessionCommand(ctx, base, guest_command);
    runtime->memory()->SystemHeapFree(guest_records);
    FailSessionCommandCallback(ctx, base, guest_callback);
    ctx.r3.u64 = 0;
    return;
  }
  PPCContext queue_ctx = ctx;
  queue_ctx.r3.u64 = guest_session;
  queue_ctx.r4.u64 = guest_command;
  sub_829F52E8(queue_ctx, base);
  ctx.r3.u64 = 1;
}

extern "C" void sub_829F7208(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_command = ctx.r4.u32;
  const mp64::ParticipantCommandState state = g_participant_commands.Get(guest_command);
  if (state.guest_record_table == 0) {
    __imp__sub_829F7208(ctx, base);
    return;
  }
  std::vector<uint32_t> public_records;
  std::vector<uint32_t> private_records;
  for (uint32_t index = 0; index < state.public_count; ++index) {
    const auto record = mp64::CheckedGuestArrayAddress(state.guest_record_table, index,
                                                       mp64::kJoinCommandRecordSize);
    if (record) {
      public_records.push_back(*record);
    }
  }
  for (uint32_t index = 0; index < state.private_count; ++index) {
    const auto record = mp64::CheckedGuestArrayAddress(
        state.guest_record_table, state.public_count + index, mp64::kJoinCommandRecordSize);
    if (record) {
      private_records.push_back(*record);
    }
  }
  size_t public_start = 0;
  size_t private_start = 0;
  bool success = true;
  while (public_start < public_records.size() || private_start < private_records.size()) {
    const size_t public_batch = std::min<size_t>(mp64::kLegacyCommandParticipantCapacity,
                                                 public_records.size() - public_start);
    const size_t private_batch =
        std::min<size_t>(mp64::kLegacyCommandParticipantCapacity - public_batch,
                         private_records.size() - private_start);
    if (!RunJoinPlatformBatch(ctx, base, guest_session, public_records, private_records,
                              public_start, private_start)) {
      success = false;
      break;
    }
    public_start += public_batch;
    private_start += private_batch;
  }
  ctx.r3.u64 = success ? 1 : 0;
}

extern "C" void sub_829F8F40(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_session = ctx.r3.u32;
  const uint32_t guest_command = ctx.r4.u32;
  mp64::ParticipantCommandState command_state = g_participant_commands.Get(guest_command);
  const mp64::SessionParticipantState participant_state = g_session_participants.Get(guest_session);
  if (command_state.guest_record_table == 0 || participant_state.guest_record_table == 0) {
    __imp__sub_829F8F40(ctx, base);
    return;
  }

  std::vector<std::array<uint8_t, mp64::kJoinCommandRecordSize>> public_records;
  std::vector<std::array<uint8_t, mp64::kJoinCommandRecordSize>> private_records;
  {
    std::scoped_lock mutation_lock(g_session_participant_mutation_mutex);
    for (uint32_t index = 0; index < command_state.public_count; ++index) {
      const auto record = mp64::CheckedGuestArrayAddress(command_state.guest_record_table, index,
                                                         mp64::kJoinCommandRecordSize);
      if (!record) {
        continue;
      }
      PPCContext identity_ctx = ctx;
      identity_ctx.r3.u64 = REX_LOAD_U32(*record);
      __imp__sub_829DBAA8(identity_ctx, base);
      if (FindSessionParticipant(ctx, base, participant_state, identity_ctx.r3.u32) >= 0) {
        continue;
      }
      public_records.emplace_back();
      std::memcpy(public_records.back().data(), base + *record, mp64::kJoinCommandRecordSize);
    }
    for (uint32_t index = 0; index < command_state.private_count; ++index) {
      const auto record = mp64::CheckedGuestArrayAddress(command_state.guest_record_table,
                                                         command_state.public_count + index,
                                                         mp64::kJoinCommandRecordSize);
      if (!record) {
        continue;
      }
      PPCContext identity_ctx = ctx;
      identity_ctx.r3.u64 = REX_LOAD_U32(*record);
      __imp__sub_829DBAA8(identity_ctx, base);
      if (FindSessionParticipant(ctx, base, participant_state, identity_ctx.r3.u32) >= 0) {
        continue;
      }
      private_records.emplace_back();
      std::memcpy(private_records.back().data(), base + *record, mp64::kJoinCommandRecordSize);
    }
  }
  uint32_t output_index = 0;
  for (const auto& record : public_records) {
    const auto destination = mp64::CheckedGuestArrayAddress(
        command_state.guest_record_table, output_index++, mp64::kJoinCommandRecordSize);
    if (destination) {
      std::memcpy(base + *destination, record.data(), record.size());
    }
  }
  for (const auto& record : private_records) {
    const auto destination = mp64::CheckedGuestArrayAddress(
        command_state.guest_record_table, output_index++, mp64::kJoinCommandRecordSize);
    if (destination) {
      std::memcpy(base + *destination, record.data(), record.size());
    }
  }
  command_state.public_count = static_cast<uint32_t>(public_records.size());
  command_state.private_count = static_cast<uint32_t>(private_records.size());
  g_participant_commands.Set(guest_command, command_state);
  REX_STORE_U32(guest_command + 20, command_state.public_count);
  REX_STORE_U32(guest_command + 24, command_state.private_count);
  REX_STORE_U32(guest_command + 284, command_state.guest_record_table +
                                         command_state.public_count * mp64::kJoinCommandRecordSize);

  PPCContext platform_ctx = ctx;
  platform_ctx.r3.u64 = guest_session;
  platform_ctx.r4.u64 = guest_command;
  sub_829F7208(platform_ctx, base);
  const bool success = platform_ctx.r3.u8 != 0;
  if (success) {
    for (uint32_t index = 0; index < command_state.public_count + command_state.private_count;
         ++index) {
      const auto record = mp64::CheckedGuestArrayAddress(command_state.guest_record_table, index,
                                                         mp64::kJoinCommandRecordSize);
      if (!record) {
        continue;
      }
      PPCContext add_ctx = ctx;
      add_ctx.r3.u64 = guest_session;
      add_ctx.r4.u64 = REX_LOAD_U32(*record);
      add_ctx.r5.u64 = REX_LOAD_U8(*record + 4) != 0;
      sub_829F5628(add_ctx, base);
    }
  }
  ReleaseParticipantCommandSidecar(guest_command);
  ctx.r3.u64 = success ? 1 : 0;
}

extern "C" void sub_82801730(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_task = ctx.r3.u32;
  const uint32_t guest_owner = ctx.r4.u32;
  if (!g_migration_snapshot_override.active ||
      g_migration_snapshot_override.guest_owner != guest_owner) {
    __imp__sub_82801730(ctx, base);
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t guest_records =
      runtime->memory()->SystemHeapAlloc(mp64::kExtendedMigrationRecordTableSize);
  if (guest_records == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  std::memcpy(
      base + guest_records, base + g_migration_snapshot_override.guest_records,
      static_cast<size_t>(g_migration_snapshot_override.count) * mp64::kMigrationRecordSize);
  uint32_t count = g_migration_snapshot_override.count;
  PPCContext local_ctx = ctx;
  local_ctx.r3.u64 = guest_owner;
  __imp__sub_827C9998(local_ctx, base);
  const uint32_t guest_local_record = local_ctx.r3.u32;
  bool contains_local = false;
  if (guest_local_record != 0) {
    PPCContext local_generation_ctx = ctx;
    local_generation_ctx.r3.u64 = guest_local_record;
    __imp__sub_829ED378(local_generation_ctx, base);
    for (uint32_t index = 0; index < count; ++index) {
      const auto record =
          mp64::CheckedGuestArrayAddress(guest_records, index, mp64::kMigrationRecordSize);
      if (!record) {
        continue;
      }
      PPCContext generation_ctx = ctx;
      generation_ctx.r3.u64 = *record;
      __imp__sub_829ED378(generation_ctx, base);
      if (generation_ctx.r3.u32 == local_generation_ctx.r3.u32) {
        contains_local = true;
        break;
      }
    }
    if (!contains_local && count < mp64::kExtendedPeerCapacity) {
      const auto destination =
          mp64::CheckedGuestArrayAddress(guest_records, count++, mp64::kMigrationRecordSize);
      if (destination) {
        std::memcpy(base + *destination, base + guest_local_record, mp64::kMigrationRecordSize);
      }
    }
  }
  ctx.r5.u64 = guest_records;
  ctx.r6.u64 = std::min<uint32_t>(count, mp64::kLegacyCommandParticipantCapacity - 1);
  __imp__sub_82801730(ctx, base);
  if (ctx.r3.u8 == 0 ||
      !g_migration_tasks.Set(
          guest_task, {.guest_record_table = guest_records, .count = count, .current = -1})) {
    runtime->memory()->SystemHeapFree(guest_records);
    return;
  }
  PrepareMigrationTaskAlias(base, guest_task, g_migration_tasks.Get(guest_task));
}

extern "C" void sub_828019F8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_task = ctx.r3.u32;
  mp64::MigrationTaskState state = g_migration_tasks.Get(guest_task);
  if (state.guest_record_table == 0 ||
      state.current < static_cast<int32_t>(mp64::kLegacyCommandParticipantCapacity - 1)) {
    __imp__sub_828019F8(ctx, base);
    if (state.guest_record_table != 0) {
      const int32_t current =
          static_cast<int32_t>(REX_LOAD_U32(guest_task + mp64::kMigrationTaskCurrentOffset));
      g_migration_tasks.SetCurrent(guest_task, current);
    }
    return;
  }
  const auto inline_table =
      mp64::CheckedGuestAddress(guest_task, mp64::kMigrationTaskInlineTableOffset);
  if (!inline_table) {
    ctx.r3.u64 = 0;
    return;
  }
  if (state.current + 1 < static_cast<int32_t>(state.count)) {
    const auto current_source = mp64::CheckedGuestArrayAddress(
        state.guest_record_table, static_cast<uint32_t>(state.current), mp64::kMigrationRecordSize);
    const auto next_source = mp64::CheckedGuestArrayAddress(
        state.guest_record_table, static_cast<uint32_t>(state.current + 1),
        mp64::kMigrationRecordSize);
    const auto current_destination = mp64::CheckedGuestArrayAddress(
        *inline_table, mp64::kLegacyCommandParticipantCapacity - 2, mp64::kMigrationRecordSize);
    const auto next_destination = mp64::CheckedGuestArrayAddress(
        *inline_table, mp64::kLegacyCommandParticipantCapacity - 1, mp64::kMigrationRecordSize);
    if (!current_source || !next_source || !current_destination || !next_destination) {
      ctx.r3.u64 = 0;
      return;
    }
    std::memcpy(base + *current_destination, base + *current_source, mp64::kMigrationRecordSize);
    std::memcpy(base + *next_destination, base + *next_source, mp64::kMigrationRecordSize);
    REX_STORE_U32(guest_task + mp64::kMigrationTaskCurrentOffset,
                  mp64::kLegacyCommandParticipantCapacity - 2);
    REX_STORE_U32(guest_task + mp64::kMigrationTaskCountOffset,
                  mp64::kLegacyCommandParticipantCapacity);
    __imp__sub_828019F8(ctx, base);
    ++state.current;
    g_migration_tasks.SetCurrent(guest_task, state.current);
    PrepareMigrationTaskAlias(base, guest_task, g_migration_tasks.Get(guest_task));
    return;
  }
  const auto current_source = mp64::CheckedGuestArrayAddress(
      state.guest_record_table, static_cast<uint32_t>(state.current), mp64::kMigrationRecordSize);
  const auto current_destination = mp64::CheckedGuestArrayAddress(
      *inline_table, mp64::kLegacyCommandParticipantCapacity - 1, mp64::kMigrationRecordSize);
  if (!current_source || !current_destination) {
    ctx.r3.u64 = 0;
    return;
  }
  std::memcpy(base + *current_destination, base + *current_source, mp64::kMigrationRecordSize);
  REX_STORE_U32(guest_task + mp64::kMigrationTaskCurrentOffset,
                mp64::kLegacyCommandParticipantCapacity - 1);
  REX_STORE_U32(guest_task + mp64::kMigrationTaskCountOffset,
                mp64::kLegacyCommandParticipantCapacity);
  __imp__sub_828019F8(ctx, base);
  g_migration_tasks.SetCurrent(guest_task, static_cast<int32_t>(state.count));
  PrepareMigrationTaskAlias(base, guest_task, g_migration_tasks.Get(guest_task));
}

// This completion/event path derives a record address directly from
// task->current before it calls any of the task helpers. Refresh the rolling
// alias here as well so current values 31..63 never reach the inline-table
// expression.
extern "C" void sub_828076C8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_task = ctx.r3.u32;
  const mp64::MigrationTaskState state = g_migration_tasks.Get(guest_task);
  if (state.guest_record_table != 0) {
    PrepareMigrationTaskAlias(base, guest_task, state);
  }
  __imp__sub_828076C8(ctx, base);
}

extern "C" void sub_82808720(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_task = ctx.r3.u32;
  const mp64::MigrationTaskState state = g_migration_tasks.Get(guest_task);
  if (state.guest_record_table != 0) {
    PrepareMigrationTaskAlias(base, guest_task, state);
  }
  __imp__sub_82808720(ctx, base);
}

extern "C" void sub_82807308(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_task = ctx.r3.u32;
  const mp64::MigrationTaskState state = g_migration_tasks.Get(guest_task);
  if (state.guest_record_table != 0) {
    PrepareMigrationTaskAlias(base, guest_task, state);
  }
  __imp__sub_82807308(ctx, base);
  ReleaseMigrationTaskSidecar(guest_task);
}

// snMigrateSessionTask's deleting destructor is the unconditional lifetime
// endpoint. vfunc[4] normally releases the relocated table first, while this
// hook covers cancellation, construction rollback and owner teardown paths.
extern "C" void sub_828086B8(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_task = ctx.r3.u32;
  ReleaseMigrationTaskSidecar(guest_task);
  ctx.r3.u64 = guest_task;
  __imp__sub_828086B8(ctx, base);
}

extern "C" void sub_827CD7B0(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_owner = ctx.r3.u32;
  const uint32_t guest_input = ctx.r4.u32;
  const int32_t input_count = ctx.r5.s32;
  if (g_migration_snapshot_override.active &&
      g_migration_snapshot_override.guest_owner == guest_owner) {
    ctx.r4.u64 = guest_owner + 4152;
    ctx.r5.u64 = 1;
    __imp__sub_827CD7B0(ctx, base);
    return;
  }
  if (input_count <= static_cast<int32_t>(mp64::kLegacyCommandParticipantCapacity)) {
    __imp__sub_827CD7B0(ctx, base);
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t guest_records =
      runtime->memory()->SystemHeapAlloc(mp64::kExtendedMigrationRecordTableSize);
  if (guest_records == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  for (uint32_t index = 0; index < mp64::kExtendedPeerCapacity; ++index) {
    const auto record =
        mp64::CheckedGuestArrayAddress(guest_records, index, mp64::kMigrationRecordSize);
    if (record) {
      PPCContext initialize_ctx = ctx;
      initialize_ctx.r3.u64 = *record;
      __imp__sub_829ED580(initialize_ctx, base);
    }
  }
  uint32_t record_count = 0;
  for (int32_t index = 0; index < input_count && record_count < mp64::kExtendedPeerCapacity;
       ++index) {
    const auto source = mp64::CheckedGuestArrayAddress(guest_input, static_cast<size_t>(index),
                                                       mp64::kMigrationRecordSize);
    if (!source) {
      continue;
    }
    PPCContext generation_ctx = ctx;
    generation_ctx.r3.u64 = *source;
    __imp__sub_829ED378(generation_ctx, base);
    PPCContext lookup_ctx = ctx;
    lookup_ctx.r3.u64 = guest_owner;
    lookup_ctx.r4.u64 = generation_ctx.r3.u32;
    __imp__sub_827C9EF8(lookup_ctx, base);
    if (lookup_ctx.r3.u32 == 0 || (static_cast<int32_t>(REX_LOAD_U32(lookup_ctx.r3.u32)) < 0 &&
                                   (REX_LOAD_U8(lookup_ctx.r3.u32 + 92) & 0x80) == 0)) {
      continue;
    }
    const auto destination =
        mp64::CheckedGuestArrayAddress(guest_records, record_count++, mp64::kMigrationRecordSize);
    if (destination) {
      std::memcpy(base + *destination, base + *source, mp64::kMigrationRecordSize);
    }
  }
  if (record_count == 0) {
    std::memcpy(base + guest_records, base + guest_owner + 4152, mp64::kMigrationRecordSize);
    record_count = 1;
  }
  g_migration_snapshot_override = {
      .active = true,
      .guest_owner = guest_owner,
      .guest_records = guest_records,
      .count = record_count,
  };
  ctx.r4.u64 = guest_owner + 4152;
  ctx.r5.u64 = 1;
  __imp__sub_827CD7B0(ctx, base);
  g_migration_snapshot_override = {};
  runtime->memory()->SystemHeapFree(guest_records);
}

extern "C" void sub_827CFD20(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_owner = ctx.r3.u32;
  if (!g_migration_snapshot_override.active ||
      g_migration_snapshot_override.guest_owner != guest_owner) {
    __imp__sub_827CFD20(ctx, base);
    return;
  }
  const uint32_t guest_output = g_migration_snapshot_override.guest_records;
  std::memcpy(base + guest_output, base + guest_owner + 4152, mp64::kMigrationRecordSize);
  uint32_t count = 1;
  const uint32_t participant_count = REX_LOAD_U32(guest_owner + 11268);
  for (uint32_t index = 0; index < participant_count && count < mp64::kExtendedPeerCapacity;
       ++index) {
    const auto pointer_address =
        mp64::CheckedGuestArrayAddress(guest_owner + 11140, index, mp64::kGuestPointerSize);
    if (!pointer_address) {
      continue;
    }
    const uint32_t guest_participant = REX_LOAD_U32(*pointer_address);
    if (guest_participant == 0 || static_cast<int32_t>(REX_LOAD_U32(guest_participant)) < 0) {
      continue;
    }
    const auto destination =
        mp64::CheckedGuestArrayAddress(guest_output, count++, mp64::kMigrationRecordSize);
    if (destination) {
      std::memcpy(base + *destination, base + guest_participant + 8, mp64::kMigrationRecordSize);
    }
  }
  if (count > 1) {
    const auto end =
        mp64::CheckedGuestArrayAddress(guest_output, count, mp64::kMigrationRecordSize);
    if (end) {
      PPCContext sort_ctx = ctx;
      sort_ctx.r3.u64 = guest_output;
      sort_ctx.r4.u64 = *end;
      sort_ctx.r5.u64 = 0;
      __imp__sub_827CFC98(sort_ctx, base);
    }
  }
  g_migration_snapshot_override.count = count;
  ctx.r3.u64 = count;
}

extern "C" void sub_827CFE18(PPCContext& ctx, uint8_t* base) {
  const uint32_t guest_owner = ctx.r3.u32;
  if (g_migration_snapshot_override.active) {
    __imp__sub_827CFE18(ctx, base);
    return;
  }
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    __imp__sub_827CFE18(ctx, base);
    return;
  }
  const uint32_t guest_records =
      runtime->memory()->SystemHeapAlloc(mp64::kExtendedMigrationRecordTableSize);
  if (guest_records == 0) {
    __imp__sub_827CFE18(ctx, base);
    return;
  }
  g_migration_snapshot_override = {
      .active = true,
      .guest_owner = guest_owner,
      .guest_records = guest_records,
      .count = 0,
  };
  __imp__sub_827CFE18(ctx, base);
  g_migration_snapshot_override = {};
  runtime->memory()->SystemHeapFree(guest_records);
}

extern "C" void sub_827D0D00(PPCContext& ctx, uint8_t* base) {
  const int32_t input_count = ctx.r5.s32;
  if (input_count <= static_cast<int32_t>(mp64::kLegacyCommandParticipantCapacity)) {
    __imp__sub_827D0D00(ctx, base);
    return;
  }
  const uint32_t guest_owner = ctx.r3.u32;
  const uint32_t guest_inputs = ctx.r4.u32;
  const uint32_t argument = ctx.r6.u32;
  const uint32_t guest_callback = ctx.r7.u32;
  rex::Runtime* runtime = rex::Runtime::instance();
  if (runtime == nullptr) {
    CompleteSnapshotCallback(ctx, base, guest_callback, false);
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t guest_snapshot = runtime->memory()->SystemHeapAlloc(512);
  if (guest_snapshot == 0) {
    CompleteSnapshotCallback(ctx, base, guest_callback, false);
    ctx.r3.u64 = 0;
    return;
  }
  std::memset(base + guest_snapshot, 0xFF, 512);
  PPCContext ready_ctx = ctx;
  ready_ctx.r3.u64 = guest_owner + 2264;
  __imp__sub_829F5110(ready_ctx, base);
  PPCContext generation_ctx = ctx;
  generation_ctx.r3.u64 = guest_owner + 4152;
  __imp__sub_829ED378(generation_ctx, base);
  if (ready_ctx.r3.u8 == 0 || generation_ctx.r3.u32 != REX_LOAD_U32(guest_owner + 4244)) {
    runtime->memory()->SystemHeapFree(guest_snapshot);
    CompleteSnapshotCallback(ctx, base, guest_callback, false);
    ctx.r3.u64 = 0;
    return;
  }
  uint32_t snapshot_count = 0;
  for (int32_t index = 0; index < input_count && snapshot_count < mp64::kExtendedPeerCapacity;
       ++index) {
    const auto input = mp64::CheckedGuestArrayAddress(guest_inputs, static_cast<size_t>(index), 8);
    if (!input) {
      continue;
    }
    PPCContext lookup_ctx = ctx;
    lookup_ctx.r3.u64 = guest_owner;
    lookup_ctx.r4.u64 = *input;
    __imp__sub_827C9E78(lookup_ctx, base);
    if (lookup_ctx.r3.u32 == 0) {
      continue;
    }
    PPCContext valid_ctx = ctx;
    valid_ctx.r3.u64 = lookup_ctx.r3.u32;
    __imp__sub_829DBA18(valid_ctx, base);
    if (valid_ctx.r3.u8 == 0) {
      continue;
    }
    const auto output = mp64::CheckedGuestArrayAddress(guest_snapshot, snapshot_count++, 8);
    if (output) {
      REX_STORE_U64(*output, REX_LOAD_U64(*input));
    }
  }
  if (snapshot_count == 0) {
    runtime->memory()->SystemHeapFree(guest_snapshot);
    CompleteSnapshotCallback(ctx, base, guest_callback, true);
    ctx.r3.u64 = 1;
    return;
  }
  PPCContext remove_ctx = ctx;
  remove_ctx.r3.u64 = guest_owner;
  remove_ctx.r4.u64 = guest_owner + 4144;
  remove_ctx.r5.u64 = guest_snapshot;
  remove_ctx.r6.u64 = snapshot_count;
  remove_ctx.r7.u64 = argument;
  remove_ctx.r8.u64 = 0;
  remove_ctx.r9.u64 = guest_callback;
  sub_827D04D8(remove_ctx, base);
  runtime->memory()->SystemHeapFree(guest_snapshot);
  ctx.r3.u64 = remove_ctx.r3.u64;
}

extern "C" void sub_827D04D8(PPCContext& ctx, uint8_t* base) {
  const int32_t record_count = ctx.r6.s32;
  if (record_count <= static_cast<int32_t>(mp64::kLegacyCommandParticipantCapacity)) {
    __imp__sub_827D04D8(ctx, base);
    return;
  }
  const uint32_t guest_owner = ctx.r3.u32;
  const uint32_t guest_context = ctx.r4.u32;
  const uint32_t guest_records = ctx.r5.u32;
  const uint32_t argument = ctx.r7.u32;
  const uint32_t existing_task = ctx.r8.u32;
  const uint32_t guest_callback = ctx.r9.u32;
  if (record_count <= 0 || record_count > static_cast<int32_t>(mp64::kExtendedPeerCapacity) ||
      existing_task != 0) {
    CompleteSnapshotCallback(ctx, base, guest_callback, false);
    ctx.r3.u64 = 0;
    return;
  }
  uint32_t processed = 0;
  bool success = true;
  while (processed < static_cast<uint32_t>(record_count)) {
    const uint32_t batch_count = std::min<uint32_t>(
        mp64::kLegacyCommandParticipantCapacity, static_cast<uint32_t>(record_count) - processed);
    const auto batch_records = mp64::CheckedGuestArrayAddress(guest_records, processed, 8);
    if (!batch_records) {
      success = false;
      break;
    }
    PPCContext batch_ctx = ctx;
    batch_ctx.r3.u64 = guest_owner;
    batch_ctx.r4.u64 = guest_context;
    batch_ctx.r5.u64 = *batch_records;
    batch_ctx.r6.u64 = batch_count;
    batch_ctx.r7.u64 = argument;
    batch_ctx.r8.u64 = 0;
    batch_ctx.r9.u64 =
        processed + batch_count == static_cast<uint32_t>(record_count) ? guest_callback : 0;
    __imp__sub_827D04D8(batch_ctx, base);
    if (batch_ctx.r3.u8 == 0) {
      success = false;
      break;
    }
    processed += batch_count;
  }
  if (!success && guest_callback != 0) {
    CompleteSnapshotCallback(ctx, base, guest_callback, false);
  }
  ctx.r3.u64 = success ? 1 : 0;
}

// GTA's script natives resolve a player ID to CPlayerInfo and read/write the
// authoritative team at +1384. Mirror that value into a host-side atomic
// cache so the UI thread can route Y/team chat without reading mutable guest
// memory concurrently with the PPC thread.
extern "C" void sub_825B4510(PPCContext& ctx, uint8_t* base) {
  __imp__sub_825B4510(ctx, base);
  const uint32_t guest_player_info = ctx.r3.u32;
  if (!guest_player_info) return;
  const uint8_t peer_id =
      REX_LOAD_U8(guest_player_info + mp64::kPlayerInfoPlayerIdOffset);
  g_peer_teams.Set(
      peer_id,
      static_cast<int32_t>(REX_LOAD_U32(guest_player_info + 1384)));
}

extern "C" void sub_825B4560(PPCContext& ctx, uint8_t* base) {
  __imp__sub_825B4560(ctx, base);
  const uint32_t guest_player_info = ctx.r3.u32;
  if (!guest_player_info) return;
  const uint8_t peer_id =
      REX_LOAD_U8(guest_player_info + mp64::kPlayerInfoPlayerIdOffset);
  g_peer_teams.Set(
      peer_id,
      static_cast<int32_t>(REX_LOAD_U32(guest_player_info + 1384)));
}
