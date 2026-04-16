# ReXGlue SDK - Xbox 360 Recompilation Toolkit
# Modern C++23 cross-platform SDK for recompiling Xbox 360 executables
cmake_minimum_required(VERSION 3.25)

set(REXGLUE_PROJECT_LANGUAGES C CXX)
if(APPLE)
    list(APPEND REXGLUE_PROJECT_LANGUAGES OBJC OBJCXX)
endif()

project(ReXGlue
    VERSION 0.7.5
    LANGUAGES ${REXGLUE_PROJECT_LANGUAGES}
    DESCRIPTION "Xbox 360 Recompilation SDK"
)

#==========================================================
# Build Options
#==========================================================
option(REXGLUE_BUILD_TESTS       "Build PPC instruction and unit tests" OFF)
option(REXGLUE_ENABLE_SANITIZERS "Enable UndefinedBehaviorSanitizer" OFF)
option(REXGLUE_ENABLE_FIDELITYFX "Enable optional AMD FidelityFX integration (experimental)" OFF)
option(REXGLUE_ENABLE_TRACY                "Enable Tracy profiler integration" ON)
option(REXGLUE_ENABLE_PERF_COUNTERS        "Enable lightweight perf counter registry and overlay" ON)

# Detect iOS / Android for downstream gating. Consoles + mobile are _REXGLUE_IS_MOBILE-ish
# for the purposes of disabling host-only debug features (Tracy, etc).
set(_REXGLUE_IS_MOBILE OFF)
if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(_REXGLUE_IS_MOBILE ON)
endif()

# Tracy is host-only: force OFF on consoles and mobile regardless of user/default.
if(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4"
   OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch"
   OR _REXGLUE_IS_MOBILE)
    set(REXGLUE_ENABLE_TRACY OFF CACHE BOOL "" FORCE)
endif()

# Propagate profiling defines globally (compiled out in Release)
if(REXGLUE_ENABLE_TRACY)
    add_compile_definitions($<$<NOT:$<CONFIG:Release>>:REXGLUE_ENABLE_PROFILING>)
endif()
if(REXGLUE_ENABLE_PERF_COUNTERS)
    add_compile_definitions($<$<NOT:$<CONFIG:Release>>:REXGLUE_ENABLE_PERF_COUNTERS>)
endif()

# Master graphics toggle. When OFF, the graphics subtree is skipped entirely
# and the "at least one backend" fatal_error is bypassed. Default ON; forced
# OFF on PS4 (GNM-only, no Vulkan/D3D12 impl yet).
option(REXGLUE_ENABLE_GRAPHICS "Build graphics subsystem (backends + renderer)" ON)

# Graphics backend selection
# D3D12: Windows only, ON by default.
# Vulkan: Cross-platform, ON by default on Linux/Switch, OFF by default on Windows.
# PS4:   GNM-only — force all desktop backends OFF, disable graphics subtree.
# Switch: Vulkan ON (deko3d/NVN wrapper path), D3D12/FidelityFX OFF.
if(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4")
    set(REXGLUE_USE_D3D12 OFF CACHE BOOL "" FORCE)
    set(REXGLUE_USE_VULKAN OFF CACHE BOOL "" FORCE)
    set(REXGLUE_ENABLE_FIDELITYFX OFF CACHE BOOL "" FORCE)
    set(REXGLUE_ENABLE_GRAPHICS OFF CACHE BOOL "" FORCE)
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch")
    set(REXGLUE_USE_D3D12 OFF CACHE BOOL "" FORCE)
    set(REXGLUE_USE_VULKAN ON CACHE BOOL "" FORCE)
    set(REXGLUE_ENABLE_FIDELITYFX OFF CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # iOS: no D3D12; Vulkan via MoltenVK is possible but off by default.
    set(REXGLUE_USE_D3D12 OFF CACHE BOOL "" FORCE)
    option(REXGLUE_USE_VULKAN "Enable Vulkan graphics backend" OFF)
    set(REXGLUE_ENABLE_FIDELITYFX OFF CACHE BOOL "" FORCE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    # Android: Vulkan is the native backend.
    set(REXGLUE_USE_D3D12 OFF CACHE BOOL "" FORCE)
    option(REXGLUE_USE_VULKAN "Enable Vulkan graphics backend" ON)
    set(REXGLUE_ENABLE_FIDELITYFX OFF CACHE BOOL "" FORCE)
elseif(WIN32)
    option(REXGLUE_USE_D3D12  "Enable D3D12 graphics backend" ON)
    option(REXGLUE_USE_VULKAN "Enable Vulkan graphics backend" OFF)
else()
    set(REXGLUE_USE_D3D12 OFF)
    option(REXGLUE_USE_VULKAN "Enable Vulkan graphics backend" ON)
endif()

# Only enforce "at least one backend" when the graphics subsystem is actually being built.
if(REXGLUE_ENABLE_GRAPHICS AND NOT REXGLUE_USE_D3D12 AND NOT REXGLUE_USE_VULKAN)
    message(FATAL_ERROR "At least one graphics backend must be enabled (REXGLUE_USE_D3D12 or REXGLUE_USE_VULKAN), or set REXGLUE_ENABLE_GRAPHICS=OFF")
endif()

if (REXGLUE_ENABLE_FIDELITYFX AND
    NOT (REXGLUE_USE_VULKAN OR REXGLUE_USE_D3D12))
    message(FATAL_ERROR
        "REXGLUE_ENABLE_FIDELITYFX requires at least one graphics backend")
endif()

# Root of the SDK source tree
set(REXGLUE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

# Resolve the effective target architecture. On macOS, CMAKE_SYSTEM_PROCESSOR
# may reflect the host CMake process rather than CMAKE_OSX_ARCHITECTURES.
set(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR "${CMAKE_SYSTEM_PROCESSOR}")
if(APPLE AND CMAKE_OSX_ARCHITECTURES)
    list(GET CMAKE_OSX_ARCHITECTURES 0 REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR)
endif()
string(TOLOWER "${REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR}" REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER)

# SDK install support
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

if(UNIX AND NOT APPLE)
    # Keep installed binaries self-contained by resolving shared libs relative
    # to the install prefix (for example, bin -> ../lib).
    set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
    list(APPEND CMAKE_INSTALL_RPATH
        "$ORIGIN"
        "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
elseif(APPLE)
    list(APPEND CMAKE_INSTALL_RPATH
        "@loader_path"
        "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()

# Enforce the default Clang toolchain, with AppleClang accepted for native
# macOS builds. Consoles (PS4 uses OpenOrbis Clang fork; Switch uses devkitPro
# GCC) are exempt from this enforcement.
if(NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch"))
    if(APPLE)
        if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
                CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
            message(FATAL_ERROR
                "ReXGlue requires Clang or AppleClang on macOS. Please select a supported compiler.")
        endif()

        if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "17.0")
                message(FATAL_ERROR
                    "ReXGlue requires AppleClang 17 or newer (found ${CMAKE_CXX_COMPILER_VERSION})")
            endif()
        elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "18.0")
            message(FATAL_ERROR
                "ReXGlue requires Clang 18 or newer on macOS (found ${CMAKE_CXX_COMPILER_VERSION})")
        endif()
    else()
        if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            message(FATAL_ERROR
                "ReXGlue requires Clang. Please set CMAKE_CXX_COMPILER to clang++")
        endif()

        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "18.0")
            message(FATAL_ERROR
                "ReXGlue requires Clang 18 or newer (found ${CMAKE_CXX_COMPILER_VERSION})")
        endif()
    endif()
endif()

# Enforce x64 architecture
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "ReXGlue only supports 64‑bit architectures (x86_64 or ARM64)")
endif()

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RELWITHDEBINFO_POSTFIX rd)

if(UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4")
    # PS4 OpenOrbis toolchain requires small code model.
    add_compile_options(-mcmodel=small)
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch")
    # Switch is AArch64 — no -mcmodel tuning needed.
elseif(UNIX AND NOT APPLE)
    # Use appropriate code model based on architecture on Linux. macOS doesn't
    # support the same code-model tuning.
    if(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(amd64|x86_64)$")
        add_compile_options(-mcmodel=large)
    else()
        add_compile_options(-mcmodel=small)
    endif()
endif()

# Global compile options
add_compile_options(
    -fno-strict-aliasing
    -ffp-model=strict
    $<$<CONFIG:DEBUG>:-g>
    $<$<CONFIG:DEBUG>:-O0>
    $<$<CONFIG:RELEASE>:-O3>
    $<$<CONFIG:RELEASE>:-DNDEBUG>
    -fno-char8_t
)

# Platform detection (supports AMD64 and ARM64)
# NOTE: iOS must be checked BEFORE the generic APPLE branch — CMAKE_SYSTEM_NAME==iOS
# still has APPLE=1 and would otherwise silently be treated as desktop macOS.
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(REX_PLATFORM "ios-arm64")
    add_compile_definitions(REX_PLATFORM_IOS=1 REX_PLATFORM_MAC=1 _DARWIN_C_SOURCE=1 REX_ARCH_ARM64=1)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    if(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(aarch64|arm64)$")
        set(REX_PLATFORM "android-arm64")
        add_compile_definitions(REX_ARCH_ARM64=1)
    else()
        set(REX_PLATFORM "android-${REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER}")
    endif()
    add_compile_definitions(REX_PLATFORM_ANDROID=1 REX_PLATFORM_LINUX=1)
elseif(WIN32)
    if(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(amd64|x86_64)$")
        set(REX_PLATFORM "win-amd64")
    elseif(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(aarch64|arm64)$")
        set(REX_PLATFORM "win-arm64")
    else()
        message(FATAL_ERROR
            "Unsupported Windows architecture: ${REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR}")
    endif()
    add_compile_definitions(REX_PLATFORM_WINDOWS=1)
elseif(APPLE)
    if(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(aarch64|arm64)$")
        set(REX_PLATFORM "mac-arm64")
        add_compile_definitions(REX_ARCH_ARM64=1)
        add_compile_options(-march=armv8-a)
    elseif(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(amd64|x86_64)$")
        set(REX_PLATFORM "mac-amd64")
    else()
        message(FATAL_ERROR
            "Unsupported macOS architecture: ${REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR}")
    endif()

    set(REXGLUE_USE_D3D12 OFF CACHE BOOL "" FORCE)
    set(REXGLUE_USE_VULKAN ON CACHE BOOL "" FORCE)
    set(REXGLUE_ENABLE_FIDELITYFX OFF CACHE BOOL "" FORCE)
    add_compile_definitions(REX_PLATFORM_MAC=1 _DARWIN_C_SOURCE=1)
elseif(UNIX AND NOT APPLE)
    if(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(amd64|x86_64)$")
        set(REX_PLATFORM "linux-amd64")
    elseif(REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR_LOWER MATCHES "^(aarch64|arm64)$")
        set(REX_PLATFORM "linux-arm64")
    else()
        set(REX_PLATFORM "linux-${REXGLUE_EFFECTIVE_SYSTEM_PROCESSOR}")
    endif()
    add_compile_definitions(REX_PLATFORM_LINUX=1)
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR CMAKE_SYSTEM_NAME STREQUAL "PS4")
    set(REX_PLATFORM "ps4-amd64")
    add_compile_definitions(REX_PLATFORM_PS4=1)
elseif(LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
    set(REX_PLATFORM "switch-arm64")
    add_compile_definitions(REX_PLATFORM_SWITCH=1 REX_PLATFORM_NX=1)
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

# Build config as compile definition (multi-config generators don't know config at configure time)
add_compile_definitions(REXGLUE_BUILD_CONFIG="$<CONFIG>")

# Detect dev builds -- append "-dev" suffix when HEAD is not on a version tag
execute_process(
    COMMAND git describe --tags --exact-match
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_QUIET ERROR_QUIET
    RESULT_VARIABLE REXGLUE_TAG_RESULT
)
if(REXGLUE_TAG_RESULT EQUAL 0)
    set(REXGLUE_VERSION_SUFFIX "")
else()
    set(REXGLUE_VERSION_SUFFIX "-dev")
endif()

# Generate version header -- must run after REX_PLATFORM is set
# Timestamp is set at configure time -- intentionally not updated on every rebuild
string(TIMESTAMP REXGLUE_BUILD_TIMESTAMP "%Y%m%d_%H%M")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/rex/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/rex/version.h
    @ONLY
)

# Output directories (multi-config appends /<Config>/ automatically)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${REXGLUE_ROOT}/out/${REX_PLATFORM}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${REXGLUE_ROOT}/out/${REX_PLATFORM}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${REXGLUE_ROOT}/out/${REX_PLATFORM}")

# Sanitizer support for debugging memory corruption and undefined behavior
if(REXGLUE_ENABLE_SANITIZERS)
    message(STATUS "Enabling UndefinedBehaviorSanitizer (UBSan only - ASan conflicts with custom memory mapping)")
    # Note: ASan doesn't work well with the custom mmap at 0x100000000, so we only use UBSan
    add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=undefined)
endif()

if(REXGLUE_BUILD_TESTS)
    list(APPEND CMAKE_MODULE_PATH "${REXGLUE_ROOT}/cmake")
    list(APPEND CMAKE_MODULE_PATH "${REXGLUE_ROOT}/thirdparty/catch2/extras")
endif()

# Third-party dependencies (vendored sources only)
add_subdirectory(thirdparty)

# Enable warnings after thirdparty
add_compile_options(
    -Wall
    -Wextra
)
include_directories(${REXGLUE_ROOT}/include)

add_subdirectory(src/core)
add_subdirectory(src/filesystem)
add_subdirectory(src/ui)
add_subdirectory(src/input)
add_subdirectory(src/audio)
if(REXGLUE_ENABLE_GRAPHICS)
    add_subdirectory(src/graphics)
endif()
add_subdirectory(src/system)
add_subdirectory(src/kernel)
add_subdirectory(src/codegen)
add_subdirectory(src/rexglue)

# Add tests subdirectory if tests are enabled
if(REXGLUE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Print configuration summary
message(STATUS "===========================================")
message(STATUS "ReXGlue Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}${REXGLUE_VERSION_SUFFIX}")
message(STATUS "  Platform: ${REX_PLATFORM}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Configurations: ${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "  Output Dir: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Graphics: enabled=${REXGLUE_ENABLE_GRAPHICS} D3D12=${REXGLUE_USE_D3D12} Vulkan=${REXGLUE_USE_VULKAN}")
message(STATUS "  FidelityFX: ${REXGLUE_ENABLE_FIDELITYFX}")
message(STATUS "  Tracy: ${REXGLUE_ENABLE_TRACY}")
message(STATUS "  Perf Counters: ${REXGLUE_ENABLE_PERF_COUNTERS}")
message(STATUS "===========================================")

# SDK install/export rules (skipped when consumed as a subdirectory)
if(PROJECT_IS_TOP_LEVEL)
    include(cmake/rexglue_devenv.cmake)
    include(cmake/rexglue_install.cmake)
else()
    # When consumed via add_subdirectory(), provide the same interface as find_package().
    # This sets REXGLUE_SHARE_DIR and makes rexglue_configure_target() available.
    set(REXGLUE_SHARE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/ui" PARENT_SCOPE)
    set(REXGLUE_SHARE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/ui")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/rexglue_helpers.cmake)
endif()
