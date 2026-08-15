/**
 ******************************************************************************
 * @file        xsession.cpp
 * @brief       Xbox 360 session object backed by LibertyRecomp directories.
 ******************************************************************************
 */

#include <rex/system/xam/xsession.h>

#include <algorithm>
#include <cstring>
#include <ranges>

#include <rex/logging.h>
#include <rex/memory.h>
#include <rex/system/kernel_state.h>
#include <rex/system/user_module.h>
#include <rex/system/xam/user_profile.h>

namespace rex::system::xam {
namespace {

constexpr uint32_t kSessionFlagHost = 0x00000001;
constexpr uint32_t kMemberFlagPrivate = 0x00000001;
constexpr uint32_t kNoUserIndex = 0xFFFFFFFF;
constexpr uint16_t kDefaultGamePort = 3074;
constexpr uint32_t kMaximumSearchResults = 64;
constexpr uint32_t kMaximumSearchContexts = 64;
constexpr uint32_t kMaximumSearchProperties = 64;
constexpr uint32_t kMaximumPropertySize = 512;
constexpr X_RESULT kErrorNotEnoughMemory = X_RESULT_FROM_WIN32(8);

struct TitleIdentity {
  uint32_t title_id = 0;
  uint32_t media_id = 0;
  uint32_t title_version = 0;
};

TitleIdentity GetTitleIdentity(KernelState* kernel_state) {
  TitleIdentity identity{.title_id = kernel_state->title_id()};
  auto module = kernel_state->GetExecutableModule();
  if (!module) {
    return identity;
  }
  xex2_opt_execution_info* execution_info = nullptr;
  if (XSUCCEEDED(module->GetOptHeader(XEX_HEADER_EXECUTION_INFO, &execution_info)) &&
      execution_info) {
    identity.media_id = execution_info->media_id;
    identity.title_version = execution_info->version_value;
  }
  return identity;
}

bool AddMemberToRecord(SessionRecord& record, SessionMember member) {
  if (std::ranges::find(record.members, member.xuid, &SessionMember::xuid) !=
      record.members.end()) {
    return true;
  }
  if (member.private_slot && record.open_private_slots) {
    --record.open_private_slots;
  } else if (record.open_public_slots) {
    member.private_slot = false;
    --record.open_public_slots;
  } else if (record.open_private_slots) {
    member.private_slot = true;
    --record.open_private_slots;
  } else {
    return false;
  }
  record.members.push_back(member);
  return true;
}

bool RemoveMemberFromRecord(SessionRecord& record, uint64_t xuid) {
  auto member = std::ranges::find(record.members, xuid, &SessionMember::xuid);
  if (member == record.members.end()) {
    return true;
  }
  if (member->private_slot) {
    record.open_private_slots = std::min(record.max_private_slots, record.open_private_slots + 1);
  } else {
    record.open_public_slots = std::min(record.max_public_slots, record.open_public_slots + 1);
  }
  record.members.erase(member);
  return true;
}

XUserDataType PropertyType(uint32_t id) {
  return static_cast<XUserDataType>((id >> 28) & 0x0F);
}

size_t ScalarPropertySize(XUserDataType type) {
  switch (type) {
    case XUserDataType::kContext:
    case XUserDataType::kInt32:
    case XUserDataType::kFloat:
      return sizeof(uint32_t);
    case XUserDataType::kInt64:
    case XUserDataType::kDouble:
    case XUserDataType::kDateTime:
      return sizeof(uint64_t);
    case XUserDataType::kWString:
    case XUserDataType::kBinary:
    case XUserDataType::kUnset:
      return 0;
  }
  return 0;
}

SessionProperty ReadProperty(KernelState* kernel_state, const XUSER_PROPERTY& property) {
  SessionProperty result;
  result.id = property.property_id;
  const auto type =
      property.data.type == XUserDataType::kUnset ? PropertyType(result.id) : property.data.type;
  if (type == XUserDataType::kWString || type == XUserDataType::kBinary) {
    const uint32_t size = property.data.value.binary.size;
    const uint32_t pointer = property.data.value.binary.pointer;
    if (size && pointer) {
      const auto* bytes = kernel_state->memory()->TranslateVirtual<const uint8_t*>(pointer);
      result.value.assign(bytes, bytes + size);
    }
  } else {
    const size_t size = ScalarPropertySize(type);
    const auto* bytes = reinterpret_cast<const uint8_t*>(&property.data.value);
    result.value.assign(bytes, bytes + size);
  }
  return result;
}

bool WriteProperty(KernelState* kernel_state, const SessionProperty& source,
                   XUSER_PROPERTY& destination, std::vector<uint32_t>& allocations) {
  std::memset(&destination, 0, sizeof(destination));
  destination.property_id = source.id;
  destination.data.type = PropertyType(source.id);
  if (destination.data.type == XUserDataType::kWString ||
      destination.data.type == XUserDataType::kBinary) {
    if (!source.value.empty()) {
      const uint32_t data_ptr =
          kernel_state->memory()->SystemHeapAlloc(static_cast<uint32_t>(source.value.size()));
      if (!data_ptr) {
        return false;
      }
      allocations.push_back(data_ptr);
      auto* data = kernel_state->memory()->TranslateVirtual<uint8_t*>(data_ptr);
      std::memcpy(data, source.value.data(), source.value.size());
      destination.data.value.binary.size = static_cast<uint32_t>(source.value.size());
      destination.data.value.binary.pointer = data_ptr;
    }
  } else if (!source.value.empty()) {
    std::memcpy(&destination.data.value, source.value.data(),
                std::min(source.value.size(), sizeof(destination.data.value)));
  }
  return true;
}

bool WriteSearchResult(KernelState* kernel_state, const SessionRecord& session,
                       XSESSION_SEARCHRESULT& result, std::vector<uint32_t>& allocations) {
  std::memset(&result, 0, sizeof(result));
  SessionRecordToGuestInfo(session, result.info);
  result.open_public_slots = session.open_public_slots;
  result.open_private_slots = session.open_private_slots;
  result.filled_public_slots = session.max_public_slots - session.open_public_slots;
  result.filled_private_slots = session.max_private_slots - session.open_private_slots;

  if (!session.contexts.empty()) {
    const uint32_t contexts_ptr = kernel_state->memory()->SystemHeapAlloc(
        static_cast<uint32_t>(session.contexts.size() * sizeof(XUSER_CONTEXT)));
    if (!contexts_ptr) {
      return false;
    }
    allocations.push_back(contexts_ptr);
    auto* contexts = kernel_state->memory()->TranslateVirtual<XUSER_CONTEXT*>(contexts_ptr);
    for (size_t index = 0; index < session.contexts.size(); ++index) {
      contexts[index].context_id = session.contexts[index].id;
      contexts[index].value = session.contexts[index].value;
    }
    result.contexts_count = static_cast<uint32_t>(session.contexts.size());
    result.contexts_ptr = contexts_ptr;
  }

  if (!session.properties.empty()) {
    const uint32_t properties_ptr = kernel_state->memory()->SystemHeapAlloc(
        static_cast<uint32_t>(session.properties.size() * sizeof(XUSER_PROPERTY)));
    if (!properties_ptr) {
      return false;
    }
    allocations.push_back(properties_ptr);
    auto* properties = kernel_state->memory()->TranslateVirtual<XUSER_PROPERTY*>(properties_ptr);
    for (size_t index = 0; index < session.properties.size(); ++index) {
      if (!WriteProperty(kernel_state, session.properties[index], properties[index], allocations)) {
        return false;
      }
    }
    result.properties_count = static_cast<uint32_t>(session.properties.size());
    result.properties_ptr = properties_ptr;
  }
  return true;
}

std::vector<SessionContext> ReadSearchContexts(KernelState* kernel_state,
                                               const XGI_SESSION_SEARCH& request) {
  std::vector<SessionContext> result;
  const uint32_t count = request.context_count;
  const uint32_t pointer = request.contexts_ptr;
  if (!count || !pointer) {
    return result;
  }
  const auto* contexts = kernel_state->memory()->TranslateVirtual<const XUSER_CONTEXT*>(pointer);
  result.reserve(count);
  for (uint32_t index = 0; index < count; ++index) {
    result.push_back({.id = contexts[index].context_id, .value = contexts[index].value});
  }
  return result;
}

std::vector<SessionProperty> ReadSearchProperties(KernelState* kernel_state,
                                                  const XGI_SESSION_SEARCH& request) {
  std::vector<SessionProperty> result;
  const uint32_t count = request.property_count;
  const uint32_t pointer = request.properties_ptr;
  if (!count || !pointer) {
    return result;
  }
  const auto* properties = kernel_state->memory()->TranslateVirtual<const XUSER_PROPERTY*>(pointer);
  result.reserve(count);
  for (uint32_t index = 0; index < count; ++index) {
    result.push_back(ReadProperty(kernel_state, properties[index]));
  }
  return result;
}

X_RESULT WriteSearchResults(KernelState* kernel_state, uint32_t results_ptr,
                            uint32_t results_buffer_size, std::span<const SessionRecord> sessions) {
  const size_t required_size =
      sizeof(XSESSION_SEARCHRESULT_HEADER) + sessions.size() * sizeof(XSESSION_SEARCHRESULT);
  if (!results_ptr || results_buffer_size < required_size) {
    return X_ERROR_INSUFFICIENT_BUFFER;
  }

  auto* output = kernel_state->memory()->TranslateVirtual<uint8_t*>(results_ptr);
  std::memset(output, 0, results_buffer_size);
  auto* header = reinterpret_cast<XSESSION_SEARCHRESULT_HEADER*>(output);
  auto* results = reinterpret_cast<XSESSION_SEARCHRESULT*>(output + sizeof(*header));
  header->search_results_count = static_cast<uint32_t>(sessions.size());
  header->search_results_ptr = results_ptr + static_cast<uint32_t>(sizeof(*header));
  std::vector<uint32_t> allocations;
  for (size_t index = 0; index < sessions.size(); ++index) {
    if (!WriteSearchResult(kernel_state, sessions[index], results[index], allocations)) {
      for (const uint32_t allocation : allocations) {
        kernel_state->memory()->SystemHeapFree(allocation);
      }
      std::memset(output, 0, results_buffer_size);
      return kErrorNotEnoughMemory;
    }
  }
  return X_ERROR_SUCCESS;
}

}  // namespace

uint64_t XnkidToUint64(const XNKID& id) {
  rex::be<uint64_t> value;
  std::memcpy(&value, id.value.data(), id.value.size());
  return value;
}

void Uint64ToXnkid(uint64_t value, XNKID& id) {
  rex::be<uint64_t> encoded = value;
  std::memcpy(id.value.data(), &encoded, id.value.size());
}

void SessionRecordToGuestInfo(const SessionRecord& record, XSESSION_INFO& info) {
  std::memset(&info, 0, sizeof(info));
  Uint64ToXnkid(record.session_id, info.session_id);
  info.host_address.local_ipv4 = record.host_ipv4;
  info.host_address.online_ipv4 = record.host_ipv4;
  info.host_address.online_port = record.host_port;
  info.host_address.ethernet_address = record.host_ethernet_address;
  rex::be<uint64_t> machine_id = record.host_machine_id;
  std::memcpy(info.host_address.online_identity.data(), &machine_id, sizeof(machine_id));
  info.exchange_key.value = record.exchange_key;
}

XSession::XSession(KernelState* kernel_state) : XObject(kernel_state, kObjectType) {}

XSession::~XSession() {
  if (created_ && host_ && kernel_state_->live_compatibility() &&
      kernel_state_->live_compatibility()->session_directory()) {
    kernel_state_->live_compatibility()->session_directory()->Delete(record_.session_id);
  }
}

X_STATUS XSession::Initialize() {
  auto* object = CreateNative<X_KSESSION>();
  if (!object) {
    return X_STATUS_NO_MEMORY;
  }
  object->handle = handle();
  return X_STATUS_SUCCESS;
}

X_RESULT XSession::Create(const XGI_SESSION_CREATE& request) {
  if (created_ || !request.session_info_ptr || !request.nonce_ptr) {
    return X_ERROR_INVALID_PARAMETER;
  }
  auto* live = kernel_state_->live_compatibility();
  if (!live || !live->available() || !live->session_directory()) {
    return X_ERROR_NOT_LOGGED_ON;
  }
  if (request.user_index != 0) {
    return X_ERROR_NO_SUCH_USER;
  }
  if (request.public_slots > kMaximumSessionMembers ||
      request.private_slots > kMaximumSessionMembers ||
      request.private_slots > kMaximumSessionMembers - request.public_slots) {
    return X_ERROR_INVALID_PARAMETER;
  }

  auto* session_info =
      kernel_state_->memory()->TranslateVirtual<XSESSION_INFO*>(request.session_info_ptr);
  auto* nonce = kernel_state_->memory()->TranslateVirtual<rex::be<uint64_t>*>(request.nonce_ptr);
  const auto title = GetTitleIdentity(kernel_state_);

  host_ = (static_cast<uint32_t>(request.flags) & kSessionFlagHost) != 0;
  record_.title_id = title.title_id;
  record_.media_id = title.media_id;
  record_.title_version = title.title_version;
  record_.protocol_version = live->config().session_protocol_version;
  record_.flags = request.flags;
  record_.max_public_slots = request.public_slots;
  record_.max_private_slots = request.private_slots;
  record_.open_public_slots = request.public_slots;
  record_.open_private_slots = request.private_slots;
  record_.contexts = live->user_contexts();
  record_.properties = live->user_properties();

  if (host_) {
    record_.session_id = live->GenerateSessionId();
    record_.nonce = live->GenerateNonce();
    live->GenerateExchangeKey(record_.exchange_key);
    record_.host_xuid = live->identity().xuid;
    record_.host_machine_id = live->identity().machine_id;
    record_.host_ipv4 = live->local_ipv4();
    record_.host_port = live->online_port() ? live->online_port() : kDefaultGamePort;
    record_.host_ethernet_address = live->identity().ethernet_address;
    if (!live->session_directory()->Create(record_)) {
      return X_ERROR_FUNCTION_FAILED;
    }
  } else {
    const uint64_t requested_id = XnkidToUint64(session_info->session_id);
    auto existing = live->session_directory()->Get(requested_id);
    if (!existing) {
      return X_ERROR_NOT_FOUND;
    }
    record_ = *existing;
  }

  SessionRecordToGuestInfo(record_, *session_info);
  *nonce = record_.nonce;
  live->RegisterKey(record_.session_id, record_.exchange_key);
  live->RegisterRoute(record_.host_ipv4, record_);
  created_ = true;
  RefreshLocalDetails();
  REXSYS_INFO("Created {} session {:016X} with {} public and {} private slots",
              host_ ? "host" : "peer", record_.session_id, record_.max_public_slots,
              record_.max_private_slots);
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::Delete(const XGI_SESSION_STATE& request) {
  if (!created_) {
    return X_ERROR_SUCCESS;
  }
  auto* live = kernel_state_->live_compatibility();
  if (live && live->session_directory() && host_) {
    live->session_directory()->Delete(record_.session_id);
  }
  if (live) {
    live->UnregisterKey(record_.session_id);
    live->UnregisterRoute(record_.host_ipv4);
  }
  created_ = false;
  host_ = false;
  record_.lifecycle_state = SessionLifecycleState::kDeleted;
  local_details_.state = static_cast<uint32_t>(XSessionState::kDeleted);
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::Join(const XGI_SESSION_MANAGE& request) {
  if (!created_ || !request.count || request.count > kMaximumSessionMembers) {
    return X_ERROR_INVALID_PARAMETER;
  }
  auto* live = kernel_state_->live_compatibility();
  if (!live || !live->session_directory()) {
    return X_ERROR_NOT_LOGGED_ON;
  }
  if (!SyncRecordFromDirectory()) {
    return X_ERROR_NOT_FOUND;
  }

  const auto* xuids =
      request.xuids_ptr
          ? kernel_state_->memory()->TranslateVirtual<const rex::be<uint64_t>*>(request.xuids_ptr)
          : nullptr;
  const auto* user_indices =
      request.user_indices_ptr
          ? kernel_state_->memory()->TranslateVirtual<const rex::be<uint32_t>*>(
                request.user_indices_ptr)
          : nullptr;
  const auto* private_slots =
      request.private_slots_ptr
          ? kernel_state_->memory()->TranslateVirtual<const rex::be<uint32_t>*>(
                request.private_slots_ptr)
          : nullptr;

  SessionRecord candidate = record_;
  std::vector<SessionMember> members;
  members.reserve(request.count);
  for (uint32_t index = 0; index < request.count; ++index) {
    uint64_t xuid = 0;
    if (xuids) {
      xuid = xuids[index];
    } else {
      if (!user_indices || user_indices[index] != 0) {
        return X_ERROR_NO_SUCH_USER;
      }
      xuid = kernel_state_->user_profile()->xuid();
    }
    if (!xuid) {
      return X_ERROR_INVALID_PARAMETER;
    }
    SessionMember member{.xuid = xuid, .private_slot = private_slots && private_slots[index] != 0};
    if (!AddMemberToRecord(candidate, member)) {
      return X_ERROR_FUNCTION_FAILED;
    }
    members.push_back(member);
  }

  if (host_) {
    if (!live->session_directory()->Modify(candidate)) {
      return X_ERROR_FUNCTION_FAILED;
    }
  } else {
    std::vector<uint64_t> joined_xuids;
    joined_xuids.reserve(members.size());
    for (const auto& member : members) {
      if (!live->session_directory()->Join(record_.session_id, member)) {
        for (const uint64_t joined_xuid : joined_xuids) {
          live->session_directory()->Leave(record_.session_id, joined_xuid);
        }
        return X_ERROR_FUNCTION_FAILED;
      }
      joined_xuids.push_back(member.xuid);
    }
  }

  record_ = std::move(candidate);
  RefreshLocalDetails();
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::Leave(const XGI_SESSION_MANAGE& request) {
  if (!created_ || !request.count || request.count > kMaximumSessionMembers) {
    return X_ERROR_INVALID_PARAMETER;
  }
  auto* live = kernel_state_->live_compatibility();
  if (!live || !live->session_directory()) {
    return X_ERROR_NOT_LOGGED_ON;
  }
  if (!SyncRecordFromDirectory()) {
    return X_ERROR_NOT_FOUND;
  }
  const auto* xuids =
      request.xuids_ptr
          ? kernel_state_->memory()->TranslateVirtual<const rex::be<uint64_t>*>(request.xuids_ptr)
          : nullptr;
  const auto* user_indices =
      request.user_indices_ptr
          ? kernel_state_->memory()->TranslateVirtual<const rex::be<uint32_t>*>(
                request.user_indices_ptr)
          : nullptr;
  SessionRecord candidate = record_;
  std::vector<SessionMember> removed_members;
  removed_members.reserve(request.count);
  for (uint32_t index = 0; index < request.count; ++index) {
    uint64_t xuid = 0;
    if (xuids) {
      xuid = xuids[index];
    } else {
      if (!user_indices || user_indices[index] != 0) {
        return X_ERROR_NO_SUCH_USER;
      }
      xuid = kernel_state_->user_profile()->xuid();
    }
    if (!xuid) {
      return X_ERROR_INVALID_PARAMETER;
    }
    const auto existing = std::ranges::find(candidate.members, xuid, &SessionMember::xuid);
    if (existing != candidate.members.end()) {
      removed_members.push_back(*existing);
      RemoveMemberFromRecord(candidate, xuid);
    }
  }

  if (host_) {
    if (!live->session_directory()->Modify(candidate)) {
      return X_ERROR_FUNCTION_FAILED;
    }
  } else {
    std::vector<SessionMember> left_members;
    left_members.reserve(removed_members.size());
    for (const auto& member : removed_members) {
      if (!live->session_directory()->Leave(record_.session_id, member.xuid)) {
        for (const auto& left_member : left_members) {
          live->session_directory()->Join(record_.session_id, left_member);
        }
        return X_ERROR_FUNCTION_FAILED;
      }
      left_members.push_back(member);
    }
  }

  record_ = std::move(candidate);
  RefreshLocalDetails();
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::Start(const XGI_SESSION_STATE& request) {
  if (!created_) {
    return X_ERROR_FUNCTION_FAILED;
  }
  if (!SyncRecordFromDirectory()) {
    return X_ERROR_NOT_FOUND;
  }
  SessionRecord updated = record_;
  updated.lifecycle_state = SessionLifecycleState::kInGame;
  auto* live = kernel_state_->live_compatibility();
  if (host_ &&
      (!live || !live->session_directory() || !live->session_directory()->Modify(updated))) {
    return X_ERROR_FUNCTION_FAILED;
  }
  record_ = std::move(updated);
  RefreshLocalDetails();
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::End(const XGI_SESSION_STATE& request) {
  if (!created_) {
    return X_ERROR_FUNCTION_FAILED;
  }
  if (!SyncRecordFromDirectory()) {
    return X_ERROR_NOT_FOUND;
  }
  SessionRecord updated = record_;
  updated.lifecycle_state = SessionLifecycleState::kReporting;
  auto* live = kernel_state_->live_compatibility();
  if (host_ &&
      (!live || !live->session_directory() || !live->session_directory()->Modify(updated))) {
    return X_ERROR_FUNCTION_FAILED;
  }
  record_ = std::move(updated);
  RefreshLocalDetails();
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::Modify(const XGI_SESSION_MODIFY& request) {
  if (!created_) {
    return X_ERROR_FUNCTION_FAILED;
  }
  if (!SyncRecordFromDirectory()) {
    return X_ERROR_NOT_FOUND;
  }
  if (request.public_slots > kMaximumSessionMembers ||
      request.private_slots > kMaximumSessionMembers ||
      request.private_slots > kMaximumSessionMembers - request.public_slots) {
    return X_ERROR_INVALID_PARAMETER;
  }
  const uint32_t occupied_public = record_.max_public_slots - record_.open_public_slots;
  const uint32_t occupied_private = record_.max_private_slots - record_.open_private_slots;
  if (request.public_slots < occupied_public || request.private_slots < occupied_private) {
    return X_ERROR_INVALID_PARAMETER;
  }
  SessionRecord updated = record_;
  updated.flags = request.flags;
  updated.max_public_slots = request.public_slots;
  updated.max_private_slots = request.private_slots;
  updated.open_public_slots = updated.max_public_slots - occupied_public;
  updated.open_private_slots = updated.max_private_slots - occupied_private;
  auto* directory = kernel_state_->live_compatibility()->session_directory();
  if (host_ && (!directory || !directory->Modify(updated))) {
    return X_ERROR_FUNCTION_FAILED;
  }
  record_ = std::move(updated);
  RefreshLocalDetails();
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::GetDetails(const XGI_SESSION_DETAILS& request) {
  if (!created_) {
    return X_ERROR_INVALID_PARAMETER;
  }
  if (!SyncRecordFromDirectory()) {
    return X_ERROR_NOT_FOUND;
  }
  const uint32_t required_size = static_cast<uint32_t>(
      sizeof(XSESSION_LOCAL_DETAILS) + record_.members.size() * sizeof(XSESSION_MEMBER));
  if (!request.details_ptr || request.details_buffer_size < required_size) {
    return X_ERROR_INSUFFICIENT_BUFFER;
  }

  RefreshLocalDetails();
  auto* details =
      kernel_state_->memory()->TranslateVirtual<XSESSION_LOCAL_DETAILS*>(request.details_ptr);
  std::memcpy(details, &local_details_, sizeof(*details));
  auto* members = reinterpret_cast<XSESSION_MEMBER*>(details + 1);
  details->session_members_ptr =
      request.details_ptr + static_cast<uint32_t>(sizeof(XSESSION_LOCAL_DETAILS));
  for (size_t index = 0; index < record_.members.size(); ++index) {
    members[index].online_xuid = record_.members[index].xuid;
    members[index].user_index =
        record_.members[index].xuid == kernel_state_->user_profile()->xuid() ? 0 : kNoUserIndex;
    members[index].flags = record_.members[index].private_slot ? kMemberFlagPrivate : 0;
  }
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::Migrate(const XGI_SESSION_MIGRATE& request) {
  if (!created_ || !request.session_info_ptr) {
    return X_ERROR_INVALID_PARAMETER;
  }
  auto* live = kernel_state_->live_compatibility();
  auto* directory = live ? live->session_directory() : nullptr;
  if (!live || !directory) {
    return X_ERROR_NOT_LOGGED_ON;
  }
  if (request.user_index != kNoUserIndex && !SyncRecordFromDirectory()) {
    return X_ERROR_NOT_FOUND;
  }

  const uint64_t old_session_id = record_.session_id;
  const uint32_t old_host_ipv4 = record_.host_ipv4;
  if (request.user_index != kNoUserIndex) {
    if (request.user_index != 0) {
      return X_ERROR_NO_SUCH_USER;
    }
    SessionRecord replacement = record_;
    replacement.session_id = live->GenerateSessionId();
    replacement.nonce = live->GenerateNonce();
    live->GenerateExchangeKey(replacement.exchange_key);
    replacement.host_xuid = live->identity().xuid;
    replacement.host_machine_id = live->identity().machine_id;
    replacement.host_ipv4 = live->local_ipv4();
    replacement.host_port = live->online_port() ? live->online_port() : kDefaultGamePort;
    replacement.host_ethernet_address = live->identity().ethernet_address;
    if (!directory->Migrate(old_session_id, replacement)) {
      return X_ERROR_FUNCTION_FAILED;
    }
    record_ = std::move(replacement);
    host_ = true;
  } else {
    const auto title = GetTitleIdentity(kernel_state_);
    auto candidates = directory->Search(
        title.title_id, title.media_id, title.title_version,
        live->config().session_protocol_version, {}, {}, kMaximumSearchResults);
    const auto replacement =
        std::ranges::find(candidates, old_session_id, &SessionRecord::previous_session_id);
    if (replacement == candidates.end()) {
      return X_ERROR_NOT_FOUND;
    }
    record_ = *replacement;
    host_ = false;
  }

  live->UnregisterKey(old_session_id);
  live->UnregisterRoute(old_host_ipv4);
  live->RegisterKey(record_.session_id, record_.exchange_key);
  live->RegisterRoute(record_.host_ipv4, record_);

  auto* info = kernel_state_->memory()->TranslateVirtual<XSESSION_INFO*>(request.session_info_ptr);
  SessionRecordToGuestInfo(record_, *info);
  RefreshLocalDetails();
  return X_ERROR_SUCCESS;
}

X_RESULT XSession::Search(KernelState* kernel_state, XGI_SESSION_SEARCH& request) {
  auto* live = kernel_state->live_compatibility();
  if (!live || !live->available() || !live->session_directory()) {
    return X_ERROR_NOT_LOGGED_ON;
  }
  const uint32_t maximum_results = request.maximum_results;
  if (request.user_index != 0 || maximum_results > kMaximumSearchResults ||
      request.context_count > kMaximumSearchContexts ||
      request.property_count > kMaximumSearchProperties ||
      (request.context_count && !request.contexts_ptr) ||
      (request.property_count && !request.properties_ptr)) {
    return X_ERROR_INVALID_PARAMETER;
  }
  if (request.property_count) {
    const auto* properties =
        kernel_state->memory()->TranslateVirtual<const XUSER_PROPERTY*>(request.properties_ptr);
    for (uint32_t index = 0; index < request.property_count; ++index) {
      const auto type = properties[index].data.type == XUserDataType::kUnset
                            ? PropertyType(properties[index].property_id)
                            : properties[index].data.type;
      if ((type == XUserDataType::kWString || type == XUserDataType::kBinary) &&
          (properties[index].data.value.binary.size > kMaximumPropertySize ||
           (properties[index].data.value.binary.size &&
            !properties[index].data.value.binary.pointer))) {
        return X_ERROR_INVALID_PARAMETER;
      }
    }
  }
  const uint32_t minimum_size = static_cast<uint32_t>(
      sizeof(XSESSION_SEARCHRESULT_HEADER) + maximum_results * sizeof(XSESSION_SEARCHRESULT));
  if (!request.results_buffer_size) {
    request.results_buffer_size = minimum_size;
    return X_ERROR_INSUFFICIENT_BUFFER;
  }
  if (!request.results_ptr || request.results_buffer_size < sizeof(XSESSION_SEARCHRESULT_HEADER)) {
    request.results_buffer_size = minimum_size;
    return X_ERROR_INSUFFICIENT_BUFFER;
  }

  const auto title = GetTitleIdentity(kernel_state);
  const auto contexts = ReadSearchContexts(kernel_state, request);
  const auto properties = ReadSearchProperties(kernel_state, request);
  auto sessions = live->session_directory()->Search(
      title.title_id, title.media_id, title.title_version,
      live->config().session_protocol_version, contexts, properties, maximum_results);
  const uint32_t required_size = static_cast<uint32_t>(
      sizeof(XSESSION_SEARCHRESULT_HEADER) + sessions.size() * sizeof(XSESSION_SEARCHRESULT));
  if (request.results_buffer_size < required_size) {
    request.results_buffer_size = required_size;
    return X_ERROR_INSUFFICIENT_BUFFER;
  }
  return WriteSearchResults(kernel_state, request.results_ptr, request.results_buffer_size,
                            sessions);
}

X_RESULT XSession::SearchById(KernelState* kernel_state, XGI_SESSION_SEARCH_BY_ID& request) {
  auto* live = kernel_state->live_compatibility();
  if (!live || !live->available() || !live->session_directory()) {
    return X_ERROR_NOT_LOGGED_ON;
  }
  if (request.user_index != 0) {
    return X_ERROR_NO_SUCH_USER;
  }
  const uint32_t minimum_size =
      static_cast<uint32_t>(sizeof(XSESSION_SEARCHRESULT_HEADER) + sizeof(XSESSION_SEARCHRESULT));
  if (!request.results_ptr || !request.results_buffer_size ||
      request.results_buffer_size < minimum_size) {
    request.results_buffer_size = minimum_size;
    return X_ERROR_INSUFFICIENT_BUFFER;
  }
  auto session = live->session_directory()->Get(XnkidToUint64(request.session_id));
  if (!session ||
      session->protocol_version != live->config().session_protocol_version) {
    const std::span<const SessionRecord> empty;
    return WriteSearchResults(kernel_state, request.results_ptr, request.results_buffer_size,
                              empty);
  }
  const std::array<SessionRecord, 1> result = {*session};
  return WriteSearchResults(kernel_state, request.results_ptr, request.results_buffer_size, result);
}

bool XSession::SyncRecordFromDirectory() {
  auto* live = kernel_state_->live_compatibility();
  auto* directory = live ? live->session_directory() : nullptr;
  if (!directory) {
    return false;
  }
  auto current = directory->Get(record_.session_id);
  if (!current) {
    return false;
  }
  record_ = std::move(*current);
  return true;
}

void XSession::RefreshLocalDetails() {
  local_details_.user_index_host = host_ ? 0 : kNoUserIndex;
  local_details_.flags = record_.flags;
  local_details_.max_public_slots = record_.max_public_slots;
  local_details_.max_private_slots = record_.max_private_slots;
  local_details_.available_public_slots = record_.open_public_slots;
  local_details_.available_private_slots = record_.open_private_slots;
  local_details_.actual_member_count = static_cast<uint32_t>(record_.members.size());
  local_details_.returned_member_count = static_cast<uint32_t>(record_.members.size());
  local_details_.state = static_cast<uint32_t>(record_.lifecycle_state);
  local_details_.nonce = record_.nonce;
  SessionRecordToGuestInfo(record_, local_details_.session_info);
}

}  // namespace rex::system::xam
