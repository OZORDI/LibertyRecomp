#pragma once

// Shim header — original aspect_ratio_patches.cpp was Sonicteam-specific
// and removed. gta4_aspect_ratio_patches.cpp provides the live implementation.
// This header declares every symbol both the live .cpp and all UI consumers
// still reference.

#include <cstdint>
#include <optional>
#include <xxhash.h>
#include <unordered_dense.h>

// xxHashMap alias used by gta4_aspect_ratio_patches.cpp
template <typename V>
using xxHashMap = ankerl::unordered_dense::map<XXH64_hash_t, V>;

struct CsdModifier
{
    float scale  = 1.0f;
    float offset = 0.0f;
};

struct MovieModifier
{
    float scale  = 1.0f;
    float offset = 0.0f;
};

extern const xxHashMap<CsdModifier>   g_csdModifiers;
extern const xxHashMap<MovieModifier> g_movieModifiers;

std::optional<CsdModifier> FindCsdModifier(uint32_t data);
MovieModifier              FindMovieModifier(XXH64_hash_t nameHash);

inline constexpr float NARROW_ASPECT_RATIO = 4.0f / 3.0f;
inline constexpr float WIDE_ASPECT_RATIO   = 16.0f / 9.0f;

inline float g_aspectRatio              = WIDE_ASPECT_RATIO;
inline float g_aspectRatioMovie         = WIDE_ASPECT_RATIO;
inline float g_aspectRatioScale         = 1.0f;
inline float g_aspectRatioGameplayScale = 1.0f;
inline float g_aspectRatioNarrowScale   = 1.0f;
inline float g_aspectRatioNarrowMargin  = 0.0f;
inline float g_aspectRatioOffsetX       = 0.0f;
inline float g_aspectRatioOffsetY       = 0.0f;
inline float g_aspectRatioMultiplayerOffsetX = 0.0f;
inline float g_horzCentre               = 640.0f;
inline float g_vertCentre               = 360.0f;
inline float g_radarMapScale            = 1.0f;

namespace AspectRatioPatches
{
    void  Init();
    void  ComputeOffsets();
    inline float GetAspectRatio() { return g_aspectRatio; }
    inline bool  IsWidescreen()   { return g_aspectRatio >= WIDE_ASPECT_RATIO; }
}
