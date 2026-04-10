cmake_minimum_required (VERSION 3.20)

set(LIBERTY_RECOMP_THIRDPARTY_ROOT ${CMAKE_SOURCE_DIR}/thirdparty)
set(LIBERTY_RECOMP_TOOLS_ROOT ${CMAKE_SOURCE_DIR}/tools)
set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS OFF)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project("LibertyRecomp-ALL")

# Force FMT_USE_CONSTEVAL off - Homebrew LLVM consteval is broken with vcpkg spdlog
add_compile_definitions(FMT_USE_CONSTEVAL=0)

# ─── vcpkg check (desktop platforms only) ──────────────────────────────────
# Mobile/console platforms use system SDKs and don't need vcpkg
if(NOT DEFINED LIBERTY_RECOMP_TARGET_PLATFORM OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "desktop")
    if(NOT DEFINED ENV{VCPKG_ROOT})
        message(FATAL_ERROR "VCPKG_ROOT is not defined! Set it or use a platform-specific preset.")
    endif()
endif()
# ────────────────────────────────────────────────────────────────────────────

# ─── Cross-Platform Target Selection ───────────────────────────────────────
# Set LIBERTY_RECOMP_TARGET_PLATFORM to one of: desktop, ios, android, ps4, switch
# The toolchain files (toolchains/*.cmake) set this automatically.
if(NOT DEFINED LIBERTY_RECOMP_TARGET_PLATFORM)
    set(LIBERTY_RECOMP_TARGET_PLATFORM "desktop" CACHE STRING
        "Target platform: desktop | ios | android | ps4 | switch")
endif()
message(STATUS "Target platform: ${LIBERTY_RECOMP_TARGET_PLATFORM}")

# ─── Build-time Asset Path ──────────────────────────────────────────────────
# For non-desktop platforms that cannot use a file-picker installer, all game
# assets must be available at configure time and bundled into the package.
set(LIBERTY_RECOMP_EMBEDDED_GAME_PATH
    "${CMAKE_SOURCE_DIR}/tools/local_game_payload"
    CACHE PATH "Path to the staged GTA IV Xbox 360 game files for embedded builds")

if(NOT LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "desktop")
    message(STATUS "Embedded game path: ${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}")

    if(NOT EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}")
        message(FATAL_ERROR
            "LIBERTY_RECOMP_EMBEDDED_GAME_PATH does not exist:\n"
            "  ${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}\n"
            "Populate tools/local_game_payload/ with the required game files or set "
            "LIBERTY_RECOMP_EMBEDDED_GAME_PATH to the correct path.\n"
            "See tools/local_game_payload/README.md for the required layout.")
    endif()

    # Validate required files
    foreach(_req_file default.xex common.rpf xbox360.rpf audio.rpf)
        if(NOT EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/${_req_file}")
            message(FATAL_ERROR
                "Required game file missing from embedded payload:\n"
                "  ${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/${_req_file}\n"
                "See tools/local_game_payload/README.md for the required layout.")
        endif()
    endforeach()

    message(STATUS "Embedded payload validated successfully.")

    # ── Detect optional DLC ───────────────────────────────────────────────
    set(LIBERTY_PAYLOAD_HAS_TLAD  "false")
    set(LIBERTY_PAYLOAD_HAS_TBOGT "false")
    if(EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/dlc/TLAD")
        set(LIBERTY_PAYLOAD_HAS_TLAD "true")
        message(STATUS "  Found embedded DLC: TLAD")
    endif()
    if(EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/dlc/TBOGT")
        set(LIBERTY_PAYLOAD_HAS_TBOGT "true")
        message(STATUS "  Found embedded DLC: TBOGT")
    endif()

    # ── Manifest variables ────────────────────────────────────────────────
    set(LIBERTY_PAYLOAD_GAME_VERSION "1.0.0")
    set(LIBERTY_PAYLOAD_TU_VERSION   "")
    string(TIMESTAMP LIBERTY_PAYLOAD_BUILD_TIMESTAMP "%s")

    configure_file(
        "${CMAKE_SOURCE_DIR}/LibertyRecomp/install/manifest.json.in"
        "${CMAKE_BINARY_DIR}/manifest.json"
        @ONLY)
endif()
# ────────────────────────────────────────────────────────────────────────────



if (APPLE)
    enable_language(OBJC OBJCXX)
endif()

if (CMAKE_OSX_ARCHITECTURES)
    set(LIBERTY_RECOMP_ARCHITECTURE ${CMAKE_OSX_ARCHITECTURES})
elseif(CMAKE_SYSTEM_PROCESSOR)
    set(LIBERTY_RECOMP_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
else()
    set(LIBERTY_RECOMP_ARCHITECTURE ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()
string(TOLOWER "${LIBERTY_RECOMP_ARCHITECTURE}" LIBERTY_RECOMP_ARCHITECTURE)
message(STATUS "Detected architecture: ${LIBERTY_RECOMP_ARCHITECTURE}")

if (LIBERTY_RECOMP_ARCHITECTURE STREQUAL "x86_64" OR LIBERTY_RECOMP_ARCHITECTURE STREQUAL "amd64")
    # Target Sandy Bridge for all projects
    add_compile_options(
        -march=sandybridge
    )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Normally only defined by Visual Studio, added for consistency
    add_compile_definitions(_DEBUG)
endif()

add_subdirectory(${LIBERTY_RECOMP_THIRDPARTY_ROOT})
add_subdirectory(${LIBERTY_RECOMP_TOOLS_ROOT})

# RexGlue SDK integration (core/filesystem/runtime/kernel only)
option(USE_REXGLUE_RUNTIME "Use RexGlue SDK for runtime infrastructure" ON)
if(USE_REXGLUE_RUNTIME)
    add_subdirectory("glue")
endif()

# Include sub-projects.
add_subdirectory("LibertyRecompLib")
add_subdirectory("LibertyRecomp")
