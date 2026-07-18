/**
 * @file        ui/overlay/achievements_overlay.cpp
 * @brief       Achievements overlay implementation. See achievements_overlay.h for details.
 *
 * @copyright   Copyright (c) 2026 Rien Gupta <rgupta9@scu.edu>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */
#include <rex/ui/overlay/achievements_overlay.h>

#include <imgui.h>

#include <rex/runtime.h>
#include <rex/ui/immediate_drawer.h>

namespace rex::ui {

AchievementsOverlayDialog::AchievementsOverlayDialog(ImGuiDrawer* imgui_drawer,
                                                     ImmediateDrawer* immediate_drawer,
                                                     rex::Runtime* runtime,
                                                     rex::system::AchievementManager* achievements)
    : ImGuiDialog(imgui_drawer),
      achievements_(achievements),
      icon_cache_(immediate_drawer, runtime) {}

AchievementsOverlayDialog::~AchievementsOverlayDialog() {}

namespace {
// Palette — kept ASCII-only; the bundled overlay font has no em dash / check glyphs.
constexpr ImVec4 kUnlockedTitle{0.45f, 1.00f, 0.55f, 1.00f};  // bright green
constexpr ImVec4 kUnlockedDesc{0.70f, 0.85f, 0.72f, 1.00f};   // soft green
constexpr ImVec4 kLockedTitle{0.78f, 0.80f, 0.84f, 1.00f};    // light grey
constexpr ImVec4 kLockedDesc{0.50f, 0.52f, 0.56f, 1.00f};     // dim grey
constexpr ImVec4 kBadgeGS{1.00f, 0.82f, 0.30f, 1.00f};        // gamerscore gold
constexpr ImVec4 kRowUnlockedBg{0.16f, 0.30f, 0.18f, 0.55f};  // green tint
constexpr ImVec4 kHeaderText{0.60f, 0.85f, 1.00f, 1.00f};     // accent blue
constexpr float kIconSize = 44.0f;
}  // namespace

ImmediateTexture* AchievementsOverlayDialog::GetIcon(
    const rex::system::AchievementInfo& achievement) {
  return icon_cache_.GetIcon(achievement);
}

void AchievementsOverlayDialog::OnDraw(ImGuiIO& io) {
  ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, 40.0f), ImGuiCond_FirstUseEver,
                          ImVec2(0.5f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(640.0f, 560.0f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowBgAlpha(0.92f);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 12.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 6.0f));

  if (ImGui::Begin("Achievements##overlay", nullptr, ImGuiWindowFlags_NoCollapse)) {
    const auto achievements = achievements_->ListAchievements();

    int unlocked_count = 0;
    int total_gs = 0;
    int earned_gs = 0;
    for (const auto& a : achievements) {
      total_gs += static_cast<int>(a.gamerscore);
      if (achievements_->IsUnlocked(a.id)) {
        ++unlocked_count;
        earned_gs += static_cast<int>(a.gamerscore);
      }
    }
    const int total_count = static_cast<int>(achievements.size());

    // ---- Header: summary line + progress bar -------------------------------
    ImGui::PushStyleColor(ImGuiCol_Text, kHeaderText);
    ImGui::Text("%d / %d unlocked", unlocked_count, total_count);
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::TextColored(kBadgeGS, "%dG / %dG", earned_gs, total_gs);

    float frac = total_count > 0 ? static_cast<float>(unlocked_count) / total_count : 0.0f;
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.30f, 0.80f, 0.40f, 1.0f));
    ImGui::ProgressBar(frac, ImVec2(-1.0f, 6.0f), "");
    ImGui::PopStyleColor();

    ImGui::TextDisabled("Unlock progress is saved automatically");
    ImGui::Separator();
    ImGui::Spacing();

    // ---- List --------------------------------------------------------------
    ImGui::BeginChild("##achlist", ImVec2(0.0f, 0.0f), false);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (const auto& a : achievements) {
      const bool is_unlocked = achievements_->IsUnlocked(a.id);
      const std::string& desc = is_unlocked ? a.description : a.unachieved_description;

      ImGui::PushID(static_cast<int>(a.id));

      // Split the draw list so the row band (channel 0) renders behind the
      // text (channel 1); we paint the rect after measuring, then merge.
      draw_list->ChannelsSplit(2);
      draw_list->ChannelsSetCurrent(1);

      const float row_start_y = ImGui::GetCursorScreenPos().y;
      const float pad = 4.0f;
      ImGui::Dummy(ImVec2(0.0f, pad * 0.5f));

      // Icon on the left. Locked icons are dimmed so the state reads clearly.
      ImmediateTexture* icon = GetIcon(a);
      if (icon) {
        ImVec4 tint = is_unlocked ? ImVec4(1, 1, 1, 1) : ImVec4(0.45f, 0.45f, 0.45f, 0.8f);
        ImGui::ImageWithBg(reinterpret_cast<ImTextureID>(icon), ImVec2(kIconSize, kIconSize),
                           ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tint);
      } else {
        ImGui::Dummy(ImVec2(kIconSize, kIconSize));
      }
      ImGui::SameLine();

      // Text block to the right of the icon.
      ImGui::BeginGroup();
      const char* marker = is_unlocked ? "[*]" : "[ ]";
      ImGui::TextColored(is_unlocked ? kUnlockedTitle : kLockedTitle, "%s", marker);
      ImGui::SameLine();
      ImGui::TextColored(kBadgeGS, "%dG", static_cast<int>(a.gamerscore));
      ImGui::SameLine();
      ImGui::TextColored(is_unlocked ? kUnlockedTitle : kLockedTitle, "%s", a.label.c_str());

      ImGui::PushStyleColor(ImGuiCol_Text, is_unlocked ? kUnlockedDesc : kLockedDesc);
      ImGui::TextWrapped("%s", desc.c_str());
      ImGui::PopStyleColor();
      ImGui::EndGroup();

      ImGui::Dummy(ImVec2(0.0f, pad * 0.5f));
      const float row_end_y = ImGui::GetCursorScreenPos().y;

      // Paint the band behind unlocked rows on the lower channel.
      if (is_unlocked) {
        draw_list->ChannelsSetCurrent(0);
        const float x0 = ImGui::GetWindowPos().x + 2.0f;
        const float x1 = x0 + ImGui::GetWindowSize().x - 4.0f;
        draw_list->AddRectFilled(ImVec2(x0, row_start_y), ImVec2(x1, row_end_y),
                                 ImGui::GetColorU32(kRowUnlockedBg), 3.0f);
      }
      draw_list->ChannelsMerge();

      ImGui::Separator();
      ImGui::PopID();
    }
    ImGui::EndChild();
  }
  ImGui::End();

  ImGui::PopStyleVar(2);
}

}  // namespace rex::ui
