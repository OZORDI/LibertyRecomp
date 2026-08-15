/**
 ******************************************************************************
 * @file        xsession.h
 * @brief       Xbox 360 session object and guest ABI structures.
 ******************************************************************************
 */

#pragma once

#include <array>
#include <cstdint>

#include <rex/system/xam/live_compatibility.h>
#include <rex/system/xobject.h>
#include <rex/system/xtypes.h>
#include <rex/types.h>

namespace rex::system::xam {

struct XNKID {
  std::array<uint8_t, 8> value{};
};
static_assert_size(XNKID, 0x8);

struct XNKEY {
  std::array<uint8_t, 16> value{};
};
static_assert_size(XNKEY, 0x10);

struct XNADDR {
  uint32_t local_ipv4;
  uint32_t online_ipv4;
  rex::be<uint16_t> online_port;
  std::array<uint8_t, 6> ethernet_address{};
  std::array<uint8_t, 20> online_identity{};
};
static_assert_size(XNADDR, 0x24);

struct XSESSION_INFO {
  XNKID session_id;
  XNADDR host_address;
  XNKEY exchange_key;
};
static_assert_size(XSESSION_INFO, 0x3C);

struct XUSER_CONTEXT {
  rex::be<uint32_t> context_id;
  rex::be<uint32_t> value;
};
static_assert_size(XUSER_CONTEXT, 0x8);

enum class XUserDataType : uint8_t {
  kContext = 0,
  kInt32 = 1,
  kInt64 = 2,
  kDouble = 3,
  kWString = 4,
  kFloat = 5,
  kBinary = 6,
  kDateTime = 7,
  kUnset = 0xFF,
};

union XUSER_DATA_VALUE {
  rex::be<int32_t> s32;
  rex::be<int64_t> s64;
  rex::be<uint32_t> u32;
  rex::be<double> f64;
  struct {
    rex::be<uint32_t> size;
    rex::be<uint32_t> pointer;
  } unicode;
  rex::be<float> f32;
  struct {
    rex::be<uint32_t> size;
    rex::be<uint32_t> pointer;
  } binary;
  rex::be<uint64_t> filetime;
};
static_assert_size(XUSER_DATA_VALUE, 0x8);

struct alignas(8) XUSER_DATA {
  XUserDataType type = XUserDataType::kUnset;
  std::array<uint8_t, 7> reserved{};
  XUSER_DATA_VALUE value{};
};
static_assert_size(XUSER_DATA, 0x10);

struct XUSER_PROPERTY {
  rex::be<uint32_t> property_id;
  uint32_t reserved = 0;
  XUSER_DATA data;
};
static_assert_size(XUSER_PROPERTY, 0x18);

struct XSESSION_SEARCHRESULT {
  XSESSION_INFO info;
  rex::be<uint32_t> open_public_slots;
  rex::be<uint32_t> open_private_slots;
  rex::be<uint32_t> filled_public_slots;
  rex::be<uint32_t> filled_private_slots;
  rex::be<uint32_t> properties_count;
  rex::be<uint32_t> contexts_count;
  rex::be<uint32_t> properties_ptr;
  rex::be<uint32_t> contexts_ptr;
};
static_assert_size(XSESSION_SEARCHRESULT, 0x5C);

struct XSESSION_SEARCHRESULT_HEADER {
  rex::be<uint32_t> search_results_count;
  rex::be<uint32_t> search_results_ptr;
};
static_assert_size(XSESSION_SEARCHRESULT_HEADER, 0x8);

enum class XSessionState : uint32_t {
  kLobby,
  kRegistration,
  kInGame,
  kReporting,
  kDeleted,
};

struct XSESSION_MEMBER {
  rex::be<uint64_t> online_xuid;
  rex::be<uint32_t> user_index;
  rex::be<uint32_t> flags;
};
static_assert_size(XSESSION_MEMBER, 0x10);

struct XSESSION_LOCAL_DETAILS {
  rex::be<uint32_t> user_index_host;
  rex::be<uint32_t> game_type;
  rex::be<uint32_t> game_mode;
  rex::be<uint32_t> flags;
  rex::be<uint32_t> max_public_slots;
  rex::be<uint32_t> max_private_slots;
  rex::be<uint32_t> available_public_slots;
  rex::be<uint32_t> available_private_slots;
  rex::be<uint32_t> actual_member_count;
  rex::be<uint32_t> returned_member_count;
  rex::be<uint32_t> state;
  rex::be<uint64_t> nonce;
  XSESSION_INFO session_info;
  XNKID arbitration_session_id;
  rex::be<uint32_t> session_members_ptr;
};
static_assert_size(XSESSION_LOCAL_DETAILS, 0x80);

struct X_KSESSION {
  rex::be<uint32_t> handle;
};
static_assert_size(X_KSESSION, 0x4);

struct XGI_SESSION_CREATE {
  rex::be<uint32_t> object_ptr;
  rex::be<uint32_t> flags;
  rex::be<uint32_t> public_slots;
  rex::be<uint32_t> private_slots;
  rex::be<uint32_t> user_index;
  rex::be<uint32_t> session_info_ptr;
  rex::be<uint32_t> nonce_ptr;
};
static_assert_size(XGI_SESSION_CREATE, 0x1C);

struct XGI_SESSION_STATE {
  rex::be<uint32_t> object_ptr;
  rex::be<uint32_t> flags;
  rex::be<uint64_t> nonce;
};
static_assert_size(XGI_SESSION_STATE, 0x10);

struct XGI_SESSION_MANAGE {
  rex::be<uint32_t> object_ptr;
  rex::be<uint32_t> count;
  rex::be<uint32_t> xuids_ptr;
  rex::be<uint32_t> user_indices_ptr;
  rex::be<uint32_t> private_slots_ptr;
};
static_assert_size(XGI_SESSION_MANAGE, 0x14);

struct XGI_SESSION_MODIFY {
  rex::be<uint32_t> object_ptr;
  rex::be<uint32_t> flags;
  rex::be<uint32_t> public_slots;
  rex::be<uint32_t> private_slots;
};
static_assert_size(XGI_SESSION_MODIFY, 0x10);

struct XGI_SESSION_SEARCH {
  rex::be<uint32_t> procedure_index;
  rex::be<uint32_t> user_index;
  rex::be<uint32_t> maximum_results;
  rex::be<uint16_t> property_count;
  rex::be<uint16_t> context_count;
  rex::be<uint32_t> properties_ptr;
  rex::be<uint32_t> contexts_ptr;
  rex::be<uint32_t> results_buffer_size;
  rex::be<uint32_t> results_ptr;
};
static_assert_size(XGI_SESSION_SEARCH, 0x20);

struct XGI_SESSION_SEARCH_EX {
  XGI_SESSION_SEARCH search;
  rex::be<uint32_t> user_count;
};
static_assert_size(XGI_SESSION_SEARCH_EX, 0x24);

struct XGI_SESSION_DETAILS {
  rex::be<uint32_t> object_ptr;
  rex::be<uint32_t> details_buffer_size;
  rex::be<uint32_t> details_ptr;
  rex::be<uint32_t> reserved1;
  rex::be<uint32_t> reserved2;
  rex::be<uint32_t> reserved3;
};
static_assert_size(XGI_SESSION_DETAILS, 0x18);

struct XGI_SESSION_MIGRATE {
  rex::be<uint32_t> object_ptr;
  rex::be<uint32_t> session_info_ptr;
  rex::be<uint32_t> user_index;
  rex::be<uint32_t> reserved1;
  rex::be<uint32_t> reserved2;
  rex::be<uint32_t> reserved3;
};
static_assert_size(XGI_SESSION_MIGRATE, 0x18);

struct XGI_SESSION_SEARCH_BY_ID {
  rex::be<uint32_t> user_index;
  XNKID session_id;
  rex::be<uint32_t> results_buffer_size;
  rex::be<uint32_t> results_ptr;
};
static_assert_size(XGI_SESSION_SEARCH_BY_ID, 0x14);

struct XGI_XUSER_SET_CONTEXT {
  rex::be<uint32_t> user_index;
  rex::be<uint32_t> unused;
  rex::be<uint64_t> xuid;
  XUSER_CONTEXT context;
};
static_assert_size(XGI_XUSER_SET_CONTEXT, 0x18);

struct XGI_XUSER_SET_PROPERTY {
  rex::be<uint32_t> user_index;
  rex::be<uint32_t> unused;
  rex::be<uint64_t> xuid;
  rex::be<uint32_t> property_id;
  rex::be<uint32_t> data_size;
  rex::be<uint32_t> data_ptr;
};
static_assert_size(XGI_XUSER_SET_PROPERTY, 0x20);

class XSession final : public XObject {
 public:
  static const XObject::Type kObjectType = XObject::Type::Session;

  explicit XSession(KernelState* kernel_state);
  ~XSession() override;

  X_STATUS Initialize();
  X_RESULT Create(const XGI_SESSION_CREATE& request);
  X_RESULT Delete(const XGI_SESSION_STATE& request);
  X_RESULT Join(const XGI_SESSION_MANAGE& request);
  X_RESULT Leave(const XGI_SESSION_MANAGE& request);
  X_RESULT Start(const XGI_SESSION_STATE& request);
  X_RESULT End(const XGI_SESSION_STATE& request);
  X_RESULT Modify(const XGI_SESSION_MODIFY& request);
  X_RESULT GetDetails(const XGI_SESSION_DETAILS& request);
  X_RESULT Migrate(const XGI_SESSION_MIGRATE& request);

  static X_RESULT Search(KernelState* kernel_state, XGI_SESSION_SEARCH& request);
  static X_RESULT SearchById(KernelState* kernel_state, XGI_SESSION_SEARCH_BY_ID& request);

  const SessionRecord& record() const { return record_; }

 private:
  bool SyncRecordFromDirectory();
  void RefreshLocalDetails();

  bool created_ = false;
  bool host_ = false;
  SessionRecord record_;
  XSESSION_LOCAL_DETAILS local_details_{};
};

uint64_t XnkidToUint64(const XNKID& id);
void Uint64ToXnkid(uint64_t value, XNKID& id);
void SessionRecordToGuestInfo(const SessionRecord& record, XSESSION_INFO& info);

}  // namespace rex::system::xam
