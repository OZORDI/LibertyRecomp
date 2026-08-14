/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

// Disable warnings about unused parameters for kernel functions
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <rex/kernel/xam/private.h>
#include <rex/kernel/xboxkrnl/error.h>
#include <rex/logging.h>
#include <rex/hook.h>
#include <rex/types.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xevent.h>
#include <rex/system/xio.h>
#include <rex/system/xthread.h>
#include <rex/system/xtypes.h>
#include <rex/system/xam/xsession.h>

#include <cstring>
#include <limits>
#include <memory>
#include <vector>

namespace rex {
namespace kernel {
namespace xam {
using namespace rex::system;
using namespace rex::system::xam;

namespace {

constexpr uint32_t kXgiAppId = 0xFB;
constexpr uint32_t kMaximumSessionArrayCount = 64;
constexpr uint32_t kMaximumPropertyPayloadSize = 512;

struct GuestInputSegment {
  uint32_t address = 0;
  std::vector<uint8_t> bytes;
};

class AsyncInputSnapshot {
 public:
  explicit AsyncInputSnapshot(memory::Memory* memory) : memory_(memory) {}

  bool Capture(uint32_t address, size_t size) {
    if (!size) {
      return true;
    }
    if (!address || size > std::numeric_limits<uint32_t>::max()) {
      valid_ = false;
      return false;
    }
    const uint64_t end = static_cast<uint64_t>(address) + size - 1;
    if (end > std::numeric_limits<uint32_t>::max() || !memory_->LookupHeap(address) ||
        !memory_->LookupHeap(static_cast<uint32_t>(end))) {
      valid_ = false;
      return false;
    }
    GuestInputSegment segment{.address = address, .bytes = {}};
    segment.bytes.resize(size);
    std::memcpy(segment.bytes.data(), memory_->TranslateVirtual(address), size);
    segments_.push_back(std::move(segment));
    return true;
  }

  bool Restore() const {
    if (!valid_) {
      return false;
    }
    for (const auto& segment : segments_) {
      const uint64_t end = static_cast<uint64_t>(segment.address) + segment.bytes.size() - 1;
      if (!memory_->LookupHeap(segment.address) || end > std::numeric_limits<uint32_t>::max() ||
          !memory_->LookupHeap(static_cast<uint32_t>(end))) {
        return false;
      }
      std::memcpy(memory_->TranslateVirtual(segment.address), segment.bytes.data(),
                  segment.bytes.size());
    }
    return true;
  }

  void Invalidate() { valid_ = false; }
  bool valid() const { return valid_; }

 private:
  memory::Memory* memory_;
  bool valid_ = true;
  std::vector<GuestInputSegment> segments_;
};

size_t XgiRequestSize(uint32_t message) {
  switch (message) {
    case 0x000B0006:
      return sizeof(XGI_XUSER_SET_CONTEXT);
    case 0x000B0007:
      return sizeof(XGI_XUSER_SET_PROPERTY);
    case 0x000B0010:
      return sizeof(XGI_SESSION_CREATE);
    case 0x000B0011:
    case 0x000B0014:
    case 0x000B0015:
      return sizeof(XGI_SESSION_STATE);
    case 0x000B0012:
    case 0x000B0013:
      return sizeof(XGI_SESSION_MANAGE);
    case 0x000B0016:
      return sizeof(XGI_SESSION_SEARCH);
    case 0x000B0018:
      return sizeof(XGI_SESSION_MODIFY);
    case 0x000B001B:
      return sizeof(XGI_SESSION_SEARCH_BY_ID);
    case 0x000B001C:
      return sizeof(XGI_SESSION_SEARCH_EX);
    case 0x000B001D:
      return sizeof(XGI_SESSION_DETAILS);
    case 0x000B001E:
      return sizeof(XGI_SESSION_MIGRATE);
    default:
      return 0;
  }
}

bool CaptureProperties(AsyncInputSnapshot& snapshot, memory::Memory* memory,
                       uint32_t properties_ptr, uint32_t property_count) {
  if (!property_count) {
    return true;
  }
  if (!properties_ptr || property_count > kMaximumSessionArrayCount ||
      !snapshot.Capture(properties_ptr,
                        static_cast<size_t>(property_count) * sizeof(XUSER_PROPERTY))) {
    return false;
  }
  const auto* properties = memory->TranslateVirtual<const XUSER_PROPERTY*>(properties_ptr);
  for (uint32_t index = 0; index < property_count; ++index) {
    auto type = properties[index].data.type;
    if (type == XUserDataType::kUnset) {
      type = static_cast<XUserDataType>(
          (static_cast<uint32_t>(properties[index].property_id) >> 28) & 0x0F);
    }
    if (type != XUserDataType::kWString && type != XUserDataType::kBinary) {
      continue;
    }
    const uint32_t size = properties[index].data.value.binary.size;
    const uint32_t pointer = properties[index].data.value.binary.pointer;
    if (size > kMaximumPropertyPayloadSize || !snapshot.Capture(pointer, size)) {
      return false;
    }
  }
  return true;
}

bool CaptureSearchInputs(AsyncInputSnapshot& snapshot, memory::Memory* memory,
                         const XGI_SESSION_SEARCH& request) {
  const uint32_t context_count = request.context_count;
  const uint32_t property_count = request.property_count;
  if (context_count > kMaximumSessionArrayCount ||
      (context_count && !snapshot.Capture(request.contexts_ptr, static_cast<size_t>(context_count) *
                                                                    sizeof(XUSER_CONTEXT)))) {
    return false;
  }
  return CaptureProperties(snapshot, memory, request.properties_ptr, property_count);
}

std::shared_ptr<AsyncInputSnapshot> CaptureAsyncInputs(uint32_t app, uint32_t message,
                                                       uint32_t buffer_ptr,
                                                       uint32_t buffer_length) {
  auto* memory = REX_KERNEL_MEMORY();
  auto snapshot = std::make_shared<AsyncInputSnapshot>(memory);
  const size_t expected_size = app == kXgiAppId ? XgiRequestSize(message) : 0;
  const size_t request_size = expected_size ? expected_size : buffer_length;
  if (!snapshot->Capture(buffer_ptr, request_size) || app != kXgiAppId || !buffer_ptr) {
    return snapshot;
  }

  switch (message) {
    case 0x000B0007: {
      const auto* request = memory->TranslateVirtual<const XGI_XUSER_SET_PROPERTY*>(buffer_ptr);
      if (request->data_size > kMaximumPropertyPayloadSize ||
          !snapshot->Capture(request->data_ptr, request->data_size)) {
        return snapshot;
      }
      break;
    }
    case 0x000B0010: {
      const auto* request = memory->TranslateVirtual<const XGI_SESSION_CREATE*>(buffer_ptr);
      snapshot->Capture(request->session_info_ptr, sizeof(XSESSION_INFO));
      break;
    }
    case 0x000B0012:
    case 0x000B0013: {
      const auto* request = memory->TranslateVirtual<const XGI_SESSION_MANAGE*>(buffer_ptr);
      const uint32_t count = request->count;
      if (count > kMaximumSessionArrayCount) {
        snapshot->Invalidate();
        break;
      }
      if (request->xuids_ptr) {
        snapshot->Capture(request->xuids_ptr,
                          static_cast<size_t>(count) * sizeof(rex::be<uint64_t>));
      }
      if (request->user_indices_ptr) {
        snapshot->Capture(request->user_indices_ptr,
                          static_cast<size_t>(count) * sizeof(rex::be<uint32_t>));
      }
      if (request->private_slots_ptr) {
        snapshot->Capture(request->private_slots_ptr,
                          static_cast<size_t>(count) * sizeof(rex::be<uint32_t>));
      }
      break;
    }
    case 0x000B0016: {
      const auto* request = memory->TranslateVirtual<const XGI_SESSION_SEARCH*>(buffer_ptr);
      CaptureSearchInputs(*snapshot, memory, *request);
      break;
    }
    case 0x000B001C: {
      const auto* request = memory->TranslateVirtual<const XGI_SESSION_SEARCH_EX*>(buffer_ptr);
      CaptureSearchInputs(*snapshot, memory, request->search);
      break;
    }
    default:
      break;
  }
  return snapshot;
}

}  // namespace

u32 XMsgInProcessCall_entry(u32 app, u32 message, u32 arg1, u32 arg2) {
  auto result = REX_KERNEL_STATE()->app_manager()->DispatchMessageSync(app, message, arg1, arg2);
  if (result == X_ERROR_NOT_FOUND) {
    REXKRNL_ERROR("XMsgInProcessCall: app {:08X} undefined", app);
  }
  return result;
}

u32 XMsgSystemProcessCall_entry(u32 app, u32 message, u32 buffer, u32 buffer_length) {
  auto result =
      REX_KERNEL_STATE()->app_manager()->DispatchMessageAsync(app, message, buffer, buffer_length);
  if (result == X_ERROR_NOT_FOUND) {
    REXKRNL_ERROR("XMsgSystemProcessCall: app {:08X} undefined", app);
  }
  return result;
}

struct XMSGSTARTIOREQUEST_UNKNOWNARG {
  be<uint32_t> unk_0;
  be<uint32_t> unk_1;
};

X_HRESULT xeXMsgStartIORequestEx(uint32_t app, uint32_t message, uint32_t overlapped_ptr,
                                 uint32_t buffer_ptr, uint32_t buffer_length,
                                 XMSGSTARTIOREQUEST_UNKNOWNARG* unknown) {
  if (overlapped_ptr) {
    auto input_snapshot = CaptureAsyncInputs(app, message, buffer_ptr, buffer_length);
    REX_KERNEL_STATE()->CompleteOverlappedDeferredEx(
        [app, message, buffer_ptr, buffer_length, input_snapshot = std::move(input_snapshot)](
            uint32_t& extended_error, uint32_t& length) -> X_RESULT {
          X_RESULT result = X_E_INVALIDARG;
          if (input_snapshot && input_snapshot->Restore()) {
            result = REX_KERNEL_STATE()->app_manager()->DispatchMessageAsync(
                app, message, buffer_ptr, buffer_length);
          }
          if (result == X_E_NOTFOUND) {
            REXKRNL_ERROR("XMsgStartIORequestEx: app {:08X} undefined", app);
            result = X_E_INVALIDARG;
          }
          extended_error = result;
          length = 0;
          return result;
        },
        overlapped_ptr);
    XThread::SetLastError(0);
    return X_ERROR_IO_PENDING;
  }

  auto result = REX_KERNEL_STATE()->app_manager()->DispatchMessageAsync(app, message, buffer_ptr,
                                                                        buffer_length);
  if (result == X_E_NOTFOUND) {
    REXKRNL_ERROR("XMsgStartIORequestEx: app {:08X} undefined", app);
    result = X_E_INVALIDARG;
    XThread::SetLastError(X_ERROR_NOT_FOUND);
  } else if (result == X_ERROR_SUCCESS) {
    XThread::SetLastError(0);
  }
  return result;
}

u32 XMsgStartIORequestEx_entry(u32 app, u32 message, ppc_ptr_t<XAM_OVERLAPPED> overlapped_ptr,
                               u32 buffer_ptr, u32 buffer_length,
                               ppc_ptr_t<XMSGSTARTIOREQUEST_UNKNOWNARG> unknown_ptr) {
  return xeXMsgStartIORequestEx(app, message, overlapped_ptr.guest_address(), buffer_ptr,
                                buffer_length, unknown_ptr);
}

u32 XMsgStartIORequest_entry(u32 app, u32 message, ppc_ptr_t<XAM_OVERLAPPED> overlapped_ptr,
                             u32 buffer_ptr, u32 buffer_length) {
  return xeXMsgStartIORequestEx(app, message, overlapped_ptr.guest_address(), buffer_ptr,
                                buffer_length, nullptr);
}

u32 XMsgCancelIORequest_entry(ppc_ptr_t<XAM_OVERLAPPED> overlapped_ptr, u32 wait) {
  X_HANDLE event_handle = XOverlappedGetEvent(overlapped_ptr);
  if (event_handle && wait) {
    auto ev = REX_KERNEL_OBJECTS()->LookupObject<XEvent>(event_handle);
    if (ev) {
      ev->Wait(0, 0, true, nullptr);
    }
  }

  return 0;
}

u32 XMsgCompleteIORequest_entry(ppc_ptr_t<XAM_OVERLAPPED> overlapped_ptr, u32 result,
                                u32 extended_error, u32 length) {
  REX_KERNEL_STATE()->CompleteOverlappedImmediateEx(overlapped_ptr.guest_address(), result,
                                                    extended_error, length);
  return X_ERROR_SUCCESS;
}

u32 XamGetOverlappedResult_entry(ppc_ptr_t<XAM_OVERLAPPED> overlapped_ptr, mapped_u32 length_ptr,
                                 u32 unknown) {
  uint32_t result;
  if (overlapped_ptr->result != X_ERROR_IO_PENDING) {
    result = overlapped_ptr->result;
  } else if (!overlapped_ptr->event) {
    result = X_ERROR_IO_INCOMPLETE;
  } else {
    auto ev = REX_KERNEL_OBJECTS()->LookupObject<XEvent>(overlapped_ptr->event);
    result = ev->Wait(3, 1, 0, nullptr);
    if (XSUCCEEDED(result)) {
      result = overlapped_ptr->result;
    } else {
      result = xboxkrnl::xeRtlNtStatusToDosError(result);
    }
  }
  if (XSUCCEEDED(result) && length_ptr) {
    *length_ptr = overlapped_ptr->length;
  }
  return result;
}

}  // namespace xam
}  // namespace kernel
}  // namespace rex

REX_EXPORT(__imp__XMsgInProcessCall, rex::kernel::xam::XMsgInProcessCall_entry)
REX_EXPORT(__imp__XMsgSystemProcessCall, rex::kernel::xam::XMsgSystemProcessCall_entry)
REX_EXPORT(__imp__XMsgStartIORequestEx, rex::kernel::xam::XMsgStartIORequestEx_entry)
REX_EXPORT(__imp__XMsgStartIORequest, rex::kernel::xam::XMsgStartIORequest_entry)
REX_EXPORT(__imp__XMsgCancelIORequest, rex::kernel::xam::XMsgCancelIORequest_entry)
REX_EXPORT(__imp__XMsgCompleteIORequest, rex::kernel::xam::XMsgCompleteIORequest_entry)
REX_EXPORT(__imp__XamGetOverlappedResult, rex::kernel::xam::XamGetOverlappedResult_entry)

REX_EXPORT_STUB(__imp__XMsgAcquireAsyncMessageFromOverlapped);
REX_EXPORT_STUB(__imp__XMsgProcessRequest);
REX_EXPORT_STUB(__imp__XMsgReleaseAsyncMessageToOverlapped);
