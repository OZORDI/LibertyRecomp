# =============================================================================
# RexGlue SDK Integration for LibertyRecomp
# =============================================================================
# Selectively builds only the RexGlue subsystems needed by LibertyRecomp:
#   - rexcore       (threading, memory utils, exception handling, logging)
#   - rexfilesystem (VFS, STFS/disc image devices)
#   - rexruntime    (processor, MMIO handler, XEX module loading)
#   - rexkernel     (kernel objects, threading, kernel state)
#
# LibertyRecomp provides its own: GPU, audio, input, UI, file I/O, patches.
# =============================================================================

include(GNUInstallDirs)

set(REXGLUE_SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/rexglue-sdk-main")
set(REXGLUE_ROOT "${REXGLUE_SDK_ROOT}")

# --- Configuration -----------------------------------------------------------

# Enable headless mode: LibertyRecomp provides its own GPU/audio/input
# subsystems, so RexGlue's graphics/input/SDL dependencies are not needed.
# REX_HEADLESS affects: kernel_state (no input drivers), xobject (wait caps),
# runtime (no GPU/APU init), xam_ui (headless dialog dispatch).
# LibertyRecomp's rendering, audio, and input code handle these instead.
set(REXGLUE_HEADLESS ON)

# Compiler flags needed by RexGlue code (normally set by its root CMakeLists)
add_compile_options(-fno-char8_t)

# Disable RexGlue tests when building as part of LibertyRecomp
set(REXGLUE_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# --- vcpkg dependencies needed by rexcore/runtime/kernel --------------------
# These are declared in the root vcpkg.json; just find them here.

find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(date CONFIG REQUIRED)
# utf8cpp: use SDK's vendored copy so rexcore's unnamespaced 'utf8cpp' target resolves.
# vcpkg provides utf8cpp::utf8cpp (namespaced) which rexcore doesn't use.
if(NOT TARGET utf8cpp)
    add_subdirectory(
        "${REXGLUE_SDK_ROOT}/thirdparty/utfcpp"
        "${CMAKE_CURRENT_BINARY_DIR}/rexglue-utfcpp"
        EXCLUDE_FROM_ALL
    )
endif()

# --- RexGlue vendored thirdparty (selective) ---------------------------------
# Only build the targets needed by rexcore/rexruntime/rexkernel.
# Skips xxHash, disasm (conflict with XenonRecomp), glslang, FFmpeg (unused).

set(REX_3P "${REXGLUE_SDK_ROOT}/thirdparty")

# Header-only libraries
add_library(disruptorplus INTERFACE)
target_include_directories(disruptorplus INTERFACE
    $<BUILD_INTERFACE:${REX_3P}/disruptorplus/include>
    $<INSTALL_INTERFACE:include/disruptorplus>)

add_library(tomlplusplus INTERFACE)
target_include_directories(tomlplusplus INTERFACE
    $<BUILD_INTERFACE:${REX_3P}/tomlplusplus/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_library(simde INTERFACE)
target_include_directories(simde INTERFACE
    $<BUILD_INTERFACE:${REX_3P}/simde>
    $<BUILD_INTERFACE:${REX_3P}/simde/simde>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# aes_128 - AES-128 encryption/decryption (used by rexkernel crypt routines)
add_library(aes128 STATIC "${REX_3P}/aes_128/aes.c")
target_include_directories(aes128 PUBLIC
    $<BUILD_INTERFACE:${REX_3P}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_options(aes128 PRIVATE -w)

# libmspack - LZX decompression (used by rexruntime XEX loader)
set(MSPACK_DIR "${REX_3P}/libmspack/cabextract/mspack")
add_library(mspack STATIC "${MSPACK_DIR}/lzxd.c")
target_include_directories(mspack PUBLIC
    $<BUILD_INTERFACE:${MSPACK_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_options(mspack PRIVATE -w)

# abseil-cpp - use vcpkg's abseil to avoid duplicate symbols with
# protobuf/GameNetworkingSockets which also link vcpkg abseil.
find_package(absl CONFIG REQUIRED)

# --- Fix: vcpkg fmt/spdlog compatibility -------------------------------------
# XenonRecomp vendors fmt 11.0.2 with `basic_format_string` alias removed.
# vcpkg's spdlog was compiled against vcpkg's fmt which retains the alias.
# Create a wrapper target that uses vcpkg fmt headers + XenonRecomp's fmt lib.
set(_VCPKG_INC "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include")
if(EXISTS "${_VCPKG_INC}/fmt")
    message(STATUS "RexGlue: Creating rexglue_fmt wrapper (vcpkg headers + vendored lib)")
    # Copy vcpkg fmt headers outside vcpkg_installed/ so the vcpkg toolchain
    # won't auto-convert the include path to -isystem (which loses to -I).
    set(_REXFMT_INC "${CMAKE_CURRENT_BINARY_DIR}/rexglue_fmt_inc")
    file(MAKE_DIRECTORY "${_REXFMT_INC}")
    file(COPY "${_VCPKG_INC}/fmt" DESTINATION "${_REXFMT_INC}")
    # Wrapper target: vcpkg fmt headers (as -I) + XenonRecomp fmt library only
    add_library(rexglue_fmt INTERFACE)
    target_include_directories(rexglue_fmt INTERFACE "${_REXFMT_INC}")
    target_link_libraries(rexglue_fmt INTERFACE $<LINK_ONLY:fmt>)
    set(REXGLUE_FMT_TARGET rexglue_fmt)
else()
    message(STATUS "RexGlue: Using default fmt::fmt target")
endif()

# --- RexGlue core subsystems ------------------------------------------------

add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/core"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-core"
)

add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/filesystem"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-filesystem"
)

add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/system"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-runtime"
)

# --- Pre-built rexui from SDK (provides ImGuiDialog, WindowedAppContext etc.)
# The SDK ships librexui.a for macOS ARM64. rexkernel/xam_ui.cpp needs it.
set(_REXGLUE_PREBUILT "${REXGLUE_SDK_ROOT}/out/macos-arm64/Release")
if(EXISTS "${_REXGLUE_PREBUILT}/librexui.a")
    add_library(rexui STATIC IMPORTED)
    set_target_properties(rexui PROPERTIES
        IMPORTED_LOCATION "${_REXGLUE_PREBUILT}/librexui.a"
        INTERFACE_INCLUDE_DIRECTORIES "${REXGLUE_SDK_ROOT}/include"
    )
    add_library(rex::ui ALIAS rexui)
    message(STATUS "RexGlue: Using pre-built librexui.a for UI symbols")
else()
    message(WARNING "RexGlue: Pre-built librexui.a not found at ${_REXGLUE_PREBUILT}")
endif()

# Also need libimgui.a from pre-built SDK (used by rexui)
if(EXISTS "${_REXGLUE_PREBUILT}/libimgui.a")
    add_library(imgui_prebuilt STATIC IMPORTED)
    set_target_properties(imgui_prebuilt PROPERTIES
        IMPORTED_LOCATION "${_REXGLUE_PREBUILT}/libimgui.a")
    message(STATUS "RexGlue: Using pre-built libimgui.a")
endif()

# --- Headless stubs for rexgraphics / rexinput / rexaudio ------------------
# In headless mode, LibertyRecomp provides its own GPU/audio/input.
# rexkernel links PRIVATE against these, so we need stub targets to satisfy
# CMake dependency resolution without pulling in the full GPU stack.
# The SDK headers (rex/graphics/, rex/input/) are already in the include path
# via rexcore's PUBLIC include directory.
if(NOT TARGET rexgraphics)
    add_library(rexgraphics INTERFACE)
    add_library(rex::graphics ALIAS rexgraphics)
    message(STATUS "RexGlue: Using headless stub for rexgraphics")
endif()
if(NOT TARGET rexinput)
    add_library(rexinput INTERFACE)
    add_library(rex::input ALIAS rexinput)
    message(STATUS "RexGlue: Using headless stub for rexinput")
endif()
if(NOT TARGET rexaudio)
    add_library(rexaudio INTERFACE)
    add_library(rex::audio ALIAS rexaudio)
    message(STATUS "RexGlue: Using headless stub for rexaudio")
endif()

add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/kernel"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-kernel"
)

# --- Tell rexkernel that LibertyRecomp overrides certain PPC hooks (avoids duplicate symbols)
# This must be set BEFORE rexkernel sources are compiled.
target_compile_definitions(rexkernel PRIVATE LIBERTY_RECOMP_OVERRIDES)

# --- Link pre-built rexui into rexkernel (provides ImGuiDialog etc.)
if(TARGET rexui AND TARGET rexkernel)
    target_link_libraries(rexkernel PUBLIC rexui)
    if(TARGET imgui_prebuilt)
        target_link_libraries(rexkernel PUBLIC imgui_prebuilt)
    endif()
    message(STATUS "RexGlue: Linked pre-built rexui into rexkernel")
endif()

# --- Fix: inject vcpkg fmt headers into rexcore so spdlog finds the right fmt
# XenonRecomp is added to the build before glue/ and registers fmt::fmt pointing
# to its own vendored copy (which removed basic_format_string). rexcore links
# fmt::fmt and includes spdlog headers that need basic_format_string. Fix by
# injecting the copied vcpkg fmt headers (with the alias) as a high-priority
# include dir directly on rexcore and its dependents.
if(DEFINED _REXFMT_INC AND EXISTS "${_REXFMT_INC}/fmt")
    foreach(_rexlib rexcore rexfilesystem rexsystem rexkernel)
        if(TARGET ${_rexlib})
            target_include_directories(${_rexlib} BEFORE PUBLIC "${_REXFMT_INC}")
        endif()
    endforeach()
    message(STATUS "RexGlue: Injected vcpkg fmt headers into rexcore/filesystem/runtime/kernel")
endif()

# --- Fix: utf8cpp include path
# vcpkg utf8cpp installs headers under include/utf8cpp/ but the SDK includes
# them as <utf8.h>. Inject the SDK's own vendored utfcpp source path so
# #include <utf8.h> resolves correctly without modifying SDK source.
if(TARGET rexcore)
    target_include_directories(rexcore PRIVATE
        "${REXGLUE_SDK_ROOT}/thirdparty/utfcpp/source")
    message(STATUS "RexGlue: Injected SDK utfcpp include path into rexcore")
endif()

# --- Fix: add thirdparty/imgui to rexkernel include path
# xam_ui.cpp uses #include <imgui.h> which needs thirdparty/imgui/ specifically
# (thirdparty/ alone doesn't resolve imgui.h since it lives in the imgui subdir).
if(TARGET rexkernel)
    target_include_directories(rexkernel PRIVATE
        "${REXGLUE_SDK_ROOT}/thirdparty/imgui")
    message(STATUS "RexGlue: Injected imgui include path into rexkernel")
endif()
