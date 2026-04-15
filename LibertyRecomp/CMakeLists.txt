project("LibertyRecomp")

# ── zlib: system library on desktop, built from source for console cross-compilation ──
if (CMAKE_SYSTEM_NAME STREQUAL "PS4" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
    include(FetchContent)
    FetchContent_Declare(zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG        v1.3.1
    )
    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
    # zlib's cmake uses option(BUILD_SHARED_LIBS ... ON) which wins over cache,
    # so we have to set it before add_subdirectory via FetchContent.
    set(_saved_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
    set(BUILD_SHARED_LIBS OFF)
    FetchContent_MakeAvailable(zlib)
    # zlib unconditionally adds a SHARED target — exclude it from build
    if (TARGET zlib)
        set_target_properties(zlib PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()
    # Alias so target_link_libraries(... ZLIB::ZLIB) works
    if (NOT TARGET ZLIB::ZLIB)
        add_library(ZLIB::ZLIB ALIAS zlibstatic)
    endif()
    # Make <zlib.h> includable
    target_include_directories(zlibstatic PUBLIC
        ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
endif()

if (WIN32)
    option(LIBERTY_RECOMP_D3D12 "Add D3D12 support for rendering" ON)
endif()

if (APPLE)
    option(LIBERTY_RECOMP_METAL "Add Metal support for rendering" ON)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    option(LIBERTY_RECOMP_FLATPAK "Configure the build for Flatpak compatibility." OFF)
endif()

function(BIN2C)
    cmake_parse_arguments(BIN2C_ARGS "" "TARGET_OBJ;SOURCE_FILE;DEST_FILE;ARRAY_NAME;COMPRESSION_TYPE" "" ${ARGN})

    if(NOT BIN2C_ARGS_TARGET_OBJ)
        message(FATAL_ERROR "TARGET_OBJ not specified.")
    endif()

    if(NOT BIN2C_ARGS_SOURCE_FILE)
        message(FATAL_ERROR "SOURCE_FILE not specified.")
    endif()

    if(NOT BIN2C_ARGS_DEST_FILE)
        set(BIN2C_ARGS_DEST_FILE "${BIN2C_ARGS_SOURCE_FILE}")
    endif()

    if(NOT BIN2C_ARGS_COMPRESSION_TYPE)
        set(BIN2C_ARGS_COMPRESSION_TYPE "none")
    endif()

    # Only register the regeneration custom-command if the source .bin/.dds
    # actually exists on disk. The generated .c files are committed under
    # LibertyRecomp/res/, so builds without the external LibertyRecompResources
    # tree (now removed) can still link against the pre-generated sources.
    if(EXISTS "${BIN2C_ARGS_SOURCE_FILE}")
        if(TARGET file_to_c)
            # Desktop: use the native file_to_c tool
            add_custom_command(OUTPUT "${BIN2C_ARGS_DEST_FILE}.c"
                COMMAND $<TARGET_FILE:file_to_c> "${BIN2C_ARGS_SOURCE_FILE}" "${BIN2C_ARGS_ARRAY_NAME}" "${BIN2C_ARGS_COMPRESSION_TYPE}" "${BIN2C_ARGS_DEST_FILE}.c" "${BIN2C_ARGS_DEST_FILE}.h"
                DEPENDS "${BIN2C_ARGS_SOURCE_FILE}"
                BYPRODUCTS "${BIN2C_ARGS_DEST_FILE}.h"
                COMMENT "Generating binary header for ${BIN2C_ARGS_SOURCE_FILE}..."
            )
        else()
            # Cross-compile: use Python fallback
            add_custom_command(OUTPUT "${BIN2C_ARGS_DEST_FILE}.c"
                COMMAND ${CMAKE_COMMAND} -E env python3 "${CMAKE_SOURCE_DIR}/scripts/file_to_c.py"
                    "${BIN2C_ARGS_SOURCE_FILE}" "${BIN2C_ARGS_ARRAY_NAME}" "${BIN2C_ARGS_COMPRESSION_TYPE}"
                    "${BIN2C_ARGS_DEST_FILE}.c" "${BIN2C_ARGS_DEST_FILE}.h"
                DEPENDS "${BIN2C_ARGS_SOURCE_FILE}"
                BYPRODUCTS "${BIN2C_ARGS_DEST_FILE}.h"
                COMMENT "Generating binary header for ${BIN2C_ARGS_SOURCE_FILE} (Python)..."
            )
        endif()
    endif()

    # If the destination .c doesn't exist either, skip this asset entirely
    # (rare — only happens for a brand-new resource that was never committed).
    if(EXISTS "${BIN2C_ARGS_DEST_FILE}.c" OR EXISTS "${BIN2C_ARGS_SOURCE_FILE}")
        set_source_files_properties(${BIN2C_ARGS_DEST_FILE}.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
        target_sources(${BIN2C_ARGS_TARGET_OBJ} PRIVATE ${BIN2C_ARGS_DEST_FILE}.c)
    endif()
endfunction()

add_compile_options(
    -fno-strict-aliasing
    -Wno-switch
    -Wno-unused-function
    -Wno-unused-variable
    -Wno-unused-but-set-variable
    -Wno-void-pointer-to-int-cast
    -Wno-int-to-void-pointer-cast
    -Wno-invalid-offsetof
    -Wno-null-arithmetic
    -Wno-null-conversion
    -Wno-tautological-undefined-compare
)

if (WIN32)
    add_compile_options(/fp:strict)
else()
    add_compile_options(-ffp-model=strict)
endif()

add_compile_definitions(
    LIBERTY_RECOMP_OVERRIDES
    SDL_MAIN_HANDLED
    _DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR # Microsoft wtf?
    _CRT_SECURE_NO_WARNINGS)

# Detect target platform early so conditional source lists work correctly.
if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR CMAKE_SYSTEM_NAME STREQUAL "PS4")
    set(LIBERTY_RECOMP_PS4 TRUE)
    add_compile_definitions(LIBERTY_RECOMP_PS4)
endif()
if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
    set(LIBERTY_RECOMP_NX TRUE)
    add_compile_definitions(LIBERTY_RECOMP_NX)
endif()

set(LIBERTY_RECOMP_PRECOMPILED_HEADERS
    "stdafx.h"
)

set(LIBERTY_RECOMP_KERNEL_CXX_SOURCES
    # Remaining kernel files — thin adapters over rexglue runtime.
    # Duplicated subsystems (xam, xenon_memory, vfs, crt_init, save_system,
    # user_profile, menu_hooks, mod_overlay, game_init, game_fixes, network/
    # p2p/http/gxt/rpf/img_loader, kernel/imports, apu/) have been removed —
    # rexglue provides these natively via src/kernel/{xam,xboxkrnl,crt},
    # src/filesystem, src/audio. Do NOT re-add them.
    "kernel/heap.cpp"            # thin adapter over rexglue heap
    "kernel/memory.cpp"          # thin adapter — exposes g_memory with .base
                                 #   and .Translate(guest_addr) used by patches
    "kernel/xdm.cpp"             # Liberty KernelObject table (g_kernelLock,
                                 #   LookupKernelObject...) — consumed by
                                 #   cpu/guest_thread; TODO migrate to rex
    "kernel/compat_stubs.cpp"    # no-op stubs for symbols orphaned by
                                 #   imports.cpp removal: InitKernelMainThread,
                                 #   KernelPhase_EnterRuntime, sub_821966D0_hook,
                                 #   XamNotifyEnqueueEvent
    "kernel/lod_hooks.cpp"       # LOD rendering distance tuning
    "kernel/button_prompts.cpp"  # input UI (button prompt glyphs)
    "kernel/io/texture_convert.cpp"  # rendering — texture format conversion
)

set(LIBERTY_RECOMP_LOCALE_CXX_SOURCES
    "locale/config_locale.cpp"
    "locale/locale.cpp"
    "os/diag.cpp"
)

if (WIN32)
    set(LIBERTY_RECOMP_OS_CXX_SOURCES
        "os/win32/logger_win32.cpp"
        "os/win32/media_win32.cpp"
        "os/win32/process_win32.cpp"
        "os/win32/user_win32.cpp"
        "os/win32/version_win32.cpp"
        "os/discord/discord_presence.cpp"
    )
elseif (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ios" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(LIBERTY_RECOMP_OS_CXX_SOURCES
        "os/ios/logger_ios.cpp"
        "os/ios/media_ios.cpp"
        "os/ios/process_ios.cpp"
        "os/ios/user_ios.cpp"
        "os/ios/version_ios.cpp"
        "os/ios/ios_paths_objc.mm"
        "os/ios/haptics_ios.mm"
        "os/ios/haptics_ios.h"
        "os/ios/adaptive_triggers_ios.mm"
        "os/ios/adaptive_triggers_ios.h"
        "os/gamecenter/achievement_bridge_gc.mm"
    )
elseif (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "android" OR ANDROID)
    set(LIBERTY_RECOMP_OS_CXX_SOURCES
        "os/android/logger_android.cpp"
        "os/android/media_android.cpp"
        "os/android/process_android.cpp"
        "os/android/user_android.cpp"
        "os/android/version_android.cpp"
        "os/android/jni_glue.cpp"
        "os/android/vibration_android.cpp"
        "os/android/vibration_android.h"
        "os/android/achievement_bridge_android.cpp"
        "os/android/achievement_bridge_android.h"
    )
elseif (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR CMAKE_SYSTEM_NAME STREQUAL "PS4")
    set(LIBERTY_RECOMP_OS_CXX_SOURCES
        "os/ps4/logger_ps4.cpp"
        "os/ps4/media_ps4.cpp"
        "os/ps4/process_ps4.cpp"
        "os/ps4/user_ps4.cpp"
        "os/ps4/version_ps4.cpp"
        "os/ps4/ps4_main.cpp"
        "os/ps4/user_malloc_ps4.cpp"
        "os/ps4/trophy_ps4.cpp"
        "os/ps4/achievement_bridge_ps4.cpp"
        "os/ps4/np_presence_ps4.cpp"
        "os/ps4/savedata_ps4.cpp"
        "os/ps4/mspace_ps4.cpp"
        "os/ps4/np_activity_ps4.cpp"
        "os/ps4/atomic_wait_ps4.cpp"
    )
elseif (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
    set(LIBERTY_RECOMP_OS_CXX_SOURCES
        "os/switch/logger_switch.cpp"
        "os/switch/media_switch.cpp"
        "os/switch/process_switch.cpp"
        "os/switch/user_switch.cpp"
        "os/switch/version_switch.cpp"
        "os/switch/switch_main.cpp"
    )
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LIBERTY_RECOMP_OS_CXX_SOURCES
        "os/linux/logger_linux.cpp"
        "os/linux/media_linux.cpp"
        "os/linux/process_linux.cpp"
        "os/linux/user_linux.cpp"
        "os/linux/version_linux.cpp"
        "os/discord/discord_presence.cpp"
    )
elseif (APPLE)
    set(LIBERTY_RECOMP_OS_CXX_SOURCES
        "os/macos/logger_macos.cpp"
        "os/macos/media_macos.cpp"
        "os/macos/process_macos.cpp"
        "os/macos/user_macos.cpp"
        "os/macos/version_macos.cpp"
        "os/gamecenter/achievement_bridge_gc.mm"
        "os/discord/discord_presence.cpp"
    )
endif()

# ── Embedded assets compile definition ──────────────────────────────────────
if (LIBERTY_RECOMP_EMBEDDED_ASSETS)
    add_compile_definitions(
    LIBERTY_RECOMP_OVERRIDES
    LIBERTY_RECOMP_EMBEDDED_ASSETS)
    message(STATUS "Building with embedded game assets (no installer wizard)")
endif()

# ── Embedded audio resources (SDL_mixer installer sounds) ───────────────────
# Desktop only. PS4/Switch builds use their native audio pipelines and ship
# without the installer-era SDL_mixer ogg assets, so apu/embedded_player.cpp
# falls through to its no-op stub branch on those platforms.
# Requires SDL_mixer, which isn't wired into the vcpkg manifest. Leave
# disabled until someone adds the dependency; embedded_player.cpp has
# no-op fallback branches when this is undefined.
# if (NOT (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX))
#     add_compile_definitions(LIBERTY_RECOMP_HAS_AUDIO_RESOURCES)
# endif()

set(LIBERTY_RECOMP_CPU_CXX_SOURCES
    "cpu/guest_thread.cpp"
    "cpu/ppc_missing_stubs.cpp"
)

set(LIBERTY_RECOMP_GPU_CXX_SOURCES
    "gpu/video.cpp"
    "gpu/gfx_state.cpp"
    "gpu/gtaiv_render_state.cpp"
    "gpu/postprocess_aa.cpp"
    "gpu/postprocess_renderer.cpp"
    "gpu/upscaler.cpp"
    "gpu/shadow_quality.cpp"
    "gpu/camera_extract.cpp"
    "gpu/imgui/imgui_common.cpp"
    "gpu/imgui/imgui_font_builder.cpp"
    "gpu/imgui/imgui_snapshot.cpp"
)

# Windows-only upscaler implementations (require D3D12)
if (WIN32)
    list(APPEND LIBERTY_RECOMP_GPU_CXX_SOURCES
        "gpu/upscaler_xess.cpp"
        "gpu/upscaler_fsr3.cpp"
        "gpu/upscaler_dlss.cpp"
    )
endif()

if (APPLE)
    list(APPEND LIBERTY_RECOMP_GPU_CXX_SOURCES
        "gpu/upscaler_metalfx.mm"
    )
endif()

# APU (audio) sources — removed. rexglue handles all audio natively via
# its src/audio + src/kernel/xboxkrnl/xboxkrnl_audio*.cpp layers.
set(LIBERTY_RECOMP_APU_CXX_SOURCES)

# Select HID driver per platform
if (LIBERTY_RECOMP_PS4)
    set(LIBERTY_RECOMP_HID_CXX_SOURCES
        "hid/hid.cpp"
        "hid/driver/ps4_hid.cpp"
    )
elseif (LIBERTY_RECOMP_NX)
    set(LIBERTY_RECOMP_HID_CXX_SOURCES
        "hid/hid.cpp"
        "hid/driver/switch_hid.cpp"
    )
else()
    set(LIBERTY_RECOMP_HID_CXX_SOURCES
        "hid/hid.cpp"
        "hid/mouse_camera.cpp"
        "hid/dualsense.cpp"
        "hid/driver/sdl_hid.cpp"
    )
endif()

set(LIBERTY_RECOMP_PATCHES_CXX_SOURCES
    # Input
    "patches/gta4_input_patches.cpp"         # GTA IV input remapping
    "patches/gta4_motion_patches.cpp"        # GTA IV motion (PlayStation gyro/accel)
    "patches/gta4_ds4_patches.cpp"           # DualShock 4/DualSense features

    # Rendering
    "patches/gta4_aspect_ratio_patches.cpp"
    "patches/grm_setup_patches.cpp"          # rage::grmSetup singleton
    "patches/postfx_hooks.cpp"               # PostFX (bloom, sun shafts)
    "patches/scene_render_patches.cpp"       # Scene render pipeline
    "patches/render_phase_patches.cpp"       # RAGE render phase system
    "patches/shader_collection_patches.cpp"  # fiCollection shader VFS mount chain
    "patches/shader_factory_patches.cpp"     # grmShaderFactory device guard bypass
)

set(LIBERTY_RECOMP_UI_CXX_SOURCES
    "ui/achievement_menu.cpp"
    "ui/achievement_overlay.cpp"
    "ui/black_bar.cpp"
    "ui/button_window.cpp"
    "ui/common_menu.cpp"
    "ui/fader.cpp"
    "ui/game_window.cpp"
    "ui/imgui_utils.cpp"
    "ui/main_menu.cpp"             # Main menu launcher (GTA IV style)
    "ui/message_window_stub.cpp"   # Stub - GTA IV version needed
    "ui/options_menu_stub.cpp"     # Stub - GTA IV version needed
)
# Installer wizard — desktop only (uses NFD file dialogs)
if (NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4"   OR CMAKE_SYSTEM_NAME STREQUAL "PS4" OR
         LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch"))
    list(APPEND LIBERTY_RECOMP_UI_CXX_SOURCES "ui/installer_wizard.cpp")
endif()

# Console builds (PS4/Switch) are full embedded archives — no installer, no update checker.
if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4"   OR CMAKE_SYSTEM_NAME STREQUAL "PS4" OR
    LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
    set(LIBERTY_RECOMP_INSTALL_CXX_SOURCES
        "install/platform_paths.cpp"
        "install/embedded_assets.cpp"
    )
else()
    set(LIBERTY_RECOMP_INSTALL_CXX_SOURCES
        "install/auto_installer.cpp"
        "install/installer.cpp"
        "install/iso_file_system.cpp"
        "install/iso_extractor.cpp"
        "install/platform_paths.cpp"
        "install/embedded_assets.cpp"
        "install/rpf_extractor.cpp"
        "install/shader_converter.cpp"
        "install/thread_pool.cpp"
        "install/update_checker.cpp"
        "install/xcontent_file_system.cpp"
        "install/xbox360/stfs_parser.cpp"
        "install/xbox360/xex_patcher.cpp"
        "install/xbox360/xex_validator.cpp"
        "install/xbox360/title_update_manager.cpp"
        "install/hashes/game.cpp"
        "install/hashes/dlc_tlad.cpp"
        "install/hashes/dlc_tbogt.cpp"
        "install/hashes/title_update.cpp"
    )
endif()

set(LIBERTY_RECOMP_USER_CXX_SOURCES
    "user/achievement_data.cpp"
    "user/achievement_manager.cpp"
    "user/config.cpp"
    "user/persistent_storage_manager.cpp"  # GTA IV save data handling
    "user/registry.cpp"
    "user/paths.cpp"
)

set(LIBERTY_RECOMP_MOD_CXX_SOURCES
    "mod/mod_loader.cpp"
)

set(LIBERTY_RECOMP_UTILS_CXX_SOURCES
    "utils/bit_stream.cpp"
    "utils/ring_buffer.cpp"
)

# RexGlue runtime integration (headers-only mode for now)
set(LIBERTY_RECOMP_RUNTIME_CXX_SOURCES
    # rex_adapter.h is header-only, no .cpp files yet
)

set(LIBERTY_RECOMP_THIRDPARTY_SOURCES
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/imgui/imgui.cpp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/imgui/imgui_demo.cpp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/imgui/imgui_draw.cpp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/imgui/imgui_tables.cpp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/imgui/imgui_widgets.cpp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/implot/implot.cpp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/implot/implot_demo.cpp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/implot/implot_items.cpp"
    "${LIBERTY_RECOMP_TOOLS_ROOT}/XenosRecomp/thirdparty/smol-v/source/smolv.cpp"
)
# imgui SDL3 backend — desktop only (consoles don't use SDL3)
if (NOT (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX))
    list(APPEND LIBERTY_RECOMP_THIRDPARTY_SOURCES
        "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/imgui/backends/imgui_impl_sdl3.cpp"
    )
endif()

set(LIBERTY_RECOMP_THIRDPARTY_INCLUDES
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/concurrentqueue"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/ddspp"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/imgui"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/implot"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/json/include"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/magic_enum/include"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/stb"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/unordered_dense/include"
    "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/GameNetworkingSockets/include"
    "${LIBERTY_RECOMP_TOOLS_ROOT}/bc_diff"
    "${LIBERTY_RECOMP_TOOLS_ROOT}/XenosRecomp/thirdparty/smol-v/source"
    # Upscaler SDKs
    "${LIBERTY_RECOMP_TOOLS_ROOT}/FidelityFX-SDK/Kits/FidelityFX"
    "${LIBERTY_RECOMP_TOOLS_ROOT}/DLSS/include"
    "${LIBERTY_RECOMP_TOOLS_ROOT}/xess/inc"
    # Note: graine rex::* targets propagate include dirs via INTERFACE_INCLUDE_DIRECTORIES
)

set_source_files_properties(${LIBERTY_RECOMP_THIRDPARTY_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

set(LIBERTY_RECOMP_CXX_SOURCES
    "app.cpp"
    "exports.cpp"
    "main.cpp"
    "misc_impl.cpp"
    "preload_executable.cpp"
    "stdafx.cpp"
    "version.cpp"
    
    ${LIBERTY_RECOMP_KERNEL_CXX_SOURCES}
    ${LIBERTY_RECOMP_LOCALE_CXX_SOURCES}
    ${LIBERTY_RECOMP_OS_CXX_SOURCES}
    ${LIBERTY_RECOMP_CPU_CXX_SOURCES}
    ${LIBERTY_RECOMP_GPU_CXX_SOURCES}
    ${LIBERTY_RECOMP_APU_CXX_SOURCES}
    ${LIBERTY_RECOMP_HID_CXX_SOURCES}
    ${LIBERTY_RECOMP_PATCHES_CXX_SOURCES}
    ${LIBERTY_RECOMP_UI_CXX_SOURCES}
    ${LIBERTY_RECOMP_INSTALL_CXX_SOURCES}
    ${LIBERTY_RECOMP_USER_CXX_SOURCES}
    ${LIBERTY_RECOMP_UTILS_CXX_SOURCES}
    ${LIBERTY_RECOMP_MOD_CXX_SOURCES}
    ${LIBERTY_RECOMP_RUNTIME_CXX_SOURCES}
    ${LIBERTY_RECOMP_THIRDPARTY_SOURCES}
)

# sdl_listener.cpp uses SDL_Event — desktop only (consoles don't use SDL3)
if (NOT (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX))
    list(APPEND LIBERTY_RECOMP_CXX_SOURCES "sdl_listener.cpp")
endif()

include("version.cmake")

set(VERSION_TXT "${PROJECT_SOURCE_DIR}/res/version.txt")

# Only show Git info and build type if not Release.
set(SHOW_GIT_INFO_AND_BUILD_TYPE 0)
if (NOT ${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(SHOW_GIT_INFO_AND_BUILD_TYPE 1)
endif()

if (LIBERTY_RECOMP_METAL)
    set(XCRUN_TOOL "/usr/bin/xcrun")
    # Select the correct Metal SDK: iphoneos for iOS, macosx for macOS
    if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ios" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(LIBERTY_RECOMP_METAL_SDK "iphoneos")
    else()
        set(LIBERTY_RECOMP_METAL_SDK "macosx")
    endif()
endif()

GenerateVersionSources(
    OUTPUT_DIR ${PROJECT_SOURCE_DIR}
    VERSION_TXT ${VERSION_TXT}
    H_TEMPLATE "${PROJECT_SOURCE_DIR}/res/version.h.template"
    CXX_TEMPLATE "${PROJECT_SOURCE_DIR}/res/version.cpp.template"
    BUILD_TYPE ${CMAKE_BUILD_TYPE}
    SHOW_GIT_INFO ${SHOW_GIT_INFO_AND_BUILD_TYPE}
    SHOW_BUILD_TYPE ${SHOW_GIT_INFO_AND_BUILD_TYPE}
)

if (WIN32)
    # Create binary version number for Win32 integer attributes.
    CreateVersionString(
        VERSION_TXT ${VERSION_TXT}
        OUTPUT_CSV 1
        OUTPUT_VAR WIN32_VERSION_BINARY
    )

    # Create string version number for Win32 detailed attributes.
    CreateVersionString(
        VERSION_TXT ${VERSION_TXT}
        BUILD_TYPE ${CMAKE_BUILD_TYPE}
        SHOW_GIT_INFO ${SHOW_GIT_INFO_AND_BUILD_TYPE}
        SHOW_BUILD_TYPE ${SHOW_GIT_INFO_AND_BUILD_TYPE}
        OUTPUT_VAR WIN32_VERSION_STRING
    )

    # Set Win32 icon path.
    set(WIN32_ICON_PATH "${PROJECT_SOURCE_DIR}/../LibertyRecompResources/images/game_icon.ico")

    configure_file("res/win32/res.rc.template" "${CMAKE_BINARY_DIR}/res.rc" @ONLY)
    add_executable(LibertyRecomp ${LIBERTY_RECOMP_CXX_SOURCES} "${CMAKE_BINARY_DIR}/res.rc")

    # Hide console for release configurations.
    if (${CMAKE_BUILD_TYPE} MATCHES "Release")
        target_link_options(LibertyRecomp PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
    endif()
elseif (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ios" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # ── iOS App Bundle ─────────────────────────────────────────────────────────
    CreateVersionString(
            VERSION_TXT ${VERSION_TXT}
            OUTPUT_VAR IOS_BUNDLE_VERSION
    )

    add_executable(LibertyRecomp MACOSX_BUNDLE
            ${LIBERTY_RECOMP_CXX_SOURCES}
            "res/ios/Assets.xcassets"
            "res/ios/LaunchScreen.storyboard"
    )
    set_target_properties(LibertyRecomp PROPERTIES
            OUTPUT_NAME "Liberty Recompiled"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/res/ios/iOSBundleInfo.plist.in"
            MACOSX_BUNDLE_GUI_IDENTIFIER "${LIBERTY_IOS_BUNDLE_IDENTIFIER}"
            MACOSX_BUNDLE_BUNDLE_NAME "Liberty Recompiled"
            MACOSX_BUNDLE_BUNDLE_VERSION ${IOS_BUNDLE_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${IOS_BUNDLE_VERSION}
            MACOSX_BUNDLE_ICON_FILE "AppIcon"
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${LIBERTY_IOS_BUNDLE_IDENTIFIER}"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${LIBERTY_IOS_DEVELOPMENT_TEAM}"
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
            XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
            XCODE_ATTRIBUTE_INFOPLIST_KEY_UILaunchStoryboardName "LaunchScreen"
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
            XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "16.0"
            XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
    )

    # Optional manual provisioning profile (advanced users). Leave blank for automatic signing.
    if (LIBERTY_IOS_PROVISIONING_PROFILE)
        set_target_properties(LibertyRecomp PROPERTIES
                XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
                XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER "${LIBERTY_IOS_PROVISIONING_PROFILE}"
        )
    endif()

    if (NOT LIBERTY_IOS_DEVELOPMENT_TEAM)
        message(WARNING
            "LIBERTY_IOS_DEVELOPMENT_TEAM is not set. Device builds will fail to sign. "
            "Pass -DLIBERTY_IOS_DEVELOPMENT_TEAM=<TEAM_ID> or use scripts/ios-setup.sh.")
    endif()

    # Post-build: copy game payload into the app bundle at <Bundle>/game/
    if (LIBERTY_RECOMP_EMBEDDED_GAME_PATH)
        add_custom_command(TARGET LibertyRecomp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}"
                "$<TARGET_BUNDLE_CONTENT_DIR:LibertyRecomp>/game"
            COMMENT "Staging game payload into iOS bundle..."
        )
    endif()

elseif (APPLE)
    # ── macOS App Bundle ───────────────────────────────────────────────────────
    CreateVersionString(
            VERSION_TXT ${VERSION_TXT}
            OUTPUT_VAR MACOS_BUNDLE_VERSION
    )

    add_executable(LibertyRecomp MACOSX_BUNDLE
            ${LIBERTY_RECOMP_CXX_SOURCES}
            res/macos/game_icon.icns
    )
    set_source_files_properties(res/macos/game_icon.icns PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources)
    set_target_properties(LibertyRecomp PROPERTIES
            OUTPUT_NAME "Liberty Recompiled"
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/macos/MacOSXBundleInfo.plist.in
            MACOSX_BUNDLE_GUI_IDENTIFIER OZORDI.LibertyRecomp
            MACOSX_BUNDLE_BUNDLE_NAME "Liberty Recompiled"
            MACOSX_BUNDLE_BUNDLE_VERSION ${MACOS_BUNDLE_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${MACOS_BUNDLE_VERSION}
            MACOSX_BUNDLE_ICON_FILE "game_icon.icns"
    )

    # Linking with MoltenVK directly would prevent using the system Vulkan loader to load with debug layers.
    # Instead, copy the MoltenVK dylib to the app bundle along with an ICD file for the loader to find it.
    # In the event the loader is not installed, the MoltenVK dylib can still be picked up directly in the app bundle.
    set(MVK_BUNDLED_PATH "Resources/vulkan/icd.d")
    set(MVK_DST "${CMAKE_CURRENT_BINARY_DIR}/Liberty Recompiled.app/Contents/${MVK_BUNDLED_PATH}")
    set_property(TARGET LibertyRecomp APPEND PROPERTY BUILD_RPATH "@executable_path/../${MVK_BUNDLED_PATH}")

    set(MVK_ICD_SRC "${LIBERTY_RECOMP_THIRDPARTY_ROOT}/MoltenVK/MoltenVK/MoltenVK/icd/MoltenVK_icd.json")
    set(MVK_ICD_DST "${MVK_DST}/MoltenVK_icd.json")
    set(MVK_DYLIB_SRC "${CMAKE_BINARY_DIR}/thirdparty/MoltenVK/libMoltenVK.dylib")
    set(MVK_DYLIB_DST "${MVK_DST}/libMoltenVK.dylib")

    add_custom_command(
            OUTPUT ${MVK_DST}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MVK_DST})
    add_custom_command(
            OUTPUT ${MVK_ICD_DST}
            DEPENDS ${MVK_ICD_SRC} ${MVK_DST}
            COMMAND ${CMAKE_COMMAND} -E copy ${MVK_ICD_SRC} ${MVK_ICD_DST})
    add_custom_command(
            OUTPUT ${MVK_DYLIB_DST}
            DEPENDS ${MVK_DYLIB_SRC} ${MVK_DST}
            COMMAND ${CMAKE_COMMAND} -E copy ${MVK_DYLIB_SRC} ${MVK_DYLIB_DST})
    add_custom_target(CopyMoltenVK DEPENDS ${MVK_ICD_DST} ${MVK_DYLIB_DST})
    add_dependencies(CopyMoltenVK MoltenVK)
    add_dependencies(LibertyRecomp CopyMoltenVK)
else()
    add_executable(LibertyRecomp ${LIBERTY_RECOMP_CXX_SOURCES})

    # ── Switch: NRO packaging (post-build) ─────────────────────────────────────
    if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
        # Manifest sources (JSON, human-editable, checked into res/switch/)
        set(LIBERTY_SWITCH_CONTROL_JSON "${CMAKE_CURRENT_SOURCE_DIR}/res/switch/control.json")
        set(LIBERTY_SWITCH_NPDM_JSON    "${CMAKE_CURRENT_SOURCE_DIR}/res/switch/main.npdm.json")
        set(LIBERTY_SWITCH_ICON         "${CMAKE_CURRENT_SOURCE_DIR}/res/switch/icon.jpg")

        # NACP metadata — shown in homebrew menu and Switch system UI.
        # Defaults are pulled from control.json at configure time so the JSON
        # stays the single source of truth; literals below are used only if the
        # JSON is absent or the field is missing.
        set(LIBERTY_SWITCH_APP_TITLE   "Liberty Recompiled")
        set(LIBERTY_SWITCH_APP_AUTHOR  "LibertyRecomp Contributors")
        set(LIBERTY_SWITCH_APP_VERSION "1.0.0")
        if (EXISTS "${LIBERTY_SWITCH_CONTROL_JSON}")
            file(READ "${LIBERTY_SWITCH_CONTROL_JSON}" _SWITCH_CONTROL_RAW)
            string(JSON _SWITCH_NAME    ERROR_VARIABLE _e1 GET "${_SWITCH_CONTROL_RAW}" name)
            string(JSON _SWITCH_AUTHOR  ERROR_VARIABLE _e2 GET "${_SWITCH_CONTROL_RAW}" author)
            string(JSON _SWITCH_VERSION ERROR_VARIABLE _e3 GET "${_SWITCH_CONTROL_RAW}" version)
            if (NOT _e1)
                set(LIBERTY_SWITCH_APP_TITLE   "${_SWITCH_NAME}")
            endif()
            if (NOT _e2)
                set(LIBERTY_SWITCH_APP_AUTHOR  "${_SWITCH_AUTHOR}")
            endif()
            if (NOT _e3)
                set(LIBERTY_SWITCH_APP_VERSION "${_SWITCH_VERSION}")
            endif()
        endif()
        message(STATUS "Switch manifest: ${LIBERTY_SWITCH_APP_TITLE} by ${LIBERTY_SWITCH_APP_AUTHOR} v${LIBERTY_SWITCH_APP_VERSION}")

        # RomFS staging tree — mirrors the layout libnx exposes at `romfs:/`.
        # The tracked tree at res/switch/romfs/ ships empty .gitkeep markers;
        # the build populates game/, dlc/ and aes_key.bin per-build and elf2nro
        # packs the whole directory into the NRO.
        set(LIBERTY_SWITCH_ROMFS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/res/switch/romfs")
        set(LIBERTY_SWITCH_ROMFS_DIR    "${CMAKE_CURRENT_BINARY_DIR}/switch_romfs"
            CACHE PATH "Switch RomFS staging directory packed into the NRO")

        # Step 1: seed the staging tree from the tracked skeleton (cheap — just
        # .gitkeep markers), then populate the payload subdirectories.
        add_custom_command(TARGET LibertyRecomp PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_SWITCH_ROMFS_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${LIBERTY_SWITCH_ROMFS_SOURCE}" "${LIBERTY_SWITCH_ROMFS_DIR}"
            COMMENT "Switch: seeding RomFS staging tree at ${LIBERTY_SWITCH_ROMFS_DIR}"
        )

        if (LIBERTY_RECOMP_EMBEDDED_GAME_PATH)
            add_custom_command(TARGET LibertyRecomp PRE_LINK
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}"
                    "${LIBERTY_SWITCH_ROMFS_DIR}/game"
                COMMENT "Switch: staging game payload into romfs/game/"
            )
        endif()

        # DLC: prefer an explicit path, otherwise fall back to
        # ${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/dlc if it exists.
        set(_liberty_switch_dlc_src "")
        if (LIBERTY_RECOMP_EMBEDDED_DLC_PATH AND EXISTS "${LIBERTY_RECOMP_EMBEDDED_DLC_PATH}")
            set(_liberty_switch_dlc_src "${LIBERTY_RECOMP_EMBEDDED_DLC_PATH}")
        elseif (LIBERTY_RECOMP_EMBEDDED_GAME_PATH AND
                EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/dlc")
            set(_liberty_switch_dlc_src "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/dlc")
        endif()
        if (_liberty_switch_dlc_src)
            add_custom_command(TARGET LibertyRecomp PRE_LINK
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${_liberty_switch_dlc_src}"
                    "${LIBERTY_SWITCH_ROMFS_DIR}/dlc"
                COMMENT "Switch: staging DLC payload into romfs/dlc/ from ${_liberty_switch_dlc_src}"
            )
        endif()

        # AES key is consumed by the XEX loader at runtime. Only copy when
        # provided; absence is a legitimate configuration (e.g. CI dry-run).
        if (LIBERTY_RECOMP_EMBEDDED_AES_KEY AND EXISTS "${LIBERTY_RECOMP_EMBEDDED_AES_KEY}")
            add_custom_command(TARGET LibertyRecomp PRE_LINK
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${LIBERTY_RECOMP_EMBEDDED_AES_KEY}"
                    "${LIBERTY_SWITCH_ROMFS_DIR}/aes_key.bin"
                COMMENT "Switch: staging XEX AES key into romfs/aes_key.bin"
            )
        endif()

        # devkitPro tools (switch-tools package under devkitA64).
        find_program(NACPTOOL_EXE nacptool HINTS "${DEVKITPRO}/tools/bin" "$ENV{DEVKITPRO}/tools/bin")
        find_program(ELF2NRO_EXE  elf2nro  HINTS "${DEVKITPRO}/tools/bin" "$ENV{DEVKITPRO}/tools/bin")
        find_program(NPDMTOOL_EXE npdmtool HINTS "${DEVKITPRO}/tools/bin" "$ENV{DEVKITPRO}/tools/bin")

        # control.nacp — regenerated whenever control.json changes.
        set(LIBERTY_SWITCH_NACP_OUT "${CMAKE_CURRENT_BINARY_DIR}/LibertyRecomp.nacp")
        if (NACPTOOL_EXE)
            add_custom_command(
                OUTPUT  "${LIBERTY_SWITCH_NACP_OUT}"
                COMMAND ${NACPTOOL_EXE} --create
                        "${LIBERTY_SWITCH_APP_TITLE}"
                        "${LIBERTY_SWITCH_APP_AUTHOR}"
                        "${LIBERTY_SWITCH_APP_VERSION}"
                        "${LIBERTY_SWITCH_NACP_OUT}"
                DEPENDS "${LIBERTY_SWITCH_CONTROL_JSON}"
                COMMENT "Generating Switch control.nacp from control.json"
                VERBATIM
            )
            add_custom_target(LibertyRecompSwitchNacp DEPENDS "${LIBERTY_SWITCH_NACP_OUT}")
            add_dependencies(LibertyRecomp LibertyRecompSwitchNacp)
        else()
            message(WARNING "nacptool not found – Switch control.nacp will not be generated (set DEVKITPRO).")
        endif()

        # main.npdm — only needed for NSP / exefs builds (NRO ignores it), but
        # we emit it whenever npdmtool is available so CI artefacts are ready
        # for a downstream hacbrewpack / nsp-forwarder step.
        set(LIBERTY_SWITCH_NPDM_OUT "${CMAKE_CURRENT_BINARY_DIR}/main.npdm")
        if (NPDMTOOL_EXE)
            add_custom_command(
                OUTPUT  "${LIBERTY_SWITCH_NPDM_OUT}"
                COMMAND ${NPDMTOOL_EXE}
                        "${LIBERTY_SWITCH_NPDM_JSON}"
                        "${LIBERTY_SWITCH_NPDM_OUT}"
                DEPENDS "${LIBERTY_SWITCH_NPDM_JSON}"
                COMMENT "Generating Switch main.npdm from main.npdm.json"
                VERBATIM
            )
            add_custom_target(LibertyRecompSwitchNpdm DEPENDS "${LIBERTY_SWITCH_NPDM_OUT}")
            add_dependencies(LibertyRecomp LibertyRecompSwitchNpdm)
        else()
            message(STATUS "npdmtool not found – main.npdm not generated (only needed for NSP/exefs builds).")
        endif()

        # NRO packaging: pulls nacp + icon + romfs into the final .nro.
        if (NACPTOOL_EXE AND ELF2NRO_EXE)
            add_custom_command(TARGET LibertyRecomp POST_BUILD
                COMMAND ${ELF2NRO_EXE}
                    "$<TARGET_FILE:LibertyRecomp>"
                    "$<TARGET_FILE_DIR:LibertyRecomp>/LibertyRecomp.nro"
                    "--nacp=${LIBERTY_SWITCH_NACP_OUT}"
                    "--icon=${LIBERTY_SWITCH_ICON}"
                    "--romfsdir=${LIBERTY_SWITCH_ROMFS_DIR}"
                COMMENT "Packaging Switch NRO with RomFS from ${LIBERTY_SWITCH_ROMFS_DIR}..."
                VERBATIM
            )
        else()
            message(WARNING "nacptool or elf2nro not found – Switch NRO will not be packaged automatically.")
        endif()

        # ── NSP packaging (hacbrewpack) ────────────────────────────────────
        # Produces an Atmosphere-installable NSP alongside the NRO. Requires
        # hacbrewpack on PATH or under ${DEVKITPRO}/tools/bin, plus the
        # npdmtool / nacptool / elf2nro pipeline above.
        #
        # NOTE: stock (unmodified) Switch consoles cannot install these NSPs.
        # They are signed with a zero title-key and development keygeneration,
        # which only Atmosphere CFW accepts. Retail-signed NSPs would need
        # Nintendo signing keys that are not available to homebrew developers.
        find_program(HACBREWPACK_EXE hacbrewpack
            HINTS "${DEVKITPRO}/tools/bin" "$ENV{DEVKITPRO}/tools/bin"
                  "/usr/local/bin" "/opt/homebrew/bin")

        if (HACBREWPACK_EXE AND NPDMTOOL_EXE AND NACPTOOL_EXE AND ELF2NRO_EXE)
            set(LIBERTY_SWITCH_NSP_STAGING "${CMAKE_CURRENT_BINARY_DIR}/hacbrewpack_input")
            set(LIBERTY_SWITCH_NSP_OUTDIR  "${CMAKE_CURRENT_BINARY_DIR}/hacbrewpack_nsp")
            set(LIBERTY_SWITCH_NSP_TEMPDIR "${CMAKE_CURRENT_BINARY_DIR}/hacbrewpack_temp")
            set(LIBERTY_SWITCH_NSP_NCADIR  "${CMAKE_CURRENT_BINARY_DIR}/hacbrewpack_nca")

            # Title ID: 0x0100_0000_0001_0000 is the conventional dev id used
            # by nx-hbmenu forwarders. Override with
            # -DLIBERTY_SWITCH_NSP_TITLEID=01xxxxxxxxxxxxxx at configure time.
            if (NOT DEFINED LIBERTY_SWITCH_NSP_TITLEID)
                set(LIBERTY_SWITCH_NSP_TITLEID "0100000000010000")
            endif()

            add_custom_target(LibertyRecompSwitchNsp ALL
                # Fresh staging tree every build.
                COMMAND ${CMAKE_COMMAND} -E rm -rf
                    "${LIBERTY_SWITCH_NSP_STAGING}"
                    "${LIBERTY_SWITCH_NSP_OUTDIR}"
                    "${LIBERTY_SWITCH_NSP_TEMPDIR}"
                    "${LIBERTY_SWITCH_NSP_NCADIR}"
                COMMAND ${CMAKE_COMMAND} -E make_directory
                    "${LIBERTY_SWITCH_NSP_STAGING}/exefs"
                    "${LIBERTY_SWITCH_NSP_STAGING}/control"
                    "${LIBERTY_SWITCH_NSP_STAGING}/logo"
                    "${LIBERTY_SWITCH_NSP_STAGING}/romfs"

                # exefs: NRO acts as `main`, npdm alongside. Installed via a
                # nx-hbmenu forwarder; for a native NSO-based exefs, drop a
                # compiled NSO in as `main` instead.
                COMMAND ${CMAKE_COMMAND} -E copy
                    "$<TARGET_FILE_DIR:LibertyRecomp>/LibertyRecomp.nro"
                    "${LIBERTY_SWITCH_NSP_STAGING}/exefs/main"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${LIBERTY_SWITCH_NPDM_OUT}"
                    "${LIBERTY_SWITCH_NSP_STAGING}/exefs/main.npdm"

                # control: NACP + icon (hacbrewpack picks up icon_*.dat).
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${LIBERTY_SWITCH_NACP_OUT}"
                    "${LIBERTY_SWITCH_NSP_STAGING}/control/control.nacp"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${LIBERTY_SWITCH_ICON}"
                    "${LIBERTY_SWITCH_NSP_STAGING}/control/icon_AmericanEnglish.dat"

                # romfs: mirror the NRO staging tree so installed and loose
                # builds see identical asset layouts.
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${LIBERTY_SWITCH_ROMFS_DIR}"
                    "${LIBERTY_SWITCH_NSP_STAGING}/romfs"

                # hacbrewpack: dev keys / zero title key → CFW-only install.
                # --nologo skips the NintendoLogo NCA section (we don't ship
                # one); remove it once a 160x40 PNG is placed under logo/.
                COMMAND ${HACBREWPACK_EXE}
                    --titleid       ${LIBERTY_SWITCH_NSP_TITLEID}
                    --titlekey      00000000000000000000000000000000
                    --keygeneration 6
                    --nologo
                    --exefsdir      "${LIBERTY_SWITCH_NSP_STAGING}/exefs"
                    --controldir    "${LIBERTY_SWITCH_NSP_STAGING}/control"
                    --romfsdir      "${LIBERTY_SWITCH_NSP_STAGING}/romfs"
                    --nspdir        "${LIBERTY_SWITCH_NSP_OUTDIR}"
                    --ncadir        "${LIBERTY_SWITCH_NSP_NCADIR}"
                    --tempdir       "${LIBERTY_SWITCH_NSP_TEMPDIR}"

                # Publish the signed NSP next to the NRO for convenience.
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${LIBERTY_SWITCH_NSP_OUTDIR}/${LIBERTY_SWITCH_NSP_TITLEID}.nsp"
                    "$<TARGET_FILE_DIR:LibertyRecomp>/LibertyRecomp.nsp"

                WORKING_DIRECTORY "${LIBERTY_SWITCH_NSP_STAGING}"
                COMMENT "Packaging Switch NSP (Atmosphere CFW only, title id ${LIBERTY_SWITCH_NSP_TITLEID})"
                VERBATIM
            )
            # NSP depends on NRO + npdm + nacp being ready.
            add_dependencies(LibertyRecompSwitchNsp LibertyRecomp)
            if (TARGET LibertyRecompSwitchNpdm)
                add_dependencies(LibertyRecompSwitchNsp LibertyRecompSwitchNpdm)
            endif()
            if (TARGET LibertyRecompSwitchNacp)
                add_dependencies(LibertyRecompSwitchNsp LibertyRecompSwitchNacp)
            endif()
        else()
            if (NOT HACBREWPACK_EXE)
                message(STATUS "hacbrewpack not found – Switch NSP will not be built "
                               "(install from https://github.com/The-4n/hacBrewPack "
                               "or set DEVKITPRO). NRO output is unaffected.")
            else()
                message(STATUS "Switch NSP skipped – needs hacbrewpack, npdmtool, "
                               "nacptool and elf2nro all present.")
            endif()
        endif()
    endif()

    # ── PS4: FSELF + param.sfo / trophy.trp / PKG pipeline ─────────────────────
    if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR CMAKE_SYSTEM_NAME STREQUAL "PS4")
        set(LIBERTY_PS4_TOOLCHAIN   "${CMAKE_SOURCE_DIR}/tools/OpenOrbis-PS4-Toolchain")
        set(LIBERTY_PS4_FSELF_PY    "${LIBERTY_PS4_TOOLCHAIN}/scripts/make_fself.py")
        set(LIBERTY_PS4_SFO_PY      "${LIBERTY_PS4_TOOLCHAIN}/scripts/make_param_sfo.py")
        set(LIBERTY_PS4_TRP_PY      "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/create_trp.py")
        set(LIBERTY_PS4_TRP_VALIDATE_PY "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/validate_trp.py")
        set(LIBERTY_PS4_PKG_STAGING "${CMAKE_BINARY_DIR}/pkg_staging")
        set(LIBERTY_PS4_SCE_SYS_DIR "${LIBERTY_PS4_PKG_STAGING}/sce_sys")
        set(LIBERTY_PS4_TROPHY_SRC  "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/trophy00")
        set(LIBERTY_PS4_NPBIND_TEMPLATE  "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/npbind.dat")
        set(LIBERTY_PS4_NPTITLE_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/nptitle.dat")
        set(LIBERTY_PS4_NP_CONF_H   "${CMAKE_CURRENT_SOURCE_DIR}/os/ps4/np_conf_ps4.h")

        # ── Parse np_conf_ps4.h so CMake and the runtime stay in sync ─────────
        # Update the header (not this CMakeLists) to change title / NP IDs.
        file(READ "${LIBERTY_PS4_NP_CONF_H}" _np_conf_src)
        string(REGEX MATCH
            "LIBERTY_PS4_TITLE_ID_STR[ \t]+\"([A-Z0-9_]+)\""
            _dummy "${_np_conf_src}")
        set(LIBERTY_PS4_TITLE_ID "${CMAKE_MATCH_1}")
        string(REGEX MATCH
            "LIBERTY_PS4_CONTENT_ID_STR[ \t]+\"([A-Z0-9_\\-]+)\""
            _dummy "${_np_conf_src}")
        set(LIBERTY_PS4_CONTENT_ID "${CMAKE_MATCH_1}")
        if (NOT LIBERTY_PS4_TITLE_ID OR NOT LIBERTY_PS4_CONTENT_ID)
            message(FATAL_ERROR
                "Failed to parse TITLE_ID / CONTENT_ID from ${LIBERTY_PS4_NP_CONF_H}")
        endif()
        set(LIBERTY_PS4_VERSION     "01.00")
        set(LIBERTY_PS4_TITLE_STR   "Liberty Recompiled")
        set(LIBERTY_PS4_CATEGORY    "gd")  # gd = game data

        message(STATUS "PS4: TITLE_ID=${LIBERTY_PS4_TITLE_ID}  CONTENT_ID=${LIBERTY_PS4_CONTENT_ID}")

        # Detect host OS for toolchain bin subdir.
        # OpenOrbis ships per-host binaries with platform-specific names:
        #   macos   -> create-fself-macos (Go), create-gp4, PkgTool.Core
        #   linux   -> create-fself        (Go), create-gp4, PkgTool.Core
        #   windows -> create-fself.exe    (Go), create-gp4.exe, PkgTool.Core.exe
        # WSL2 users should treat the build as Linux and use bin/linux/*.
        #
        # If a binary is missing on your host (partial toolchain checkout),
        # obtain it from:
        #   - Prebuilt releases:
        #     https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/releases
        #   - Source: git clone the toolchain and `go build` under
        #     tools/OpenOrbis-PS4-Toolchain/src/tools/create-fself (etc.)
        if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
            set(LIBERTY_PS4_BINDIR  "${LIBERTY_PS4_TOOLCHAIN}/bin/macos")
            set(_liberty_ps4_fself_name "create-fself-macos")
            set(_liberty_ps4_exe_suffix "")
        elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
            set(LIBERTY_PS4_BINDIR  "${LIBERTY_PS4_TOOLCHAIN}/bin/linux")
            set(_liberty_ps4_fself_name "create-fself")
            set(_liberty_ps4_exe_suffix "")
        elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
            set(LIBERTY_PS4_BINDIR  "${LIBERTY_PS4_TOOLCHAIN}/bin/windows")
            set(_liberty_ps4_fself_name "create-fself")
            set(_liberty_ps4_exe_suffix ".exe")
        else()
            message(FATAL_ERROR
                "Unsupported host OS for PS4 build: ${CMAKE_HOST_SYSTEM_NAME}")
        endif()

        set(LIBERTY_PS4_CREATE_FSELF
            "${LIBERTY_PS4_BINDIR}/${_liberty_ps4_fself_name}${_liberty_ps4_exe_suffix}")

        # Configure-time sanity check — warn (don't fail) so other targets in
        # a partial toolchain checkout still configure. The actual post-build
        # command will fail at build time if the binary is genuinely needed.
        if (NOT EXISTS "${LIBERTY_PS4_CREATE_FSELF}")
            message(WARNING
                "PS4: create-fself not found at ${LIBERTY_PS4_CREATE_FSELF} — "
                "eboot.bin generation will fail at build time. "
                "Download from https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain/releases "
                "or build from tools/OpenOrbis-PS4-Toolchain/src/tools/create-fself.")
        endif()

        # find_program() auto-appends CMAKE_EXECUTABLE_SUFFIX on Windows, so
        # searching for the base name resolves create-trp.exe / create-gp4.exe /
        # PkgTool.Core.exe transparently. No extra suffix handling needed here.

        find_program(LIBERTY_PS4_CREATE_TRP   create-trp   HINTS "${LIBERTY_PS4_BINDIR}")
        find_program(LIBERTY_PS4_CREATE_GP4   create-gp4   HINTS "${LIBERTY_PS4_BINDIR}")
        find_program(LIBERTY_PS4_PKGTOOL      PkgTool.Core HINTS "${LIBERTY_PS4_BINDIR}")

        if (NOT LIBERTY_PS4_CREATE_GP4)
            message(WARNING
                "PS4: create-gp4 not found in ${LIBERTY_PS4_BINDIR} — "
                "PKG generation will be skipped.")
        endif()
        if (NOT LIBERTY_PS4_PKGTOOL)
            message(WARNING
                "PS4: PkgTool.Core not found in ${LIBERTY_PS4_BINDIR} — "
                "PKG generation will be skipped.")
        endif()
        find_package(Python3 COMPONENTS Interpreter)

        # Post-build step 1: ELF → OELF → fake-SELF eboot.bin (always runs)
        # create-fself rewrites the ELF with PS4 headers (ET_SCE_DYNEXEC,
        # PT_SCE_DYNLIBDATA, SCE import tables) then wraps it in a fake SELF.
        add_custom_command(TARGET LibertyRecomp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E env "OO_PS4_TOOLCHAIN=${LIBERTY_PS4_TOOLCHAIN}"
                "${LIBERTY_PS4_CREATE_FSELF}"
                -in "$<TARGET_FILE:LibertyRecomp>"
                -out "$<TARGET_FILE_DIR:LibertyRecomp>/LibertyRecomp.oelf"
                -eboot "$<TARGET_FILE_DIR:LibertyRecomp>/eboot.bin"
                -ptype fake
            COMMENT "PS4: Creating OELF + fake-SELF eboot.bin..."
        )

        # ── param.sfo generation (LibertyRecomp-sfo) ─────────────────────────
        # Prefer OpenOrbis's bundled make_param_sfo.py (portable, pure Python).
        # Fall back to PkgTool.Core sfo_new/sfo_setentry if the script is gone.
        set(LIBERTY_PS4_PARAM_SFO "${LIBERTY_PS4_SCE_SYS_DIR}/param.sfo")
        if (Python3_Interpreter_FOUND AND EXISTS "${LIBERTY_PS4_SFO_PY}")
            add_custom_command(
                OUTPUT  "${LIBERTY_PS4_PARAM_SFO}"
                DEPENDS "${LIBERTY_PS4_SFO_PY}" "${LIBERTY_PS4_NP_CONF_H}"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_SCE_SYS_DIR}"
                COMMAND ${Python3_EXECUTABLE} "${LIBERTY_PS4_SFO_PY}"
                    "${LIBERTY_PS4_TITLE_ID}"
                    "${LIBERTY_PS4_CONTENT_ID}"
                    "${LIBERTY_PS4_TITLE_STR}"
                    "${LIBERTY_PS4_VERSION}"
                    "${LIBERTY_PS4_PARAM_SFO}"
                COMMENT "PS4: Generating sce_sys/param.sfo (make_param_sfo.py)"
                VERBATIM
            )
        elseif (LIBERTY_PS4_PKGTOOL)
            add_custom_command(
                OUTPUT  "${LIBERTY_PS4_PARAM_SFO}"
                DEPENDS "${LIBERTY_PS4_NP_CONF_H}"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_SCE_SYS_DIR}"
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_new             "${LIBERTY_PS4_PARAM_SFO}"
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" APP_TYPE           --type Integer --maxsize 4   --value 1
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" APP_VER            --type Utf8    --maxsize 8   --value "${LIBERTY_PS4_VERSION}"
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" ATTRIBUTE          --type Integer --maxsize 4   --value 0
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" CATEGORY           --type Utf8    --maxsize 4   --value "${LIBERTY_PS4_CATEGORY}"
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" CONTENT_ID         --type Utf8    --maxsize 48  --value "${LIBERTY_PS4_CONTENT_ID}"
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" DOWNLOAD_DATA_SIZE --type Integer --maxsize 4   --value 0
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" SYSTEM_VER         --type Integer --maxsize 4   --value 0
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" TITLE              --type Utf8    --maxsize 128 --value "${LIBERTY_PS4_TITLE_STR}"
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" TITLE_ID           --type Utf8    --maxsize 12  --value "${LIBERTY_PS4_TITLE_ID}"
                COMMAND "${LIBERTY_PS4_PKGTOOL}" sfo_setentry "${LIBERTY_PS4_PARAM_SFO}" VERSION            --type Utf8    --maxsize 8   --value "${LIBERTY_PS4_VERSION}"
                COMMENT "PS4: Generating sce_sys/param.sfo (PkgTool.Core)"
                VERBATIM
            )
        else()
            message(WARNING
                "PS4: Neither make_param_sfo.py nor PkgTool.Core is available — "
                "param.sfo generation will be a placeholder. Install Python 3 "
                "or the OpenOrbis toolchain to produce a real SFO.")
            add_custom_command(
                OUTPUT  "${LIBERTY_PS4_PARAM_SFO}"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_SCE_SYS_DIR}"
                COMMAND ${CMAKE_COMMAND} -E echo "TODO: generate param.sfo TITLE_ID=${LIBERTY_PS4_TITLE_ID} CONTENT_ID=${LIBERTY_PS4_CONTENT_ID}"
                COMMAND ${CMAKE_COMMAND} -E touch "${LIBERTY_PS4_PARAM_SFO}"
                COMMENT "PS4: param.sfo placeholder (no generator found)"
            )
        endif()

        add_custom_target(LibertyRecomp-sfo
            DEPENDS "${LIBERTY_PS4_PARAM_SFO}"
        )

        # ── trophy00.trp generation (LibertyRecomp-trp) ──────────────────────
        # Prefer OpenOrbis create-trp (also produces npbind.dat + nptitle.dat).
        # Otherwise use our bundled create_trp.py which packs the TRP container
        # only; npbind/nptitle must be supplied elsewhere.
        set(LIBERTY_PS4_TROPHY_TRP "${LIBERTY_PS4_SCE_SYS_DIR}/trophy/trophy00.trp")
        file(GLOB LIBERTY_PS4_TROPHY_INPUTS
            "${LIBERTY_PS4_TROPHY_SRC}/TROP*.PNG"
            "${LIBERTY_PS4_TROPHY_SRC}/ICON0.PNG"
            "${LIBERTY_PS4_TROPHY_SRC}/TROPCONF.ESFM"
            "${LIBERTY_PS4_TROPHY_SRC}/TROP.ESFM")

        if (LIBERTY_PS4_CREATE_TRP)
            add_custom_command(
                OUTPUT  "${LIBERTY_PS4_TROPHY_TRP}"
                        "${LIBERTY_PS4_SCE_SYS_DIR}/npbind.dat"
                        "${LIBERTY_PS4_SCE_SYS_DIR}/nptitle.dat"
                DEPENDS ${LIBERTY_PS4_TROPHY_INPUTS}
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_SCE_SYS_DIR}/trophy"
                COMMAND "${LIBERTY_PS4_CREATE_TRP}"
                    -CSYS "${LIBERTY_PS4_TITLE_ID}"
                    -PTS  "${LIBERTY_PS4_TROPHY_SRC}"
                    "${LIBERTY_PS4_TROPHY_TRP}"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${LIBERTY_PS4_PKG_STAGING}/npbind.dat"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/npbind.dat"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${LIBERTY_PS4_PKG_STAGING}/nptitle.dat"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/nptitle.dat"
                WORKING_DIRECTORY "${LIBERTY_PS4_PKG_STAGING}"
                COMMENT "PS4: Packing trophy00.trp + npbind/nptitle (create-trp)"
                VERBATIM
            )
        elseif (Python3_Interpreter_FOUND AND EXISTS "${LIBERTY_PS4_TRP_PY}")
            # Python fallback: packs trophy00.trp bit-identically to OpenOrbis
            # create-trp (verified against tools/OpenOrbis-PS4-Toolchain sample).
            # Also copies the template npbind.dat / nptitle.dat from res/ps4/sce_sys
            # so the PKG step has all three trophy-related sce_sys files present.
            # NOTE: template npbind/nptitle are dev-signed stubs from the OpenOrbis
            # sample — production release needs Sony-signed replacements.
            add_custom_command(
                OUTPUT  "${LIBERTY_PS4_TROPHY_TRP}"
                        "${LIBERTY_PS4_SCE_SYS_DIR}/npbind.dat"
                        "${LIBERTY_PS4_SCE_SYS_DIR}/nptitle.dat"
                DEPENDS "${LIBERTY_PS4_TRP_PY}"
                        "${LIBERTY_PS4_NPBIND_TEMPLATE}"
                        "${LIBERTY_PS4_NPTITLE_TEMPLATE}"
                        ${LIBERTY_PS4_TROPHY_INPUTS}
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_SCE_SYS_DIR}/trophy"
                COMMAND ${Python3_EXECUTABLE} "${LIBERTY_PS4_TRP_PY}"
                    "${LIBERTY_PS4_TROPHY_SRC}"
                    "${LIBERTY_PS4_TROPHY_TRP}"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${LIBERTY_PS4_NPBIND_TEMPLATE}"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/npbind.dat"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${LIBERTY_PS4_NPTITLE_TEMPLATE}"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/nptitle.dat"
                COMMENT "PS4: Packing trophy00.trp (create_trp.py) + staging template npbind/nptitle"
                VERBATIM
            )
        else()
            message(WARNING
                "PS4: No create-trp binary or Python 3 available — trophy00.trp "
                "generation skipped. Install OpenOrbis's create-trp or Python 3.")
            add_custom_command(
                OUTPUT  "${LIBERTY_PS4_TROPHY_TRP}"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_SCE_SYS_DIR}/trophy"
                COMMAND ${CMAKE_COMMAND} -E echo "TODO: pack trophy00.trp from ${LIBERTY_PS4_TROPHY_SRC}"
                COMMAND ${CMAKE_COMMAND} -E touch "${LIBERTY_PS4_TROPHY_TRP}"
                COMMENT "PS4: trophy00.trp placeholder (no packer found)"
            )
        endif()

        add_custom_target(LibertyRecomp-trp
            DEPENDS "${LIBERTY_PS4_TROPHY_TRP}"
        )

        # ── Trophy TRP validation (LibertyRecomp-trp-validate) ────────────────
        # Round-trip sanity check: re-parses the packed trophy00.trp and
        # compares every entry payload back against the source trophy00/
        # directory, and re-computes the header SHA-1. Fails the build if
        # anything is off. Runs automatically as part of LibertyRecomp-trp
        # when a real packer produced the file (skipped for the placeholder).
        if (Python3_Interpreter_FOUND
            AND EXISTS "${LIBERTY_PS4_TRP_VALIDATE_PY}"
            AND (LIBERTY_PS4_CREATE_TRP OR EXISTS "${LIBERTY_PS4_TRP_PY}"))
            add_custom_command(TARGET LibertyRecomp-trp POST_BUILD
                COMMAND ${Python3_EXECUTABLE} "${LIBERTY_PS4_TRP_VALIDATE_PY}"
                    "${LIBERTY_PS4_TROPHY_TRP}"
                    "${LIBERTY_PS4_TROPHY_SRC}"
                COMMENT "PS4: Validating trophy00.trp round-trip"
                VERBATIM
            )
            # Also expose as a standalone target for CI / manual runs.
            add_custom_target(LibertyRecomp-trp-validate
                DEPENDS "${LIBERTY_PS4_TROPHY_TRP}"
                COMMAND ${Python3_EXECUTABLE} "${LIBERTY_PS4_TRP_VALIDATE_PY}"
                    "${LIBERTY_PS4_TROPHY_TRP}"
                    "${LIBERTY_PS4_TROPHY_SRC}"
                COMMENT "PS4: Validating trophy00.trp round-trip (standalone)"
                VERBATIM
            )
        endif()

        # ── Full PKG build target ─────────────────────────────────────────────
        # Depends on LibertyRecomp-sfo + LibertyRecomp-trp so param.sfo and
        # trophy00.trp are guaranteed present before the .gp4 / pkg_build step.
        if (LIBERTY_PS4_CREATE_GP4 AND LIBERTY_PS4_PKGTOOL)
            add_custom_target(LibertyRecomp-pkg
                DEPENDS LibertyRecomp LibertyRecomp-sfo LibertyRecomp-trp
                WORKING_DIRECTORY "${LIBERTY_PS4_PKG_STAGING}"

                # Ensure staging dirs exist
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_SCE_SYS_DIR}/trophy"
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_PKG_STAGING}/game"

                # Copy static sce_sys image assets
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/icon0.png"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/icon0.png"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/pic1.png"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/pic1.png"

                # Copy home-menu background audio. Retail PS4 XMB plays
                # sce_sys/snd0.at9 (ATRAC9) when hovering the tile. If an
                # .at9 exists alongside the .wav in res/ps4/sce_sys/, prefer
                # it; otherwise ship the PCM .wav fallback (accepted by
                # OpenOrbis-built dev PKGs, ignored by retail XMB).
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/snd0.wav"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/snd0.wav"

                # Copy eboot.bin + game payload
                COMMAND ${CMAKE_COMMAND} -E copy
                    "$<TARGET_FILE_DIR:LibertyRecomp>/eboot.bin"
                    "${LIBERTY_PS4_PKG_STAGING}/eboot.bin"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}"
                    "${LIBERTY_PS4_PKG_STAGING}/game"

                # Generate .gp4 manifest
                COMMAND "${LIBERTY_PS4_CREATE_GP4}"
                    -out "${LIBERTY_PS4_PKG_STAGING}/pkg.gp4"
                    --content-id="${LIBERTY_PS4_CONTENT_ID}"
                    --files "eboot.bin sce_sys/param.sfo sce_sys/icon0.png sce_sys/pic1.png sce_sys/snd0.wav sce_sys/npbind.dat sce_sys/nptitle.dat sce_sys/trophy/trophy00.trp"

                # Build the .pkg
                COMMAND "${LIBERTY_PS4_PKGTOOL}" pkg_build
                    "${LIBERTY_PS4_PKG_STAGING}/pkg.gp4"
                    "${LIBERTY_PS4_PKG_STAGING}"

                COMMENT "PS4: Building ${LIBERTY_PS4_CONTENT_ID}.pkg..."
            )
        else()
            message(WARNING
                "PS4 packaging tools (create-gp4, PkgTool.Core) not found in "
                "${LIBERTY_PS4_BINDIR}. Place the OpenOrbis binaries there to "
                "enable the LibertyRecomp-pkg target. The LibertyRecomp-sfo and "
                "LibertyRecomp-trp targets remain available independently.")
            # Fallback: basic staging only (no .pkg produced)
            add_custom_target(LibertyRecomp-pkg
                DEPENDS LibertyRecomp LibertyRecomp-sfo LibertyRecomp-trp
                COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBERTY_PS4_PKG_STAGING}/game"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "$<TARGET_FILE_DIR:LibertyRecomp>/eboot.bin"
                    "${LIBERTY_PS4_PKG_STAGING}/eboot.bin"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/icon0.png"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/icon0.png"
                COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/pic1.png"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/pic1.png"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_CURRENT_SOURCE_DIR}/res/ps4/sce_sys/snd0.wav"
                    "${LIBERTY_PS4_SCE_SYS_DIR}/snd0.wav"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}"
                    "${LIBERTY_PS4_PKG_STAGING}/game"
                COMMENT "PS4: Basic file staging only (.pkg tools not found)."
            )
        endif()
    endif()
endif()

if (LIBERTY_RECOMP_FLATPAK)
    target_compile_definitions(LibertyRecomp PRIVATE 
        "LIBERTY_RECOMP_FLATPAK"
        "GAME_INSTALL_DIRECTORY=\"/var/data\""
    )
endif()

# ── CURL (desktop only — consoles have no update checker / session tracker HTTP) ──
set(LIBERTY_RECOMP_HAS_CURL TRUE)
if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4"   OR CMAKE_SYSTEM_NAME STREQUAL "PS4" OR
    LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
    set(LIBERTY_RECOMP_HAS_CURL FALSE)
    message(STATUS "CURL: DISABLED — console embedded build")
else()
    find_package(CURL REQUIRED)
    message(STATUS "CURL: ENABLED")
endif()

# ── GNS availability check (set variable; definitions applied after exe target) ──
set(LIBERTY_RECOMP_HAS_GNS TRUE)
if (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4"   OR
    CMAKE_SYSTEM_NAME STREQUAL "PS4"                 OR
    LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR
    CMAKE_SYSTEM_NAME STREQUAL "Switch")
    set(LIBERTY_RECOMP_HAS_GNS FALSE)
    message(STATUS "GameNetworkingSockets: DISABLED — PS4/Switch use raw-socket P2P fallback")
else()
    message(STATUS "GameNetworkingSockets: ENABLED")
endif()

# ── Embedded-build payload validation ────────────────────────────────────────
# For platforms that bundle the game at build time (iOS / Android / PS4 / Switch),
# fail fast with a clear message if the required XEX / RPF files are missing.
if (LIBERTY_RECOMP_EMBEDDED_ASSETS)
    if (NOT LIBERTY_RECOMP_EMBEDDED_GAME_PATH)
        message(FATAL_ERROR
            "LIBERTY_RECOMP_EMBEDDED_ASSETS is ON but "
            "LIBERTY_RECOMP_EMBEDDED_GAME_PATH is not set.\n"
            "Point it at the directory containing default.xex, common.rpf, "
            "xbox360.rpf and audio.rpf.")
    endif()
    foreach(_required_file default.xex common.rpf xbox360.rpf audio.rpf)
        if (NOT EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/${_required_file}")
            message(FATAL_ERROR
                "Required game file missing from payload: "
                "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/${_required_file}\n"
                "See docs/PLATFORM_SETUP.md#build-time-asset-payload")
        endif()
    endforeach()
    message(STATUS "Embedded payload validated: ${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}")

    # Generate manifest.json so EmbeddedAssets::ReadManifest() works at runtime.
    string(TIMESTAMP _build_ts "%s" UTC)
    set(_has_tlad  0)
    set(_has_tbogt 0)
    if (EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/dlc/TLAD")
        set(_has_tlad 1)
    endif()
    if (EXISTS "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/dlc/TBOGT")
        set(_has_tbogt 1)
    endif()
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/res/manifest.json.in"
        "${LIBERTY_RECOMP_EMBEDDED_GAME_PATH}/manifest.json"
        @ONLY
    )

    # Switch: also emit the manifest at the romfs root so that
    # `romfs:/manifest.json` (the path EmbeddedAssets::ReadManifest() looks at,
    # derived from GetGameRoot().parent_path()) resolves inside the NRO.
    if ((LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch")
        AND LIBERTY_SWITCH_ROMFS_DIR)
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/res/manifest.json.in"
            "${LIBERTY_SWITCH_ROMFS_DIR}/manifest.json"
            @ONLY
        )
    endif()
endif()

# Discord Rich Presence (desktop platforms only — PS4 uses sceNpActivity)
if (NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4" OR CMAKE_SYSTEM_NAME STREQUAL "PS4" OR
         LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps5" OR CMAKE_SYSTEM_NAME STREQUAL "PS5" OR
         LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch"))
    find_package(discord-rpc CONFIG QUIET)
    if (discord-rpc_FOUND)
        set(LIBERTY_RECOMP_HAS_DISCORD_RPC TRUE)
        message(STATUS "Discord RPC: found via vcpkg")
    else()
        message(STATUS "Discord RPC: not found, presence will be disabled")
    endif()
endif()

if (LIBERTY_RECOMP_METAL)
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_METAL)
endif()

if (LIBERTY_RECOMP_D3D12)
    find_package(directx-headers CONFIG REQUIRED)
    find_package(directx12-agility CONFIG REQUIRED)
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_D3D12)

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/D3D12)
    add_custom_command(TARGET LibertyRecomp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PROPERTY:Microsoft::DirectX12-Core,IMPORTED_LOCATION_RELEASE> $<TARGET_FILE_DIR:LibertyRecomp>/D3D12
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PROPERTY:Microsoft::DirectX12-Layers,IMPORTED_LOCATION_DEBUG> $<TARGET_FILE_DIR:LibertyRecomp>/D3D12
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PROPERTY:Microsoft::DirectXShaderCompiler,IMPORTED_LOCATION> $<TARGET_FILE_DIR:LibertyRecomp>
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PROPERTY:Microsoft::DXIL,IMPORTED_LOCATION> $<TARGET_FILE_DIR:LibertyRecomp>
            COMMAND_EXPAND_LISTS
    )

    target_link_libraries(LibertyRecomp PRIVATE
            Microsoft::DirectXShaderCompiler
            Microsoft::DXIL
            dxgi
    )
endif()

if (WIN32)
    target_link_libraries(LibertyRecomp PRIVATE
        comctl32
        dwmapi
        ntdll
        Shcore
        Synchronization
        winmm
        windowsapp
    )
endif()

target_link_libraries(LibertyRecomp PRIVATE
    fmt::fmt
    libzstd_static
    msdf-atlas-gen::msdf-atlas-gen
    XenonUtils
    tomlplusplus
    xxHash::xxhash
    plume
    liberty_debugger
)

# SDL3 — desktop only (consoles use native HID/audio drivers)
if (NOT (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX))
    target_link_libraries(LibertyRecomp PRIVATE SDL3::SDL3-static)
endif()

# zlib — system on desktop, built from source on console
if (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX)
    target_link_libraries(LibertyRecomp PRIVATE zlibstatic)
endif()

# Console SDK include paths
if (LIBERTY_RECOMP_PS4)
    target_include_directories(LibertyRecomp PRIVATE
        "${CMAKE_SOURCE_DIR}/tools/OpenOrbis-PS4-Toolchain/include"
    )
elseif (LIBERTY_RECOMP_NX)
    target_include_directories(LibertyRecomp PRIVATE
        "${CMAKE_SOURCE_DIR}/tools/libnx/nx/include"
    )
endif()

# NFD (native file dialogs) — desktop only, consoles use embedded assets
if (NOT (LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "ps4"   OR CMAKE_SYSTEM_NAME STREQUAL "PS4" OR
         LIBERTY_RECOMP_TARGET_PLATFORM STREQUAL "switch" OR CMAKE_SYSTEM_NAME STREQUAL "Switch"))
    target_link_libraries(LibertyRecomp PRIVATE nfd::nfd)
else()
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_NO_NFD)
endif()

# CURL — desktop only
if (LIBERTY_RECOMP_HAS_CURL)
    target_link_libraries(LibertyRecomp PRIVATE CURL::libcurl)
else()
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_NO_CURL)
endif()
if (LIBERTY_RECOMP_HAS_GNS)
    target_link_libraries(LibertyRecomp PRIVATE GameNetworkingSockets::static)
else()
    # Tell the source code that GNS is not available on this platform
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_NO_GNS)
endif()

# Discord RPC (desktop only)
if (LIBERTY_RECOMP_HAS_DISCORD_RPC)
    target_link_libraries(LibertyRecomp PRIVATE discord-rpc)
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_DISCORD_RPC)
endif()

# Force-load LibertyRecompLib on macOS to include all weak symbols (PPC recompiled functions)
if (APPLE)
    target_link_libraries(LibertyRecomp PRIVATE -Wl,-force_load,$<TARGET_FILE:LibertyRecompLib>)
    target_link_libraries(LibertyRecomp PRIVATE "-framework CoreGraphics")
    target_link_libraries(LibertyRecomp PRIVATE "-framework GameKit")
    target_link_libraries(LibertyRecomp PRIVATE "-framework Metal")
    target_link_libraries(LibertyRecomp PRIVATE "-framework Foundation")
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_GAMECENTER _XOPEN_SOURCE=600 _DARWIN_C_SOURCE)
    if (APPLE AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
        # IOKit is macOS-only (not available on iOS)
        target_link_libraries(LibertyRecomp PRIVATE "-framework IOKit")
        # Link against Homebrew LLVM's libc++ to match the compiler (macOS host only)
        target_link_options(LibertyRecomp PRIVATE -L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++ -lc++)
        # Prefer Homebrew arm64 freetype over /usr/local x86_64 version (macOS host only)
        target_link_directories(LibertyRecomp PRIVATE /opt/homebrew/lib)
    endif()
    # iOS-specific frameworks for haptics + adaptive controller features
    if (CMAKE_SYSTEM_NAME STREQUAL "iOS")
        target_link_libraries(LibertyRecomp PRIVATE
            "-framework CoreHaptics"
            "-framework GameController"
        )
    endif()
elseif (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX)
    # Force-load on console targets too (ELF --whole-archive)
    target_link_libraries(LibertyRecomp PRIVATE -Wl,--whole-archive $<TARGET_FILE:LibertyRecompLib> -Wl,--no-whole-archive)
else()
    target_link_libraries(LibertyRecomp PRIVATE LibertyRecompLib)
endif()

# Android NDK system libraries (native InputDevice access, logging)
if (ANDROID)
    target_link_libraries(LibertyRecomp PRIVATE
        android  # native access to InputDevice etc.
        log
    )
endif()

# FFmpeg (libavcodec + libavutil) — desktop only. Used by both the SDK's
# rex::audio::XmaContext pool AND the app-layer XMAPlayback decoder in
# apu/xma_decoder.cpp. Both call avcodec_find_decoder(AV_CODEC_ID_XMAFRAMES).
#
# IMPORTANT: codec descriptors (ff_xmaframes_decoder, ff_mp3_decoder, etc.)
# are referenced only through codec_list[] in the generated codec_list.c.
# Without force-load the platform linker will dead-strip every object file
# that defines a decoder body, and avcodec_find_decoder returns null for
# every lookup at runtime → 320 "XmaContext N: Codec not found" errors.
if (NOT (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX))
    target_link_libraries(LibertyRecomp PRIVATE libavcodec libavutil)

    if(APPLE)
        target_link_options(LibertyRecomp PRIVATE
            -Wl,-force_load,$<TARGET_FILE:libavcodec>)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_link_options(LibertyRecomp PRIVATE
            -Wl,--whole-archive $<TARGET_FILE:libavcodec> -Wl,--no-whole-archive)
    elseif(MSVC)
        target_link_options(LibertyRecomp PRIVATE
            /WHOLEARCHIVE:$<TARGET_FILE:libavcodec>)
    endif()
else()
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_NO_FFMPEG)
endif()

# graine SDK runtime libraries
if(USE_REXGLUE_RUNTIME)
    target_compile_definitions(LibertyRecomp PRIVATE REX_USE_FULL_SDK)
    # Suppress rexkernels Vd*/Xam* PPC_HOOKs - Liberty owns those directly
    target_compile_definitions(LibertyRecomp PRIVATE LIBERTY_RECOMP_OVERRIDES)
    target_link_libraries(LibertyRecomp PRIVATE
        rex::core
        rex::system
        rex::kernel
        rex::audio
        rexglue_stubs
    )
endif()

# Link abseil libraries for GameNetworkingSockets (protobuf dependency)
if (APPLE)
    find_library(ABSL_LOG_INTERNAL_CHECK_OP absl_log_internal_check_op HINTS /opt/homebrew/lib)
    find_library(ABSL_LOG_INTERNAL_MESSAGE absl_log_internal_message HINTS /opt/homebrew/lib)
    find_library(ABSL_LOG_INTERNAL_NULLGUARD absl_log_internal_nullguard HINTS /opt/homebrew/lib)
    find_library(ABSL_EXAMINE_STACK absl_examine_stack HINTS /opt/homebrew/lib)
    find_library(ABSL_STRERROR absl_strerror HINTS /opt/homebrew/lib)
    find_library(ABSL_LOG_INTERNAL_GLOBALS absl_log_internal_globals HINTS /opt/homebrew/lib)
    find_library(ABSL_RAW_HASH_SET absl_raw_hash_set HINTS /opt/homebrew/lib)
    find_library(ABSL_HASHTABLEZ_SAMPLER absl_hashtablez_sampler HINTS /opt/homebrew/lib)
    find_library(ABSL_LOG_SINK absl_log_sink HINTS /opt/homebrew/lib)
    if (ABSL_LOG_INTERNAL_CHECK_OP)
        target_link_libraries(LibertyRecomp PRIVATE 
            ${ABSL_LOG_INTERNAL_CHECK_OP}
            ${ABSL_LOG_INTERNAL_MESSAGE}
            ${ABSL_LOG_INTERNAL_NULLGUARD}
            ${ABSL_EXAMINE_STACK}
            ${ABSL_STRERROR}
            ${ABSL_LOG_INTERNAL_GLOBALS}
            ${ABSL_RAW_HASH_SET}
            ${ABSL_HASHTABLEZ_SAMPLER}
            ${ABSL_LOG_SINK}
        )
    endif()
endif()

target_include_directories(LibertyRecomp PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../LibertyRecompLib"
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/api"
    ${LIBERTY_RECOMP_THIRDPARTY_INCLUDES}
    "${CMAKE_CURRENT_SOURCE_DIR}/../glue/rexglue-sdk-main/thirdparty/cli11/include"
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(PkgConfig REQUIRED)
    find_package(X11 REQUIRED)
    pkg_search_module(GLIB REQUIRED glib-2.0)
    pkg_search_module(GIO REQUIRED gio-2.0)
    target_include_directories(LibertyRecomp PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../LibertyRecompLib" ${X11_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
    target_link_libraries(LibertyRecomp PRIVATE ${X11_LIBRARIES} ${GLIB_LIBRARIES} ${GIO_LIBRARIES})
endif()

target_precompile_headers(LibertyRecomp PUBLIC ${LIBERTY_RECOMP_PRECOMPILED_HEADERS})

function(compile_shader FILE_PATH TARGET_NAME)
    set(HLSL_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/gpu/shader/hlsl/${FILE_PATH}.hlsl)
    set(MSL_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/gpu/shader/msl/${FILE_PATH}.metal)
    cmake_path(GET HLSL_FILE_PATH STEM HLSL_NAME)
    cmake_path(GET MSL_FILE_PATH STEM MSL_NAME)
    if (LIBERTY_RECOMP_METAL)
        # Newer Xcode (16+) ships the `metal` driver but the actual Metal
        # Toolchain (libraries/headers) is an optional download. If it is
        # missing, regeneration fails with: "cannot execute tool 'metal' due
        # to missing Metal Toolchain; use: xcodebuild -downloadComponent
        # MetalToolchain". The pre-built `.metallib`/`.metallib.h` are
        # checked in to the source tree, so when the toolchain is absent we
        # consume the pre-built artifacts directly. Mirrors the pre-built
        # SPIR-V handling for PS4/Switch below.
        if (NOT DEFINED LIBERTY_RECOMP_METAL_TOOLCHAIN_AVAILABLE)
            execute_process(
                COMMAND ${XCRUN_TOOL} -sdk ${LIBERTY_RECOMP_METAL_SDK} metal --version
                RESULT_VARIABLE _liberty_metal_probe_rc
                OUTPUT_QUIET
                ERROR_QUIET
            )
            if (_liberty_metal_probe_rc EQUAL 0)
                set(LIBERTY_RECOMP_METAL_TOOLCHAIN_AVAILABLE ON CACHE BOOL "" FORCE)
            else()
                set(LIBERTY_RECOMP_METAL_TOOLCHAIN_AVAILABLE OFF CACHE BOOL "" FORCE)
                message(STATUS "Metal Toolchain not installed (run 'xcodebuild -downloadComponent MetalToolchain' to enable shader regeneration); using pre-built .metallib artifacts.")
            endif()
        endif()
        if (LIBERTY_RECOMP_METAL_TOOLCHAIN_AVAILABLE)
            add_custom_command(
                    OUTPUT ${MSL_FILE_PATH}.ir
                    COMMAND ${XCRUN_TOOL} -sdk ${LIBERTY_RECOMP_METAL_SDK} metal -o ${MSL_FILE_PATH}.ir -c ${MSL_FILE_PATH} -D__air__ -frecord-sources -gline-tables-only
                    DEPENDS ${MSL_FILE_PATH}
            )
            add_custom_command(
                    OUTPUT ${MSL_FILE_PATH}.metallib
                    COMMAND ${XCRUN_TOOL} -sdk ${LIBERTY_RECOMP_METAL_SDK} metallib -o ${MSL_FILE_PATH}.metallib ${MSL_FILE_PATH}.ir
                    DEPENDS ${MSL_FILE_PATH}.ir
            )
        else()
            if (NOT EXISTS "${MSL_FILE_PATH}.metallib")
                message(FATAL_ERROR
                    "Metal Toolchain is not installed and pre-built .metallib is missing for ${MSL_FILE_PATH}.\n"
                    "Install via: xcodebuild -downloadComponent MetalToolchain\n"
                    "(or: open Xcode -> Settings -> Components -> Metal Toolchain)")
            endif()
        endif()
        BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${MSL_FILE_PATH}.metallib" DEST_FILE "${MSL_FILE_PATH}.metallib" ARRAY_NAME "g_${MSL_NAME}_air")
    endif()
    if (LIBERTY_RECOMP_D3D12)
        add_custom_command(
            OUTPUT ${HLSL_FILE_PATH}.dxil.h
            COMMAND ${DIRECTX_DXC_TOOL} -T ${TARGET_NAME} -HV 2021 -all-resources-bound -Wno-ignored-attributes -E shaderMain -Fh ${HLSL_FILE_PATH}.dxil.h ${HLSL_FILE_PATH} -Vn g_${HLSL_NAME}_dxil
            DEPENDS ${HLSL_FILE_PATH}
        )
        target_sources(LibertyRecomp PRIVATE ${HLSL_FILE_PATH}.dxil.h)
    endif()
    # SPIR-V shader compilation via DXC.
    # On console cross-builds (PS4/Switch), DXC is a host-only tool that can't be located
    # through CMAKE_FIND_ROOT_PATH. The .spirv.h files are pre-generated and checked in,
    # so we skip regeneration. On desktop builds DXC is always available.
    if(NOT LIBERTY_RECOMP_PS4 AND NOT LIBERTY_RECOMP_NX)
    add_custom_command(
        OUTPUT ${HLSL_FILE_PATH}.spirv.h
        COMMAND ${DIRECTX_DXC_TOOL} -T ${TARGET_NAME} -HV 2021 -all-resources-bound -spirv -fvk-use-dx-layout ${ARGN} -E shaderMain -Fh ${HLSL_FILE_PATH}.spirv.h ${HLSL_FILE_PATH} -Vn g_${HLSL_NAME}_spirv
        DEPENDS ${HLSL_FILE_PATH}
    )
    endif()
    target_sources(LibertyRecomp PRIVATE ${HLSL_FILE_PATH}.spirv.h)
endfunction()

function(compile_vertex_shader FILE_PATH)
    compile_shader(${FILE_PATH} vs_6_0 -fvk-invert-y -DLIBERTY_RECOMP)
endfunction()

function(compile_pixel_shader FILE_PATH)
    compile_shader(${FILE_PATH} ps_6_0 -DLIBERTY_RECOMP)
endfunction()

compile_pixel_shader(blend_color_alpha_ps)
compile_vertex_shader(copy_vs)
compile_pixel_shader(copy_color_ps)
compile_pixel_shader(copy_depth_ps)
compile_pixel_shader(csd_filter_ps)
compile_vertex_shader(csd_no_tex_vs)
compile_vertex_shader(csd_vs)
compile_vertex_shader(enhanced_burnout_blur_vs)
compile_pixel_shader(enhanced_burnout_blur_ps)
compile_pixel_shader(gaussian_blur_3x3)
compile_pixel_shader(gaussian_blur_5x5)
compile_pixel_shader(gaussian_blur_7x7)
compile_pixel_shader(gaussian_blur_9x9)
compile_pixel_shader(gamma_correction_ps)
compile_pixel_shader(hdr_tonemap_ps)
compile_pixel_shader(imgui_ps)
compile_vertex_shader(imgui_vs)
compile_pixel_shader(resolve_msaa_color_2x)
compile_pixel_shader(resolve_msaa_color_4x)
compile_pixel_shader(resolve_msaa_color_8x)
compile_pixel_shader(resolve_msaa_depth_2x)
compile_pixel_shader(resolve_msaa_depth_4x)
compile_pixel_shader(resolve_msaa_depth_8x)

# Post-process shaders for TAA, SMAA, FSR 1.0
compile_vertex_shader(fullscreen_vs)
compile_pixel_shader(taa_ps)
compile_pixel_shader(smaa_edge_detect_ps)
compile_pixel_shader(smaa_blend_ps)
compile_pixel_shader(smaa_neighborhood_blend_ps)
compile_pixel_shader(fsr1_easu_ps)
compile_pixel_shader(fsr1_rcas_ps)

function(generate_aggregate_header INPUT_DIRECTORY OUTPUT_FILE)
    get_filename_component(ABS_OUTPUT_FILE "${OUTPUT_FILE}" ABSOLUTE)
    file(GLOB_RECURSE HEADER_FILES "${INPUT_DIRECTORY}/*.h")
    set(HEADER_CONTENT "#pragma once\n\n")

    foreach(HEADER_FILE IN LISTS HEADER_FILES)
        get_filename_component(ABS_HEADER_FILE "${HEADER_FILE}" ABSOLUTE)
        if (ABS_HEADER_FILE STREQUAL ABS_OUTPUT_FILE)
            continue()
        endif()
        file(RELATIVE_PATH RELATIVE_HEADER_FILE "${INPUT_DIRECTORY}" "${HEADER_FILE}")
        string(APPEND HEADER_CONTENT "#include \"${RELATIVE_HEADER_FILE}\"\n")
    endforeach()

    if (EXISTS "${OUTPUT_FILE}")
        file(READ "${OUTPUT_FILE}" EXISTING_CONTENT)
        if (EXISTING_CONTENT STREQUAL HEADER_CONTENT)
            return()
        endif()
    endif()

    file(WRITE "${OUTPUT_FILE}" "${HEADER_CONTENT}")
endfunction()

generate_aggregate_header(
    "${CMAKE_CURRENT_SOURCE_DIR}/api"
    "${CMAKE_CURRENT_SOURCE_DIR}/api/Liberty.h"
)

set(RESOURCES_SOURCE_PATH "${PROJECT_SOURCE_DIR}/../LibertyRecompResources")
set(RESOURCES_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/res")

## Miscellaneous ##
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/bc_diff/button_bc_diff.bin" DEST_FILE "${RESOURCES_OUTPUT_PATH}/bc_diff/button_bc_diff.bin" ARRAY_NAME "g_button_bc_diff" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/font/im_font_atlas.bin" DEST_FILE "${RESOURCES_OUTPUT_PATH}/font/im_font_atlas.bin" ARRAY_NAME "g_im_font_atlas" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/font/im_font_atlas.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/font/im_font_atlas.dds" ARRAY_NAME "g_im_font_atlas_texture" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/achievements_menu/trophy.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/achievements_menu/trophy.dds" ARRAY_NAME "g_trophy" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/button_window.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/button_window.dds" ARRAY_NAME "g_button_window" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/controller.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/controller.dds" ARRAY_NAME "g_controller" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/libertyrecomp.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/libertyrecomp.dds" ARRAY_NAME "g_libertyrecomp" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/kbm.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/kbm.dds" ARRAY_NAME "g_kbm" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/main_menu1.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/main_menu1.dds" ARRAY_NAME "g_main_menu1" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/main_menu7.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/main_menu7.dds" ARRAY_NAME "g_main_menu7" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/main_menu8.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/main_menu8.dds" ARRAY_NAME "g_main_menu8" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/main_menu9.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/main_menu9.dds" ARRAY_NAME "g_main_menu9" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/arrow.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/arrow.dds" ARRAY_NAME "g_arrow" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/window.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/window.dds" ARRAY_NAME "g_window" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/common/select_arrow.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/common/select_arrow.dds" ARRAY_NAME "g_select_arrow" COMPRESSION_TYPE "zstd")

## Installer (desktop only — consoles ship with embedded game data) ##
if (NOT (LIBERTY_RECOMP_PS4 OR LIBERTY_RECOMP_NX))
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/gta4_logo.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/gta4_logo.dds" ARRAY_NAME "g_gta4_logo" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/DLC/tlad_logo.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/DLC/tlad_logo.dds" ARRAY_NAME "g_tlad_logo" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/DLC/tbogt_logo.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/DLC/tbogt_logo.dds" ARRAY_NAME "g_tbogt_logo" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_001.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_001.dds" ARRAY_NAME "g_install_001" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_002.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_002.dds" ARRAY_NAME "g_install_002" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_003.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_003.dds" ARRAY_NAME "g_install_003" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_004.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_004.dds" ARRAY_NAME "g_install_004" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_005.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_005.dds" ARRAY_NAME "g_install_005" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_006.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_006.dds" ARRAY_NAME "g_install_006" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_007.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_007.dds" ARRAY_NAME "g_install_007" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/installer/install_008.dds" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/installer/install_008.dds" ARRAY_NAME "g_install_008" COMPRESSION_TYPE "zstd")
endif()

## Game Icon ##
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/images/game_icon.bmp" DEST_FILE "${RESOURCES_OUTPUT_PATH}/images/game_icon.bmp" ARRAY_NAME "g_game_icon")

## Button Prompts (Controller-specific XTD files) ##
set(BUTTON_PROMPTS_SOURCE "${PROJECT_SOURCE_DIR}/../LibertyRecompLib/private/button_prompts")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/xbox360/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/xbox360.xtd" ARRAY_NAME "g_button_prompts_xbox360" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/xbox_one/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/xbox_one.xtd" ARRAY_NAME "g_button_prompts_xbox_one" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/xbox_series_x/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/xbox_series_x.xtd" ARRAY_NAME "g_button_prompts_xbox_series_x" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/ps3/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/ps3.xtd" ARRAY_NAME "g_button_prompts_ps3" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/ps4/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/ps4.xtd" ARRAY_NAME "g_button_prompts_ps4" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/ps5/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/ps5.xtd" ARRAY_NAME "g_button_prompts_ps5" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/switch/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/switch.xtd" ARRAY_NAME "g_button_prompts_switch" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/steam_controller/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/steam_controller.xtd" ARRAY_NAME "g_button_prompts_steam_controller" COMPRESSION_TYPE "zstd")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${BUTTON_PROMPTS_SOURCE}/steam_deck/buttons_360.xtd" DEST_FILE "${RESOURCES_OUTPUT_PATH}/button_prompts/steam_deck.xtd" ARRAY_NAME "g_button_prompts_steam_deck" COMPRESSION_TYPE "zstd")

## Audio ##
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/music/installer.ogg" DEST_FILE "${RESOURCES_OUTPUT_PATH}/music/installer.ogg" ARRAY_NAME "g_installer_music")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/sounds/window_open.ogg" DEST_FILE "${RESOURCES_OUTPUT_PATH}/sounds/window_open.ogg" ARRAY_NAME "g_window_open")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/sounds/window_close.ogg" DEST_FILE "${RESOURCES_OUTPUT_PATH}/sounds/window_close.ogg" ARRAY_NAME "g_window_close")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/sounds/cursor2.ogg" DEST_FILE "${RESOURCES_OUTPUT_PATH}/sounds/cursor2.ogg" ARRAY_NAME "g_cursor2")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/sounds/deside.ogg" DEST_FILE "${RESOURCES_OUTPUT_PATH}/sounds/deside.ogg" ARRAY_NAME "g_deside")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/sounds/move.ogg" DEST_FILE "${RESOURCES_OUTPUT_PATH}/sounds/move.ogg" ARRAY_NAME "g_move")
BIN2C(TARGET_OBJ LibertyRecomp SOURCE_FILE "${RESOURCES_SOURCE_PATH}/sounds/main_deside.ogg" DEST_FILE "${RESOURCES_OUTPUT_PATH}/sounds/main_deside.ogg" ARRAY_NAME "g_main_deside")
