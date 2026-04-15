#pragma once

// Stub — Liberty's embedded music player was removed. rexglue handles all
// audio natively (or not at all in stub mode). These are no-ops so the
// installer wizard / exports.cpp keep compiling.

namespace EmbeddedPlayer
{
    inline bool s_isActive = false;
    inline void Init()                    {}
    inline void Shutdown()                {}
    inline void Play(const char*)         {}
    inline void PlayMusic()               {}
    inline void FadeOutMusic()            {}
}
