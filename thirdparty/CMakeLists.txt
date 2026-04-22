cmake_policy(SET CMP0077 NEW)

set(MSDF_ATLAS_BUILD_STANDALONE OFF)
set(MSDF_ATLAS_USE_SKIA OFF)
set(MSDF_ATLAS_NO_ARTERY_FONT ON)
set(MSDFGEN_DISABLE_PNG ON)

# SDL2_mixer DISABLED — migrated to SDL3 (via rexglue thirdparty/sdl3)
# SDL3_mixer not yet integrated; embedded_player.cpp uses #if 0 guards
# set(SDL2MIXER_DEPS_SHARED OFF)
# set(SDL2MIXER_VENDORED ON)
# set(SDL2MIXER_FLAC OFF)
# set(SDL2MIXER_MOD OFF)
# set(SDL2MIXER_MP3 OFF)
# set(SDL2MIXER_MIDI OFF)
# set(SDL2MIXER_OPUS OFF)
# set(SDL2MIXER_VORBIS "VORBISFILE")
# set(SDL2MIXER_WAVPACK OFF)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(SDL_VULKAN_ENABLED ON CACHE BOOL "")
endif()

if (WIN32)
    set(D3D12_AGILITY_SDK_ENABLED ON CACHE BOOL "")
endif()

if (APPLE AND CMAKE_OSX_ARCHITECTURES)
    set(BASE_ARCHITECTURE "${CMAKE_OSX_ARCHITECTURES}")
elseif (CMAKE_SYSTEM_PROCESSOR)
    set(BASE_ARCHITECTURE "${CMAKE_SYSTEM_PROCESSOR}")
else()
    set(BASE_ARCHITECTURE "${CMAKE_HOST_SYSTEM_PROCESSOR}")
endif()

# Next, match common architecture strings down to a known common value.
if (BASE_ARCHITECTURE MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set(ARCHITECTURE "x86_64")
elseif (BASE_ARCHITECTURE MATCHES "(aarch64)|(AARCH64)|(arm64)|(ARM64)")
    set(ARCHITECTURE "arm64")
else()
    message(FATAL_ERROR "Unsupported CPU architecture: ${BASE_ARCHITECTURE}")
endif()

# Disable vcpkg for msdf-atlas-gen on console/Android cross-compile builds
if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR ANDROID)
    set(MSDF_ATLAS_USE_VCPKG OFF)
    set(MSDFGEN_USE_VCPKG OFF)
    # Build Freetype from source for console/Android platforms
    include(FetchContent)
    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-2
        GIT_SHALLOW TRUE
    )
    set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
    set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
    set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
    set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
    set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
    set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "" FORCE)
    FetchContent_MakeAvailable(freetype)
    # Create the Freetype::Freetype alias that msdfgen expects
    if (NOT TARGET Freetype::Freetype)
        add_library(Freetype::Freetype ALIAS freetype)
    endif()
endif()

add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/msdf-atlas-gen")
# NFD (native file dialogs) — desktop only, consoles/Android use embedded assets or picker
if (NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR ANDROID))
    add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/nativefiledialog-extended")
endif()
add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/o1heap")
# SDL2 + SDL2_mixer DISABLED — migrated to SDL3 (built in glue/CMakeLists.txt)
# add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/SDL")
# add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/SDL_mixer")
# Plume (Vulkan abstraction) — Switch uses NVN via libnx, not Vulkan.
# Volk (plume dep) requires dlfcn.h which newlib doesn't provide.
if (NOT LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch")
    add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/plume")
endif()
# ffmpeg-core downloads prebuilt binaries — only available for desktop platforms
if (NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR ANDROID))
    add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/ffmpeg-core")
endif()

if (APPLE)
    add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/MoltenVK")
endif()

# GameNetworkingSockets for P2P multiplayer (desktop only — consoles/Android use raw-socket fallback)
if (NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR ANDROID))
    set(BUILD_STATIC_LIB ON CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIB OFF CACHE BOOL "" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
    set(ENABLE_ICE ON CACHE BOOL "" FORCE)
    set(USE_STEAMWEBRTC OFF CACHE BOOL "" FORCE)
    set(Protobuf_USE_STATIC_LIBS OFF CACHE BOOL "" FORCE)
    add_subdirectory("${LIBERTY_RECOMP_THIRDPARTY_ROOT}/GameNetworkingSockets")
endif()
