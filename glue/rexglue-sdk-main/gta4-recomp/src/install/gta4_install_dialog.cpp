#include "gta4_install_dialog.h"

#include <algorithm>
#include <utility>

#include <SDL3/SDL_dialog.h>
#include <imgui.h>

#include <rex/filesystem.h>

namespace gta4::install {
namespace {

struct PickerRequest {
  std::function<void(std::filesystem::path, std::string)> complete;
};

void SDLCALL OnPathPicked(void* userdata, const char* const* file_list, int) {
  std::unique_ptr<PickerRequest> request(static_cast<PickerRequest*>(userdata));
  if (!request || !request->complete) {
    return;
  }
  if (!file_list) {
    request->complete({}, SDL_GetError());
    return;
  }
  if (!file_list[0]) {
    request->complete({}, {});
    return;
  }
  request->complete(rex::to_path(file_list[0]), {});
}

constexpr SDL_DialogFileFilter kGameFilters[] = {
    {"Xbox 360 disc image", "iso"},
    {"All supported files", "*"},
};

constexpr SDL_DialogFileFilter kUpdateFilters[] = {
    {"Xbox title update", "xexp"},
    {"Xbox content package", "*"},
};

constexpr SDL_DialogFileFilter kDlcFilters[] = {
    {"Xbox content package", "*"},
};

}  // namespace

InstallDialog::InstallDialog(rex::ui::ImGuiDrawer* drawer, std::filesystem::path install_root,
                             bool dlc_only, CompleteCallback complete, CancelCallback cancel)
    : ImGuiDialog(drawer),
      install_root_(std::move(install_root)),
      dlc_only_(dlc_only),
      complete_(std::move(complete)),
      cancel_(std::move(cancel)),
      picker_state_(std::make_shared<PickerState>()) {}

void InstallDialog::OnClose() {
  progress_.cancel_requested = true;
  if (install_thread_.joinable()) {
    install_thread_.join();
  }
}

void InstallDialog::AssignPickedPath(PickerTarget target, std::filesystem::path path) {
  std::lock_guard lock(picker_state_->mutex);
  switch (target) {
    case PickerTarget::kGame:
      picker_state_->game = std::move(path);
      break;
    case PickerTarget::kUpdate:
      picker_state_->update = std::move(path);
      break;
    case PickerTarget::kTlad:
      picker_state_->tlad = std::move(path);
      break;
    case PickerTarget::kTbogt:
      picker_state_->tbogt = std::move(path);
      break;
  }
}

std::filesystem::path InstallDialog::PathFor(PickerTarget target) const {
  std::lock_guard lock(picker_state_->mutex);
  switch (target) {
    case PickerTarget::kGame:
      return picker_state_->game;
    case PickerTarget::kUpdate:
      return picker_state_->update;
    case PickerTarget::kTlad:
      return picker_state_->tlad;
    case PickerTarget::kTbogt:
      return picker_state_->tbogt;
  }
  return {};
}

void InstallDialog::ShowFilePicker(PickerTarget target) {
  auto weak_state = std::weak_ptr<PickerState>(picker_state_);
  auto* request =
      new PickerRequest{[weak_state, target](std::filesystem::path path, std::string error) {
        auto state = weak_state.lock();
        if (!state) {
          return;
        }
        std::lock_guard lock(state->mutex);
        if (!error.empty()) {
          state->error = std::move(error);
          return;
        }
        if (path.empty()) {
          return;
        }
        state->error.clear();
        switch (target) {
          case PickerTarget::kGame:
            state->game = std::move(path);
            break;
          case PickerTarget::kUpdate:
            state->update = std::move(path);
            break;
          case PickerTarget::kTlad:
            state->tlad = std::move(path);
            break;
          case PickerTarget::kTbogt:
            state->tbogt = std::move(path);
            break;
        }
      }};

  const SDL_DialogFileFilter* filters = kDlcFilters;
  int filter_count = static_cast<int>(std::size(kDlcFilters));
  if (target == PickerTarget::kGame) {
    filters = kGameFilters;
    filter_count = static_cast<int>(std::size(kGameFilters));
  } else if (target == PickerTarget::kUpdate) {
    filters = kUpdateFilters;
    filter_count = static_cast<int>(std::size(kUpdateFilters));
  }
  SDL_ShowOpenFileDialog(OnPathPicked, request, nullptr, filters, filter_count, nullptr, false);
}

void InstallDialog::ShowFolderPicker(PickerTarget target) {
  auto weak_state = std::weak_ptr<PickerState>(picker_state_);
  auto* request =
      new PickerRequest{[weak_state, target](std::filesystem::path path, std::string error) {
        auto state = weak_state.lock();
        if (!state) {
          return;
        }
        std::lock_guard lock(state->mutex);
        if (!error.empty()) {
          state->error = std::move(error);
          return;
        }
        if (path.empty()) {
          return;
        }
        state->error.clear();
        switch (target) {
          case PickerTarget::kGame:
            state->game = std::move(path);
            break;
          case PickerTarget::kUpdate:
            state->update = std::move(path);
            break;
          case PickerTarget::kTlad:
            state->tlad = std::move(path);
            break;
          case PickerTarget::kTbogt:
            state->tbogt = std::move(path);
            break;
        }
      }};
  SDL_ShowOpenFolderDialog(OnPathPicked, request, nullptr, nullptr, false);
}

void InstallDialog::DrawSourceRow(const char* label, PickerTarget target,
                                  const std::filesystem::path& value, bool required) {
  ImGui::PushID(label);
  ImGui::Text("%s%s", label, required ? " *" : "");
  ImGui::SameLine();
  ImGui::TextDisabled("%s", value.empty() ? "Not selected" : value.string().c_str());
  if (ImGui::Button("Select File")) {
    ShowFilePicker(target);
  }
  ImGui::SameLine();
  if (ImGui::Button("Select Folder")) {
    ShowFolderPicker(target);
  }
  if (!value.empty()) {
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
      AssignPickedPath(target, {});
    }
  }
  ImGui::PopID();
}

void InstallDialog::StartInstall() {
  if (install_thread_.joinable()) {
    install_thread_.join();
  }

  Selection selection;
  if (!dlc_only_) {
    selection.game_source = PathFor(PickerTarget::kGame);
    selection.update_source = PathFor(PickerTarget::kUpdate);
  }
  if (auto path = PathFor(PickerTarget::kTlad); !path.empty()) {
    selection.dlc_sources.push_back({Episode::kTlad, std::move(path)});
  }
  if (auto path = PathFor(PickerTarget::kTbogt); !path.empty()) {
    selection.dlc_sources.push_back({Episode::kTbogt, std::move(path)});
  }

  progress_.copied_bytes = 0;
  progress_.total_bytes = 0;
  progress_.cancel_requested = false;
  result_ = {};
  install_done_ = false;
  state_ = State::kInstalling;
  install_thread_ = std::thread([this, selection = std::move(selection)]() {
    result_ = Install(selection, install_root_, progress_);
    install_done_.store(true, std::memory_order_release);
  });
}

void InstallDialog::FinishInstallIfNeeded() {
  if (state_ != State::kInstalling || !install_done_.load(std::memory_order_acquire)) {
    return;
  }
  if (install_thread_.joinable()) {
    install_thread_.join();
  }
  state_ = result_.success ? State::kInstalled : State::kFailed;
}

void InstallDialog::OnDraw(ImGuiIO& io) {
  FinishInstallIfNeeded();

  if (completion_frames_ >= 0) {
    if (completion_frames_ == 0) {
      completion_frames_ = -1;
      auto complete = std::move(complete_);
      Close();
      if (complete) {
        complete();
      }
      return;
    }
    --completion_frames_;
  }

  ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.10f, io.DisplaySize.y * 0.08f),
                          ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x * 0.80f, io.DisplaySize.y * 0.84f),
                           ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.98f);
  constexpr ImGuiWindowFlags kWindowFlags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
  if (!ImGui::Begin("Liberty Recompiled Setup##installer", nullptr, kWindowFlags)) {
    ImGui::End();
    return;
  }

  ImGui::SetWindowFontScale(1.45f);
  ImGui::TextUnformatted(dlc_only_ ? "INSTALL EPISODES" : "INSTALL LIBERTY RECOMPILED");
  ImGui::SetWindowFontScale(1.0f);
  ImGui::Separator();
  ImGui::Spacing();

  if (state_ == State::kSelecting || state_ == State::kFailed) {
    if (!dlc_only_) {
      ImGui::TextWrapped(
          "Select your legally obtained Xbox 360 GTA IV source and the v8 (0.0.8.5) "
          "title update. The update may be an STFS package or raw default.xexp.");
      ImGui::Spacing();
      DrawSourceRow("Base game", PickerTarget::kGame, PathFor(PickerTarget::kGame), true);
      ImGui::Spacing();
      DrawSourceRow("Title update v8", PickerTarget::kUpdate, PathFor(PickerTarget::kUpdate), true);
      ImGui::Spacing();
      ImGui::Separator();
    } else {
      ImGui::TextWrapped(
          "Add either or both installed episodes to the existing GTA IV installation.");
    }

    ImGui::Spacing();
    DrawSourceRow("The Lost and Damned", PickerTarget::kTlad, PathFor(PickerTarget::kTlad), false);
    ImGui::Spacing();
    DrawSourceRow("The Ballad of Gay Tony", PickerTarget::kTbogt, PathFor(PickerTarget::kTbogt),
                  false);
    ImGui::Spacing();
    ImGui::TextDisabled("Install directory: %s", install_root_.string().c_str());

    std::string picker_error;
    {
      std::lock_guard lock(picker_state_->mutex);
      picker_error = picker_state_->error;
    }
    if (!picker_error.empty()) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.35f, 0.35f, 1.0f));
      ImGui::TextWrapped("File picker error: %s", picker_error.c_str());
      ImGui::PopStyleColor();
    }
    if (state_ == State::kFailed && !result_.error.empty()) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.35f, 0.35f, 1.0f));
      ImGui::TextWrapped("Installation failed: %s", result_.error.c_str());
      ImGui::PopStyleColor();
    }

    const bool has_game = dlc_only_ || !PathFor(PickerTarget::kGame).empty();
    const bool has_update = dlc_only_ || !PathFor(PickerTarget::kUpdate).empty();
    const bool has_dlc =
        !PathFor(PickerTarget::kTlad).empty() || !PathFor(PickerTarget::kTbogt).empty();
    const bool may_install = has_game && has_update && (!dlc_only_ || has_dlc);
    ImGui::Spacing();
    ImGui::BeginDisabled(!may_install);
    if (ImGui::Button(state_ == State::kFailed ? "Retry Installation" : "Install",
                      ImVec2(190, 0))) {
      StartInstall();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      auto cancel = std::move(cancel_);
      Close();
      if (cancel) {
        cancel();
      }
    }
  } else if (state_ == State::kInstalling) {
    ImGui::TextWrapped("Validating, extracting, and publishing the installation...");
    const uint64_t copied = progress_.copied_bytes.load(std::memory_order_relaxed);
    const uint64_t total = progress_.total_bytes.load(std::memory_order_relaxed);
    const float fraction =
        total == 0 ? 0.0f
                   : std::clamp(static_cast<float>(copied) / static_cast<float>(total), 0.0f, 1.0f);
    ImGui::ProgressBar(fraction, ImVec2(-1.0f, 0.0f));
    ImGui::Text("Copied %llu of %llu bytes", static_cast<unsigned long long>(copied),
                static_cast<unsigned long long>(total));
    ImGui::Spacing();
    if (ImGui::Button("Cancel Installation")) {
      progress_.cancel_requested = true;
    }
  } else {
    ImGui::TextWrapped(
        "Installation and integrity validation completed successfully. The game can now start.");
    ImGui::Spacing();
    if (completion_frames_ < 0 && ImGui::Button("Start Game", ImVec2(190, 0))) {
      completion_frames_ = 1;
    }
  }

  ImGui::End();
}

}  // namespace gta4::install
