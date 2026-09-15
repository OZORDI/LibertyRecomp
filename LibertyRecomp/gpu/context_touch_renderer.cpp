#include "context_touch_renderer.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <vector>

#include <stb_image.h>

#include <rex/input/absolute_pointer.h>
#include "input/context_touch_controls.h"
#include "input/context_touch_draw.h"

void ContextTouchRenderer::ConfigureIcons(std::filesystem::path directory, IconTextureLoader loader) {
    // Configuration is published with the immutable font snapshot at startup.
    if (initialized_.load(std::memory_order_acquire)) return;
    icon_directory_ = std::move(directory);
    icon_loader_ = loader;
}

bool ContextTouchRenderer::ResolveIcon(void* context, std::string_view id,
                                      gta4::input::ContextTouchIcon* icon) {
    auto& self = *static_cast<ContextTouchRenderer*>(context);
    constexpr size_t kMaximumEncodedBytes = 4194304;
    constexpr size_t kMaximumDecodedBytes = 33554432;
    constexpr size_t kMaximumCachedIcons = 256;
    constexpr int kMaximumDimension = 1024;
    constexpr std::array<uint8_t, 8> kPngSignature{137, 80, 78, 71, 13, 10, 26, 10};
    if (!icon || !self.icon_loader_ || self.icon_directory_.empty() || id.empty() ||
        id.size() > 64 || !std::all_of(id.begin(), id.end(), [](char c) {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                   (c >= '0' && c <= '9') || c == '_';
        })) return false;
    const std::string key(id);
    auto found = self.icons_.find(key);
    if (found == self.icons_.end()) {
        if (self.icons_.size() >= kMaximumCachedIcons) return false;
        IconEntry entry;
        std::ifstream file(self.icon_directory_ / (key + ".png"), std::ios::binary | std::ios::ate);
        if (file) {
            const auto length = file.tellg();
            if (length >= static_cast<std::streamoff>(kPngSignature.size()) &&
                length <= static_cast<std::streamoff>(kMaximumEncodedBytes)) {
                std::vector<uint8_t> bytes(static_cast<size_t>(length));
                file.seekg(0);
                if (file.read(reinterpret_cast<char*>(bytes.data()), length) &&
                    std::equal(kPngSignature.begin(), kPngSignature.end(), bytes.begin())) {
                    int width = 0, height = 0, channels = 0;
                    if (stbi_info_from_memory(bytes.data(), static_cast<int>(bytes.size()),
                            &width, &height, &channels) && width > 0 && height > 0 &&
                        width <= kMaximumDimension && height <= kMaximumDimension) {
                        const size_t decoded_bytes = size_t(width) * size_t(height) * 4;
                        if (decoded_bytes <= kMaximumDecodedBytes - self.icon_decoded_bytes_) {
                            entry.texture = self.icon_loader_(bytes.data(), bytes.size(),
                                static_cast<uint32_t>(width), static_cast<uint32_t>(height));
                            if (entry.texture) {
                                entry.aspect = float(width) / float(height);
                                self.icon_decoded_bytes_ += decoded_bytes;
                            }
                        }
                    }
                }
            }
        }
        found = self.icons_.emplace(key, std::move(entry)).first;
    }
    if (!found->second.texture) return false;
    icon->texture = found->second.texture.get();
    icon->aspect = found->second.aspect;
    return true;
}

void ContextTouchRenderer::Initialize(const ImFont& source, const ImFontAtlas& source_atlas) {
    // ImVector copies own their buffers. Repair the two borrowed pointers so
    // font rendering never reads mutable data from the menu's ImGui context.
    font_ = source;
    font_.FallbackGlyph = font_.FindGlyphNoFallback(source.FallbackChar);
    font_.ConfigData = nullptr;
    font_.ConfigDataCount = 0;
    font_.ContainerAtlas = &atlas_;
    atlas_.TexID = source_atlas.TexID;
    atlas_.TexUvWhitePixel = source_atlas.TexUvWhitePixel;
    shared_.TexUvWhitePixel = atlas_.TexUvWhitePixel;
    shared_.Font = &font_;
    shared_.FontSize = 18.0f;
    shared_.FontScale = shared_.FontSize / font_.FontSize;
    shared_.CurveTessellationTol = 1.25f;
    shared_.SetCircleTessellationMaxError(0.30f);
    shared_.InitialFlags = ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill |
                           ImDrawListFlags_AllowVtxOffset;
    initialized_.store(true, std::memory_order_release);
}

const ImDrawData* ContextTouchRenderer::Draw(uint32_t physical_width, uint32_t physical_height) {
    if (!initialized_.load(std::memory_order_acquire) || !physical_width || !physical_height) return nullptr;
    rex::input::TouchPresentationState presentation;
    if (!rex::input::GetTouchPresentationState(&presentation) || !presentation.valid ||
        presentation.logical_width <= 0.0f || presentation.logical_height <= 0.0f) return nullptr;
    const auto snapshot = gta4::input::GetContextTouchDrawableOverlaySnapshot();
    if (!snapshot.visible) return nullptr;
    if (snapshot.layout.viewport.generation != presentation.generation) return nullptr;

    const float width = presentation.logical_width;
    const float height = presentation.logical_height;
    shared_.ClipRectFullscreen = ImVec4(0.0f, 0.0f, width, height);
    draw_._ResetForNewFrame();
    draw_.PushClipRect(ImVec2(0.0f, 0.0f), ImVec2(width, height));
    draw_.PushTextureID(atlas_.TexID);
    gta4::input::DrawContextTouchOverlay(&draw_, &font_, shared_.FontSize,
                                        snapshot, width, height, &ResolveIcon, this);
    draw_.PopTextureID();
    draw_.PopClipRect();
    draw_._PopUnusedDrawCmd();
    if (draw_.VtxBuffer.empty() || draw_.IdxBuffer.empty()) return nullptr;

    // The legacy ImGui pipeline takes framebuffer pixels. Convert the private
    // draw list once; hit testing and UI layout continue to use logical units.
    const float scale_x = float(physical_width) / width;
    const float scale_y = float(physical_height) / height;
    for (auto& vertex : draw_.VtxBuffer) {
        vertex.pos.x *= scale_x;
        vertex.pos.y *= scale_y;
    }
    for (auto& command : draw_.CmdBuffer) {
        command.ClipRect.x *= scale_x;
        command.ClipRect.z *= scale_x;
        command.ClipRect.y *= scale_y;
        command.ClipRect.w *= scale_y;
    }
    data_.Clear();
    data_.Valid = true;
    data_.DisplaySize = ImVec2(float(physical_width), float(physical_height));
    data_.FramebufferScale = ImVec2(1.0f, 1.0f);
    data_.CmdLists.push_back(&draw_);
    data_.CmdListsCount = data_.CmdLists.Size;
    data_.TotalIdxCount = draw_.IdxBuffer.Size;
    data_.TotalVtxCount = draw_.VtxBuffer.Size;
    return &data_;
}
