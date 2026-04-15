#pragma once

// Audio removed from LibertyRecomp — rexglue handles audio natively.
// These functions are intentionally no-ops so the UI achievement
// overlay (ui/achievement_menu.cpp, ui/achievement_overlay.cpp) keeps
// compiling without forcing an audio dependency back into Liberty.
inline void Game_PlaySound(const char*) {}
inline void Game_PlaySound(const char*, const char*) {}
