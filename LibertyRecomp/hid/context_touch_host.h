#pragma once

#include <cstdint>
#include <filesystem>

union SDL_Event;
struct SDL_Window;

namespace TouchHost {

void Initialize(const std::filesystem::path& settings_path);
void Shutdown();
void Cancel();
void UpdatePresentation(uint32_t surface_width, uint32_t surface_height,
                        uint32_t output_width, uint32_t output_height);

// SDL operations stay on the event-pumping thread. Render workers only use
// the copied geometry through UpdatePresentation.
void AttachSDLWindow(SDL_Window* window);
void OnSDLEvent(const SDL_Event& event);
void PumpSDLEvents();
void PumpSwitch();

}  // namespace TouchHost
