# =============================================================================
# RexGlue SDK Integration for LibertyRecomp (v0.7.4)
# =============================================================================
# Selectively builds the RexGlue subsystems needed by LibertyRecomp:
#   - rexcore       (threading, memory utils, exception handling, logging)
#   - rexfilesystem (VFS, STFS/disc image devices)
#   - rexsystem     (processor, MMIO handler, XEX module loading)
#   - rexkernel     (kernel objects, threading, kernel state)
#   - rexaudio      (REAL - SDL3 audio backend for XMA/audio processing)
#   - rexinput      (REAL - SDL3 input backend for controller emulation)
#
# LibertyRecomp provides its own: GPU, UI, file I/O, patches.
# =============================================================================

include(GNUInstallDirs)

set(REXGLUE_SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/rexglue-sdk-main")
set(REXGLUE_ROOT "${REXGLUE_SDK_ROOT}")

# --- Version header generation -----------------------------------------------
# The 0.7.4 root CMakeLists generates version.h from version.h.in using
# project(rexglue VERSION 0.7.4) variables. Since we're add_subdirectory'd,
# PROJECT_VERSION_* won't be set. Generate it manually.
set(PROJECT_VERSION "0.7.4")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 7)
set(PROJECT_VERSION_PATCH 4)
set(REXGLUE_VERSION_SUFFIX "-liberty")

# macOS needs these globally for ucontext (fibers) and BSD extensions
if(APPLE)
    add_compile_definitions(_XOPEN_SOURCE=600 _DARWIN_C_SOURCE)
endif()

# Platform detection for version.h (mirrors root CMakeLists logic)
if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|arm64")
        set(REX_PLATFORM "macos-arm64")
    else()
        set(REX_PLATFORM "macos-amd64")
    endif()
elseif(WIN32)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64")
        set(REX_PLATFORM "win-amd64")
    else()
        set(REX_PLATFORM "win-arm64")
    endif()
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64")
        set(REX_PLATFORM "linux-amd64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
        set(REX_PLATFORM "linux-arm64")
    else()
        set(REX_PLATFORM "linux-${CMAKE_SYSTEM_PROCESSOR}")
    endif()
endif()

string(TIMESTAMP REXGLUE_BUILD_TIMESTAMP "%Y%m%d_%H%M")

configure_file(
    "${REXGLUE_SDK_ROOT}/include/rex/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/rex/version.h"
    @ONLY
)

# --- Configuration -----------------------------------------------------------

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
# Only build the targets needed by rexcore/rexruntime/rexkernel/rexaudio/rexinput.
# Skips xxHash, disasm (conflict with XenonRecomp), glslang, Vulkan (unused).

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

# o1heap for rexglue - SDK uses a different version (O1HEAP_ALIGNMENT = sizeof(void*)*2)
# from the top-level project's o1heap (sizeof(void*)*4). Build SDK's copy separately.
if(TARGET o1heap)
    # Rename the top-level o1heap target reference so rexkernel doesn't pick it up
    set(_REXGLUE_O1HEAP_DIR "${REX_3P}/o1heap/o1heap")
    add_library(rexglue_o1heap STATIC "${_REXGLUE_O1HEAP_DIR}/o1heap.c")
    target_include_directories(rexglue_o1heap PUBLIC
        $<BUILD_INTERFACE:${_REXGLUE_O1HEAP_DIR}>)
    target_compile_options(rexglue_o1heap PRIVATE -w)
endif()

# abseil-cpp - use vcpkg's abseil to avoid duplicate symbols with
# protobuf/GameNetworkingSockets which also link vcpkg abseil.
find_package(absl CONFIG REQUIRED)

# --- SDL3 from rexglue thirdparty -------------------------------------------
# rexaudio and rexinput need SDL3. Build it from the SDK's vendored copy
# with audio+joystick only (no video/render/GPU).
if(NOT TARGET SDL3-static)
    set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    set(SDL_UNIX_CONSOLE_BUILD ON CACHE BOOL "" FORCE)

    # Enable subsystems needed by LibertyRecomp (video, input, audio, haptic, sensor)
    # rexaudio only uses audio but extra subsystems don't hurt
    set(SDL_VIDEO ON CACHE BOOL "" FORCE)
    set(SDL_RENDER OFF CACHE BOOL "" FORCE)
    set(SDL_HAPTIC ON CACHE BOOL "" FORCE)
    set(SDL_SENSOR ON CACHE BOOL "" FORCE)
    set(SDL_JOYSTICK ON CACHE BOOL "" FORCE)
    set(SDL_GAMEPAD ON CACHE BOOL "" FORCE)
    set(SDL_POWER OFF CACHE BOOL "" FORCE)
    set(SDL_FILESYSTEM OFF CACHE BOOL "" FORCE)
    set(SDL_LOCALE OFF CACHE BOOL "" FORCE)
    set(SDL_FILE OFF CACHE BOOL "" FORCE)
    set(SDL_MISC OFF CACHE BOOL "" FORCE)

    # Enable Metal on macOS for window surface creation
    set(SDL_OPENGL OFF CACHE BOOL "" FORCE)
    set(SDL_OPENGLES OFF CACHE BOOL "" FORCE)
    set(SDL_VULKAN ON CACHE BOOL "" FORCE)
    set(SDL_METAL ON CACHE BOOL "" FORCE)

    # Disable dummy drivers
    set(SDL_DISKAUDIO OFF CACHE BOOL "" FORCE)
    set(SDL_DUMMYAUDIO OFF CACHE BOOL "" FORCE)
    set(SDL_DUMMYVIDEO OFF CACHE BOOL "" FORCE)

    add_subdirectory(
        "${REX_3P}/sdl3"
        "${CMAKE_CURRENT_BINARY_DIR}/rexglue-sdl3"
        EXCLUDE_FROM_ALL
    )

    # SDL3 uses getpagesize() which Clang 19+ rejects as implicit in C. Suppress.
    if(TARGET SDL3-static)
        target_compile_options(SDL3-static PRIVATE -Wno-implicit-function-declaration)
    endif()
endif()

# --- imgui (needed by rexkernel xam_ui.cpp for #include <imgui.h>) -----------
if(NOT TARGET imgui)
    set(IMGUI_DIR "${REX_3P}/imgui")
    if(EXISTS "${IMGUI_DIR}/imgui.cpp")
        add_library(imgui STATIC
            ${IMGUI_DIR}/imgui.cpp
            ${IMGUI_DIR}/imgui_demo.cpp
            ${IMGUI_DIR}/imgui_draw.cpp
            ${IMGUI_DIR}/imgui_tables.cpp
            ${IMGUI_DIR}/imgui_widgets.cpp
        )
        add_library(imgui::imgui ALIAS imgui)
        target_include_directories(imgui PUBLIC
            $<BUILD_INTERFACE:${IMGUI_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
        target_compile_options(imgui PRIVATE -w)
        message(STATUS "RexGlue: Built imgui from SDK thirdparty")
    endif()
endif()

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
# The sub-CMakeLists use ${PROJECT_SOURCE_DIR}/include to find rex headers.
# When built as add_subdirectory, PROJECT_SOURCE_DIR is LibertyRecomp root,
# not rexglue root. Fix by overriding the include dirs after each target.

# Common include dirs to inject into every rexglue target
set(_REXGLUE_INC "${REXGLUE_SDK_ROOT}/include")
set(_REXGLUE_GEN_INC "${CMAKE_CURRENT_BINARY_DIR}/include")  # generated version.h

add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/core"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-core"
)
target_include_directories(rexcore PUBLIC "${_REXGLUE_INC}" "${_REXGLUE_GEN_INC}")
target_include_directories(rexcore PRIVATE "${REXGLUE_SDK_ROOT}" "${REX_3P}/cli11/include")

add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/filesystem"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-filesystem"
)
target_include_directories(rexfilesystem PUBLIC "${_REXGLUE_INC}" "${_REXGLUE_GEN_INC}")
target_include_directories(rexfilesystem PRIVATE "${REXGLUE_SDK_ROOT}")

add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/system"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-runtime"
)
target_include_directories(rexsystem PUBLIC "${_REXGLUE_INC}" "${_REXGLUE_GEN_INC}")
target_include_directories(rexsystem PRIVATE "${REXGLUE_SDK_ROOT}")

# --- rexaudio (REAL build, SDL3 backend) ------------------------------------
add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/audio"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-audio"
)
target_include_directories(rexaudio PUBLIC "${_REXGLUE_INC}" "${_REXGLUE_GEN_INC}")
target_include_directories(rexaudio PRIVATE "${REXGLUE_SDK_ROOT}")

# --- rexinput (REAL build, SDL3 backend) ------------------------------------
add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/input"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-input"
)
target_include_directories(rexinput PUBLIC "${_REXGLUE_INC}" "${_REXGLUE_GEN_INC}")
target_include_directories(rexinput PRIVATE "${REXGLUE_SDK_ROOT}")

# --- Headless stubs for rexgraphics and rexui --------------------------------
# LibertyRecomp provides its own GPU and UI. rexkernel links PRIVATE against
# rexgraphics so we need an INTERFACE stub. rexui is only needed for
# ImGuiDialog symbols in xam_ui.cpp — stub it since we have our own UI.
if(NOT TARGET rexgraphics)
    add_library(rexgraphics INTERFACE)
    add_library(rex::graphics ALIAS rexgraphics)
    message(STATUS "RexGlue: Using headless stub for rexgraphics")
endif()
if(NOT TARGET rexui)
    add_library(rexui INTERFACE)
    add_library(rex::ui ALIAS rexui)
    message(STATUS "RexGlue: Using headless stub for rexui (LibertyRecomp has own UI)")
endif()

# --- rexkernel ---------------------------------------------------------------
add_subdirectory(
    "${REXGLUE_SDK_ROOT}/src/kernel"
    "${CMAKE_CURRENT_BINARY_DIR}/rexglue-kernel"
)
target_include_directories(rexkernel PUBLIC "${_REXGLUE_INC}" "${_REXGLUE_GEN_INC}")
target_include_directories(rexkernel PRIVATE "${REXGLUE_SDK_ROOT}")

# rexkernel's CMakeLists links the top-level o1heap (v2), but heap.cpp needs
# the SDK's o1heap v3 (O1HEAP_ALIGNMENT = sizeof(void*)*2, has o1heapReallocate).
# Swap the link: remove v2, add v3.
if(TARGET rexglue_o1heap)
    get_target_property(_kern_libs rexkernel LINK_LIBRARIES)
    list(REMOVE_ITEM _kern_libs o1heap)
    list(APPEND _kern_libs rexglue_o1heap)
    set_target_properties(rexkernel PROPERTIES LINK_LIBRARIES "${_kern_libs}")
endif()

# --- Tell rexkernel that LibertyRecomp overrides certain PPC hooks (avoids duplicate symbols)
target_compile_definitions(rexkernel PRIVATE LIBERTY_RECOMP_OVERRIDES)

# --- Link imgui into rexkernel (xam_ui.cpp needs <imgui.h>) ---
if(TARGET imgui AND TARGET rexkernel)
    target_link_libraries(rexkernel PUBLIC imgui)
    message(STATUS "RexGlue: Linked imgui into rexkernel")
endif()

# --- Fix: inject vcpkg fmt headers into rexcore so spdlog finds the right fmt
# XenonRecomp is added to the build before glue/ and registers fmt::fmt pointing
# to its own vendored copy (which removed basic_format_string). rexcore links
# fmt::fmt and includes spdlog headers that need basic_format_string. Fix by
# injecting the copied vcpkg fmt headers (with the alias) as a high-priority
# include dir directly on rexcore and its dependents.
if(DEFINED _REXFMT_INC AND EXISTS "${_REXFMT_INC}/fmt")
    foreach(_rexlib rexcore rexfilesystem rexsystem rexkernel rexaudio rexinput)
        if(TARGET ${_rexlib})
            target_include_directories(${_rexlib} BEFORE PUBLIC "${_REXFMT_INC}")
        endif()
    endforeach()
    message(STATUS "RexGlue: Injected vcpkg fmt headers into all rexglue targets")
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
if(TARGET rexkernel)
    target_include_directories(rexkernel PRIVATE
        "${REXGLUE_SDK_ROOT}/thirdparty/imgui")
    message(STATUS "RexGlue: Injected imgui include path into rexkernel")
endif()

# --- Linker stubs for rex::ui and rex::audio symbols -------------------------
# rexkernel references ImGuiDialog / WindowedAppContext / XmaDecoder but rexui
# and rexaudio are INTERFACE stubs. Provide empty implementations so the linker
# is satisfied without building the full graphics stack.
add_library(rexglue_stubs STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/rexglue-stubs/ui_stubs.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rexglue-stubs/audio_stubs.cpp"
)
target_include_directories(rexglue_stubs PRIVATE "${_REXGLUE_INC}" "${_REXGLUE_GEN_INC}")
target_compile_options(rexglue_stubs PRIVATE -w)
message(STATUS "RexGlue: Built rexglue_stubs (ui + audio linker stubs)")
