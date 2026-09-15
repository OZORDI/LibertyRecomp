#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <deque>
#include <functional>
#include <string>

#include <rex/system/xam/live_compatibility.h>
#include <rex/ui/imgui_dialog.h>

namespace gta4::input {

class TextChatDialog final : public rex::ui::ImGuiDialog {
 public:
  TextChatDialog(rex::ui::ImGuiDrawer* drawer,
                 std::function<void(bool)> set_input_capture);
  ~TextChatDialog() override;

  void AttachLive(rex::system::xam::LiveCompatibilityRuntime* live);
  void RequestOpen(rex::system::xam::TextChatChannel channel);
  void Stop();
  bool WantsContinuousRepaint() const override {
    return composing_ || requested_channel_.load(std::memory_order_acquire) != 0;
  }

 protected:
  void OnDraw(ImGuiIO& io) override;

 private:
  void SynchronizeSession();
  void DrainReceived();
  void FinishComposition();
  bool SendComposition();

  static constexpr size_t kInputBufferBytes = 257;
  static constexpr size_t kHistoryMessages = 64;
  static constexpr uint32_t kReceiveBatchMessages = 128;

  std::function<void(bool)> set_input_capture_;
  rex::system::xam::LiveCompatibilityRuntime* live_ = nullptr;
  rex::system::xam::ITextChatTransport* transport_ = nullptr;
  uint64_t configured_session_id_ = 0;
  uint32_t next_sequence_ = 0;
  std::atomic<uint32_t> requested_channel_{0};
  rex::system::xam::TextChatChannel channel_ =
      rex::system::xam::TextChatChannel::kAll;
  std::array<char, kInputBufferBytes> input_{};
  std::deque<rex::system::xam::TextChatMessage> history_;
  std::string status_;
  bool composing_ = false;
  bool focus_input_ = false;
};

}  // namespace gta4::input
