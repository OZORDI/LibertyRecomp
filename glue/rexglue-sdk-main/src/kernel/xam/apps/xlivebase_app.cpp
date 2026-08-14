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

#include <rex/kernel/xam/apps/xlivebase_app.h>
#include <rex/logging.h>
#include <rex/thread.h>

#include <cstring>

namespace rex {
namespace kernel {
namespace xam {
using namespace rex::system;
using namespace rex::system::xam;
namespace apps {
using namespace rex::system;

namespace {

struct XCONTENT_MARKETPLACE_COUNTS_REQUEST {
  rex::be<uint32_t> user_index;
  rex::be<uint32_t> title_id;
  rex::be<uint32_t> content_categories;
  rex::be<uint32_t> result_ptr;
};
static_assert_size(XCONTENT_MARKETPLACE_COUNTS_REQUEST, 0x10);

struct XCONTENT_MARKETPLACE_COUNTS_RESULT {
  rex::be<uint32_t> new_offers;
  rex::be<uint32_t> total_offers;
};
static_assert_size(XCONTENT_MARKETPLACE_COUNTS_RESULT, 0x8);

}  // namespace

XLiveBaseApp::XLiveBaseApp(KernelState* kernel_state) : App(kernel_state, 0xFC) {}

// http://mb.mirage.org/bugzilla/xliveless/main.c

X_HRESULT XLiveBaseApp::DispatchMessageSync(uint32_t message, uint32_t buffer_ptr,
                                            uint32_t buffer_length) {
  // NOTE: buffer_length may be zero or valid.
  auto buffer = buffer_ptr ? memory_->TranslateVirtual(buffer_ptr) : nullptr;
  switch (message) {
    case 0x00058004: {
      // Called on startup, seems to just return a bool in the buffer.
      if (!buffer || (buffer_length && buffer_length != 4)) {
        return X_E_INVALIDARG;
      }
      REXKRNL_DEBUG("XLiveBaseGetLogonId({:08X})", buffer_ptr);
      const auto* live = kernel_state_->live_compatibility();
      memory::store_and_swap<uint32_t>(buffer + 0, live && live->available() ? 1 : 0);
      return X_E_SUCCESS;
    }
    case 0x00058006: {
      if (!buffer || (buffer_length && buffer_length != 4)) {
        return X_E_INVALIDARG;
      }
      REXKRNL_DEBUG("XLiveBaseGetNatType({:08X})", buffer_ptr);
      const auto* live = kernel_state_->live_compatibility();
      memory::store_and_swap<uint32_t>(buffer + 0,
                                       live && live->available() ? 1 : 0);
      return X_E_SUCCESS;
    }
    case 0x00058007: {
      // Occurs if title calls XOnlineGetServiceInfo, expects dwServiceId
      // and pServiceInfo. pServiceInfo should contain pointer to
      // XONLINE_SERVICE_INFO structure.
      REXKRNL_DEBUG("CXLiveLogon::GetServiceInfo({:08X}, {:08X})", buffer_ptr, buffer_length);
      return kernel_state_->live_compatibility() &&
                     kernel_state_->live_compatibility()->available()
                 ? X_E_SUCCESS
                 : 0x80151802;  // ERROR_CONNECTION_INVALID
    }
    case 0x00058009: {
      if (!buffer || (buffer_length &&
                      buffer_length != sizeof(XCONTENT_MARKETPLACE_COUNTS_REQUEST))) {
        return X_E_INVALIDARG;
      }
      const auto& request =
          *reinterpret_cast<const XCONTENT_MARKETPLACE_COUNTS_REQUEST*>(buffer);
      if (request.user_index != 0 || !request.result_ptr) {
        return X_E_INVALIDARG;
      }
      auto* result = memory_->TranslateVirtual<XCONTENT_MARKETPLACE_COUNTS_RESULT*>(
          request.result_ptr);
      std::memset(result, 0, sizeof(*result));
      REXKRNL_DEBUG("XContentGetMarketplaceCounts(title={:08X}, categories={:08X}) -> 0",
                    static_cast<uint32_t>(request.title_id),
                    static_cast<uint32_t>(request.content_categories));
      return X_E_SUCCESS;
    }
    case 0x00058020: {
      // 0x00058004 is called right before this.
      // We should create a XamEnumerate-able empty list here, but I'm not
      // sure of the format.
      // buffer_length seems to be the same ptr sent to 0x00058004.
      REXKRNL_DEBUG("CXLiveFriends::Enumerate({:08X}, {:08X}) unimplemented", buffer_ptr,
                    buffer_length);
      return X_E_FAIL;
    }
    case 0x00058023: {
      REXKRNL_DEBUG(
          "CXLiveMessaging::XMessageGameInviteGetAcceptedInfo({:08X}, {:08X}) "
          "unimplemented",
          buffer_ptr, buffer_length);
      return X_E_FAIL;
    }
    case 0x00058046: {
      // Required to be successful for 4D530910 to detect signed-in profile
      // Doesn't seem to set anything in the given buffer, probably only takes
      // input
      REXKRNL_DEBUG("XLiveBaseUnk58046({:08X}, {:08X}) unimplemented", buffer_ptr, buffer_length);
      return X_E_SUCCESS;
    }
    case 0x00058037: {
      REXKRNL_DEBUG("XPresenceInitialize({:08X}, {:08X})", buffer_ptr, buffer_length);
      return X_E_SUCCESS;
    }
  }
  REXKRNL_ERROR(
      "Unimplemented XLIVEBASE message app={:08X}, msg={:08X}, arg1={:08X}, "
      "arg2={:08X}",
      app_id(), message, buffer_ptr, buffer_length);
  return X_E_FAIL;
}

}  // namespace apps
}  // namespace xam
}  // namespace kernel
}  // namespace rex
