#include "input/text_chat_dialog.h"

#include "input/text_chat_team.h"

#include <algorithm>
#include <span>

#include <imgui.h>

namespace gta4::input {

using rex::system::xam::TextChatChannel;
using rex::system::xam::TextChatMessage;

TextChatDialog::TextChatDialog(rex::ui::ImGuiDrawer* drawer,
                               std::function<void(bool)> set_input_capture)
    : ImGuiDialog(drawer),
      set_input_capture_(std::move(set_input_capture)) {}

TextChatDialog::~TextChatDialog() { Stop(); }

void TextChatDialog::AttachLive(
    rex::system::xam::LiveCompatibilityRuntime* live) {
  live_ = live;
  transport_ = live ? live->text_chat_transport() : nullptr;
}

void TextChatDialog::RequestOpen(TextChatChannel channel) {
  requested_channel_.store(channel == TextChatChannel::kTeam ? 2u : 1u,
                           std::memory_order_release);
  set_input_capture_(true);
  RequestRepaint();
}

void TextChatDialog::Stop() {
  requested_channel_.store(0, std::memory_order_release);
  FinishComposition();
  if (transport_) transport_->CloseTextChat();
  configured_session_id_ = 0;
  transport_ = nullptr;
  live_ = nullptr;
}

void TextChatDialog::SynchronizeSession() {
  const uint64_t session_id =
      live_ && live_->available() ? live_->active_session_id() : 0;
  if (!transport_ || !transport_->ready() || !session_id) {
    if (configured_session_id_ && transport_) transport_->CloseTextChat();
    configured_session_id_ = 0;
    return;
  }
  if (session_id != configured_session_id_) {
    if (transport_->Configure(session_id)) {
      configured_session_id_ = session_id;
      history_.clear();
      status_.clear();
    } else {
      configured_session_id_ = 0;
      status_ = "Text chat is unavailable";
    }
  }
}

void TextChatDialog::DrainReceived() {
  if (!transport_ || !configured_session_id_) return;
  auto messages = transport_->ReceiveMessages(kReceiveBatchMessages);
  for (auto& message : messages) {
    if (message.session_id != configured_session_id_) continue;
    if (history_.size() >= kHistoryMessages) history_.pop_front();
    history_.push_back(std::move(message));
  }
}

void TextChatDialog::FinishComposition() {
  composing_ = false;
  focus_input_ = false;
  input_.fill('\0');
  set_input_capture_(false);
}

bool TextChatDialog::SendComposition() {
  if (!transport_ || !configured_session_id_) {
    status_ = "Join an online session to use text chat";
    return false;
  }

  std::span<const uint64_t> targets;
  std::vector<uint64_t> team_targets;
  if (channel_ == TextChatChannel::kTeam) {
    auto resolved = FindTeamChatTargets(live_);
    if (!resolved || resolved->empty()) {
      status_ = "Team Chat has no valid teammates";
      return false;
    }
    team_targets = std::move(*resolved);
    targets = team_targets;
  }

  const std::string text(input_.data());
  const uint32_t sequence = next_sequence_;
  if (!transport_->Send(channel_, targets, sequence, text)) {
    status_ = "Text chat message was not accepted";
    return false;
  }
  ++next_sequence_;
  TextChatMessage local{
      .source_xuid = live_->identity().xuid,
      .session_id = configured_session_id_,
      .sequence = sequence,
      .channel = channel_,
      .player_name = live_->identity().player_name,
      .text = text,
  };
  if (history_.size() >= kHistoryMessages) history_.pop_front();
  history_.push_back(std::move(local));
  status_.clear();
  return true;
}

void TextChatDialog::OnDraw(ImGuiIO& io) {
  SynchronizeSession();
  DrainReceived();

  const uint32_t request = requested_channel_.exchange(0, std::memory_order_acq_rel);
  if (request) {
    channel_ = request == 2 ? TextChatChannel::kTeam : TextChatChannel::kAll;
    input_.fill('\0');
    io.ClearInputKeys();
    composing_ = true;
    focus_input_ = true;
  }

  if (history_.empty() && status_.empty() && !composing_) return;

  ImGui::SetNextWindowPos(ImVec2(20.0f, io.DisplaySize.y), ImGuiCond_Always,
                          ImVec2(0.0f, 1.0f));
  ImGui::SetNextWindowBgAlpha(0.70f);
  constexpr ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav;
  if (!ImGui::Begin("##gta4_text_chat", nullptr, flags)) {
    ImGui::End();
    return;
  }

  for (const auto& message : history_) {
    const char* label = message.channel == TextChatChannel::kTeam ? "Team" : "All";
    ImGui::TextWrapped("[%s] %s: %s", label, message.player_name.c_str(),
                       message.text.c_str());
  }
  if (!status_.empty()) {
    ImGui::TextUnformatted(status_.c_str());
  }

  if (composing_) {
    ImGui::Separator();
    ImGui::TextUnformatted(channel_ == TextChatChannel::kTeam ? "Team Chat" : "All Chat");
    if (focus_input_) {
      ImGui::SetKeyboardFocusHere();
      focus_input_ = false;
    }
    const bool submitted = ImGui::InputText(
        "##message", input_.data(), input_.size(),
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
    if (submitted && SendComposition()) {
      FinishComposition();
    } else if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      FinishComposition();
    }
  }
  ImGui::End();
}

}  // namespace gta4::input
