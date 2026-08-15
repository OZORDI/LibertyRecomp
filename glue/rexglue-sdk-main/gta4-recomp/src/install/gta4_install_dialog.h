#pragma once

#include <atomic>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <rex/ui/imgui_dialog.h>

#include "gta4_installer.h"

namespace gta4::install {

class InstallDialog final : public rex::ui::ImGuiDialog {
 public:
  using CompleteCallback = std::function<void()>;
  using CancelCallback = std::function<void()>;

  InstallDialog(rex::ui::ImGuiDrawer* drawer, std::filesystem::path install_root, bool dlc_only,
                CompleteCallback complete, CancelCallback cancel);

 protected:
  void OnClose() override;
  void OnDraw(ImGuiIO& io) override;

 private:
  enum class State {
    kSelecting,
    kInstalling,
    kInstalled,
    kFailed,
  };

  enum class PickerTarget {
    kGame,
    kUpdate,
    kTlad,
    kTbogt,
  };

  struct PickerState {
    std::mutex mutex;
    std::filesystem::path game;
    std::filesystem::path update;
    std::filesystem::path tlad;
    std::filesystem::path tbogt;
    std::string error;
  };

  void ShowFilePicker(PickerTarget target);
  void ShowFolderPicker(PickerTarget target);
  void StartInstall();
  void FinishInstallIfNeeded();
  void DrawSourceRow(const char* label, PickerTarget target, const std::filesystem::path& value,
                     bool required);
  void AssignPickedPath(PickerTarget target, std::filesystem::path path);
  std::filesystem::path PathFor(PickerTarget target) const;

  std::filesystem::path install_root_;
  bool dlc_only_ = false;
  CompleteCallback complete_;
  CancelCallback cancel_;
  std::shared_ptr<PickerState> picker_state_;
  Progress progress_;
  Result result_;
  std::thread install_thread_;
  std::atomic<bool> install_done_{false};
  State state_ = State::kSelecting;
  int completion_frames_ = -1;
};

}  // namespace gta4::install
