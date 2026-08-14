/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <rex/kernel/xam/apps/xgi_app.h>
#include <rex/logging.h>
#include <rex/system/xam/xsession.h>
#include <rex/thread.h>

#include <atomic>
#include <limits>

namespace rex {
namespace kernel {
namespace xam {
using namespace rex::system;
using namespace rex::system::xam;
namespace apps {
using namespace rex::system;

namespace {

constexpr uint32_t kAchievementEntryIdOffset = 0x4;
constexpr uint32_t kAchievementEntryStride = 0x8;
constexpr uint32_t kFirstAchievementId = 1;
constexpr uint32_t kLastAchievementId = 65;
constexpr uint32_t kGta4SessionCreateReportedLength = 0x10;
constexpr X_HRESULT kUnsupportedHresult = X_HRESULT_FROM_WIN32(0x32);

std::atomic<AchievementUnlockCallback> g_achievement_unlock_callback{nullptr};

void DispatchAchievementUnlock(uint32_t xbox_id) {
  AchievementUnlockCallback callback =
      g_achievement_unlock_callback.load(std::memory_order_acquire);
  if (callback) {
    callback(xbox_id);
  }
}

X_HRESULT SessionResult(X_RESULT result) {
  return X_HRESULT_FROM_WIN32(result);
}

object_ref<XSession> LookupSession(KernelState* kernel_state, memory::Memory* memory,
                                   uint32_t object_ptr) {
  if (!object_ptr) {
    return nullptr;
  }
  const auto* guest_session = memory->TranslateVirtual<const X_KSESSION*>(object_ptr);
  if (!guest_session) {
    return nullptr;
  }
  return kernel_state->object_table()->LookupObject<XSession>(guest_session->handle);
}

}  // namespace

void SetAchievementUnlockCallback(AchievementUnlockCallback callback) {
  g_achievement_unlock_callback.store(callback, std::memory_order_release);
}

XgiApp::XgiApp(KernelState* kernel_state) : App(kernel_state, 0xFB) {}

// http://mb.mirage.org/bugzilla/xliveless/main.c

X_HRESULT XgiApp::DispatchMessageSync(uint32_t message, uint32_t buffer_ptr,
                                      uint32_t buffer_length) {
  // NOTE: buffer_length may be zero or valid.
  auto buffer = buffer_ptr ? memory_->TranslateVirtual(buffer_ptr) : nullptr;
  switch (message) {
    case 0x000B0006: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_XUSER_SET_CONTEXT))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_XUSER_SET_CONTEXT*>(buffer);
      const uint32_t user_index = request.user_index;
      const uint32_t context_id = request.context.context_id;
      const uint32_t context_value = request.context.value;
      REXKRNL_DEBUG("XGIUserSetContextEx({:08X}, {:08X}, {:08X})", user_index, context_id,
                    context_value);
      if (user_index != 0 || !kernel_state_->live_compatibility()) {
        return X_E_NO_SUCH_USER;
      }
      return kernel_state_->live_compatibility()->SetUserContext(context_id, context_value)
                 ? X_E_SUCCESS
                 : X_E_INVALIDARG;
    }
    case 0x000B0007: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_XUSER_SET_PROPERTY))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_XUSER_SET_PROPERTY*>(buffer);
      const uint32_t user_index = request.user_index;
      const uint32_t property_id = request.property_id;
      const uint32_t value_size = request.data_size;
      const uint32_t value_ptr = request.data_ptr;
      REXKRNL_DEBUG("XGIUserSetPropertyEx({:08X}, {:08X}, {}, {:08X})", user_index, property_id,
                    value_size, value_ptr);
      if (user_index != 0 || !kernel_state_->live_compatibility()) {
        return X_E_NO_SUCH_USER;
      }
      if (value_size && !value_ptr) {
        return X_E_INVALIDARG;
      }
      std::span<const uint8_t> value;
      if (value_size) {
        value = {memory_->TranslateVirtual<const uint8_t*>(value_ptr), value_size};
      }
      return kernel_state_->live_compatibility()->SetUserProperty(property_id, value)
                 ? X_E_SUCCESS
                 : X_E_INVALIDARG;
    }
    case 0x000B0008: {
      if (!buffer || (buffer_length && buffer_length != 8)) {
        REXKRNL_WARN("XGIUserWriteAchievements invalid buffer ({:08X}, {})", buffer_ptr,
                     buffer_length);
        return X_E_SUCCESS;
      }

      uint32_t raw0 = buffer_length >= 4 ? memory::load_and_swap<uint32_t>(buffer + 0) : 0;
      uint32_t raw4 = buffer_length >= 8 ? memory::load_and_swap<uint32_t>(buffer + 4) : 0;
      REXKRNL_INFO("XGIUserWriteAchievements called: buf_len={} raw[0]={:08X} raw[4]={:08X}",
                   buffer_length, raw0, raw4);

      uint32_t achievement_count = raw0;
      uint32_t achievements_ptr = raw4;

      if (!achievements_ptr || !achievement_count) {
        REXKRNL_INFO("XGIUserWriteAchievements: skipped (count={} ptr={:08X})", achievement_count,
                     achievements_ptr);
        return X_E_SUCCESS;
      }

      if (achievement_count > kLastAchievementId) {
        REXKRNL_WARN(
            "XGIUserWriteAchievements: count={} exceeds GTA IV achievement count {}; "
            "clamping",
            achievement_count, kLastAchievementId);
        achievement_count = kLastAchievementId;
      }

      const uint64_t span_end64 =
          static_cast<uint64_t>(achievements_ptr) +
          (static_cast<uint64_t>(achievement_count) * kAchievementEntryStride) - 1;
      if (span_end64 > std::numeric_limits<uint32_t>::max() ||
          !memory_->LookupHeap(achievements_ptr) ||
          !memory_->LookupHeap(static_cast<uint32_t>(span_end64))) {
        REXKRNL_WARN("XGIUserWriteAchievements: ptr {:08X} OOB", achievements_ptr);
        return X_E_SUCCESS;
      }

      auto* base = memory_->TranslateVirtual(achievements_ptr);
      for (uint32_t i = 0; i < achievement_count; ++i) {
        const uint8_t* entry = base + (i * kAchievementEntryStride);
        uint32_t user_index = memory::load_and_swap<uint32_t>(entry);
        uint32_t id = memory::load_and_swap<uint32_t>(entry + kAchievementEntryIdOffset);
        if (id < kFirstAchievementId || id > kLastAchievementId) {
          REXKRNL_WARN("XGIUserWriteAchievements: ignored invalid id={} user={} index={}", id,
                       user_index, i);
          continue;
        }

        REXKRNL_INFO("XGIUserWriteAchievements: id={} user={} index={}", id, user_index, i);
        kernel_state_->UnlockAchievement(id);
        DispatchAchievementUnlock(id);
      }
      return X_E_SUCCESS;
    }
    case 0x000B0010: {
      // GTA IV reports 0x10 here despite passing the complete 0x1C-byte
      // request. The generated call site stores all seven fields before the
      // call, so validate and snapshot the actual ABI structure.
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_CREATE) &&
                      buffer_length != kGta4SessionCreateReportedLength)) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_CREATE*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->Create(request));
    }
    case 0x000B0011: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_STATE))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_STATE*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->Delete(request));
    }
    case 0x000B0012: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_MANAGE))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_MANAGE*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->Join(request));
    }
    case 0x000B0013: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_MANAGE))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_MANAGE*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->Leave(request));
    }
    case 0x000B0014: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_STATE))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_STATE*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->Start(request));
    }
    case 0x000B0015: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_STATE))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_STATE*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->End(request));
    }
    case 0x000B0016: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_SEARCH))) {
        return X_E_INVALIDARG;
      }
      auto& request = *reinterpret_cast<XGI_SESSION_SEARCH*>(buffer);
      return SessionResult(XSession::Search(kernel_state_, request));
    }
    case 0x000B0018: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_MODIFY))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_MODIFY*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->Modify(request));
    }
    case 0x000B001C: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_SEARCH_EX))) {
        return X_E_INVALIDARG;
      }
      auto& request = *reinterpret_cast<XGI_SESSION_SEARCH_EX*>(buffer);
      return SessionResult(XSession::Search(kernel_state_, request.search));
    }
    case 0x000B001D: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_DETAILS))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_DETAILS*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->GetDetails(request));
    }
    case 0x000B001E: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_MIGRATE))) {
        return X_E_INVALIDARG;
      }
      const auto& request = *reinterpret_cast<const XGI_SESSION_MIGRATE*>(buffer);
      auto session = LookupSession(kernel_state_, memory_, request.object_ptr);
      if (!session) {
        return SessionResult(X_ERROR_INVALID_HANDLE);
      }
      return SessionResult(session->Migrate(request));
    }
    case 0x000B0019: {
      if (!buffer || (buffer_length && buffer_length != 8)) {
        return X_E_INVALIDARG;
      }

      uint32_t user_index = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t session_info_ptr = memory::load_and_swap<uint32_t>(buffer + 4);

      REXKRNL_DEBUG("XSessionGetInvitationData - unimplemented({}, {:08X})", user_index,
                    session_info_ptr);

      return kUnsupportedHresult;
    }
    case 0x000B001A: {
      if (!buffer || (buffer_length && buffer_length != 32)) {
        return X_E_INVALIDARG;
      }

      uint32_t obj_ptr = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t flags = memory::load_and_swap<uint32_t>(buffer + 4);
      uint64_t session_nonce = memory::load_and_swap<uint64_t>(buffer + 8);
      uint32_t session_duration_sec = memory::load_and_swap<uint32_t>(buffer + 16);  // 300
      uint32_t results_buffer_size = memory::load_and_swap<uint32_t>(buffer + 20);
      uint32_t results_ptr = memory::load_and_swap<uint32_t>(buffer + 24);

      REXKRNL_DEBUG("XSessionArbitrationRegister({:08X}, {:08X}, {:016X}, {:08X}, {:08X}, {:08X})",
                    obj_ptr, flags, session_nonce, session_duration_sec, results_buffer_size,
                    results_ptr);

      return kUnsupportedHresult;
    }
    case 0x000B001B: {
      if (!buffer || (buffer_length && buffer_length != sizeof(XGI_SESSION_SEARCH_BY_ID))) {
        return X_E_INVALIDARG;
      }
      auto& request = *reinterpret_cast<XGI_SESSION_SEARCH_BY_ID*>(buffer);
      return SessionResult(XSession::SearchById(kernel_state_, request));
    }
    case 0x000B001F: {
      if (!buffer || (buffer_length && buffer_length != 24)) {
        return X_E_INVALIDARG;
      }

      uint32_t obj_ptr = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t array_count = memory::load_and_swap<uint32_t>(buffer + 4);
      uint32_t xuid_array_ptr = memory::load_and_swap<uint32_t>(buffer + 8);
      uint32_t reserved1 = memory::load_and_swap<uint32_t>(buffer + 12);
      uint32_t reserved2 = memory::load_and_swap<uint32_t>(buffer + 16);
      uint32_t reserved3 = memory::load_and_swap<uint32_t>(buffer + 20);

      REXKRNL_DEBUG("XSessionModifySkill({:08X}, {}, {:08X}, {}, {}, {})", obj_ptr, array_count,
                    xuid_array_ptr, reserved1, reserved2, reserved3);

      return kUnsupportedHresult;
    }
    case 0x000B0020: {
      if (!buffer || (buffer_length && buffer_length != 8)) {
        return X_E_INVALIDARG;
      }

      uint32_t user_index = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t view_id = memory::load_and_swap<uint32_t>(buffer + 4);

      REXKRNL_DEBUG("XUserResetStatsView({:08X}, {})", user_index, view_id);

      return kUnsupportedHresult;
    }
    case 0x000B0021: {
      if (!buffer || (buffer_length && buffer_length != 28)) {
        return X_E_INVALIDARG;
      }

      uint32_t title_id = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t xuids_count = memory::load_and_swap<uint32_t>(buffer + 4);
      uint32_t xuids_ptr = memory::load_and_swap<uint32_t>(buffer + 8);
      uint32_t specs_count = memory::load_and_swap<uint32_t>(buffer + 12);
      uint32_t specs_ptr = memory::load_and_swap<uint32_t>(buffer + 16);
      uint32_t results_size = memory::load_and_swap<uint32_t>(buffer + 20);
      uint32_t results_ptr = memory::load_and_swap<uint32_t>(buffer + 24);

      REXKRNL_DEBUG("XUserReadStats({}, {}, {:08X}, {}, {:08X}, {}, {:08X})", title_id, xuids_count,
                    xuids_ptr, specs_count, specs_ptr, results_size, results_ptr);

      return kUnsupportedHresult;
    }
    case 0x000B0025: {
      if (!buffer || (buffer_length && buffer_length != 24)) {
        return X_E_INVALIDARG;
      }

      uint32_t obj_ptr = memory::load_and_swap<uint32_t>(buffer + 0);
      uint64_t xuid = memory::load_and_swap<uint64_t>(buffer + 8);
      uint32_t num_views = memory::load_and_swap<uint32_t>(buffer + 16);
      uint32_t views_ptr = memory::load_and_swap<uint32_t>(buffer + 20);

      REXKRNL_DEBUG("XSessionWriteStats({:08X}, {:016X}, {:08X}, {:08X})", obj_ptr, xuid, num_views,
                    views_ptr);

      return kUnsupportedHresult;
    }
    case 0x000B0026: {
      if (!buffer || (buffer_length && buffer_length != 24)) {
        return X_E_INVALIDARG;
      }

      uint32_t obj_ptr = memory::load_and_swap<uint32_t>(buffer + 0);
      uint64_t xuid = memory::load_and_swap<uint64_t>(buffer + 8);
      uint32_t num_views = memory::load_and_swap<uint32_t>(buffer + 16);
      uint32_t views_ptr = memory::load_and_swap<uint32_t>(buffer + 20);

      REXKRNL_DEBUG("XSessionFlushStats({:08X}, {:016X}, {:08X}, {:08X})", obj_ptr, xuid, num_views,
                    views_ptr);

      return kUnsupportedHresult;
    }
    case 0x000B0036: {
      // Called after opening xbox live arcade and clicking on xbox live v5759
      // to 5787 and called after clicking xbox live in the game library from
      // v6683 to v6717
      // Does not get sent a buffer
      REXKRNL_DEBUG("XInvalidateGamerTileCache, unimplemented");
      return X_E_FAIL;
    }
    case 0x000B003D: {
      if (!buffer || (buffer_length && buffer_length != 16)) {
        return X_E_INVALIDARG;
      }

      uint32_t user_index = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t AnId_buffer_size = memory::load_and_swap<uint32_t>(buffer + 4);
      uint32_t AnId_buffer_ptr = memory::load_and_swap<uint32_t>(buffer + 8);
      uint32_t block = memory::load_and_swap<uint32_t>(buffer + 12);

      REXKRNL_DEBUG("XUserGetANID({:08X}, {:08X}, {:08X}, {:08X})", user_index, AnId_buffer_size,
                    AnId_buffer_ptr, block);
      auto* live = kernel_state_->live_compatibility();
      if (user_index != 0 || !AnId_buffer_size || !AnId_buffer_ptr || !live) {
        return X_E_INVALIDARG;
      }
      static constexpr char kHexDigits[] = "0123456789abcdef";
      const auto& secret = live->identity().install_secret;
      auto* output = memory_->TranslateVirtual<uint8_t*>(AnId_buffer_ptr);
      for (uint32_t index = 0; index + 1 < AnId_buffer_size; ++index) {
        const uint8_t byte = secret[(index / 2) % secret.size()];
        const uint8_t nibble =
            index & 1 ? static_cast<uint8_t>(byte & 0x0F) : static_cast<uint8_t>(byte >> 4);
        output[index] = static_cast<uint8_t>(kHexDigits[nibble]);
      }
      output[AnId_buffer_size - 1] = 0;
      return X_E_SUCCESS;
    }
    case 0x000B0041: {
      if (!buffer || (buffer_length && buffer_length != 32)) {
        return X_E_INVALIDARG;
      }
      // 00000000 2789fecc 00000000 00000000 200491e0 00000000 200491f0 20049340
      uint32_t user_index = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t context_ptr = memory::load_and_swap<uint32_t>(buffer + 16);
      auto context = context_ptr ? memory_->TranslateVirtual(context_ptr) : nullptr;
      uint32_t context_id = context ? memory::load_and_swap<uint32_t>(context + 0) : 0;
      REXKRNL_DEBUG("XGIUserGetContext({:08X}, {:08X}, {:08X}))", user_index, context_ptr,
                    context_id);
      auto* live = kernel_state_->live_compatibility();
      if (user_index != 0 || !context || !live) {
        return X_E_INVALIDARG;
      }
      const auto value = live->GetUserContext(context_id);
      if (!value) {
        return X_E_FAIL;
      }
      memory::store_and_swap<uint32_t>(context + 4, *value);
      return X_E_SUCCESS;
    }
    case 0x000B0060: {
      if (!buffer || (buffer_length && buffer_length != 32)) {
        return X_E_INVALIDARG;
      }

      uint32_t user_index = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t num_session_ids = memory::load_and_swap<uint32_t>(buffer + 4);
      uint32_t session_ids_ptr = memory::load_and_swap<uint32_t>(buffer + 8);
      uint32_t results_buffer_size = memory::load_and_swap<uint32_t>(buffer + 12);
      uint32_t search_results_ptr = memory::load_and_swap<uint32_t>(buffer + 16);
      uint32_t reserved1 = memory::load_and_swap<uint32_t>(buffer + 20);
      uint32_t reserved2 = memory::load_and_swap<uint32_t>(buffer + 24);
      uint32_t reserved3 = memory::load_and_swap<uint32_t>(buffer + 28);

      REXKRNL_DEBUG("XSessionSearchByIds({:08X}, {:08X}, {:08X}, {:08X}, {:08X}, {}, {}, {})",
                    user_index, num_session_ids, session_ids_ptr, results_buffer_size,
                    search_results_ptr, reserved1, reserved2, reserved3);

      return kUnsupportedHresult;
    }
    case 0x000B0065: {
      if (!buffer || (buffer_length && buffer_length != 52)) {
        return X_E_INVALIDARG;
      }

      uint32_t proc_index = memory::load_and_swap<uint32_t>(buffer + 0);
      uint32_t user_index = memory::load_and_swap<uint32_t>(buffer + 4);
      uint32_t num_results = memory::load_and_swap<uint32_t>(buffer + 8);
      uint16_t num_weighted_properties = memory::load_and_swap<uint16_t>(buffer + 12);
      uint16_t num_weighted_contexts = memory::load_and_swap<uint16_t>(buffer + 14);
      uint32_t weighted_search_properties_ptr = memory::load_and_swap<uint32_t>(buffer + 16);
      uint32_t weighted_search_contexts_ptr = memory::load_and_swap<uint32_t>(buffer + 20);
      uint16_t num_props = memory::load_and_swap<uint16_t>(buffer + 24);
      uint16_t num_ctx = memory::load_and_swap<uint16_t>(buffer + 26);
      uint32_t non_weighted_search_properties_ptr = memory::load_and_swap<uint32_t>(buffer + 28);
      uint32_t non_weighted_search_contexts_ptr = memory::load_and_swap<uint32_t>(buffer + 32);
      uint32_t results_buffer_size = memory::load_and_swap<uint32_t>(buffer + 36);
      uint32_t search_results_ptr = memory::load_and_swap<uint32_t>(buffer + 40);
      uint32_t num_users = memory::load_and_swap<uint32_t>(buffer + 44);
      uint32_t weighted_search = memory::load_and_swap<uint32_t>(buffer + 48);

      REXKRNL_DEBUG(
          "XSessionSearchWeighted({:08X}, {:08X}, {:08X}, {}, {}, {:08X}, {:08X}, {}, {}, {:08X}, "
          "{:08X}, {:08X}, {:08X}, {:08X}, {:08X})",
          proc_index, user_index, num_results, num_weighted_properties, num_weighted_contexts,
          weighted_search_properties_ptr, weighted_search_contexts_ptr, num_props, num_ctx,
          non_weighted_search_properties_ptr, non_weighted_search_contexts_ptr, results_buffer_size,
          search_results_ptr, num_users, weighted_search);

      return kUnsupportedHresult;
    }
    case 0x000B0071: {
      REXKRNL_DEBUG("XGI 0x000B0071, unimplemented");
      return kUnsupportedHresult;
    }
  }
  REXKRNL_ERROR(
      "Unimplemented XGI message app={:08X}, msg={:08X}, arg1={:08X}, "
      "arg2={:08X}",
      app_id(), message, buffer_ptr, buffer_length);
  return X_E_FAIL;
}

}  // namespace apps
}  // namespace xam
}  // namespace kernel
}  // namespace rex
