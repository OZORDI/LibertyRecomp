#include "input/context_touch_overlay.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <vector>
#include <rex/input/absolute_pointer.h>
#include <rex/ui/image_decode.h>

#include "input/context_touch_settings.h"

namespace gta4::input {

ContextTouchOverlay::ContextTouchOverlay(rex::ui::ImGuiDrawer* drawer,
                                         rex::ui::ImmediateDrawer* immediate,
                                         std::filesystem::path icon_directory,
                                         rex::ui::Window* window)
    : ImGuiDialog(drawer), immediate_(immediate), icon_directory_(std::move(icon_directory)) {
  if (window) editor_mouse_ = std::make_unique<ContextTouchEditorMouse>(window);
}

void ContextTouchOverlay::SetIconResolver(ContextTouchIconResolver resolver, void* context) noexcept {
  icon_resolver_ = resolver;
  icon_context_ = context;
}

bool ContextTouchOverlay::ResolveIcon(void* context, std::string_view id, ContextTouchIcon* icon) {
  auto& self = *static_cast<ContextTouchOverlay*>(context);
  if (!icon || !self.immediate_ || self.icon_directory_.empty() || id.empty() ||
      !std::all_of(id.begin(), id.end(), [](unsigned char c) { return std::isalnum(c) || c == '_'; })) return false;
  const std::string key(id);
  auto found = self.icons_.find(key);
  if (found == self.icons_.end()) {
    if (self.icons_.size() >= 256) return false;
    std::unique_ptr<rex::ui::ImmediateTexture> texture;
    std::ifstream file(self.icon_directory_ / (key + ".png"), std::ios::binary | std::ios::ate);
    if (file) {
      const auto length = file.tellg();
      if (length > 0 && length <= 4194304) {
        std::vector<uint8_t> bytes(static_cast<size_t>(length));
        file.seekg(0);
        if (file.read(reinterpret_cast<char*>(bytes.data()), length)) {
          int width = 0, height = 0;
          const auto rgba = rex::ui::DecodeImageRGBA(bytes.data(), bytes.size(), width, height, 1024, 1024);
          if (!rgba.empty() && width > 0 && height > 0 &&
              rgba.size() <= 33554432 - self.decoded_icon_bytes_) {
            texture = self.immediate_->CreateTexture(static_cast<uint32_t>(width),
                static_cast<uint32_t>(height), rex::ui::ImmediateTextureFilter::kLinear, false, rgba.data());
            if (texture) self.decoded_icon_bytes_ += rgba.size();
          }
        }
      }
    }
    found = self.icons_.emplace(key, std::move(texture)).first;
  }
  if (!found->second) return false;
  icon->texture = static_cast<ImTextureID>(reinterpret_cast<uintptr_t>(found->second.get()));
  icon->aspect = float(found->second->width) / float(found->second->height);
  return true;
}

void ContextTouchOverlay::OnDraw(ImGuiIO& io) {
  (void)FlushContextTouchSettings();
  const auto snapshot = GetContextTouchDrawableOverlaySnapshot();
  repaint_needed_ = snapshot.visible && snapshot.layout.mode != ContextTouchMode::kFrontend &&
                    snapshot.layout.mode != ContextTouchMode::kMap;
  DrawContextTouchOverlay(ImGui::GetForegroundDrawList(), ImGui::GetFont(), ImGui::GetFontSize(),
                          snapshot, io.DisplaySize.x, io.DisplaySize.y,
                          icon_resolver_ ? icon_resolver_ : ResolveIcon,
                          icon_resolver_ ? icon_context_ : this);
}

}  // namespace gta4::input
