#pragma once

#define NOMINMAX

#if defined(_WIN32)
#include <windows.h>
#include <ShlObj_core.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
#elif defined(__linux__)
#include <unistd.h>
#include <pwd.h>
#endif

#ifdef LIBERTY_RECOMP_D3D12
#include <dxcapi.h>
#endif

#include <algorithm>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <chrono>
#include <span>
#include <xbox.h>
#include <xxhash.h>
#include <ankerl/unordered_dense.h>
#include <ddspp.h>
// RexGlue PPC types + gta4-recomp function declarations
#include "../glue/gta4-recomp/generated/gta4_init.h"
#include <toml++/toml.hpp>
#include <zstd.h>
#include <stb_image.h>
#include <blockingconcurrentqueue.h>
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
#include <SDL3/SDL.h>
// SDL3_mixer not yet integrated — embedded_player.cpp has #if 0 guards
// #include <SDL3_mixer/SDL_mixer.h>
#else
// Console builds don't use SDL3 — provide stub types for config_def.h key bindings.
// Values match SDL3 USB HID scancodes for config file interoperability.
using SDL_Scancode = int;
#define SDL_SCANCODE_UNKNOWN   0
#define SDL_SCANCODE_A         4
#define SDL_SCANCODE_C         6
#define SDL_SCANCODE_D         7
#define SDL_SCANCODE_E         8
#define SDL_SCANCODE_F         9
#define SDL_SCANCODE_G        10
#define SDL_SCANCODE_H        11
#define SDL_SCANCODE_Q        20
#define SDL_SCANCODE_R        21
#define SDL_SCANCODE_S        22
#define SDL_SCANCODE_T        23
#define SDL_SCANCODE_UP       82
#define SDL_SCANCODE_V        25
#define SDL_SCANCODE_W        26
#define SDL_SCANCODE_X        27
#define SDL_SCANCODE_Z        29
#define SDL_SCANCODE_ESCAPE   41
#define SDL_SCANCODE_SPACE    44
#define SDL_SCANCODE_GRAVE    53
#define SDL_SCANCODE_LCTRL   224
#define SDL_SCANCODE_LSHIFT  225
#endif
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#if !defined(LIBERTY_RECOMP_PS4) && !defined(LIBERTY_RECOMP_NX)
#include <backends/imgui_impl_sdl3.h>
#endif
#include <cstddef>
#include <smolv.h>
#include <set>
#include <fmt/core.h>
#include <list>
#include <semaphore>
#include <numeric>
#include <charconv>

#include "framework.h"
#include "mutex.h"

#ifndef _WIN32
#include <sys/mman.h>
#endif
