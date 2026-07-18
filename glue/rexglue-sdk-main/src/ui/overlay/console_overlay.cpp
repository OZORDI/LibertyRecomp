/**
 * @file        ui/overlay/console_overlay.cpp
 *
 * @brief       Console overlay implementation. See console_overlay.h for details.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */
#include <rex/ui/overlay/console_overlay.h>
#include <rex/cvar.h>
#include <imgui.h>
#include <algorithm>
#include <string>

namespace rex::ui {

static ImVec4 LevelColor(spdlog::level::level_enum level) {
  // todo(tomc): make these cvar driven
  switch (level) {
    case spdlog::level::trace:
      return {0.5f, 0.5f, 0.5f, 1.0f};  // grey
    case spdlog::level::debug:
      return {0.4f, 0.9f, 0.9f, 1.0f};  // cyan
    case spdlog::level::info:
      return {1.0f, 1.0f, 1.0f, 1.0f};  // white
    case spdlog::level::warn:
      return {1.0f, 1.0f, 0.0f, 1.0f};  // yellow
    case spdlog::level::err:
      return {1.0f, 0.4f, 0.4f, 1.0f};  // red
    case spdlog::level::critical:
      return {1.0f, 0.0f, 0.0f, 1.0f};  // bright red
    default:
      return {1.0f, 1.0f, 1.0f, 1.0f};
  }
}

ConsoleDialog::ConsoleDialog(ImGuiDrawer* imgui_drawer, std::shared_ptr<rex::LogCaptureSink> sink)
    : ImGuiDialog(imgui_drawer), sink_(std::move(sink)) {}

ConsoleDialog::~ConsoleDialog() {}

void ConsoleDialog::RefreshCategories() {
  for (auto& entry : entries_) {
    if (entry.category.empty() || entry.category == "console")
      continue;
    if (known_categories_.insert(entry.category).second) {
      // New category discovered - enable by default.
      category_filter_[entry.category] = true;
    }
  }
}

int ConsoleDialog::InputTextCallback(ImGuiInputTextCallbackData* data) {
  auto* self = static_cast<ConsoleDialog*>(data->UserData);
  switch (data->EventFlag) {
    case ImGuiInputTextFlags_CallbackAlways:
      self->UpdateCompletionCandidates(data->Buf, data->BufTextLen);
      break;
    case ImGuiInputTextFlags_CallbackCompletion:
      self->ApplyCompletion(data);
      break;
    case ImGuiInputTextFlags_CallbackHistory:
      self->HandleHistoryOrCompletionNav(data);
      break;
    default:
      break;
  }
  return 0;
}

void ConsoleDialog::UpdateCompletionCandidates(const char* buf, int len) {
  std::string_view text(buf, static_cast<size_t>(len));
  // Complete the command/cvar name only (the first token). Once a space is
  // typed the user is editing arguments, so close the popup.
  if (text.empty() || text.find(' ') != std::string_view::npos) {
    completion_candidates_.clear();
    completion_open_ = false;
    completion_index_ = -1;
    return;
  }
  std::vector<std::string> matches;
  for (auto& name : rex::cvar::ListFlags()) {
    if (name.size() >= text.size() && std::string_view(name).substr(0, text.size()) == text) {
      matches.push_back(name);
    }
  }
  if (matches != completion_candidates_) {
    completion_candidates_ = std::move(matches);
    completion_index_ = -1;
  }
  completion_open_ = !completion_candidates_.empty();
}

void ConsoleDialog::ApplyCompletion(ImGuiInputTextCallbackData* data) {
  if (completion_candidates_.empty())
    return;
  std::string completion;
  bool full = false;
  if (completion_index_ >= 0 &&
      completion_index_ < static_cast<int>(completion_candidates_.size())) {
    completion = completion_candidates_[completion_index_];
    full = true;
  } else if (completion_candidates_.size() == 1) {
    completion = completion_candidates_[0];
    full = true;
  } else {
    // Longest common prefix of all candidates.
    completion = completion_candidates_[0];
    for (size_t i = 1; i < completion_candidates_.size(); ++i) {
      const std::string& cand = completion_candidates_[i];
      size_t j = 0;
      while (j < completion.size() && j < cand.size() && completion[j] == cand[j])
        ++j;
      completion.resize(j);
    }
  }
  data->DeleteChars(0, data->BufTextLen);
  data->InsertChars(0, completion.c_str());
  if (full) {
    data->InsertChars(data->CursorPos, " ");
    completion_candidates_.clear();
    completion_open_ = false;
    completion_index_ = -1;
  }
}

void ConsoleDialog::HandleHistoryOrCompletionNav(ImGuiInputTextCallbackData* data) {
  // When the completion popup is open, arrows move the selection.
  if (completion_open_ && !completion_candidates_.empty()) {
    const int count = static_cast<int>(completion_candidates_.size());
    if (data->EventKey == ImGuiKey_UpArrow) {
      completion_index_ = (completion_index_ <= 0) ? count - 1 : completion_index_ - 1;
    } else if (data->EventKey == ImGuiKey_DownArrow) {
      completion_index_ = (completion_index_ + 1) % count;
    }
    return;
  }
  // Otherwise: command history.
  const int prev = history_pos_;
  if (data->EventKey == ImGuiKey_UpArrow) {
    if (history_pos_ == -1) {
      history_pos_ = static_cast<int>(history_.size()) - 1;
    } else if (history_pos_ > 0) {
      --history_pos_;
    }
  } else if (data->EventKey == ImGuiKey_DownArrow) {
    if (history_pos_ != -1) {
      if (++history_pos_ >= static_cast<int>(history_.size())) {
        history_pos_ = -1;
      }
    }
  }
  if (prev != history_pos_) {
    const char* hist = (history_pos_ >= 0) ? history_[history_pos_].c_str() : "";
    data->DeleteChars(0, data->BufTextLen);
    data->InsertChars(0, hist);
  }
}

void ConsoleDialog::ExecuteCommand(std::string_view cmd) {
  // Trim whitespace.
  while (!cmd.empty() && cmd.front() == ' ')
    cmd.remove_prefix(1);
  while (!cmd.empty() && cmd.back() == ' ')
    cmd.remove_suffix(1);
  if (cmd.empty())
    return;

  // Record in history.
  if (history_.empty() || history_.back() != cmd) {
    if (history_.size() >= kMaxHistory)
      history_.pop_front();
    history_.push_back(std::string(cmd));
  }
  history_pos_ = -1;

  if (cmd == "help" || cmd == "?") {
    auto names = rex::cvar::ListFlags();
    std::sort(names.begin(), names.end());
    for (auto& n : names) {
      const auto* info = rex::cvar::GetFlagInfo(n);
      std::string line = "  " + n;
      if (info)
        line += " = " + info->getter() + "  (" + info->description + ")";
      local_entries_.push_back({spdlog::level::info, "console", line});
    }
    return;
  }

  // Split on first space into name + args.
  auto sep = cmd.find(' ');
  std::string name(sep == std::string_view::npos ? cmd : cmd.substr(0, sep));
  std::string args;
  if (sep != std::string_view::npos) {
    std::string_view rest = cmd.substr(sep + 1);
    while (!rest.empty() && rest.front() == ' ')
      rest.remove_prefix(1);
    args = std::string(rest);
  }

  const auto* info = rex::cvar::GetFlagInfo(name);

  // Command dispatch takes priority over get/set.
  if (info && info->type == rex::cvar::FlagType::Command) {
    rex::cvar::InvokeCommand(name, args);
    local_entries_.push_back(
        {spdlog::level::info, "console", "[console] > " + name + (args.empty() ? "" : " " + args)});
    scroll_to_bottom_ = true;
    return;
  }

  if (sep == std::string_view::npos) {
    // No args: treat as "get" - show current value.
    std::string val = rex::cvar::GetFlagByName(name);
    if (val.empty() && !info) {
      local_entries_.push_back({spdlog::level::warn, "console", "[console] unknown cvar: " + name});
    } else {
      local_entries_.push_back({spdlog::level::info, "console", "[console] " + name + " = " + val});
    }
    return;
  }

  // Has args, non-command: set.
  if (rex::cvar::SetFlagByName(name, args)) {
    local_entries_.push_back({spdlog::level::info, "console", "[console] " + name + " = " + args});
  } else {
    local_entries_.push_back({spdlog::level::warn, "console", "[console] unknown cvar: " + name});
  }
  scroll_to_bottom_ = true;
}

void ConsoleDialog::OnDraw(ImGuiIO& io) {
  // Refresh entries if sink has new data.
  if (sink_) {
    uint64_t gen = sink_->generation();
    if (gen != last_generation_) {
      sink_->CopyEntries(entries_);
      // Re-append console-local entries (command feedback) that aren't in the sink.
      entries_.insert(entries_.end(), local_entries_.begin(), local_entries_.end());
      last_generation_ = gen;
      RefreshCategories();
    }
  }

  if (console_height_ <= 0.0f)
    console_height_ = io.DisplaySize.y * 0.45f;
  const float min_height = ImGui::GetFrameHeightWithSpacing() * 3.0f;
  console_height_ = std::clamp(console_height_, min_height, io.DisplaySize.y);

  ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.y - console_height_), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, console_height_), ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.80f);

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                           ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

  if (!ImGui::Begin("Console##rex", nullptr, flags)) {
    ImGui::End();
    return;
  }

  // Drag handle along the top edge to resize the console vertically.
  ImGui::InvisibleButton("##resize_handle", ImVec2(-1.0f, 4.0f));
  if (ImGui::IsItemHovered() || ImGui::IsItemActive())
    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
  if (ImGui::IsItemActive())
    console_height_ = std::clamp(console_height_ - io.MouseDelta.y, min_height, io.DisplaySize.y);

  // --- Filter bar ---
  static const char* kLevelNames[] = {"trace", "debug", "info", "warn", "error", "critical"};
  ImGui::Text("Level:");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(80.0f);
  ImGui::Combo("##lvl", &min_level_, kLevelNames, 6);
  ImGui::SameLine();
  ImGui::Text("Categories:");
  ImGui::SameLine();
  int cat_idx = 0;
  for (auto& [cat_name, enabled] : category_filter_) {
    ImGui::Checkbox(cat_name.c_str(), &enabled);
    if (++cat_idx < static_cast<int>(category_filter_.size()))
      ImGui::SameLine();
  }

  // --- Log area ---
  const float input_height = ImGui::GetFrameHeightWithSpacing() + 4.0f;
  ImGui::BeginChild("##log", ImVec2(0, -input_height), false, ImGuiWindowFlags_HorizontalScrollbar);

  bool at_bottom = (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 2.0f);

  for (auto& entry : entries_) {
    // Level filter.
    if (static_cast<int>(entry.level) < min_level_)
      continue;
    // Category filter.
    bool show_cat = false;
    auto it = category_filter_.find(entry.category);
    if (it != category_filter_.end()) {
      show_cat = it->second;
    }
    // "console" pseudo-category always shown.
    if (entry.category == "console")
      show_cat = true;
    if (!show_cat)
      continue;

    ImGui::PushStyleColor(ImGuiCol_Text, LevelColor(entry.level));
    ImGui::TextUnformatted(entry.text.c_str());
    ImGui::PopStyleColor();
  }

  if (scroll_to_bottom_ || at_bottom) {
    ImGui::SetScrollHereY(1.0f);
    scroll_to_bottom_ = false;
  }
  ImGui::EndChild();

  // --- Command input ---
  ImGui::Separator();
  bool submit = false;
  ImGuiInputTextFlags input_flags =
      ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory |
      ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackAlways;
  ImGui::SetNextItemWidth(-1.0f);
  if (focus_input_next_frame_) {
    ImGui::SetKeyboardFocusHere();
    focus_input_next_frame_ = false;
  }
  if (ImGui::InputText("##cmd", input_buf_, sizeof(input_buf_), input_flags, InputTextCallback,
                       this)) {
    submit = true;
  }
  const ImVec2 input_min = ImGui::GetItemRectMin();
  const ImVec2 input_max = ImGui::GetItemRectMax();

  // Close the completion popup whenever the input loses keyboard focus. The
  // popup is a separate window; without this it could stay open and (if it ever
  // grabbed focus) block the input until an app restart.
  if (!ImGui::IsItemFocused()) {
    completion_open_ = false;
    completion_candidates_.clear();
    completion_index_ = -1;
  }

  if (submit && input_buf_[0] != '\0') {
    ExecuteCommand(input_buf_);
    input_buf_[0] = '\0';
    completion_candidates_.clear();
    completion_open_ = false;
    completion_index_ = -1;
    ImGui::SetKeyboardFocusHere(-1);
  }

  ImGui::End();

  // Completion popup: anchored to the input's top-left, growing upward.
  if (completion_open_ && !completion_candidates_.empty()) {
    const float width = input_max.x - input_min.x;
    ImGui::SetNextWindowPos(ImVec2(input_min.x, input_min.y), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(width, 0.0f), ImVec2(width, 200.0f));
    ImGui::SetNextWindowBgAlpha(0.95f);
    // NoInputs makes the popup purely visual: it can never be hovered, clicked,
    // or focused, so it cannot steal focus from the input. Navigation is driven
    // entirely by the input's Tab/Up/Down callbacks.
    ImGuiWindowFlags popup_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoInputs |
                                   ImGuiWindowFlags_NoNavInputs;
    if (ImGui::Begin("##rex_completions", nullptr, popup_flags)) {
      for (int i = 0; i < static_cast<int>(completion_candidates_.size()); ++i) {
        const bool selected = (i == completion_index_);
        ImGui::Selectable(completion_candidates_[i].c_str(), selected);
        if (selected)
          ImGui::SetScrollHereY();
      }
    }
    ImGui::End();
  }
}

}  // namespace rex::ui
