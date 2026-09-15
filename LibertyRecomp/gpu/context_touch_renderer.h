#pragma once

#include <cstdint>
#include <atomic>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <imgui.h>
#include <imgui_internal.h>

#include "input/context_touch_draw.h"

// Initialized before the guest/render worker starts. Thereafter only the
// render worker accesses this object's font, atlas, shared data and buffers.
class ContextTouchRenderer {
public:
    // The loader returns ownership of the native texture object expected by
    // the legacy ImGui pipeline (GuestTexture). It runs only on the render worker.
    using IconTextureLoader = std::shared_ptr<void> (*)(const uint8_t* png, size_t size,
                                                       uint32_t width, uint32_t height);
    void ConfigureIcons(std::filesystem::path directory, IconTextureLoader loader);
    void Initialize(const ImFont& source, const ImFontAtlas& source_atlas);
    const ImDrawData* Draw(uint32_t physical_width, uint32_t physical_height);

private:
    struct IconEntry {
        std::shared_ptr<void> texture;
        float aspect = 1.0f;
    };
    static bool ResolveIcon(void* context, std::string_view id,
                            gta4::input::ContextTouchIcon* icon);
    std::filesystem::path icon_directory_;
    IconTextureLoader icon_loader_ = nullptr;
    // Entries are never evicted while rendering: submitted GPU draws may
    // still reference their descriptors on another in-flight frame.
    std::unordered_map<std::string, IconEntry> icons_;
    size_t icon_decoded_bytes_ = 0;
    ImFontAtlas atlas_;
    ImFont font_;
    ImDrawListSharedData shared_;
    ImDrawList draw_{&shared_};
    ImDrawData data_;
    std::atomic<bool> initialized_{false};
};
